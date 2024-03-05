// Fill out your copyright notice in the Description page of Project Settings.


#include "GsInvenHandler.h"
#include "Management/ScopeGame/GsInventoryManager.h"



FGsInvenHandler::FGsInvenHandler()
{
}

FGsInvenHandler::~FGsInvenHandler()
{
}

void FGsInvenHandler::Close()
{
	if (UGsInventoryManager* invenMgr = GSInventory())
	{
		invenMgr->ResetItemList();
	}
}

void FGsInvenHandler::Init()
{
	if (UGsInventoryManager* invenMgr = GSInventory())
	{
		invenMgr->CreateItemList();
	}
}

void FGsInvenHandler::OnReconection()
{
	Init();
}


/// /////////////////////////// 리뉴얼 ////////////////////////////////////////////

/*
void FGsInvenHandler::OpenInventory(EInventoryTab InTab)
{
	BeginGetItemList();
}

TArray<TWeakPtr<FGsItem>> FGsInvenHandler::FindItemByPresetNum(ItemType InType)
{
	return GItem()->FindItemByPresetNum(InType);
}

void FGsInvenHandler::FindItem(ItemType InItemType)
{
	TArray<TWeakPtr<FGsItem>> itemList = GItem()->FindItem(InItemType);
	for (uint16 i = 0; i < itemList.Num(); ++i)
	{
		if (false == itemList[i].IsValid())
			continue;

		//if(true == GItem()->IsCurrentPresetItem(itemList[i]))
			//continue;

		FGsItem* item = itemList[i].Pin().Get();
		if (false == _onlyCreateOwnedItems.Contains(item))
		{
			_onlyCreateOwnedItems.Add(item);
		}
	}
}

// TID 로 Find - 아직은 필요 없음
void FGsInvenHandler::FindItem(uint32 InItemTID)
{
}

// 후처리 함수
void FGsInvenHandler::GetOwnedItem()
{
	GSInventory()->SendOwnedItemList(_onlyCreateOwnedItems);
	//GSInventory()->SendPresetItemList(FindItemByPresetNum(ItemType::EQUIP));
}




void FGsInvenHandler::CreateItem(const uint64 InItemDBID)
{
	// 탭 구별하지 않고 바로 보낸다. ( 아이템 개수 갱신 처리를 위해서 )
	GSInventory()->AddItem(InItemDBID);

//	TWeakPtr<FGsItem> item = GItem()->FindItem(InItemDBID);
//	if (true == item.IsValid())
//	{
//		if (EInventoryTab::AllTab == _eTabType)
//		{
//			GSInventory()->AddItem(InItemDBID);
//		}
//
//		ItemType type = item.Pin()->GetType();
//		if (EInventoryTab::EquipmentTab == _eTabType && ItemType::EQUIP == type)
//		{
//			GSInventory()->AddItem(InItemDBID);
//		}
//		else if (EInventoryTab::ConsumableTab == _eTabType && ItemType::CONSUME == type)
//		{
//			GSInventory()->AddItem(InItemDBID);
//		}
//		else if (EInventoryTab::MaterialTab == _eTabType && (ItemType::INGREDIENT == type || ItemType::CURRENCY == type))
//		{
//			GSInventory()->AddItem(InItemDBID);
//		}
//	}
//	else
//	{
//#if WITH_EDITOR
//		GSLOG(Warning, TEXT("[FGsInvenHandler::CreateItem] InItemDBID is NotValid"));
//#endif
//	}
}

void FGsInvenHandler::DeleteItem(const uint64 InItemDBID)
{
	GSInventory()->DeleteItem(InItemDBID);
}

void FGsInvenHandler::UpdateItem(const uint64 InItemDBID)
{
	if (true == FGsItem::IsValidItemByDBID(InItemDBID))
	{
		GSInventory()->UpdateItem(InItemDBID);

		if (findItem->IsEquipment())
		{
			GInventory()->SendPresetItemList(FindItemByPresetNum(ItemType::EQUIP));
		}

	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsInvenHandler::UpdateItem] InItemDBID is NotValid"));
#endif
	}
}

void FGsInvenHandler::BeginGetItemList()
{
	ResetContainValue();

	switch (_eTabType)
	{
	case EInventoryTab::AllTab:
		GetOwnedItem(ItemType::CONSUME, ItemType::EQUIP , ItemType::INGREDIENT, ItemType::CURRENCY);
		break;
	case EInventoryTab::EquipmentTab:
		GetOwnedItem(ItemType::EQUIP);
		break;
	case EInventoryTab::ConsumableTab:
		GetOwnedItem(ItemType::CONSUME);
		break;
	case EInventoryTab::MaterialTab:
		GetOwnedItem(ItemType::INGREDIENT, ItemType::CURRENCY);
		break;

	default:
		GetOwnedItem(ItemType::CONSUME, ItemType::EQUIP, ItemType::INGREDIENT, ItemType::CURRENCY);
		break;
	}
}

// Tab 변경은 모든 아이템 갱신 (아이템 타입이 달라짐)
void FGsInvenHandler::ChangeInvenTabState(EInventoryTab InTab)
{
	_eTabType = InTab;
	BeginGetItemList();
}

void FGsInvenHandler::CloseInventory()
{
	ResetContainValue();
	_eTabType = EInventoryTab::AllTab;
}

void FGsInvenHandler::ResetContainValue()
{
	if (0 < _onlyCreateOwnedItems.Num())
	{
		_onlyCreateOwnedItems.Empty();
	}
}



*/
