// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerMarket.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Market/GsMarketControllerNormal.h"
#include "Market/GsMarketControllerWorld.h"
#include "Item/GsItemManager.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "Currency/GsCurrencyHelper.h"

using namespace PD::SC;
using namespace PD;

void FGsNetMessageHandlerMarket::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& world = inManager->GetProtocalWorld();

	InitializeMessageDelegateList(&world);

	// 일반 거래소
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_MARKET_TRANSACTION_PURCHASE,
		this, &FGsNetMessageHandlerMarket::TransactionPurchaseAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_MARKET_TRANSACTION_REGISTER,
		this, &FGsNetMessageHandlerMarket::TransactionRegisterAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_MARKET_TRANSACTION_CANCEL,
		this, &FGsNetMessageHandlerMarket::TransactionCancelAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_MARKET_CALCULATE,
		this, &FGsNetMessageHandlerMarket::CalculateAck));

	// 월드 거래소
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_WDMARKET_TRANSACTION_PURCHASE,
		this, &FGsNetMessageHandlerMarket::WdTransactionPurchaseAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_WDMARKET_TRANSACTION_REGISTER,
		this, &FGsNetMessageHandlerMarket::WdTransactionRegisterAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_WDMARKET_TRANSACTION_CANCEL,
		this, &FGsNetMessageHandlerMarket::WdTransactionCancelAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_WDMARKET_CALCULATE,
		this, &FGsNetMessageHandlerMarket::WdCalculateAck));
}

void FGsNetMessageHandlerMarket::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerMarket::TransactionPurchaseAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_MARKET_TRANSACTION_PURCHASE_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_MARKET_TRANSACTION_PURCHASE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		// 예외 상황 블록 풀기
		FGsUIHelper::HideBlockUIExplicit(FName(TEXT("TransactionPurchase")));
		return;
	}

	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		FGsUIHelper::HideBlockUIExplicit(FName(TEXT("TransactionPurchase")));
		return;
	}

	FGsMarketControllerNormal* controller = marketMgr->GetNormalController();
	if (nullptr == controller)
	{
		return;
	}

	// 구매 결과 저장(다중구매 시 표시등에 사용하기 위함)
	marketMgr->SetPurchaseResult(false, pkt->MarketTransactionId(), pkt->Result());
	 
	if (PACKET_RESULT_SUCCESS == pkt->Result())
	{
		int32 successCount = marketMgr->GetPurchaseSuccessCount();

		// 다중 구매 시 사운드 중복이 있을까봐 첫 성공 한 번만 소리 재생
		if (1 == successCount)
		{
			if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
			{
				soundPlayer->PlaySound2D(TEXT("UI_RewardOne"));
			}
		}

		// 성공 시 거래소 아이템 삭제
		marketMgr->RemoveItem(false, pkt->MarketTransactionId());
	}
	else
	{
		FGsUIHelper::LogNetError(static_cast<Result>(pkt->Result()));
	}

	FGsMarketMsgParamPurchaseResult param(pkt->MarketTransactionId(), pkt->Result());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_PURCHASE_RESULT, &param);

	// 위의 삭제 및 갱신 로직 이후에 불러서 풀도록 변경
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("TransactionPurchase")));
}

void FGsNetMessageHandlerMarket::TransactionRegisterAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_MARKET_TRANSACTION_REGISTER_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_MARKET_TRANSACTION_REGISTER_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		Result result = static_cast<Result>(pkt->Result());
		if (Result::MARKET_ERROR_ITEM_REGISTER_COUNT_OVER == result)
		{
			// TEXT: 더 이상 아이템을 판매 등록할 수 없습니다.
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_MaxRegistration"));
			return;
		}
		else if (Result::MARKET_ERROR_NOT_TRADABLE_ITEM == result)
		{
			// TEXT: 판매할 수 없는 아이템 입니다.
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_Bound"));
			return;
		}
		else if (Result::MARKET_ERROR_CANT_REGISTER_EQUIPED_ITEM == result)
		{
			// TEXT: 장착 중인 아이템입니다.
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_Equipped"));
			return;
		}

		// TEXTL 아이템 판매 등록을 실패하였습니다.
		ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Sales_Fail"));

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 등록된 아이템 저장
	if (FGsMarketControllerNormal* controller = GSMarket()->GetNormalController())
	{
		controller->SetSellRegisteredItem(pkt);
	}

	FGsMarketMsgParamId param(pkt->MarketTransactionId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_SELL_REGISTERED_ITEM, &param);
}

void FGsNetMessageHandlerMarket::TransactionCancelAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_MARKET_TRANSACTION_CANCEL_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_MARKET_TRANSACTION_CANCEL_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		Result result = static_cast<Result>(pkt->Result());

		if (Result::MARKET_ERROR_NOT_FOUND_TRANSACTION == result ||
			Result::MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION == result)
		{
			// TEXT: 이미 팔리거나 판매 취소된 아이템입니다.
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Purchase_Fail_Soldout"));

			// 새로고침 요청
			GMessage()->GetMarket().SendMessage(MessageContentMarket::REQ_REFRESH, nullptr);
			return;
		}

		// TEXT: 거래소에 판매 등록한 아이템의 회수에 실패하였습니다.
		ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_SalesWithdraw_Fail"));

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 아이템 제거
	GSMarket()->RemoveItem(false, pkt->MarketTransactionId());

	FGsMarketMsgParamId param(pkt->MarketTransactionId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_SELL_CANCEL_ITEM, &param);

	// TEXT: 거래소에 판매 등록한 아이템의 회수에 성공하였습니다.
	ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_SalesWithdraw_Success"));
}

void FGsNetMessageHandlerMarket::CalculateAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_MARKET_CALCULATE_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_MARKET_CALCULATE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		Result result = static_cast<Result>(pkt->Result());

		if (Result::MARKET_ERROR_EXCESS_DIA == result)
		{
			// TEXT: 다이아 최대 보유 개수를 초과하여 정산할 수 없습니다
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Adjustment_Fail_MaxDia"));
			return;
		}

		// TEXT: 판매 금액 정산에 실패하였습니다.
		ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Adjustment_Fail"));

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 정산된 목록 제거
	if (FGsMarketManager* marketMgr = GSMarket())
	{
		if (pkt->IsAll())
		{
			marketMgr->RemoveAllSettlementItems(false);
		}
		else
		{
			marketMgr->RemoveSettlementItem(false, pkt->MarketTransactionId());
		}
	}

	// 갱신 메시지
	FGsMessageHolder* messageMgr = GMessage();
	messageMgr->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
	messageMgr->GetMarket().SendMessage(MessageContentMarket::UPDATE_RED_DOT, nullptr);

	// TEXT: 판매 금액 정산에 성공하였습니다.
	ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Adjustment_Success"));
}

void FGsNetMessageHandlerMarket::WdTransactionPurchaseAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_WDMARKET_TRANSACTION_PURCHASE_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_WDMARKET_TRANSACTION_PURCHASE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		// 예외 상황 블록 풀기
		FGsUIHelper::HideBlockUIExplicit(FName(TEXT("TransactionPurchase")));
		return;
	}

	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		FGsUIHelper::HideBlockUIExplicit(FName(TEXT("TransactionPurchase")));
		return;
	}

	FGsMarketControllerWorld* controller = marketMgr->GetWorldController();
	if (nullptr == controller)
	{
		return;
	}

	// 구매 결과 저장(다중구매 시 표시등에 사용하기 위함)
	marketMgr->SetPurchaseResult(true, pkt->MarketTransactionId(), pkt->Result());

	if (PACKET_RESULT_SUCCESS == pkt->Result())
	{
		int32 successCount = marketMgr->GetPurchaseSuccessCount();

		// 다중 구매 시 사운드 중복이 있을까봐 첫 성공 한 번만 소리 재생
		if (1 == successCount)
		{
			if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
			{
				soundPlayer->PlaySound2D(TEXT("UI_RewardOne"));
			}
		}

		// 성공 시 거래소 아이템 삭제
		marketMgr->RemoveItem(true, pkt->MarketTransactionId());
	}
	else
	{
		FGsUIHelper::LogNetError(static_cast<Result>(pkt->Result()));
	}

	FGsMarketMsgParamPurchaseResult param(pkt->MarketTransactionId(), pkt->Result());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_PURCHASE_RESULT, &param);

	// 위의 삭제 및 갱신 로직 이후에 불러서 풀도록 변경
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("TransactionPurchase")));
}

void FGsNetMessageHandlerMarket::WdTransactionRegisterAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_WDMARKET_TRANSACTION_REGISTER_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_WDMARKET_TRANSACTION_REGISTER_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		Result result = static_cast<Result>(pkt->Result());
		if (Result::MARKET_ERROR_ITEM_REGISTER_COUNT_OVER == result)
		{
			// TEXT: 더 이상 아이템을 판매 등록할 수 없습니다.
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_MaxRegistration"));
			return;
		}
		else if (Result::MARKET_ERROR_NOT_TRADABLE_ITEM == result)
		{
			// TEXT: 판매할 수 없는 아이템 입니다.
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_Bound"));
			return;
		}
		else if (Result::MARKET_ERROR_CANT_REGISTER_EQUIPED_ITEM == result)
		{
			// TEXT: 장착 중인 아이템입니다.
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_Equipped"));
			return;
		}

		// TEXTL 아이템 판매 등록을 실패하였습니다.
		ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Sales_Fail"));

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 등록된 아이템 저장
	if (FGsMarketControllerWorld* controller = GSMarket()->GetWorldController())
	{
		controller->SetSellRegisteredItem(pkt);
	}

	FGsMarketMsgParamId param(pkt->MarketTransactionId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_SELL_REGISTERED_ITEM, &param);
}

void FGsNetMessageHandlerMarket::WdTransactionCancelAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_WDMARKET_TRANSACTION_CANCEL_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_WDMARKET_TRANSACTION_CANCEL_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		Result result = static_cast<Result>(pkt->Result());

		if (Result::MARKET_ERROR_NOT_FOUND_TRANSACTION == result ||
			Result::MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION == result)
		{
			// TEXT: 이미 팔리거나 판매 취소된 아이템입니다.
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Purchase_Fail_Soldout"));

			// 새로고침 요청
			GMessage()->GetMarket().SendMessage(MessageContentMarket::REQ_REFRESH, nullptr);
			return;
		}

		// TEXT: 거래소에 판매 등록한 아이템의 회수에 실패하였습니다.
		ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_SalesWithdraw_Fail"));

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 아이템 제거
	GSMarket()->RemoveItem(true, pkt->MarketTransactionId());

	FGsMarketMsgParamId param(pkt->MarketTransactionId());
	GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_SELL_CANCEL_ITEM, &param);

	// TEXT: 거래소에 판매 등록한 아이템의 회수에 성공하였습니다.
	ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_SalesWithdraw_Success"));
}

void FGsNetMessageHandlerMarket::WdCalculateAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_WDMARKET_CALCULATE_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_WDMARKET_CALCULATE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		Result result = static_cast<Result>(pkt->Result());

		if (Result::MARKET_ERROR_EXCESS_DIA == result)
		{
			// TEXT: 다이아 최대 보유 개수를 초과하여 정산할 수 없습니다
			ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Adjustment_Fail_MaxDia"));
			return;
		}

		// TEXT: 판매 금액 정산에 실패하였습니다.
		ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Adjustment_Fail"));

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 정산된 목록 제거
	if (FGsMarketManager* marketMgr = GSMarket())
	{
		if (pkt->IsAll())
		{
			marketMgr->RemoveAllSettlementItems(true);
		}
		else
		{
			marketMgr->RemoveSettlementItem(true, pkt->MarketTransactionId());
		}
	}

	// 갱신 메시지
	FGsMessageHolder* messageMgr = GMessage();
	messageMgr->GetMarket().SendMessage(MessageContentMarket::UPDATE_ITEM_LIST, nullptr);
	messageMgr->GetMarket().SendMessage(MessageContentMarket::UPDATE_RED_DOT, nullptr);

	// TEXT: 판매 금액 정산에 성공하였습니다.
	ShowMessageTicker(TEXT("MarketText"), TEXT("Ticker_Adjustment_Success"));
}

void FGsNetMessageHandlerMarket::ShowMessageTicker(const FTextKey& InNamespace, const FTextKey& InKey)
{
	FText textMsg;
	if (FText::FindText(InNamespace, InKey, textMsg))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}
