// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestActionManagement.h"

#include "Camera/Define/EGsDialogCameraViewType.h"

#include "UI/UIContent/Tray/GsUITrayTickerChapterOpen.h"

#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Quest/Main/GsSchemaQuestMainChapter.h"
#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/MessageParam/GsCameraMessageParam.h"
#include "Message/MessageParam/GsEventMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "NpcDialog/GsSchemaNpcDialogData.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "DataSchema/Camera/GsSchemaCameraDialogViewData.h"
#include "Message/GsMessageSystem.h"

#include "Sequence/GsSequencePlayer.h"
#include "Sequence/GsSequenceManager.h"

#include "UI/UIContent/Tray/GsUITrayFadeEffect.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UTIL/GsTableUtil.h"

#include "GameObject/Fairy/GsFairyHandlerPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"

#include "ActorEx/GsActorFieldFairy.h"
#include "Data/GsDataContainManager.h"


void FGsQuestActionManagement::Initialize()
{
	if (nullptr == _questSubDialog)
	{
		_questSubDialog = TUniquePtr<FGsSubDialog>(new FGsSubDialog());
	}

	if (nullptr == _questNpcDialog)
	{
		_questNpcDialog = TUniquePtr<FGsNpcDialog>(new FGsNpcDialog());
	}

	RegisterMessages();
}

void FGsQuestActionManagement::Finalize()
{	
	UnregisterMessages();

	Clear();

	if (_questNpcDialog.IsValid())
	{
		_questNpcDialog = nullptr;
	}
	if (_questSubDialog.IsValid())
	{
		_questSubDialog = nullptr;
	}
}

void FGsQuestActionManagement::Clear()
{
	ClearData();

	if (nullptr != _questNpcDialog)
	{
		if (_questNpcDialog->_delegateOnFinished.IsBound())
		{
			_questNpcDialog->_delegateOnFinished.Unbind();
		}
	}
	if (nullptr != _questSubDialog)
	{
		if (_questSubDialog->_delegateOnFinished.IsBound())
		{
			_questSubDialog->_delegateOnFinished.Unbind();
		}
	}

	_npcDialogIdTarget = nullptr;

	if (nullptr != _questNpcDialog)
	{
		_questNpcDialog->OnCloseDialog();
	}

}

void FGsQuestActionManagement::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	MGameObject& gameObjectMessage = msg->GetGameObject();
	_listMsgHandle.Emplace(gameObjectMessage.AddRaw(MessageGameObject::CAMERA_DIALOG_START_LERP_END, this, &FGsQuestActionManagement::CameraLerpEndPlayNpcDialog));
	_listMsgHandle.Emplace(gameObjectMessage.AddRaw(MessageGameObject::CAMERA_DIALOG_END_LERP_END, this, &FGsQuestActionManagement::CloseNpcDialog));	
	_listMsgHandle.Emplace(gameObjectMessage.AddRaw(MessageGameObject::LOCAL_DIE, this, &FGsQuestActionManagement::OnLocalPlayerDie));
	_listMsgHandle.Emplace(gameObjectMessage.AddRaw(MessageGameObject::LOCAL_HIT, this, &FGsQuestActionManagement::OnHitLocal));

	_listMsgHandle.Emplace(gameObjectMessage.AddRaw(MessageGameObject::LOCAL_WARP, this, &FGsQuestActionManagement::OnLocalWarp));

	MSystem& systemMessage = msg->GetSystem();
	_listSystemMsgHandle.Emplace(systemMessage.AddRaw(MessageSystem::RECONNECT_END, this, &FGsQuestActionManagement::OnReconnection));

	MEvent& eventMessage = msg->GetEvent();
	_listMsgHandleEvent.Emplace(eventMessage.AddRaw(MessageContentEvent::LAST_EVENT_ACTION_FINISHED, this, &FGsQuestActionManagement::FinishedEventAction));
	_listMsgHandleEvent.Emplace(eventMessage.AddRaw(MessageContentEvent::ALL_CLEAR_EVENT_ACTION_IMMEDIATE, this, &FGsQuestActionManagement::EventActionAllClearImmediate));

	MQuest& qusetMessage = msg->GetQuest();
	_messageContentQuestList.Empty();
	_messageContentQuestList.Emplace(qusetMessage.AddRaw(MessageContentQuest::QUEST_EVENT_BEGIN_START, this, &FGsQuestActionManagement::OnQuestEventBeginStart));
	_messageContentQuestList.Emplace(qusetMessage.AddRaw(MessageContentQuest::QUEST_EVENT_CLEAR_START, this, &FGsQuestActionManagement::OnQuestEventClearStart));

}

void FGsQuestActionManagement::UnregisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	MGameObject& gameObjectMessage = msg->GetGameObject();	
	for (auto iter : _listMsgHandle)
	{
		gameObjectMessage.Remove(iter);
	}	
	_listMsgHandle.Empty();

	MEvent& eventMessage = msg->GetEvent();
	for (auto iter : _listMsgHandleEvent)
	{
		eventMessage.Remove(iter);
	}
	_listMsgHandleEvent.Empty();

	MSystem& systemMessage = msg->GetSystem();
	for (auto iter : _listSystemMsgHandle)
	{
		systemMessage.Remove(iter);
	}
	_listSystemMsgHandle.Empty();	

	MQuest& qusetMessage = msg->GetQuest();
	for (auto iter : _messageContentQuestList)
	{
		qusetMessage.Remove(iter);
	}
	_messageContentQuestList.Empty();

	if (_systemMsg.Num() != 0)
	{
		for (auto iter : _systemMsg)
		{
			msg->GetSystemParam().Remove(iter);
		}
		_systemMsg.Empty();
	}
}

// 초기화
void FGsQuestActionManagement::ClearData()
{
	_questId = 0;
	_nextQuestId = 0;
	_subDialogId = 0;
	_gameId = 0;
	_preEventActionGroupId = 0;
	_postEventActionGroupId = 0;
	_questEventType = QuestEventType::EVENT_STROY_BEGIN;
	_preEventActionIndex = INVALID_EVENT_ACTION_INDEX;
	_postEventActionIndex = INVALID_EVENT_ACTION_INDEX;

	_npcDialogData = nullptr;
	_npcDialogIdTarget = nullptr;
	_npcDialogCameraViewType = nullptr;
	_isEventReStart = false;

	_backupEventActionDialogId = 0;
	_backupEventActionTargetNpcId = 0;

	if (_delegateNpcDialog.IsBound())
	{		
		_delegateNpcDialog.Unbind();
	}
}

void FGsQuestActionManagement::ClearQuestAction(QuestId InQuestId)
{
	if (InQuestId == _questId)
	{
		if (nullptr != _questNpcDialog)
		{
			if (_questNpcDialog->_delegateOnFinished.IsBound())
			{
				_questNpcDialog->_delegateOnFinished.Unbind();
			}
		}	
	}
}

// 액션 시작
void FGsQuestActionManagement::OnQuestEventBeginStart(const struct IGsMessageParam* inData)
{
	const FGsQuestBeginActionParam* param =
		inData->Cast<const FGsQuestBeginActionParam>();

	if (nullptr == param)
	{
		// quest event가 불렸는데 param이 비워왔다.
		return;
	}

	// 이벤트 시작 전 데이터 클리어
	ClearData();

	_questEventType = param->_questEventType;
	switch (_questEventType)
	{
	case QuestEventType::EVENT_STROY_INIT_BEGIN:
	{
		_questId = param->_questId;
		_preEventActionGroupId = param->_eventActionGroupId;
		_preEventActionIndex = param->_eventActionIndex;
		_isEventReStart = param->_isReStart;
	}
	break;
	case QuestEventType::EVENT_STROY_BEGIN:
	{
		_questId = param->_questId;
		_subDialogId = param->_subDialogId;
	}
	break;
	case QuestEventType::EVENT_PE_QUEST_BEGIN:
	{
		_questId = param->_questId;
		_nextQuestId = param->_questId;
		_preEventActionGroupId = param->_eventActionGroupId;
		_preEventActionIndex = param->_eventActionIndex;

		PalyChapterStartTicker();
		return;
	}
	break;
	case QuestEventType::EVENT_QUEST_BEGIN:
	{
		_questId = param->_questId;
		_subDialogId = param->_subDialogId;
	}
	break;
	default:
		break;
	}

	OnEventAction();	
}

void FGsQuestActionManagement::OnQuestEventClearStart(const struct IGsMessageParam* inData)
{
	const FGsQuestClearActionParam* param =
		inData->Cast<const FGsQuestClearActionParam>();

	if (nullptr == param)
	{
		// quest event가 불렸는데 param이 비워왔다.
		return;
	}

	// 이벤트 시작 전 데이터 클리어
	ClearData();

	_questEventType = param->_questEventType;
	switch (_questEventType)
	{
	case QuestEventType::EVENT_STROY_CLEAR:
	{
		_questId = param->_questId;
		_postEventActionGroupId = param->_postEventActionGroupId;
		_postEventActionIndex = param->_postEventActionIndex;
	}
	break;	
	case QuestEventType::EVENT_QUEST_CLEAR:
	{
		_questId = param->_questId;
		_nextQuestId = param->_nextQuestId;
		_preEventActionGroupId = param->_preEventActionGroupId;
		_postEventActionGroupId = param->_postEventActionGroupId;
		_preEventActionIndex = param->_preEventActionIndex;
		_postEventActionIndex = param->_postEventActionIndex;
		_subDialogId = param->_subDialogId;
	}
	break;
	case QuestEventType::EVENT_OBJECTIVE_CLEAR:
	{
		_questId = param->_questId;
		_subDialogId = param->_subDialogId;
	}
	break;
	default:
		break;
	}

	OnEventAction();
}

// 모든 액션이 끝났을 때
void FGsQuestActionManagement::EndQuestAction()
{
	if (0 < _questId)
	{
		FGsFinishedQuestEventParam parm(_questEventType, _questId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_EVENT_FINISHED, &parm);
	}
}

void FGsQuestActionManagement::StopQuestAction()
{
	ClearData();
}

bool FGsQuestActionManagement::IsActioning()
{
	// 대사 연출 중이나 시네마틱 중이면 이펙트 출력 안한다
	bool isCameraDialogPlay = GSCameraMode()->GetIsCameraDialogPlay();
	bool isSequencePlayer = false;

	if (UGsSequencePlayer* sequencePlayer = GSequencePlayer())
		isSequencePlayer = sequencePlayer->IsPlaying();

	if (isCameraDialogPlay == true ||
		isSequencePlayer == true)
	{
		return true;
	}

	return false;
}

// 챕터 연출 시작
void FGsQuestActionManagement::PalyChapterStartTicker()
{	
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (true == questManager->IsNewChapterOpen(_questId))
	{
		FGsQuestMainTableCacheSet* questMainTableCache = questManager->GetQuestMainTableCache(_questId);
		if (nullptr == questMainTableCache)
			return;

		int32 chapterIndex = questMainTableCache->GetChapterIndex();

		TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = questMainTableCache->GetCurrentQuestChapterTableCache();
		if (nullptr == chapterTableCache)
			return;
				
		const FGsSchemaQuestMainChapter* schemaQuestChapter = chapterTableCache->GetSchemaQuestChapter();
		if (nullptr == schemaQuestChapter)
			return;

		FText text = schemaQuestChapter->chapterTitleText;		

		UGsUIManager* uiManager = GUI();
		if (nullptr != uiManager)
		{
			TWeakObjectPtr<class UGsUIWidgetBase> ticker = uiManager->GetWidgetByKey(TEXT("TrayTickerChaterOpen"));
			if (false == ticker.IsValid())
			{
				ticker = uiManager->OpenAndGetWidget(TEXT("TrayTickerChaterOpen"));
			}

			if (class UGsUITrayTickerChapterOpen* chapterAcceptTicker = Cast<UGsUITrayTickerChapterOpen>(ticker))
			{
				chapterAcceptTicker->SetData(chapterIndex, text);
				chapterAcceptTicker->_delegateOnFinished.BindRaw(this, &FGsQuestActionManagement::FinishChapterStartTicker);
			}
		}
	}
	else
	{
		// 챕터 오픈 연출이 없어서 EventAction 시작
		OnEventAction();
	}
}

//챕터 오픈 연출 끝
void FGsQuestActionManagement::FinishChapterStartTicker()
{
	// 챕터오픈 연출이 끝났다면
	// 서브대화창 연출 시작

	OnEventAction();
}

// 서브대화창 열기
void FGsQuestActionManagement::OpenSubDialog()
{
	// 튜토리얼 중이면 연출 안함
	if (false == GSTutorial()->IsActiveTutorial())
	{		
		if (nullptr != _questSubDialog &&
			0 < _subDialogId)
		{
			bool isVoicePlayable = true;
			if (nullptr != _questNpcDialog)
			{
				isVoicePlayable = _questNpcDialog->IsActive() ? false : true;
			}
			_questSubDialog->OpenDialog(TEXT("TraySubDialog"), GetSubDialogData(_subDialogId), isVoicePlayable);
		}	
	}	

	EndQuestAction();
}

// 서브대화창 닫기
void FGsQuestActionManagement::CloseSubDialog()
{
	_questSubDialog->ForceStopDialog();
}

void FGsQuestActionManagement::OnEventAction()
{	
	if (0 < _postEventActionGroupId)
	{
		StartPostEventAction();
		return;
	}
	else
	{
		if (0 < _preEventActionGroupId)
		{
			StartPreEventAction();
			return;
		}
	}	

	OpenSubDialog();
}

void FGsQuestActionManagement::StartPreEventAction()
{
	GSLOG(Log, TEXT("[Quest] GsQuestActionManagement::StartPreEventAction -> _nextQuestId : %llu, _preEventActionGroupId : %d, preEventActionIndex : %d, _isEventReStart : %s"), 
		_nextQuestId, _preEventActionGroupId, _preEventActionIndex, _isEventReStart ? TEXT("true") : TEXT("false"));

	const FGsReqEventActionParam Param(_nextQuestId, EventActionState::PRE, _preEventActionGroupId, _preEventActionIndex, _isEventReStart);
	GMessage()->GetEvent().SendMessage(MessageContentEvent::EVENT_ACTION_PLAY, Param);
}

void FGsQuestActionManagement::FinishedPreEventAction()
{
	OpenSubDialog();
}

void FGsQuestActionManagement::StartPostEventAction()
{
	GSLOG(Log, TEXT("[Quest] GsQuestActionManagement::StartPostEventAction -> questId : %llu, _postEventActionGroupId : %d, _postEventActionIndex : %d"), 
		_questId, _postEventActionGroupId, _postEventActionIndex);

	const FGsReqEventActionParam Param(_questId, EventActionState::POST, _postEventActionGroupId, _postEventActionIndex, false);
	GMessage()->GetEvent().SendMessage(MessageContentEvent::EVENT_ACTION_PLAY, Param);
}

void FGsQuestActionManagement::FinishedPostEventAction()
{
	if (0 < _preEventActionGroupId)
	{
		StartPreEventAction();
	}
	else
	{
		OpenSubDialog();
	}
}

void FGsQuestActionManagement::FinishedEventAction(const IGsMessageParam& In_data)
{
	const FGsEventActionFinishedParam* Param = In_data.Cast<const FGsEventActionFinishedParam>();

	if (_postEventActionGroupId == Param->_eventActionGroupId)
	{
		FinishedPostEventAction();
	}
	else if (_preEventActionGroupId == Param->_eventActionGroupId)
	{
		FinishedPreEventAction();
	}
}

// 치트용
// 서브대화창 열기
void FGsQuestActionManagement::OpenSubDialog(int32 inDialogId)
{
	if (nullptr != _questSubDialog)
	{
		_questSubDialog->OpenDialog(TEXT("TraySubDialog"), GetSubDialogData(inDialogId));
	}
}

const FGsSchemaSubDialogData* FGsQuestActionManagement::GetSubDialogData(int32 InRowKey)
{
	const UGsTableSubDialogData* table = Cast<UGsTableSubDialogData>(FGsSchemaSubDialogData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaSubDialogData* schemaSubDialogData = nullptr;
	if (false == table->FindRowById(InRowKey, schemaSubDialogData))
	{
		return nullptr;
	}

	return schemaSubDialogData;
}

// 챕터오픈 연출 시작
void FGsQuestActionManagement::PalyChapterStartTicker(QuestId inQuestId)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;
	
	FGsQuestMainTableCacheSet* questMainTableCache = questManager->GetQuestMainTableCache(_questId);
	if (nullptr == questMainTableCache)
		return;

	int32 chapterIndex = questMainTableCache->GetChapterIndex();

	TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = questMainTableCache->GetCurrentQuestChapterTableCache();
	if (nullptr == chapterTableCache)
		return;

	const FGsSchemaQuestMainChapter* schemaQuestChapter = chapterTableCache->GetSchemaQuestChapter();
	if (nullptr == schemaQuestChapter)
		return;

	FText text = schemaQuestChapter->chapterTitleText;

	UGsUIManager* uiManager = GUI();
	if (nullptr != uiManager)
	{
		TWeakObjectPtr<class UGsUIWidgetBase> ticker = uiManager->GetWidgetByKey(TEXT("TrayTickerChaterOpen"));
		if (false == ticker.IsValid())
		{
			ticker = uiManager->OpenAndGetWidget(TEXT("TrayTickerChaterOpen"));
		}

		if (class UGsUITrayTickerChapterOpen* chapterAcceptTicker = Cast<UGsUITrayTickerChapterOpen>(ticker))
		{
			chapterAcceptTicker->SetData(chapterIndex, text);
		}
	}
}

void FGsQuestActionManagement::OnCloseSubDialog()
{
	CloseSubDialog();
}

void FGsQuestActionManagement::OnCloseNpcDialog()
{
	if (_delegateNpcDialog.IsBound())
	{
		_delegateNpcDialog.ExecuteIfBound(false, _gameId);
		_delegateNpcDialog.Unbind();
	}

	EndNpcDialog();
}

// 이벤트에서 npc 대화 요청 할때
void FGsQuestActionManagement::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	const FGsSchemaNpcDialogData* npcDialogData = UGsTableUtil::FindRowById<UGsTableNpcDialogData, FGsSchemaNpcDialogData>(_backupEventActionDialogId);
	if (npcDialogData == nullptr)
	{
		return;
	}

	UGsGameObjectBase* findObj =
		GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(_backupEventActionTargetNpcId);

	if (nullptr == findObj)
	{
		return;
	}

	int64 targetGameId = findObj->GetGameId();

	_npcDialogData = npcDialogData;
	_gameId = targetGameId;

	PlayNpcDialog(EGsCamearDialogEndCallBackType::EventAction);
}

// 퀘스트에서 npc 대화 요청 할때
void FGsQuestActionManagement::OpenNpcDialog(QuestId inQuestId, int64 inGameId, const FGsSchemaNpcDialogData* inDialogData, FNpcDialogEndDelegate const& inDelegate)
{
	_delegateNpcDialog = inDelegate;
	_npcDialogData = inDialogData;
	_gameId = inGameId;
	_questId = inQuestId;

	PlayNpcDialog(EGsCamearDialogEndCallBackType::Quest);
}

// npc다이얼로그 연출 시작
void FGsQuestActionManagement::PlayNpcDialog(EGsCamearDialogEndCallBackType inCamearDialogEndCallBackType)
{
	// 내 케릭터 위치 백업 ( npc다이얼로그 종료 시 내캐릭터 위치 복원 할 때 화면가려주는 연출을 위해 백업한다.)
	SaveLocalPlayerLocation();

#if WITH_EDITOR
	GSLOG(Log, TEXT("[FGsQuestActionManagement::OpenNpcDialog] quest -> dialog (id : %lld, questid : %d)"), _gameId, _questId);
#endif

	if (0 != _gameId)
	{
		_npcDialogIdTarget = GSGameObject()->FindObject(EGsGameObjectType::NonPlayer, _gameId);

		// B1: not use camera move(himichani)
		// https://jira.com2us.com/jira/browse/CHR-17798
		_isCameraWalking = false;
		TArray<UGsGameObjectBase*> arrTarget;
		if (_npcDialogData != nullptr &&
			_npcDialogIdTarget != nullptr)
		{
			// 페어리 숨김
			SetFairyHiddenInGame(true);

			const FGsSchemaCameraDialogViewData* viewData = _npcDialogData->dialogViewType.GetRow();
			if (viewData != nullptr)
			{
				_npcDialogCameraViewType = viewData;
				if (_npcDialogCameraViewType->type ==
					EGsDialogCameraViewType::DialogBackLocalMultiNpc)
				{
					int32 npcTblId;
					UGsGameObjectBase* targetNpc;
					for (auto iter : _npcDialogData->dialogMultiNpc)
					{
						npcTblId = iter.GetRow()->id;
						targetNpc = GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(npcTblId);
						arrTarget.Add(targetNpc);
					}

					if (0 < _npcDialogData->playerPosSpotId)
					{
						_isCameraWalking = false;
					}
				}
				else
				{
					arrTarget.Add(_npcDialogIdTarget);

				}




				// 페어리 스폰 처리
				const FGsSchemaNpcData* fairyData = _npcDialogData->fairyId.GetRow();
				if (fairyData != nullptr)
				{
					UGsGameObjectBase* localObject =
						GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
					GSClientSpawn()->SpawnDialogFairy(
						fairyData, localObject, _npcDialogData->fairyPosType);
				}

				// B1: not use camera move(himichani)
				// https://jira.com2us.com/jira/browse/CHR-17798
				// FadeIn 연출 시작
				float fadeInTime = GData()->GetGlobalData()->_cutSpotWarpFadeInTime;
				FGsUIHelper::TrayFadeIn(fadeInTime);
				//FadeInEndPlayNpcDialog();

				TArray<int> dialogSpawnObjects = GSClientSpawn()->GetDialogAllSpawnID();

				FGsCameraDialogMessageParam cameraDialogParam(
					arrTarget,
					inCamearDialogEndCallBackType,
					_npcDialogCameraViewType, dialogSpawnObjects, _isCameraWalking,
					_npcDialogData->playerPosSpotId,
					false,
					_npcDialogData->IsZoomFirstCameraMove);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_START, &cameraDialogParam);

				// npc 다이얼로그 발생 시 sub다이얼로그 중지
				OnCloseSubDialog();
			}
		}
	}
}

void FGsQuestActionManagement::OpenEventDialog(int In_targetNpcTblId, int In_npcDialogId)
{
	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqStartEvent(EGsEventProgressType::NPC_DIALOG, this);
	}

	_backupEventActionDialogId = In_npcDialogId;
	_backupEventActionTargetNpcId = In_targetNpcTblId;
}

void FGsQuestActionManagement::CameraLerpEndPlayNpcDialog(const IGsMessageParam* In_data)
{

	NpcDialogOpneUI();
}

void FGsQuestActionManagement::FadeInEndPlayNpcDialog()
{
	if (true == _isCameraWalking)
		return;

	NpcDialogOpneUI();
}

// npc대화 연출 시작
void FGsQuestActionManagement::NpcDialogOpneUI()
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[FGsQuestActionManagement::PlayNpcDialog] camera -> dialog (id : %lld, questid : %d)"), _gameId, _questId);
#endif

	if (nullptr != _questSubDialog)
	{
		_questSubDialog->OnStopVoice();
	}

	if (nullptr != _questNpcDialog)
	{
		_questNpcDialog->OpenDialog(TEXT("NpcDialog"), _npcDialogData, _npcDialogIdTarget, _npcDialogCameraViewType);
		_questNpcDialog->_delegateOnFinished.BindRaw(this, &FGsQuestActionManagement::EndNpcDialog);
	}
	else
	{
		if (0 < _npcDialogId)
		{
			_questNpcDialog->OpenDialog(TEXT("NpcDialog"), _npcDialogId, _npcDialogIdTarget, _npcDialogCameraViewType);
			_questNpcDialog->_delegateOnFinished.BindRaw(this, &FGsQuestActionManagement::EndNpcDialog);
		}
	}
}

void FGsQuestActionManagement::EndNpcDialog()
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[FGsQuestActionManagement::EndNpcDialog] dialog -> camera (id : %lld, questid : %d)"), _gameId, _questId);
#endif	

	if (_questNpcDialog)
	{
		if (false == _questNpcDialog->IsActive())
			return;
	}

	//// 내 위치가 변경 된 경우 페이드아웃 처리
	//if (false == IsLocationSame())
	//// FadeIn 연출 시작
	//float fadeInTime = GData()->GetGlobalData()->_cutSpotWarpFadeInTime;
	//FGsUIHelper::TrayFadeIn(fadeInTime);

	if (_npcDialogData)
	{
		float fadeInTime = _npcDialogData->closeFadeInTime;
		FGsUIHelper::TrayFadeIn(fadeInTime);
	}	

	if (0 != _gameId)
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_END, nullptr);
		GSClientSpawn()->FinishDialog();
	}

	// 페어리 출현
	SetFairyHiddenInGame(false);

	if (nullptr != _questNpcDialog)
	{
		_questNpcDialog->OnCloseDialog();
	}
}

// npc대화창 닫기
void FGsQuestActionManagement::CloseNpcDialog(const IGsMessageParam* In_data)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[FGsQuestActionManagement::CloseNpcDialog] dialog -> quest (id : %lld, questid : %d)"), _gameId, _questId);
#endif

	const FGsCameraDialogEndMessageParam* paramTarget =
		In_data->Cast<const FGsCameraDialogEndMessageParam>();
	if (nullptr == paramTarget ||
		EGsCamearDialogEndCallBackType::Quest != paramTarget->_cameraDialogCallbackType)
	{
		return;
	}

	if (_delegateNpcDialog.IsBound())
	{
		_delegateNpcDialog.ExecuteIfBound(true, _gameId);
		_delegateNpcDialog.Unbind();
	}
}

void FGsQuestActionManagement::SetFairyHiddenInGame(bool inValue)
{
	if (UGsGameObjectManager* objManager = GSGameObject())
	{
		if (UGsGameObjectBase* local = objManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
			if (FGsFairyHandlerPlayer* fairyHandler = castLocal->GetFairyHandler())
			{
				if (AGsActorFieldFairy* actorFairy = fairyHandler->GetFairyActor())
				{
					actorFairy->SetActorHiddenInGame(inValue);
				}
			}
		}
	}
}

void FGsQuestActionManagement::SaveLocalPlayerLocation()
{
	if (UGsGameObjectManager* objManager = GSGameObject())
	{
		if (UGsGameObjectBase* local = objManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
			_LocalPlayerPosBackup = castLocal->GetLocation();
		}
	}
}

bool FGsQuestActionManagement::IsLocationSame()
{
	if (UGsGameObjectManager* objManager = GSGameObject())
	{
		if (UGsGameObjectBase* local = objManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();

			return FVector::PointsAreSame(_LocalPlayerPosBackup, castLocal->GetLocation());
		}
	}

	return false;
}

void FGsQuestActionManagement::EventActionAllClearImmediate(const IGsMessageParam& Param)
{
	// 던전시간이 다 되었을때 불린다.
	// 대화중이였음 대화 종료
	OnCloseNpcDialog();
}

void FGsQuestActionManagement::OnLocalPlayerDie(const IGsMessageParam*)
{
	if (nullptr != _questNpcDialog)
	{
		if (_questNpcDialog->IsActive())
			EndNpcDialog();
	}
}

void FGsQuestActionManagement::OnHitLocal(const IGsMessageParam*)
{
	OnCloseNpcDialog();
}


void FGsQuestActionManagement::OnReconnection()
{
	OnCloseNpcDialog();
}


// 치트용 NpcDialog (스트링 효과 테스트)
void FGsQuestActionManagement::OpenNpcDialog(int32 inDialogId, float inVelocity)
{
	if (nullptr != _questNpcDialog)
	{
		_questNpcDialog->OpenDialog(TEXT("NpcDialog"), inDialogId, inVelocity);
	}
}

// 치트용 NpcDialog (대상이 있을 때 카메라 연출 on)
void FGsQuestActionManagement::OpenNpcDialog(int32 inDialogId, int32 npcTblId)
{
	TArray<UGsGameObjectBase*> listnpc = GSGameObject()->FindObjectArray(EGsGameObjectType::NonPlayer);

	FGsMessageHolder* msg = GMessage();

	int32 multiNpcTblId;
	UGsGameObjectBase* targetNpc;
	for (UGsGameObjectBase* el : listnpc)
	{
		if (FGsGameObjectData* objectData = el->GetData())
		{
			if (objectData->GetTableId() == npcTblId)
			{
				_npcDialogId = inDialogId;
				_npcDialogData = GetNpcDialogData(_npcDialogId);
				_gameId = objectData->GetGameId();
				_npcDialogIdTarget = el;

				bool isCameraWalking = true;
				if (_npcDialogData != nullptr)
				{
					const FGsSchemaCameraDialogViewData* viewData = _npcDialogData->dialogViewType.GetRow();
					if (viewData != nullptr)
					{
						TArray<UGsGameObjectBase*> arrTarget;
						if (viewData->type == EGsDialogCameraViewType::DialogBackLocalMultiNpc)
						{
							for (auto iter : _npcDialogData->dialogMultiNpc)
							{
								multiNpcTblId = iter.GetRow()->id;
								targetNpc = GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(multiNpcTblId);
								arrTarget.Add(targetNpc);
							}

							if (0 < _npcDialogData->playerPosSpotId)
							{
								isCameraWalking = false;
								
							}
						}
						else
						{
							arrTarget.Add(_npcDialogIdTarget);
						}


						// B1: not use camera move(himichani)
						// https://jira.com2us.com/jira/browse/CHR-17798
						// FadeIn 연출 시작
						float fadeInTime = GData()->GetGlobalData()->_cutSpotWarpFadeInTime;
						FGsUIHelper::TrayFadeIn(fadeInTime);
						//FadeInEndPlayNpcDialog();


						TArray<int> dialogSpawnObjects = GSClientSpawn()->GetDialogAllSpawnID();
						FGsCameraDialogMessageParam cameraDialogParam(arrTarget,
							EGsCamearDialogEndCallBackType::Quest,
							_npcDialogData->dialogViewType.GetRow(), dialogSpawnObjects, isCameraWalking,
							_npcDialogData->playerPosSpotId,
							false);
						msg->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_START, &cameraDialogParam);
					}
				}
				return;
			}
		}
	}
}

// 치트용 SchemaNpcDialogData
const FGsSchemaNpcDialogData* FGsQuestActionManagement::GetNpcDialogData(int32 InRowKey)
{
	const UGsTableNpcDialogData* table = Cast<UGsTableNpcDialogData>(FGsSchemaNpcDialogData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaNpcDialogData* schemaNpcDialogData = nullptr;
	if (false == table->FindRowById(InRowKey, schemaNpcDialogData))
	{
		return nullptr;
	}

	return schemaNpcDialogData;
}

// 워프전 처리
void FGsQuestActionManagement::OnLocalWarp(const IGsMessageParam* inParam)
{

	// https://jira.com2us.com/jira/browse/CHR-19928
	OnCloseNpcDialog();
}