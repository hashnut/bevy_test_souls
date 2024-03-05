// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Management/ScopeGame/GsNpcShopManager.h"
#include "NpcShop/GsNpcShopData.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

/**
 *  NpcShop Message에 사용될 구조체
 */
struct T1PROJECT_API FGsNpcShopMessageParamBase : public IGsMessageParam
{
public:
	FGsNpcShopMessageParamBase() = default;
	virtual ~FGsNpcShopMessageParamBase() = default;
};

struct T1PROJECT_API FGsNpcShopInteraction final : public FGsNpcShopMessageParamBase
{
	NpcFunctionType _eNpcFunctionType;
	TArray<uint8> _npcShopGroupIDList;

public:
	FGsNpcShopInteraction() = default;
	virtual ~FGsNpcShopInteraction() = default;
	explicit FGsNpcShopInteraction(NpcFunctionType NpcFunctionType, TArray<uint8> GroupIDLIst)
		:
		_eNpcFunctionType(NpcFunctionType), _npcShopGroupIDList(MoveTemp(GroupIDLIst))
	{
	}
};

struct T1PROJECT_API FGsNpcShopInvalidateAll final : public FGsNpcShopMessageParamBase
{
	TArray<int32> _npcShopGroupIDList;

public:
	FGsNpcShopInvalidateAll() = default;
	virtual ~FGsNpcShopInvalidateAll() = default;
	explicit FGsNpcShopInvalidateAll(TArray<int32> GroupIDLIst)
		:
		_npcShopGroupIDList(MoveTemp(GroupIDLIst))
	{
	}
};

struct T1PROJECT_API FGsInvalidateNpcShopItemList final : public FGsNpcShopMessageParamBase
{
	NpcFunctionType _npcFunctionType;
	TArray<int32> _npcShopGroupIDList;
	ENpcShopProductListTab _shopProductTabType;
	bool _isBuyMode;

public:
	FGsInvalidateNpcShopItemList() = default;
	virtual ~FGsInvalidateNpcShopItemList() = default;
	explicit FGsInvalidateNpcShopItemList(NpcFunctionType InNpcFunctionType, TArray<int32> GroupIDLIst, ENpcShopProductListTab ShopProductTabType, bool IsBuyMode)
		:
		_npcFunctionType(InNpcFunctionType), _npcShopGroupIDList(MoveTemp(GroupIDLIst)) , _shopProductTabType(ShopProductTabType), _isBuyMode(IsBuyMode)
	{
	}
};

struct T1PROJECT_API FGsCloseNpcShop final : public FGsNpcShopMessageParamBase
{
public:
	FGsCloseNpcShop() = default;
	virtual ~FGsCloseNpcShop() = default;
};

struct T1PROJECT_API FGsInvalidateNpcShopInven final : public FGsNpcShopMessageParamBase
{
public:
	FGsInvalidateNpcShopInven() = default;
	virtual ~FGsInvalidateNpcShopInven() = default;
};

struct T1PROJECT_API FGsInvalidateShoppingBasket final : public FGsNpcShopMessageParamBase
{
	int32 _changedSlotIndex;

public:
	FGsInvalidateShoppingBasket() = default;
	virtual ~FGsInvalidateShoppingBasket() = default;
	explicit FGsInvalidateShoppingBasket(int32 InSlotIndex) : _changedSlotIndex(InSlotIndex)
	{
	}
};

struct T1PROJECT_API FGsTryBuyItem final : public FGsNpcShopMessageParamBase
{
public:
	FGsTryBuyItem() = default;
	virtual ~FGsTryBuyItem() = default;
};

struct T1PROJECT_API FGsTrySellItem final : public FGsNpcShopMessageParamBase
{
public:
	FGsTrySellItem() = default;
	virtual ~FGsTrySellItem() = default;
};

struct T1PROJECT_API FGsInputShoppingBasket final : public FGsNpcShopMessageParamBase
{
	TArray<FGsBasketTryInputInfo> _itemList;
	bool _isBuyMode;
	bool _isAdded;

	FGsInputShoppingBasket() = default;
	virtual ~FGsInputShoppingBasket() = default;
	explicit FGsInputShoppingBasket(TArray<FGsBasketTryInputInfo> InItemList , bool isBuyMode, bool isAdded)
		:
		_itemList(InItemList), _isBuyMode(isBuyMode), _isAdded(isAdded)
	{
	}

};

struct T1PROJECT_API FGsInputAutoBuyItem final : public FGsNpcShopMessageParamBase
{
public:
	FGsInputAutoBuyItem() = default;
	virtual ~FGsInputAutoBuyItem() = default;
};

struct T1PROJECT_API FGsInputAutoSellItem final : public FGsNpcShopMessageParamBase
{
public:
	FGsInputAutoSellItem() = default;
	virtual ~FGsInputAutoSellItem() = default;
};

struct T1PROJECT_API FGsSaveAutoBuyOption final : public FGsNpcShopMessageParamBase
{
public:
	FGsSaveAutoBuyOption() = default;
	virtual ~FGsSaveAutoBuyOption() = default;
};

struct T1PROJECT_API FGsOnChangeNpcShopMode final : public FGsNpcShopMessageParamBase
{
	bool _isBuyMode;

public:
	FGsOnChangeNpcShopMode() = default;
	virtual ~FGsOnChangeNpcShopMode() = default;
	explicit FGsOnChangeNpcShopMode(bool isBuyMode)
		:
		_isBuyMode(isBuyMode)
	{
	}
};

struct T1PROJECT_API FGsEnterNpcShop final : public FGsNpcShopMessageParamBase
{
	NpcFunctionType _npcFunctionType;
	TArray<int32> _npcShopGroupIDList;
	ENpcShopProductListTab _shopProductTabType;
	bool _isBuyMode;
	const FGsSchemaNpcData* _npcData = nullptr;

public:
	FGsEnterNpcShop() = default;
	virtual ~FGsEnterNpcShop() = default;
	explicit FGsEnterNpcShop(NpcFunctionType InNpcFunctionType, TArray<int32> GroupIDLIst, ENpcShopProductListTab ShopProductTabType, bool IsBuyMode, const FGsSchemaNpcData* InNpcData = nullptr)
		:
		_npcFunctionType(InNpcFunctionType), _npcShopGroupIDList(MoveTemp(GroupIDLIst)), _shopProductTabType(ShopProductTabType), _isBuyMode(IsBuyMode), _npcData(InNpcData)
	{
	}
};

struct T1PROJECT_API FGsUpdatePurchasedItem final : public FGsNpcShopMessageParamBase
{
	TArray<ItemIdAmountPair> _purchasedItemList;

public:
	FGsUpdatePurchasedItem() = default;
	virtual ~FGsUpdatePurchasedItem() = default;
	explicit FGsUpdatePurchasedItem(TArray<ItemIdAmountPair> InPurchasedItemList)
		:
		_purchasedItemList(MoveTemp(InPurchasedItemList))
	{
	}
};

