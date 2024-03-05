// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupBattlePassBuyLevel.h"
#include "BattlePass/GsBattlePassData.h"
#include "BattlePass/GsBattlePassLevelData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"

#include "Currency/GsCurrencyHelper.h"
#include "Item/GsItemManager.h"

#include "Components/TextBlock.h"

#include "Algo/Transform.h"


void UGsUIPopupBattlePassBuyLevel::BeginDestroy()
{	
	if (nullptr != _listHelper)
	{
		_listHelper->OnCreateIcon.RemoveDynamic(this, &UGsUIPopupBattlePassBuyLevel::OnCreateIcon);
		_listHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIPopupBattlePassBuyLevel::OnRefreshIcon);
		_listHelper = nullptr;
	}
	Super::BeginDestroy();
}

void UGsUIPopupBattlePassBuyLevel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_listHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_listHelper->Initialize(_iconSelector);
	_listHelper->OnCreateIcon.AddDynamic(this, &UGsUIPopupBattlePassBuyLevel::OnCreateIcon);
	_listHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupBattlePassBuyLevel::OnRefreshIcon);
}

void UGsUIPopupBattlePassBuyLevel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIPopupBattlePassBuyLevel::NativeDestruct()
{
	_iconSelector->RemoveAllChildren();
	Super::NativeDestruct();
}

void UGsUIPopupBattlePassBuyLevel::SetInit(InitParam* param, TFunction<void(bool, bool, Level)> callback)
{
	_buttonCallback = callback;
	_curPassData = nullptr;
	if (param)
	{
		_curPassData = param->curPassData;

		if (auto passData = _curPassData.Pin())
		{
			if (auto level = passData->GetProgressLevelData().Pin())
			{
				_curLevel = level->GetLevel() + 1;
				_minLevel = _curLevel;
				_maxLevel = passData->GetMaxLevel();

				_textPreLevel = FText::FromString(FString::Format(TEXT("Lv.{0}"), { level->GetLevel() }));

				RefreshUI();
			}
		}
	}
}

void UGsUIPopupBattlePassBuyLevel::RefreshUI()
{
	if (auto passData = _curPassData.Pin())
	{
		if (auto level = passData->FindLevelData(_curLevel).Pin())
		{
			auto pay = passData->GetLevelOpenPay(_curLevel);

			auto num = GetLevelupRewards();
			_listHelper->RefreshAll(num);

			_amountText = FText::FromString(FString::Format(TEXT("{0} / {1}"), { _curLevel,_maxLevel }));
			_textNxtLevel = FText::FromString(FString::Format(TEXT("Lv.{0}"), { _curLevel }));

			if (pay._payType == pay.CURRENCY)
			{
				Currency amount = FGsCurrencyHelper::GetCurrencyAmount(pay._currency.type);

				_currencyCommonPay->SetData(pay._currency.type, pay._currency.amount);
				_currencyCommonPay->SetAmountTextColor(int64(amount) >= pay._currency.amount ? 
					EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
				_currencyLocalPay->SetData(pay._currency.type, amount);
			}
			else if (pay._payType == pay.ITEM)
			{
				ItemAmount amount = GItem()->GetAllItemCountByTID(pay._item.itemId);

				_currencyCommonPay->SetData(pay._item.itemId, pay._item.amount);
				_currencyCommonPay->SetAmountTextColor(amount >= pay._item.amount ? 
					EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
				_currencyLocalPay->SetData(pay._item.itemId, amount);
			}

			_btnMin->SetIsEnabled(_curLevel > _minLevel);
			_btnPlus->SetIsEnabled(_curLevel < _maxLevel);
		}
	}
}

void UGsUIPopupBattlePassBuyLevel::OnCreateIcon(UGsUIIconBase* InIcon)
{
	if (auto itemIcon = Cast<UGsUIIconItemReward>(InIcon))
	{
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::LongPressType);
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupBattlePassBuyLevel::OnLongPressIcon);
	}
}

void UGsUIPopupBattlePassBuyLevel::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (_rewards.IsValidIndex(InIndex))
	{
		if (auto itemIcon = Cast<UGsUIIconItemReward>(InIcon))
		{
			if (_rewards.IsValidIndex(InIndex))
			{
				if(_rewards[InIndex]._payType == _rewards[InIndex].CURRENCY)
				{
					itemIcon->SetCurrency(_rewards[InIndex]._currency.type, _rewards[InIndex]._currency.amount);
				}
				else
				{
					itemIcon->SetNotOwnItem(_rewards[InIndex]._item.itemId, _rewards[InIndex]._item.amount);
				}
			}
			itemIcon->SetIsRewardNoti(false);
			itemIcon->SetIsAcceptable(false);
			itemIcon->SetIsRewarded(false);
		}
	}
}

// 아이콘 상세보기 창
void UGsUIPopupBattlePassBuyLevel::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIPopupBattlePassBuyLevel::OnClickYes()
{
	auto ownAmount = 0;
	if (auto passData = _curPassData.Pin())
	{
		auto pay = passData->GetLevelOpenPay(_curLevel);
		if (pay._payType == pay.CURRENCY)
		{
			ownAmount = FGsCurrencyHelper::GetCurrencyAmount(pay._currency.type);
			if (pay._currency.amount > ownAmount)
			{
				FGsCurrencyHelper::OpenLackCurrencyPopup(pay._currency.type, pay._currency.amount - ownAmount);
				return;
			}
		}
		else if (pay._payType == pay.ITEM)
		{
			ownAmount = GItem()->GetAllItemCountByTID(pay._item.itemId);

			if (pay._item.amount > ownAmount)
			{
				FGsCurrencyHelper::OpenLackItemPopup(pay._item.itemId, pay._item.amount - ownAmount);
				return;
			}
		}
	}

	Super::OnClickYes();
}

int UGsUIPopupBattlePassBuyLevel::GetLevelupRewards()
{
	_rewards.Reset();

	auto maxLv = _curLevel;
	auto minLv = _minLevel - 1;

	TArray<FGsRewardUnboxingData> rewards;
	if (auto passData = _curPassData.Pin())
	{
		TArray<TWeakPtr<FGsBattlePassLevelData>> levels;
		passData->GetPassLevelList(levels);

		TArray<TWeakPtr<FGsBattlePassLevelData>> ackLevels;
		ackLevels.Append(levels.FilterByPredicate([minLv, maxLv](auto e) {

				if (auto levelData = e.Pin())
				{
					auto lv = levelData->GetLevel();
					return lv <= maxLv && lv > minLv;
				}
				return false;
			}));

		Algo::Transform(ackLevels, rewards, [](auto e) {
				FGsRewardUnboxingData reward;
				e.Pin()->GetRewardInfo(reward);
				return reward;
			});

		if (passData->IsUnlockAdditionalReward())
		{
			Algo::Transform(ackLevels, rewards, [](auto e) {
					FGsRewardUnboxingData reward;
					e.Pin()->GetAddtionalReward(reward);
					return reward;
				});
		}
	}

	TMap<CurrencyType, int64> currency;
	TMap<ItemId, int64> items;

	for (auto& reward : rewards)
	{
		if (reward.IsCurrency())
		{
			currency.FindOrAdd(reward._currencyType) += reward.GetCommonCurrencyAmount();
		}
		else if(reward._itemDataList.IsValidIndex(0))
		{
			for (auto bag : reward._itemDataList)
			{
				items.FindOrAdd(bag->itemId) += bag->itemCountMin;
			}
		}
	}

	for (auto use : currency)
	{	
		_rewards.Add(FGsBattlePassOpenPay{ FGsBattlePassOpenPay::CURRENCY, {INVALID_ITEM_DB_ID, 0}, {use.Key, use.Value} });
	}

	for (auto use : items)
	{
		_rewards.Add(FGsBattlePassOpenPay{ FGsBattlePassOpenPay::ITEM, {use.Key, use.Value}, {CurrencyType::NONE, 0} });
	}

	return _rewards.Num();
}