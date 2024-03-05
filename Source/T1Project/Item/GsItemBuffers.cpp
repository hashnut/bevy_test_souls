// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemBuffers.h"
#include "T1Project.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"

#include "T1Project/UTIL/GsTimeSyncUtil.h"


FGsItemBuffers::FGsItemBuffers()
{
	ResetData();
}

// Item Find 함수 : 파라미터의 타입인 아이템 모두 Find
void FGsItemBuffers::Find(const ItemType In_Type, OUT TMap<uint64, TSharedPtr<FGsItem>>& outMapData)
{
	if (true == _items.Contains(In_Type))
	{
		outMapData = *_items.Find(In_Type);
	}
}

TArray<TWeakPtr<FGsItem>> FGsItemBuffers::FindByItemType(const ItemType In_Type)
{
	TArray<TWeakPtr<FGsItem>> result;
	TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
	Find(In_Type, foundItemMap);
	for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
	{
		TSharedPtr<FGsItem> item = iter.Value;
		if (false == FGsItem::IsValidItem(item.Get()))
			continue;

		result.Emplace(item);
	}

	return result;
}

// Item Find 함수 : 해당 Type 에 In_ItemTID Find
TArray<TWeakPtr<FGsItem>> FGsItemBuffers::FindByTID(const int32 In_ItemTID, const ItemType In_Type)
{
	TArray<TWeakPtr<FGsItem>> result;
	TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
	Find(In_Type, foundItemMap);
	for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
	{
		TSharedPtr<FGsItem> item =	iter.Value;
		FGsItem* itemData = item.Get();
		if (FGsItem::IsValidItem(itemData))
		{
			if (itemData->GetTID() == In_ItemTID)
			{
				result.Emplace(item);
			}
		}
	}

	return result;
}

// Item Find 함수 : Item DBID 로 Find
TWeakPtr<FGsItem> FGsItemBuffers::Find(const uint64 In_ItemDBID , const ItemType InItemType)
{
	//TMap<ItemType, TMap<uint64, TSharedPtr<FGsItem>>>
	if (InItemType == ItemType::NONE || InItemType == ItemType::MAX)
	{
		for (uint16 i = 0; i < (uint8)ItemType::MAX; ++i)
		{
			TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
			Find((ItemType)i , foundItemMap);
			if (true == foundItemMap.Contains(In_ItemDBID))
			{
				return foundItemMap[In_ItemDBID];
			}
		}
	}
	else
	{
		TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
		Find(InItemType, foundItemMap);
		if (true == foundItemMap.Contains(In_ItemDBID))
		{
			return foundItemMap[In_ItemDBID];
		}
	}

	return nullptr;
}

TArray<TWeakPtr<FGsItem>> FGsItemBuffers::Find(const ItemCategorySub In_ItemCategorySub)
{
	TArray<TWeakPtr<FGsItem>> result;
	for (uint16 i = 0; i < (uint8)ItemType::MAX; ++i)
	{
		TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
		Find((ItemType)i, foundItemMap);
		for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
		{
			if (false == FGsItem::IsValidItem(iter.Value.Get()))
				continue;

			if(In_ItemCategorySub != iter.Value->GetSubCategory())
				continue;

			result.Emplace(iter.Value);
		}
	}
	return result;
}

// Item Find 함수 : 파라미터 프리셋넘버와 동일한 아이템들 Find
TArray<TWeakPtr<FGsItem>> FGsItemBuffers::Find(const uint8 InUserPresetNum, const ItemType InType)
{
	TArray<TWeakPtr<FGsItem>> result;
	//TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
	//Find(InType, foundItemMap);
	//for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
	//{
	//	TSharedPtr<FGsItem> item = iter.Value;
	//	FGsItem* itemData = item.Get();
	//	if (FGsItem::IsValidItem(itemData))
	//	{
	//		if (itemData->GetEquipPresetValue() == InUserPresetNum)
	//		{
	//			result.Emplace(item);
	//		}
	//	}
	//}

	return result;
}

TArray<TWeakPtr<FGsItem>> FGsItemBuffers::Find(const ItemType InType, const ItemCategoryMain In_ItemCategoryMain, const ItemCategorySub In_ItemCategorySub)
{
	TArray<TWeakPtr<FGsItem>> result;
	TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
	Find(InType, foundItemMap);
	for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
	{
		TSharedPtr<FGsItem> item = iter.Value;
		FGsItem* itemData = item.Get();
		if (FGsItem::IsValidItem(itemData))
		{
			if ((itemData->GetMainCategory() == In_ItemCategoryMain) 
				&& (itemData->GetSubCategory() == In_ItemCategorySub))
			{
				result.Emplace(item);
			}
		}
	}

	return result;
}

TArray<TWeakPtr<FGsItem>> FGsItemBuffers::Find(const ItemType InType, const ItemCategoryMain In_ItemCategoryMain)
{
	TArray<TWeakPtr<FGsItem>> result;
	TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
	Find(InType, foundItemMap);
	for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
	{
		TSharedPtr<FGsItem> item = iter.Value;
		FGsItem* itemData = item.Get();
		if (FGsItem::IsValidItem(itemData))
		{
			if (itemData->GetMainCategory() == In_ItemCategoryMain)
			{
				result.Emplace(item);
			}
		}
	}

	return result;
}

TArray<TWeakPtr<FGsItem>> FGsItemBuffers::Find(const ItemType InType, const ItemCategorySub In_ItemCategorySub)
{
	TArray<TWeakPtr<FGsItem>> result;
	TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
	Find(InType, foundItemMap);
	for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
	{
		TSharedPtr<FGsItem> item = iter.Value;
		FGsItem* itemData = item.Get();
		if (FGsItem::IsValidItem(itemData))
		{
			if (itemData->GetSubCategory() == In_ItemCategorySub)
			{
				result.Emplace(item);
			}
		}
	}

	return result;
}

void FGsItemBuffers::AddItems(FGsItem* In_addItem)
{
	if (nullptr == In_addItem)
		return;

	if (false == _items.Contains(In_addItem->GetType()))
		return;

	TSharedPtr<FGsItem> make = MakeShareable(In_addItem);
	TMap<uint64, TSharedPtr<FGsItem>>& itemMap = _items[In_addItem->GetType()];
	itemMap.Emplace(In_addItem->GetDBID(), make);
}

void FGsItemBuffers::RemoveItem(FGsItem* In_addItem)
{
	if (nullptr == In_addItem)
		return;

	TMap<uint64, TSharedPtr<FGsItem>>& itemMap = _items[In_addItem->GetType()];
	if (true == itemMap.Contains(In_addItem->GetDBID()))
	{
		for (const TPair<uint64, TSharedPtr<FGsItem>> iter : itemMap)
		{
			TSharedPtr<FGsItem> item = iter.Value;
			if (iter.Key == In_addItem->GetDBID())
			{
				int32 removeResult = itemMap.Remove(In_addItem->GetDBID());
				//GSLOG(Warning, TEXT("[FGsItemBuffers::RemoveItem] removeResult : %d"), removeResult);
				//GSLOG(Warning, TEXT("[FGsItemBuffers::RemoveItem] itemMap.Num() : %d"), itemMap.Num());

				return;
			}
		}
	}
}

void FGsItemBuffers::ResetData()
{
	ClearItems();

	for (uint16 i = 0; i < (uint8)ItemType::MAX; ++i)
	{
		if (i == (uint8)ItemType::NONE)
			continue;

		TMap<uint64, TSharedPtr<FGsItem>> sub;
		sub.Reset();

		_items.Add((ItemType)i, sub);
	}
}


void FGsItemBuffers::ClearItems()
{
	_items.Empty();
}

//TMap<ItemType, TMap<uint64, TSharedPtr<FGsItem>>>
const uint16 FGsItemBuffers::GetOwnedItemCount()
{
	uint16 ownedCount = 0;
	for (uint16 i = 0; i < (uint8)ItemType::MAX; ++i)
	{
		if (i == (uint8)ItemType::NONE)
			continue;

		TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
		Find((ItemType)i , foundItemMap);
		ownedCount += foundItemMap.Num();
		//GSLOG(Warning, TEXT("[GetOwnedItemCount] ownedCount : %d"), ownedCount);

		/*
		for (const auto& iter : foundItemMap)
		{
			TSharedPtr<FGsItem> item = iter.Value;
			if (false == item.IsValid())
			{
				GSLOG(Warning, TEXT("[FGsItemBuffers::GetOwnedItemCount] item.IsValid() == false - DBID : %llu"), iter.Key);
				continue;
			}

			++ownedCount;
		}
		*/
	}
	

	return ownedCount;
}

//TMap<ItemType, TMap<uint64, TSharedPtr<FGsItem>>>
const int32 FGsItemBuffers::GetOwnedItemWeight()
{
	int32 totalWeight = 0;
	for (uint16 i = 0; i < (uint8)ItemType::MAX; ++i)
	{
		if (i == (uint8)ItemType::NONE)
			continue;

		TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
		Find((ItemType)i , foundItemMap);
		for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
		{
			TSharedPtr<FGsItem> item = iter.Value;
			FGsItem* itemData = item.Get();
			if (false == FGsItem::IsValidItem(itemData))
			{
				GSLOG(Warning, TEXT("[FGsItemBuffers::GetOwnedItemWeight] item.IsValid() == false - DBID : %llu"), iter.Key);
				continue;
			}

			totalWeight += (item->GetWeight() * item->GetAmount());
		}
	}

	return totalWeight;
}

const TMap<ItemType, TMap<uint64, TSharedPtr<FGsItem>>>& FGsItemBuffers::GetAllItems() const
{
	return _items;
}

const TArray<TWeakPtr<FGsItem>> FGsItemBuffers::GetAllItem()
{
	TArray<TWeakPtr<FGsItem>> dataList;
	for (uint16 i = 0; i < (uint8)ItemType::MAX; ++i)
	{
		if (i == (uint8)ItemType::NONE)
			continue;

		TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
		Find((ItemType)i, foundItemMap);
		for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
		{
			TSharedPtr<FGsItem> item = iter.Value;
			FGsItem* itemData = item.Get();
			if (false == FGsItem::IsValidItem(itemData))
				continue;
			
			dataList.Emplace(iter.Value);
		}
	}

	return dataList;
}

// For Debug Log
void FGsItemBuffers::PrintItemListLog()
{
	for (uint16 i = 0; i < (uint8)ItemType::MAX; ++i)
	{
		if (i == (uint8)ItemType::NONE)
			continue;

		TMap<uint64, TSharedPtr<FGsItem>> foundItemMap;
		Find((ItemType)i, foundItemMap);
		for (const TPair<uint64, TSharedPtr<FGsItem>> iter : foundItemMap)
		{
			if (false == iter.Value.IsValid())
			{
				GSLOG(Warning, TEXT("[PrintItemListLog] - iter.Value.IsValid() is false !!! "));
				continue;
			}

			FGsItem* itemData = iter.Value.Get();
			if (nullptr == itemData)
			{
				GSLOG(Warning, TEXT("[PrintItemListLog] - iter.Value.Get() is nullptr !!! "));
				continue;
			}

			GSLOG(Warning, TEXT("-------------------------------------------------------------------"));
			GSLOG(Warning, TEXT("[PrintItemListLog] ItemDBID  : %llu"), iter.Key);
			GSLOG(Warning, TEXT("[PrintItemListLog] ItemName : %s"), *itemData->GetName().ToString());
			GSLOG(Warning, TEXT("[PrintItemListLog] ItemAmount : %d"), itemData->GetAmount());
			GSLOG(Warning, TEXT("[PrintItemListLog] ExpiredTime : %lld"), itemData->GetExpiredTime());
			GSLOG(Warning, TEXT("[PrintItemListLog] InvenSortValue : %d"), itemData->GetInvenSortValue());
			GSLOG(Warning, TEXT("-------------------------------------------------------------------"));
		}
	}
}

/*
enum class ItemType : uint8
{
	NONE = 0,	// 에러값
	EQUIP = 1,	// 장비
	INGREDIENT = 2,	// 재료 - 사용성이 없다
	CONSUME = 3,	// 소모품 - 사용성 있음
	ETC = 4,	// 기타 - 뭘까? 일단 만들어두긴 했는데...

	MAX = 5,
};
*/