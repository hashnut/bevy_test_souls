#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"


struct FGsNpcShopQuickSlotItemInfo
{
	ItemDBId _itemDBID;
	ItemId _itemTID;
	EventProductId _productId;
	ItemAmount _itemAmount;
	int32 _shopGroupId;
	int32 _enchantLevel;

	FGsNpcShopQuickSlotItemInfo(ItemDBId itemDBID, ItemId itemTID, EventProductId InProductId, ItemAmount itemAmount, int32 shopGroupId, int32 enchantLevel)
		: _itemDBID(itemDBID), _itemTID(itemTID), _productId(InProductId), _itemAmount(itemAmount), _shopGroupId(shopGroupId), _enchantLevel(enchantLevel)
	{
	}
};

struct FGsNpcShopItemInfo
{
public:
	uint8 _displayPriority;
	EventProductId _productId;
	int32 _itemTID;
	CostType		_costType;
	CurrencyType	_currencyType;
	int32			_currencyItemTID;
	int32					_pcLimitLevel;
	GuildLevel				_pcLimitGuildLevel;
	GuildMemberGradeType	_pcLimitGuildMemberGrade;
	PurchaseLimitBuyerType	_pcLimitBuyerType;
	ItemAmount				_pcLimitBuyerAmount;
	ResetTimeTicket			_pcLimitResetType;
	ItemAmount				_purchasedAmount = 0;
	ItemAmount				_autoPurchaseMaxCount = 0;

private:
	Currency		_currencyAmount = 0;

public:
	FGsNpcShopItemInfo
	(
		uint8 InDisPlayPriority,
		EventProductId InProductId,
		int32 InItemTID,
		CostType InCostType,
		CurrencyType InCurrencyType,
		int32 InCurrencyItemTID,
		int32 InLimitLevel,
		GuildLevel InLimitGuildLevel,
		GuildMemberGradeType InLimitGuildMemberGrade,
		PurchaseLimitBuyerType InLimitBuyerType,
		ItemAmount InLimitBuyerAmount,
		ResetTimeTicket InLimitResetType,
		ItemAmount InAutoPurchaseMaxCount,
		Currency InCurrencyAmount
	)
		:
		_displayPriority(InDisPlayPriority),
		_productId(InProductId),
		_itemTID(InItemTID),
		_costType(InCostType),
		_currencyType(InCurrencyType),
		_currencyItemTID(InCurrencyItemTID),
		_pcLimitLevel(InLimitLevel),
		_pcLimitGuildLevel(InLimitGuildLevel),
		_pcLimitGuildMemberGrade(InLimitGuildMemberGrade),
		_pcLimitBuyerType(InLimitBuyerType),
		_pcLimitBuyerAmount(InLimitBuyerAmount),
		_pcLimitResetType(InLimitResetType),
		_autoPurchaseMaxCount(InAutoPurchaseMaxCount),
		_currencyAmount(InCurrencyAmount)
	{
	}

public:
	Currency GetCurrencyAmount(NpcFunctionType InType) const;
};

struct FGsBasketTryInputInfo
{
	ItemDBId _itemDBId;
	ItemId _itemId;
	EventProductId _productId;
	ItemAmount _itemAmount;

	FGsBasketTryInputInfo(ItemDBId InItemDBId, ItemId InItemId, EventProductId InProductId, ItemAmount InAmount)
		: _itemDBId(InItemDBId), _itemId(InItemId), _productId(InProductId), _itemAmount(InAmount)
	{
	}
};