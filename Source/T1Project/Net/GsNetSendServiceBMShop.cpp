// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/GsNetSendServiceBMShop.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "T1Project.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


// 상품 리스트 send
void FGsNetSendServiceBMShop::SendBMShopProductList()
{
	if (FGsREQ_BM_SHOP_ITEM_LIST().Send())
	{
		FGsUIHelper::ShowBlockUI();
	}
}

void FGsNetSendServiceBMShop::SendZpayBMShopProductList()
{
	if (FGsREQ_Z_PAY_PURCHASED_ITEM_LIST().Send())
	{
		FGsUIHelper::ShowBlockUI();
	}
}

void FGsNetSendServiceBMShop::SendRestoreItem(FString InMarketPID, std::string InBypassInfo)
{
	if (true == InMarketPID.IsEmpty() || true == InBypassInfo.empty())
	{
		GMessage()->GetBMShop().SendMessageAsync(FGsBMShopHiveBlockUiClose::MESSAGE_ID, FGsSharedParam(nullptr));
		return;
	}

	const unsigned char* convert = reinterpret_cast<const unsigned char*>(InBypassInfo.data());
	FGsREQ_BM_SHOP_ITEM_RESTORE(InMarketPID, InBypassInfo.length(), convert).Send();
}

// 인게임 재화 상품 구매시
void FGsNetSendServiceBMShop::SendBMShopPurchaseItem(TArray<BMProductIdAmountPair> InAmountPairList, const uint64 InIdentifier, const bool InIsBulkSale, uint64 InCouponItemDbid)
{
	if (0 >= InAmountPairList.Num())
		return;

	FGsBMShopManager* shopMgr = GBMShop();
	for (BMProductIdAmountPair& iter : InAmountPairList)
	{
		FGsBMShopProductData* data = shopMgr->FindProduct(iter.mBMProductId);
		if (nullptr != data)
		{
			iter.mBMProductId = true == data->IsExistParentProductID() ? data->GetParentProductID() : iter.mBMProductId;
		}
	}

	std::string tempString = "";
	const unsigned char* convert = reinterpret_cast<const unsigned char*>(tempString.data());
	if (FGsREQ_BM_SHOP_PURCHASE_ITEM(InIdentifier, InIsBulkSale, "", InCouponItemDbid, tempString.length(), convert, &InAmountPairList).Send())
	{
		FGsUIHelper::ShowBlockUI();
	}
}

#if WITH_EDITOR
// 에디터로 현금성 상품 구매시 (테스트 전용)
void FGsNetSendServiceBMShop::SendBMShopPurchaseItemTest(const BMProductId InProductId, std::string InBypassInfo, const uint64 InIdentifier)
{
	GSLOG(Log, TEXT("FGsNetSendServiceBMShop::SendBMShopPurchaseItemTest(%u, InBypassInfo, %llu)"), InProductId, InIdentifier);

	FGsBMShopManager* shopMgr = GBMShop();
	FGsBMShopProductData* findData = shopMgr->FindProduct(InProductId);
	if (nullptr == findData)
	{
		return;
	}

	if (findData->IsExistParentProductID())
	{
		findData = shopMgr->FindProduct(findData->GetParentProductID());
	}

	uint16 amount = 1;
	TArray<BMProductIdAmountPair> amountPairList;
	amountPairList.Emplace(BMProductIdAmountPair(findData->GetProductID(), amount));
	const unsigned char* convert = reinterpret_cast<const unsigned char*>(InBypassInfo.data());

	FGsREQ_BM_SHOP_PURCHASE_ITEM(InIdentifier, false, findData->GetTableData()->googleMarketPID, 0, InBypassInfo.length(), convert, &amountPairList).Send();
}
#endif

// 현금성 상품 구매시 (Hive 정보)
void FGsNetSendServiceBMShop::SendBMShopPurchaseItem(FString InMarketPID, std::string InBypassInfo, const uint64 InIdentifier)
{
	FGsBMShopManager* shopMgr = GBMShop();
	FGsBMShopProductData* findData = shopMgr->FindProduct(InMarketPID);
	if (nullptr == findData)
	{
		return;
	}

	if (findData->IsExistParentProductID())
	{
		findData = shopMgr->FindProduct(findData->GetParentProductID());
	}

	uint16 amount = 1;
	TArray<BMProductIdAmountPair> amountPairList;
	amountPairList.Emplace(BMProductIdAmountPair(findData->GetProductID(), amount));
	const unsigned char* convert = reinterpret_cast<const unsigned char*>(InBypassInfo.data());

	FGsREQ_BM_SHOP_PURCHASE_ITEM(InIdentifier, false, findData->GetConvertHiveMarketPID(), 0, InBypassInfo.length(), convert, &amountPairList).Send();
}

void FGsNetSendServiceBMShop::SendBMShopStorageList()
{
	if (FGsREQ_BM_DEPOT_ITEM_LIST().Send())
	{
		FGsUIHelper::ShowBlockUI();
	}
}

void FGsNetSendServiceBMShop::SendReceiveStorageItem(const uint64 InStorageDBID)
{
	if (FGsREQ_RECIVIE_BM_DEPOT(InStorageDBID).Send())
	{
		FGsUIHelper::ShowBlockUI();
	}
}

void FGsNetSendServiceBMShop::SendBMShopProductValidCheck(TArray<BMProductIdAmountPair> InAmountPairList, const bool InBulkSale, const uint64 InCouponItemDbid)
{
	if (0 >= InAmountPairList.Num())
		return;

	FGsBMShopManager* shopMgr = GBMShop();
	for (BMProductIdAmountPair& iter : InAmountPairList)
	{
		FGsBMShopProductData* data = shopMgr->FindProduct(iter.mBMProductId);
		if (nullptr != data)
		{
			iter.mBMProductId = true == data->IsExistParentProductID() ? data->GetParentProductID() : iter.mBMProductId;
		}
	}

	if (InBulkSale)
	{
		if (FGsREQ_BM_SHOP_PREPARE_HIVE_ITEM(TEXT(""), false, true, InCouponItemDbid, &InAmountPairList).Send())
		{
			FGsUIHelper::ShowBlockUI();
		}
	}
	else
	{
		BMProductIdAmountPair pairData = InAmountPairList[0];
		FGsBMShopProductData* findData = shopMgr->FindProduct(pairData.mBMProductId);
		if (nullptr == findData)
			return;

		bool isPG = false;
		FString pid = "";

#if WITH_EDITOR
		if (nullptr != findData->GetTableData())
		{
			pid = findData->GetTableData()->googleMarketPID;
		}
#elif PLATFORM_ANDROID || PLATFORM_IOS
		pid = findData->GetConvertHiveMarketPID();
#elif PLATFORM_WINDOWS
		pid = findData->GetConvertHiveMarketPID();
		isPG = true;
#endif

		if (FGsREQ_BM_SHOP_PREPARE_HIVE_ITEM(pid, true == findData->IsCashProductItem() ? isPG : false, false, InCouponItemDbid, &InAmountPairList).Send())
		{
			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceBMShop::SendBMShopZpayProductValidCheck(const TArray<BMProductIdAmountPair>& InAmountPairList)
{
	if (false == InAmountPairList.IsValidIndex(0))
		return;

	const BMProductIdAmountPair& pairData = InAmountPairList[0];
	if (0 >= pairData.mBMProductId)
		return;

	FGsBMShopManager* shopMgr = GBMShop();
	FGsBMShopProductData* data = shopMgr->FindProduct(pairData.mBMProductId);
	if (nullptr == data)
		return;
	
	const BMProductId reusltId = true == data->IsExistParentProductID() ? data->GetParentProductID() : pairData.mBMProductId;
	FGsBMShopProductData* findData = shopMgr->FindProduct(reusltId);
	if (nullptr == findData)
		return;

	bool isPG = false;
	FString pid = "";
#if WITH_EDITOR
	//pid = true == findData->GetGoogleMarketPID().IsEmpty() ? findData->GetAppStoreMarketPID() : findData->GetGoogleMarketPID();
#elif PLATFORM_ANDROID || PLATFORM_IOS
	pid = findData->GetConvertHiveMarketPID();
#elif PLATFORM_WINDOWS
	pid = findData->GetConvertHiveMarketPID();
	isPG = true;
#endif

	if (FGsREQ_PREPARE_Z_PAY_PURCHASE_ITEM(findData->GetProductID()).Send())
	{
		FGsUIHelper::ShowBlockUI();
	}
}

void FGsNetSendServiceBMShop::SendSaveBulkSaleSetting(const uint16 InPageIndex, TArray<BMProductBulkSale> InPairList)
{
	if (0 >= InPairList.Num())
		return;

	if (FGsREQ_SAVE_BM_BULK_SALE_LIST(InPageIndex, &InPairList).Send())
	{
		FGsUIHelper::ShowBlockUI();
	}
}

void FGsNetSendServiceBMShop::SendPopupShopOpen()
{
	if (FGsREQ_BM_SHOP_POPUP_LIST().Send())
	{
		FGsUIHelper::ShowBlockUI();
	}
}

void FGsNetSendServiceBMShop::SendSearchUsingZpay()
{
	if (FGsREQ_SEACRHING_Z_PAY_USER().Send())
	{
		FGsUIHelper::ShowBlockUI();
	}
}

void FGsNetSendServiceBMShop::SendNothingToRestore()
{
	FGsZ_PAY_CANNOT_RESTORE().Send();
}

void FGsNetSendServiceBMShop::SendServerMigrateConditionList()
{
	GSLOG(Error, TEXT("[FGsNetSendServiceBMShop::SendServerMigrateConditionList]"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_SERVER_MIGRATE_CONDITION_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());

			// 2023/10/31 PKT - Block을 해야 할까???
			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceBMShop::SendServerMigrate(PlanetId planetId, WorldId worldId, ServerMigrateType type)
{
	GSLOG(Error, TEXT("[FGsNetSendServiceBMShop::SendServerMigrate]"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_SERVER_MIGRATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), type, planetId, worldId);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceBMShop::SendReqChangeMigrationNickname(UserDBId dbId, const FString nickName)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_SERVER_MIGRATE_NICKNAME_CHANGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), dbId, nickName);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceBMShop::SendReqChangeMigrationGuildName(UserDBId dbId, const FString guildName)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_SERVER_MIGRATE_GUILD_NAME_CHANGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), dbId, guildName);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_ITEM_LIST_WRITE& InPacket)
{
	return TEXT("PKT_CS_REQ_BM_SHOP_ITEM_LIST_WRITE");
}

FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_PREPARE_HIVE_ITEM_WRITE& InPacket)
{
	PD::CS::PKT_CS_REQ_BM_SHOP_PREPARE_HIVE_ITEM_READ* ReadPacket = reinterpret_cast<PD::CS::PKT_CS_REQ_BM_SHOP_PREPARE_HIVE_ITEM_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CS_REQ_BM_SHOP_PREPARE_HIVE_ITEM_WRITE: IsPGPurchase: %s, IsBulkSale: %s, CouponItemDBId: %s\n"), *LexToString(ReadPacket->IsPGPurchase()), *LexToString(ReadPacket->IsBulkSale()), *LexToString(ReadPacket->CouponItemDBId()));

	int32 i = 0;
	TArray<BMProductIdAmountPair> PurchaseList;
	ReadPacket->MakePurchaseListVector<TArray<BMProductIdAmountPair>, BMProductIdAmountPair>(PurchaseList);
	for (const BMProductIdAmountPair& PurchaseListItem : PurchaseList)
	{
		Logs.Appendf(TEXT(" - PurchaseList[%d] mBMProductId: %s mAmount: %s\n"), i++, *LexToString(PurchaseListItem.mBMProductId), *LexToString(PurchaseListItem.mAmount));
	}

	return Logs;
}

FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_PURCHASE_ITEM_WRITE& InPacket)
{
	PD::CS::PKT_CS_REQ_BM_SHOP_PURCHASE_ITEM_READ* ReadPacket = reinterpret_cast<PD::CS::PKT_CS_REQ_BM_SHOP_PURCHASE_ITEM_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CS_REQ_BM_SHOP_PURCHASE_ITEM_WRITE: BmHiveIdentifier: %s, IsBulkSale: %s, CouponItemDBId: %s, MarketPId: %s\n"), *LexToString(ReadPacket->BmHiveIdentifier()), *LexToString(ReadPacket->IsBulkSale()), *LexToString(ReadPacket->CouponItemDBId()), *ReadPacket->MarketPId());

	int32 i = 0;
	TArray<BMProductIdAmountPair> PurchaseList;
	ReadPacket->MakePurchaseListVector<TArray<BMProductIdAmountPair>, BMProductIdAmountPair>(PurchaseList);
	for (const BMProductIdAmountPair& PurchaseListItem : PurchaseList)
	{
		Logs.Appendf(TEXT(" - PurchaseList[%d] mBMProductId: %s mAmount: %s\n"), i++, *LexToString(PurchaseListItem.mBMProductId), *LexToString(PurchaseListItem.mAmount));
	}

	Logs.Appendf(TEXT(" - Purchase_bypass_info: %s"), *FBase64::Encode(ReadPacket->Purchase_bypass_info(), ReadPacket->Purchase_bypass_infoSize()));
	return Logs;
}

FString LexToString(const PD::CS::PKT_CS_REQ_HIVE_PG_UNCONSUMED_PURCHASE_WRITE& InPacket)
{
	PD::CS::PKT_CS_REQ_HIVE_PG_UNCONSUMED_PURCHASE_READ* ReadPacket = reinterpret_cast<PD::CS::PKT_CS_REQ_HIVE_PG_UNCONSUMED_PURCHASE_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CS_REQ_HIVE_PG_UNCONSUMED_PURCHASE_WRITE: AppId: %s"), *ReadPacket->AppId());

	return Logs;
}

FString LexToString(const PD::CS::PKT_CS_REQ_SAVE_BM_BULK_SALE_LIST_WRITE& InPacket)
{
	PD::CS::PKT_CS_REQ_SAVE_BM_BULK_SALE_LIST_READ* ReadPacket = reinterpret_cast<PD::CS::PKT_CS_REQ_SAVE_BM_BULK_SALE_LIST_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CS_REQ_SAVE_BM_BULK_SALE_LIST_WRITE: PageId: %s\n"), *LexToString(ReadPacket->PageId()));

	int32 i = 0;
	TArray<BMProductBulkSale> BmProductBulkSaleList;
	ReadPacket->MakeBmProductBulkSaleListVector<TArray<BMProductBulkSale>, BMProductBulkSale>(BmProductBulkSaleList);
	for (const BMProductBulkSale& BmProductBulkSale : BmProductBulkSaleList)
	{
		Logs.Appendf(TEXT(" - BmProductBulkSaleList[%d] mIncluding: %s mBMProductId: %s, mAmount: %s\n"), i++, *LexToString(BmProductBulkSale.mIncluding), *LexToString(BmProductBulkSale.mBMProductIdAmountPair.mBMProductId), *LexToString(BmProductBulkSale.mBMProductIdAmountPair.mAmount));
	}

	return Logs;
}

FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_POPUP_LIST_WRITE& InPacket)
{
	return TEXT("PKT_CS_REQ_BM_SHOP_POPUP_LIST_WRITE");
}

FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_ITEM_RESTORE_WRITE& InPacket)
{
	PD::CS::PKT_CS_REQ_BM_SHOP_ITEM_RESTORE_READ* ReadPacket = reinterpret_cast<PD::CS::PKT_CS_REQ_BM_SHOP_ITEM_RESTORE_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CS_REQ_BM_SHOP_ITEM_RESTORE_WRITE: MarketPId: %s\n"), *ReadPacket->MarketPId());
	Logs.Appendf(TEXT(" - Purchase_bypass_info: %s"), *FBase64::Encode(ReadPacket->Purchase_bypass_info(), ReadPacket->Purchase_bypass_infoSize()));
	return Logs;
}

FString LexToString(const PD::CS::PKT_CS_REQ_BM_DEPOT_ITEM_LIST_WRITE& InPacket)
{
	return TEXT("PKT_CS_REQ_BM_DEPOT_ITEM_LIST_WRITE");
}

FString LexToString(const PD::CS::PKT_CS_REQ_RECIVIE_BM_DEPOT_WRITE& InPacket)
{
	PD::CS::PKT_CS_REQ_RECIVIE_BM_DEPOT_READ* ReadPacket = reinterpret_cast<PD::CS::PKT_CS_REQ_RECIVIE_BM_DEPOT_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CS_REQ_RECIVIE_BM_DEPOT_WRITE: BmItemDBId: %s"), *LexToString(ReadPacket->BmItemDBId()));
	return Logs;
}

FString LexToString(const PD::CG::PKT_CG_REQ_Z_PAY_PURCHASED_ITEM_LIST_WRITE& InPacket)
{
	return TEXT("PKT_CG_REQ_Z_PAY_PURCHASED_ITEM_LIST_WRITE");
}

FString LexToString(const PD::CG::PKT_CG_REQ_SEACRHING_Z_PAY_USER_WRITE& InPacket)
{
	return TEXT("PKT_CG_REQ_SEACRHING_Z_PAY_USER_WRITE");
}

FString LexToString(const PD::CG::PKT_CG_REQ_PREPARE_Z_PAY_PURCHASE_ITEM_WRITE& InPacket)
{
	PD::CG::PKT_CG_REQ_PREPARE_Z_PAY_PURCHASE_ITEM_READ* ReadPacket = reinterpret_cast<PD::CG::PKT_CG_REQ_PREPARE_Z_PAY_PURCHASE_ITEM_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CG_REQ_PREPARE_Z_PAY_PURCHASE_ITEM_WRITE: BmProductId: %s"), *LexToString(ReadPacket->BmProductId()));

	return Logs;
}

FString LexToString(const PD::CG::PKT_CG_REQ_Z_PAY_PURCHASE_ITEM_WRITE& InPacket)
{
	PD::CG::PKT_CG_REQ_Z_PAY_PURCHASE_ITEM_READ* ReadPacket = reinterpret_cast<PD::CG::PKT_CG_REQ_Z_PAY_PURCHASE_ITEM_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CG_REQ_Z_PAY_PURCHASE_ITEM_WRITE: BmHiveIdentifier: %s, BmProductId: %s, MarketPId: %s\n"), *LexToString(ReadPacket->BmHiveIdentifier()), *LexToString(ReadPacket->BmProductId()), *ReadPacket->MarketPId());
	Logs.Appendf(TEXT(" - Purchase_bypass_info: %s"), *FBase64::Encode(ReadPacket->Purchase_bypass_info(), ReadPacket->Purchase_bypass_infoSize()));
	return Logs;
}

FString LexToString(const PD::CG::PKT_CG_REQ_Z_PAY_ITEM_RESTORE_WRITE& InPacket)
{
	PD::CG::PKT_CG_REQ_Z_PAY_ITEM_RESTORE_READ* ReadPacket = reinterpret_cast<PD::CG::PKT_CG_REQ_Z_PAY_ITEM_RESTORE_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CG_REQ_Z_PAY_ITEM_RESTORE_WRITE: MarketPId: %s\n"), *ReadPacket->MarketPId());
	Logs.Appendf(TEXT(" - Purchase_bypass_info: %s"), *FBase64::Encode(ReadPacket->Purchase_bypass_info(), ReadPacket->Purchase_bypass_infoSize()));
	return Logs;
}

FString LexToString(const PD::CG::PKT_CG_Z_PAY_CANNOT_RESTORE_WRITE& InPacket)
{
	return TEXT("PKT_CG_Z_PAY_CANNOT_RESTORE_WRITE");
}

FString LexToString(const PD::CG::PKT_CG_OPEN_PG_PURCHASE_WND_WRITE& InPacket)
{
	return TEXT("PKT_CG_OPEN_PG_PURCHASE_WND_WRITE");
}

FString LexToString(const PD::SC::PKT_SC_ACK_BM_SHOP_PURCHASE_ITEM_READ& InPacket)
{
	PD::SC::PKT_SC_ACK_BM_SHOP_PURCHASE_ITEM_READ* ReadPacket = const_cast<PD::SC::PKT_SC_ACK_BM_SHOP_PURCHASE_ITEM_READ*>(&InPacket);
	FString Logs;
	Logs.Appendf(TEXT("PKT_SC_ACK_BM_SHOP_PURCHASE_ITEM_READ: Result: %s, IsBulkSale: %s, CreateItemDBId: %s\n"), *LexToString(ReadPacket->Result()), *LexToString(ReadPacket->IsBulkSale()), *LexToString(ReadPacket->CreateItemDBId()));

	int32 i = 0;
	TArray<BMPurchasedResult> PurchasedResultList;
	ReadPacket->MakePurchasedResultVector<TArray<BMPurchasedResult>, BMPurchasedResult>(PurchasedResultList);
	for (const BMPurchasedResult& PurchasedResult : PurchasedResultList)
	{
		Logs.Appendf(TEXT(" - PurchasedResult[%d] mBMProductId: %s, mAmount: %s, mTotalQuantity: %s\n"), i++, *LexToString(PurchasedResult.mBMProductIdAmountPair.mBMProductId), *LexToString(PurchasedResult.mBMProductIdAmountPair.mAmount), *LexToString(PurchasedResult.mTotalQuantity));
	}
	return Logs;
}

FString LexToString(const PD::SC::PKT_SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE_READ& InPacket)
{
	PD::SC::PKT_SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE_READ* ReadPacket = const_cast<PD::SC::PKT_SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE_READ*>(&InPacket);
	FString Logs;
	Logs.Appendf(TEXT("PKT_SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE_READ: Result: %s\n"), *LexToString(ReadPacket->Result()));

	int32 i = 0;
	for (PD::SC::PKT_SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE_READ::UnconsumedPurchaseListIterator It = ReadPacket->GetFirstUnconsumedPurchaseListIterator(); It != ReadPacket->GetLastUnconsumedPurchaseListIterator(); ++It)
	{
		Logs.Appendf(TEXT(" - UnconsumedPurchaseList[%d] ProvideItemResult: %s, ProductId: %s, PurchasedQuantity: %s, TotalPurchasedQuantity: %s\n"), i++, *LexToString(It->ProvideItemResult()), *LexToString(It->ProductId()), *LexToString(It->PurchasedQuantity()), *LexToString(It->TotalPurchasedQuantity()));
	}
	return Logs;
}

FString LexToString(const PD::SC::PKT_SC_ACK_BM_SHOP_PREPARE_HIVE_ITEM_READ& InPacket)
{
	PD::SC::PKT_SC_ACK_BM_SHOP_PREPARE_HIVE_ITEM_READ* ReadPacket = const_cast<PD::SC::PKT_SC_ACK_BM_SHOP_PREPARE_HIVE_ITEM_READ*>(&InPacket);
	FString Logs;
	Logs.Appendf(TEXT("PKT_SC_ACK_BM_SHOP_PREPARE_HIVE_ITEM_READ: Result: %s, IsBulkSale: %s, BmHiveIdentifier: %s, CouponItemDBId: %s\n"), *LexToString(ReadPacket->Result()), *LexToString(ReadPacket->IsBulkSale()), *LexToString(ReadPacket->BmHiveIdentifier()), *LexToString(ReadPacket->CouponItemDBId()));

	int32 i = 0;
	TArray<BMProductIdAmountPair> PurchaseList;
	ReadPacket->MakePurchaseListVector<TArray<BMProductIdAmountPair>, BMProductIdAmountPair>(PurchaseList);
	for (const BMProductIdAmountPair& PurchaseListItem : PurchaseList)
	{
		Logs.Appendf(TEXT(" - PurchaseList[%d] mBMProductId: %s mAmount: %s\n"), i++, *LexToString(PurchaseListItem.mBMProductId), *LexToString(PurchaseListItem.mAmount));
	}
	return Logs;
}

FString LexToString(const PD::GC::PKT_GC_ACK_PREPARE_Z_PAY_PURCHASE_ITEM_READ& InPacket)
{
	PD::GC::PKT_GC_ACK_PREPARE_Z_PAY_PURCHASE_ITEM_READ* ReadPacket = const_cast<PD::GC::PKT_GC_ACK_PREPARE_Z_PAY_PURCHASE_ITEM_READ*>(&InPacket);
	FString Logs;
	Logs.Appendf(TEXT("PKT_GC_ACK_PREPARE_Z_PAY_PURCHASE_ITEM_READ: Result: %s, BmHiveIdentifier: %s, BmProductId: %s\n"), *LexToString(ReadPacket->Result()), *LexToString(ReadPacket->BmHiveIdentifier()), *LexToString(ReadPacket->BmProductId()));
	return Logs;
}
