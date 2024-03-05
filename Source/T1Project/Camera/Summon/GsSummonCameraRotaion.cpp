#include "GsSummonCameraRotaion.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "ActorEx/GsActorSummonSceneObject.h"

#include "Engine/Classes/Curves/CurveFloat.h"
#include "Engine/Classes/Camera/CameraComponent.h"

void FGsSummonCameraRotation::Update(float In_deltaTime)
{
	if (_isFinish == true)
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
	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressRate), 0.f, 1.f);
	}

	FRotator newRot = FMath::Lerp(_rotFrom, _rotTo, curveVal);

	bool isLastSet = false;
	// 목적지 값이랑 같은가
	if (false == newRot.Equals(_rotTo, 0.1f))
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

void FGsSummonCameraRotation::OnFinishLerp()
{
	_isFinish = true;

	if (_callbackFinishLerp == nullptr)
	{
		return;
	}

	_callbackFinishLerp(EGsCameraWorkType::RotationWork);
}
void FGsSummonCameraRotation::SetRotation(const FRotator& In_rot)
{
	if (UCameraComponent* camComponent = _owner->GetCameraComponent())
	{
		// 월드
		camComponent->SetWorldRotation(In_rot);
	}
}

void FGsSummonCameraRotation::SetTargetRot(const FRotator& In_rotTo, const FRotator& In_rotFrom)
{
	_rotTo = In_rotTo;
	_rotFrom = In_rotFrom;
	_rotCurrent = _rotFrom;

	// 회전 하기위해서 초기화
	_isFinish = false;
	// 진행도 초기화
	_progressRate = 0.0f;

}

// 거리에 의한 속도 만들기
float FGsSummonCameraRotation::MakeDistSpeed()
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

	float speed =
		GData()->GetGlobalData()->_summonCameraHighGradeSpeed;
	// 거리가 멀수록 느려짐(rate 로 진행되므로 일정한 속도 만든다)
	float nowSpeed = speed / dist;
	return nowSpeed;
}
