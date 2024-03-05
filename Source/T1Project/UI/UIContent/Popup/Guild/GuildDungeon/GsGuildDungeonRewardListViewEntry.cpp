// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildDungeonRewardListViewEntry.h"
#include "Components/ListView.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "Currency/GsCurrencyHelper.h"

void UGsGuildDungeonRewardListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _rewardSlotHelper)
	{
		_rewardSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_rewardSlotHelper->Initialize(Cast<IGsIconSelectorInterface>(_rewardIconSelector));
		_rewardSlotHelper->OnCreateIcon.AddDynamic(this, &UGsGuildDungeonRewardListViewEntry::OnCreateIcon);
		_rewardSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsGuildDungeonRewardListViewEntry::OnRefreshIcon);
	}
}

void UGsGuildDungeonRewardListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsGuildDungeonRewardListViewEntryData* data = Cast<UGsGuildDungeonRewardListViewEntryData>(inListItemObject);
	if (nullptr == data)
	{
		return;
	}

	_rewardData = data->_rewardData;

	if (_rewardData._currencyType == CurrencyType::NONE)
	{
		const UGsTable* table = FGsSchemaItemCommon::GetStaticTable();
		const UGsTableItemCommon* itemTable = Cast<UGsTableItemCommon>(table);
		if (itemTable)
		{
			const FGsSchemaItemCommon* findItem;
			if (_rewardData._rewardItem
				&& itemTable->FindRowById(_rewardData._rewardItem->itemId, findItem))
			{
				_rewardTextBlock->SetText(findItem->name);
			}
		}
	}
	else
	{
		_rewardTextBlock->SetText(FGsCurrencyHelper::GetCurrencyLocalizedText(_rewardData._currencyType));
	}

	_rewardSlotHelper->RefreshAll(1);
}

void UGsGuildDungeonRewardListViewEntry::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (itemIcon)
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsGuildDungeonRewardListViewEntry::OnLongPressIcon);
	}
}

void UGsGuildDungeonRewardListViewEntry::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{	
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}
	
	if (CurrencyType::NONE == _rewardData._currencyType)
	{
		const FGsSchemaRewardItemBagData* itemBag = _rewardData._rewardItem;

		itemIcon->SetEnableDetailPopup(true);
		itemIcon->SetCenterPopup(true);
		itemIcon->SetItemNameToolTipFlag(true);
		itemIcon->SetNotOwnItem(_rewardData._rewardItem->itemId, 1);
		itemIcon->SetItemCountMinMax(_rewardData._minAmount, _rewardData._maxAmount);

#if GUILD_DUNGEON_DEBUG
		UE_LOG(LogTemp, Log, TEXT("reward - id : %d\tcount : %d"), itemBag->itemId, count);
#endif
	}
	else
	{
		if (_rewardData._minAmount == _rewardData._maxAmount)
		{
			itemIcon->SetCurrency(_rewardData._currencyType, _rewardData._maxAmount);
		}
		else
		{
			itemIcon->SetCurrency(_rewardData._currencyType, _rewardData._maxAmount);
			itemIcon->SetItemCountMinMax(_rewardData._minAmount, _rewardData._maxAmount);
		}
	}
}

void UGsGuildDungeonRewardListViewEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

