// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Stat/GsStatInfo.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "UI/UIContent/Define/GsUIContentDefine.h"


class FGsItem;
/**
 * Player Detail Info Message 사용될 구조체 정의
 */
struct FGsPlayerDetailInfoMessageParamBase
{
	FGsPlayerDetailInfoMessageParamBase() = default;
	virtual ~FGsPlayerDetailInfoMessageParamBase() = default;
};


// Update Equipped ItemList
struct FGsUpdateEquippedInfoList final : public FGsPlayerDetailInfoMessageParamBase
{
	TArray<FGsItem*> _equippedItemList;
public:
	FGsUpdateEquippedInfoList() = default;
	virtual ~FGsUpdateEquippedInfoList() = default;
	explicit FGsUpdateEquippedInfoList(TArray<FGsItem*> itemList)
		:
		_equippedItemList(itemList)
	{
	}
};

// Update Equipped Item
struct FGsUpdateEquippedInfo final : public FGsPlayerDetailInfoMessageParamBase
{
	FGsItem* _equippedItem;
public:
	FGsUpdateEquippedInfo() = default;
	virtual ~FGsUpdateEquippedInfo() = default;
	explicit FGsUpdateEquippedInfo(FGsItem* item)
		:
		_equippedItem(item)
	{
	}
};

// Update Player StatList
struct FGsUpdatePlayerStatInfo final : public FGsPlayerDetailInfoMessageParamBase
{
	TArray<const FGsStatInfo*> _statInfoList;
public:
	FGsUpdatePlayerStatInfo() = default;
	virtual ~FGsUpdatePlayerStatInfo() = default;
	explicit FGsUpdatePlayerStatInfo(TArray<const FGsStatInfo*> statList)
		:
		_statInfoList(statList)
	{
	}
};

// Notify Item Equip / UnEquip SlotType Info
struct FGsInvalidateEquipSlotInfo final : public FGsPlayerDetailInfoMessageParamBase
{
	EGsEquipSlotType _slotType;
public:
	FGsInvalidateEquipSlotInfo() = default;
	virtual ~FGsInvalidateEquipSlotInfo() = default;
	explicit FGsInvalidateEquipSlotInfo(EGsEquipSlotType InSlotType)
		:
		_slotType(InSlotType)
	{
	}
};