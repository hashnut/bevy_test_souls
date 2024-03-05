#include "GsCameraLookAtHandlerBase.h"

#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"

#include "ActorEx/GsCharacterPlayer.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Management/GsMessageHolder.h"

#include "T1Project.h"

//#define TEST_DRAW_LINE


void FGsCameraLookAtHandlerBase::Finalize()
{
	// 크래쉬 이슈때문에 일단 null 처리
	_char = nullptr;
	_playerController = nullptr;

	GMessage()->GetCamera().Remove(_delegateDebugDrawLookAxisLine);
}


USkeletalMeshComponent* FGsCameraLookAtHandlerBase::GetMesh()
{
	if (_char == nullptr)
	{
		return nullptr;
	}
	return _char->GetMesh();
}

USkeletalMeshComponent* FGsCameraLookAtHandlerBase::GetMeshTargetHead()
{
	if (_char == nullptr)
	{
		return nullptr;
	}
	return _char->GetPartsSkeletalComponentBODY();
}

// 초기화(처음 한번만)
void FGsCameraLookAtHandlerBase::InitializeActor(AActor* In_char)
{
	_char = Cast<AGsCharacterPlayer>(In_char);
	_playerController = Cast<APlayerController>(_char->Controller);
	_headBoneName = FName("Bip001-Head");
	_baseBoneName = FName("Bip001-Neck");
	
	_delegateDebugDrawLookAxisLine =
		GMessage()->GetCamera().AddRaw(MessageContentCamera::DEBUG_DRAW_LOOK_AXIS_LINE,
			this, &FGsCameraLookAtHandlerBase::OnDebugDrawLookAxisLine);
	
}

// 갱신
void FGsCameraLookAtHandlerBase::Update(float In_delta)
{
	CheckLookAt();
	UpdateLookAt(In_delta);
}


// 바라보기 끝
void FGsCameraLookAtHandlerBase::ReachLookAtTarget()
{
	_isLookAtRotChanging = false;

	// 보간 회전 플래그 끈다
	_isLerpRotation = false;
	if (_isBackToNormal == true)
	{		
		_isBackToNormal = false;
		// 원상복구되면 원래 애니 회전과 같다
		_isDiffAniRot = false;
		// 메쉬에 썻던값 초기화
		USkeletalMeshComponent* meshPtr = GetMesh();
		if (meshPtr != nullptr)
		{
#ifdef NOT_USE_LOOK_AT
			meshPtr->BoneRotationClear(_headBoneName);
#endif
		}
		// 본 회전값 초기화
		_isBoneRotChanged = false;
	}
}
// 바라보기 갱신
void FGsCameraLookAtHandlerBase::UpdateLookAt(float In_delta)
{

	if (false == _isLookAtRotChanging)
	{
		return;
	}
	
	float addRate = In_delta * GData()->GetGlobalData()->LookAtRotSpeed;
	_progressLookAtRate = FMath::Clamp<float>(_progressLookAtRate + addRate, 0.f, 1.f);
	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressLookAtRate), 0.f, 1.f);
	}

	FQuat curveNewQuat = FMath::Lerp(_lookAtQuatFrom, _lookAtQuatTo, curveVal);

	// 일단 세팅하고
	BoneRotation(curveNewQuat);
	if (false == curveNewQuat.Equals(_lookAtQuatTo, 0.01f))
	{
	}
	// 끝에 도달
	else
	{		
		ReachLookAtTarget();
	}
}

// 본 회전
void FGsCameraLookAtHandlerBase::BoneRotation(const FQuat& In_quat)
{
	USkeletalMeshComponent* meshPtr = GetMesh();
	if (meshPtr != nullptr)
	{
#ifdef NOT_USE_LOOK_AT
		meshPtr->SetBoneRotation(_headBoneName, In_quat);
#endif
	}

	SetBoneRotation(In_quat);

	// 본 회전값 수정
	_isBoneRotChanged = true;
}

// 바라보기 체크 하기(min, max 값 안에 있으면 바라보기, 아니면 원래 회전으로 돌아가기)
void FGsCameraLookAtHandlerBase::CheckLookAt()
{
	// 모드가 아니면 할필요 없음
	if (_isLookAtMode == false)
	{
		return;
	}

	// 바라볼 회전값
	FRotator lookAtRot = FRotator::ZeroRotator;
	// 아직 준비 전이면 리턴
	if (GetLookAtRot(lookAtRot) == false)
	{
		return;
	}

	FRotator ctrlRot = _char->GetControlRotation();
	AActor* targetActor = _playerController->GetViewTarget();

	// 처음 받아놓고 하면 월드 값이라서
	// 계산할때마다 새로 받아야함



	USkeletalMeshComponent* meshPtr = GetMesh();
	if (meshPtr != nullptr )
	{
		_baseBoneY =
			meshPtr->GetBoneAxis(_baseBoneName, EAxis::Y);

		FVector newXAxis = _baseBoneY * -1.0f;

		// 액터 회전이 있었는가?
		FQuat currActorQuat = _char->GetActorQuat();

		if (_baseBoneRecalcX.Equals(FVector::ZeroVector) == true ||
			currActorQuat.Equals(_oldActorQuat) == false ||
			newXAxis.Equals(_baseBoneRecalcX, 0.1f) == false)
		{
			// 저장
			_oldActorQuat = currActorQuat;

			_baseBoneX =
				meshPtr->GetBoneAxis(_baseBoneName, EAxis::X);
			
			_baseBoneZ =
				meshPtr->GetBoneAxis(_baseBoneName, EAxis::Z);

			// 목은 x축이 y축 역방향이다
			_baseBoneRecalcX = newXAxis;
		}
	}

	// 액터 기준이면 캐릭터가 비스듬하게 있어서 기준점이 틀어진다
	// 본중에 최상위(root 바로 밑) 의 x축을 forward로 삼자
	// neck을 쓰되 y축 반대 방향을 x축으로 보고 처리하자(AD님과 논의)
	//FVector backwardVector = _baseBoneX * -1;
	FVector backwardVector = _baseBoneRecalcX * -1;
	FVector upVec = _char->GetActorUpVector();
	FRotator actorBackRot = UKismetMathLibrary::MakeRotFromXZ(backwardVector, upVec);


	// 액터 회전 역으로 곱해서 로컬값을 얻는다...
	FQuat localCtrlQuat = ctrlRot.Quaternion() * actorBackRot.Quaternion().Inverse();
	FRotator localCtrlRot = localCtrlQuat.Rotator();

	if (_isDrawDebugLine == true)
	{
		FVector dir = targetActor->GetActorForwardVector();
		//FVector actorPos = targetActor->GetActorLocation();
		//actorPos.Z += 60.0f;

		FVector actorPos = FVector::ZeroVector;
		if (meshPtr != nullptr)
		{
			actorPos = meshPtr->GetBoneLocation(_headBoneName, EBoneSpaces::WorldSpace);
		}

		float length = 100.0f;

		FVector hypotenuse1 = actorPos + dir.RotateAngleAxis(GData()->GetGlobalData()->LookAtYawMax, FVector::UpVector) * length;
		FVector hypotenuse2 = actorPos + dir.RotateAngleAxis(GData()->GetGlobalData()->LookAtYawMin, FVector::UpVector) * length;

		FVector fowardPos = actorPos + dir * length;

		DrawDebugLine(targetActor->GetWorld(), actorPos, hypotenuse1, FColor::Green, false, -1.0f);
		DrawDebugLine(targetActor->GetWorld(), actorPos, hypotenuse2, FColor::Green, false, -1.0f);
		DrawDebugLine(targetActor->GetWorld(), actorPos, fowardPos, FColor::Green, false, -1.0f);

		// 모가지는 y 역방향이 x축 처럼 쓰자..
		FVector baseXPos = actorPos + _baseBoneRecalcX * length;
		//FVector baseYPos = actorPos + _baseBoneY * length;
		//FVector baseZPos = actorPos + _baseBoneZ * length;

		DrawDebugLine(targetActor->GetWorld(), actorPos, baseXPos, FColor::Red, false, -1.0f);
		//DrawDebugLine(targetActor->GetWorld(), actorPos, baseYPos, FColor::Magenta, false, -1.0f);
		//DrawDebugLine(targetActor->GetWorld(), actorPos, baseZPos, FColor::Orange, false, -1.0f);

		FVector baseXhypotenuse1 = actorPos + _baseBoneRecalcX.RotateAngleAxis(GData()->GetGlobalData()->LookAtYawMax, FVector::UpVector) * length;
		FVector baseXhypotenuse2 = actorPos + _baseBoneRecalcX.RotateAngleAxis(GData()->GetGlobalData()->LookAtYawMin, FVector::UpVector) * length;

		DrawDebugLine(targetActor->GetWorld(), actorPos, baseXhypotenuse1, FColor::Red, false, -1.0f);
		DrawDebugLine(targetActor->GetWorld(), actorPos, baseXhypotenuse2, FColor::Red, false, -1.0f);
	}


	// 제한값보다 크거나 작으면	
	if (localCtrlRot.Yaw > GData()->GetGlobalData()->LookAtYawMax  ||
		localCtrlRot.Yaw < GData()->GetGlobalData()->LookAtYawMin)
	{
		// 원래 애니랑 다른 회전이라면
		if (true == _isDiffAniRot )
		{
			EndLookAt();
		}
		return;
	}

	// 바라보기 시작
	StartLookAt(lookAtRot.Quaternion());
	// 요건 바라보기이므로 초기화
	_isBackToNormal = false;
	// 바라보기 하면 애니랑 다른 회전이다
	_isDiffAniRot = true;
}

// 바라보기 시작
void FGsCameraLookAtHandlerBase::StartLookAt(FQuat In_targetQuat)
{
	// 보간 처리 회전인지
	if (_isLerpRotation == true)
	{
		// 이미 변환중이면서 
		// 이전 목표랑 같으면 리턴
		if (_isLookAtRotChanging == true &&
			_lookAtQuatTo.Equals(In_targetQuat) == true)
		{
			return;
		}

		FQuat boneQuat = FQuat::Identity;
		USkeletalMeshComponent* meshPtr = GetMesh();
		if (meshPtr != nullptr)
		{
			boneQuat =
				meshPtr->GetBoneQuaternion(_headBoneName, EBoneSpaces::ComponentSpace);
		}
		// 현재꺼 저장
		_lookAtQuatFrom = boneQuat;
		_lookAtQuatTo = In_targetQuat;


		_isLookAtRotChanging = true;
		_progressLookAtRate = 0.0f;
	}
	// 기본 세팅 회전(바로 처리)
	else
	{
		FQuat boneQuat = FQuat::Identity;
		USkeletalMeshComponent* meshPtr = GetMesh();
		if (meshPtr != nullptr)
		{
			boneQuat =
				meshPtr->GetBoneQuaternion(_headBoneName, EBoneSpaces::ComponentSpace);
		}

		// 기존꺼랑 다르면
		if (boneQuat.Equals(In_targetQuat, 0.08f) == false)
		{
			// 바로 세팅
			BoneRotation(In_targetQuat);
		}

		_isLookAtRotChanging = false;

	}
}

// 바라보기 끝
void FGsCameraLookAtHandlerBase::EndLookAt()
{
	// 회전한적이 없으므로 할필요 없음
	// 원복중이면 할필요 없음
	if (false == _isBoneRotChanged||
		_isBackToNormal == true)
	{
		return;
	}

	// 원복 시작
	_isBackToNormal = true;

	FQuat aniRot = FQuat::Identity;
	USkeletalMeshComponent* meshPtr = GetMesh();
	if (meshPtr != nullptr)
	{
#ifdef NOT_USE_LOOK_AT
		aniRot = meshPtr->FindBoneAniRotation(_headBoneName);
#else
		aniRot = meshPtr->GetBoneQuaternion(_headBoneName);
#endif
	}
	StartLookAt(aniRot);	
}

// 바라보기 rot 가져오기
bool FGsCameraLookAtHandlerBase::GetLookAtRot(FRotator& Out_rot)
{
	USkeletalMeshComponent* meshPtr = GetMesh();
	USkeletalMeshComponent* meshTargetHead = GetMeshTargetHead();
	if (nullptr == meshPtr ||
		nullptr == _playerController ||
		nullptr == _char||
		nullptr == meshTargetHead)
	{
		return false;
	}

	FVector loc = FVector::ZeroVector;
	FRotator rot = FRotator::ZeroRotator;

	_playerController->GetPlayerViewPoint(loc, rot);

	// 아직 카메라 세팅 전
	if (loc.Equals(FVector::ZeroVector))
	{
		return false;
	}

	FVector bonePos =
		meshPtr->GetBoneLocation(_headBoneName, EBoneSpaces::WorldSpace);
	FVector foward = loc - bonePos;

	FVector upVec = _char->GetActorUpVector();
	FRotator lookRot = UKismetMathLibrary::MakeRotFromXZ(foward, upVec);

	FQuat relativeQuat = meshTargetHead->GetRelativeRotationFromWorld(lookRot.Quaternion());
	FRotator relativeRot = relativeQuat.Rotator();
	FRotator fixRelativeRot = FRotator(90 - relativeRot.Pitch, relativeRot.Yaw - 90, relativeRot.Roll + 90);
	Out_rot = fixRelativeRot;

	return true;
}

// 룩앳모드 변경
void FGsCameraLookAtHandlerBase::ChangeLookAtMode(bool In_mode)
{
	// 보간을 위한 회전
	_isLerpRotation = true;
	if (In_mode == true)
	{
		_isLookAtMode = true;
	}
	else
	{
		_isLookAtMode = false;	
		EndLookAt();	
	}	
}

// 처음 모드 세팅할때
void FGsCameraLookAtHandlerBase::InitMode(bool In_mode)
{
	_isLookAtMode = In_mode;
	
	_isLookAtRotChanging = false;
	_isBackToNormal = false;
	// 원상복구되면 원래 애니 회전과 같다
	_isDiffAniRot = false;
	// 메쉬에 썻던값 초기화
	USkeletalMeshComponent* meshPtr = GetMesh();
	if (meshPtr != nullptr)
	{
#ifdef NOT_USE_LOOK_AT
		meshPtr->BoneRotationClear(_headBoneName);
#endif
	}
	// 본 회전값 초기화
	_isBoneRotChanged = false;
}

// 디버그 라인 그릴지
void FGsCameraLookAtHandlerBase::OnDebugDrawLookAxisLine()
{
	_isDrawDebugLine = !_isDrawDebugLine;
}

void FGsCameraLookAtHandlerBase::SetBoneRotation(const FQuat& In_quat)
{

}