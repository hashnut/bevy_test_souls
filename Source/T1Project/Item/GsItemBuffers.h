// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

class FGsItem;

/**
 *  저장 위치별 Item Buffer Class
 */
class T1PROJECT_API FGsItemBuffers
{
private:
	// key : ItemType  /  value : ItemDBID , TSharedPtr<FGsItem>
	// 절대 TSharedPtr 통으로 넘기지 말것. (refCount 관리 안되면 절대 안됌)
	TMap<ItemType, TMap<uint64, TSharedPtr<FGsItem>>> _items;

public:
	FGsItemBuffers();
	~FGsItemBuffers() = default;

public:
	void ClearItems();

public:
	TArray<TWeakPtr<FGsItem>> FindByItemType(const ItemType In_Type);
	TWeakPtr<FGsItem> Find(const uint64 In_ItemDBID, const ItemType InItemType);
	TArray<TWeakPtr<FGsItem>> Find(const uint8 InUserPresetNum, const ItemType InType);
	TArray<TWeakPtr<FGsItem>> Find(const ItemType InType, const ItemCategoryMain In_ItemCategoryMain, const ItemCategorySub In_ItemCategorySub);
	TArray<TWeakPtr<FGsItem>> Find(const ItemType InType, const ItemCategoryMain In_ItemCategoryMain);
	TArray<TWeakPtr<FGsItem>> Find(const ItemType InType, const ItemCategorySub In_ItemCategorySub);
	TArray<TWeakPtr<FGsItem>> Find(const ItemCategorySub In_ItemCategorySub);
	TArray<TWeakPtr<FGsItem>> FindByTID(const int32 In_ItemTID, const ItemType In_Type);

public:
	void AddItems(FGsItem* In_addItem);
	void RemoveItem(FGsItem* In_addItem);
	void ResetData();

public:
	const uint16 GetOwnedItemCount();
	const int32 GetOwnedItemWeight();
	const TMap<ItemType, TMap<uint64, TSharedPtr<FGsItem>>>& GetAllItems() const;
	const TArray<TWeakPtr<FGsItem>> GetAllItem();

private:
	void Find(const ItemType In_Type , OUT TMap<uint64, TSharedPtr<FGsItem>>& outMapData);

public:
	// DebugLog Test 확인용
	void PrintItemListLog();
};
