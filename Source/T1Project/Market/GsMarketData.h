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
		struct PKT_GC_ACK_MARKET_ITEM_PRICE_READ;
		struct PKT_GC_ACK_WDMARKET_ITEM_PRICE_READ;
		struct PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ;
		struct PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ;
	}
}


//----------------------------------------------------------------------------------------------------------------------
/**
 * 거래소 아이템 시세 정보
 */
struct T1PROJECT_API FGsMarketItemPriceData final
{
public:
	ItemId _itemId = INVALID_ITEM_ID;
	Level _enchantLevel = INVALID_LEVEL;

	MarketPrice _lowestPrice;
	MarketPrice _heighestPrice;
	MarketPrice _averagePrice;
	MarketPrice _latestPrice;
	MarketPrice _lowestPriceCurrent;

	bool _bIsWorldMarket = false;

public:
	void Clear();
	void SetData(PD::GC::PKT_GC_ACK_MARKET_ITEM_PRICE_READ* InData);
	void SetDataWd(PD::GC::PKT_GC_ACK_WDMARKET_ITEM_PRICE_READ* InData);
};
//----------------------------------------------------------------------------------------------------------------------
/**
 * 거래소 정산 정보
 */
struct T1PROJECT_API FGsMarketSettlementData final
{
	MarketTransactionId _transactionId;
	ItemId _itemId;
	Level _enchantLevel;
	ItemAmount _amount;
	MarketPrice _price;
	MarketPrice _earnings;
	time_t _timeOfSale;

	bool _bIsWorldMarket = false;

	// 상세정보 받았을 때만 생김
	FGsItem* _item = nullptr;

public:
	FGsMarketSettlementData() = default;
	~FGsMarketSettlementData();

public:
	static void GetSaleTimeInfo(const time_t& InTime, 
		OUT FDateTime& OutDateTime, OUT FText& OutDateFixed, OUT FText& OutTimeFixed);

public:
	void Clear();
	
	// 일반 거래소
public:
	// PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ::CalculatedList 
	void SetData(void* InData);	
	// PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ::CalculatedList 
	void SetLogData(void* InData);
	void SetDetailData(PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ* InData);

	// 월드 거래소
public:
	// PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ::CalculatedList
	void SetDataWd(void* InData);
	// PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ::CalculatedList
	void SetLogDataWd(void* InData);
	void SetDetailDataWd(PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ* InData);

public:	
	const FGsSchemaItemCommon* GetTable() const;
	ItemId GetItemId() const { return _itemId; }
};
