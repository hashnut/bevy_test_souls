#include "GsCameraModeSpaceCrack.h"

#include "GameObject/Camera/Work/Rotation/GsCameraRotationQuarter.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"

#include "ControllerEx/GsPlayerController.h"

FGsCameraModeSpaceCrack::FGsCameraModeSpaceCrack() :FGsCameraModeControlBase(EGsCameraMode::SpaceCrack)
{
}
void FGsCameraModeSpaceCrack::Initialize()
{
	// 대사창 업데이트시 회전갱신후 그값으로 이동처리해야하므로 
	// rot을 먼저 등록!!!
	// 쿼터용으로 쓰자
	IGsCameraWork* rotationWork = new FGsCameraRotationQuarter();
	rotationWork->Initialize();
	rotationWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::RotationWork, rotationWork);

	Super::Initialize();
}

// 줌인
void FGsCameraModeSpaceCrack::ZoomIn(float In_mulVal)
{
	Super::ZoomIn(In_mulVal);
	// 모드 변환 연출 끊기
	if (FGsCameraRotationQuarter* rotationWork = GetCameraRotationQuarter())
	{
		rotationWork->SetIsModeRotChanging(false);
		rotationWork->SetIsUsingZoomRot(true);
	}
}
// 줌아웃
void FGsCameraModeSpaceCrack::ZoomOut(float In_mulVal)
{
	Super::ZoomOut(In_mulVal);
	// 모드 변환 연출 끊기
	if (FGsCameraRotationQuarter* rotationWork = GetCameraRotationQuarter())
	{
		rotationWork->SetIsModeRotChanging(false);
		rotationWork->SetIsUsingZoomRot(true);
	}
}

// 줌 종료 콜백
void FGsCameraModeSpaceCrack::OnZoomFinished()
{
	if (FGsCameraRotationQuarter* rotationWork = GetCameraRotationQuarter())
	{
		rotationWork->SetIsUsingZoomRot(false);
	}
}

// 시작 모드 줌 값 얻어오기
float FGsCameraModeSpaceCrack::GetStartModeZoom()
{
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		return 0.0f;
	}

	float zoomVal = camModeData->ArmLengthTo;
	return zoomVal;
}

// 모드 회전 구하기
// 시공의 틈새는 백업된 데이터를 쓰지 않는다
FRotator FGsCameraModeSpaceCrack::GetModeRot(const FGsSchemaCameraModeData* In_modeData)
{
	FRotator controlRotTo = FRotator::ZeroRotator;
	if (In_modeData == nullptr)
	{
		return controlRotTo;
	}

	// 캐릭터 방향에 맞춘다
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr ||
		localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false)
	{
		return controlRotTo;
	}

	FGsCameraModeManager* camModeManager = GSCameraMode();
	if (camModeManager == nullptr)
	{
		return controlRotTo;
	}

	controlRotTo = localChar->Controller->GetControlRotation();
		
	EGsCameraMode nowMode = GetType();
	float checkedControllerPitch =
		GSCameraMode()->GetModeBackupCheckedPitch(nowMode);
	controlRotTo.Pitch = checkedControllerPitch;
	
	return controlRotTo;
}