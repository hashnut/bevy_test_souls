// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Message/MessageParam/GsMessageParam.h"


/**
 * Item Message 사용될 구조체 정의
 */
struct FGsItemMessageParamBase
{
public:
	FGsItemMessageParamBase() = default;
	virtual ~FGsItemMessageParamBase() = default;
};

// Update Item
struct FGsUpdateItem final : public FGsItemMessageParamBase
{
	uint64 _itemDBID;
	ItemStorageType _itemStorageType = ItemStorageType::INVENTORY;
	// Lock 버튼을 눌렀을 때는 ItemDetailInfo 를 갱신하지 않아야 한다 (C2URWQ-3201)
	bool _skipItemDetailInfo = false;

public:
	FGsUpdateItem() = default;
	virtual ~FGsUpdateItem() = default;
	explicit FGsUpdateItem(uint64 itemDBID)
		:
		_itemDBID(itemDBID)
	{
	}
	explicit FGsUpdateItem(uint64 itemDBID, ItemStorageType itemStorageType)
		:
		_itemDBID(itemDBID), _itemStorageType(itemStorageType)
	{
	}
	explicit FGsUpdateItem(uint64 itemDBID, bool skipItemDetailInfo)
		:
		_itemDBID(itemDBID), _skipItemDetailInfo(skipItemDetailInfo)
	{
	}
	explicit FGsUpdateItem(uint64 itemDBID, ItemStorageType itemStorageType, bool skipItemDetailInfo)
		:
		_itemDBID(itemDBID), _itemStorageType(itemStorageType), _skipItemDetailInfo(skipItemDetailInfo)
	{
	}
};

struct FGsUpdateItemList final : public FGsItemMessageParamBase
{
	TArray<ItemDBId> _itemDbidList;
	ItemStorageType _itemStorageType = ItemStorageType::INVENTORY;

public:
	FGsUpdateItemList() = default;
	virtual ~FGsUpdateItemList() = default;
	explicit FGsUpdateItemList(TArray<ItemDBId> ItemDbidList)
		:
		_itemDbidList(MoveTemp(ItemDbidList))
	{
	}
	explicit FGsUpdateItemList(TArray<ItemDBId> ItemDbidList, ItemStorageType itemStorageType)
		:
		_itemDbidList(MoveTemp(ItemDbidList)), _itemStorageType(itemStorageType)
	{
	}
};

// Add Item
struct FGsAddItem final : public FGsItemMessageParamBase
{
	uint64 _itemDBID;

public:
	FGsAddItem() = default;
	virtual ~FGsAddItem() = default;
	explicit FGsAddItem(uint64 itemDBID)
		:
		_itemDBID(itemDBID)
	{
	}
};

// Add Item List
struct FGsAddItemList final : public FGsItemMessageParamBase
{
	TArray<ItemDBId> _itemDbidList;

public:
	FGsAddItemList() = default;
	virtual ~FGsAddItemList() = default;
	explicit FGsAddItemList(TArray<ItemDBId> ItemDbidList)
		:
		_itemDbidList(MoveTemp(ItemDbidList))
	{
	}
};

// Remove Item
struct FGsRemoveItem final : public FGsItemMessageParamBase
{
	ItemDBId _itemDBID;
	ItemId _itemTID;
	const Level _itemLevel = 0;

public:
	FGsRemoveItem() = default;
	virtual ~FGsRemoveItem() = default;
	explicit FGsRemoveItem(ItemDBId itemDBID)
		:
		FGsRemoveItem(itemDBID, INVALID_ITEM_ID)
	{
	}
	explicit FGsRemoveItem(ItemDBId itemDBID, ItemId itemTID)
		:
		FGsRemoveItem(itemDBID, itemTID, INVALID_LEVEL)
	{
	}
	FGsRemoveItem(ItemDBId itemDBID, ItemId itemTID, const Level InItemLevel)
		:
		_itemDBID(itemDBID), _itemTID(itemTID), _itemLevel(InItemLevel)
	{
	}
};

// Lock Item
struct FGsReactionItemLock final : public FGsItemMessageParamBase
{
	bool _isLock;
	TArray<ItemCategorySub> _lockItemCategorySubList;

public:
	FGsReactionItemLock() = default;
	virtual ~FGsReactionItemLock() = default;
	explicit FGsReactionItemLock(bool isLock, TArray<ItemCategorySub> lockItemCategorySubList)
		:
		_isLock(isLock), _lockItemCategorySubList(lockItemCategorySubList)
	{
	}
};

// Removed Item List
struct FGsRemoveItemList final : public FGsItemMessageParamBase
{
	TArray<ItemDBId> _itemDBIdList;

public:
	FGsRemoveItemList() = default;
	virtual ~FGsRemoveItemList() = default;
	explicit FGsRemoveItemList(const TArray<ItemDBId>& InItemList)
		: _itemDBIdList(InItemList)
	{
	}
};