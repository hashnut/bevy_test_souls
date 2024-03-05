// Fill out your copyright notice in the Description page of Project Settings.

#include "Net/MessageHandler/GsNetMessageHandlerBMShop.h"

#include "T1Project.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Shared/Client/SharedEnums/SharedDebugEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Item/GsItemManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateHandler.h"

#include "Net/GsNetSendServiceBMShop.h"

#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGame/GsSummonManager.h"

#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "UTIL/GsTimeSyncUtil.h"


using namespace PD::SC;
using namespace PD::GC;

void FGsNetMessageHandlerBMShop::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& worldProtocal = inManager->GetProtocalWorld();
	InitializeMessageDelegateList(&worldProtocal);

	// Packet
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_BM_SHOP_ITEM_LIST, this, &FGsNetMessageHandlerBMShop::BMShopProductListAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_BM_SHOP_PURCHASE_ITEM, this, &FGsNetMessageHandlerBMShop::BMShopPurchaseProductAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_BM_DEPOT_ITEM_LIST, this, &FGsNetMessageHandlerBMShop::BMShopStorageItemListAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_RECIVIE_BM_DEPOT, this, &FGsNetMessageHandlerBMShop::BMShopReceiveStorageItem
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_BM_SHOP_ITEM_RESTORE, this, &FGsNetMessageHandlerBMShop::BMShopRestoreItem
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_BM_SHOP_PREPARE_HIVE_ITEM, this, &FGsNetMessageHandlerBMShop::BMShopProductValidCheckAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_INFORM_GUILD_GIFT, this, &FGsNetMessageHandlerBMShop::BMShopInformGuildGiftNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE, this, &FGsNetMessageHandlerBMShop::BMShopPG_PurchaseAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_SAVE_BM_BULK_SALE_LIST, this, &FGsNetMessageHandlerBMShop::BMShopSaveBulkSettingAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_LOAD_BM_BULK_SALE_LIST, this, &FGsNetMessageHandlerBMShop::BMShopSaveBulkSettingNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_BM_SHOP_POPUP_LIST, this, &FGsNetMessageHandlerBMShop::BMShopOpenPopupShopAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_BM_SHOP_POPUP_LIST, this, &FGsNetMessageHandlerBMShop::BMShopOpenPopupShopNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_PURCHASED_BM_ITEM_BY_Z_PAY, this, &FGsNetMessageHandlerBMShop::BMShopPurchaseZpayNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_BM_SHOP_BLOCK_MARKET_PID, this, &FGsNetMessageHandlerBMShop::BMShopBlockListNotify
	));

	// Z pay
	// GC
	MProtocalGateway& gatewayProtocol = inManager->GetProtocalGateway();

	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		GCPacketId::GC_NOTIFY_PG_UNCONSUMED_PURCHASE, this, &FGsNetMessageHandlerBMShop::NotifyPgUnconsumedPurchase
	));
	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		GCPacketId::GC_ACK_Z_PAY_PURCHASED_ITEM_LIST, this, &FGsNetMessageHandlerBMShop::BMShopZpayProductListAck
	));
	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		GCPacketId::GC_ACK_Z_PAY_PURCHASE_ITEM, this, &FGsNetMessageHandlerBMShop::BMShopZpayPurchaseProductAck
	));
	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		GCPacketId::GC_ACK_Z_PAY_ITEM_RESTORE, this, &FGsNetMessageHandlerBMShop::BMShopZpayRestoreItem
	));
	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		GCPacketId::GC_ACK_SEACRHING_Z_PAY_USER, this, &FGsNetMessageHandlerBMShop::BMShopSearchZpayUser
	));
	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		GCPacketId::GC_ACK_PREPARE_Z_PAY_PURCHASE_ITEM, this, &FGsNetMessageHandlerBMShop::BMShopZpayProductValidCheckAck
	));
	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		GCPacketId::GC_CALL_Z_PAY_USER, this, &FGsNetMessageHandlerBMShop::BMShopCallZpayUserNotify
	));
	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		GCPacketId::GC_Z_PAY_CANNOT_RESTORE, this, &FGsNetMessageHandlerBMShop::BMShopNothingToRestoreNotify
	));

	/**
	 * 2023/11/1 PKT - 서버 이주
	 */
	_gateWayPackets.Add(gatewayProtocol.AddRawPacket<PKT_GC_ACK_SERVER_MIGRATE_CONDITION_LIST_READ>(
		GCPacketId::GC_ACK_SERVER_MIGRATE_CONDITION_LIST, this, &FGsNetMessageHandlerBMShop::ServerMigrationConditionListAck
	));	

	_gateWayPackets.Add(gatewayProtocol.AddRawPacket<PKT_GC_ACK_SERVER_MIGRATE_READ>(
		GCPacketId::GC_ACK_SERVER_MIGRATE, this, &FGsNetMessageHandlerBMShop::ServerMigrationAck
	));
}

void FGsNetMessageHandlerBMShop::FinalizeMessage()
{
	MProtocalGateway& gatewayProtocol = GNet()->GetProtocalGateway();
	for (auto& e : _gateWayPackets)
	{
		gatewayProtocol.Remove(e);
	}
	_gateWayPackets.Empty();

	FinalizeMessageDelegateList();
}

// 구매 횟수가 있는 아이템들만 정보가 내려옴
void FGsNetMessageHandlerBMShop::BMShopProductListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("BMShopProductListAck()"));

	PKT_SC_ACK_BM_SHOP_ITEM_LIST_READ* pkt = reinterpret_cast<PKT_SC_ACK_BM_SHOP_ITEM_LIST_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}
	
	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	bool isNeedUpdate = false;
	TArray<BMShopProductInfo> productList;
	pkt->MakeProductListVector<TArray<BMShopProductInfo>, BMShopProductInfo>(productList);
	if (0 < productList.Num())
	{
		bmShopMgr->UpdateProduct(productList);
		isNeedUpdate = true;
	}
	
	// 동적으로 인앱결제 상품에 한하여 on / off 처리
	if (0 < pkt->GetBlockListCount())
	{
		TArray<FString> blockProductPidList;
		using blockList = PktIterator<PKT_SC_ACK_BM_SHOP_ITEM_LIST_READ::BlockList>;
		for (blockList iter = pkt->GetFirstBlockListIterator(); iter != pkt->GetLastBlockListIterator(); ++iter)
		{
			blockProductPidList.Emplace(iter->BlockMarketPId());
		}
		bmShopMgr->UpdateBlockList(blockProductPidList);
		isNeedUpdate = true;
	}

	if (isNeedUpdate)
	{
		FGsBMShopManager::FinishedUpdateList();
	}
}

// 인게임 재화(다이아, 골드) / 현금결제 아이템 구매 정보
void FGsNetMessageHandlerBMShop::BMShopPurchaseProductAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	FGsBMShopManager* BMShopManager = GBMShop();
	UGsItemManager* ItemManager = GItem();
	if (nullptr == BMShopManager || nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == BMShopManager || nullptr == ItemManager"));
		return;
	}

	BMShopManager->OnIAPV4SheetsLog(TEXT("BMShopPurchaseProductAck"));

	PKT_SC_ACK_BM_SHOP_PURCHASE_ITEM_READ* pkt = reinterpret_cast<PKT_SC_ACK_BM_SHOP_PURCHASE_ITEM_READ*>(inPacket.Buffer);
	GSLOG(Log, TEXT("%s"), *LexToString(*pkt));

	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		BMShopManager->DeletePurchaseSequentially();

		if (result == PD::Result::ITEM_ERROR_EXPIRED_DATE_TIME)
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Cannot"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	TArray<BMPurchasedResult> resultList;
	TArray<BMProductId> idList;
	TArray<TPair<BMProductId,ItemAmount>> productIdList;
	pkt->MakePurchasedResultVector<TArray<BMPurchasedResult>, BMPurchasedResult>(resultList);
	for (BMPurchasedResult& iter : resultList)
	{
		BMProductIdAmountPair pair = iter.mBMProductIdAmountPair;
		ItemAmount amount =  pair.mAmount;
		ItemAmount totalAmount = iter.mTotalQuantity;
		BMShopManager->UpdatePurchaseQuantity(pair.mBMProductId, totalAmount);
		productIdList.Emplace(TPair<BMProductId, ItemAmount>(pair.mBMProductId, amount));
		idList.Emplace(pair.mBMProductId);
	}
	BMShopManager->FinishedPurchaseItem(productIdList, pkt->IsBulkSale(), true);

	if (false == pkt->IsBulkSale() && 0 < resultList.Num())
	{
		BMProductId productID = resultList[0].mBMProductIdAmountPair.mBMProductId;
		const ItemDBId UsedItemDBId = pkt->CreateItemDBId();// UseItemDBId();
		if (INVALID_ITEM_DB_ID != UsedItemDBId && BMShopManager->IsSummonTypeProduct(productID) || BMShopManager->IsSameTypeProductBySubCategory(productID, ItemCategorySub::GUILD_GIFT_BOX))
		{
			if (true == ItemManager->TryUseItem(UsedItemDBId))
			{
				if (BMShopManager->IsSameTypeProductBySubCategory(productID, ItemCategorySub::SUMMON_COSTUME))
				{
					FGsBMShopManager::CheckCostumeSummonWarnPopup();
				}
				else if (BMShopManager->IsSameTypeProductBySubCategory(productID, ItemCategorySub::SUMMON_FAIRY))
				{
					FGsBMShopManager::CheckFairySummonWarnPopup();
				}
			}
		}
	}

	// [B1] | ejrrb10 | BM 상점 상품 구매 시, 하이브로 유저 추적용 이벤트 전송
	BMShopManager->SendPurchaseEventToHive(idList);

	// 자동순차구매봇 카운트다운 및 후처리
	BMShopManager->CountUpPurchaseSequentially();
	BMShopManager->RunNextPurchaseSequentially();
}

// 보관함 아이템 정보
void FGsNetMessageHandlerBMShop::BMShopStorageItemListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GBMShop()->OnIAPV4SheetsLog(TEXT("BMShopStorageItemListAck"));
	GSLOG(Warning, TEXT("BMShopStorageItemListAck()"));
	
	PKT_SC_ACK_BM_DEPOT_ITEM_LIST_READ* pkt = reinterpret_cast<PKT_SC_ACK_BM_DEPOT_ITEM_LIST_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	TArray<BMDepotDetail> depotDetailList;
	pkt->MakeItemListVector<TArray<BMDepotDetail>, BMDepotDetail>(depotDetailList);
	GBMShop()->UpdateStorageItems(depotDetailList);
}

// 보관함에서 아이템 수령 할 시, SC_ITEMS_UPDATE 패킷을 통해 인벤토리로 지급되며
// 이 패킷에서는 Result 와 받은 보관함 정보를 받음
// SC_ITEMS_UPDATE 후에 이 패킷 호출된다고함
void FGsNetMessageHandlerBMShop::BMShopReceiveStorageItem(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("BMShopReceiveStorageItem()"));

	PKT_SC_ACK_RECIVIE_BM_DEPOT_READ* pkt = reinterpret_cast<PKT_SC_ACK_RECIVIE_BM_DEPOT_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	if (FGsBMShopManager* bmShopMgr = GBMShop())
	{
		TArray<BMItemDBId> depotDetailList;
		pkt->MakeBmItemDBIdListVector<TArray<BMItemDBId>, BMItemDBId>(depotDetailList);
		bmShopMgr->RemoveStorageItems(depotDetailList);
	}

	FText findText;
	FText::FindText(TEXT("BMShopText"), TEXT("tickerGetItem"), findText);
	FGsUIHelper::TrayMessageTicker(findText);
}

void FGsNetMessageHandlerBMShop::BMShopRestoreItem(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("BMShopRestoreItem()"));

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	PKT_SC_ACK_BM_SHOP_ITEM_RESTORE_READ* pkt = reinterpret_cast<PKT_SC_ACK_BM_SHOP_ITEM_RESTORE_READ*>(inPacket.Buffer);

	// [B1] | ejrrb10 | BM 상점 상품 복구 시, 하이브로 유저 추적용 이벤트 전송 (실제 지급 여부와 상관없이)
	bmShopMgr->SendRestoreEventToHive(pkt->MarketPID());

	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (pkt->FinishTransaction())
		{
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("Notice_Error_Restore_NoItem"), findText);

			FString resultText;
			resultText.Append(PD::ResultEnumToDisplayKey(static_cast<PD::Result>(result)));
			resultText.Append(findText.ToString());
			FGsUIHelper::PopupSystemMsg(FText::FromString(resultText));

			MakeShared<FGsBMShopSendTransactionFinishSingle>(pkt->MarketPID())->Send();

			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	FGsNetSendServiceBMShop::SendBMShopStorageList();

	FText findText;
	FText::FindText(TEXT("BMShopText"), TEXT("purchaseSuccess"), findText);
	FGsUIHelper::TrayMessageTicker(findText);

	MakeShared<FGsBMShopSendTransactionFinishSingle>(pkt->MarketPID())->Send();
}

void FGsNetMessageHandlerBMShop::BMShopProductValidCheckAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("BMShopProductValidCheckAck()"));

	PKT_SC_ACK_BM_SHOP_PREPARE_HIVE_ITEM_READ* pkt = reinterpret_cast<PKT_SC_ACK_BM_SHOP_PREPARE_HIVE_ITEM_READ*>(inPacket.Buffer);
	GSLOG(Log, TEXT("%s"), *LexToString(*pkt));

	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_EXPIRED_DATE_TIME)
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Cannot"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	TArray<BMProductIdAmountPair> amountPairList;
	pkt->MakePurchaseListVector<TArray<BMProductIdAmountPair>, BMProductIdAmountPair>(amountPairList);
	
	uint64 Identifier = pkt->BmHiveIdentifier();
	GSLOG(Warning, TEXT("BMShopProductValidCheckAck - pkt->Identifier() : %llu"), Identifier);

	if (false == pkt->IsBulkSale() && 0 < amountPairList.Num())
	{
		BMProductIdAmountPair amountPair =  amountPairList[0];
		// 소환 아이템이면 retry 정보 백업 처리
		if (true == GBMShop()->IsSummonTypeProduct(amountPair.mBMProductId))
		{
			if (FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler())
			{
				summonHandler->SetRetryDataBMShopByProductId(amountPair.mBMProductId, amountPair.mAmount);
			}
		}
	}

	MakeShared<FGsBMShopCheckValidProductResult>(amountPairList, Identifier, pkt->IsBulkSale(), pkt->CouponItemDBId())->Send();
}

void FGsNetMessageHandlerBMShop::BMShopZpayProductValidCheckAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("BMShopZpayProductValidCheckAck()"));

	PD::GC::PKT_GC_ACK_PREPARE_Z_PAY_PURCHASE_ITEM_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_PREPARE_Z_PAY_PURCHASE_ITEM_READ*>(inPacket.Buffer);
	GSLOG(Log, TEXT("%s"), *LexToString(*pkt));

	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_EXPIRED_DATE_TIME)
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Cannot"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}
	
	BMProductId productId = pkt->BmProductId();
	BMHiveIdentifier Identifier = pkt->BmHiveIdentifier();
	GSLOG(Warning, TEXT("BMShopProductValidCheckAck - pkt->BmProductId() : %d"), productId);
	GSLOG(Warning, TEXT("BMShopProductValidCheckAck - pkt->BmHiveIdentifier() : %llu"), Identifier);

	TArray<BMProductIdAmountPair> amountPairList;
	amountPairList.Emplace(BMProductIdAmountPair(productId,1));

	MakeShared<FGsBMShopCheckValidProductResult>(amountPairList, Identifier, false, 0)->Send();
}

void FGsNetMessageHandlerBMShop::BMShopInformGuildGiftNotify(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("BMShopInformGuildGiftAck()"));

	PKT_SC_INFORM_GUILD_GIFT_READ* pkt = reinterpret_cast<PKT_SC_INFORM_GUILD_GIFT_READ*>(inPacket.Buffer);
	if (false == pkt->Validate())
	{
		GSLOG(Error, TEXT("BMShopInformGuildGiftAck() - false == pkt->Validate() "));
		return;
	}

	GSLOG(Error, TEXT("BMShopInformGuildGiftNotify - pkt->UserName() : %s"), *pkt->UserName());
	GSLOG(Error, TEXT("BMShopInformGuildGiftNotify - pkt->ItemId() : %d"), pkt->ItemId());

	FGsBMShopManager::GuildGiftNotice(pkt->UserName(), pkt->ItemId(), pkt->UserDBId());
}

void FGsNetMessageHandlerBMShop::BMShopPG_PurchaseAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("BMShopPG_PurchaseAck()"));

	PKT_SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE_READ* pkt = reinterpret_cast<PKT_SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE_READ*>(inPacket.Buffer);
	GSLOG(Log, TEXT("%s"), *LexToString(*pkt));

	const int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (pkt->IsShowResultToUser())
		{
			// IsShowResultToUser() 인 경우는 유저가 직접 요청한 경우라 에러 팝업을 출력함.
			// spawn 등 자동으로 보낼 때는 에러를 수신해도 에러를 출력하지 않기 위해 추가함.
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		}

		return;
	}

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
	{
		return;
	}

	// 유효한 상품이 없다면, 알림을 사용하지 않기 위한 용도로 사용함.
	bool HasValidUnconsumedPurchase = false;

	if (0 < pkt->GetUnconsumedPurchaseListCount())
	{
		TArray<TPair<BMProductId, ItemAmount>> productIdList;
		using purchaseiter = PktIterator<PD::SC::PKT_SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE_READ::UnconsumedPurchaseList>;
		for (purchaseiter iter = pkt->GetFirstUnconsumedPurchaseListIterator(); iter != pkt->GetLastUnconsumedPurchaseListIterator(); ++iter)
		{
			int iterResult = iter->ProvideItemResult();
			if (iterResult != PACKET_RESULT_SUCCESS)
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
				continue;
			}

			HasValidUnconsumedPurchase = true;

			BMProductId id = iter->ProductId();
			ItemAmount amount = iter->PurchasedQuantity();
			ItemAmount totalAmount = iter->TotalPurchasedQuantity();

			// 결과 티커 출력을 최종적으로 한번에 하는 방식으로 바뀌어야 할듯
			bmShopMgr->UpdatePurchaseQuantity(id, totalAmount);
			productIdList.Emplace(TPair<BMProductId, ItemAmount>(id, totalAmount));
		}

		bmShopMgr->FinishedPurchaseItem(productIdList, false, HasValidUnconsumedPurchase);
	}

	if (pkt->IsShowResultToUser() && !HasValidUnconsumedPurchase)
	{
		// MakeShared<FGsBMShopDisplayNothingToRestore>()->Send();
		// 결제 확인 버튼을 클릭만 해도 발생하여 일단 주석 처리함.
	}
}

void FGsNetMessageHandlerBMShop::BMShopSaveBulkSettingAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("BMShopSaveBulkSettingAck()"));

	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_SAVE_BM_BULK_SALE_LIST_READ* pkt = reinterpret_cast<PKT_SC_ACK_SAVE_BM_BULK_SALE_LIST_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	//pkt->Page();
	if (0 < pkt->BmProductBulkSaleListSize())
	{
		TArray<BMProductBulkSale> settingList;
		pkt->MakeBmProductBulkSaleListVector<TArray<BMProductBulkSale>, BMProductBulkSale>(settingList);
		if (0 < settingList.Num())
		{
			bmShopMgr->ResetBulkSaleSettingData();
		}
		bmShopMgr->UpdateBulkSaleProductList(settingList);
	}
}

void FGsNetMessageHandlerBMShop::BMShopSaveBulkSettingNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("BMShopSaveBulkSettingNotify()"));

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	PKT_SC_LOAD_BM_BULK_SALE_LIST_READ* pkt = reinterpret_cast<PKT_SC_LOAD_BM_BULK_SALE_LIST_READ*>(inPacket.Buffer);
	using bulksaleIter = PktIterator<PD::SC::PKT_SC_LOAD_BM_BULK_SALE_LIST_READ::BmBulkSalePageList>;
		
	TArray<BMProductBulkSale> settingList;
	using saleDataIter = PktIterator<PD::SC::PKT_SC_LOAD_BM_BULK_SALE_LIST_READ::BmBulkSalePageList::BmProductBulkSaleList>;
	for (bulksaleIter iter = pkt->GetFirstBmBulkSalePageListIterator(); iter != pkt->GetLastBmBulkSalePageListIterator(); ++iter)
	{
		//iter->Page();
		if (0 < iter->GetBmProductBulkSaleListCount())
		{
			for (saleDataIter iterAmount = iter->GetFirstBmProductBulkSaleListIterator(); iterAmount != iter->GetLastBmProductBulkSaleListIterator(); ++iterAmount)
			{
				settingList.Emplace(iterAmount->ProductBulkSale());
			}
		}
	}
	bmShopMgr->UpdateBulkSaleProductList(settingList);
}

void FGsNetMessageHandlerBMShop::BMShopOpenPopupShopNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("BMShopOpenPopupShopNotify()"));

	PKT_SC_BM_SHOP_POPUP_LIST_READ* pkt = reinterpret_cast<PKT_SC_BM_SHOP_POPUP_LIST_READ*>(inPacket.Buffer);
	int count = pkt->GetPopupProductListCount();
	if (0 < count)
	{
		FGsBMShopManager* bmShopMgr = GBMShop();
		if (nullptr == bmShopMgr)
			return;

		TArray<TPair<BMProductId, time_t>> popupLisPiarDataList;
		using popupIter = PktIterator<PD::SC::PKT_SC_BM_SHOP_POPUP_LIST_READ::PopupProductList>;
		for (popupIter iter = pkt->GetFirstPopupProductListIterator(); iter != pkt->GetLastPopupProductListIterator(); ++iter)
		{
			FGsBMShopProductData* productData = bmShopMgr->FindProduct(iter->ProductId());
			if (nullptr == productData)
				continue;

			if (productData->IsSlodOut())
			{
				bmShopMgr->CheckPopupShopSoldOut(productData->GetProductID());
				continue;
			}
			popupLisPiarDataList.Emplace(TPair<BMProductId, time_t>(iter->ProductId(), iter->CloseTime()));
		}
		bmShopMgr->UpdatePopupShopNotifyPacketInfo(popupLisPiarDataList);
	}

	MakeShared<FGsBMShopInvalidateProductList>()->Send();
	MakeShared<FGsBMShopInvalidatePopupShopProduct>()->Send();
}

void FGsNetMessageHandlerBMShop::BMShopOpenPopupShopAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("BMShopOpenPopupShopAck()"));

	PKT_SC_ACK_BM_SHOP_POPUP_LIST_READ* pkt = reinterpret_cast<PKT_SC_ACK_BM_SHOP_POPUP_LIST_READ*>(inPacket.Buffer);
	int count = pkt->GetPopupProductListCount();
	if (0 < count)
	{
		FGsBMShopManager* bmShopMgr = GBMShop();
		if (nullptr == bmShopMgr)
			return;

		TArray<TPair<BMProductId, time_t>> popupLisPiarDataList;
		using popupIter = PktIterator<PD::SC::PKT_SC_ACK_BM_SHOP_POPUP_LIST_READ::PopupProductList>;
		for (popupIter iter = pkt->GetFirstPopupProductListIterator(); iter != pkt->GetLastPopupProductListIterator(); ++iter)
		{
			FGsBMShopProductData* productData = bmShopMgr->FindProduct(iter->ProductId());
			if (nullptr == productData)
				continue;

			if (productData->IsSlodOut())
			{
				bmShopMgr->CheckPopupShopSoldOut(productData->GetProductID());
				continue;
			}
			popupLisPiarDataList.Emplace(TPair<BMProductId, time_t>(iter->ProductId(), iter->CloseTime()));
		}
		bmShopMgr->UpdatePopupShopPacketInfo(popupLisPiarDataList);
	}

	MakeShared<FGsBMShopInvalidateProductList>()->Send();
	MakeShared<FGsBMShopInvalidatePopupShopProduct>()->Send();
}

void FGsNetMessageHandlerBMShop::BMShopZpayProductListAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("BMShopZpayProductListAck()"));

	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_Z_PAY_PURCHASED_ITEM_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_Z_PAY_PURCHASED_ITEM_LIST_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;
	
	bool isNeedUpdate = false;
	if (0 < pkt->GetPurchasedLimitListCount())
	{
		//GSLOG(Error, TEXT("pkt->GetPurchasedLimitListCount() : %d"), pkt->GetPurchasedLimitListCount());

		TArray<BMProductIdAmountPair> productList;
		using limitList = PktIterator<PD::GC::PKT_GC_ACK_Z_PAY_PURCHASED_ITEM_LIST_READ::PurchasedLimitList>;
		for (limitList iter = pkt->GetFirstPurchasedLimitListIterator(); iter != pkt->GetLastPurchasedLimitListIterator(); ++iter)
		{
			productList.Emplace(iter->BmProductIdAmountPair());
		}
		bmShopMgr->UpdateProduct(productList);	
		isNeedUpdate = true;
	}

	// 동적으로 인앱결제 상품에 한하여 on / off 처리
	if (0 < pkt->GetBlockListCount())
	{
		//GSLOG(Error, TEXT("pkt->GetBlockListCount() : %d"), pkt->GetBlockListCount());

		TArray<FString> blockProductPidList;
		using blockList = PktIterator<PD::GC::PKT_GC_ACK_Z_PAY_PURCHASED_ITEM_LIST_READ::BlockList>;
		for (blockList iter = pkt->GetFirstBlockListIterator(); iter != pkt->GetLastBlockListIterator(); ++iter)
		{
			blockProductPidList.Emplace(iter->BlockMarketPIdList());
		}
		bmShopMgr->UpdateBlockList(blockProductPidList);
		isNeedUpdate = true;
	}

	if (isNeedUpdate)
	{
		FGsBMShopManager::FinishedUpdateList();
	}
}

// Zpay 구입 관련 패킷. Zpay 로 구매할 경우, PKT_SC_ACK_BM_SHOP_PURCHASE_ITEM_READ 패킷 안내려옴 
void FGsNetMessageHandlerBMShop::BMShopZpayPurchaseProductAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("BMShopZpayPurchaseProductAck()"));

	PD::GC::PKT_GC_ACK_Z_PAY_PURCHASE_ITEM_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_Z_PAY_PURCHASE_ITEM_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	FGsBMShopManager* shopMgr = GBMShop();
	BMPurchasedResult resultData = pkt->BmPurchasedResult();
	BMProductIdAmountPair pair = resultData.mBMProductIdAmountPair;
	ItemAmount amount = pair.mAmount;
	ItemAmount totalAmount = resultData.mTotalQuantity;
	shopMgr->UpdatePurchaseQuantity(pair.mBMProductId, totalAmount);

	TArray<BMProductId> idList;
	TArray<TPair<BMProductId, ItemAmount>> productIdList;
	productIdList.Emplace(TPair<BMProductId, ItemAmount>(pair.mBMProductId, amount));
	idList.Emplace(pair.mBMProductId);
	shopMgr->FinishedPurchaseItem(productIdList, false, true);

	// [B1] | ejrrb10 | BM 상점 상품 구매 시, 하이브로 유저 추적용 이벤트 전송
	shopMgr->SendPurchaseEventToHive(idList);
}

void FGsNetMessageHandlerBMShop::BMShopZpayRestoreItem(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("BMShopZpayRestoreItem()"));

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	PD::GC::PKT_GC_ACK_Z_PAY_ITEM_RESTORE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_Z_PAY_ITEM_RESTORE_READ*>(inPacket.Buffer);

	// [B1] | ejrrb10 | BM 상점 상품 복구 시, 하이브로 유저 추적용 이벤트 전송 (실제 지급 여부와 상관없이)
	bmShopMgr->SendRestoreEventToHive(pkt->MarketPID());

	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (pkt->FinishTransaction())
		{
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("Notice_Error_Restore_NoItem"), findText);

			FString resultText;
			resultText.Append(PD::ResultEnumToDisplayKey(static_cast<PD::Result>(result)));
			resultText.Append(findText.ToString());
			FGsUIHelper::PopupSystemMsg(FText::FromString(resultText));

			MakeShared<FGsBMShopSendTransactionFinishSingle>(pkt->MarketPID())->Send();

			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

#if PLATFORM_WINDOWS
	FGsNetSendServiceBMShop::SendBMShopStorageList();

	FText findText;
	FText::FindText(TEXT("BMShopText"), TEXT("purchaseSuccess"), findText);
	FGsUIHelper::TrayMessageTicker(findText);
#endif

	MakeShared<FGsBMShopSendTransactionFinishSingle>(pkt->MarketPID())->Send();
}

void FGsNetMessageHandlerBMShop::BMShopPurchaseZpayNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("BMShopPurchaseZpayNotify()"));

	PKT_SC_PURCHASED_BM_ITEM_BY_Z_PAY_READ* pkt = reinterpret_cast<PKT_SC_PURCHASED_BM_ITEM_BY_Z_PAY_READ*>(inPacket.Buffer);
	
	// Z 페이를 통하여 구매하였을때 pc 유저에게 구매 완료 되었다는 notify 패킷에서
	// 보관함 업데이트 패킷 요청 함
#if PLATFORM_WINDOWS
	FGsNetSendServiceBMShop::SendBMShopStorageList();

	FText findText;
	FText::FindText(TEXT("BMShopText"), TEXT("purchaseSuccess"), findText);
	FGsUIHelper::TrayMessageTicker(findText);
#endif
}

// pc 유저에게만 응답온다고함
void FGsNetMessageHandlerBMShop::BMShopSearchZpayUser(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("BMShopSearchZpayUser()"));

	PD::GC::PKT_GC_ACK_SEACRHING_Z_PAY_USER_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_SEACRHING_Z_PAY_USER_READ*>(inPacket.Buffer);

	MakeShared<FGsBMShopZpayUserAck>(pkt->IsConnZpayUser())->Send();
}
 
// PKT_GC_ACK_SEACRHING_Z_PAY_USER_READ 패킷 이후에 
// Zpay 사용중이면 PKT_GC_CALL_Z_PAY_USER_READ 응답이 Zpay 기기 한테만 온다고 함
// Zpay 미사용 중이면 아래 패킷 응답 안옴
void FGsNetMessageHandlerBMShop::BMShopCallZpayUserNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("BMShopCallZpayUserNotify()"));

	MakeShared<FGsBMShopSearchZpayUserNotify>()->Send();
}

// pc 유저에게만 응답옴
void FGsNetMessageHandlerBMShop::BMShopNothingToRestoreNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("BMShopNothingToRestoreNotify()"));

	PD::GC::PKT_GC_Z_PAY_CANNOT_RESTORE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_Z_PAY_CANNOT_RESTORE_READ*>(inPacket.Buffer);

#if PLATFORM_WINDOWS
	MakeShared<FGsBMShopDisplayNothingToRestore>()->Send();
#endif
}

// 동적으로 인앱결제 상품에 한하여 on / off 처리
void FGsNetMessageHandlerBMShop::BMShopBlockListNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("BMShopBlockListNotify()"));

	PKT_SC_BM_SHOP_BLOCK_MARKET_PID_READ* pkt = reinterpret_cast<PKT_SC_BM_SHOP_BLOCK_MARKET_PID_READ*>(inPacket.Buffer);
	if (FGsBMShopManager* bmShop = GBMShop())
	{
		bmShop->UpdateBlockProductNotify(pkt->MarketPId(), pkt->IsRemove());
	}
}

void FGsNetMessageHandlerBMShop::NotifyPgUnconsumedPurchase(const FGsNet::Packet& inPacket)
{
	// https://jira.com2us.com/wiki/x/4edCFw
	// 2. 결제 내역이 있는데 아이템 지급이 되지않았다면 클라이언트로 노티한다.(GC_NOTIFY_PG_UNCONSUMED_PURCHASE)
	GSLOG(Warning, TEXT("NotifyPgUnconsumedPurchase()"));

	FGsBMShopManager* BMShopManager = GBMShop();
	if (nullptr != BMShopManager)
	{
		BMShopManager->SetWaitForReqPgUnconsumedPurchase(true);
	}
}

void FGsNetMessageHandlerBMShop::ServerMigrationConditionListAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("ServerMigrationConditionListAck()"));

	// 2023/10/31 PKT - Block을 해야 할까???
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		PKT_GC_ACK_SERVER_MIGRATE_CONDITION_LIST_READ* pkt = data->PacketCast< PKT_GC_ACK_SERVER_MIGRATE_CONDITION_LIST_READ>();
		if (FGsBMShopManager* bmShop = GBMShop())
		{
			if (FGsServerMigrateHandler* handler = bmShop->GetServerMigrateHandler())
			{
				//server 이전 가능 시간, 계정 이동 회수, 기사단 이동 회수
				handler->SetInitServerMigrateInfo(FGsTimeSyncUtil::ConvertToDateTime(pkt->MigrateEndTime()), pkt->AccountMigrateCount(), pkt->GuildMigrateCount());

				auto endMigrateIter = pkt->GetLastMigrateServerListIterator();
				for (auto iter = pkt->GetFirstMigrateServerListIterator(); iter != endMigrateIter; ++iter)
				{
					handler->AddMigrateServerList(MigrateServer
					(iter->PlanetId(),
						iter->WorldId(),
						iter->ServerCongestion(),
						iter->ServerMigrateLimitStatus(),
						iter->IsRecommended(),
						iter->IsNewOpend(),
						iter->UserCount()));
				}

				auto endAccountMigrateIter = pkt->GetLastAccountServerMirateConditionListIterator();
				for (auto iter = pkt->GetFirstAccountServerMirateConditionListIterator(); iter != endAccountMigrateIter; ++iter)
				{
					handler->SetAccountServerMigrateData(iter->ServerMigrateConditionType(), iter->IsPossible());
				}

				auto endGuildMigrateIter = pkt->GetLastGuildServerMirateConditionListIterator();
				for (auto iter = pkt->GetFirstGuildServerMirateConditionListIterator(); iter != endGuildMigrateIter; ++iter)
				{
					handler->SetGuildServerMigrateData(iter->ServerMigrateConditionType(), iter->IsPossible());
				}

				handler->SetEndServerMigrateInfo();
			}
		}
	}
}

void FGsNetMessageHandlerBMShop::ServerMigrationAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		PKT_GC_ACK_SERVER_MIGRATE_READ* pkt = data->PacketCast< PKT_GC_ACK_SERVER_MIGRATE_READ>();

		if (FGsBMShopManager* bmShop = GBMShop())
		{
			if (FGsServerMigrateHandler* handler = bmShop->GetServerMigrateHandler())
			{
				handler->OnAckServerMigration(data->Result());
			}
		}
	}
}
