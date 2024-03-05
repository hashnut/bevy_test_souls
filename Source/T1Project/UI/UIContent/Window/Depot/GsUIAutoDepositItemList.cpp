// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIAutoDepositItemList.h"

#include "Components/WidgetSwitcher.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"

#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"


void UGsUIAutoDepositItemList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIAutoDepositItemList::OnRefreshIcon);
	}
}

void UGsUIAutoDepositItemList::SetItemList(const TArray<ItemId>& InItemIdList, const TMap<ItemId, bool>& InItemSelectMap)
{
	if (0 < InItemIdList.Num())
	{
		_switcherItemList->SetActiveWidgetIndex(0);

		_itemIdList.Empty();
		_itemIdList = InItemIdList;

		_itemSelectMap.Empty();
		_itemSelectMap = InItemSelectMap;

		_isSelectRefresh = false;
		//_iconSlotHelper->RefreshAll(_itemIdList.Num());
	}
	else
	{
		_switcherItemList->SetActiveWidgetIndex(1);
	}
}

void UGsUIAutoDepositItemList::SelectItem(ItemId InItemId, bool InIsSelect)
{
	if (_itemSelectMap.Contains(InItemId))
	{
		_itemSelectMap[InItemId] = InIsSelect;
	}
}

void UGsUIAutoDepositItemList::InvalidateItemListSelect(const TMap<ItemId, bool>& InItemSelectMap)
{
	_itemSelectMap.Empty();
	_itemSelectMap = InItemSelectMap;

	_isSelectRefresh = false;
	//_isSelectRefresh = true;
	//_iconSlotHelper->RefreshAll();
	//_isSelectRefresh = false;
}

void UGsUIAutoDepositItemList::InvalidateAll()
{
	_iconSlotHelper->RefreshAll(_itemIdList.Num());
}

void UGsUIAutoDepositItemList::OnClickIcon(UGsUIIconItem& InIcon)
{
	OnClickedIcon.ExecuteIfBound(InIcon);
}

void UGsUIAutoDepositItemList::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUIAutoDepositItemList::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	if (false == _itemIdList.IsValidIndex(InIndex))
		return;

	ItemId itemId = _itemIdList[InIndex];

	if (false == _itemSelectMap.Contains(itemId))
		return;

	bool isSelect = _itemSelectMap[itemId];
	if (_isSelectRefresh)
	{
		itemIcon->SetSlotSelectionImage(isSelect);
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBound())
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIAutoDepositItemList::OnLongPressIcon);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIAutoDepositItemList::OnClickIcon);
	}

	itemIcon->SetNotOwnItem(itemId, 1);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetItemNameToolTipHoverMode(true);
	itemIcon->SetSlotSelectionImage(isSelect);
}
