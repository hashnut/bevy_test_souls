#include "GsCameraDOFGame.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Engine/Classes/Curves/CurveFloat.h"
#include "Engine/Classes/Engine/Scene.h"

#include "Camera/CameraComponent.h"

void FGsCameraDOFGame::Initialize()
{
	_cameraZoomMinLerpSpeed = GData()->GetGlobalData()->_cameraZoomMinLerpSpeed;
}

// 마지막 처리
void FGsCameraDOFGame::Finalize()
{
	_targetMode = nullptr;
	RemoveLocalPlayer();
}


void FGsCameraDOFGame::Update(float In_deltaTime)
{
	if (_isFinish == true)
	{
		return;
	}

	float speed = _cameraZoomMinLerpSpeed;
	float addRate = In_deltaTime * speed;
	_progressRate = FMath::Clamp<float>(_progressRate + addRate, 0.f, 1.f);

	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressRate), 0.f, 1.f);
	}

	float newScale = FMath::Lerp(_mobileScaleFrom, _mobileScaleTo, curveVal);

	if (FMath::IsNearlyEqual(1.0f, curveVal, 0.001f) == false)
	{

	}
	else
	{
		_isFinish = true;
	}

	UCameraComponent* camComponent = GetCameraComponent();
	if (camComponent == nullptr)
	{
		return;
	}
	camComponent->PostProcessSettings.DepthOfFieldScale = newScale;
	

	if (_isFinish == true)
	{
		// 꺼지는 상황이면 scale 0에 도달하면 끄자
		if (_isOn == false)
		{
			SetDofOption(false);
		}
	}
}

// dof 옵션값 on, off
void FGsCameraDOFGame::SetDofOption(bool In_isOn)
{

	UCameraComponent* camComponent = GetCameraComponent();
	if (camComponent == nullptr)
	{
		return;
	}

	// 키는 경우면 scale 0으로 세팅하고 on한다(튈까봐)
	if (In_isOn == true)
	{
		camComponent->PostProcessSettings.DepthOfFieldScale = 0.0f;
	}

	uint8 isOnInt = (In_isOn == true) ? 1 : 0;

	camComponent->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = isOnInt;
	camComponent->PostProcessSettings.bOverride_DepthOfFieldFstop = isOnInt;


	// 스케일 on, off 처리
	camComponent->PostProcessSettings.bOverride_DepthOfFieldScale = isOnInt;
	// focal region on, off 처리
	camComponent->PostProcessSettings.bOverride_DepthOfFieldFocalRegion = isOnInt;


	float focalDistance = GetFocalDistance();

	camComponent->PostProcessSettings.DepthOfFieldFocalDistance = focalDistance;
	camComponent->PostProcessSettings.DepthOfFieldFstop = GData()->GetGlobalData()->_cameraZoomDepthofFieldFstop;;
}
void FGsCameraDOFGame::UpdateFocalDistance()
{	
	if (_isOn == false)
	{
		return;
	}

	UCameraComponent* camComponent = GetCameraComponent();
	if (camComponent == nullptr)
	{
		return;
	}

	float focalDistance = GetFocalDistance();
	camComponent->PostProcessSettings.DepthOfFieldFocalDistance = focalDistance;
}
void FGsCameraDOFGame::StartDOF(float In_mobileScaleFrom, float In_mobileScaleTo,
	 bool In_isOn)
{
	_mobileScaleFrom = In_mobileScaleFrom;
	_mobileScaleTo = In_mobileScaleTo;

	_progressRate = 0.0f;

	_isFinish = false;

	_isOn = In_isOn;

	SetDofOption(_isOn);
}

// 바로 세팅 하기
void FGsCameraDOFGame::SetDirectDOF(float In_mobileFocalRegion, float In_mobileScale, bool In_isOn,
	float In_focalDistance)
{
	SetDofOption(In_isOn);

	if (In_isOn == false)
	{
		return;
	}

	UCameraComponent* camComponent = GetCameraComponent();

	if (camComponent == nullptr)
	{
		return;
	}
	camComponent->PostProcessSettings.DepthOfFieldScale = In_mobileScale;
	camComponent->PostProcessSettings.DepthOfFieldFocalRegion = In_mobileFocalRegion;
	
	camComponent->PostProcessSettings.DepthOfFieldFocalDistance = In_focalDistance;

}
void FGsCameraDOFGame::RemoveLocalPlayer()
{
	_local = nullptr;
	_targetObj = nullptr;
}

float FGsCameraDOFGame::GetFocalDistance()
{
	UCameraComponent* camComponent = GetCameraComponent();

	if (camComponent == nullptr ||
		_targetObj == nullptr)
	{
		return 0.0f;
	}

	float fDistance = (camComponent->GetComponentLocation() - _targetObj->GetLocation()).Size();
	return fDistance;
}

UCameraComponent* FGsCameraDOFGame::GetCameraComponent()
{
	if (_local == nullptr)
	{
		return nullptr;
	}

	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return nullptr;
	}

	UCameraComponent* camComponent = localChar->GetFollowCamera();
	return camComponent;
}