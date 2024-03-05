// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillGuideEffectComponent.h"
#include "DrawDebugHelpers.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "Classes/LandscapeProxy.h"
#include "GameFramework/PlayerController.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/Engine/World.h"
#include "Classes/GameFramework/PlayerController.h"
#include "ActorEx/GsActorSkillGuideEffect.h"
#include "DataSchema/Skill/GsSchemaSkillCollision.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifySkillGuide.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UGsSkillGuideEffectComponent::UGsSkillGuideEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
#if (WITH_EDITOR)
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	bTickInEditor = true;
#endif
	// ...
}

void UGsSkillGuideEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// Ftransform 정보가 초기화 되는 ScenComponent
// 	if (nullptr == _attachScene)
// 	{
// 		_attachScene = NewObject<USceneComponent>(this, TEXT("SkillGuideEffectAttach"));
// 		_attachScene->SetupAttachment(this);
// 		_attachScene->RegisterComponent();
// 	}

	// DecalCopmponent 초기 설정 처리
	/*if (nullptr == _decalEffect._validDecal)
	{
		_decalEffect._validDecal = NewObject<UDecalComponent>(this, TEXT("SkillGuideValidDecalEffect"));
		_decalEffect._validDecal->SetupAttachment(_attachScene);
		_decalEffect._validDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		_decalEffect._validDecal->RegisterComponent();
	}

	if (nullptr == _decalEffect._invalidDecal)
	{
		_decalEffect._invalidDecal = NewObject<UDecalComponent>(this, TEXT("SkillGuideInvalidDecalEffect"));
		_decalEffect._invalidDecal->SetupAttachment(_attachScene);
		_decalEffect._invalidDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		_decalEffect._invalidDecal->RegisterComponent();
	}

	_decalEffect.Off();*/
}

FVector UGsSkillGuideEffectComponent::GetScale(const FGsSchemaSkillCollision& CollisionInfo)
{
	switch (CollisionInfo.type)
	{
	case SkillCollisionType::CIRCLE:
	case SkillCollisionType::FAN:
		return FVector(CollisionInfo.maxRadius / _scaleUnit * 2.f, CollisionInfo.maxRadius / _scaleUnit * 2.f, 1.f);
	case SkillCollisionType::RECT:
		return FVector(CollisionInfo.width / _scaleUnit, CollisionInfo.height / _scaleUnit, 1.f);
	}
	return FVector::OneVector;
}

void UGsSkillGuideEffectComponent::Set(const FGsSchemaSkillCollision& CollisionInfo, const FSoftObjectPath& EffectPath,
	float Duration, bool IsAttach)
{
	// _attachScene->ResetRelativeTransform();
	FQuat rot = GetComponentRotation().Quaternion();
	// 1. 기준 위치 변경 (offsetX, offsetY)
	// offsetX = RightVector, offsetY = ForwardVector
	// 문서 : B1_24.1.39_스킬 컬리전 영역 offset 관련 기능 추가_new
	FVector centerPos = GetComponentLocation() + rot.RotateVector(FVector(CollisionInfo.offsetY, CollisionInfo.offsetX, 0.f));

	// 2. 회전 반영(angleOffset)
	rot *= FQuat::MakeFromEuler(FVector(0.f, 0.f, CollisionInfo.angleOffset));
	// 3. 거리 반영(posOffset)
	FVector pos = rot.Vector() * CollisionInfo.posOffset + centerPos;

	FTransform tm(rot, pos);

	// 4. 최종 회전 반영 (angleOffset2)
	tm.SetRotation(rot * FQuat::MakeFromEuler(FVector(0.f, 0.f, CollisionInfo.angleOffset2)));

	// _attachScene->MoveComponent(pos, rot, true, nullptr, MOVECOMP_NoFlags, ETeleportType::None);

	FString path = EffectPath.ToString();
	path += TEXT("_C");

	FVector scale = GetScale(CollisionInfo);
	if (AGsActorSkillGuideEffect* guideActor = SpawnGuideEffect(path, tm.GetLocation(), tm.GetRotation().Rotator()
		, scale, IsAttach, FMath::Max(0.1f, Duration)))
	{
		if (CollisionInfo.type != SkillCollisionType::RECT)
		{
			float angle = (CollisionInfo.type == SkillCollisionType::CIRCLE) ? 360.f : CollisionInfo.angle;
			guideActor->SetDegree(angle);

			// Max Radius는 스케일로 키우고 MinRadius 계산
			float minScale = (CollisionInfo.minRadius / (_scaleUnit * scale.X)) * 2.f;
			guideActor->SetMinRadius(minScale);
		}
	}
}

void UGsSkillGuideEffectComponent::Set(const FVector& Location, const FGsSchemaSkillCollision& CollisionInfo, 
	const FSoftObjectPath& EffectPath, float Duration)
{
	// _attachScene->ResetRelativeTransform();
	// _attachScene->SetWorldLocation(Location);

	FString path = EffectPath.ToString();
	path += TEXT("_C");
	FVector scale = GetScale(CollisionInfo);
	if (AGsActorSkillGuideEffect* guideActor =
		SpawnGuideEffect(path, Location, FRotator::ZeroRotator, scale, false, FMath::Max(0.1f, Duration)))
	{
		if (CollisionInfo.type != SkillCollisionType::RECT)
		{
			float angle = (CollisionInfo.type == SkillCollisionType::CIRCLE) ? 360.f : CollisionInfo.angle;
			guideActor->SetDegree(angle);

			// Max Radius는 스케일로 키우고 MinRadius 계산
			float minScale = (CollisionInfo.minRadius / (_scaleUnit * scale.X)) * 2.f;
			guideActor->SetMinRadius(minScale);
		}
	}
}

AGsActorSkillGuideEffect* UGsSkillGuideEffectComponent::SpawnGuideEffect(
	const FString& path, const FVector& Location, 
	const FRotator& Rotator, const FVector& Scale, bool IsAttach, float LifeTime
)
{
	if (UClass * loadObjectC = LoadObject<UClass>(nullptr, *path))
	{
		if (UWorld* world = GetWorld())
		{
			FActorSpawnParameters spawnParam = FActorSpawnParameters();
			spawnParam.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* actor = world->SpawnActor(loadObjectC, &Location, &Rotator, spawnParam);
			if (nullptr == actor)
			{
				return nullptr;
			}
			_elapsedTime = LifeTime;
			actor->SetLifeSpan(LifeTime);
			actor->SetActorScale3D(Scale);

			if (IsAttach)
			{
				actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			}

			return Cast<AGsActorSkillGuideEffect>(actor);
		}
	}

	return nullptr;
}

AActor* UGsSkillGuideEffectComponent::OnStaticMeshEffect(
	const FGsSchemaSkillCollision& CollisionInfo, const FSoftObjectPath& GuideEffectPath
)
{
	if (nullptr == _preesActor)
	{
		if (GuideEffectPath.IsValid())
		{
			FString path = GuideEffectPath.ToString();
			path += TEXT("_C");
			_preesActor = SpawnGuideEffect(path, FVector::ZeroVector, FRotator::ZeroRotator, GetScale(CollisionInfo));
			return _preesActor;
		}
	}
	return nullptr;
}

void UGsSkillGuideEffectComponent::OnPressEffectToTarget(AActor* Target)
{
	// _attachScene->ResetRelativeTransform();
	// _lastPosition = Target->GetActorLocation();
}

void UGsSkillGuideEffectComponent::OnPressEffectToMaxDistance(float MaxDistance)
{
	// _attachScene->ResetRelativeTransform();
	// _lastPosition = _attachScene->GetForwardVector() * MaxDistance + GetComponentLocation();
}

void UGsSkillGuideEffectComponent::PressEffectRotator(const FVector& Pos, float MaxDistance)
{
	//Screen to World
	FVector worldPos;
	FVector worldDir;
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->DeprojectScreenPositionToWorld(Pos.X, Pos.Y, worldPos, worldDir);
	
	FVector endPos = worldDir * _rayCastMaxDistance + worldPos;

	// Raytrace for overlapping actors.
	TArray<FHitResult> HitResult;
	GetWorld()->LineTraceMultiByObjectType(
		OUT HitResult,
		worldPos,
		endPos,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		FCollisionQueryParams()
	);

	if (HitResult.Num() > 0)
	{
		FHitResult* pick = HitResult.FindByPredicate([](const FHitResult& el)
		{
			return el.Actor->IsA<ALandscapeProxy>();
		});
		_lastPosition = (pick) ? pick->Location : HitResult[0].Location;
		// _attachScene->SetWorldLocation(_lastPosition);

		//// 최대 사거리 체크
		//FVector distance = GetComponentTransform().GetLocation() - _lastPosition;
		//if (distance.SizeSquared() >= MaxDistance * MaxDistance)
		//{
		//	_decalEffect.Invalid();
		//}
		//else
		//{
		//	_decalEffect.Valid();
		//}
	}
	/*else
	{
		_decalEffect.Invalid();
	}*/

	// regacy
	//_joyAxis = Dir;

	//// 플레이어 컨트롤러 각도 얻기
	////AController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//FVector camDir = controller->GetControlRotation().Vector().GetSafeNormal2D();
	//FRotator camRot = camDir.Rotation();

	//// 회전값 반영
	//FRotator addRot = camRot + Dir.Rotation();

	//// Offset
	//float distance = MaxDistance * Dir.Size();

	//FVector pos = _attachScene->GetComponentRotation().Vector() * distance;
	//_attachScene->ResetRelativeTransform();
	//_attachScene->MoveComponent(pos, addRot, true, nullptr, MOVECOMP_NoFlags, ETeleportType::None);

	//_lastPosition = _attachScene->GetComponentLocation();
}

void UGsSkillGuideEffectComponent::ClearPressEffect()
{
	// _attachScene->ResetRelativeTransform();

	if (UWorld* world = GetWorld())
	{
		if (_preesActor)
		{
			// Destory
			world->DestroyActor(_preesActor);
		}

		// _decalEffect.Off();

		_preesActor = nullptr;
		_preesDecal = nullptr;
		_joyAxis = FVector::ZeroVector;
		//LastPosition = AttachScene->GetComponentLocation();
	}
}

// 현재 좌표에 해당하는 지형 높이 얻기
float UGsSkillGuideEffectComponent::GetSurface(const FVector2D& Point)
{
	if (UWorld* World = GetOwner()->GetWorld())
	{
		// 천장이 있는 형태의 맵이 있기때문에 현재 좌표에서 약 5m정도만 올림
		FVector StartLocation{ Point.X, Point.Y, GetOwner()->GetActorLocation().Z + 500.f };
		FVector EndLocation{ Point.X, Point.Y, -10000 };

		// Raytrace for overlapping actors.
		FHitResult HitResult;
		World->LineTraceSingleByObjectType(
			OUT HitResult,
			StartLocation,
			EndLocation,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
			FCollisionQueryParams()
		);
		
		// Return Z location.
		if (HitResult.GetActor()) return HitResult.ImpactPoint.Z;
	}

	return 0;
}

#if WITH_EDITOR
// 테스트용
void UGsSkillGuideEffectComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	if (GetWorld() && !GetWorld()->IsGameWorld())
	{
		if (_debugPath.IsValid())
		{
			FTransform tm = GetComponentTransform();

			FString path = _debugPath.ToString();
			path += TEXT("_C");

			if (_debugPath != _preDebugPath)
			{
				if (_debugSpawnActor)
				{
					GetWorld()->DestroyActor(_debugSpawnActor);
				}

				_preDebugPath = _debugPath;
				if (UClass * loadObjectC = LoadObject<UClass>(nullptr, *path))
				{
					_debugSpawnActor = Spawn(loadObjectC, tm);
				}
			}

			if (_debugSpawnActor)
			{
				if (_debugWidth != 0.f)
				{
					_debugSpawnActor->SetActorScale3D(FVector(_debugWidth / _scaleUnit, _debugHegiht / _scaleUnit, 1.f));
					_debugRadius = 0.f;
				}

				if (_debugRadius != 0.f)
				{
					_debugSpawnActor->SetActorScale3D(FVector(_debugRadius / _scaleUnit, _debugRadius / _scaleUnit, 1.f));
					if (AGsActorSkillGuideEffect* guideActor = Cast<AGsActorSkillGuideEffect>(_debugSpawnActor))
					{
						guideActor->SetDegree(_debugRadius);
					}
					
					_debugWidth = 0.f;
					_debugHegiht = 0.f;
				}
			}
		}
	}
}

AActor* UGsSkillGuideEffectComponent::Spawn(
	TSubclassOf<AActor> Actor, const FTransform& Transform
)
{
	if (UWorld* world = GetWorld())
	{
		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		return world->SpawnActor(Actor, &Transform, spawnParam);
	}

	return nullptr;
}
#endif