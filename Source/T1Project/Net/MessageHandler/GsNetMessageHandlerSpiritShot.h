#pragma once

#include "GsNetMessageHandleInterface.h"
#include "Message/GsMessageNet.h"
#include "Net/GSNet.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsNetManager;

class FGsNetMessageHandlerSpiritShot final : public IGsNetMessageHandler, FGsNetMessageWorld
{
	/************************************************************************/
	/* instancing															*/
	/************************************************************************/

public:
	void InitializeMessage(FGsNetManager* InManager) final;

	void FinalizeMessage() final;

	/************************************************************************/
	/* acknowledge															*/
	/************************************************************************/

private:
	/** 천마석 정보 수신 */
	void SPIRIT_SHOT_INFO(const FGsNet::Packet& InPacket);

	/** 천마석 강화 결과 수신 */
	void ACK_SPIRIT_SHOT_ENCHANT(const FGsNet::Packet& InPacket);

private:
	/** 에러 패킷일 경우 처리 */
	void HandleErrorPacket(const PD::Result InResult) const;
};