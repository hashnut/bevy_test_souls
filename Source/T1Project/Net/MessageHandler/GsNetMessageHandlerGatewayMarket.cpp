// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerGatewayMarket.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedUnlockEnum.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsSiegeWarfareManager.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Market/GsMarketControllerNormal.h"
#include "Market/GsMarketControllerWorld.h"

using namespace PD::GC;

void FGsNetMessageHandlerGatewayMarket::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalGateway& protocalGatewayHandler = inManager->GetProtocalGateway();

	InitializeMessageDelegateList(&protocalGatewayHandler);	

	// 거래소 패킷
	AddMessageDelegate(protocalGatewayHandler.AddRaw(GCPacketId::GC_ACK_MARKET_SALES_ITEM_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::MarketSalesItemListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE,
		this, &FGsNetMessageHandlerGatewayMarket::MarketTransactionListInstackableAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE,
		this, &FGsNetMessageHandlerGatewayMarket::MarketTransactionListStackableAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_CALCULATE_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::MarketCalculateListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_MY_TRANSACTION_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::MarketMyTransactionListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_ITEM_PRICE,
		this, &FGsNetMessageHandlerGatewayMarket::MarketItemPriceAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_ITEM_SEARCH,
		this, &FGsNetMessageHandlerGatewayMarket::MarketItemSearchAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_NOTIFY_MARKET_UNCALCULATED_TRANSACTION,
		this, &FGsNetMessageHandlerGatewayMarket::MarketUncalculatedTransactionNotify));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP,
		this, &FGsNetMessageHandlerGatewayMarket::MarketTransactionDetailEquipAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_BOOKMARK_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::MarketBookmarkListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_BOOKMARK_REGISTER,
		this, &FGsNetMessageHandlerGatewayMarket::MarketBookmarkRegisterAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_BOOKMARK_DEREGISTERED,
		this, &FGsNetMessageHandlerGatewayMarket::MarketBookmarkDeregisteredAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MARKET_CALCULATED_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::MarketCalculatedListAck));
	
	// 컬렉션 거래소 연동
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST,
		this, &FGsNetMessageHandlerGatewayMarket::CollectionItemMarketPriceLowestAck));

	// 누적 세금
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CONTINENT_TAX,
		this, &FGsNetMessageHandlerGatewayMarket::AckContinentTax));	

	// 월드 거래소 -------------------------------------------------------------
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_SALES_ITEM_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketSalesItemListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_TRANSACTION_LIST_INSTACKABLE,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketTransactionListInstackableAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_TRANSACTION_LIST_STACKABLE,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketTransactionListStackableAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_CALCULATE_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketCalculateListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_MY_TRANSACTION_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketMyTransactionListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_ITEM_PRICE,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketItemPriceAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_ITEM_SEARCH,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketItemSearchAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_NOTIFY_WDMARKET_UNCALCULATED_TRANSACTION,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketUncalculatedTransactionNotify));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketTransactionDetailEquipAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_BOOKMARK_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketBookmarkListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_BOOKMARK_REGISTER,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketBookmarkRegisterAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_BOOKMARK_DEREGISTERED,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketBookmarkDeregisteredAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WDMARKET_CALCULATED_LIST,
		this, &FGsNetMessageHandlerGatewayMarket::WdMarketCalculatedListAck));

	// 일단 연동 안함. 필요 시 추가
	//GC_ACK_COMMUNITY_TALK_WDMARKET
}

void FGsNetMessageHandlerGatewayMarket::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerGatewayMarket::MarketSalesItemListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_SALES_ITEM_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_SALES_ITEM_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetItemGroupList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::MarketTransactionListInstackableAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		// 바뀐게 없어서 빈 리스트가 왔음. 에러가 아님.
		if (PD::Result::MARKET_ERROR_NO_UPDATED_TRANSACTION == result)
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetInstackableItemList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::MarketTransactionListStackableAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		// 바뀐게 없어서 빈 리스트가 왔음. 에러가 아님.
		if (PD::Result::MARKET_ERROR_NO_UPDATED_TRANSACTION == result)
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetStackableItemList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::MarketCalculateListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_CALCULATE_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_CALCULATE_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 정산 리스트 저장
	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetSettlementItemList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::MarketMyTransactionListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_MY_TRANSACTION_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_MY_TRANSACTION_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetSellItemList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::MarketItemPriceAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_ITEM_PRICE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_ITEM_PRICE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetItemPrice(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_ITEM_PRICE, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::MarketItemSearchAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_ITEM_SEARCH_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_ITEM_SEARCH_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetSearchedList(pkt);
	}

	if (GUI()->IsActiveWidget(TEXT("WindowMarket")))
	{
		FGsMarketMsgParamBool param(false);
		GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_SEARCH_RESULT, &param);
	}
	else
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET))
		{
			GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MARKET);
		}
		else
		{
			GSUnlock()->ShowLockTicker(EGsUnlockContentsType::MARKET);
		}
	}
}

void FGsNetMessageHandlerGatewayMarket::MarketUncalculatedTransactionNotify(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_NOTIFY_MARKET_UNCALCULATED_TRANSACTION_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_NOTIFY_MARKET_UNCALCULATED_TRANSACTION_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->SetMarketSettlementCount(false, pkt->UncalculatedTransactionCount());
	// 레드닷 갱신 요청. 파라미터 false값 상관 없음.
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MARKET, false);
	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_RED_DOT, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::MarketTransactionDetailEquipAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	// 아이템 상세정보 저장
	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetItemDetailInfo(pkt);
	}

	FGsMarketMsgParamId param(pkt->MarketTransactionId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_ITEM_DETAIL_INFO, &param);
}

void FGsNetMessageHandlerGatewayMarket::MarketBookmarkListAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetBookmarkList(pkt);
	}

	FGsMessageHolder* messageMgr = GMessage();
	messageMgr->GetMarket().SendMessage(MessageContentMarket::RCV_BOOKMARK_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::MarketBookmarkRegisterAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_REGISTER_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_REGISTER_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetBookmark(pkt->ItemId(), true);
	}

	FGsMarketMsgParamId param(pkt->ItemId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_BOOKMARK, &param);
}

void FGsNetMessageHandlerGatewayMarket::MarketBookmarkDeregisteredAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_DEREGISTERED_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_DEREGISTERED_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetBookmark(pkt->ItemId(), false);
	}

	FGsMarketMsgParamId param(pkt->ItemId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_BOOKMARK, &param);
}

void FGsNetMessageHandlerGatewayMarket::MarketCalculatedListAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetSettlementLogList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::CollectionItemMarketPriceLowestAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GSMarket()->SetCollectionData(pkt);

	FGsMarketMsgParamId param(pkt->CollectionId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_COLLECTION_PRICE_LIST, &param);
}

void FGsNetMessageHandlerGatewayMarket::AckContinentTax(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CONTINENT_TAX_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_CONTINENT_TAX_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	//if (PACKET_RESULT_SUCCESS != pkt->Result())
	//{
	//	FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
	//	return;
	//}

	FGsTaxDataHandler* taxHandler = nullptr;
	if (FGsSiegeWarfareManager* siegeMgr = GSSiegeWarfare())
	{
		taxHandler = siegeMgr->GetTaxHandler();
	}

	if (nullptr == taxHandler)
	{
		return;
	}

	taxHandler->SetContinentTax(*pkt);

	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_ACCUM_TAX_LIST, nullptr);
}

//------------------------------------------------------------------------------------------------------------------------
void FGsNetMessageHandlerGatewayMarket::WdMarketSalesItemListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_SALES_ITEM_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_SALES_ITEM_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetItemGroupList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketTransactionListInstackableAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_INSTACKABLE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_INSTACKABLE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		// 바뀐게 없어서 빈 리스트가 왔음. 에러가 아님.
		if (PD::Result::MARKET_ERROR_NO_UPDATED_TRANSACTION == result)
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetInstackableItemList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketTransactionListStackableAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_STACKABLE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_STACKABLE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		// 바뀐게 없어서 빈 리스트가 왔음. 에러가 아님.
		if (PD::Result::MARKET_ERROR_NO_UPDATED_TRANSACTION == result)
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetStackableItemList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketCalculateListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_CALCULATE_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_CALCULATE_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 정산 리스트 저장
	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetSettlementItemList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketMyTransactionListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_MY_TRANSACTION_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_MY_TRANSACTION_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetSellItemList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketItemPriceAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_ITEM_PRICE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_ITEM_PRICE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetItemPrice(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_ITEM_PRICE, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketItemSearchAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_ITEM_SEARCH_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_ITEM_SEARCH_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetSearchedList(pkt);
	}

	if (GUI()->IsActiveWidget(TEXT("WindowMarket")))
	{
		FGsMarketMsgParamBool param(true);
		GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_SEARCH_RESULT, &param);
	}
	else
	{
		if (FGsUnlockManager* unlockMgr = GSUnlock())
		{
			if (unlockMgr->IsContentsUnlock(EGsUnlockContentsType::MARKET))
			{
				if (unlockMgr->IsContentsUnlock(EGsUnlockContentsType::MARKET_WORLD))
				{
					GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MARKET);
				}
				else
				{
					unlockMgr->ShowLockTicker(EGsUnlockContentsType::MARKET_WORLD);
				}
			}
			else
			{
				unlockMgr->ShowLockTicker(EGsUnlockContentsType::MARKET);
			}
		}
	}
}

void FGsNetMessageHandlerGatewayMarket::WdMarketUncalculatedTransactionNotify(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_NOTIFY_WDMARKET_UNCALCULATED_TRANSACTION_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_NOTIFY_WDMARKET_UNCALCULATED_TRANSACTION_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->SetMarketSettlementCount(true, pkt->UncalculatedTransactionCount());
	// 레드닷 갱신 요청. 파라미터 false값 상관 없음.
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MARKET, false);
	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_RED_DOT, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketTransactionDetailEquipAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	// 아이템 상세정보 저장
	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetItemDetailInfo(pkt);
	}

	FGsMarketMsgParamId param(pkt->MarketTransactionId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_ITEM_DETAIL_INFO, &param);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketBookmarkListAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_BOOKMARK_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_BOOKMARK_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetBookmarkList(pkt);
	}

	FGsMessageHolder* messageMgr = GMessage();
	messageMgr->GetMarket().SendMessage(MessageContentMarket::RCV_BOOKMARK_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketBookmarkRegisterAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_BOOKMARK_REGISTER_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_BOOKMARK_REGISTER_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetBookmark(pkt->ItemId(), true);
	}

	FGsMarketMsgParamId param(pkt->ItemId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_BOOKMARK, &param);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketBookmarkDeregisteredAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_BOOKMARK_DEREGISTERED_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_BOOKMARK_DEREGISTERED_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetBookmark(pkt->ItemId(), false);
	}

	FGsMarketMsgParamId param(pkt->ItemId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_BOOKMARK, &param);
}

void FGsNetMessageHandlerGatewayMarket::WdMarketCalculatedListAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetSettlementLogList(pkt);
	}

	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
}
