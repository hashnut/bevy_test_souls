// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupQuestAccept.h"

#include "Net/GsNetSendServiceWorld.h"

#include "GameObject/Quest/GsQuestHandler.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/GsMessageHolder.h"

#include "DataCenter/Public/EventProgress/GsEventProgressDefine.h"

#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Option/GsServerOption.h"
#include "Message/GsMessageGameObject.h"

void UGsUIPopupQuestAccept::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 퀘스트 수락창에서는 슬롯 선택기능을 사용하지 않음
	_rewardInfo->SetEnableToggleGroup(false);
}

void UGsUIPopupQuestAccept::CloseInternal()
{
	if (FGsEventProgressManager* eventProgressMgr = GSEventProgress())
	{
		eventProgressMgr->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_QUESTACCEPT);
	}

	Super::CloseInternal();
}

void UGsUIPopupQuestAccept::OnInputOk()
{
	// TODO: 퀘스트 수락 버튼 클릭 처리

	if (_questType == QuestType::MAIN)
	{
		FGsNetSendServiceWorld::SendQuestMainAccept(_questId);
	}
	else if (_questType == QuestType::SUB)
	{
		if (GSQuest()->GetQuestCount(QuestType::SUB) >= MAX_ACTIVE_QUEST_SUB_COUNT)
		{
			// 퀘스트 동시 수행 횟수 초과
			FText findText;
			FText::FindText(TEXT("QuestUIText"), TEXT("ExceedSub"), findText);
			FGsUIHelper::TrayMessageTicker(findText);

			// Quest 조건에 부합하지 않아 캔들됨
			GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_QUEST_CANCEL, nullptr);

			Close();
			return;
		}

		FGsNetSendServiceWorld::SendQuestSubAccept(_storyId, _questId, _spawnId);
		Close(true);
		return;
	}
	else if (_questType == QuestType::GUIDE)
	{
		FGsNetSendServiceWorld::SendQuestGuideAccept(_questId);
		Close(true);
		return;
	}

	Close();
}

void UGsUIPopupQuestAccept::OnInputCancel()
{
	// 퀘스트 npc 대화 시작후(이벤트 프로그래스 등록)
	// 대화 연출 없이 나가는 구간이어서 
	// 여기서 이벤트 프로그레스에 초기화 호출을 해준다
	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::NPC_DIALOG);
	}

	if (_questType == QuestType::SUB)
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_QUEST_CANCEL, nullptr);
	}

	Close();
}

void UGsUIPopupQuestAccept::SetData(const IGsMessageParam* InParam)
{
	Super::SetData(InParam);

	SetRewardData(_rewardMainId, _rewardSubId);
}
