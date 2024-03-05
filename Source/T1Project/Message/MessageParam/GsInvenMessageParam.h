// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Message/MessageParam/GsMessageParam.h"

class FGsItem;

/**
 * Inventory Message 사용될 구조체 정의
 */

 using FGsInventoryMessageParamBase = IGsMessageParam;

struct FGsOpenInventory final : public FGsInventoryMessageParamBase
{
	EInventoryTab _tabType;

public:
	FGsOpenInventory() = default;
	virtual ~FGsOpenInventory() = default;
	explicit FGsOpenInventory(EInventoryTab tabType)
		:
		_tabType(tabType)
	{
	}
};

// Update Item Weight
struct FGsUpdateInvenWeightInfo final : public FGsInventoryMessageParamBase
{
	int32 _weight = 0;
	float _weightPercent = 0.f;

public:
	FGsUpdateInvenWeightInfo() = default;
	virtual ~FGsUpdateInvenWeightInfo() = default;
	explicit FGsUpdateInvenWeightInfo(int32 Weight, float WeightPercent)
		:
		_weight(Weight), _weightPercent(WeightPercent)
	{
	}
};

// Update Item Weight PenaltyType From PacketData
struct FGsUpdateInvenWeightPenaltyType final : public FGsInventoryMessageParamBase
{
	WeightPenaltyGrade _gradeType;

public:
	FGsUpdateInvenWeightPenaltyType() = default;
	virtual ~FGsUpdateInvenWeightPenaltyType() = default;
	explicit FGsUpdateInvenWeightPenaltyType(WeightPenaltyGrade PenaltyType)
		:
		_gradeType(PenaltyType)
	{
	}
};
















/// /////////////////////////// 리뉴얼 ////////////////////////////////////////////


/**
 * Inventory Message 사용될 구조체 정의
 */
/*
struct FGsInvenMessageParamBase
{
	FGsInvenMessageParamBase() = default;
	virtual ~FGsInvenMessageParamBase() = default;
};

// Inven ItemList Data
struct FGsInvenItemList final : public FGsInvenMessageParamBase
{
	TMap<ItemDBId,bool> _displayItemListDbidMap;
	bool _isDimmedState = false;

public:
	FGsInvenItemList() = default;
	explicit FGsInvenItemList(TMap<ItemDBId, bool> displayItemListDbidMap, bool isDimmedState)
		:
		_displayItemListDbidMap(displayItemListDbidMap) , _isDimmedState(isDimmedState)
	{
	}
	
	explicit FGsInvenItemList(TArray<FGsItem*>&& ItemList ,  bool isDimmedState)
		:
		_itemList(MoveTemp(ItemList)) , _isDimmedState(isDimmedState)
	{
	}
	
};

// Preset ItemList Data
struct FGsPresetItemList final : public FGsInvenMessageParamBase
{
	TArray<FGsItem*> _itemList;

public:
	FGsPresetItemList() = default;
	explicit FGsPresetItemList(const TArray<FGsItem*>& ItemList)
		:
		_itemList(ItemList)
	{
	}
	explicit FGsPresetItemList(TArray<FGsItem*>&& ItemList)
		:
		_itemList(MoveTemp(ItemList))
	{
	}
};

// Open Event
struct FGsOpenInventory final : public FGsInvenMessageParamBase
{
	EInventoryTab _eInvenTab;

public:
	FGsOpenInventory() = default;
	explicit FGsOpenInventory(EInventoryTab InvenTab)
		:
		_eInvenTab(InvenTab)
	{
	}
};

// Close Event
struct FGsCloseInventory final : public FGsInvenMessageParamBase
{
	FGsCloseInventory() = default;
};

// Reset Selected Icon
struct FGsResetInvenSelectedIcon final : public FGsInvenMessageParamBase
{
	bool _isKeepSelectionFlag = false;

	FGsResetInvenSelectedIcon() = default;
	explicit FGsResetInvenSelectedIcon(bool isKeepSelectionFlag)
		:
		_isKeepSelectionFlag(isKeepSelectionFlag)
	{
	}
};

// Change Tab Event
struct FGsInvenTabChange final : public FGsInvenMessageParamBase
{
	EInventoryTab _eInvenTab;

public:
	FGsInvenTabChange() = default;
	explicit FGsInvenTabChange(EInventoryTab InvenTab)
		:
		_eInvenTab(InvenTab)
	{
	}
};

// Add Item
struct FGsAddInvenItem final : public FGsInvenMessageParamBase
{
	uint64 _addItemDBID = 0;

public:
	FGsAddInvenItem() = default;
	explicit FGsAddInvenItem(uint64 ItemDBID)
		:
		_addItemDBID(ItemDBID)
	{
	}
};

// Update Item
struct FGsUpdateInvenItem final : public FGsInvenMessageParamBase
{
	uint64 _updateItemDBID = 0;

public:
	FGsUpdateInvenItem() = default;
	explicit FGsUpdateInvenItem(uint64 ItemDBID)
		:
		_updateItemDBID(ItemDBID)
	{
	}
};

// Remove Item
struct FGsRemoveInvenItem final : public FGsInvenMessageParamBase
{
	uint64 _removeItemDBID = 0;

public:
	FGsRemoveInvenItem() = default;
	explicit FGsRemoveInvenItem(uint64 ItemDBID)
		:
		_removeItemDBID(ItemDBID)
	{
	}
};

// Send Inven Item Weight
struct FGsSendItemWeight final : public FGsInvenMessageParamBase
{
	FGsSendItemWeight() = default;
};

// OnChange Icon Dimmed
struct FGsOnChangeInvenIconDimmed final : public FGsInvenMessageParamBase
{
	bool _isDimmed = false;

public:
	FGsOnChangeInvenIconDimmed() = default;
	explicit FGsOnChangeInvenIconDimmed(bool dimmed)
		:
		_isDimmed(dimmed)
	{
	}
};

// OnChange Inven Slot
struct FGsOnChangeInvenSlotCount final : public FGsInvenMessageParamBase
{
	uint32 _maxSlotCount;
	uint32 _totalExtendCount;

public:
	FGsOnChangeInvenSlotCount() = default;
	explicit FGsOnChangeInvenSlotCount(uint32 maxSlotCount, uint32 totalExtendCount)
		:
		_maxSlotCount(maxSlotCount), _totalExtendCount(totalExtendCount)
	{
	}
};

*/