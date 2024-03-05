// Fill out your copyright notice in the Description page of Project Settings.

#include "GsQuestGuildInstance.h"
#include "GsQuestInstance.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Message/MessageParam/GsGuildMessageParam.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Quest/Table/GsQuestGuildTableCacheSet.h"
#include "Quest/Guild/GsSchemaQuestGuildStory.h"
#include "Option/GsServerOption.h"


void FGsQuestGuildInstance::Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey)
{
	Super::Initialize(inOwner, inStoryId, inQuestKey);

	UGsQuestManager* questManger = GSQuest();
	if (nullptr == questManger)
		return;

	TSharedPtr<FGsQuestGuildStoryTableCache> cacheTable = questManger->GetQuestGuildStoryTableCache(inStoryId);
	_schemaQuestStory = cacheTable->GetSchemaQuestStory();

	_storyId = inStoryId;
	_questType = QuestType::GUILD;
}

void FGsQuestGuildInstance::Clear()
{
	_schemaQuestStory = nullptr;

	Super::Clear();
}

// 퀘스트hud에서 쓸 ui 상태 셋팅
void FGsQuestGuildInstance::SetQuestState(QuestState inState)
{
	bool bSendMessage = false;

	switch (inState)
	{
	case QuestState::ACCEPTED:
	{
		_questUIState = EGsQuestUIState::QUEST_STATE_PROGRESS;
		
		// 진행중인 수치 업데이트
		bSendMessage = true;		
	}
	break;
	case QuestState::COMPLETED:
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[Quest] FGsQuestGuildInstance::SetQuestState QuestState::COMPLETED ID: %llu"), _questId);
#endif
		if (_questUIState == EGsQuestUIState::QUEST_STATE_PROGRESS)
		{
			SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_COMPLETED);			
		}

		// 다른 퀘스트 상태들은 패킷 핸들러 쪽에서 수행함
		// ACCEPTED는 퀘스트 카운트 증가시마다 들어오기 때문이고 
		// REWARDED, GiveUp 시에는 RemoveInstance하며 이 함수에 안들어올 가능성이 있기 때문임
		if (FGsGuildManager* guildMgr = GGuild())
		{
			if (FGsGuildQuestDataHandler* guildQuestDataHandler = guildMgr->GetQuestHandler())
			{
				guildQuestDataHandler->SetGuildQuestState(_storyId, _questId, QuestState::COMPLETED);
			}
		}

		// 보상받기 상태로 갱신 위함
		_questUIState = EGsQuestUIState::QUEST_STATE_REWARD;	// 이곳을 눌러 보상을 받으세요. 상태로 만든다.

		bSendMessage = true;
	}
	break;
	default:
		break;
	}

	// 부모가 뒤에 유아이 타입 셋팅 끝나고
	Super::SetQuestState(inState);

	if (bSendMessage)
	{
		FGsGuildMsgParamQuest param(_storyId, _questId, false);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::UPDATE_QUEST, &param);
	}
}

const int32 FGsQuestGuildInstance::GetRequireLevel()
{
	return 0;
}

bool FGsQuestGuildInstance::StartAutoProgress(bool inIsAuto)
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

// 퀘스트 타이틀 Text
void FGsQuestGuildInstance::GetTitleText(OUT FText& outTitleText)
{
	if (nullptr == _schemaQuestStory)
	{
		outTitleText = FText::GetEmpty();
		return;
	}
	outTitleText = _schemaQuestStory->questTitleText;
}

// 퀘스트 설명 Text
void FGsQuestGuildInstance::GetDescText(OUT FText& outDescText)
{
	if (nullptr == _schemaQuestStory)
	{
		outDescText = FText::GetEmpty();
		return;
	}

	outDescText = _schemaQuestStory->questDescriptionText;
}

// 메인 보상Id
const RewardId FGsQuestGuildInstance::GetMainRewardId() const
{
	if (_schemaQuestStory)
	{
		if (const FGsSchemaRewardData* rewardData = _schemaQuestStory->mainRewardId.GetRow())
		{
			return rewardData->id;
		}
	}

	return 0;
}

// 서브 보상Id
const RewardId FGsQuestGuildInstance::GetSubRewardId() const
{
	if (_schemaQuestStory)
	{
		if (const FGsSchemaRewardData* rewardData = _schemaQuestStory->subRewardId.GetRow())
		{
			return rewardData->id;
		}
	}

	return 0;
}

bool FGsQuestGuildInstance::IsShowHudUI() const
{
	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		if (unlockManager->IsContentsUnlock(EGsUnlockContentsType::GUILD_QUEST_TAB, false))
		{
			return Super::IsShowHudUI();
		}
	}

	return false;
}

// 텔레포트 비용
bool FGsQuestGuildInstance::GetTeleportData(OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId) const
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