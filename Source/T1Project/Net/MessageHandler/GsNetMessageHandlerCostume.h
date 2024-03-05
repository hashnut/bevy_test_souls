#pragma once
#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerCostume : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerCostume() = default;

public:
	virtual void InitializeMessage(FGsNetManager * inManager) override;
	virtual void FinalizeMessage() override;

protected:

	// Notify
	void NotifyCostumeList(const FGsNet::Packet& inPacket);
	void NotifyCreatedCostume(const FGsNet::Packet& inPacket); 
	void NotifyDeletedCostume(const FGsNet::Packet& inPacket);
	void NotifyCostumeCollectionList(const FGsNet::Packet& inPacket);
	void NotifyCostumeSummonWaitList(const FGsNet::Packet& inPacket);
	void NotifyCostumeObtain(const FGsNet::Packet& inPacket);

	//ack
	void AckCostumeEquip(const FGsNet::Packet& inPacket); // 내것이어도 SC_USER_LOOK_INFO_CHANGED 시에 외형 처리
	void AckCostumeCompose(const FGsNet::Packet& inPacket);
	void AckCostumeSummonWaitChange(const FGsNet::Packet& inPacket);
	void AckCostumeSummonConfirm(const FGsNet::Packet& inPacket);
	void AckCostumeComposeDeliveryBoxRetry(const FGsNet::Packet& inPacket);
	void AckCostumeCeiling(const FGsNet::Packet& inPacket);

	void AckExchangeCostume(const FGsNet::Packet& inPacket);
};