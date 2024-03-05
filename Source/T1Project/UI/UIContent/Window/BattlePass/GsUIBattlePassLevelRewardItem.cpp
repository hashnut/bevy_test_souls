// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBattlePassLevelRewardItem.h"
#include "UI/UIContent/Common/Inventory/GsWrapBoxInventoryIconSelector.h"
#include "UI/UIControlLib//ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"

#include "Image.h"

void UGsUIBattlePassLevelRewardItem::BeginDestroy()
{
	if (nullptr != _basicRewardIconSlotHelper)
	{
		_basicRewardIconSlotHelper->OnCreateIcon.RemoveDynamic(this, &UGsUIBattlePassLevelRewardItem::OnCreateBasicRewardIcon);
		_basicRewardIconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIBattlePassLevelRewardItem::OnRefreshBasicRewardIcon);
		_basicRewardIconSlotHelper = nullptr;
	}

	if (nullptr != _addRewardIconSlotHelper)
	{
		_addRewardIconSlotHelper->OnCreateIcon.RemoveDynamic(this, &UGsUIBattlePassLevelRewardItem::OnCreateAddRewardIcon);
		_addRewardIconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIBattlePassLevelRewardItem::OnRefreshAddRewardIcon);
		_addRewardIconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}


void UGsUIBattlePassLevelRewardItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_basicRewardIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_basicRewardIconSlotHelper->Initialize(Cast<IGsIconSelectorInterface>(_basicRewardIconSelector));
	_basicRewardIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIBattlePassLevelRewardItem::OnCreateBasicRewardIcon);
	_basicRewardIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIBattlePassLevelRewardItem::OnRefreshBasicRewardIcon);
	_basicRewardIconSlotHelper->Clear();


	_addRewardIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_addRewardIconSlotHelper->Initialize(Cast<IGsIconSelectorInterface>(_addRewardIconSelector));
	_addRewardIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIBattlePassLevelRewardItem::OnCreateAddRewardIcon);
	_addRewardIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIBattlePassLevelRewardItem::OnRefreshAddRewardIcon);
	_addRewardIconSlotHelper->Clear();
}

void UGsUIBattlePassLevelRewardItem::NativeDestruct()
{
	_basicRewardIconSlotHelper->Clear();
	_addRewardIconSlotHelper->Clear();

	Super::NativeDestruct();
}

void UGsUIBattlePassLevelRewardItem::RefreshUI()
{
	if (auto data = _data.Pin())
	{
		_redDot->SetVisibility((data->IsEnableLevelReward() || data->IsEnableLevelAdditionalReward()) ? 
			ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		_runLevel->SetVisibility(data->IsProgressLevel() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		_levelText = FText::AsNumber(data->GetLevel());		
		_basicRewardIconSlotHelper->RefreshAll(1);

		FGsRewardUnboxingData addReward;
		data->GetAddtionalReward(addReward);
		{
			TArray<TPair<CurrencyType, Currency>> currencyList;
			addReward.GetCurrencyList(currencyList);

			_addRewardIconSlotHelper->RefreshAll(addReward._itemDataList.Num() + currencyList.Num());
		}
	}
}

void UGsUIBattlePassLevelRewardItem::OnCreateBasicRewardIcon(UGsUIIconBase* InIcon)
{
	if(auto itemIcon = Cast<UGsUIIconItemReward>(InIcon))
	{
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::LongPressType);
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIBattlePassLevelRewardItem::OnLongPressIcon);
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIBattlePassLevelRewardItem::OnRecvBasicReward);
	}
}

void UGsUIBattlePassLevelRewardItem::OnRefreshBasicRewardIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (auto data = _data.Pin())
	{
		FGsRewardUnboxingData reward;
		bool isEnableReward = data->GetRewardInfo(reward);
		{
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

				bool isRewared = !isEnableReward && data->IsOpenLevel();
				itemIcon->SetIsRewardNoti(isEnableReward);
				itemIcon->SetIsAcceptable(data->IsOpenLevel());
				//itemIcon->SetIsOpenReward(data->IsOpenLevel());
				itemIcon->SetIsRewarded(isRewared);
			}
		}
	}
}

void UGsUIBattlePassLevelRewardItem::OnCreateAddRewardIcon(UGsUIIconBase* InIcon)
{
	if(auto itemIcon = Cast<UGsUIIconItemReward>(InIcon))
	{
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::LongPressType);
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIBattlePassLevelRewardItem::OnLongPressIcon);
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIBattlePassLevelRewardItem::OnRecvAdditionalReward);
	}
}

void UGsUIBattlePassLevelRewardItem::OnRefreshAddRewardIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (auto data = _data.Pin())
	{
		FGsRewardUnboxingData reward;

		auto isUnlock = data->IsUnlockAddtionalReward();
		auto isEnableAddReward = data->GetAddtionalReward(reward);
		{
			UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
			if (itemIcon)
			{
				TArray<TPair<CurrencyType, Currency>> currencyList;
				reward.GetCurrencyList(currencyList);

				auto currencyIndex = InIndex - reward._itemDataList.Num();
				if (reward._itemDataList.IsValidIndex(InIndex))
				{
					itemIcon->SetNotOwnItem(reward._itemDataList[InIndex]->itemId, reward._itemDataList[InIndex]->itemCountMin);
				}
				else if(currencyIndex >= 0 && currencyList.IsValidIndex(currencyIndex))
				{
					itemIcon->SetCurrency(currencyList[currencyIndex].Key, currencyList[currencyIndex].Value);
				}

				auto activeLevel = (data->IsOpenLevel()); 

				_acceptable->SetVisibility(activeLevel ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

				bool isRewared = !isEnableAddReward && data->IsOpenLevel() && isUnlock;

				itemIcon->SetIsRewardNoti(isEnableAddReward);
				itemIcon->SetIsAcceptable(data->IsOpenLevel() && isUnlock);
				itemIcon->SetIsRewarded(isRewared);
				itemIcon->SetIsRewardLock(isUnlock == false);
				itemIcon->SetIsOpenReward(isUnlock);
			}
		}
	}
}

//reward 받으라고 알림
//reward closed
//reward 받았음
//reward lock 상태

void UGsUIBattlePassLevelRewardItem::OnRecvBasicReward(UGsUIIconItem& icon)
{
	if (auto data = _data.Pin())
	{
		FGsRewardUnboxingData reward;
		if (data->GetRewardInfo(reward))
		{
			OnReceiveReward.ExecuteIfBound(false, _data);
		}
	}
}

void UGsUIBattlePassLevelRewardItem::OnRecvAdditionalReward(UGsUIIconItem& icon)
{
	if (auto data = _data.Pin())
	{
		FGsRewardUnboxingData reward;
		if (data->GetAddtionalReward(reward))
		{
			OnReceiveReward.ExecuteIfBound(true, _data);
		}
	}
}
// 아이콘 상세보기 창
void UGsUIBattlePassLevelRewardItem::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
