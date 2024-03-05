// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "UTIL/GsIntervalChecker.h"

namespace PD
{
	namespace GC
	{
		struct PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ;
	}
}

/**
 * 컬렉션 최저가 체크에 사용되는 객체
 */
struct FGsMarketLowestPriceRequestInfo final
{
public:
	ItemId _itemId = INVALID_ITEM_ID;
	Level _enchantLevel = INVALID_LEVEL;
	int32 _transactionCountRequired = 0;

public:
	explicit FGsMarketLowestPriceRequestInfo(ItemId InItemId, Level InLevel, int32 InReqCount)
		: _itemId(InItemId), _enchantLevel(InLevel), _transactionCountRequired(InReqCount)
	{
	}
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 컬렉션에서 사용되는 아이템 정보. 개선: FGsMarketItemData로 대체할 수 있을지 생각해볼 것
 */
struct FGsMarketCollectionItemData final
{
public:
	MarketTransactionId _transactionId = INVALID_MARKET_TRANSACTION_ID;
	ItemId _itemId = INVALID_ITEM_ID;
	Level _enchantLevel = INVALID_LEVEL;
	ItemAmount _amount = 0;
	MarketPrice _price = 0;

	bool _bIsWorldMarket = false;

public:
	FGsMarketCollectionItemData() = default;
	// PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ::TransactionList
	void SetData(void* InData);

public:
	const FGsSchemaItemCommon* GetTable() const;

	bool IsSameItem(const TPair<ItemId, Level>& InKeyPair) const;
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 컬렉션 거래소 정보
 */
struct FGsMarketCollectionData final
{
public:
	using PriceKeyPair = TPair<ItemId, Level>;	

protected:
	CollectionId _collectionId;

	TArray<FGsMarketCollectionItemData> _pricePerUnitDataList; // 최저단가(메인 값)
	TArray<FGsMarketCollectionItemData> _priceDataList; // 최저가
	TMap<ItemId, bool> _otherLevelMap; // 다른 레벨 아이템 존재팔 경우 데이터 존재, value: isWorldMarket

	// 서버에 자주 요청을 보내지 않기 위해 시간을 체크
	// 주의: 아이템 대체했을 경우 갱신 필요
	FGsIntervalChecker _checkerPrice;
	FGsIntervalChecker _checkerPricePerUnit;

public:
	FGsMarketCollectionData() = default;

public:
	static void GetLowestPriceRequestInfo(const class FGsItemCollectionField* InData, 
		OUT TArray<FGsMarketLowestPriceRequestInfo>& OutList);
	static bool IsStackableItem(ItemId InItemId);

public:
	void Init(CollectionId InCollectionId);
	void Clear();
	void SetData(PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ* InData);

protected:
	FGsIntervalChecker& GetChecker(bool bInUsePricePerUnit = true);

public:
	TArray<FGsMarketCollectionItemData>& GetDataList(bool bInUsePricePerUnit = true);

	// 최저단가 가져감
	const FGsMarketCollectionItemData* GetLowestPricePerUnitItem(const PriceKeyPair& InKey);

	void GetMarketItemList(TArray<PriceKeyPair>& InKeyList, bool bInUsePricePerUnit, 
		OUT TArray<const FGsMarketCollectionItemData*>& OutList);

	bool IsOtherLevelExist(ItemId InItem) const;
	bool GetOtherLevelMarket(ItemId InItem, OUT bool& OutIsWorldMarket);

	CollectionId GetCollectionId() const { return _collectionId; }

	void UpdateCoolTime(bool bInUsePricePerUnit = true);
	void ResetCoolTime(bool bInUsePricePerUnit = true);
	bool IsCoolTime(bool bInUsePricePerUnit = true);
};
