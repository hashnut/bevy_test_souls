#include "GsCameraModeQuarter.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationQuarter.h"
#include "GameObject/Camera/GsCameraModeHandler.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameFramework/PlayerController.h"
#include "GameFrameWork/SpringArmComponent.h"

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Camera/GsSchemaCameraModeData.h"

#include "UTIL/GsClassUtil.h"

#include "T1Project.h"

FGsCameraModeQuarter::FGsCameraModeQuarter() : FGsCameraModeControlBase(EGsCameraMode::Quarter)
{
}
void FGsCameraModeQuarter::Initialize()
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
void FGsCameraModeQuarter::NextStep(FGsCameraModeHandler* In_handler)
{
	Super::NextStep(In_handler);
	if (In_handler == nullptr)
	{
		return;
	}

	// 다음은 fixed quarter 다
	In_handler->ChangeState(EGsCameraMode::FixedQuarter);
}

// 인풋에 의한 모드 변환
void FGsCameraModeQuarter::ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData)
{
	Super::ChangeModeNormal(In_modeData);

	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetRotationType(EGsCameraRotationType::ControlRotaion);
	}
	
	SetModeRot(In_modeData);

	PostChangeModeNormal();
}
// 그리팅 끝 호출
void FGsCameraModeQuarter::OnGreetingOff(bool In_isDirect)
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

// 모드 회전 세팅하기
void FGsCameraModeQuarter::SetModeRot(const FGsSchemaCameraModeData* In_modeData, bool In_isDirect)
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
void FGsCameraModeQuarter::ZoomIn(float In_mulVal)
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
void FGsCameraModeQuarter::ZoomOut(float In_mulVal)
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
void FGsCameraModeQuarter::OnZoomFinished()
{
	Super::OnZoomFinished();

	if (FGsCameraRotationQuarter* rotationWork = GetCameraRotationQuarter())
	{	
		rotationWork->SetIsUsingZoomRot(false);
	}
}
// 현재 줌min 체크 하는가
bool FGsCameraModeQuarter::GetIsNowCheckZoomMin()
{
	// 그리팅이 아니면 함
	return (_isBeforeGreeting == true)? false: true;
}