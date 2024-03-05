// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"

class FGsItem;

namespace PD
{
	namespace GC
	{
		struct PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ;
		struct PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ;
	}
	namespace SC
	{
		struct PKT_SC_ACK_MARKET_TRANSACTION_REGISTER_READ;
		struct PKT_SC_ACK_WDMARKET_TRANSACTION_REGISTER_READ;
	}
}

/**
 * 거래소 아이템 정보
 */
struct T1PROJECT_API FGsMarketItemData final
{
public:
	MarketTransactionId _transactionId; // Key값	
	ItemId _itemId;
	MarketPrice _price;
	FGsItem* _item;

	// 판매 페이지에서만 쓰이는 값. FGsMarketHelper::GetSellState 이용
	time_t _sellStartTime; // 판매 시작 시간

	// 빠른 등록/취소를 막기 위한 시간 체크 로직에서 활용
	time_t _registeredServerTime = 0;

public:
	FGsMarketItemData() = default;
	~FGsMarketItemData();

public:
	void Clear();
	
	// 일반 거래소
public:
	// PKT_GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE_READ::TransactionList
	void SetInstackableItem(ItemId InItemId, void* InData);
	// PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ::TransactionList
	void SetStackableItem(ItemId InItemId, void* InData);

	// PKT_GC_ACK_MARKET_MY_TRANSACTION_LIST_READ::TransactionList
	void SetSellItem(void* InData);
	void SetDetailData(PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ* InData);

	void SetSellRegisteredItem(PD::SC::PKT_SC_ACK_MARKET_TRANSACTION_REGISTER_READ* InData);	

	// 월드 거래소
public:
	// PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_INSTACKABLE_READ::TransactionList
	void SetInstackableItemWd(ItemId InItemId, void* InData);
	// PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ::TransactionList
	void SetStackableItemWd(ItemId InItemId, void* InData);

	// PKT_GC_ACK_WDMARKET_MY_TRANSACTION_LIST_READ::TransactionList
	void SetSellItemWd(void* InData);
	void SetDetailDataWd(PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ* InData);

	void SetSellRegisteredItemWd(PD::SC::PKT_SC_ACK_WDMARKET_TRANSACTION_REGISTER_READ* InData);

public:
	ItemId GetItemId() const { return _itemId; }
	const FGsSchemaItemCommon* GetTable() const;
	MarketPrice GetPricePerUnit() const;
	int32 GetEnchantLevel() const;
	ItemAmount GetAmount() const;
	bool CanCompare() const;
};
