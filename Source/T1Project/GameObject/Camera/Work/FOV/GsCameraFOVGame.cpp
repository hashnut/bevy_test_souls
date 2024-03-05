#include "GsCameraFOVGame.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Engine/Classes/Curves/CurveFloat.h"
#include "Engine/Classes/Camera/CameraComponent.h"

void FGsCameraFOVGame::Finalize()
{
	RemoveLocalPlayer();
}

void FGsCameraFOVGame::RemoveLocalPlayer()
{
	_local = nullptr;
	_targetObj = nullptr;
}

// 모드 오프셋 시작
void FGsCameraFOVGame::StartModeFOV(float In_FOVFrom, float in_FOVTo)
{
	_modeFOVFrom = In_FOVFrom;
	_modeFOVTo = in_FOVTo;

	// 목적값이 0이면 안한다는 뜻
	if (_modeFOVTo == 0.0f)
	{
		_isModeFOVChanging = false;
		return;
	}

	if (_modeFOVFrom == _modeFOVTo)
	{
		_isModeFOVChanging = false;
	}
	else
	{
		_isModeFOVChanging = true;
		_progressModeFOVRate = 0.0f;
	}

	MakeSpeed();
}
// fov 갱신
void FGsCameraFOVGame::Update(float In_deltaTime)
{
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	if (false == _isModeFOVChanging)
	{
		return;
	}

	// 속도
	float speed = _nowSpeed;

	float addRate = In_deltaTime * speed;
	_progressModeFOVRate = FMath::Clamp<float>(_progressModeFOVRate + addRate, 0.f, 1.f);
	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressModeFOVRate), 0.f, 1.f);
	}

	float curveNewFOV =
		FMath::Lerp(_modeFOVFrom, _modeFOVTo, curveVal);

	if (curveNewFOV != _modeFOVTo)
	{

	}
	else
	{
		_isModeFOVChanging = false;
	}
	localChar->GetFollowCamera()->SetFieldOfView(curveNewFOV);

}

// 속도 만들기
void FGsCameraFOVGame::MakeSpeed()
{
	if (_isAfterDialog == true)
	{
		// 회전과 이동 같은 속도로 처리하게 하기위해 외부에서 밀어넣음
	}
	else
	{
		_nowSpeed = GetRotSpeed();
	}
}

// 회전속도 구하기
float FGsCameraFOVGame::GetRotSpeed()
{
	float speed = 0.0f;
	if (_isAfterDialog == true)
	{
		speed =
			GData()->GetGlobalData()->CamDialogLocLerpSpeed;
	}
	else
	{
		speed =
			GData()->GetGlobalData()->CamModeChangeRotLerpSpeed;
	}
	return speed;
}
// 바로 세팅
void FGsCameraFOVGame::SetDirectFOV(float In_FOV)
{
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	localChar->GetFollowCamera()->SetFieldOfView(In_FOV);
}

void FGsCameraFOVGame::SetNowSpeed(float In_speed)
{ 
	// 대사 연출 복구일때만 세팅 하자
	if (_isAfterDialog == true)
	{
		_nowSpeed = In_speed;
	}
}