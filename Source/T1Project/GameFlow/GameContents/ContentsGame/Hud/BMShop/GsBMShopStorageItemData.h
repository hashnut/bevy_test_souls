// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * BM상점 보관함 아이템 Data
 */
class T1PROJECT_API FGsBMShopStorageItemData
{
private:
	BMItemDBId _purchaseItemDBID = 0;
	ItemId _itemTID;
	FDateTime _purchaseDateTime;
	BMProductId _productId = 0;


public:
	FGsBMShopStorageItemData() = default;
	explicit FGsBMShopStorageItemData(const BMItemDBId InBMShopItemDBID, const BMProductId InProductId, const ItemId InItemTID, const time_t InPurchaseDateTime);
	~FGsBMShopStorageItemData() = default;

public:
	void UpdatePacketData(struct BMDepotDetail& InServerData);

public:
	const BMItemDBId GetStorageItemDBID() { return _purchaseItemDBID; }
	const ItemId GetStorageItemTID() { return _itemTID; }
	const FDateTime GetStorageItemPurchaseDateTime() { return _purchaseDateTime; }
	const BMProductId GetProductId() { return _productId; }

};


/*
*   BMItemDBId  mBMItemDBId = INVALID_BM_ITEM_DB_ID;
	ItemId      mItemId = INVALID_ITEM_ID;
	time_t      mPurchasedDateTime = 0;
*/