#include "GsCameraRotationDialogCutLoop.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Curves/CurveFloat.h"

void FGsCameraRotationDialogCutLoop::Initialize()
{
	_nowSpeed = GData()->GetGlobalData()->_dialogCutLoopCameraMoveSpeed;

	_dialogCutLoopOffset = GData()->GetGlobalData()->_dialogCutLoopCameraMoveOffset;
}
void FGsCameraRotationDialogCutLoop::Finalize()
{
	RemoveLocalPlayer();
}
void FGsCameraRotationDialogCutLoop::Update(float In_deltaTime)
{
	if (_isRotaionLoopStart == false)
	{
		return;
	}

	if (FMath::IsNearlyZero(_nowSpeed) == true)
	{
		_isRotaionLoopStart = false;
		return;
	}

	float addRate = In_deltaTime * _nowSpeed;
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
		ChangeRotDirection();
	}
}

void FGsCameraRotationDialogCutLoop::SetRotation(const FRotator& In_rot)
{
	if (_local == nullptr)
	{
		return;
	}
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (UCameraComponent* camComponent = localChar->GetFollowCamera())
	{
		camComponent->SetWorldRotation(In_rot);
	}
}

void FGsCameraRotationDialogCutLoop::RemoveLocalPlayer()
{
	_local = nullptr;
}

// 현재 사용 커브 접근
UCurveFloat* FGsCameraRotationDialogCutLoop::GetCurve()
{
	UCurveFloat* curve = GData()->GetCamFollowCurve();
	return curve;
}

void FGsCameraRotationDialogCutLoop::StartRotation(const FRotator& In_currentRot)
{
	_isRotaionLoopStart = true;
	// 
	_isAdd = true;
	_rotStartCurrentVal = In_currentRot;

	_rotFrom = _rotStartCurrentVal;
	_rotTo = _rotStartCurrentVal + _dialogCutLoopOffset;

	// 진행도 초기화
	_progressRate = 0.0f;

}

void FGsCameraRotationDialogCutLoop::ChangeRotDirection()
{
	// reverse direction
	_isAdd = !_isAdd;

	// 진행도 초기화
	_progressRate = 0.0f;

	if (_isAdd == true)
	{
		_rotFrom = _rotStartCurrentVal;
		_rotTo = _rotStartCurrentVal + _dialogCutLoopOffset;
	}
	else
	{
		_rotFrom = _rotStartCurrentVal + _dialogCutLoopOffset;
		_rotTo = _rotStartCurrentVal;
	}
}