// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/MessageParam/GsMessageParam.h"

class FGsItem;
/**
 * Enchant Message 사용될 구조체 정의
 */
 using FGsItemEnchantMessageParamBase = IGsMessageParam;


// Open Event
struct FGsOpenItemEnchant final : public FGsItemEnchantMessageParamBase
{
	ItemDBId _itemDBID = 0;
	bool _isUseBG = true;
	bool _isOpenSingleEnchant = true;

public:
	FGsOpenItemEnchant() = default;
	virtual ~FGsOpenItemEnchant() = default;
	explicit FGsOpenItemEnchant(ItemDBId itemDBID, bool isUseBG, bool isOpenSingleEnchant)
		:
		_itemDBID(itemDBID), _isUseBG(isUseBG), _isOpenSingleEnchant(isOpenSingleEnchant)
	{
	}
};

struct FGsShowEnchantEffect final : public FGsItemEnchantMessageParamBase
{
	ItemDBId _equipItemDbid;
	ItemDBId _ingredientItemDbid;
	ItemEnchantResult _enchantResultType;
	bool _isResultEffect;

public:
	FGsShowEnchantEffect() = default;
	virtual ~FGsShowEnchantEffect() = default;
	explicit FGsShowEnchantEffect(ItemDBId equipItemDbid, ItemDBId ingredientItemDbid, ItemEnchantResult enchantResultType, bool isResultEffect)
		:
		_equipItemDbid(equipItemDbid), _ingredientItemDbid(ingredientItemDbid), _enchantResultType(enchantResultType), _isResultEffect(isResultEffect)
	{
	}
};

// Update Target Item Event
struct FGsUpdateEnchantItem final : public FGsItemEnchantMessageParamBase
{
	ItemDBId _equipItemDBID;
	ItemDBId _scollItemDBID;
	ItemEnchantResult _enchantAckResultType;
	Level _equipItemPrevLevel;

public:
	FGsUpdateEnchantItem() = default;
	virtual ~FGsUpdateEnchantItem() = default;
	explicit FGsUpdateEnchantItem(ItemDBId equipItemDBID, ItemDBId scollItemDBID, ItemEnchantResult ackResultType, const Level InEquipItemPrevLevel)
		:
		_equipItemDBID(equipItemDBID),_scollItemDBID(scollItemDBID), _enchantAckResultType(ackResultType), _equipItemPrevLevel(InEquipItemPrevLevel)
	{
	}
};

// Update Target Item Event
struct FGsUpdateMultiEnchantItem final : public FGsItemEnchantMessageParamBase
{
	TArray<ItemEnchantResultData> _enchantReusltDataList;
	TArray<ItemDBId> _useIngredientItemDbidList;

public:
	FGsUpdateMultiEnchantItem() = default;
	virtual ~FGsUpdateMultiEnchantItem() = default;
	explicit FGsUpdateMultiEnchantItem(TArray<ItemEnchantResultData> enchantReusltDataList, TArray<ItemDBId> useIngredientItemDbidList)
		:
		_enchantReusltDataList(MoveTemp(enchantReusltDataList)), _useIngredientItemDbidList(MoveTemp(useIngredientItemDbidList))
	{
	}
};

// Open Expect Refine Option Popup
struct FGsOpenExpectRefineOption final : public FGsItemEnchantMessageParamBase
{
	ItemDBId _targetItemDbid;
	ItemDBId _ingredientItemDbid;

public:
	FGsOpenExpectRefineOption() = default;
	virtual ~FGsOpenExpectRefineOption() = default;
	explicit FGsOpenExpectRefineOption(ItemDBId targetItemDbid, ItemDBId ingredientItemDbid)
		:
		_targetItemDbid(targetItemDbid), _ingredientItemDbid(ingredientItemDbid)
	{
	}
};

struct FGsUpdateMagicalForgeItem final : public FGsItemEnchantMessageParamBase
{
	ItemDBId _targetItemDBId = INVALID_ITEM_DB_ID;
	ItemMagicalForgeResult _resultType = ItemMagicalForgeResult::NONE;

public:
	FGsUpdateMagicalForgeItem() = default;
	virtual ~FGsUpdateMagicalForgeItem() = default;
	explicit FGsUpdateMagicalForgeItem(ItemDBId InTargetItemDBId, ItemMagicalForgeResult InResultType)
		:
		_targetItemDBId(InTargetItemDBId), _resultType(InResultType)
	{
	}
};