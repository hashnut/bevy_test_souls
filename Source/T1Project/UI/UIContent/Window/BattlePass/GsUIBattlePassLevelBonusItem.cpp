// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIBattlePassLevelBonusItem.h"
#include "UI/UIContent/Common/Inventory/GsWrapBoxInventoryIconSelector.h"
#include "UI/UIControlLib//ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"

#include "BattlePass/GsBattlePassLevelData.h"

#include "Image.h"

void UGsUIBattlePassLevelBonusItem::BeginDestroy()
{
	if (nullptr != _rewardIconSlotHelper)
	{
		_rewardIconSlotHelper->OnCreateIcon.RemoveDynamic(this, &UGsUIBattlePassLevelBonusItem::OnCreateBonusRewardIcon);
		_rewardIconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIBattlePassLevelBonusItem::OnRefreshBonusRewardIcon);
		_rewardIconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBattlePassLevelBonusItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_rewardIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_rewardIconSlotHelper->Initialize(Cast<IGsIconSelectorInterface>(_rewardIconSelector));
	_rewardIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIBattlePassLevelBonusItem::OnCreateBonusRewardIcon);
	_rewardIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIBattlePassLevelBonusItem::OnRefreshBonusRewardIcon);
	_rewardIconSlotHelper->Clear();
}

void UGsUIBattlePassLevelBonusItem::NativeDestruct()
{
	_rewardIconSlotHelper->Clear();
	Super::NativeDestruct();
}

void UGsUIBattlePassLevelBonusItem::RefreshUI()
{
	if (auto data = _data.Pin())
	{
		_redDot->SetVisibility(data->IsEnableBonus() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		_rewardIconSlotHelper->RefreshAll(1);

		_textAmount = FText::AsNumber(data->GetRewardableBonusCount());
	}
}

void UGsUIBattlePassLevelBonusItem::OnCreateBonusRewardIcon(UGsUIIconBase* InIcon)
{
	if (auto itemIcon = Cast<UGsUIIconItemReward>(InIcon))
	{
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::LongPressType);
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIBattlePassLevelBonusItem::OnLongPressIcon);
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIBattlePassLevelBonusItem::OnRecvBonusReward);
	}
}

void UGsUIBattlePassLevelBonusItem::OnRefreshBonusRewardIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (auto data = _data.Pin())
	{
		FGsRewardUnboxingData reward = data->GetRewardData();

		UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
		if (itemIcon)
		{
			if (reward._itemDataList.IsValidIndex(0))
			{
				itemIcon->SetNotOwnItem(reward._itemDataList[0]->itemId, reward._itemDataList[0]->itemCountMin);
			}
			else if (reward.IsCurrency())
			{
				itemIcon->SetCurrency(reward._currencyType, reward.GetCommonCurrencyAmount());
			}

			itemIcon->SetIsRewardNoti(data->IsEnableBonus());
			itemIcon->SetIsAcceptable(data->IsOpenBonus());
			itemIcon->SetIsRewarded(data->IsRewardedBonus());

			_textAmount = FText::FromString(FString::Format(TEXT("X{0}"), { data->GetRewardableBonusCount() }));
		}
	}
}

void UGsUIBattlePassLevelBonusItem::OnRecvBonusReward(UGsUIIconItem& icon)
{
	if (auto data = _data.Pin())
	{
		if (data->IsEnableBonus())
		{ 
			OnReceiveReward.ExecuteIfBound(_data);
		}
	}
}

void UGsUIBattlePassLevelBonusItem::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
