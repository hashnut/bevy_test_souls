// Fill out your copyright notice in the Description page of Project Settings.


#include "GsInventoryLogicBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"


void FGsInventoryLogicBase::Clear()
{
	_currTabType = EInventoryTab::AllTab;
	_selectedItemDBId = INVALID_ITEM_DB_ID;
	//_focusIndex = INDEX_NONE;
}

void FGsInventoryLogicBase::OnNativeDestruct()
{
	Clear();
}

void FGsInventoryLogicBase::OnSelectTab(EInventoryTab InType)
{
	SetCurrTabType(InType);
}

ItemAmount FGsInventoryLogicBase::GetInventorySlotCount() const
{
	return GSInventory()->GetInvenSlotMaxCount();
}

ItemAmount FGsInventoryLogicBase::GetOwnedItemCount() const
{
	return GItem()->GetOwnedItemCount();	
}

//void FGsInventoryLogicBase::SetFocus(int32 InIndex, UGsUIIconItem* InIcon, bool InbFocus)
//{
//	if (InbFocus)
//	{
//		_focusIndex = InIndex;
//	}
//	else
//	{
//		_focusIndex = INDEX_NONE;
//	}
//}
//
//int32 FGsInventoryLogicBase::GetFocusIndex() const
//{
//	return _focusIndex;
//}