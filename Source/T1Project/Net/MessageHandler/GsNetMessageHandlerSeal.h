#pragma once

#include "GsNetMessageHandleInterface.h"
#include "Message/GsMessageNet.h"
#include "Net/GSNet.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsNetManager;
struct SealBonusData;
struct SealData;
struct SealSlotData;

class FGsNetMessageHandlerSeal final : public IGsNetMessageHandler, FGsNetMessageWorld
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
	/** 봉인 정보 수신 */
	void SEAL(const FGsNet::Packet& InPacket);

	/** 봉인 슬롯 잠금 정보 수신 */
	void SEAL_SLOT_LOCK(const FGsNet::Packet& InPacket);

	/** 봉인 레벨업 응답 */
	void ACK_SEAL_LEVEL_UP(const FGsNet::Packet& InPacket);

	/** 봉인 슬롯 전체 변경 응답 */
	void ACK_SEAL_SLOT_CHANGE_ALL(const FGsNet::Packet& InPacket);

	/** 봉인 슬롯 잠금 저장 요청 응답 */
	void ACK_SEAL_SLOT_LOCK_SAVE(const FGsNet::Packet& InPacket);
};
