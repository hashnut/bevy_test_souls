#include "GameObject/Camera/GsCameraModeHandler.h"

#include "GameObject/Camera/Mode/GsCameraModeBackView.h"
#include "GameObject/Camera/Mode/GsCameraModeQuarter.h"
#include "GameObject/Camera/Mode/GsCameraModeDialog.h"
#include "GameObject/Camera/Mode/GsCameraModeWarpStart.h"
#include "GameObject/Camera/Mode/GsCameraModeSpaceCrack.h"
#include "GameObject/Camera/Mode/GsCameraModeFixedQuarter.h"
#include "GameObject/Camera/Mode/GsCameraModePlayableEvent.h"
#include "GameObject/Camera/Mode/GsCameraModeEquipView.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Data/GsGameObjectData.h"

#include "GameObject/Movement/GsMovementHandlerBase.h"

#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "Camera/Define/EGsDialogCameraViewType.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/GsSchemaEnums.h"

#include "DataCenter/Public/DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "Message/MessageParam/GsCameraMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "UTIL/GsLevelUtil.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "Input/GsInputEventMsgBase.h"
#include "Input/GsInputBindingLocalPlayer.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsActorPlayableEventCameraData.h"

#include "ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "ActorComponentEx/GsSpringArmComponentEx.h"

#include "GameFlow/Stage/GsStageMode.h"

#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"
#include "GameFlow/GameContents/ContentsLobby/Hud/GsStateLobbyHud.h"

#include "GameFlow/GameContents/ContentsLobby/Hud/PlayableEvent/GsPlayableEventHandler.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/SkinnedMeshComponent.h"
#include "Engine/Classes/Engine/Scene.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
//------------------------------------------------
// 할당 하는 넘
//------------------------------------------------

FGsCameraModeBase* FGsCameraModeAllocator::Alloc(EGsCameraMode In_mode)
{
	if (In_mode == EGsCameraMode::BackView)
	{
		return new FGsCameraModeBackView();
	}
	else if (In_mode == EGsCameraMode::Quarter ||
		In_mode == EGsCameraMode::Boss)
	{
		return new FGsCameraModeQuarter();
	}
	else if (In_mode == EGsCameraMode::Dialog)
	{
		return new FGsCameraModeDialog();
	}
	else if (In_mode == EGsCameraMode::WarpStart)
	{
		return new FGsCameraModeWarpStart();
	}
	else if (In_mode == EGsCameraMode::SpaceCrack)
	{
		return new FGsCameraModeSpaceCrack();
	}
	else if (In_mode == EGsCameraMode::FixedQuarter)
	{
		return new FGsCameraModeFixedQuarter();
	}
	else if (In_mode == EGsCameraMode::PlayableEvent)
	{
		return new FGsCameraModePlayableEvent();
	}
	else if (In_mode == EGsCameraMode::EquipView)
	{
		return new FGsCameraModeEquipView();
	}
	return nullptr;
}


// 초기화
void FGsCameraModeHandler::Initialize(UGsGameObjectBase* In_local)
{
	// 핸들러는 initialize랑 캐릭터 세팅이랑 같은시점이다

	// 초기화
	TArray<EGsCameraMode> arrMode;

	arrMode.Add(EGsCameraMode::BackView);
	arrMode.Add(EGsCameraMode::Quarter);
	arrMode.Add(EGsCameraMode::Dialog);
	arrMode.Add(EGsCameraMode::WarpStart);
	arrMode.Add(EGsCameraMode::SpaceCrack);
	arrMode.Add(EGsCameraMode::FixedQuarter);
	arrMode.Add(EGsCameraMode::PlayableEvent);
	arrMode.Add(EGsCameraMode::EquipView);

	// 캐릭터 세팅
	_localPlayer = In_local->CastGameObject<UGsGameObjectLocalPlayer>();

	for (auto& iter : arrMode)
	{
		// 모드 생성
		auto makeObj = MakeInstance(iter);
		makeObj->Initialize();
		makeObj->SetHandler(this);
		// 로컬 등록
		makeObj->SetModeLocalObject(_localPlayer);
		// 카메라 대상 등록(처음은 둘다 로컬이다)
		makeObj->SetModeTarget(_localPlayer);
	}

	_directGreetingCheckNextTickCount =
		GData()->GetGlobalData()->_directGreetingNextTick;
}


void FGsCameraModeHandler::InitializeActor(class AActor* Owner)
{
	// 여기서부터는 원래 매니저에서 set character 로
	// init과 분리되어있던곳

	// 현재가 시공의 틈새 맵이면 모드를 시공의 틈새로 바꾼다

	bool isSpaceCrack = false;
	if (UGsLevelManager* levelMgr = GLevel())
	{
		if (true == levelMgr->IsSpaceCrackMapContents())
		{
			isSpaceCrack = true;
		}
	}
	EGsCameraMode newMode = EGsCameraMode::PlayableEvent;
	if (UWorld* world = GLevel()->GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() != EGsGameModeType::Lobby)
			{
				newMode = EGsCameraMode::Quarter;
				if (FGsCameraModeManager* camModeManager = GSCameraMode())
				{
					newMode = camModeManager->GetBackupMode();
				}
			}
		}
	}

	if (isSpaceCrack == true)
	{
		newMode = EGsCameraMode::SpaceCrack;
	}

	// 처음이거나 모드가 다르면
	if (CurrentState.IsValid() == false ||
		CurrentState.Pin()->GetType() != newMode)
	{
		// 바꿈
		ChangeState(newMode);
	}
	else
	{
		// 기존꺼랑 같으면
		// 초기화 해준다 
		// 워프, 사망시 아무런 세팅이 안되어있음....
		// exit는 안해도 될꺼 같지만... 해준다
		CurrentState.Pin()->Exit();
		CurrentState.Pin()->Enter();
	}
	// 원래는 OnCloseLoading 시점에 그리팅 시작했지만
	// 로딩 닫히고 난뒤 sub class 세팅하므로
	// 여기로 해보자
	// 스폰상태에서 처리할려고 했는데
	// 처음에 무비가 나오면 freeze 상태로가고 스폰 상태로 안감
	// 무조건 탈수 있는 여기로 다시 돌림
	OnGreeting();

	_isInitCharacter = true;

	InitializeMessage();

	// 첫스폰 모드 처리 끝
	_isSpawnInitModeChange = false;
}

// 해제
void FGsCameraModeHandler::Finalize()
{
	_isInitCharacter = false;

	_localPlayer = nullptr;

	TMap<EGsCameraMode, TSharedPtr<FGsCameraModeBase>> container = GetContainer();
	for (auto& iter : container)
	{
		iter.Value->Close();
	}	

	FinalizeMessage();
}


// 컨트롤러 possess이후 호출됨
void FGsCameraModeHandler::OnGreeting()
{
	// 로비에선 그리팅 필요 없음
	if (UWorld* world = GLevel()->GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::Lobby)
			{
				return;
			}
		}
	}
	//https://jira.com2us.com/jira/browse/CHR-15270
	// first quest dialog no greeting
	if (GSCameraMode()->GetIsGreetingView() == true &&
		CurrentState.IsValid())
	{
		GSCameraMode()->SetIsGreetingView(false);

		// 그리팅 값 세팅
		CurrentState.Pin()->SetGreetingView();

		// 그리팅 시작 메시지
		GMessage()->GetCamera().SendMessage(MessageContentCamera::GREETING_ON);
		
	}
}

void FGsCameraModeHandler::Update(float In_deltaTime)
{
	if (_isInitCharacter == false)
	{
		return;
	}

	if (CurrentState.IsValid())
	{
		CurrentState.Pin()->Update(In_deltaTime);
	}

	if (_isDirtyStartDialog == true)
	{		
		_isDirtyStartDialog = false;
			
		PostDirectGreetingDialogStart();		
	}
}
// 다음 스텝으로 진행
void FGsCameraModeHandler::NextStep()
{
	if (CurrentState.IsValid())
	{
		CurrentState.Pin()->NextStep(this);
	}
}

void FGsCameraModeHandler::ChangeState(TStateType In_state)
{
	Super::ChangeState(In_state);

	// 로비에선 빽업 필요 없음
	UGsLevelManager* levelMgr = GLevel();

	if (levelMgr == nullptr)
	{
		return;
	}

	if (UWorld* world = levelMgr->GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::Lobby)
			{
				return;
			}
		}
	}

	EGsCameraMode camMode = (EGsCameraMode)In_state;
	GSCameraMode()->SetCurrentMode(camMode);


	// 카메라 값 백업해놓기(유효한 상황에서만 백업/gameusersetting에 저장될 값)
	//카메라 회전
	//카메라 줌
	//모드 변경
	//back view 갱신시

	// 모드별 디폴트값으로 백업(gameusersetting)
	// 관전 모드에서는 저장 안함
	EGsCameraInitModeType initModeType = GetInitModeType();
	if (initModeType == EGsCameraInitModeType::ModeChange &&
		GSObserver()->GetIsObserveStart() == false)
	{
		GSCameraMode()->SaveModeBackupData(camMode);
	}
}

// 메시지 초기화
void FGsCameraModeHandler::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();
	
	_arrCameraDialogDelegate.Emplace(
		msg->GetGameObject().AddRaw(
			MessageGameObject::CAMERA_DIALOG_START, this,
			&FGsCameraModeHandler::OnCameraDialogStart)
	);
	_arrCameraDialogDelegate.Emplace(
		msg->GetGameObject().AddRaw(
			MessageGameObject::CAMERA_DIALOG_END, this,
			&FGsCameraModeHandler::OnCameraDialogEnd)
	);

	_arrCameraDialogDelegate.Emplace(
		msg->GetGameObject().AddRaw(
			MessageGameObject::CAMERA_DIALOG_MODE_VIEW_CHANGE, this,
			&FGsCameraModeHandler::OnCameraDialogModeViewChange)
	);

	_arrCameraDialogDelegate.Emplace(
		msg->GetGameObject().AddRaw(
			MessageGameObject::CAMERA_EQUIP_VIEW_START, this,
			&FGsCameraModeHandler::OnCameraEquipViewStart)
	);

	_arrCameraDialogDelegate.Emplace(
		msg->GetGameObject().AddRaw(
			MessageGameObject::CAMERA_EQUIP_VIEW_END, this,
			&FGsCameraModeHandler::OnCameraEquipViewEnd)
	);

	_arrCameraDialogDelegate.Emplace(
		msg->GetGameObject().AddRaw(
			MessageGameObject::LOCAL_WARP, this,
			&FGsCameraModeHandler::OnLocalWarp)
	);

	_systemMsg.Emplace(
		msg->GetSystemParam().AddRaw(MessageSystem::LOAD_TOPO_START, this,
			&FGsCameraModeHandler::OnLoadTopoStart));

	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::ReleaseJoystick, this,
			&FGsCameraModeHandler::OnReleaseJoystick));

	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::PressJoystick, this,
			&FGsCameraModeHandler::OnPressJoystick));

	_msgContentCameraDelegates.Emplace(
		msg->GetCamera().AddRaw(MessageContentCamera::CLICK_FIXED_QUARTER_YAW, this,
			&FGsCameraModeHandler::OnClickFixedQuarterYaw));
}

// 메시지 클리어
void FGsCameraModeHandler::FinalizeMessage()
{
	FGsMessageHolder* msg = GMessage();
	
	if (_arrCameraDialogDelegate.Num() != 0)
	{
		for (auto iter : _arrCameraDialogDelegate)
		{
			msg->GetGameObject().Remove(iter);
		}
		_arrCameraDialogDelegate.Empty();
	}

	if (_systemMsg.Num() != 0)
	{
		for (auto iter : _systemMsg)
		{
			msg->GetSystemParam().Remove(iter);
		}
		_systemMsg.Empty();
	}

	if (_actionDelegates.Num() != 0)
	{
		for (auto iter : _actionDelegates)
		{
			msg->GetInput().Remove(iter);
		}
		_actionDelegates.Empty();
	}
	if (_msgContentCameraDelegates.Num() != 0)
	{
		for (auto iter : _msgContentCameraDelegates)
		{
			msg->GetCamera().Remove(iter);
		}
		_msgContentCameraDelegates.Empty();
	}
}

void FGsCameraModeHandler::PostDirectGreetingDialogStart()
{
	for (auto iter : _camDialogMessage._arrDialogTargetObj)
	{
		if (iter == nullptr)
		{
			FString failTickerString = FString::Printf(TEXT("OnCameraDialogStart failed paramTarget->_arrDialogTargetObj is null"));
			FText failTickerText = FText::FromString(failTickerString);
			FGsUIHelper::TrayMessageTicker(failTickerText);
			FGsUIHelper::PopupOK(failTickerText);
			return;
		}
	}

	CloseEquipUI();

	bool isUseSpotIdPos = !_camDialogMessage._isCameraWalking;
	SetLocalUseSpotIdPos(isUseSpotIdPos);

	// 카메라 워킹이면 페이드 아웃 사용 안함
	GSCameraMode()->SetIsUseCutFadeOut(isUseSpotIdPos);
	GSCameraMode()->SetIsZoomFirstCameraMove(_camDialogMessage._isZoomFirstCameraMove);

	GSCameraMode()->SetDialogCameraViewData(_camDialogMessage._viewData);
	
	SetLocalPosition(_camDialogMessage._localSpotId, _camDialogMessage._arrDialogTargetObj);
	
	GSCameraMode()->SetCameraDialogEndCallbackType(_camDialogMessage._cameraDialogCallbackType);
	

	// 대상 저장
	SetDialogTarget(_camDialogMessage._arrDialogTargetObj);

	StartDialog();	

	SendEnvStartDialog();

	FGsUIHelper::TrayDimmed(false);
}

// 카메라 대화 연출 시작
void FGsCameraModeHandler::OnCameraDialogStart(const IGsMessageParam* In_param)
{
	// 이미 카메라 모드면 막자
	if (GSCameraMode()->GetCurrentMode() == EGsCameraMode::Dialog)
	{
		return;
	}

	const FGsCameraDialogMessageParam* paramTarget =
		In_param->Cast<const FGsCameraDialogMessageParam>();
	if (nullptr == paramTarget)
	{
		return;
	}

	if (paramTarget->_isGreetingOffForced == true &&
		CurrentState.IsValid() &&
		CurrentState.Pin()->GetIsBeforeGreeting() == true)
	{
		CurrentState.Pin()->GreetingOffForeced();

		_camDialogMessage = *paramTarget;

		_isDirtyStartDialog = true;
		_currentDirectGreetingNextTickCount = 0;
		return;
	}


	for (auto iter : paramTarget->_arrDialogTargetObj)
	{
		if (iter == nullptr)
		{
			FString failTickerString = FString::Printf(TEXT("OnCameraDialogStart failed paramTarget->_arrDialogTargetObj is null"));
			FText failTickerText = FText::FromString(failTickerString);
			FGsUIHelper::TrayMessageTicker(failTickerText);
			FGsUIHelper::PopupOK(failTickerText);
			return;
		}
	}

	CloseEquipUI();

	bool isUseSpotIdPos = !paramTarget->_isCameraWalking;
	SetLocalUseSpotIdPos(isUseSpotIdPos);

	// 카메라 워킹이면 페이드 아웃 사용 안함
	GSCameraMode()->SetIsUseCutFadeOut(isUseSpotIdPos);
	GSCameraMode()->SetIsZoomFirstCameraMove(paramTarget->_isZoomFirstCameraMove);	

	GSCameraMode()->SetDialogCameraViewData(paramTarget->_viewData);

	SetLocalPosition(paramTarget->_localSpotId, paramTarget->_arrDialogTargetObj);

	GSCameraMode()->SetCameraDialogEndCallbackType(paramTarget->_cameraDialogCallbackType);
	

	// 대상 저장
	SetDialogTarget(paramTarget->_arrDialogTargetObj);
	StartDialog();

	SendEnvStartDialog();


	if (_localPlayer != nullptr)
	{
		// https://jira.com2us.com/jira/browse/C2URWQ-2536
		// clear target 
		FGsTargetHandlerLocalPlayer* targetHandler = _localPlayer->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		if (targetHandler == nullptr)
		{
			return;
		}

		targetHandler->ClearTarget();

		// https://jira.com2us.com/jira/browse/C2URWQ-2476
		// clear touch move data
		UGsInputBindingLocalPlayer* inputBinder = _localPlayer->GetInputBinder();
		if (nullptr == inputBinder)
		{
			return;
		}
		inputBinder->ClearReserveTouchMoveData();
	}

	FGsUIHelper::TrayDimmed(false);
}

void FGsCameraModeHandler::GreetingOffDirect()
{
	if (CurrentState.IsValid() &&
		CurrentState.Pin()->GetIsBeforeGreeting() == true)
	{
		CurrentState.Pin()->GreetingOffForeced();
	}
}
// 카메라 대화 연출 끝
void FGsCameraModeHandler::OnCameraDialogEnd(const IGsMessageParam*)
{
	EndDialog();
}

// 대화 연출 시작
void FGsCameraModeHandler::StartDialog()
{

	// 카메라를 떼고 처리 해야함
	// 나중에 때면 이상한 각도에서 나옴(바로 세팅시)
	SetAttachFollowCamera(false);



	// 현재 회전값, 모드 저장
	// 시공의 틈새에서 대사 연출이 생기면
	// 시공의 틈새 전에 백업 데이터를 따로 구성해야한다
	GSCameraMode()->SaveBackUpTransform();

	// 상태 변경이 먼저다
	// 대사 모드에서 카메라 플레이 상태가 아니어야 ChangeModeNormal를 탄다
	// 플래그 위치가 위로 올라가면 ChangeModeDialogEnd 함수 이름 바꾸어서 거기서 처리
	ChangeState(EGsCameraMode::Dialog);

	// 대화 카메라 연출중(연출 시작부터 연출 끝까지)
	GSCameraMode()->SetIsCameraDialogPlay(true);
	GSCameraMode()->SetIsCameraDialogPlayAll(true);
	

	// 룩앳 시작
	StartLookatTarget(_arrDialogTargetObj);

	// 인풋 막기(줌, 회전)
	//FGsInputEventMsgBase msg;
	//msg.Id = 1;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_HUD_CAMERA_MODE, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	// ui hide
	if (UGsUIManager* uiManage = GUI())
	{
		uiManage->SetHideFlags(EGsUIHideFlags::STATE_DIALOG);
	}	
}

// 대화 연출 끝
void FGsCameraModeHandler::EndDialog()
{
	// 사용된적 있으면 on, 아니면 false
	GSCameraMode()->SetIsUseCutFadeOut(_isUsedSpotIdPos);

	if (_isUsedSpotIdPos == true)
	{
		SetLocalPosBackup();
		_isUsedSpotIdPos = false;
	}


	// 원래 모드로 돌아가기
	GSCameraMode()->ChangeToBackupMode();


	_arrDialogTargetObj.Empty();

	// 대화 카메라 연출중 해제(연출 시작부터 연출 끝까지)
	// https://jira.com2us.com/jira/browse/C2URWQ-2255
	// 처음 수정엔 lerpEnd에 했었는데
	// 로컬이 없는 상황에서 카메라 복구되고 꺼지니까 어색하다
	// 플래그를 카메라 복구전에 세팅
	GSCameraMode()->SetIsCameraDialogPlay(false);
}

void FGsCameraModeHandler::SaveCurrentLocalRotation()
{
	if (_localPlayer == nullptr )
	{
		return;
	}
	// 회전 저장
	_backupLocalRot = _localPlayer->GetRotation();
}

// 로컬이랑 타겟 서로 바라보게 처리
void FGsCameraModeHandler::StartLookatTarget(TArray<UGsGameObjectBase*> In_dialogTargets)
{
	if (_localPlayer == nullptr ||
		In_dialogTargets.Num() == 0)
	{
		return;
	}
	// 회전 저장
	//_backupLocalRot = _localPlayer->GetRotation();


	FVector lookPos = FVector::ZeroVector;
	// 1. 1명이면 그넘
	if (In_dialogTargets.Num() == 1)
	{
		if (In_dialogTargets[0] != nullptr)
		{			
			lookPos = In_dialogTargets[0]->GetLocation();
		}
	}
	// 2. 2명이면 그사이값
	else
	{
		if (In_dialogTargets[0] != nullptr &&
			In_dialogTargets[1] != nullptr)
		{
			lookPos =
				FMath::Lerp(
					In_dialogTargets[0]->GetLocation(), 
					In_dialogTargets[1]->GetLocation(), 0.5f);
		}
	}
	_localPlayer->LookAtPosition(lookPos);

	// 페어리 위치 보정, 회전
	GSClientSpawn()->RepositionDialogFairy(lookPos);


	EGsDialogCameraViewType viewType = GSCameraMode()->GetDialogCameraViewType();
	// 오브젝트 그리팅이면 룩 처리 안하기
	if (viewType != EGsDialogCameraViewType::DialogGreetingObject)
	{
		for (auto iter : In_dialogTargets)
		{
			if (iter != nullptr)
			{
				iter->LookAtTarget(_localPlayer);
			}
		}
	}
}
// 로컬 원래 회전으로 돌아가기
void FGsCameraModeHandler::EndLookatTarget()
{
	if (_localPlayer == nullptr)
	{
		return;
	}
	_localPlayer->GetActor()->SetActorRotation(_backupLocalRot);
}
void FGsCameraModeHandler::SetAttachFollowCamera(bool In_isAttach)
{
	if (_localPlayer == nullptr)
	{
		return;
	}

	if (AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter())
	{
		localChar->SetAttachFollowCamera(In_isAttach);
	}	
}
void FGsCameraModeHandler::SetAttachFollowCameraToController(bool In_isAttach)
{
	if (_localPlayer == nullptr)
	{
		return;
	}

	if (AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter())
	{
		localChar->SetAttachFollowCameraToController(In_isAttach);
	}
}
// 대사 백업값으로 보간 끝
void FGsCameraModeHandler::OnFinishDialogBackupLerp()
{
	SetAttachFollowCamera(true);

	if (nullptr != _localPlayer)
	{
		if (AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter())
		{
			if (UCameraComponent* camComponent = localChar->GetFollowCamera())
			{
				// 이전에 대사이면서 마지막세팅이면 relative zero로 세팅한다
				// 백업 위치로 하니까 다른 값들이 누적됨

				camComponent->SetRelativeRotation(FRotator::ZeroRotator);
				// relative 위치는 본 높이 + shift(숄더뷰) 조정이 필요해서 밑에서 처리
				// -> Target offset으로 바뀌면서 예전처럼 relative 도 초기화 하자

				camComponent->SetRelativeLocation(FVector::ZeroVector);
			}
		}
	}

	// relate location 값 복구
	if (CurrentState.IsValid() == false)
	{
		return;
	}


	auto modeControlBase =
		StaticCastSharedPtr<FGsCameraModeControlBase>(CurrentState.Pin());

	if (modeControlBase == nullptr)
	{
		return;
	}

	// 현재 모드에게 대화 연출 복구 완료 알려줌
	modeControlBase->OnFinishDialogBackupLerp();
	


	// 대사 끝이 아니라 카메라 복구후 나머지를 처리하자
	// 복구중 이동하면 카메라 오류 등등


	// 인풋 막기 해제(줌, 회전)
	//FGsInputEventMsgBase msg;
	//msg.Id = 0;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_HUD_CAMERA_MODE, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	// 나자신이 죽어있으면 ui 복구 할 필요없다
	if (_localPlayer->IsZeroHP() == false)
	{
		// ui 복구
		if (UGsUIManager* uiManage = GUI())
		{
			uiManage->ClearHideFlags(EGsUIHideFlags::STATE_DIALOG);
		}
	}

	GSCameraMode()->SetIsCameraDialogPlayAll(false);
	
	EGsCamearDialogEndCallBackType callbackType = GSCameraMode()->GetCameraDialogEndCallbackType();
	if (callbackType != EGsCamearDialogEndCallBackType::None)
	{
		FGsCameraDialogEndMessageParam cameraDialogEndParam(callbackType);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_END_LERP_END, 
			&cameraDialogEndParam);
	}

	// 이벤트 진행 매니저에게 npc 대화 종료 알린다
	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::NPC_DIALOG);
	}

	SendEnvEndDialog();


	if (_isDirtyCallbackWarp == true &&
		_callbackWarp != nullptr)
	{
		_isDirtyCallbackWarp = false;
		_callbackWarp();

		ProcWarp();
	}
}

EGsCameraInitModeType FGsCameraModeHandler::GetInitModeType()
{
	// 채널변경후
	// 버튼으로 카메라 모드 변경시 samemapspawn으로 무조건 타서
	// 초기화 상태일때만 load topo effect type 체크하기
	if (_isSpawnInitModeChange == false)
	{
		return EGsCameraInitModeType::ModeChange;
	}

	if (UGsLevelManager* levelMgr = GLevel())
	{
		if (EGsLoadTopoEffectType::SAME_MAP_SAME_POS == levelMgr->GetLoadTopoEffectType())
		{
			return EGsCameraInitModeType::SameMapSpawn;
		}
	}

	return EGsCameraInitModeType::OtherMapSpawn;
}

// 대화 모드 뷰 변경(컷당 변화)
void FGsCameraModeHandler::ChangeDialogModeView(TArray<UGsGameObjectBase*> In_dialogTargets)
{
	// 대상 저장
	SetDialogTarget(In_dialogTargets);

	SendEnvStartDialog();

	if (CurrentState.IsValid() == false)
	{
		return;
	}
	auto modeDialog =
		StaticCastSharedPtr<FGsCameraModeDialog>(CurrentState.Pin());

	if (modeDialog == nullptr)
	{
		return;
	}

	modeDialog->ChangeModeView();
	
}

// 카메라 대화 모드 뷰 변경
void FGsCameraModeHandler::OnCameraDialogModeViewChange(const IGsMessageParam* In_param)
{
	const FGsCameraDialogMessageParam* paramTarget =
		In_param->Cast<const FGsCameraDialogMessageParam>();
	if (nullptr == paramTarget)
	{
		return;
	}

	if (CurrentState.IsValid() == false)
	{
		return;
	}

	// 대화 모드가 아니면 처리하면 안된다
	if ( CurrentState.Pin()->GetType() != EGsCameraMode::Dialog)
	{
		return;
	}


	bool isUseSpotIdPos = !paramTarget->_isCameraWalking;
	SetLocalUseSpotIdPos(isUseSpotIdPos);

	// 카메라 워킹이면 페이드 아웃 사용 안함
	GSCameraMode()->SetIsUseCutFadeOut(isUseSpotIdPos);	

	GSCameraMode()->SetDialogCameraViewData(paramTarget->_viewData);

	SetLocalPosition(paramTarget->_localSpotId, paramTarget->_arrDialogTargetObj);
	

	// 룩앳 시작
	StartLookatTarget(paramTarget->_arrDialogTargetObj);


	ChangeDialogModeView(paramTarget->_arrDialogTargetObj);
}

// 로컬이 spot id pos 사용하는지
void FGsCameraModeHandler::SetLocalUseSpotIdPos(bool In_isUse)
{
	// false면 처리할 필요 없다
	if (In_isUse == false)
	{
		return;
	}

	// 이전은 안쓰고 이제 쓰면 빽업 하자
	if (_isUsedSpotIdPos == false)
	{
		_isUsedSpotIdPos = true;
		// 카메라를 떼고 처리 해야함
		// 나중에 때면 이상한 각도에서 나옴(바로 세팅시)
		SetAttachFollowCamera(false);

		if (_localPlayer == nullptr)
		{
			return;
		}
		_backupLocalPos = _localPlayer->GetLocation();
	}
}

// 로컬 위치 변경(spot id 위치 이동)
void FGsCameraModeHandler::SetLocalPosSpotId(int64 In_spotId)
{
	FGsClientSpawnManager* spawnManager = GSClientSpawn();
	if (spawnManager == nullptr)
	{
		return;
	}
	int currentMapId = GLevel()->GetCurrentLevelId();
	FGsSchemaSpotInfo findSpotInfo;
	if (false == spawnManager->TryGetSpot(
		currentMapId, In_spotId, findSpotInfo))
	{
		return;
	}

	if (findSpotInfo.pos == FVector::ZeroVector)
	{
		return;
	}

	SetLocalPlayerPos(findSpotInfo.pos);
}

void FGsCameraModeHandler::SetLocalPlayerPos(FVector In_pos)
{
	if (_localPlayer == nullptr)
	{
		return;
	}

	const FGsSchemaCreatureCommonInfo* commonTable = _localPlayer->GetCommonInfoTable();

	if (commonTable == nullptr)
	{
		return;
	}

	float height = commonTable->cylinderHeight;
	float radius = commonTable->cylinderRadius;

	UWorld* world = GLevel()->GetWorld();

	if (world == nullptr)
	{
		return;
	}
	FVector navPos = In_pos;
	FNavLocation navLoc;
	if (UGsGameObjectManager::TryFindNearestNavmesh(navLoc, world, In_pos))
	{
		navPos = navLoc.Location;
	}

	AGsCharacterBase* actorChar = _localPlayer->GetCharacter();
	if (actorChar == nullptr)
	{
		return;
	}

	UCapsuleComponent* capsule = actorChar->GetCapsuleComponent();

	if (capsule == nullptr)
	{
		return;
	}

	FVector reCalcPos = navPos;
	FHitResult hit;
	if (UGsLevelUtil::TrySweepToLand(hit, world, navPos, radius, height))
	{
		reCalcPos = hit.ImpactPoint + FVector(0, 0, capsule->GetScaledCapsuleHalfHeight());
	}

	_localPlayer->GetActor()->SetActorLocation(reCalcPos);
}

// 로컬 위치 변경()
void FGsCameraModeHandler::SetLocalPosBackup()
{
	if (_localPlayer == nullptr)
	{
		return;
	}
	
	_localPlayer->GetActor()->SetActorLocation(_backupLocalPos);
}



// 워프전 처리
void FGsCameraModeHandler::OnLoadTopoStart(const IGsMessageParam* inParam)
{
	// 이전에 그리팅이었다면
	if (CurrentState.IsValid() && CurrentState.Pin()->GetIsBeforeGreeting() == true)
	{
		// 그리팅 안하고 맵이동시 클리어 처리
		GSCameraMode()->SetIsBackupUserSetting(false);
	}
}

// 조이스틱 땜
void FGsCameraModeHandler::OnReleaseJoystick(const FGsInputEventMsgBase&)
{
	_isJoystickPressed = false;
}
// 조이스틱 누름
void FGsCameraModeHandler::OnPressJoystick(const FGsInputEventMsgBase&)
{
	_isJoystickPressed = true;
}


// mode랑 work 타겟 obj 변경
void FGsCameraModeHandler::ChangeModeTargetObj(UGsGameObjectBase* In_target, bool Is_firstTime)
{
	TMap<EGsCameraMode, TSharedPtr<FGsCameraModeBase>> container = GetContainer();
	for (auto& iter : container)
	{
		iter.Value->SetModeTarget(In_target);
	}

	// 쿼터(프리뷰)랑 다르면 변경
	// 처음만 체크
	if (CurrentState.IsValid() && CurrentState.Pin()->GetType() != EGsCameraMode::Quarter &&
		true == Is_firstTime)
	{
		ChangeState(EGsCameraMode::Quarter);
	}
	else if (CurrentState.IsValid() == true)
	{
		// 기존꺼랑 같으면
		// 초기화 해준다 
		// 워프, 사망시 아무런 세팅이 안되어있음....
		// exit는 안해도 될꺼 같지만... 해준다
		CurrentState.Pin()->Exit();
		CurrentState.Pin()->Enter();
	}
	
}

void FGsCameraModeHandler::ChangeFixedQuarterYawMode()
{
	if (CurrentState.IsValid() && CurrentState.Pin()->GetType() != EGsCameraMode::FixedQuarter)
	{
		return;
	}

	auto modeFixedQuarter =
		StaticCastSharedPtr<FGsCameraModeFixedQuarter>(CurrentState.Pin());

	if (modeFixedQuarter == nullptr)
	{
		return;
	}

	modeFixedQuarter->ChangeYawMode();

	GMessage()->GetCamera().SendMessage(MessageContentCamera::CHANGED_FIXED_QUARTER_YAW);
}

// fixed quarter mode yaw type change(north, east, south, west)
void FGsCameraModeHandler::OnClickFixedQuarterYaw()
{
	ChangeFixedQuarterYawMode();
}

void FGsCameraModeHandler::OnWarp(TFunction<void()> In_callbackWarp)
{
	//https://jira.com2us.com/jira/browse/CHR-15415
	// check camera dialog mode
	if (GSCameraMode()->GetIsCameraDialogPlay() == true)
	{
		_callbackWarp = In_callbackWarp;
		_isDirtyCallbackWarp = true;
		EndDialog();		
	}
	else
	{
		if (In_callbackWarp != nullptr)
		{
			In_callbackWarp();
		}

		ProcWarp();
	}
}

void FGsCameraModeHandler::ProcWarp()
{
	// 카메라 백업
	// 기존엔 remove character 쪽이었는데
	// 워프 연출이 생겨서
	// 워프 연출상태가 저장되면 안되므로
	// 연출전 저장한다
	// 시공의 틈새에서 나갈때는 
	// 1. 현재 카메라 모드는 시공의 틈새 전용 모드라 저장 x
	// 2, 이전 백업 데이터로 복구해야되어서 저장 x
	// 관전 모드에서도 저장 x
	if (GLevel()->IsSpaceCrackMapContents() == false &&
		GSObserver()->GetIsObserveStart() == false)
	{
		GSCameraMode()->SaveBackUpTransform();
	}

	ChangeState(EGsCameraMode::WarpStart);
}

void FGsCameraModeHandler::SetStartCameraDataTransform()
{
	SetAttachFollowCameraToController(false);


	if (CurrentState.IsValid() == false)
	{
		return;
	}

	auto modePlayableEvent =
		StaticCastSharedPtr<FGsCameraModePlayableEvent>(CurrentState.Pin());

	if (modePlayableEvent == nullptr)
	{
		return;
	}

	modePlayableEvent->SetStartCameraDataTransform();
}

void FGsCameraModeHandler::StartPlayableEventStartCam()
{
	if (CurrentState.IsValid() == false)
	{
		return;
	}

	auto modePlayableEvent =
		StaticCastSharedPtr<FGsCameraModePlayableEvent>(CurrentState.Pin());

	if (modePlayableEvent == nullptr)
	{
		return;
	}

	modePlayableEvent->StartPlayableEventStartCam();
}

void FGsCameraModeHandler::OnFinishPlayableCamLerp(bool In_isStartCam)
{
	if (In_isStartCam == true)
	{
		SetAttachFollowCamera(true);

		if (nullptr != _localPlayer)
		{
			if (AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter())
			{
				if (UCameraComponent* camComponent = localChar->GetFollowCamera())
				{
					// 이전에 대사이면서 마지막세팅이면 relative zero로 세팅한다
					// 백업 위치로 하니까 다른 값들이 누적됨

					camComponent->SetRelativeRotation(FRotator::ZeroRotator);
					// relative 위치는 본 높이 + shift(숄더뷰) 조정이 필요해서 밑에서 처리
					// -> Target offset으로 바뀌면서 예전처럼 relative 도 초기화 하자

					camComponent->SetRelativeLocation(FVector::ZeroVector);
				}
			}
		}
	}

	if (nullptr == GMode())
	{
		return;
	}
	FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow();
	if (nullptr == lobbyFlow)
	{
		return;
	}
	FGsContentsManagerLobby* contents = lobbyFlow->GetContentsManagerLobby();
	if (nullptr == contents)
	{
		return;
	}
	FGsStateLobbyHud* hud =
		contents->GetContentsStateObject<FGsStateLobbyHud>(FGsContentsMode::InLobby::HUD);

	if (nullptr == hud)
	{
		return;
	}

	FGsPlayableEventHandler* handler = hud->GetPlayableEventHandler();
	if (handler == nullptr)
	{
		return;
	}

	handler->OnFinishPlayableCamLerp(In_isStartCam);

}
void FGsCameraModeHandler::StartPlayableEventEndCam()
{
	SetAttachFollowCamera(false);


	if (CurrentState.IsValid() == false)
	{
		return;
	}

	auto modePlayableEvent =
		StaticCastSharedPtr<FGsCameraModePlayableEvent>(CurrentState.Pin());

	if (modePlayableEvent == nullptr)
	{
		return;
	}

	modePlayableEvent->StartPlayableEventEndCam();
}

// B1:https://jira.com2us.com/wiki/pages/viewpage.action?pageId=329686045 
// himichani
// set local player pos quest dialog target 
void FGsCameraModeHandler::SetLocalPosTargetFront(UGsGameObjectBase* In_target)
{
	if (In_target == nullptr)
	{
		return;
	}

	AGsCharacterBase* targetChar = In_target->GetCharacter();

	if (targetChar == nullptr)
	{
		return;
	}

	FVector fwdVec =
		targetChar->GetActorForwardVector();

	FVector oppVec = fwdVec * -1.0f;

	FVector targetActorPos = targetChar->GetActorLocation();

	float distance =
		GData()->GetGlobalData()->_camDialogTargetNPCPositionDistance;

	FVector movePos = targetActorPos - oppVec * distance;

	SetLocalPlayerPos(movePos);
}

void FGsCameraModeHandler::SetLocalPosition(int In_spotId, TArray<UGsGameObjectBase*> In_arrDialogTargetObj)
{
	if (In_spotId == 0)
	{

		EGsDialogCameraViewType viewType = GSCameraMode()->GetDialogCameraViewType();

		// https://jira.com2us.com/jira/browse/CHR-17159
		// https://jira.com2us.com/jira/browse/C2URWQ-4048
		if (//viewType == EGsDialogCameraViewType::DialogGreetingNpc ||
			viewType == EGsDialogCameraViewType::DialogGreetingSpawnFairy ||
			//viewType == EGsDialogCameraViewType::DialogGreetingNpcFairy ||
			viewType == EGsDialogCameraViewType::DialogGreetingLocalFairy)
		{
			return;
		}


		if (In_arrDialogTargetObj.Num() != 0)
		{
			UGsGameObjectBase* targetObj = In_arrDialogTargetObj[0];
			if (targetObj != nullptr)
			{
				SetLocalPosTargetFront(targetObj);
			}
		}
	}
	else
	{
		SetLocalPosSpotId(In_spotId);
	}
}

void FGsCameraModeHandler::SendEnvStartDialog()
{
	if (_arrDialogTargetObj.Num() != 0)
	{
		if (AActor* targetActor =
			GSCameraMode()->GetDialogTargetAcotr(_arrDialogTargetObj[0]))
		{
			FGsActorMessageParam param(targetActor);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_SEPARATE_LIGHT_SOURCE_START, &param);
			
		}

	}
}
void FGsCameraModeHandler::SendEnvEndDialog()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_SEPARATE_LIGHT_SOURCE_END, nullptr);
}


void FGsCameraModeHandler::OnCameraEquipViewStart(const IGsMessageParam*)
{
	if (CurrentState.IsValid() == false)
	{
		return;
	}
	
	_backupBeforeEquipView = CurrentState.Pin()->GetType();
	ChangeState(EGsCameraMode::EquipView);

	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_CAMERA_MODE_EQUIP_VIEW, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
}
void FGsCameraModeHandler::OnCameraEquipViewEnd(const IGsMessageParam*)
{
	ClearEquipView();
}

void FGsCameraModeHandler::ClearEquipView()
{
	if (CurrentState.IsValid() == false ||
		CurrentState.Pin()->GetType() != EGsCameraMode::EquipView)
	{
		return;
	}

	ChangeState(_backupBeforeEquipView);

	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_CAMERA_MODE_EQUIP_VIEW, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
}

void FGsCameraModeHandler::CloseEquipUI()
{
	if (CurrentState.IsValid() == false ||
		CurrentState.Pin()->GetType() != EGsCameraMode::EquipView)
	{
		return;
	}

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseByKeyName(TEXT("PopupPlayerDetailInfo"));
	}
}

void FGsCameraModeHandler::OnLocalWarp(const IGsMessageParam* In_param)
{
	CloseEquipUI();
}