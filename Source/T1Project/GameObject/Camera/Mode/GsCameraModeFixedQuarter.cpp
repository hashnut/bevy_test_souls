#include "GsCameraModeFixedQuarter.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationQuarter.h"
#include "GameObject/Camera/GsCameraModeHandler.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "GameFramework/PlayerController.h"

FGsCameraModeFixedQuarter::FGsCameraModeFixedQuarter() : FGsCameraModeControlBase(EGsCameraMode::FixedQuarter)
{
}

void FGsCameraModeFixedQuarter::Initialize()
{
	// 대사창 업데이트시 회전갱신후 그값으로 이동처리해야하므로 
	// rot을 먼저 등록!!!
	IGsCameraWork* rotationWork = new FGsCameraRotationQuarter();
	rotationWork->Initialize();
	rotationWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::RotationWork, rotationWork);

	Super::Initialize();
}
// 다음 스텝 진행
void FGsCameraModeFixedQuarter::NextStep(FGsCameraModeHandler* In_handler)
{
	Super::NextStep(In_handler);
	if (In_handler == nullptr)
	{
		return;
	}

	// 다음은 back view 다
	In_handler->ChangeState(EGsCameraMode::BackView);
}

// 인풋에 의한 모드 변환
void FGsCameraModeFixedQuarter::ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData)
{
	Super::ChangeModeNormal(In_modeData);

	// init yaw mode north

	FGsCameraModeManager* camModeManager = GSCameraMode();
	if (camModeManager == nullptr)
	{
		return;
	}

	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetRotationType(EGsCameraRotationType::ControlRotaion);
	}

	SetModeRot(In_modeData);

	PostChangeModeNormal();
}
// 그리팅 끝 호출
void FGsCameraModeFixedQuarter::OnGreetingOff(bool In_isDirect)
{
	Super::OnGreetingOff(In_isDirect);

	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetRotationType(EGsCameraRotationType::ControlRotaion);
	}

	SetModeRot(camModeData, In_isDirect);
}

// 모드 회전 저장 하기
void FGsCameraModeFixedQuarter::SetModeRot(const FGsSchemaCameraModeData* In_modeData, bool In_isDirect)
{
	if (In_modeData == nullptr)
	{
		return;
	}

	// 캐릭터 방향에 맞춘다
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr ||
		localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false)
	{
		return;
	}

	FRotator controlRotTo = GetModeRot(In_modeData);
	FRotator controlRotFrom = localChar->Controller->GetControlRotation();

	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		if (In_isDirect == true)
		{
			rotationWork->SetRotation(controlRotTo);
		}
		else
		{
			rotationWork->SetTargetRot(controlRotTo, controlRotFrom);
		}
	}
}
// 줌인
void FGsCameraModeFixedQuarter::ZoomIn(float In_mulVal)
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
void FGsCameraModeFixedQuarter::ZoomOut(float In_mulVal)
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
void FGsCameraModeFixedQuarter::OnZoomFinished()
{
	Super::OnZoomFinished();

	if (FGsCameraRotationQuarter* rotationWork = GetCameraRotationQuarter())
	{
		rotationWork->SetIsUsingZoomRot(false);
	}
}
// 모드 회전 구하기
FRotator FGsCameraModeFixedQuarter::GetModeRot(const FGsSchemaCameraModeData* In_modeData)
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

	if (true == camModeManager->GetIsBackUpData())
	{
		controlRotTo.Pitch = camModeManager->GetBackupQuat().Rotator().Pitch;
	}
	else
	{
		EGsCameraMode nowMode = GetType();
		float checkedControllerPitch =
			GSCameraMode()->GetModeBackupCheckedPitch(nowMode);
		controlRotTo.Pitch = checkedControllerPitch;
	}

	float nowDegree = 0.0f;

	EGsCameraFixedQuarterYawType yawType = camModeManager->GetFixedQuarterYawType();
	switch (yawType)
	{
	case EGsCameraFixedQuarterYawType::North:
		nowDegree = GData()->GetGlobalData()->_cameraFixedQuarterModeYawDegreeNorth;
		break;
	case EGsCameraFixedQuarterYawType::East:
		nowDegree = GData()->GetGlobalData()->_cameraFixedQuarterModeYawDegreeEast;
		break;
	case EGsCameraFixedQuarterYawType::South:
		nowDegree = GData()->GetGlobalData()->_cameraFixedQuarterModeYawDegreeSouth;
		break;
	case EGsCameraFixedQuarterYawType::West:
		nowDegree = GData()->GetGlobalData()->_cameraFixedQuarterModeYawDegreeWest;
		break;
	}


	// 0~360 -> -180~180
	// degree -180하면 될듯
	float worldYawVal =  nowDegree - 180.0f;
	controlRotTo.Yaw = worldYawVal;

	return controlRotTo;
}

// 현재 줌min 체크 하는가
bool FGsCameraModeFixedQuarter::GetIsNowCheckZoomMin()
{
	// 그리팅이 아니면 함
	return (_isBeforeGreeting == true) ? false : true;
}

void FGsCameraModeFixedQuarter::ChangeYawMode()
{
	FGsCameraModeManager* camModeManager = GSCameraMode();
	if (camModeManager == nullptr)
	{
		return;
	}

	EGsCameraFixedQuarterYawType currType = camModeManager->GetFixedQuarterYawType();
	EGsCameraFixedQuarterYawType nextType = EGsCameraFixedQuarterYawType::North;
	switch (currType)
	{
	case EGsCameraFixedQuarterYawType::North:
		nextType = EGsCameraFixedQuarterYawType::East;
		break;
	case EGsCameraFixedQuarterYawType::East:
		nextType = EGsCameraFixedQuarterYawType::South;
		break;
	case EGsCameraFixedQuarterYawType::South:
		nextType = EGsCameraFixedQuarterYawType::West;
		break;
	case EGsCameraFixedQuarterYawType::West:
		nextType = EGsCameraFixedQuarterYawType::North;
		break;
	}
	camModeManager->SetFixedQuarterYawType(nextType);

	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetRotationType(EGsCameraRotationType::ControlRotaion);
	}

	SetModeRot(camModeData);
}