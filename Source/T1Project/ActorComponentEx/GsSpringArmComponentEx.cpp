#include "GsSpringArmComponentEx.h"
#include "GameFramework/Pawn.h"
#include "CollisionQueryParams.h"
#include "WorldCollision.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

void UGsSpringArmComponentEx::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_callbackTickComponent != nullptr)
	{
		_callbackTickComponent();
	}
}

void UGsSpringArmComponentEx::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
	FRotator DesiredRot = GetTargetRotation();

	// Apply 'lag' to rotation if desired
	if (bDoRotationLag)
	{
		if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraRotationLagSpeed > 0.f)
		{
			const FRotator ArmRotStep = (DesiredRot - PreviousDesiredRot).GetNormalized() * (1.f / DeltaTime);
			FRotator LerpTarget = PreviousDesiredRot;
			float RemainingTime = DeltaTime;
			while (RemainingTime > KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += ArmRotStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredRot = FRotator(FMath::QInterpTo(FQuat(PreviousDesiredRot), FQuat(LerpTarget), LerpAmount, CameraRotationLagSpeed));
				PreviousDesiredRot = DesiredRot;
			}
		}
		else
		{
			DesiredRot = FRotator(FMath::QInterpTo(FQuat(PreviousDesiredRot), FQuat(DesiredRot), DeltaTime, CameraRotationLagSpeed));
		}
	}
	PreviousDesiredRot = DesiredRot;

	// Get the spring arm 'origin', the target we want to look at
	FVector ArmOrigin = GetComponentLocation() + TargetOffset;
	// We lag the target, not the actual camera position, so rotating the camera around does not have lag
	FVector DesiredLoc = ArmOrigin;
	if (bDoLocationLag)
	{
		if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraLagSpeed > 0.f)
		{
			const FVector ArmMovementStep = (DesiredLoc - PreviousDesiredLoc) * (1.f / DeltaTime);
			FVector LerpTarget = PreviousDesiredLoc;

			float RemainingTime = DeltaTime;
			while (RemainingTime > KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += ArmMovementStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, LerpTarget, LerpAmount, CameraLagSpeed);
				PreviousDesiredLoc = DesiredLoc;
			}
		}
		else
		{
			DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, DesiredLoc, DeltaTime, CameraLagSpeed);
		}

		// Clamp distance if requested
		bool bClampedDist = false;
		if (CameraLagMaxDistance > 0.f)
		{
			const FVector FromOrigin = DesiredLoc - ArmOrigin;
			if (FromOrigin.SizeSquared() > FMath::Square(CameraLagMaxDistance))
			{
				DesiredLoc = ArmOrigin + FromOrigin.GetClampedToMaxSize(CameraLagMaxDistance);
				bClampedDist = true;
			}
		}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (bDrawDebugLagMarkers)
		{
			DrawDebugSphere(GetWorld(), ArmOrigin, 5.f, 8, FColor::Green);
			DrawDebugSphere(GetWorld(), DesiredLoc, 5.f, 8, FColor::Yellow);

			const FVector ToOrigin = ArmOrigin - DesiredLoc;
			DrawDebugDirectionalArrow(GetWorld(), DesiredLoc, DesiredLoc + ToOrigin * 0.5f, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
			DrawDebugDirectionalArrow(GetWorld(), DesiredLoc + ToOrigin * 0.5f, ArmOrigin, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
		}
#endif
	}

	PreviousArmOrigin = ArmOrigin;
	PreviousDesiredLoc = DesiredLoc;

	// Now offset camera position back along our rotation
	DesiredLoc -= DesiredRot.Vector() * TargetArmLength;
	// Add socket offset in local space
	DesiredLoc += FRotationMatrix(DesiredRot).TransformVector(SocketOffset);

	// Do a sweep to ensure we are not penetrating the world
	FVector ResultLoc;

	if (bDoTrace && (TargetArmLength != 0.0f))
	{
		bIsCameraFixed = true;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetOwner());

		// 물 오브젝트 때문에 몸안에 카메라가 들어가는 이슈로
		// 카메라 시작점을 캡슐 크기(34-> bp 세팅값(좀더 크게)) 밖에서 시작 해보자

		FVector startPos = ArmOrigin - DesiredRot.Vector() * _minimalCamDist;

		FHitResult Result;
		GetWorld()->SweepSingleByChannel(Result, startPos, DesiredLoc, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);

		if (UPrimitiveComponent* hitComponent = Result.Component.Get())
		{
			if (hitComponent->GetCollisionProfileName() == _waterPreset && Result.Actor.IsValid())
			{	// 2024/3/4 PKT - 수면과 충돌 했을 경우.
				/*FString message = FString::Format(TEXT("{0} / {1}"), { Result.Location.Z, DesiredLoc.Z });
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, message);*/

				// 2024/3/4 PKT - Cam이 수면 아래로 들어가면 안되기 때문에 Cam이 도달하려는 위치가 수면 위일때만 수면을 무시해준다.
				if (Result.Location.Z < DesiredLoc.Z)
				{	// 2024/3/4 PKT - 해당 수면을 무시하고 다시 한번 체크
					QueryParams.AddIgnoredActor(Result.Actor.Get());
					GetWorld()->SweepSingleByChannel(Result, startPos, DesiredLoc, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);
				}
			}
		}

		//// | Warning | 매 틱마다 불릴 수 있는 녀석이라 아래와 같이 처리 시, 성능 저하가 우려됨
		//// 만약 Hit 되었을 때의 Normal 이 수직이면서, 해당 오브젝트가 물 객체일 경우, DesiredLoc 에 오프셋 부여
		//bool isHitWater = false;
		//if (_impactNormalCheckValue < Result.ImpactNormal.Z)
		//{
		//	if (UPrimitiveComponent* hitComponent = Result.Component.Get())
		//	{
		//		if (hitComponent->GetCollisionProfileName() == _waterPreset)
		//		{
		//			isHitWater = true;
		//		}
		//	}
		//}

		UnfixedCameraPosition = DesiredLoc;

		//if (isHitWater)
		//{
		//	if (Result.Distance <= _targetArmOffset)
		//	{
		//		FVector offsetVector = DesiredLoc - Result.Location;
		//		offsetVector.Normalize();


		//		ResultLoc = BlendLocations(DesiredLoc, Result.Location + offsetVector * _targetArmOffset, Result.bBlockingHit, DeltaTime);

		//		bIsCameraFixed = false;
		//	}
		//}
		//else
		//{
		//	ResultLoc = BlendLocations(DesiredLoc, Result.Location, Result.bBlockingHit, DeltaTime);

		//	if (ResultLoc == DesiredLoc)
		//	{
		//		bIsCameraFixed = false;
		//	}
		//}

		ResultLoc = BlendLocations(DesiredLoc, Result.Location, Result.bBlockingHit, DeltaTime);

		if (ResultLoc == DesiredLoc)
		{
			bIsCameraFixed = false;
		}

	}
	else
	{
		ResultLoc = DesiredLoc;
		bIsCameraFixed = false;
		UnfixedCameraPosition = ResultLoc;
	}

	// Form a transform for new world transform for camera
	FTransform WorldCamTM(DesiredRot, ResultLoc);

	// Convert to relative to component
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(GetComponentTransform());

	// Update socket location/rotation
	RelativeSocketLocation = RelCamTM.GetLocation();
	RelativeSocketRotation = RelCamTM.GetRotation();
	
	UpdateChildTransforms();
}


