// Fill out your copyright notice in the Description page of Project Settings.

#include "GsQuestGuideInstance.h"
#include "GsQuestInstance.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Net/GsNetSendServiceWorld.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/GsMessageHolder.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Quest/Guide/GsSchemaQuestGuideStory.h"
#include "Quest/Table/GsQuestGuideTableCacheSet.h"

#include "Option/GsServerOption.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/GsMessageContents.h"


void FGsQuestGuideInstance::Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey)
{
	Super::Initialize(inOwner, inStoryId, inQuestKey);

	UGsQuestManager* questManger = GSQuest();
	if (nullptr == questManger)
		return;

	_questGuideTableCache = questManger->GetQuestGuideStoryTableCache(_questId);

	if (!_questGuideTableCache.IsValid())
		return;

	_schemaQuestStory = _questGuideTableCache->GetSchemaQuestStory();
	_storyId = inStoryId;
	_questType = QuestType::GUIDE;
}

void FGsQuestGuideInstance::Clear()
{
	_schemaQuestStory = nullptr;

	Super::Clear();
}

// 퀘스트hud에서 쓸 ui 상태 셋팅
void FGsQuestGuideInstance::SetQuestState(QuestState inState)
{
	switch (inState)
	{
	case QuestState::NONE:
		{
			const FGsNetUserData* userData = GGameData()->GetUserData();
			if (nullptr != userData)
			{
				int32 localPlayerLevel = userData->mLevel;

				_questUIState = (localPlayerLevel < _schemaQuestStory->requireLevel) ?
					EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED : EGsQuestUIState::QUEST_STATE_READY;
			}
		}
		break;
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
		GSLOG(Log, TEXT("[Quest] FGsQuestGuideInstance::SetQuestState QuestState::COMPLETED ID: %llu"), _questId);
#endif
		if (_questUIState == EGsQuestUIState::QUEST_STATE_PROGRESS)
		{
			SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_COMPLETED);			
		}

		_questUIState = EGsQuestUIState::QUEST_STATE_REWARD;	// 이곳을 눌러 보상을 받으세요. 상태로 만든다.

		FGsStoryParam param(_storyId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_GUIDE_COMPLETED, &param);		
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

const int32 FGsQuestGuideInstance::GetRequireLevel()
{
	// [R3] | ejrrb10 | 가이드 퀘스트가 HUD 에 등록되어 있을 때, PK 에 의해 레벨 다운시
	// {%s} 레벨 이상 진행할 수 있습니다 -> 부분의 {%s} 를 정확히 표기
	return _schemaQuestStory->requireLevel;
}

bool FGsQuestGuideInstance::IsShowHudUI() const
{
	// 침공서버에서는 침공 체크된 퀘스트만 표시
	if (GGameData()->IsInvadeWorld())
	{
		if (QuestContentsType::INVADE != _schemaQuest->questContentsType)
			return false;
	}

	FGsQuestGuideManagement* guideManagement = GSQuest()->GetQuestGuideManagement();
	if (nullptr == guideManagement)
		return false;

	return guideManagement->IsShow(_storyId);

	/*if (FGsServerOption* serverOption = GServerOption())
	{
		if (true == serverOption->IsHideQuestGuideUI(_storyId))
		{
			return false;
		}
	}*/

	return true;
}

bool FGsQuestGuideInstance::StartAutoProgress(bool inIsAuto)
{
	FGsStoryParam param(_storyId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_GUIDE_AUTO_PROGRESS, &param);

	return false;
}

// 퀘스트 타이틀 Text
void FGsQuestGuideInstance::GetTitleText(OUT FText& outTitleText)
{
	if (nullptr == _schemaQuestStory)
	{
		outTitleText = FText::GetEmpty();
		return;
	}
	outTitleText = _schemaQuestStory->storyTitleText;
}

// 퀘스트 설명 Text
void FGsQuestGuideInstance::GetDescText(OUT FText& outDescText)
{
	if (nullptr == _schemaQuestStory)
	{
		outDescText = FText::GetEmpty();
		return;
	}

	outDescText = _schemaQuestStory->storyDescriptionText;
}

// 메인 보상Id
const RewardId FGsQuestGuideInstance::GetMainRewardId() const
{
	if (nullptr == _schemaQuestStory)
		return 0;

	const FGsSchemaRewardData* rewardData = _schemaQuestStory->mainRewardId.GetRow();
	if (nullptr == rewardData)
		return 0;

	return rewardData->id;
}

// 서브 보상Id
const RewardId FGsQuestGuideInstance::GetSubRewardId() const
{
	if (nullptr == _schemaQuestStory)
		return 0;

	const FGsSchemaRewardData* rewardData = _schemaQuestStory->subRewardId.GetRow();
	if (nullptr == rewardData)
		return 0;

	return rewardData->id;
}

// 텔레포트 비용
bool FGsQuestGuideInstance::GetTeleportData(OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId) const
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

void FGsQuestGuideInstance::UpdateObjectiveScanType()
{
	for (FGsObjectiveData data :_objectiveDataList)
	{
		if (data._type == QuestObjectiveType::USE_RADAR_SCAN_SYSTEM)
		{
			if (GetQuestState() == QuestState::ACCEPTED)
			{
				FGsNetSendServiceWorld::SendQuestMainUpdate(_questId, _questKey._questIndex, data._objectiveId, 0, 0, 0);
			}
		}
	}
}

// [R3] | ejrrb10 | Guide Quest 에 대해, 레벨이 오르거나 할 때 QuestInstance 의 UIState 를 Update 해 주어야,
// 레벨이 올랐을 때 레벨하락으로 인해 수락할 수 없었던 가이드 퀘스트의 HUD 상태가 즉시 반영된다.
// Require Level 과 MainStory 둘 다를 체크해야 하지만, 실질적으로 메인 스토리가 롤백되는 상황은 발생할 수 없다.
// PK 를 통해 레벨이 떨어져 기존에 수행할 수 있었던 가이드 퀘스트를 수행하지 못하게 되고,
// 해당 변경 사항이 HUD 에 즉시 반영되지 않는 Edge Case 를 해결하기 위해 UpdateUIState 를 재정의한다.
void FGsQuestGuideInstance::UpdateUIState()
{
	if (_questUIState == EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED)
	{
		uint16 userLevel = 0;
		if (FGsGameDataManager* dataManager = GGameData())
		{
			if (const FGsNetUserData* userData = dataManager->GetUserData())
			{
				userLevel = userData->mLevel;
			}
		}

		StoryId currMainStoryId = GSQuest()->GetCurrentStoryId();
		if (0 >= currMainStoryId)
			return;

		if (userLevel >= _schemaQuestStory->requireLevel &&
			currMainStoryId >= _questGuideTableCache->GetRequireMainStoryId())
		{
			_questUIState = EGsQuestUIState::QUEST_STATE_READY;
		}
	}
	
}