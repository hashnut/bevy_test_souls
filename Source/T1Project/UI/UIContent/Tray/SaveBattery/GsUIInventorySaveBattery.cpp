// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIInventorySaveBattery.h"
#include "Management/ScopeGame/GsSaveBatteryManager.h"
#include "Item/GsItemManager.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"


void UGsUIInventorySaveBattery::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIInventorySaveBattery::OnRefreshItemIcon);
	}
}

void UGsUIInventorySaveBattery::NativeConstruct()
{
	Super::NativeConstruct();

	_iconSlotHelper->RefreshAll();
}

void UGsUIInventorySaveBattery::NativeDestruct()
{
	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::NativeDestruct();
}

void UGsUIInventorySaveBattery::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper->Clear();
		_iconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIInventorySaveBattery::InvalidateAll()
{
	_iconSlotHelper->RefreshAll(GSSaveBattery()->GetDisplayItemCount());
}

void UGsUIInventorySaveBattery::AddItem(const uint64 InItemDBID)
{
	TArray<int32> refreshIndexList;
	if (true == GSSaveBattery()->AddItem(InItemDBID, refreshIndexList))
	{
		for (int32 index : refreshIndexList)
		{
			RefreshIconByIndex(index);
		}
	}
}

void UGsUIInventorySaveBattery::UpdateItem(const uint64 InItemDBID)
{
	TWeakPtr<FGsItem> itemData = GItem()->FindItem(InItemDBID);
	if (false == itemData.IsValid())
		return;
	
	// 장비 아이템을 획득 했을 경우 AddItem에서 처리된다.
	// 현재 ItemManager의 로직에서 장비 아이템은 ADDITEM, UPDATEITEM 둘 다 발생하고 있어 예외처리를 한다.
	if (true == itemData.Pin()->IsEquipment())
		return;

	TArray<int32> refreshIndexList;
	if (true == GSSaveBattery()->AddItem(InItemDBID, refreshIndexList))
	{
		for (int32 index : refreshIndexList)
		{
			RefreshIconByIndex(index);
		}
	}
}

void UGsUIInventorySaveBattery::RemoveItem(const uint64 InItemDBID, const int32 InItemTID)
{
	ItemAmount currAmount = GItem()->GetAllItemCountByTID(InItemTID);
	GSSaveBattery()->SetItemAmount(InItemTID, currAmount);
}

void UGsUIInventorySaveBattery::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	int32 itemTID = 0;
	ItemAmount itemAmount = 0;
	if (true == GSSaveBattery()->GetDisplayItemByIndex(InIndex, itemTID, itemAmount))
	{
		if (UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon))
		{
			itemIcon->SetNotOwnItem(itemTID, itemAmount);
			itemIcon->SetLockImage(false);
			itemIcon->SetRedDot(false);
			itemIcon->SetActiveIconBg(true);
			itemIcon->SetItemNameVisibility(false);
			itemIcon->SetItemNameToolTipFlag(true);
			itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::NotUseDetailWndType);
			itemIcon->SetCoolTimeBg(false);
		}
	}
}

void UGsUIInventorySaveBattery::RefreshIconByIndex(int32 InIndex)
{
	if (nullptr != _iconSlotHelper->GetIcon(InIndex))
	{
		_iconSlotHelper->RefreshByIndex(InIndex);
	}
	else
	{
		_iconSlotHelper->CreateIcon(InIndex + 1);
		_iconSlotHelper->RefreshByIndex(InIndex);
	}
}