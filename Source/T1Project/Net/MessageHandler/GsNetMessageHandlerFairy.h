#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerFairy : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerFairy() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	
	// UI - Collection, 강화 합성
protected:
	// Ack
	// void FairyUpgradeAck(const FGsNet::Packet& inPacket); //UI
	// void FairyCompositionAck(const FGsNet::Packet& inPacket); //UI

	// Notify
	void NotifyFairyList(const FGsNet::Packet& inPacket); //full list SC_FAIRY	
	void NotifyCreatedFairy(const FGsNet::Packet& inPacket); //update list SC_FAIRY_CREATE
	void NotifyDeletedFairy(const FGsNet::Packet& inPacket); //update list SC_FAIRY_DELETE
	void NotifyFairyCollectionList(const FGsNet::Packet& inPacket); //fairy collection info SC_FAIRY_COLLECTION
	void NotifyFairySummonWaitList(const FGsNet::Packet& inPacket); //fairy summon 대기 리스트
	void NotifyFairyObtain(const FGsNet::Packet& inPacket); //fairy 획득
	void NotifyMountVehicle(const FGsNet::Packet& inPacket);	// 탈것 탑승
	void NotifyUnmountVehicle(const FGsNet::Packet& inPacket);	// 탈것 탑승 해제


	//ack
	void AckFairyEquip(const FGsNet::Packet& inPacket); //SC_ACK_FAIRY_EQUIP list만 갱신 - 내것이어도 SC_USER_LOOK_INFO_CHANGED 시에 외형 처리
	void AckFairyEnchant(const FGsNet::Packet& inPacket);
	void AckFairyCompose(const FGsNet::Packet& inPacket);
	void AckFairySummonWaitChange(const FGsNet::Packet& inPacket);
	void AckFairySummonConfirm(const FGsNet::Packet& inPacket);
	void AckFairyComposeDeliveryBoxRetry(const FGsNet::Packet& inPacket); // 페어리 재합성
	void AckFairyCeiling(const FGsNet::Packet& inPacket);

	void AckMountVehicleFailure(const FGsNet::Packet& inPacket);	// 탑승 요청 실패

	

	//void SpawnMeAck(const FGsNet::Packet& inPacket); //내가 다시 spawn 될때 update 필요
	//void SpawnUserAck(const FGsNet::Packet& inPacket); //타인이 spawn 될때 update 필요
	//void UserLookInfoChangeAck(const FGsNet::Packet& inPacket); //착용에 관한 Update

};

