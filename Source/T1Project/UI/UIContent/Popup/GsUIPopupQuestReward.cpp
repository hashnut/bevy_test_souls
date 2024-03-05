// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupQuestReward.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendServiceQuest.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsEventProgressManager.h"


void UGsUIPopupQuestReward::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupQuestReward::OnClickOk);

	// 퀘스트 완료창에서는 슬롯 선택기능을 사용 가능하게 켜준다.
	// ALL 타입 등 선택이 필요 없을 경우 UGsUIRewardInfoBase 내부 로직에서 클릭을 막는다.
	_rewardInfo->SetEnableToggleGroup(true);
}

void UGsUIPopupQuestReward::CloseInternal()
{
	if (FGsEventProgressManager* eventProgressMgr = GSEventProgress())
	{
		eventProgressMgr->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_QUESTCOMPLETE);
	}

	Super::CloseInternal();
}

void UGsUIPopupQuestReward::OnInputOk()
{
	if (false == SendRequestReward())
	{
		return;
	}

	Close();
}

void UGsUIPopupQuestReward::OnInputCancel()
{
	Close();
}

void UGsUIPopupQuestReward::SetData(const IGsMessageParam* InParam)
{
	Super::SetData(InParam);

	SetRewardData(_rewardMainId, _rewardSubId);
}

bool UGsUIPopupQuestReward::SendRequestReward()
{
	ItemBagIndex mainIndex = _rewardInfo->GetSelectedIndexMain();
	ItemBagIndex subIndex = _rewardInfo->GetSelectedIndexSub();

	if (RewardType::PICK_ONE == _rewardInfo->GetRewardTypeMain())
	{
		if (INVALID_ITEM_BAG_INDEX == mainIndex)
		{
			// TEXT: 보상을 선택한 뒤에 진행할 수 있습니다
			FText textMsg;
			FText::FindText(TEXT("QuestUIText"), TEXT("UI_Ticker_RequireSelect"), textMsg);
			FGsUIHelper::TrayMessageTicker(textMsg);

			return false;
		}
	}

	if (RewardType::PICK_ONE == _rewardInfo->GetRewardTypeSub() ||
		RewardType::SUB_PICK_ONE == _rewardInfo->GetRewardTypeSub())
	{
		if (INVALID_ITEM_BAG_INDEX == subIndex)
		{
			// TEXT: 보상을 선택한 뒤에 진행할 수 있습니다
			FText textMsg;
			FText::FindText(TEXT("QuestUIText"), TEXT("UI_Ticker_RequireSelect"), textMsg);
			FGsUIHelper::TrayMessageTicker(textMsg);

			return false;
		}
	}

	if (IsTest())
	{
		return true;
	}

	switch (_questType)
	{
	case QuestType::MAIN:
	{
		FGsNetSendServiceWorld::SendQuestMainReward(_questId, mainIndex, subIndex);
	}
	break;
	case QuestType::SUB:
	{
		FGsNetSendServiceWorld::SendQuestSubReward(_storyId, _questId, mainIndex, subIndex);
	}
	break;
	case QuestType::GUIDE:
	{
		FGsNetSendServiceWorld::SendQuestGuideReward(_questId, mainIndex, subIndex);
	}
	break;
	case QuestType::REPEAT:
	{
		FGsNetSendServiceWorld::SendQuestRepeatReward(_storyId, _questId, _questIndex,
			INVALID_ITEM_BAG_INDEX, INVALID_ITEM_BAG_INDEX);
	}
	break;
	case QuestType::GUILD:
	{
		// 주의: 길드 보상은 무조건 ALL타입이다
		FGsNetSendServiceQuest::SendReqQuestGuildReward(_questId);
	}
	break;
	}

	return true;
}
