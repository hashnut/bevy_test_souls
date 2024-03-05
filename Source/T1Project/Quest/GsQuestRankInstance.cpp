// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestRankInstance.h"
#include "GsQuestInstance.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Management/GsMessageHolder.h"


void FGsQuestRankInstance::Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey)
{
	Super::Initialize(inOwner, inStoryId, inQuestKey);

	_storyId = inStoryId;
	_questType = QuestType::MULTI_LEVEL_RANK;
}

void FGsQuestRankInstance::SetQuestState(QuestState inState)
{
	// 부모가 뒤에 유아이 타입 셋팅 끝나고
	Super::SetQuestState(inState);

	// 반복퀘스트 습득과 완료 시 메세지 알림
	if (inState == QuestState::ACCEPTED)
	{
		FGsQuestRankParam param(_questId, _storyId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_RANK_ACCEPTED, &param);
	}
	else if (inState == QuestState::COMPLETED)
	{
		FGsQuestRankParam param(_questId, _storyId);
		GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_RANK_COMPLETED, &param);
	}
}


bool FGsQuestRankInstance::IsShowHudUI() const
{
	return false;	
}

bool FGsQuestRankInstance::StartAutoProgress(bool inIsAuto)
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
