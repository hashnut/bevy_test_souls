// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestMainInstance.h"
#include "GsQuestInstance.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Table/GsQuestTableCache.h"
#include "Table/GsQuestMainTableCacheSet.h"

#include "Quest/Main/GsSchemaQuestMainStory.h"
#include "Quest/Management/GsQuestActionManagement.h"
#include "Quest/GsSchemaQuest.h"

#include "Event/EventAction/GsSchemaEventActionGroup.h"

#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/GsMessageContents.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "PlayableEvent/GsPlayableEventDefine.h"
#include "UI/UIContent/Tray/GsUITrayMovie.h"
#include "Data/GsDataContainManager.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#if WITH_EDITOR
#include "Cheat/GsCheatManager.h"
#endif


void FGsQuestMainInstance::Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey)
{
	Super::Initialize(inOwner, inStoryId, inQuestKey);

	_nextQuestId = 0;
	_isStoryStart = false;

	UGsQuestManager* questManger = GSQuest();
	if (nullptr == questManger)
		return;

	_questMainTableCache = questManger->GetQuestMainTableCache(_questId);	
	if (nullptr == _questMainTableCache)
		return;
	
	_questStoryTableCache = _questMainTableCache->GetCurrentQuestStoryTableCache();
	if (nullptr == _questStoryTableCache)
		return;
	
	_schemaQuestStory = _questStoryTableCache->GetSchemaQuestStory();
	_storyId = _schemaQuestStory->id;
	_questType = QuestType::MAIN;

	if (_questData.IsValid())
	{
		_questData->SetStoryId(_storyId);
	}

	RegisterMessages();
}

void FGsQuestMainInstance::RegisterMessages()
{
	MQuest& qusetMessage = GMessage()->GetQuest();
	_messageContentQuestList.Empty();
	_messageContentQuestList.Emplace(qusetMessage.AddRaw(MessageContentQuest::QUEST_EVENT_FINISHED, this, &FGsQuestMainInstance::OnFinishedQuestEvent));
}

void FGsQuestMainInstance::UnregisterMessages()
{
	MQuest& qusetMessage = GMessage()->GetQuest();
	for (TPair<MessageContentQuest, FDelegateHandle> contentQuestPair : _messageContentQuestList)
	{
		qusetMessage.Remove(contentQuestPair);
	}
	_messageContentQuestList.Empty();
}

void FGsQuestMainInstance::Clear()
{
	_questStoryTableCache = nullptr;
	_questMainTableCache = nullptr;
	_schemaQuestStory = nullptr;

	UnregisterMessages();

	Super::Clear();
}

// 메인퀘스트의 퀘스트만 습득레벨이 있다. 습득레벨은 스토리단에서 결정된다.
const int32 FGsQuestMainInstance::GetRequireLevel()
{
	if (nullptr == _schemaQuestStory)
		return 0;

	return _schemaQuestStory->requireLevel;
}

// 퀘스트hud에서 쓸 ui 상태 셋팅
void FGsQuestMainInstance::SetQuestState(QuestState inState)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Error, TEXT("[Quest] nullptr == UGsQuestManager"));
		return;
	}

	_nextQuestId = 0;

	switch (inState)
	{
	case QuestState::NONE:
	{		
		if (IsInitEventPlaying())
		{
#if WITH_EDITOR
			if (UGsCheatManager::IsEffectEmptyPlayer == false)
			{
				_questUIState = EGsQuestUIState::QUEST_STATE_READY;
				
			}
			else
			{
				StartQuestEvent(QuestEventType::EVENT_STROY_INIT_BEGIN);
			}
#else
			StartQuestEvent(QuestEventType::EVENT_STROY_INIT_BEGIN);
#endif	
		}
		else
		{			
			if (false == IsStoryRequireLevel())
			{
				_questUIState = EGsQuestUIState::QUEST_STATE_READY;

				if (_isStoryRewarded)
				{
					_isStoryRewarded = false;
					FGsStoryParam param(_storyId);
					GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_POPUP_ACCEPT, &param);					
				}
			}
			else
			{
				_questUIState = EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED;
			}
		}
	}
	break;
	case  QuestState::PRE_ACCEPT:
	{
		TSharedPtr<FGsQuestData> preQuestData = questManager->GetPreQuestData(_questId);
		if (preQuestData.IsValid())
		{
			_preQuestTableCache = preQuestData->GetQuestTableCache();
		}

		// 앞 퀘스트  QuestState::COMPLETED 일때 퀘스트를 수락 요청했어야 하는데
		// 제접속이나 여타 이유 때문에 QuestState::PRE_ACCEPT를 서버로 부터 받았다.
		// 앞 퀘스트에서 했어야 할 퀘스트 수락요청을 여기서 해줘야 한다.
		// 그러므로 해당 questId를 수락한다고 요청한다.
		_nextQuestId = _questId;
		StartQuestEvent(QuestEventType::EVENT_PE_QUEST_BEGIN);
	}
	break;
	case QuestState::ACCEPTED:
	{		
		if (IsGotoFieldType())
		{
			if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
			{
				eventProgressManager->OnReqStartEvent(EGsEventProgressType::QUEST_GOTO_FIELD, this);
			}
		}
		else
		{
			if (nullptr != _preQuestTableCache)
				_preQuestTableCache = nullptr;

			_questUIState = EGsQuestUIState::QUEST_STATE_PROGRESS;

			int8 num = _objectiveDataList.Num();
			for (int8 i = 0; i < num; i++)
			{
				FGsObjectiveData& objectiveData = _objectiveDataList[i];
				if (false == objectiveData._isComplete)
				{
					if (true == IsObjectiveCompleted(i))
					{
						StartQuestEvent(QuestEventType::EVENT_OBJECTIVE_CLEAR, i);
						break;
					}
				}
			}
		}		
	}
	break;
	case QuestState::COMPLETED:
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[Quest] FGsQuestMainInstance::SetQuestState QuestState::COMPLETED ID: %llu"), _questId);
#endif
		bool isStartActionEffect = (_questUIState == EGsQuestUIState::QUEST_STATE_PROGRESS) ? true : false;

		// 퀘스트가 완료 되었으니 다음 퀘스트를 찾아 습득하라고 매니저에 전달		
		if (!questManager->IsNextQuest(_questId, _nextQuestId))
		{
			_questUIState = EGsQuestUIState::QUEST_STATE_REWARD;	// 이곳을 눌러 보상을 받으세요. 상태로 만든다.
			SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_REWARD);

			if (isStartActionEffect)
			{
				_isStoryRewarded = true;
				FGsStoryParam param(_storyId);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_COMPLETE_REWARD, &param);
			}
		}
		else
		{
			if (isStartActionEffect)
			{
				SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_COMPLETED);
			}
		}

		//StartQuestEvent(QuestEventType::EVENT_QUEST_CLEAR);		
	}
	break;
	case QuestState::REWARDED:
	{	
		// 퀘스트 보상을 받았으니 다음 스토리에 퀘스트를 찾아 습득 하라고 매니저에 전달
		if (!questManager->IsNextStory(_questId, _nextQuestId))
			_questUIState = EGsQuestUIState::QUEST_STATE_NONE;	// 더이상 퀘스트가 없다

		bool isExposeQuest = questManager->IsCurrentMainExposeQuest();
		if (false == isExposeQuest)
		{
			_nextQuestId = INVALID_QUEST_ID;
			_questUIState = EGsQuestUIState::QUEST_STATE_NONE;	// 더이상 수행 가능 한 퀘스트가 없다
		}			

		// 스토리 종료 연출 시작
		StartQuestEvent(QuestEventType::EVENT_STROY_CLEAR);
	}
	break;
	case QuestState::MAX:
		break;
	default:
		break;
	}
	
	// 재접속 및 재연결시 여러번 연출이 플레이 되는걸 막기 위해
	// 스토리를 습득 하고 수락이 됐는지 체크해 연출을 플레이한다.
	_isStoryStart = (inState == QuestState::NONE) ? true : false;

	// 부모가 뒤에 유아이 타입 셋팅 끝나고
	Super::SetQuestState(inState);

	// 메인퀘스트 습득과 완료 시 메세지 알림
	if (inState == QuestState::ACCEPTED)
	{
		FGsQuestParam param(_questId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_MAIN_ACCEPTED, &param);
	}
	else if (inState == QuestState::COMPLETED)
	{
		FGsQuestParam param(_questId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_MAIN_COMPLETED, &param);
	}
	else if (inState == QuestState::REWARDED)
	{
		FGsStoryParam param(_storyId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_MAIN_REWARDED, &param);		
	}
	else if (inState == QuestState::NONE)
	{
		FGsStoryParam param(_storyId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_MAIN_READY, &param);
	}
}

bool FGsQuestMainInstance::IsStoryRequireLevel()
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr != userData)
	{
		int32 localPlayerLevel = userData->mLevel;

		return localPlayerLevel < _schemaQuestStory->requireLevel;
	}

	return false;
}


// 퀘스트hud에서 쓸 ui 상태 셋팅
void FGsQuestMainInstance::UpdateUIState(QuestState inState)
{
	return;
	switch (inState)
	{
	case QuestState::NONE:
	{		
		_questUIState = 
			IsStoryRequireLevel()? EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED: EGsQuestUIState::QUEST_STATE_READY;
	}
	break;
	case  QuestState::PRE_ACCEPT:
	{
	}
	break;
	case QuestState::ACCEPTED:
	{
		_questUIState = EGsQuestUIState::QUEST_STATE_PROGRESS;		
	}
	break;
	case QuestState::COMPLETED:
	{
		bool isStartActionEffect = (_questUIState == EGsQuestUIState::QUEST_STATE_PROGRESS) ? true : false;
		if (0 < _nextQuestId)
		{
			_questUIState = EGsQuestUIState::QUEST_STATE_REWARD;	// 이곳을 눌러 보상을 받으세요. 상태로 만든다.
			SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_REWARD);		
		}
		else
		{
			if (isStartActionEffect)
				SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_COMPLETED);
		}
	}
	break;
	case QuestState::REWARDED:
	{
		if (!GSQuest()->IsNextStory(_questId, _nextQuestId))
		{
			_questUIState = EGsQuestUIState::QUEST_STATE_NONE;	// 더이상 퀘스트가 없다
		}
	}
	break;
	case QuestState::MAX:
		break;
	default:
		break;
	}
}


void FGsQuestMainInstance::UpdateUIState()
{
	if (_questUIState == EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED)
	{
		const FGsNetUserData* userData = GGameData()->GetUserData();
		if (nullptr != userData)
		{
			int32 localPlayerLevel = userData->mLevel;

			_questUIState = (localPlayerLevel < _schemaQuestStory->requireLevel) ?
				EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED : EGsQuestUIState::QUEST_STATE_READY;
		}
	}

	return Super::UpdateUIState();
}

void FGsQuestMainInstance::PlayAcceptEffect()
{
	if (false == IsGotoFieldType())
	{
		if (_questStoryTableCache->GetFirstQuestId() == _questId)
		{
			StartQuestEvent(QuestEventType::EVENT_STROY_BEGIN);
		}
		else
		{
			StartQuestEvent(QuestEventType::EVENT_QUEST_BEGIN);
		}
	}
}

bool FGsQuestMainInstance::IsInitEventPlaying()
{
	if (!_questData.IsValid())
		return false;

	if (FGsQuestMainData* mainData = static_cast<FGsQuestMainData*>(_questData.Get()))
	{
		EventActionGroupId initEventActionGroupId = mainData->GetInitEventActionGroupId();
		if (0 < initEventActionGroupId)
		{
			return true;
		}
	}

	return false;
}

void FGsQuestMainInstance::StartInitEvent(QuestEventType inQuestEventType)
{
	if (!_questData.IsValid())
		return;

	if (FGsQuestMainData* mainData = static_cast<FGsQuestMainData*>(_questData.Get()))
	{
		EventActionGroupId initEventActionGroupId = mainData->GetInitEventActionGroupId();
		EventActionIndex initEventActionIndex = INVALID_EVENT_ACTION_INDEX;
		if (0 < initEventActionGroupId)
		{
			FGsQuestBeginActionParam parm(inQuestEventType, _questId, initEventActionGroupId, initEventActionIndex, true);
			GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_EVENT_BEGIN_START, &parm);
		}
	}
}

// 스토리 시작 연출
void FGsQuestMainInstance::StartStoryBeginEvent(QuestEventType inQuestEventType)
{
	int32 subDialogId = 0;
	if (nullptr != _schemaQuestStory)
	{
		if (const FGsSchemaSubDialogData* data = _schemaQuestStory->storyBeginSubDialogId.GetRow())
		{
			subDialogId = data->Id;
		}
	}
		
	FGsQuestBeginActionParam parm(inQuestEventType, _questId, subDialogId);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_EVENT_BEGIN_START, &parm);
}

// 스토리 종료 연출
void FGsQuestMainInstance::StartStoryClearEvent(QuestEventType inQuestEventType)
{	
	if (!_questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = _questData->GetQuestDynamicData();
	if (nullptr == dynamicData)
		return;

	EventActionIndex postStoryEventActionIndex = dynamicData->GetPostStoryEventActionIndex();

	EventActionGroupId postStoryEventActionGroupId = 0;
	if (nullptr != _schemaQuestStory)
	{
		if (const FGsSchemaEventActionGroup* eventActionGroup = _schemaQuestStory->postStoryEventActionGroupId.GetRow())
		{
			postStoryEventActionGroupId = eventActionGroup->id;
		}
	}

	int32 subDialogId = 0;
	if (nullptr != _schemaQuestStory)
	{
		if (const FGsSchemaSubDialogData* data = _schemaQuestStory->storyClearSubDialogId.GetRow())
		{
			subDialogId = data->Id;
		}
	}	

	GSLOG(Log, TEXT("[Quest] QuestMainInstance StoryClearEvent -> questId : %llu, subDialogId : %d, postStoryEventActionGroupId : %d, postStoryEventActionIndex : %d"),
		_questId, subDialogId, postStoryEventActionGroupId, postStoryEventActionIndex);

	FGsQuestClearActionParam parm(inQuestEventType, _questId, subDialogId, postStoryEventActionGroupId, postStoryEventActionIndex);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_EVENT_CLEAR_START, &parm);
}

void FGsQuestMainInstance::StartQuestEvent(QuestEventType inQuestEventType, int8 inindex)
{	
	Super::StartQuestEvent(inQuestEventType, inindex);

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Error, TEXT("[Quest] nullptr == UGsQuestManager"));
		return;
	}

	// 침공에서 침공아닌 퀘스트의 이벤트가 발생하는 경우를 막기 위해
	// 상태처리하는곳에서 NONE으로 뒀는데
	// 그럼 NONE으로써 동작하는 기능들이 있어서
	// 상태는 업데이트 하고 이벤트처리하는 곳에서만 침공체크 추가
	if (GGameData()->IsInvadeWorld())
	{
		if (QuestContentsType::INVADE != _schemaQuest->questContentsType)
			return;
	}

	if (questManager->IsUseRandomScroll() ||
		questManager->IsUseWarpScroll())
	{
		GSLOG(Warning, TEXT("[Quest] UseRandomScroll or UseWarpScroll"));
		return;
	}

	switch (inQuestEventType)
	{
	case QuestEventType::EVENT_STROY_INIT_BEGIN:
		{
			_questUIState = EGsQuestUIState::QUEST_STATE_NONE;
			StartInitEvent(inQuestEventType);
		}
		break;
	case QuestEventType::EVENT_STROY_BEGIN:
		{
			StartStoryBeginEvent(inQuestEventType);
		}
		break;
	case QuestEventType::EVENT_STROY_CLEAR:
		{
			StartStoryClearEvent(inQuestEventType);
		}
		break;
	case QuestEventType::EVENT_PE_QUEST_BEGIN:
		{
			// 이벤트 시작 시 슬롯 잠금
			SetTouchLock(true);
			PrAcceptBeginEvent(inQuestEventType);
		}
		break;
	case QuestEventType::EVENT_QUEST_BEGIN:
		{
			// 이벤트 시작 시 슬롯 잠금
			SetTouchLock(true);
			StartQuestBeginEvent(inQuestEventType);
		}
		break;
	case QuestEventType::EVENT_QUEST_CLEAR:
		{
			// 이벤트 시작 시 슬롯 잠금
			SetTouchLock(true);
			StartQuestClearEvent(inQuestEventType);
		}
		break;
	case QuestEventType::EVENT_OBJECTIVE_CLEAR:
		{
			// 이벤트 시작 시 슬롯 잠금
			SetTouchLock(true);
			StartObjectiveClearEvent(inQuestEventType, inindex);
		}
		break;
	default:
		break;
	}
}

void FGsQuestMainInstance::OnFinishedQuestEvent(const struct IGsMessageParam* inData)
{
	const FGsFinishedQuestEventParam* param =
		inData->Cast<const FGsFinishedQuestEventParam>();

	if (param->_questId != _questId)
		return;

	// 이벤트 종료 시 슬롯 잠금 해제
	SetTouchLock(false);

	QuestEventType questEventType = param->_questEventType;
	switch (questEventType)
	{
	case QuestEventType::EVENT_STROY_INIT_BEGIN:
		{
			_questUIState = EGsQuestUIState::QUEST_STATE_READY;

			// ui 갱신
			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
			FGsStoryParam storyParam(_storyId);
			GMessage()->GetTutorial().SendMessage(MessageContentTutorial::FIRST_TUTORIAL, &storyParam);
		}
		break;
	case QuestEventType::EVENT_STROY_BEGIN:
		{
			StartQuestEvent(QuestEventType::EVENT_QUEST_BEGIN);
		}
		break;
	case QuestEventType::EVENT_STROY_CLEAR:
		{
			AddNextStoryQuest();
		}
		break;
	case QuestEventType::EVENT_PE_QUEST_BEGIN:
		{
			AcceptQuest();
		}
		break;
	case QuestEventType::EVENT_QUEST_BEGIN:
		break;
	case QuestEventType::EVENT_QUEST_CLEAR:
		{
			AddNextQuest();
		}
		break;
	case QuestEventType::EVENT_OBJECTIVE_CLEAR:
		break;
	default:
		break;
	}
}

void FGsQuestMainInstance::SetQuestDynamicEventUpdate(QuestId inQuestId, bool isPreEventAction)
{
	TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
	if (nullptr == dynamicData)
	{
		dynamicData = questData->ClaimQuestDynamicData(_questKey._questIndex);
	}
}

void FGsQuestMainInstance::AcceptQuest()
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[Quest] FGsQuestMainInstance AcceptQuest ID: %llu"), _questId);
#endif
	GSQuest()->AddNextQuest(_questId);
}

bool FGsQuestMainInstance::StartAutoProgress(bool inIsAuto)
{
	return Super::StartAutoProgress(inIsAuto);
}

// 스토리 타이틀 Text
void FGsQuestMainInstance::GetTitleText(OUT FText& outTitleText)
{
	if (nullptr == _schemaQuestStory)
	{
		outTitleText = FText::GetEmpty();
		return;
	}
	
	outTitleText = _schemaQuestStory->storyTitleText;	
}

// 스토리 설명 Text
void FGsQuestMainInstance::GetDescText(OUT FText& outDescText)
{
	if (nullptr == _schemaQuestStory)
	{
		outDescText = FText::GetEmpty();
		return;
	}

	outDescText = _schemaQuestStory->storyDescriptionText;
}

// 메인 보상Id
const RewardId FGsQuestMainInstance::GetMainRewardId() const
{
	if (nullptr == _schemaQuestStory)
		return 0;

	const FGsSchemaRewardData* rewardData = _schemaQuestStory->mainRewardId.GetRow();
	if (nullptr == rewardData)
		return 0;
	
	return rewardData->id;
}

// 서브 보상Id
const RewardId FGsQuestMainInstance::GetSubRewardId() const
{
	if (nullptr == _schemaQuestStory)
		return 0;

	const FGsSchemaRewardData* rewardData = _schemaQuestStory->subRewardId.GetRow();
	if (nullptr == rewardData)
		return 0;

	return rewardData->id;
}

// 텔레포트 비용
bool FGsQuestMainInstance::GetTeleportData(OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId) const
{
	return Super::GetTeleportData(outTeleportGold, outObjectiveId);
}

// 이벤트 시작
void FGsQuestMainInstance::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	GSLOG(Log, TEXT("FGsQuestMainInstance::StartEvent EventProgress Call - quest id : %lld"), _questId);

	if(In_startType == EGsEventProgressType::QUEST_GOTO_FIELD)
		OnGotoField();
}

void FGsQuestMainInstance::OnPostGotoField()
{
	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::QUEST_GOTO_FIELD, false);
	}
}

bool FGsQuestMainInstance::IsShowHudUI() const
{
	bool isExposeQuest = GSQuest()->IsCurrentMainExposeQuest();
	if(false == isExposeQuest)
	{
		return false;
	}

	if (GGameData()->IsInvadeWorld())
	{
		if (QuestContentsType::INVADE != _schemaQuest->questContentsType)
			return false;
	}

	return Super::IsShowHudUI();
}