#include "GsCameraModeEquipView.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameObject/Camera/Interface/GsCameraWork.h"
#include "GameObject/Camera/Work/FOV/GsCameraFOVGame.h"
#include "GameObject/Camera/Work/Zoom/GsCameraZoomGame.h"
#include "GameObject/Camera/Work/Location/GsCameraLocationGame.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationBackView.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationQuarter.h"
#include "GameObject/Camera/Work/Offset/GsCameraTargetOffsetGame.h"
#include "GameObject/Camera/Work/Offset/GsCameraOffsetGame.h"
#include "GameObject/Camera/Work/DOF/GsCameraDOFGame.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Runtime/DataCenter/Public/DataSchema/Camera/GsSchemaCameraModeData.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "UTIL/GsClassUtil.h"

#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"

FGsCameraModeEquipView::FGsCameraModeEquipView() : FGsCameraModeBase(EGsCameraMode::EquipView)
{

}
void FGsCameraModeEquipView::Initialize()
{
	Super::Initialize();

	IGsCameraWork* fovWork = new FGsCameraFOVGame();
	fovWork->Initialize();
	_mapCamWorks.Add(EGsCameraWorkType::FOVWork, fovWork);

	IGsCameraWork* zoomWork = new FGsCameraZoomGame();
	zoomWork->Initialize();
	zoomWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::ZoomWork, zoomWork);

	IGsCameraWork* locationWork = new FGsCameraLocationGame();
	locationWork->Initialize();
	locationWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::LocationWork, locationWork);

	IGsCameraWork* rotationWork = new FGsCameraRotationBackView();
	rotationWork->Initialize();
	rotationWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::RotationWork, rotationWork);

	IGsCameraWork* targetOffsetWork = new FGsCameraTargetOffsetGame();
	targetOffsetWork->Initialize();
	_mapCamWorks.Add(EGsCameraWorkType::TargetOffsetWork, targetOffsetWork);

	IGsCameraWork* offsetWork = new FGsCameraOffsetGame();
	offsetWork->Initialize();
	_mapCamWorks.Add(EGsCameraWorkType::OffsetWork, offsetWork);
}

void FGsCameraModeEquipView::ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData)
{

	// 모드 회전 변환 시작
	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetIsModeRotChanging(true);
	}


	// 모드 줌 시작
	StartModeZoom(In_modeData);

	StartModeSocketOffset();

	StartCameraOffset();

	StartFollow(EGsCameraInitType::ModeChange);

	StartDOFChange();
}

// 모드줌 시작
void FGsCameraModeEquipView::StartModeZoom(const FGsSchemaCameraModeData* In_modeData,
	EGsCameraChangeZoomType In_type)
{
	FGsCameraZoomGame* zoomWork = GetCameraZoomGame();

	if (nullptr == zoomWork)
	{
		return;
	}
	
	if (In_modeData == nullptr)
	{
		return;
	}

	float zoomVal = In_modeData->ArmLengthTo;
	zoomWork->StartModeZoom(zoomVal, In_type);
}

// 카메라 회전 처리 접근
FGsCameraZoomGame* FGsCameraModeEquipView::GetCameraZoomGame()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::ZoomWork);
	if (work == nullptr)
	{
		return nullptr;
	}


	FGsCameraZoomGame* zoomWork = FGsClassUtil::FCheckStaticCast<FGsCameraZoomGame>(work);
	return zoomWork;
}

void FGsCameraModeEquipView::StartCameraOffset()
{
	// 2. shift 값(모드별)
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		return;
	}
	FVector shiftPos = camModeData->CamShiftPos;
	
	StartTargetSocketOffset(shiftPos);
}

void FGsCameraModeEquipView::StartTargetSocketOffset(FVector In_targetOffset)
{
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	if (FGsCameraTargetOffsetGame* targetOffsetWork = GetCameraTargetOffsetGame())
	{
				
		targetOffsetWork->StartModeTargetOffset(
			localChar->GetSpringArm()->TargetOffset, In_targetOffset);
		
	}
}

FGsCameraTargetOffsetGame* FGsCameraModeEquipView::GetCameraTargetOffsetGame()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::TargetOffsetWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraTargetOffsetGame* targetOffsetWork = FGsClassUtil::FCheckStaticCast<FGsCameraTargetOffsetGame>(work);
	return targetOffsetWork;
}

// fov 변환 시작
void FGsCameraModeEquipView::StartModeFOV()
{

	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	if (localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false)
	{
		return;
	}

	APlayerController* const PC =
		CastChecked<APlayerController>(localChar->Controller);

	if (nullptr == PC)
	{
		return;
	}

	if (nullptr == PC->PlayerCameraManager)
	{
		return;
	}

	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		return;
	}
	if (FGsCameraFOVGame* fovWork = GetCameraFOVGame())
	{
		fovWork->StartModeFOV(
			PC->PlayerCameraManager->GetFOVAngle(), camModeData->FOV);
		
	}
}

// 업데이트
void FGsCameraModeEquipView::Update(float In_deltaTime)
{
	Super::Update(In_deltaTime);
	
	UpdateFollow(In_deltaTime);
}
// follow 갱신
void FGsCameraModeEquipView::UpdateFollow(float In_deltaTime)
{
	
	// 모드 전환 회전중에는 follow를 안한다
	bool isModeRotChanging = false;
	bool isRotFinish = true;
	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		isModeRotChanging = rotationWork->GetIsModeRotChanging();
		isRotFinish = rotationWork->GetIsRotationFinish();
	}
	
	// 모드 변환 rot 
	if (isModeRotChanging == true && isRotFinish == false)
	{
		return;
	}

	// 이동에 의한 회전으로
	// 모드 변환 막는거 해제
	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetIsModeRotChanging(false);
	}

	EGsCameraInitType initMode = EGsCameraInitType::None;
	// is target exist
	if (FGsTargetHandlerBase* targetHandler = _targetPlayer->GetTargetHandler())
	{
		if (UGsGameObjectBase* targetHanderTarget = targetHandler->GetTarget())
		{
			initMode = EGsCameraInitType::TargetFollow;
		}
	}

	StartFollow(initMode);

}

// 따라가기 시작
void FGsCameraModeEquipView::StartFollow(EGsCameraInitType In_initType)
{
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		GSLOG(Error, TEXT("now mode: %s data is null"),
			*GetEnumToString(EGsCameraMode, nowMode));
		return;
	}



	// 캐릭터 방향에 맞춘다
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	AGsCharacterBase* targetChar = _targetPlayer->GetCharacter();
	if (localChar == nullptr ||
		localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false ||
		targetChar == nullptr)
	{
		return;
	}
	// mode pitch 구하기
	// 1. 기본 모드값
	// 2. min, max 검증 까지 함
	float checkedPitch = GSCameraMode()->GetModeCheckedPitch(nowMode);

	FRotator rotTo = FRotator::ZeroRotator;

	// 액터 회전에 pitch 모드 데이터 사용
	if (In_initType == EGsCameraInitType::ModeChange)
	{
		rotTo = targetChar->GetActorRotation();
		rotTo.Pitch = checkedPitch;
		
	}

	if (FGsCameraRotationBackView* rotationWork = GetCameraRotationBackView())
	{
		rotationWork->StartFollow(In_initType, rotTo, false);
	}
}

// 카메라 회전 백뷰 처리 접근
FGsCameraRotationBackView* FGsCameraModeEquipView::GetCameraRotationBackView()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::RotationWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraRotationBackView* rotationWork = FGsClassUtil::FCheckStaticCast<FGsCameraRotationBackView>(work);
	return rotationWork;
}

// 카메라 오프셋 처리 접근
FGsCameraOffsetGame* FGsCameraModeEquipView::GetCameraOffsetGame()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::OffsetWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraOffsetGame* offsetWork = FGsClassUtil::FCheckStaticCast<FGsCameraOffsetGame>(work);
	return offsetWork;
}

void FGsCameraModeEquipView::StartModeSocketOffset()
{
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		return;
	}
	if (FGsCameraOffsetGame* offsetWork = GetCameraOffsetGame())
	{		
		offsetWork->StartModeSocketOffset(
			localChar->GetSpringArm()->SocketOffset, camModeData->SocketOffset);
		
	}
}

void FGsCameraModeEquipView::StartDOFChange()
{
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	UCameraComponent* camComponent = localChar->GetFollowCamera();
	if (camComponent == nullptr)
	{
		return;
	}

	float scaleFrom =
		camComponent->PostProcessSettings.DepthOfFieldScale;

	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		return;
	}

	bool isDofOn = camModeData->UseZoomMinDOF;
	float scaleTo = (isDofOn == true) ?
		GData()->GetGlobalData()->_cameraDepthOfFieldScaleMax :
		0.0f;

	if (FGsCameraDOFGame* dofGame = GetCameraDOFGame())
	{
		dofGame->StartDOF(scaleFrom, scaleTo, isDofOn);
	}
}