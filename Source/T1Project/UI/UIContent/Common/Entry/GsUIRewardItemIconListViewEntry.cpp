#include "GsUIRewardItemIconListViewEntry.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"

void UGsUIRewardItemIconListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(Cast<IGsIconSelectorInterface>(_iconSelector));
		_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIRewardItemIconListViewEntry::OnCreateIcon);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIRewardItemIconListViewEntry::OnRefreshIcon);
	}
}

void UGsUIRewardItemIconListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsUIRewardIconListViewEntryData* data = Cast<UGsUIRewardIconListViewEntryData>(inListItemObject);
	if (data)
	{
		_rewardItemDataArray = data->_rewardItemDataArray;
		_iconSlotHelper->RefreshAll(_rewardItemDataArray.Num());
	}
}

void UGsUIRewardItemIconListViewEntry::NativeDestruct()
{
	_iconSlotHelper->Clear();
	_iconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUIRewardItemIconListViewEntry::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (itemIcon)
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIRewardItemIconListViewEntry::OnLongPressIcon);
	}
}

void UGsUIRewardItemIconListViewEntry::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (_rewardItemDataArray.IsValidIndex(InIndex))
	{		
		const FGsUIRewardIconData& rewardData = _rewardItemDataArray[InIndex];
		if (CurrencyType::NONE == rewardData._currencyType)
		{
			const FGsSchemaRewardItemBagData* itemBag = rewardData._rewardItem;			
			itemIcon->SetEnableDetailPopup(true);
			itemIcon->SetCenterPopup(true);
			itemIcon->SetItemNameToolTipFlag(true);
			itemIcon->SetNotOwnItem(itemBag->itemId, 1);
			itemIcon->SetItemCountMinMax(rewardData._minAmount, rewardData._maxAmount);

#if GUILD_DUNGEON_DEBUG
			UE_LOG(LogTemp, Log, TEXT("reward - id : %d\tcount : %d"), itemBag->itemId, count);
#endif
		}
		else
		{
			if (rewardData._minAmount == rewardData._maxAmount)
			{
				itemIcon->SetCurrency(rewardData._currencyType, rewardData._maxAmount);
			}
			else
			{
				itemIcon->SetCurrency(rewardData._currencyType, rewardData._maxAmount);
				itemIcon->SetItemCountMinMax(rewardData._minAmount, rewardData._maxAmount);
			}			
		}		
	}
}

void UGsUIRewardItemIconListViewEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
