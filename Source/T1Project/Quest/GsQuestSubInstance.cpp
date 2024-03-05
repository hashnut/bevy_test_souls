// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestSubInstance.h"
#include "GsQuestInstance.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/GsQuestTargetManagement.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Message/MessageParam/GsQuestMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Quest/Sub/GsSchemaQuestSubStory.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"

#include "Option/GsServerOption.h"

#include "UI/UIContent/Helper/GsUIHelper.h"


void FGsQuestSubInstance::Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey)
{
	Super::Initialize(inOwner, inStoryId, inQuestKey);

	UGsQuestManager* questManger = GSQuest();
	if (nullptr == questManger)
		return;

	if (!_questData.IsValid())
		return;

	FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(_questData.Get());
	if (nullptr == subData)
		return;

	_questSubTableCache = subData->GetQuestSubStoryTableCache();

	if (!_questSubTableCache.IsValid())
		return;

	_npcTblId = subData->GetNpcID();
	_schemaQuestStory = _questSubTableCache->GetSchemaQuestStory();
	_storyId = _schemaQuestStory->id;
	_questType = QuestType::SUB;
}

void FGsQuestSubInstance::Clear()
{
	_schemaQuestStory = nullptr;
	_npcTblId = 0;

	Super::Clear();
}

// 퀘스트hud에서 쓸 ui 상태 셋팅
void FGsQuestSubInstance::SetQuestState(QuestState inState)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		// 로그삽입
		return;
	}

	switch (inState)
	{
	case QuestState::NONE:
	case  QuestState::PRE_ACCEPT:
	break;
	case QuestState::ACCEPTED:
	{		
		if (QuestContentsType::INVADE == _schemaQuest->questContentsType)
		{
			_questUIState = GGameData()->IsInvadeWorld() ? 
				EGsQuestUIState::QUEST_STATE_PROGRESS : EGsQuestUIState::QUEST_STATE_INVADE_LOCKED;
		}
		else
		{
			_questUIState = EGsQuestUIState::QUEST_STATE_PROGRESS;
		}
	}
	break;
	case QuestState::COMPLETED:
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[Quest] FGsQuestSubInstance::SetQuestState QuestState::COMPLETED ID: %llu"), _questId);
#endif
		bool isStartActionEffect = (_questUIState == EGsQuestUIState::QUEST_STATE_PROGRESS)? true : false;

		// 다음 퀘스트?

		if (_questData.IsValid())
		{
			_nextQuestId = _questData->GetNextQuestId();
		}		

		if (0 >= _nextQuestId)
		{
			_questUIState = EGsQuestUIState::QUEST_STATE_REWARD;	// 이곳을 눌러 보상을 받으세요. 상태로 만든다.
			SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_REWARD);

			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::QUEST, false);
		}
		else
		{
			if (isStartActionEffect)
			{
				SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_COMPLETED);
			}

			AddNextQuest();
		}
	}
	break;
	case QuestState::REWARDED:
	case QuestState::MAX:
		break;
	default:
		break;
	}

	// 부모가 뒤에 유아이 타입 셋팅 끝나고
	Super::SetQuestState(inState);

	if(_questUIState == EGsQuestUIState::QUEST_STATE_REWARD)
	{
		if (FGsQuestTargetManagement* targetManagement = GSQuest()->GetQuestTargetManagement())
		{
			targetManagement->UpdateTartgetNpcInteractionMark(true, _npcTblId, EQuestNpcState::STATE_REWARD_WATING);
		}		
	}

	if (inState == QuestState::ACCEPTED)
	{
		FGsQuestParam param(_questId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_SUB_ACCEPTED, &param);
	}
	else if (inState == QuestState::COMPLETED)
	{
		FGsQuestParam param(_questId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_SUB_COMPLETED, &param);
	}
	else if (inState == QuestState::REWARDED)
	{
		FGsStoryParam param(_storyId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_SUB_REWARDED, &param);
	}
}

void FGsQuestSubInstance::PlayAcceptEffect()
{
	if (_questSubTableCache->GetFirstQuestId() == _questId)
	{
		if (UGsGameObjectBase * nonPlayer = GSGameObject()->FindObjectNearestNonPlayerByNPCId(
			_owner->GetActor(), _npcTblId))
		{
			OpenNpcDialog(nonPlayer->GetGameId());
		}
	}
	else
	{
		StartQuestEvent(QuestEventType::EVENT_QUEST_BEGIN);
	}
}

void FGsQuestSubInstance::OpenNpcDialog(const int64 inGameId)
{
	if (nullptr == _owner)
		return;

	if (nullptr == _schemaQuestStory)
		return;

	const FGsSchemaNpcDialogData* npcDialogData = _schemaQuestStory->questBeginNpcDialogId.GetRow();
	if (nullptr == npcDialogData)
		return;

	if (FGsQuestActionManagement* actionManagement = GSQuest()->GetQuestActionManagement())
	{
		actionManagement->OpenNpcDialog(_questId, inGameId, npcDialogData, nullptr);
	}
}

void FGsQuestSubInstance::AddNextStoryQuest()
{
	// 서브 퀘스트는 자동으로 다음 스토리를 진행 할 수없다.
	// 다음 스토리를 수락 받고 싶으면 npc 인터랙션을 통해야한다.
}

void FGsQuestSubInstance::AddNextQuest()
{
	if (0 >= _nextQuestId)
		return;

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Quest] FGsQuestInstance AddNextQuest ID: %llu"), _nextQuestId);
#endif

	GSQuest()->AddNextQuestSub(_storyId, _nextQuestId);
}

// 메인퀘스트의 퀘스트만 습득레벨이 있다. 습득레벨은 스토리단에서 결정된다.
const int32 FGsQuestSubInstance::GetRequireLevel()
{
	return 0;
}

bool FGsQuestSubInstance::IsShowHudUI() const
{
	bool isExposeQuest = GSQuest()->IsSubExposeQuest(_npcTblId, _storyId);
	if (false == isExposeQuest)
	{
		return false;
	}

	if (FGsServerOption* serverOption = GServerOption())
	{
		if (true == serverOption->IsHideQuestSubUI(_storyId))
		{
			return false;
		}
	}

	return Super::IsShowHudUI();
}

bool FGsQuestSubInstance::StartAutoProgress(bool inIsAuto)
{
	if (GLevel()->IsInstanceSingleMap())
	{		
		FText findText;
		FText::FindText(TEXT("QuestUIText"), TEXT("ps_no"), findText);
		FGsUIHelper::TrayMessageTicker(findText);		

		return false;
	}

	return Super::StartAutoProgress(inIsAuto);
}

// 스토리 타이틀 Text
void FGsQuestSubInstance::GetTitleText(OUT FText& outTitleText)
{
	if (nullptr == _schemaQuestStory)
	{
		outTitleText = FText::GetEmpty();
		return;
	}

	outTitleText = _schemaQuestStory->storyTitleText;
}

// 스토리 설명 Text
void FGsQuestSubInstance::GetDescText(OUT FText& outDescText)
{
	if (nullptr == _schemaQuestStory)
	{
		outDescText = FText::GetEmpty();
		return;
	}

	outDescText = _schemaQuestStory->storyDescriptionText;
}

// 메인 보상Id
const RewardId FGsQuestSubInstance::GetMainRewardId() const
{
	if (nullptr == _schemaQuestStory)
		return 0;

	const FGsSchemaRewardData* rewardData = _schemaQuestStory->mainRewardId.GetRow();
	if (nullptr == rewardData)
		return 0;

	return rewardData->id;
}

// 서브 보상Id
const RewardId FGsQuestSubInstance::GetSubRewardId() const
{	
	if (nullptr == _schemaQuestStory)
		return 0;

	const FGsSchemaRewardData* rewardData = _schemaQuestStory->subRewardId.GetRow();
	if (nullptr == rewardData)
		return 0;

	return rewardData->id;
}

// 텔레포트 비용
bool FGsQuestSubInstance::GetTeleportData(OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId) const
{
	if (GLevel()->IsInstanceSingleMap())
	{
		FText findText;
		FText::FindText(TEXT("QuestUIText"), TEXT("sb_no"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return false;
	}

	return Super::GetTeleportData(outTeleportGold, outObjectiveId);
}

void FGsQuestSubInstance::StartQuestEvent(QuestEventType inQuestEventType, int8 inindex)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Error, TEXT("[Quest] nullptr == UGsQuestManager"));
		return;
	}

	// 침공서버에서는 침공 체크된 퀘스트만 표시
	if (GGameData()->IsInvadeWorld())
	{
		if (QuestContentsType::INVADE == _schemaQuest->questContentsType)
		{
			if (questManager->IsUseRandomScroll() ||
				questManager->IsUseWarpScroll())
			{
				GSLOG(Warning, TEXT("[Quest] UseRandomScroll or UseWarpScroll"));
				return;
			}

			switch (inQuestEventType)
			{
			case QuestEventType::EVENT_PE_QUEST_BEGIN:
			{
				PrAcceptBeginEvent(inQuestEventType);
			}
			break;
			case QuestEventType::EVENT_QUEST_BEGIN:
			{			
				StartQuestBeginEvent(inQuestEventType);
			}
			break;
			case QuestEventType::EVENT_QUEST_CLEAR:
			{			
				StartQuestClearEvent(inQuestEventType);
			}
			break;
			case QuestEventType::EVENT_OBJECTIVE_CLEAR:
			{				
				StartObjectiveClearEvent(inQuestEventType, inindex);
			}
			break;
			default:
				break;
			}
		}
	}
}

void FGsQuestSubInstance::StartQuestClearEvent(QuestEventType inQuestEventType)
{
	int32 subDialogId = 0;
	if (nullptr != _schemaQuest)
	{
		if (const FGsSchemaSubDialogData* data = _schemaQuest->questClearSubDialogId.GetRow())
		{
			subDialogId = data->Id;
		}
	}

	// 서브 퀘스트는 서브다이얼로그 연출만 필요하여
	// 클리어 타입으로 서브타입만 부르는 경우가 없어서
	// 비긴 타입으로 부른다.
	FGsQuestBeginActionParam parm(QuestEventType::EVENT_QUEST_BEGIN, _questId, subDialogId);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_EVENT_BEGIN_START, &parm);
}