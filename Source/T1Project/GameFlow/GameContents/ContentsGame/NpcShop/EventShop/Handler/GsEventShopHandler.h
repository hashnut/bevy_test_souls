// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlow/GameContents/ContentsGame/NpcShop/GsNpcShopBase.h"


struct FGsSchemaNpcData;
struct IGsMessageParam;
/**
 * 이벤트상점 로직 수행 Class
 */
class T1PROJECT_API FGsEventShopHandler : public FGsNpcShopBase
{
	using Super = FGsNpcShopBase;

public:
	FGsEventShopHandler() = default;
	virtual ~FGsEventShopHandler() = default;

public:
	virtual int32 InputBasketBuy(ItemDBId InItemDBID, ItemId InItemTID, EventProductId InProductId, ItemAmount InItemAmount, bool InIsAddedAmount) override;
	virtual int32 InputBasketSell(ItemDBId InItemDBID, ItemId InItemTID, ItemAmount InItemAmount, bool InIsAddedAmount) override;

public:
	bool TryBuyItem() override;
	bool TrySellItem() override;

private:
	void RegisterMessage() override;

private:
	bool TryBuyEventNpcShopItem();

private:
	void UpdatePurchasedItem(const IGsMessageParam* InParam);
};

