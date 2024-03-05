#include "GsCameraModeControlBase.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "GameObject/Camera/Work/Offset/GsCameraOffsetGame.h"
#include "GameObject/Camera/Work/Offset/GsCameraTargetOffsetGame.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationBase.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationQuarter.h"
#include "GameObject/Camera/Work/Zoom/GsCameraZoomGame.h"
#include "GameObject/Camera/Work/Location/GsCameraLocationGame.h"
#include "GameObject/Camera/Work/FOV/GsCameraFOVGame.h"
#include "GameObject/Camera/Work/DOF/GsCameraDOFGame.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsObserverManager.h"

#include "Message/GsMessageInput.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Input/GsInputBindingLocalPlayer.h"
#include "Input/GsInputEventMsgBase.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "ControllerEx/GsPlayerController.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "UTIL/GsClassUtil.h"
#include "UTIL/GsGameObjectUtil.h"

#include "T1Project.h"

#include "EngineMinimal.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Camera/PlayerCameraManager.h"

const FName CAM_BONE_CENTER = TEXT("Bip001");

// 처음 한번만 하는 초기화
void FGsCameraModeControlBase::Initialize()
{
	Super::Initialize();

	IGsCameraWork* fovWork = new FGsCameraFOVGame();
	fovWork->Initialize();
	_mapCamWorks.Add(EGsCameraWorkType::FOVWork, fovWork);

	IGsCameraWork* offsetWork = new FGsCameraOffsetGame();
	offsetWork->Initialize();
	_mapCamWorks.Add(EGsCameraWorkType::OffsetWork, offsetWork);

	IGsCameraWork* zoomWork = new FGsCameraZoomGame();
	zoomWork->Initialize();
	zoomWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::ZoomWork, zoomWork);

	IGsCameraWork* locationWork = new FGsCameraLocationGame();
	locationWork->Initialize();
	locationWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::LocationWork, locationWork);

	IGsCameraWork* targetOffsetWork = new FGsCameraTargetOffsetGame();
	targetOffsetWork->Initialize();
	_mapCamWorks.Add(EGsCameraWorkType::TargetOffsetWork, targetOffsetWork);
}
// 모드 초기화(데이터들)
void FGsCameraModeControlBase::InitMode(
	AGsCharacterLocalPlayer* In_localChar, const FGsSchemaCameraModeData* In_modeData)
{
	Super::InitMode(In_localChar, In_modeData);

	if (nullptr == In_modeData ||
		nullptr == In_localChar)
	{
		return;
	}

	In_localChar->GetSpringArm()->bUsePawnControlRotation = true;
	In_localChar->GetSpringArm()->bInheritPitch = true;
	In_localChar->GetSpringArm()->bInheritRoll = true;
	In_localChar->GetSpringArm()->bInheritYaw = true;

	In_localChar->bUseControllerRotationYaw = false;

	In_localChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	In_localChar->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	In_localChar->GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// 터치 땟다고 침...(나중에 터치중에 변경이 가능하거나, 전역적인 데이터 있으면 그걸로 씀...)
	_bTouchOn = false;

	if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
	{
		// 너무 줌이 되는 불상사가 일어나서 최소한의 min값을 둔다
		float checkMin = GData()->GetGlobalData()->_cameraModeZoomMin;
		float min = FMath::Max(In_modeData->CamArmLengthMin, checkMin);
		zoomWork->InitMode(min, In_modeData->CamArmLengthMax);
	}

}
void FGsCameraModeControlBase::Enter()
{
	Super::Enter();

	FGsMessageHolder* message = GMessage();
	_arrayInputMessageDelegates.Emplace(
		message->GetInput().AddRaw(MessageInput::TouchPress, this, &FGsCameraModeControlBase::OnTouchPress));

	_arrayInputMessageDelegates.Emplace(
		message->GetInput().AddRaw(MessageInput::TouchRelease, this, &FGsCameraModeControlBase::OnTouchRelease));

	_arrayInputMessageDelegates.Emplace(
		message->GetInput().AddRaw(MessageInput::PressJoystick, this, &FGsCameraModeControlBase::OnPressJoystick));
}

void FGsCameraModeControlBase::Exit()
{
	Super::Exit();

	ReleaseMessageDelegate();

}
// 터치 시작
void FGsCameraModeControlBase::TouchPress()
{
	_bTouchOn = true;
}
// 터치 끝
void FGsCameraModeControlBase::TouchRelease(bool In_isSimpleTouch)
{
	_bTouchOn = false;
}

// 모드 소켓 오프셋 시작
void FGsCameraModeControlBase::StartModeSocketOffset(bool In_isDirect)
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
		if (In_isDirect == true)
		{
			offsetWork->SetDirectSocketOffset(camModeData->SocketOffset);
		}
		else
		{
			offsetWork->StartModeSocketOffset(
				localChar->GetSpringArm()->SocketOffset, camModeData->SocketOffset);
		}
	}
}

void FGsCameraModeControlBase::StartTargetSocketOffset(bool In_isDirect, FVector In_targetOffset)
{
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	if (FGsCameraTargetOffsetGame* targetOffsetWork = GetCameraTargetOffsetGame())
	{
		if (In_isDirect == true)
		{
			targetOffsetWork->SetDirectTargetOffset(In_targetOffset);
		}
		else
		{
			targetOffsetWork->StartModeTargetOffset(
				localChar->GetSpringArm()->TargetOffset, In_targetOffset);
		}
	}
}

// 그리팅 끝 호출
void FGsCameraModeControlBase::OnGreetingOff(bool In_isDirect)
{
	Super::OnGreetingOff(In_isDirect);

	// 최초 이동은 모드 줌처리함
	// 여기서 줌체크 해준다
	// 1. fov 변환
	// 2. dof 변환
	EGsCameraChangeZoomType setZoomType = (In_isDirect == true) ?
		EGsCameraChangeZoomType::NoLerp : EGsCameraChangeZoomType::ModeChange;
	StartModeZoom(setZoomType);


	FGsCameraZoomGame* zoomWork = GetCameraZoomGame();

	if (nullptr == zoomWork)
	{
		return;
	}
	if (In_isDirect == false)
	{
		zoomWork->SetIsGreetingOffZoom(true);
	}
	SetGreetingOffDof();
	SetGreetingOffFov();

	// 오프셋 변환 시작
	StartModeSocketOffset(In_isDirect);

	// 카메라 오프셋 변환
	StartCameraOffset(In_isDirect);

	// focal region 세팅
	EGsCameraMode nowMode = GetType();
	if (const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode))
	{
		SetModeMobileFocalRegion(camModeData);
	}
}
// 줌인(길이 짧아짐)
void FGsCameraModeControlBase::ZoomIn(float In_mulVal)
{
	// 이전에 그리팅이었나
	if (true == _isBeforeGreeting)
	{
		_isBeforeGreeting = false;
		GreetingOff();
	}
	else
	{
		if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
		{
			zoomWork->ZoomIn(In_mulVal);
		}
	}
}

// 줌아웃(길이 길어짐)
void FGsCameraModeControlBase::ZoomOut(float In_mulVal)
{
	// 이전에 그리팅이었나
	if (true == _isBeforeGreeting)
	{
		_isBeforeGreeting = false;
		GreetingOff();
	}
	else
	{
		if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
		{
			zoomWork->ZoomOut(In_mulVal);
		}
	}
}

// 그리팅 뷰 세팅
void FGsCameraModeControlBase::SetGreetingView()
{
	// 캐릭터 회전 반대방향 구하기

	if (nullptr == _localPlayer ||
		nullptr == _targetPlayer)
	{
		return;
	}

	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	AGsCharacterBase* targetChar = _targetPlayer->GetCharacter();
	if (localChar == nullptr ||
		targetChar == nullptr)
	{
		GSLOG(Error, TEXT("localChar == nullptr || targetChar == nullptr"));
		return;
	}
	FVector fwdVec =
		targetChar->GetActorForwardVector();

	FVector oppVec = fwdVec * -1.0f;

	FRotator inverseRot = oppVec.Rotation();

	// pitch
	inverseRot += GData()->GetGlobalData()->CamAddGreetingRotator;

#ifdef TEST_LOG
	FRotator nowRot = localChar->GetActorRotation();
	GSLOG(Log, TEXT(
		"nowRot: %s, inverseRot: %s"),
		*nowRot.ToString(),
		*inverseRot.ToString());
#endif

	// 회전
	if (localChar->Controller &&
		localChar->Controller->IsLocalPlayerController())
	{
		APlayerController* const PC =
			CastChecked<APlayerController>(localChar->Controller);

		if (nullptr != PC)
		{

			// viewpitch min, max 값 수정(모드값은 짤리는 값도 있어서)
			// min, max가 너무 크면 복구되는 시점에 이상현상 나옴(ex: 정수리 보다가 백뷰로 돌아가지 못하는 버그)
			// 다음 모드의 min, max를 쓰되 정면샷이 초과된값이면 그값으로 min, max값 세팅


			EGsCameraMode nowMode = GetType();
			const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

			if (camModeData == nullptr)
			{
				return;
			}
			// 모드의 최소, 최대값
			float pitchMin = camModeData->ViewPitchMin;
			float pitchMax = camModeData->ViewPitchMax;

			// 모드최소값 보다 작으면
			if (pitchMin > inverseRot.Pitch)
			{
				// 모드값을 대체
				pitchMin = inverseRot.Pitch;
			}
			// 모드최대값 보다 크면
			else if(pitchMax < inverseRot.Pitch)
			{
				// 모드값을 대체
				pitchMax = inverseRot.Pitch;
			}

			PC->PlayerCameraManager->ViewPitchMin = pitchMin;
			PC->PlayerCameraManager->ViewPitchMax = pitchMax;

			PC->SetControlRotation(inverseRot);
		}
	}


	SetGreetingSocketOffest();


	if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
	{
		zoomWork->SetGreetingView();
	}

	// fov 세팅
	if (FGsCameraFOVGame* fovWork = GetCameraFOVGame())
	{			
		// 그리팅 전용 fov
		float greetingFOV =
			GData()->GetGlobalData()->_cameraGreetingFOV;
		fovWork->SetDirectFOV(greetingFOV);
	}
	

	// dof 세팅
	if (FGsCameraDOFGame* dofGame = GetCameraDOFGame())
	{
		float focalRegionMobile =
			GData()->GetGlobalData()->_cameraGreetingDepthOfFieldFocalRegionMobile;
		float scale = GData()->GetGlobalData()->_cameraDepthOfFieldScaleMax;

		float focalDistance =
			GData()->GetGlobalData()->_cameraGreetingDepthOfFieldFocalDistance;

		dofGame->SetDirectDOF(focalRegionMobile, scale, true, focalDistance);
	}


	// 그리팅이었는지
	_isBeforeGreeting = true;

}

// 그리팅 소켓 오프셋 계산
void FGsCameraModeControlBase::SetGreetingSocketOffest()
{
	if (nullptr == _targetPlayer ||
		nullptr == _localPlayer)
	{
		return;
	}

	AGsCharacterBase* targetChar = _targetPlayer->GetCharacter();
	AGsCharacterLocalPlayer* localChar =  _localPlayer->GetLocalCharacter();
	if (targetChar == nullptr ||
		localChar == nullptr)
	{
		GSLOG(Error, TEXT("targetChar == nullptr || localChar == nullptr"));
		return;
	}


	FVector addOffset = GData()->GetGlobalData()->CamAddGreetingSocketOffset;


	float heightVal = GSCameraMode()->GetSavedCameraTargetBoneHeight();
	float calcZ = addOffset.Z + heightVal;

	// add pos
	localChar->GetSpringArm()->SocketOffset = FVector(addOffset.X, addOffset.Y, calcZ);
}

// 모드줌 시작
void FGsCameraModeControlBase::StartModeZoom(EGsCameraChangeZoomType In_type)
{	
	FGsCameraZoomGame* zoomWork = GetCameraZoomGame();

	if (nullptr == zoomWork)
	{
		return;
	}
	// 이전에 백업해놓게 있다면 그걸로 쓴다
	// 하지만 시공의 틈새 모드면 모드 데이터만 쓴다
	float zoomVal = GetStartModeZoom();	
	zoomWork->StartModeZoom(zoomVal, In_type);	
}
// 시작 모드 줌 값 얻어오기
float FGsCameraModeControlBase::GetStartModeZoom()
{
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		return 0.0f;
	}

	float zoomVal = 0.0f;


	if (UWorld* world = GLevel()->GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			// not lobby mode -> playable event init called(irregular state)
			// https://sentry.io/organizations/com2us-raon/issues/3772032378/?project=4504115468566528&query=is%3Aunresolved+release%3A%22Android+Shipping+0.102602%22&referrer=issue-stream&sort=date&statsPeriod=14d
			if (gameMode->GetGameModeType() == EGsGameModeType::World)
			{
				if (false == GSCameraMode()->GetBackupCamZoom(nowMode, zoomVal))
				{
					zoomVal = camModeData->ArmLengthTo;
				}				
			}
			else
			{
				zoomVal = camModeData->ArmLengthTo;
			}
		}
	}

	return zoomVal;
}
// 현재 줌값 구하기
float FGsCameraModeControlBase::GetCurrentZoom()
{
	if (_isBeforeGreeting == false)
	{
		AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
		if (localChar == nullptr)
		{
			return 0.0f;
		}
		return localChar->GetSpringArm()->TargetArmLength;
	}
	
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);
	if (nullptr == camModeData)
	{
		return 0.0f;
	}

	return	camModeData->ArmLengthTo;
}

// 현재 회전 구하기(백업시 사용)
FQuat FGsCameraModeControlBase::GetCurrentQuat()
{
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return FQuat::Identity;
	}
	AGsPlayerController* controller = localChar->GetPlayerController();
	if (controller == nullptr)
	{
		return FQuat::Identity;
	}
	FRotator currRot = controller->GetControlRotation();
	// 그리팅이면 기본 데이터로 저장해란 뜻이다 일껄
	if (_isBeforeGreeting == true)
	{
		EGsCameraMode nowMode = GetType();
				
		// backup pitch 구하기
		// 1. backup 값
		// 2. 없으면 기본 모드값
		// 3. min, max 검증 까지 함
		float checkedControllerPitch = GSCameraMode()->GetModeBackupCheckedPitch(nowMode);
			
		currRot.Pitch = checkedControllerPitch;
	}

	return currRot.Quaternion();
}

// 일반적인 모드 전환
void FGsCameraModeControlBase::ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData)
{
	Super::ChangeModeNormal(In_modeData);

	// 이전에 대화있었냐에 따라 회전
	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetIsAfterDialog(false);
		rotationWork->SetIsUseFinishCallback(false);
	}

	// 모드 줌 시작
	StartModeZoom();

	// 오프셋 변환 시작
	StartModeSocketOffset(false);

	// 카메라 오프셋 변환
	StartCameraOffset(false);



}
// 모드 focal region 세팅
void FGsCameraModeControlBase::SetModeMobileFocalRegion(const FGsSchemaCameraModeData* In_modeData)
{
	// focal region 세팅
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr ||
		In_modeData == nullptr)
	{
		return;
	}

	// 모드별 mobile dof focal region 세팅(모드별 최소 줌이 달라서 따로 세팅)
	UCameraComponent* camComponent = localChar->GetFollowCamera();
	if (camComponent == nullptr)
	{
		return;
	}

	camComponent->PostProcessSettings.DepthOfFieldFocalRegion =
		In_modeData->ZoomMinDepthOfFieldFocalRegionMobile;
}
void FGsCameraModeControlBase::ChangeModeDialogEnd()
{
	Super::ChangeModeDialogEnd();

	FGsCameraRotationBase* rotationWork = GetCameraRotationBase();
	FGsCameraLocationGame* locationWork = GetCameraLocationGame();
	FGsCameraFOVGame* fovWork = GetCameraFOVGame();

	if (rotationWork == nullptr ||
		locationWork == nullptr ||
		fovWork == nullptr)
	{
		return;
	}

	locationWork->SetLocationSpace(EGsCameraLocationSpaceType::WorldLocation);

	// 컷 페이드 사용하면 lerp 사용안하고 바로 세팅
	bool isDirect = (GSCameraMode()->GetIsUseCutFadeOut() == true) ? true : false;


	if (isDirect == true)
	{
		FRotator backupRot =
			GSCameraMode()->GetBackupCamRotation();

		FVector bakupPos =
			GSCameraMode()->GetBackupCamLocation();
		// 세팅
		rotationWork->SetRotation(backupRot);
		locationWork->SetDirectPos(bakupPos);

		OnFinishLerpMode();
	}
	else
	{
		// check use zoom lerp
		bool isUseDialogLerpZoom =
			GSCameraMode()->GetIsZoomFirstCameraMove();

		if (isUseDialogLerpZoom == false)
		{
			fovWork->SetIsAfterDialog(true);

			rotationWork->SetIsAfterDialog(true);
			rotationWork->SetIsUseFinishCallback(true);

			rotationWork->SetRotationType(EGsCameraRotationType::WorldRotation);

			UWorld* world = _targetPlayer->GetWorld();
			FRotator camRot = FRotator::ZeroRotator;
			FVector camLoc = FVector::ZeroVector;

			if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
			{
				APlayerCameraManager* camManager = controller->PlayerCameraManager;
				camRot = camManager->GetCameraRotation();
				camLoc = camManager->GetCameraLocation();
			}

			FRotator backupRot =
				GSCameraMode()->GetBackupCamRotation();

			rotationWork->SetTargetRot(
				backupRot,
				camRot
			);

			locationWork->SetIsAfterDialog(true);
			locationWork->SetIsUseFinishCallback(true);

			FVector bakupPos =
				GSCameraMode()->GetBackupCamLocation();

			AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
			if (localChar == nullptr)
			{
				return;
			}

			float lastCamDist =
				localChar->GetSpringArm()->TargetArmLength;

			float backupDialogCamDist =
				GSCameraMode()->GetBakcupDialogCamDist();
			FVector backupBeforeDialogLookPos =
				GSCameraMode()->GetBackupBeforeDialogLookPos();

			FVector backupDialogLookPos =
				GSCameraMode()->GetBackupDialogLookPos();

			locationWork->SetData(lastCamDist, backupDialogCamDist,
				backupBeforeDialogLookPos,
				backupDialogLookPos,
				bakupPos,
				camLoc);


			MakeDialogSpeed(locationWork, rotationWork);
		}
		else
		{
			fovWork->SetIsAfterDialog(true);

			rotationWork->SetIsAfterDialog(true);
			rotationWork->SetIsUseFinishCallback(true);

			rotationWork->SetRotationType(EGsCameraRotationType::WorldRotation);

			UWorld* world = _targetPlayer->GetWorld();
			FRotator camRot = FRotator::ZeroRotator;
			FVector camLoc = FVector::ZeroVector;

			if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
			{
				APlayerCameraManager* camManager = controller->PlayerCameraManager;
				camRot = camManager->GetCameraRotation();
				camLoc = camManager->GetCameraLocation();
			}

			FRotator backupRot =
				GSCameraMode()->GetBackupCamRotation();

			rotationWork->SetTargetRot(
				backupRot,
				camRot
			);

			locationWork->SetIsAfterDialog(true);
			locationWork->SetIsUseFinishCallback(true);

			FVector bakupPos =
				GSCameraMode()->GetBackupCamLocation();

			AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
			if (localChar == nullptr)
			{
				return;
			}

			float lastCamDist =
				localChar->GetSpringArm()->TargetArmLength;

			float backupDialogCamDist =
				GSCameraMode()->GetBakcupDialogCamDist();

			FVector backupBeforeDialogLookPos =
				GSCameraMode()->GetBackupBeforeDialogLookPos();

			FVector backupDialogLookPos =
				GSCameraMode()->GetBackupDialogLookPos();

			locationWork->SetData(backupDialogCamDist, backupDialogCamDist,
				backupBeforeDialogLookPos,
				backupDialogLookPos,
				bakupPos,
				camLoc);

			MakeDialogSpeed(locationWork, rotationWork);


			if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
			{
				float diffCamDist = backupDialogCamDist - lastCamDist;

				FVector moveFinshedCamPos = backupBeforeDialogLookPos - backupDialogCamDist * backupRot.Vector();
				zoomWork->PosZoomStart(diffCamDist, moveFinshedCamPos,backupRot.Vector(), false);
			}
		}

	}
}

// 보간 끝(백뷰, 쿼터)
void FGsCameraModeControlBase::OnFinishLerpMode()
{
	bool isUseDialogLerpZoom =
		GSCameraMode()->GetIsZoomFirstCameraMove();

	if (isUseDialogLerpZoom == false)
	{
		if (FGsCameraModeHandler* cameraModehandler = _localPlayer->GetCameraModeHandler())
		{
			cameraModehandler->OnFinishDialogBackupLerp();
		}
	}
	else
	{
		if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
		{
			zoomWork->SetIsAfterDialog(true);
			zoomWork->SetIsZoomStart(true);
		}
	}

	FGsCameraRotationBase* rotationWork = GetCameraRotationBase();
	FGsCameraLocationGame* locationWork = GetCameraLocationGame();
	FGsCameraFOVGame* fovWork = GetCameraFOVGame();

	if (rotationWork == nullptr ||
		locationWork == nullptr ||
		fovWork == nullptr)
	{
		return;
	}
	// 사용후 초기화
	locationWork->SetIsAfterDialog(false);
	locationWork->SetIsUseFinishCallback(false);

	rotationWork->SetIsAfterDialog(false);
	rotationWork->SetIsUseFinishCallback(false);

	fovWork->SetIsAfterDialog(false);
}

// 카메라 오프셋 처리 접근
FGsCameraOffsetGame* FGsCameraModeControlBase::GetCameraOffsetGame()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::OffsetWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraOffsetGame* offsetWork = FGsClassUtil::FCheckStaticCast<FGsCameraOffsetGame>(work);
	return offsetWork;
}
FGsCameraTargetOffsetGame* FGsCameraModeControlBase::GetCameraTargetOffsetGame()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::TargetOffsetWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraTargetOffsetGame* targetOffsetWork = FGsClassUtil::FCheckStaticCast<FGsCameraTargetOffsetGame>(work);
	return targetOffsetWork;
}

// 카메라 회전 처리 접근
FGsCameraZoomGame* FGsCameraModeControlBase::GetCameraZoomGame()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::ZoomWork);
	if (work == nullptr)
	{
		return nullptr;
	}


	FGsCameraZoomGame* zoomWork = FGsClassUtil::FCheckStaticCast<FGsCameraZoomGame>(work);
	return zoomWork;
}

// 다른맵 스폰 초기화
void FGsCameraModeControlBase::InitOtherMapSpawn(const FGsSchemaCameraModeData* In_modeData)
{
	if (nullptr == In_modeData)
	{
		return;
	}

	// 모드 줌 시작
	StartModeZoom(EGsCameraChangeZoomType::SpawnStart);

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

// 같은맵 스폰 초기화
void FGsCameraModeControlBase::InitSameMapSpawn(const FGsSchemaCameraModeData* In_modeData)
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

// 모드 회전 구하기
FRotator FGsCameraModeControlBase::GetModeRot(const FGsSchemaCameraModeData* In_modeData)
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
		controlRotTo = camModeManager->GetBackupQuat().Rotator();
	}
	else
	{				
		EGsCameraMode nowMode = GetType();
		float checkedControllerPitch =
			GSCameraMode()->GetModeBackupCheckedPitch(nowMode);
		controlRotTo.Pitch = checkedControllerPitch;
	}
	return controlRotTo;
}

// fov 변환 시작
void FGsCameraModeControlBase::StartModeFOV()
{
	// 컷 페이드 사용하면 lerp 사용안하고 바로 세팅
	bool isDirect = (GSCameraMode()->GetIsUseCutFadeOut() == true) ? true : false;

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

		float nowModeFOV = GetNowModeFOV();
		if (isDirect == true)
		{
			fovWork->SetDirectFOV(nowModeFOV);
		}
		else
		{
			
			fovWork->StartModeFOV(
				PC->PlayerCameraManager->GetFOVAngle(), camModeData->FOV);
			fovWork->SetNowSpeed(_currentWorkSpeed);
		}
	}
}

// 카메라 회전 쿼터 처리 접근
FGsCameraRotationQuarter* FGsCameraModeControlBase::GetCameraRotationQuarter()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::RotationWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraRotationQuarter* rotationWork = FGsClassUtil::FCheckStaticCast<FGsCameraRotationQuarter>(work);
	return rotationWork;
}

// 카메라 offset 이동 (키, shift)
void FGsCameraModeControlBase::StartCameraOffset(bool In_isDirect)
{

	if (nullptr == _targetPlayer ||
		nullptr == _localPlayer)
	{
		return;
	}

	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	AGsCharacterBase* targetChar = _targetPlayer->GetCharacter();
	if (localChar == nullptr ||
		targetChar == nullptr)
	{
		GSLOG(Error, TEXT("localChar == nullptr || targetChar == nullptr"));
		return;
	}

	// 1. 키값(본 기준)
	// 바로 계산인지
	bool isDirectCalc = false;

	float heightVal = 0.0f;

	if (UWorld* world = GLevel()->GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			// 로비(플레이어블 이벤트) 이거나
			// 관전자 모드일때면 바로 계산
			// or test client
			if (gameMode->GetGameModeType() == EGsGameModeType::Lobby ||
				gameMode->GetGameModeType() == EGsGameModeType::TestClient)
			{				
				// male, default(50)
				heightVal = UGsGameObjectUtil::GetCameraBoneHeight(CreatureGenderType::MALE);
			}
			else
			{
				if (FGsObserverManager* observerManager = GSObserver())
				{
					if (observerManager->GetIsObserveStart() == true)
					{
						// 관전은 remote라서 계산 필요
						if (UGsGameObjectBase* camTarget = GetCameraTargetPlayer())
						{	
							// by remote data
							heightVal =
								UGsGameObjectUtil::GetCameraBoneHeightByGameObjectPlayer(camTarget);
						}
						
					}
					else
					{
						// 그외는 로컬꺼이므로 로비에서 계산한걸로 씀(키 적용 딜레이 때문에)
						heightVal = GSCameraMode()->GetSavedCameraTargetBoneHeight();
					}
				}

			}
		}
	}

	
	// 2. shift 값(모드별)
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		return;
	}
	FVector shiftPos =  camModeData->CamShiftPos;

	FVector totalMovePos = FVector(shiftPos.X, shiftPos.Y, shiftPos.Z + heightVal);

#ifdef OLD_CAM_SHIFT
	UCameraComponent* camComponent = localChar->GetFollowCamera();
	if (camComponent == nullptr)
	{
		return;
	}


	// 바로면
	if (In_isDirect == true)
	{
		camComponent->SetRelativeLocation(totalMovePos);
	}
	// 바로 아니면 work를 써서 lerp하게 함
	else
	{
		FGsCameraLocationGame* locationWork = GetCameraLocationGame();
		if (locationWork == nullptr)
		{
			return;
		}

		FVector currentCamPos = camComponent->GetRelativeLocation();
		locationWork->SetLocationSpace(EGsCameraLocationSpaceType::RelativeLocation);
		locationWork->SetDataNormal( totalMovePos, currentCamPos);
	}
#else
	StartTargetSocketOffset(In_isDirect, totalMovePos);
#endif
}

// 대화 연출 복구 완료 되었음
void FGsCameraModeControlBase::OnFinishDialogBackupLerp()
{
	// 카메라 offset 이동 처리
	StartCameraOffset(true);

	if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
	{
		// 위치 복구후 dof 처리 복구한다
		zoomWork->CheckZoomMin(true);
	}
}

// zoom 에의한 fov 변경
void FGsCameraModeControlBase::OnReachZoomFOV(bool In_isReached)
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
		// 윗단에서 사용하는 모드인지 체크해서 온값임
		// FGsCameraModeBase::OnReachZoomMin
		float settingFOV = (In_isReached == true) ? camModeData->ZoomMinFOV : camModeData->FOV;
		float oldFOV = PC->PlayerCameraManager->GetFOVAngle();
		
		// 1. check fov work is working && old goal, now goal 		
		// https://jira.com2us.com/jira/browse/CHR-15704
		if (true == fovWork->GetIsFOVChanging())
		{
			if (fovWork->GetGoalFOV() == settingFOV)
			{
				return;
			}
		}
		else
		{
			// 1. not fov changing
			// 2. check current value, goal value
			if (oldFOV == settingFOV)
			{
				return;
			}
		}
		

		fovWork->StartModeFOV(
			oldFOV, settingFOV);
		float speed = GData()->GetGlobalData()->_cameraZoomMinLerpSpeed;
		fovWork->SetNowSpeed(speed);
		
	}
}

// 현재 모드 fov(줌 체크)
float FGsCameraModeControlBase::GetNowModeFOV()
{
	bool isMinZoom = false;
	if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
	{
		isMinZoom = zoomWork->IsMinZoom();
	}

	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		return 0.0f;
	}
	// 모드 사용과 실제 min이면 min fov 아니면 기본 모드 fov
	return (isMinZoom == true && camModeData->UseZoomMinFOV) ? camModeData->ZoomMinFOV : camModeData->FOV;
}

void FGsCameraModeControlBase::ChangeMode(const FGsSchemaCameraModeData* In_modeData)
{
	Super::ChangeMode(In_modeData);
	// focal region 세팅
	SetModeMobileFocalRegion(In_modeData);
}

void FGsCameraModeControlBase::OnTouchPress(const FGsInputEventMsgBase& InMsg)
{
	TouchPress();
}
void FGsCameraModeControlBase::OnTouchRelease(const FGsInputEventMsgBase& InMsg)
{
	if (FGsInputEventMsgTouchRelease* touchReleaseMsg = FGsClassUtil::FCheckStaticCast<FGsInputEventMsgTouchRelease>((FGsInputEventMsgBase*)&InMsg))
	{
		TouchRelease(touchReleaseMsg->IsSimpleTouch);
	}
}

void FGsCameraModeControlBase::ReleaseMessageDelegate()
{
	FGsMessageHolder* msg = GMessage();

	if (_arrayInputMessageDelegates.Num() != 0)
	{
		for (auto iter : _arrayInputMessageDelegates)
		{
			msg->GetInput().Remove(iter);
		}
		_arrayInputMessageDelegates.Empty();
	}
}

void FGsCameraModeControlBase::Close()
{
	Super::Close();

	ReleaseMessageDelegate();
}

void FGsCameraModeControlBase::OnPressJoystick(const FGsInputEventMsgBase& InMsg)
{
	PressJoystick();
}

void FGsCameraModeControlBase::PredictTransform(FRotator& Out_rotation, FVector& Out_pos)
{
	FVector currCamLocation = FVector::ZeroVector;
	FRotator currCamRotation = FRotator::ZeroRotator;

	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	if (AGsPlayerController* controller = localChar->GetPlayerController())
	{
		APlayerCameraManager* camManager = controller->PlayerCameraManager;
		if (camManager != nullptr)
		{
			currCamLocation = camManager->GetCameraLocation();
			currCamRotation = camManager->GetCameraRotation();
		}
	}
	FGsCameraRotationBase* rotBase = GetCameraRotationBase();
	bool rotChange = false;
	if (rotBase != nullptr)
	{
		// now rotating
		if (true == rotBase->IsRotationPossible())
		{
			rotChange = true;
			// calc goal rotation
			currCamRotation = rotBase->GetGoalRotator();
		}
	}

	UCameraComponent* camComponent = localChar->GetFollowCamera();
	if (camComponent == nullptr)
	{
		return;
	}


	FGsCameraZoomGame* zoomGame = GetCameraZoomGame();
	bool posChange = false;
	if (zoomGame != nullptr)
	{
		// now zooming
		if (true == zoomGame->IsZoomStart())
		{
			posChange = true;
			float nowCamDist =
				localChar->GetSpringArm()->TargetArmLength;

			FVector rotVec = currCamRotation.Vector();

			float goalZoom = zoomGame->GetGoalZoom();

			FVector currentCamPos = camComponent->GetRelativeLocation();
			float heightVal = GSCameraMode()->GetSavedCameraTargetBoneHeight();

			EGsCameraMode nowMode = GetType();
			const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

			if (camModeData == nullptr)
			{
				return;
			}
			FVector shiftPos = camModeData->CamShiftPos;

			FVector totalMovePos = FVector(shiftPos.X, shiftPos.Y, shiftPos.Z + heightVal);

			// calc goal pos
			// current pos + (nowZoom - newZoom) + (newShift - nowShift)
			currCamLocation = currCamLocation + ((nowCamDist - goalZoom) * rotVec) + (totalMovePos - currentCamPos);
		}
	}

//#if WITH_EDITOR
//	GSLOG(Error, TEXT("PredictTransform -- rotChange[%d]: currCamRotation[%s], posChange[%d]: currCamLocation[%s] "), 
//		(int)rotChange, *currCamRotation.ToString(),
//		(int)posChange, *currCamLocation.ToString());
//#endif
}

void FGsCameraModeControlBase::PostChangeModeNormal()
{
	//FRotator rotation;
	//FVector position;
	//PredictTransform(rotation, position);
}

// 줌 종료 콜백
void FGsCameraModeControlBase::OnZoomFinished()
{
	if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
	{
		if (zoomWork->GetIsAfterDialog() == true)
		{
			zoomWork->SetIsAfterDialog(false);
			if (FGsCameraModeHandler* cameraModehandler = _localPlayer->GetCameraModeHandler())
			{
				cameraModehandler->OnFinishDialogBackupLerp();
			}			
		}

		if (zoomWork->GetIsGreetingOffZoom() == true)
		{
			zoomWork->SetIsGreetingOffZoom(false);

			float zoomVal = GetStartModeZoom();
			EGsCameraMode nowMode = GetType();
			const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);
			if (camModeData == nullptr)
			{
				return;
			}

			bool isDofOn = false;
			if (camModeData->UseZoomMinDOF && camModeData->CamArmLengthMin >= zoomVal)
			{
				isDofOn = true;
			}

			if (FGsCameraDOFGame* dofGame = GetCameraDOFGame())
			{
				dofGame->SetDofOption(isDofOn);
			}
		}
	}
}

void FGsCameraModeControlBase::SetGreetingOffDof()
{
	float zoomVal = GetStartModeZoom();
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);
	if (camModeData == nullptr)
	{
		return;
	}
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
	
	bool isDofOn = false;
	if (camModeData->UseZoomMinDOF && camModeData->CamArmLengthMin >= zoomVal)
	{
		isDofOn = true;
	}
	// min 에도달하면 scale max로 
	// min 아니면 0으로
	float scaleTo = (isDofOn == true) ?
		GData()->GetGlobalData()->_cameraDepthOfFieldScaleMax :
		0.0f;

	if (FGsCameraDOFGame* dofGame = GetCameraDOFGame())
	{		
		dofGame->StartDOF(scaleFrom, scaleTo, isDofOn);
	}
}

void FGsCameraModeControlBase::SetGreetingOffFov()
{
	float zoomVal = GetStartModeZoom();
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);
	if (camModeData == nullptr)
	{
		return;
	}
	bool isFovOn = false;
	if (camModeData->UseZoomMinFOV && camModeData->CamArmLengthMin >= zoomVal)
	{
		isFovOn = true;
	}

	float settingFOV = (isFovOn == true) ? camModeData->ZoomMinFOV : camModeData->FOV;


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

	float oldFOV = PC->PlayerCameraManager->GetFOVAngle();

	if (FGsCameraFOVGame* fovWork = GetCameraFOVGame())
	{	
		fovWork->StartModeFOV(
			oldFOV, settingFOV);
		float speed = GData()->GetGlobalData()->_cameraZoomMinLerpSpeed;
		fovWork->SetNowSpeed(speed);
	}
}