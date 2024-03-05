#pragma once

#include "GsNetMessageHandleInterface.h"

class FGsNetManager;
/**
 * 이벤트 네트워크 메세지 핸들러
 */
class T1PROJECT_API FGsNetMessageHandlerEvent final : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerEvent() = default;
	virtual ~FGsNetMessageHandlerEvent() = default;

public:
	void InitializeMessage(FGsNetManager* inManager) override;
	void FinalizeMessage() override;

	// Ack
public:
	void AckEventAction(const FGsNet::Packet& InPacket);


protected:
	/**
	 * 순서에 의존적이지 않으면서 즉시 발동 되는 이벤트
	 */
	void AckOnEventActionImmediate(const FGsNet::Packet & InPacket);
};
