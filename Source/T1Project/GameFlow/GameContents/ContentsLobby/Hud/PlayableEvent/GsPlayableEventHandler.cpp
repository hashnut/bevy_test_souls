#include "GsPlayableEventHandler.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/PlayableEvent/GsSchemaPlayableEventQuest.h"
#include "DataSchema/PlayableEvent/GsSchemaPlayableEventAniData.h"
#include "DataSchema/SubDialog/GsSchemaSubDialogCutData.h"
#include "DataSchema/SubDialog/GsSchemaSubDialogData.h"

#include "Runtime/DataCenter/Public/PlayableEvent/EGsPlayableEventQuestGoalType.h"
#include "Runtime/DataCenter/Public/PlayableEvent/GsStatePlayableEventNPCType.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsActorPlayableEventNPCAniPlay.h"

#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"

#include "Quest/Dialog/SubDialog/GsSubDialog.h"

#include "Util/GsLevelUtil.h"

#include "GameFramework/PlayerController.h"

#include "Message/GsMessageStage.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageInput.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "UI/UIContent/Tray/GsUITraySubDialog.h"
#include "UI/UIContent/Tray/GsUITrayAreaName.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Net/GsNetSendService.h"

#include "GsTable.h"

#include "GameFramework/PlayerController.h"

#include "PlayableEvent/EGsPlayableEventQuestGoalType.h"
#include "PlayableEvent/GsPlayableEventDefine.h"

#include "Sequence/GsSequenceManager.h"
#include "Sequence/GsSequencePlayer.h"

#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"

#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "Engine/Classes/Engine/LevelStreamingDynamic.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/Engine/Level.h"
#if WITH_EDITOR
#include "Cheat/GsCheatManager.h"
#endif


// 초기화(최초 한번)
void FGsPlayableEventHandler::Init()
{
	if (nullptr == _questSubDialog)
	{
		_questSubDialog = new FGsSubDialog();
	}
}

// 정리(마지막 한번)
void FGsPlayableEventHandler::Close()
{
	if (_questSubDialog != nullptr)
	{
		delete _questSubDialog;
		_questSubDialog = nullptr;
	}

	UGsUIManager* uiManager = GUI();

	if (uiManager == nullptr)
	{
		return;
	}

	UWorld* world = uiManager->GetWorld();
	if (world == nullptr)
	{
		return;
	}


	if (_finishDelayTimeHandle.IsValid() == true)
	{
		world->GetTimerManager().ClearTimer(_finishDelayTimeHandle);
		_finishDelayTimeHandle.Invalidate();
		
	}

	if (_waitQuestGoalTimeHandle.IsValid() == true)
	{
		world->GetTimerManager().ClearTimer(_waitQuestGoalTimeHandle);
		_waitQuestGoalTimeHandle.Invalidate();
	}

	if (_trayWaitTimeHandle.IsValid() == true)
	{
		world->GetTimerManager().ClearTimer(_trayWaitTimeHandle);
		_trayWaitTimeHandle.Invalidate();
	}


	FWorldDelegates::LevelAddedToWorld.RemoveAll(this);
		
}

// 이벤트 시작 호출
void FGsPlayableEventHandler::StartEvent()
{
	// 1. go to tank
	// 2. diconnect
	// 3. complete quest
	// 4. go to lobby
	// 5. re enter playable event( _isLastQuestComplete == true)
	_isLastQuestComplete = false;

	UWorld* world = LSLobbyCharacter()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	bool isSuccess;
	_levelPath = GData()->GetGlobalData()->_playableEventlevelPath.ToString();
	_loadedLevel = ULevelStreamingDynamic::LoadLevelInstance(world, _levelPath, FVector::ZeroVector, FRotator::ZeroRotator, isSuccess);


	// 정렬된 테이블 세팅한다
	LSLobbyPlayableEvent()->SortTable();

	_isProgress = true;

	// hide screen when sub level load finished
	//float fadeInTime = GData()->GetGlobalData()->_playableEventFadeInTime;
	//FGsUIHelper::TrayFadeIn(fadeInTime, false);
	FGsUIHelper::TrayDimmed(true);

	// https://forums.unrealengine.com/t/onlevelloaded-is-called-before-level-finishes-initializing/156015/2
	// InstanceToolModul.cpp 92, 98
	FWorldDelegates::LevelAddedToWorld.AddRaw(this, &FGsPlayableEventHandler::OnLevelAddedToWorld);


	FGsMessageHolder* message = GMessage();
	if (message == nullptr)
	{
		return;
	}

	_listSystemDelegate.Emplace(
		message->GetSystem().AddRaw(MessageSystem::EXIT_GAME, this, &FGsPlayableEventHandler::OnExitGame));

	_listSystemDelegate.Emplace(
		message->GetSystem().AddRaw(MessageSystem::RESTART_GAME, this, &FGsPlayableEventHandler::OnExitGame));

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddRaw(MessageSystem::SEQUENCE_PLAYER_END, this, &FGsPlayableEventHandler::OnStopMovie));

	_msgUIHandlerList.Emplace(
		message->GetUI().AddRaw(MessageUI::FINISH_SUB_DIALOG, this, &FGsPlayableEventHandler::OnFinishSubDialog));
}

void FGsPlayableEventHandler::OnExitGame()
{
	LSLobbyCharacter()->DespawnPlayableEventLocal();
	LSLobbyPlayableEvent()->DespawnAllMonster(true);

	FWorldDelegates::LevelAddedToWorld.RemoveAll(this);

	if (_loadedLevel != nullptr)
	{
		_loadedLevel->SetIsRequestingUnloadAndRemoval(true);
	}

	RemoveMessageBind();
}

// 이벤트 종료
void FGsPlayableEventHandler::EndEvent()
{
	FWorldDelegates::LevelAddedToWorld.RemoveAll(this);


	// ui 매니저에게 hud 끄게 호출
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseByKeyName(TEXT("HUDPlayableEvent"), true);
		// sub 대화창도 켜져있으면 바로 닫음
		if (true == uiManager->IsActiveWidget(TEXT("TraySubDialogPlayableEvent")))
		{
			uiManager->CloseByKeyName(TEXT("TraySubDialogPlayableEvent"), true);
		}
	}

	// 조이스틱 hide
	HideJoystick();
	DespawnPlayableEventLocal();

	// flow 변경 메시지 전송
	// 캐릭터 생성창으로
	FGsMessageHolder* msg = GMessage();
	msg->GetStage().SendMessage(MessageStage::ENTER_POST_PLAYABLE_MOVIE_STATE);

	bool isPlayableTestEnter = false;
	if (UGsLobbyPlayableEventManager* playableEventManager = LSLobbyPlayableEvent())
	{
		isPlayableTestEnter = playableEventManager->GetAllIsTestEnterVal();
	}

	// 이미 도달한 값(테스트로 진입)이면 전송 안함
	if (isPlayableTestEnter == false &&  EGsPlayableEventServerCheckPointType::PlayableEventComplete > GGameData()->GetPlayableEventCheckPoint())
	{
		GGameData()->SetPlayableEventCheckPoint(EGsPlayableEventServerCheckPointType::PlayableEventComplete);
	}	

	if (_loadedLevel != nullptr)
	{
		_loadedLevel->SetIsRequestingUnloadAndRemoval(true);
	}	

	RemoveMessageBind();
}

void FGsPlayableEventHandler::RemoveMessageBind()
{
	FGsMessageHolder* message = GMessage();

	if (message == nullptr)
	{
		return;
	}
	if (_listSystemDelegate.Num() != 0)
	{
		for (auto iter : _listSystemDelegate)
		{
			message->GetSystem().Remove(iter);
		}
		_listSystemDelegate.Empty();
	}

	if (_listSystemParamDelegate.Num() != 0)
	{
		for (auto iter : _listSystemParamDelegate)
		{
			message->GetSystemParam().Remove(iter);
		}
		_listSystemParamDelegate.Empty();
	}

	if (_msgUIHandlerList.Num() != 0)
	{
		for (auto iter : _msgUIHandlerList)
		{
			message->GetUI().Remove(iter);
		}
		_msgUIHandlerList.Empty();
	}
}

// 로컬 스폰
void FGsPlayableEventHandler::SpawnPlayableEventLocal()
{
	LSLobbyCharacter()->SpawnPlayableEventLocal();

	// send message env playable event
	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::PLAYABLE_EVENT));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &param);
}

// 로컬 디스폰
void FGsPlayableEventHandler::DespawnPlayableEventLocal()
{
	// send message env playable event
	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::PLAYABLE_EVENT));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_END_EVENT, &param);

	LSLobbyCharacter()->DespawnPlayableEventLocal();
}

// 현재 퀘스트 완료인지
bool FGsPlayableEventHandler::IsCurrentMoveQuestComplete()
{
	// 거리체크만 한다
	if (LSLobbyPlayableEvent()->GetCurrentGoalType() != EGsPlayableEventQuestGoalType::Move)
	{
		return false;
	}

	// 로컬과 목표 거리 체크 
	UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
	if (local == nullptr)
	{
		return false;
	}

	FVector localPos = local->GetLocation();
	FVector goalPos = LSLobbyPlayableEvent()->GetCurrentGoalPos();

	float distSquared = FVector::DistSquared2D(localPos, goalPos);

	if (distSquared <= LSLobbyPlayableEvent()->GetSquaredAcceptanceDist())
	{
		return true;
	}
	return false;
}

// 현재 퀘스트 데이터로 세팅
void FGsPlayableEventHandler::ChangeQuestData(int In_index)
{
	LSLobbyPlayableEvent()->SetNowQuestData(In_index);

	// 모든 hud ui 갱신
	GMessage()->GetContentsPlayableEventHud().SendMessage(MessageContentPlayableEventHud::INVALIDATE_ALL);

	const FGsSchemaPlayableEventQuest* currentTbl = LSLobbyPlayableEvent()->GetCurrentQuestTbl();
	if (currentTbl == nullptr)
	{
		return;
	}

	// goal time wait: timmer setting
	if (LSLobbyPlayableEvent()->GetCurrentGoalType() == EGsPlayableEventQuestGoalType::WaitTimmer)
	{
		UWorld* world = LSLobbyCharacter()->GetWorld();
		if (world == nullptr)
		{
			return;
		}

		FTimerDelegate timerCallback;
		timerCallback.BindLambda([this] {
			OnFinishGoalTimmerWait();
			});

		world->GetTimerManager().SetTimer(_waitQuestGoalTimeHandle,
			timerCallback, currentTbl->waitTimeSecond, false);
	}

	if (LSLobbyPlayableEvent()->GetCurrentGoalType() == EGsPlayableEventQuestGoalType::WaitSubDialogEnd)
	{
		_isStartGoalSubDialog = true;
	}
	else
	{
		_isStartGoalSubDialog = false;
	}

	// if sequence playing, not setting block input(already set by sequence)
	bool isPlaying = false;
	if (UGsSequencePlayer* sequencePlayer = GSequencePlayer())
	{
		isPlaying = sequencePlayer->IsPlaying();
	}

	if (isPlaying == false)
	{
		// lock input
		bool isLockInput = currentTbl->lockInput;

		//FGsInputEventMsgBase msg;
		//msg.Id = (isLockInput == true) ? 1 : 0;
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_PLAYABLE, isLockInput);
		GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}

}
void FGsPlayableEventHandler::OnFinishGoalTimmerWait()
{
	ChangeNextQuest();
}
void FGsPlayableEventHandler::PostPlayableEventLevel()
{
	SpawnPlayableEventLocal();

	// ui 매니저에게 hud 키게 호출
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->Open(TEXT("HUDPlayableEvent"));
	}

	// 조이스틱 표시
	ShowJoystick();

	// 지역명 출력
	ShowAreaUI();

	FGsUIHelper::TrayDimmed(false);
	float fadeInTime = GData()->GetGlobalData()->_playableEventFadeInTime;
	FGsUIHelper::TrayFadeIn(fadeInTime);


	UWorld* world = LSLobbyCharacter()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	FTimerDelegate timerCallback;
	timerCallback.BindLambda([this] {
		_isStartCamMoveDirty = true;
		
		});

	world->GetTimerManager().SetTimer(_trayWaitTimeHandle,
		timerCallback, fadeInTime, false);


	CollectArrangedNPCAniPlayBP();

	LSLobbyPlayableEvent()->CollectArrangedCameraData();

	UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
	if (local == nullptr)
	{
		return;
	}
	// camera playeable event setting
	if (FGsCameraModeHandler* cameraModehandler = local->GetCameraModeHandler())
	{
		cameraModehandler->SetStartCameraDataTransform();
	}

	// lock input(when camera blending work)
	//FGsInputEventMsgBase msg;
	//msg.Id =  1;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_PLAYABLE, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

#if WITH_EDITOR
	if (UGsCheatManager::IsEffectEmptyPlayer == false)
	{
		ClearPlayableEvent();
		return;
	}
#endif
}

void FGsPlayableEventHandler::StartPlayableEventStartCam()
{
	UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
	if (local == nullptr)
	{
		return;
	}
	// camera playeable event setting
	if (FGsCameraModeHandler* cameraModehandler = local->GetCameraModeHandler())
	{
		cameraModehandler->StartPlayableEventStartCam();
	}
}
void FGsPlayableEventHandler::Update(float In_deltaTime)
{

	if (_isProgress == false)
	{
		if (_isMonsterAllDestroyDirty == true)
		{
			_isMonsterAllDestroyDirty = false;
			_isEndCamLerpDirty = true;
			LSLobbyPlayableEvent()->DespawnAllMonster();
		}

		if (_isEndCamLerpDirty == true)
		{
			_isEndCamLerpDirty = false;
			// finish  camera start
			StartPlayableEventEndCam();
		}

		if (_isEndDirty == true)
		{
			if (LSLobbyPlayableEvent()->GetMonsterCount() == 0)
			{
				// 카메라 yaw min, max 복구				
				RecoverPlayableEventCamYawMinMax();
				
				_isEndDirty = false;
				EndEvent();

			}
		}

		return;
	}
	if (_isSequencePlay == false &&
		IsCurrentMoveQuestComplete() == true &&
		_isLastQuestComplete == false)
	{
		ChangeNextQuest();
	}

	if (_isStartCamMoveDirty == true)
	{
		_isStartCamMoveDirty = false;
		StartPlayableEventStartCam();
	}
}

// 다음 퀘스트로 변경
void FGsPlayableEventHandler::ChangeNextQuest()
{
	// 퀘스트 완료시 하는 처리
	ActQuestCompleteJob();

	// 마지막인가
	if (true == LSLobbyPlayableEvent()->IsLastQuest())
	{
		_isLastQuestComplete = true;
		UWorld* world = LSLobbyCharacter()->GetWorld();
		if (world == nullptr)
		{
			return;
		}

		// monster die show delay

		FTimerDelegate timerCallback;
		timerCallback.BindLambda([this] {
				ClearPlayableEvent();
			});

		float delaySecond = 
			GData()->GetGlobalData()->_playableEventFinishDelaySecond;

		world->GetTimerManager().SetTimer(_finishDelayTimeHandle,
			timerCallback, delaySecond, false);

		
	}
	else
	{
		// 다음 퀘스트로 넘긴다
		int currentIndex = 
			LSLobbyPlayableEvent()->GetProgressQuestIndex();
		ChangeQuestData(currentIndex +1);
	}
}

// 킬카운트 처리
void FGsPlayableEventHandler::OnAddKillCount(int In_tblId)
{
	// 증가
	if (true == LSLobbyPlayableEvent()->AddKillCount(In_tblId))
	{
		GMessage()->GetContentsPlayableEventHud().SendMessage(MessageContentPlayableEventHud::INVALIDATE_QUEST);
	}

	if (true == LSLobbyPlayableEvent()->IsCurrentKillQuestComplete())
	{
		ChangeNextQuest();
	}
}

// 조이스틱 보이게
void FGsPlayableEventHandler::ShowJoystick()
{
	UWorld* world = LSLobbyCharacter()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	if (APlayerController* playerController = world->GetFirstPlayerController())
	{
		playerController->CreateTouchInterface();
	}
}
// 조이스틱 안보이게
void FGsPlayableEventHandler::HideJoystick()
{
	UWorld* world = LSLobbyCharacter()->GetWorld();
	if (world == nullptr)
	{
		return;
	}
	//  조이스틱 안보이게 처리
	if (APlayerController* playerController = world->GetFirstPlayerController())
	{
		playerController->ActivateTouchInterface(nullptr);
	}
}


// 퀘스트 컴플리트때 하는일 처리
void FGsPlayableEventHandler::ActQuestCompleteJob()
{
	// sub 대화창 데이터 있으면 출력
	const FGsSchemaPlayableEventQuest* currentTbl = LSLobbyPlayableEvent()->GetCurrentQuestTbl();
	if (currentTbl == nullptr)
	{
		return;
	}
	// null 체크 안하면 call stack 에러 메시지 뿜뿜
	if (currentTbl->subDialogData.IsNull() == false)
	{
		const FGsSchemaSubDialogData* dialogData = currentTbl->subDialogData.GetRow();

		if (dialogData != nullptr && nullptr != _questSubDialog)
		{
			_questSubDialog->OpenDialog(TEXT("TraySubDialogPlayableEvent"), dialogData);
		}
	}

	// check npc ani play
	for (const FGsSchemaPlayableEventAniData& iterAniData : currentTbl->npcAniData)
	{
		for (int spawnId :iterAniData.npcAniSpawnId)
		{
			AGsActorPlayableEventNPCAniPlay* targetBP = LSLobbyPlayableEvent()->FindAniPlayBP(spawnId);
			if (targetBP == nullptr)
			{
				continue;
			}			
			targetBP->ChangeAniState(iterAniData.playAniType);
			targetBP->SetActorHiddenInGame(false);
		}
	}

	if (currentTbl->sequenceId != 0)
	{
		// if move to skill
		// stop
		UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
		if (local == nullptr)
		{
			return;
		}

		if (local->GetBaseFSM()->IsState(EGsStateBase::AutoMove) == true)
		{
			if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
			{
				movement->Stop();
			}
		}

		if (FGsSkillHandlerLocalPlayer* skillHandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>())
		{
			skillHandler->ClearReserveSkill();			
		}


		FGsSequenceMessageParam Param(currentTbl->sequenceId);
		GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &Param);

		_isSequencePlay = true;
	}

}

// 지역명 처음에 출력
void FGsPlayableEventHandler::ShowAreaUI()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return;	
	}
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayArea"));
	if (widget.IsValid())
	{
		UGsUITrayAreaName* areaWidget = Cast<UGsUITrayAreaName>(widget);
		if (nullptr != areaWidget)
		{
			FText zoneNameText = 
				GData()->GetGlobalData()->_playableEventZoneName;
			areaWidget->SetAreaName(zoneNameText);
		}
	}
	
}

// 플레이어블 이벤트 카메라 yaw min, max 이전으로 복구
void FGsPlayableEventHandler::RecoverPlayableEventCamYawMinMax()
{
	UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
	if (local == nullptr)
	{
		return;
	}

	AGsCharacterLocalPlayer* localChar = local->GetLocalCharacter();


	if (localChar == nullptr ||
		localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false)
	{
		return;
	}

	APlayerController* controller =
		CastChecked<APlayerController>(localChar->Controller);

	if (controller == nullptr ||
		nullptr == controller->PlayerCameraManager)
	{
		return;
	}
	controller->PlayerCameraManager->ViewYawMin =
		_backupPlayableEventViewYawMin;
	controller->PlayerCameraManager->ViewYawMax =
		_backupPlayableEventViewYawMax;

}

// clear
void FGsPlayableEventHandler::ClearPlayableEvent()
{
	_isProgress = false;
	_isMonsterAllDestroyDirty = true;
}

void FGsPlayableEventHandler::OnLevelAddedToWorld(ULevel* Level, UWorld* World)
{

	PostPlayableEventLevel();

	// 현재 퀘스트 데이터로 세팅
	ChangeQuestData(0);
}

// collect ani play bp in level
void FGsPlayableEventHandler::CollectArrangedNPCAniPlayBP()
{
	LSLobbyPlayableEvent()->CollectArrangedNPCAniPlayBP();
}

void FGsPlayableEventHandler::OnFinishSubDialog(const IGsMessageParam*)
{
	if (LSLobbyPlayableEvent()->GetCurrentGoalType() != EGsPlayableEventQuestGoalType::WaitSubDialogEnd ||
		_isStartGoalSubDialog == false)
	{
		return;
	}

	ChangeNextQuest();
}

void FGsPlayableEventHandler::OnFinishPlayableCamLerp(bool In_isStartCam)
{
	if (In_isStartCam == true)
	{
		// release input(when camera blending work)
		//FGsInputEventMsgBase msg;
		//msg.Id = 0;
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_PLAYABLE, false);
		GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}
	else
	{
		_isEndDirty = true;
	}
}

void FGsPlayableEventHandler::StartPlayableEventEndCam()
{
	UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
	if (local == nullptr)
	{
		return;
	}
	// camera playeable event end cam start
	if (FGsCameraModeHandler* cameraModehandler = local->GetCameraModeHandler())
	{
		cameraModehandler->StartPlayableEventEndCam();
	}

	// block input(when camera blending work)
	//FGsInputEventMsgBase msg;
	//msg.Id = 1;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_PLAYABLE, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
}

void FGsPlayableEventHandler::OnStopMovie(const IGsMessageParam* In_param)
{
	_isSequencePlay = false;
}