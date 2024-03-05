// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"
#include "NpcShop/GsNpcShopData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


/**
 * NpcShop 부모 클래스
 */
struct FGsSchemaNpcData;
class T1PROJECT_API FGsNpcShopBase
{
public:
	enum NpcShopModeType : uint8
	{
		BuyMode = 0,
		SellMode ,
	};

protected:
	TArray<TPair<MessageContentNpcShop, FDelegateHandle>> _npcShopMsgHandler;

protected:
	TArray<int32> _npcShopGroupIDList;
	const FGsSchemaNpcData* _npcSchemaData;
	NpcShopModeType _npcShopModeType = NpcShopModeType::BuyMode;
	int64 _npcGameId = 0;

public:
	FGsNpcShopBase() = default;
	virtual ~FGsNpcShopBase() = default;

public:
	virtual void Init() {};
	virtual void Close();
	virtual void Exit();
	virtual void OnReconectionEnd() {};

public:
	virtual void EnterShop(const FGsSchemaNpcData* InTableData, const int64 InNpcGameId);
	virtual void CloseNpcShop();
	virtual bool TryBuyItem();
	virtual bool TrySellItem();
	virtual void InputShoppingBasket(const TArray<FGsBasketTryInputInfo>& InItemList, bool InIsBuyMode, bool InIsAddedAmount);
	virtual int32 InputBasketBuy(ItemDBId InItemDBID, ItemId InItemTID, EventProductId InProductId, ItemAmount InItemAmount, bool InIsAddedAmount);
	virtual int32 InputBasketSell(ItemDBId InItemDBID, ItemId InItemTID, ItemAmount InItemAmount, bool InIsAddedAmount);

protected:
	virtual void RegisterMessage() {};
	virtual void RemoveMessage();
};
