#include "GsCameraRotationBase.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "ControllerEx/GsPlayerController.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Engine/Classes/Camera/PlayerCameraManager.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Curves/CurveFloat.h"

#include "GameFramework/PlayerController.h"


void FGsCameraRotationBase::Finalize()
{
	RemoveLocalPlayer();
	_owner = nullptr;
}


// 회전속도 구하기
float FGsCameraRotationBase::GetRotSpeed()
{
	float rotationSpeed = GData()->GetGlobalData()->CamModeChangeRotLerpSpeed;
	return rotationSpeed;
}
// 모드 회전 갱신
void FGsCameraRotationBase::Update(float In_deltaTime)
{
	// 회전
	if (IsRotationPossible() == false)
	{
		return;
	}
	
	float rotationSpeed = _nowSpeed;

	// 속도 0이면 안함
	if (FMath::IsNearlyZero(rotationSpeed))
	{
		OnFinishLerp();
		return;
	}

	float addRate = In_deltaTime * rotationSpeed;
	_progressRate = FMath::Clamp<float>(_progressRate + addRate, 0.f, 1.f);
	UCurveFloat* curve = GetCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressRate), 0.f, 1.f);
	}

	FRotator newRot = FMath::Lerp(_rotFrom, _rotTo, curveVal);

	bool isLastSet = false;
	// 목적지 값이랑 같은가
	if (false == newRot.Equals(_rotTo, 0.01f))
	{
	}
	else
	{
		newRot = _rotTo;			
		isLastSet = true;
	}
	_rotCurrent = newRot;
	SetRotation(newRot);

	if (isLastSet == true)
	{
		// 마지막 데이터를 세팅하고 호출하자
		OnFinishLerp();
	}
		
	
}

void FGsCameraRotationBase::OnFinishLerp()
{ 
	_isFinish = true; 

/*	FRotator currCamRotation = FRotator::ZeroRotator;

	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	if (AGsPlayerController* controller = localChar->GetPlayerController())
	{
		APlayerCameraManager* camManager = controller->PlayerCameraManager;
		if (camManager != nullptr)
		{
			currCamRotation = camManager->GetCameraRotation();
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("Rotation Finish OnFinishLerp --  currCamRotation[%s] "),
		*currCamRotation.ToString());
#endif*/
}

void FGsCameraRotationBase::SetTargetRot(const FRotator& In_rotTo, const FRotator& In_rotFrom, bool In_isNowMove)
{
	_rotTo = In_rotTo;
	_rotFrom = In_rotFrom;
	_rotCurrent = _rotFrom;

	// 회전 하기위해서 초기화
	// is direct
	_isFinish = !In_isNowMove;
	// 진행도 초기화
	_progressRate = 0.0f;

	// 속도 만들기
	MakeSpeed();
}
// 속도 만들기(연산을 미리 해놓음)
void FGsCameraRotationBase::MakeSpeed()
{
	_nowSpeed = GetRotSpeed();
}

// 거리에 의한 속도 만들기
float FGsCameraRotationBase::MakeDistSpeed()
{	
	FQuat quatTo = _rotTo.Quaternion();
	FQuat quatFrom = _rotFrom.Quaternion();

	float dist =
		quatTo.AngularDistance(quatFrom);

	// 분자가 0이면 1로 바꿈
	// 1보다 작아도 1로 세팅
	if (dist == 0 || dist < 1.0f)
	{
		dist = 1.0f;
	}


	// 거리가 멀수록 느려짐(rate 로 진행되므로 일정한 속도 만든다)
	float nowSpeed = GetRotSpeed() / dist;
	return nowSpeed;
}

// 현재 사용 커브 접근
UCurveFloat* FGsCameraRotationBase::GetCurve()
{
	UCurveFloat* curve = GData()->GetCamFollowCurve();
	return curve;
}

void FGsCameraRotationBase::RemoveLocalPlayer()
{
	_local = nullptr;
	_targetObj = nullptr;
}