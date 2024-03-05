// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedMarketEnum.h"

struct FGsMarketItemData;

/**
 * 거래소 아이템 그룹 정보
 */
struct T1PROJECT_API FGsMarketItemGroupData final
{
public:
	// 내용 고정할 항목(ResetData 시 지우지 않음)
	ItemId _itemId = INVALID_ITEM_ID;
	const FGsSchemaItemCommon* _table = nullptr;
	MarketCategory _marketCategory = MarketCategory::MAX;
	
	// 서버에서 받는 항목(ResetData 시 지움)
	MarketPrice _lowestPrice = 0; // 최저 판매 금액
	uint16 _salesCount = 0; // 판매 개수

	// 편의를 위해 포인터만 저장하는 항목. 판매목록에서 받은건 저장안함.
	TArray<FGsMarketItemData*> _itemList;
	TMap<int32, TArray<FGsMarketItemData*>> _itemLevelMap;

public:
	FGsMarketItemGroupData() = default;
	~FGsMarketItemGroupData() = default;

public:
	void Set(const FGsSchemaItemCommon* InTable);
	
	// 일반 거래소
public:
	// PKT_GC_ACK_MARKET_SALES_ITEM_LIST_READ::SalesItemList
	void SetData(void* InData);
	// PKT_GC_ACK_MARKET_BOOKMARK_LIST_READ::MarketBookmarkList
	void SetDataBookmark(void* InData);

	// 월드 거래소
public:
	// PKT_GC_ACK_WDMARKET_SALES_ITEM_LIST_READ::SalesItemList
	void SetDataWd(void* InData);
	// PKT_GC_ACK_WDMARKET_BOOKMARK_LIST_READ::MarketBookmarkList
	void SetDataBookmarkWd(void* InData);


	void ClearItemGroupData();
	void ClearItemData();
	void SetItemData(FGsMarketItemData* InData);
	void RemoveItemData(MarketTransactionId InTransitionId);

public:	
	bool IsSaleItemExist() const;
	bool IsStackableItem() const { return (_table && ItemType::EQUIP != _table->type) ? true : false; }
	bool IsStatExist() const { return (_table && ItemType::EQUIP == _table->type) ? true : false; }
	const FGsSchemaItemCommon* GetTable() const { return _table; }
	ItemId GetItemId() const { return _itemId; }
	FString GetItemNameString() const;
	ItemGrade GetItemGrade() const;
};
