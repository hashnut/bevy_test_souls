#include "GsCameraModePlayableEvent.h"

#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationQuarter.h"
#include "GameObject/Camera/Work/Location/GsCameraLocationGame.h"
#include "GameObject/Camera/Work/FOV/GsCameraFOVGame.h"
#include "GameObject/Camera/GsCameraModeHandler.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Camera/GsSchemaCameraModeData.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsActorPlayableEventCameraData.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "GameFlow/Stage/GsStageMode.h"

#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"
#include "GameFlow/GameContents/ContentsLobby/Hud/GsStateLobbyHud.h"

#include "GameFlow/GameContents/ContentsLobby/Hud/PlayableEvent/GsPlayableEventHandler.h"

#include "GameFramework/PlayerController.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"


FGsCameraModePlayableEvent::FGsCameraModePlayableEvent() : FGsCameraModeControlBase(EGsCameraMode::PlayableEvent)
{
}


void FGsCameraModePlayableEvent::Initialize()
{
	// 대사창 업데이트시 회전갱신후 그값으로 이동처리해야하므로 
	// rot을 먼저 등록!!!
	IGsCameraWork* rotationWork = new FGsCameraRotationQuarter();
	rotationWork->Initialize();
	rotationWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::RotationWork, rotationWork);

	Super::Initialize();
}

// 줌인
void FGsCameraModePlayableEvent::ZoomIn(float In_mulVal)
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
void FGsCameraModePlayableEvent::ZoomOut(float In_mulVal)
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
void FGsCameraModePlayableEvent::OnZoomFinished()
{
	if (FGsCameraRotationQuarter* rotationWork = GetCameraRotationQuarter())
	{
		rotationWork->SetIsUsingZoomRot(false);
	}
}
// 모드 회전 구하기
FRotator FGsCameraModePlayableEvent::GetModeRot(const FGsSchemaCameraModeData* In_modeData)
{
	FRotator controlRotTo = FRotator::ZeroRotator;
	if (In_modeData == nullptr)
	{
		return controlRotTo;
	}

	// 캐릭터 방향에 맞춘다
	AGsCharacterBase* targetChar = _targetPlayer->GetCharacter();
	if (targetChar == nullptr)
	{
		return controlRotTo;
	}
	
	controlRotTo  = targetChar->GetActorRotation();
	controlRotTo.Pitch = In_modeData->ControllerPitch;

	
	return controlRotTo;
}
void FGsCameraModePlayableEvent::InitMode(
	AGsCharacterLocalPlayer* In_localChar, const FGsSchemaCameraModeData* In_modeData)
{
	Super::InitMode(In_localChar, In_modeData);

	

	if (In_localChar == nullptr ||
		In_localChar->Controller == nullptr ||
		In_localChar->Controller->IsLocalPlayerController() == false ||
		_targetPlayer == nullptr)
	{
		return;
	}
	APlayerController* controller =
		CastChecked<APlayerController>(In_localChar->Controller);

	AGsCharacterBase* targetChar = _targetPlayer->GetCharacter();
	if (targetChar == nullptr)
	{
		return;
	}

	FRotator actorRot = targetChar->GetActorRotation();


	float limitVal =
		GData()->GetGlobalData()->_playableEventCameraRotYawPossible;

	float minYaw = actorRot.Yaw - limitVal;
	float maxYaw = actorRot.Yaw + limitVal;

	// pitch min max
	if (nullptr != controller->PlayerCameraManager)
	{
		// backup
		if (GMode())
		{
			if (FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow())
			{
				if (FGsContentsManagerLobby* contents = lobbyFlow->GetContentsManagerLobby())
				{
					FGsStateLobbyHud* hud =
						contents->GetContentsStateObject<FGsStateLobbyHud>(FGsContentsMode::InLobby::HUD);

					if (nullptr == hud)
					{
						return;
					}

					if (FGsPlayableEventHandler* handler = hud->GetPlayableEventHandler())
					{
						handler->SetBackupPlayableEventYawMinMax(controller->PlayerCameraManager->ViewYawMin,
							controller->PlayerCameraManager->ViewYawMax);
					}
				}
			}
		}

		controller->PlayerCameraManager->ViewYawMin =
			minYaw;
		controller->PlayerCameraManager->ViewYawMax =
			maxYaw;
	}
}
void FGsCameraModePlayableEvent::SetStartCameraDataTransform()
{
	FGsCameraRotationBase* rotationWork = GetCameraRotationBase();
	FGsCameraLocationGame* locationWork = GetCameraLocationGame();

	if (rotationWork == nullptr ||
		locationWork == nullptr)
	{
		return;
	}

	locationWork->SetLocationSpace(EGsCameraLocationSpaceType::WorldLocation);
	rotationWork->SetRotationType(EGsCameraRotationType::WorldRotation);

	// find first camera tr
	AGsActorPlayableEventCameraData* camData = LSLobbyPlayableEvent()->FindCamData(true);
	
	FVector firstPos = FVector::ZeroVector;
	FRotator firstRot = FRotator::ZeroRotator;

	if (camData != nullptr)
	{
		firstPos = camData->GetActorLocation();
		firstRot = camData->GetActorRotation();
	}	

	// 1. set cam data tr
	rotationWork->SetRotation(firstRot);
	locationWork->SetDirectPos(firstPos);
}
void FGsCameraModePlayableEvent::StartPlayableEventStartCam()
{
	FGsCameraRotationBase* rotationWork = GetCameraRotationBase();
	FGsCameraLocationGame* locationWork = GetCameraLocationGame();

	if (rotationWork == nullptr ||
		locationWork == nullptr)
	{
		return;
	}

	locationWork->SetLocationSpace(EGsCameraLocationSpaceType::WorldLocation);
	rotationWork->SetRotationType(EGsCameraRotationType::WorldRotation);

	// current cam tr
	UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
	if (local == nullptr)
	{
		return;
	}
	AGsCharacterLocalPlayer* playerActor = Cast<AGsCharacterLocalPlayer>(local->GetActor());
	if (playerActor == nullptr)
	{
		return;
	}


	float modeZoom = GetStartModeZoom();

	FRotator springArmDesiredRot = playerActor->GetSpringArm()->GetTargetRotation();

	// Get the spring arm 'origin', the target we want to look at
	FVector ArmOrigin = playerActor->GetSpringArm()->GetComponentLocation() + playerActor->GetSpringArm()->TargetOffset;
	FVector DesiredLoc = ArmOrigin;
	// Now offset camera position back along our rotation
	DesiredLoc -= springArmDesiredRot.Vector() * modeZoom;
	// Add socket offset in local space
	DesiredLoc += FRotationMatrix(springArmDesiredRot).TransformVector(playerActor->GetSpringArm()->SocketOffset);

	// find first camera tr
	AGsActorPlayableEventCameraData* camData = LSLobbyPlayableEvent()->FindCamData(true);

	FVector firstPos = FVector::ZeroVector;
	FRotator firstRot = FRotator::ZeroRotator;

	if (camData != nullptr)
	{
		firstPos = camData->GetActorLocation();
		firstRot = camData->GetActorRotation();
	}

	// 2. set move pos, rot
	rotationWork->SetTargetRot(springArmDesiredRot, firstRot);
	locationWork->SetDataNormal(DesiredLoc, firstPos);


	locationWork->SetIsUseFinishCallback(true);
	rotationWork->SetIsUseFinishCallback(true);

	MakePlayableEventCamLerpSpeed(true, locationWork, rotationWork);

	_isStartCamLerp = true;
}
// 보간 끝
void FGsCameraModePlayableEvent::OnFinishLerpMode()
{
	if (_localPlayer == nullptr)
	{
		return;
	}
	if (FGsCameraModeHandler* cameraModehandler = _localPlayer->GetCameraModeHandler())
	{		
		cameraModehandler->OnFinishPlayableCamLerp(_isStartCamLerp);	
	}
}

// 다른맵 스폰 초기화
void FGsCameraModePlayableEvent::InitOtherMapSpawn(const FGsSchemaCameraModeData* In_modeData)
{
	if (nullptr == In_modeData)
	{
		return;
	}

	// 모드 줌 시작
	StartModeZoom(EGsCameraChangeZoomType::NoLerp);

	// 회전 세팅
	FRotator modeRot = GetModeRot(In_modeData);
	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetRotationType(EGsCameraRotationType::ControlRotaion);
		rotationWork->SetRotation(modeRot);
	}
	// 오프셋 세팅
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr ||
		In_modeData == nullptr)
	{
		return;
	}
	localChar->GetSpringArm()->SocketOffset = In_modeData->SocketOffset;

	// fov 세팅
	if (FGsCameraFOVGame* fovWork = GetCameraFOVGame())
	{
		float nowModeFOV = GetNowModeFOV();
		fovWork->SetDirectFOV(nowModeFOV);
	}

	// 카메라 오프셋 변환
	StartCameraOffset(true);

	// focal region 세팅
	SetModeMobileFocalRegion(In_modeData);
}

void FGsCameraModePlayableEvent::StartPlayableEventEndCam()
{
	FGsCameraRotationBase* rotationWork = GetCameraRotationBase();
	FGsCameraLocationGame* locationWork = GetCameraLocationGame();

	if (rotationWork == nullptr ||
		locationWork == nullptr)
	{
		return;
	}

	locationWork->SetLocationSpace(EGsCameraLocationSpaceType::WorldLocation);
	rotationWork->SetRotationType(EGsCameraRotationType::WorldRotation);

	// find end camera tr
	AGsActorPlayableEventCameraData* camData = LSLobbyPlayableEvent()->FindCamData(false);

	FVector lastPos = FVector::ZeroVector;
	FRotator lastRot = FRotator::ZeroRotator;

	if (camData != nullptr)
	{
		lastPos = camData->GetActorLocation();
		lastRot = camData->GetActorRotation();
	}	

	UWorld* world = LSLobbyPlayableEvent()->GetWorld();

	if (world == nullptr)
	{
		return;
	}


	FVector camLoc = FVector::ZeroVector;
	FRotator camRot = FRotator::ZeroRotator;
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
	{
		APlayerCameraManager* camManager = controller->PlayerCameraManager;
		camLoc = camManager->GetCameraLocation();
		camRot = camManager->GetCameraRotation();
	}


	//  set move pos, rot
	rotationWork->SetTargetRot(lastRot, camRot);
	locationWork->SetDataNormal(lastPos, camLoc);


	locationWork->SetIsUseFinishCallback(true);
	rotationWork->SetIsUseFinishCallback(true);

	MakePlayableEventCamLerpSpeed(false, locationWork, rotationWork);

	_isStartCamLerp = false;
}

void FGsCameraModePlayableEvent::MakePlayableEventCamLerpSpeed(bool In_isStartCam,
	FGsCameraLocationGame* In_locationWork, FGsCameraRotationBase* In_rotationWork)
{
	if (In_locationWork == nullptr ||
		In_rotationWork == nullptr)
	{
		return;
	}

	float nowSpeed = (In_isStartCam == true)? GData()->GetGlobalData()->_playableEventStartCameraBlendingSpeed :
		GData()->GetGlobalData()->_playableEventEndCameraBlendingSpeed;

	In_rotationWork->SetNowSpeed(nowSpeed);
	In_locationWork->SetNowSpeed(nowSpeed);

	_currentWorkSpeed = nowSpeed;
}