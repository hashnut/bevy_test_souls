// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestRepeatInstance.h"
#include "GsQuestInstance.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Table/GsQuestRepeatTableCacheSet.h"
#include "Quest/Repeat/GsSchemaQuestRepeatStory.h"

#include "Option/GsServerOption.h"

#include "UI/UIContent/Helper/GsUIHelper.h"


void FGsQuestRepeatInstance::Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey)
{
	Super::Initialize(inOwner, inStoryId, inQuestKey);

	UGsQuestManager* questManger = GSQuest();
	if (nullptr == questManger)
		return;

	_questRepeatTableCache = questManger->GetQuestRepeatStoryTableCache(inStoryId);

	if (!_questRepeatTableCache.IsValid())
		return;

	_schemaQuestStory = _questRepeatTableCache->GetSchemaQuestStory();
	_storyId = inStoryId;
	_questType = QuestType::REPEAT;

	// 유저가 정한 viewIndex 가 있으면 적용
}

void FGsQuestRepeatInstance::Clear()
{
	_schemaQuestStory = nullptr;

	Super::Clear();
}

// 퀘스트hud에서 쓸 ui 상태 셋팅
void FGsQuestRepeatInstance::SetQuestState(QuestState inState)
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
		_questUIState = EGsQuestUIState::QUEST_STATE_PROGRESS;
	}
	break;
	case QuestState::COMPLETED:
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[Quest] FGsQuestRepeatInstance::SetQuestState QuestState::COMPLETED ID: %llu"), _questId);
#endif
		bool isStartActionEffect = (_questUIState == EGsQuestUIState::QUEST_STATE_PROGRESS) ? true : false;

		// 다음 퀘스트?
		bool isNextQuest = questManager->IsNextQuestRepeat(_storyId, _questId, _nextQuestId);
		if (false == isNextQuest)
		{
			_questUIState = EGsQuestUIState::QUEST_STATE_REWARD;	// 이곳을 눌러 보상을 받으세요. 상태로 만든다.
			SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_REWARD);

			// 만약 현재 자동진행 중이던 퀘스트가 나 자신이면
			// 다음 퀘스트를 자동 진행하라고 상태값을 true로 바꿔둔다.
			QuestKey questKey = questManager->GetAutoProgressQuestKey();
			if (questKey == _questKey)
			{
				questManager->SetIsRepeatQuestAutoPressing(true);
			}			

			if (FGsServerOption* serverOption = GServerOption())
			{
				serverOption->SetRemoveQuestRepeatViewIndexList(0, _questKey);
			}

			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::QUEST, false);
			GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_REPEAT_COMPLETED, nullptr);
		}
		else
		{
			if (isStartActionEffect)
			{
				SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_COMPLETED);
			}
			else
			{
				AddNextQuest();
			}
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
}

void FGsQuestRepeatInstance::AddNextQuest()
{
	if (0 >= _nextQuestId)
		return;

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Quest] FGsQuestRepeatInstance AddNextQuest ID: %llu"), _nextQuestId);
#endif

	GSQuest()->AddNextQuestRepeat(_storyId, _nextQuestId);
}

const int32 FGsQuestRepeatInstance::GetRequireLevel()
{
	return 0;
}

bool FGsQuestRepeatInstance::IsShowHudUI() const
{
	if (FGsServerOption* serverOption = GServerOption())
	{
		if (true == serverOption->IsHideQuestRepeatUI(_questKey._questIndex))
		{
			return false;
		}
	}

	return Super::IsShowHudUI();
}

bool FGsQuestRepeatInstance::StartAutoProgress(bool inIsAuto)
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
void FGsQuestRepeatInstance::GetTitleText(OUT FText& outTitleText)
{
	if (nullptr == _schemaQuestStory)
	{
		outTitleText = FText::GetEmpty();
		return;
	}

	outTitleText = _schemaQuestStory->storyTitleText;
}

// 스토리 설명 Text
void FGsQuestRepeatInstance::GetDescText(OUT FText& outDescText)
{
	if (nullptr == _schemaQuestStory)
	{
		outDescText = FText::GetEmpty();
		return;
	}

	outDescText = _schemaQuestStory->storyDescriptionText;
}

// 메인 보상Id
const RewardId FGsQuestRepeatInstance::GetMainRewardId() const
{
	if (nullptr == _schemaQuestStory)
		return 0;

	const FGsSchemaRewardData* rewardData = _schemaQuestStory->mainRewardId.GetRow();
	if (nullptr == rewardData)
		return 0;

	return rewardData->id;
}

// 서브 보상Id
const RewardId FGsQuestRepeatInstance::GetSubRewardId() const
{
	if (nullptr == _schemaQuestStory)
		return 0;

	const FGsSchemaRewardData* rewardData = _schemaQuestStory->subRewardId.GetRow();
	if (nullptr == rewardData)
		return 0;

	return rewardData->id;
}

// 텔레포트 비용
bool FGsQuestRepeatInstance::GetTeleportData(OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId) const
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

RepeatStoryType FGsQuestRepeatInstance::GetRepeatStoryType() const
{
	if (nullptr == _schemaQuestStory)
		return RepeatStoryType::BOARD;

	return _schemaQuestStory->repeatStoryType;
}