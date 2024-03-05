// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIReuseItemIconSlotBase.h"
#include "T1Project.h"
#include "Item/GsItem.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"



void UGsUIReuseItemIconSlotBase::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper->Clear();
		_iconSlotHelper = nullptr;
	}

	if (nullptr != _slotIconSelector)
	{
		_slotIconSelector->RemoveAllChildren();
	}

	Super::BeginDestroy();
}

void UGsUIReuseItemIconSlotBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_slotIconSelector);
		_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIReuseItemIconSlotBase::OnCreateIconWrapBox);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIReuseItemIconSlotBase::OnRefreshIconWrapBox);
	}
}

void UGsUIReuseItemIconSlotBase::OnShow()
{

}

void UGsUIReuseItemIconSlotBase::OnHide()
{
	_iconSlotHelper->Clear();
}

void UGsUIReuseItemIconSlotBase::Init()
{
	ResetInfo();
}

void UGsUIReuseItemIconSlotBase::SetSlotItemIcon(const uint64 InItemDBID, const int32 InItemTID, const int64 InAmount, const uint8 InSlotIndex)
{
	if (0 < InSlotIndex)
	{
		_slotIndex = InSlotIndex;
	}
	_itemDBID = InItemDBID;
	_itemTID = InItemTID;
	_itemAmount = InAmount;
	_iconSlotHelper->RefreshAll(1);
}

void UGsUIReuseItemIconSlotBase::OnRefreshIconWrapBox(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	if (0 >= _itemDBID && 0 >= _itemTID)
	{
		itemIcon->SetEmptyIcon();
		itemIcon->SetItemCount(0);
	}
	else
	{
		if (0 < _itemDBID)
		{
			itemIcon->SetOwnedItem(_itemDBID);
		}
		else
		{
			itemIcon->SetNotOwnItem(_itemTID, _itemAmount);
		}
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIReuseItemIconSlotBase::OnLongPressIcon);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIReuseItemIconSlotBase::OnClickIconSlot);
	}

	itemIcon->SetSlotIntParamData(_slotIndex);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetRedDot(false);
	itemIcon->SetItemNameToolTipFlag(false);
	itemIcon->SetItemNameToolTipHoverMode(false);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
	//itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
}

void UGsUIReuseItemIconSlotBase::OnCreateIconWrapBox(class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetCoolTimeBg(false);
}

void UGsUIReuseItemIconSlotBase::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	//GSLOG(Warning, TEXT("UGsUIReuseItemIconSlotBase::OnClickIconSlot Call !!!!"));
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}
}

void UGsUIReuseItemIconSlotBase::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
}

void UGsUIReuseItemIconSlotBase::ResetInfo()
{
	_itemAmount = 0;
	_slotIndex = 0;
	_itemDBID = 0;
	_itemTID = 0;
}

const FGsItem* UGsUIReuseItemIconSlotBase::GetIconItemData()
{
	if (_iconSlotHelper)
	{
		if (UGsUIIconBase* findIcon = _iconSlotHelper->GetIcon(0))
		{
			UGsUIIconItemInventory* iconInven = Cast<UGsUIIconItemInventory>(findIcon);
			if (nullptr == iconInven)
				return nullptr;

			return iconInven->GetItemIconData();
		}
		
	}
	return nullptr;
}