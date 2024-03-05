#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;
/**
 * 거래소 관련 메시지 핸들러
 */
class T1PROJECT_API FGsNetMessageHandlerMarket : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerMarket() = default;
	virtual ~FGsNetMessageHandlerMarket() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// 일반 거래소
public:
	void TransactionPurchaseAck(const FGsNet::Packet& InPacket);
	void TransactionRegisterAck(const FGsNet::Packet& InPacket);
	void TransactionCancelAck(const FGsNet::Packet& InPacket);
	void CalculateAck(const FGsNet::Packet& InPacket);

	// 월드 거래소
public:
	void WdTransactionPurchaseAck(const FGsNet::Packet& InPacket);
	void WdTransactionRegisterAck(const FGsNet::Packet& InPacket);
	void WdTransactionCancelAck(const FGsNet::Packet& InPacket);
	void WdCalculateAck(const FGsNet::Packet& InPacket);

protected:
	void ShowMessageTicker(const FTextKey& InNamespace, const FTextKey& InKey);
};
