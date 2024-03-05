// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopStorageItemData.h"
#include "UTIL/GsTimeSyncUtil.h"


FGsBMShopStorageItemData::FGsBMShopStorageItemData(const BMItemDBId InBMShopItemDBID, const BMProductId InProductId, const ItemId InItemTID, const time_t InPurchaseDateTime)
{
	_purchaseItemDBID = InBMShopItemDBID;
	_itemTID = InItemTID;
	_productId = InProductId;
	_purchaseDateTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(InPurchaseDateTime);
}

void FGsBMShopStorageItemData::UpdatePacketData(struct BMDepotDetail& InServerData)
{
	_itemTID = InServerData.mItemId;
	_productId = InServerData.mBmProdutId;
	_purchaseDateTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(InServerData.mPurchasedDateTime);
}


/*
* // TODO: 차후 국가별로 utc 시간 계산해서 출력해야함
		FDateTime clientDataTime = FGsTimeSyncUtil::GetClientNowDateTime();
*/
