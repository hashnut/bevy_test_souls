// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/GSNet.h"
#include "GsNetMessageHandleInterface.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"


/**
 * 게이트웨이 패킷 중 거래소 관련 패킷 핸들러
 */
class T1PROJECT_API FGsNetMessageHandlerGatewayMarket : public IGsNetMessageHandler, FGsNetMessageGateway
{
public:
	FGsNetMessageHandlerGatewayMarket() = default;
	virtual ~FGsNetMessageHandlerGatewayMarket() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// 거래소
public:
	void MarketSalesItemListAck(const FGsNet::Packet &inPacket);
	void MarketTransactionListInstackableAck(const FGsNet::Packet &inPacket);
	void MarketTransactionListStackableAck(const FGsNet::Packet &inPacket);
	void MarketCalculateListAck(const FGsNet::Packet &inPacket);
	void MarketMyTransactionListAck(const FGsNet::Packet &inPacket);
	void MarketItemPriceAck(const FGsNet::Packet& InPacket);
	void MarketItemSearchAck(const FGsNet::Packet& InPacket);
	void MarketUncalculatedTransactionNotify(const FGsNet::Packet& InPacket);
	void MarketTransactionDetailEquipAck(const FGsNet::Packet& InPacket);
	void MarketBookmarkListAck(const FGsNet::Packet& InPacket);
	void MarketBookmarkRegisterAck(const FGsNet::Packet& InPacket);
	void MarketBookmarkDeregisteredAck(const FGsNet::Packet& InPacket);
	void MarketCalculatedListAck(const FGsNet::Packet& InPacket);

	// 컬렉션 거래소 연동(월드 같이 씀)
	void CollectionItemMarketPriceLowestAck(const FGsNet::Packet& InPacket);

	// 누적 세금
	void AckContinentTax(const FGsNet::Packet& InPacket);

	//------------------------------------------------------------------------------------------------------------------
	// 월드 거래소
public:
	void WdMarketSalesItemListAck(const FGsNet::Packet& inPacket);
	void WdMarketTransactionListInstackableAck(const FGsNet::Packet& inPacket);
	void WdMarketTransactionListStackableAck(const FGsNet::Packet& inPacket);
	void WdMarketCalculateListAck(const FGsNet::Packet& inPacket);
	void WdMarketMyTransactionListAck(const FGsNet::Packet& inPacket);
	void WdMarketItemPriceAck(const FGsNet::Packet& InPacket);
	void WdMarketItemSearchAck(const FGsNet::Packet& InPacket);
	void WdMarketUncalculatedTransactionNotify(const FGsNet::Packet& InPacket);
	void WdMarketTransactionDetailEquipAck(const FGsNet::Packet& InPacket);
	void WdMarketBookmarkListAck(const FGsNet::Packet& InPacket);
	void WdMarketBookmarkRegisterAck(const FGsNet::Packet& InPacket);
	void WdMarketBookmarkDeregisteredAck(const FGsNet::Packet& InPacket);
	void WdMarketCalculatedListAck(const FGsNet::Packet& InPacket);
};
