#pragma once
#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"
#include "Net/GSNet.h"

class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerChat : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerChat() = default;

public:
	virtual void InitializeMessage(FGsNetManager * inManager) override;
	virtual void FinalizeMessage() override;

protected:
	void NotifyGateWayChat(const FGsNet::Packet& inPacket);		//world가 아닌 gateway에서 오는 챗
	void NotifyGateWayWhisper(const FGsNet::Packet& inPacket);
	void NotifyGateWayGuildChatList(const FGsNet::Packet& inPacket);

	void NotifyGateWayChatBlockList(const FGsNet::Packet& inPacket);
	void NotifyGateWayOffLineWhisperList(const FGsNet::Packet& inPacket);

	void AckGateWayChat(const FGsNet::Packet& inPacket);
	void AckGateWayChatWhisper(const FGsNet::Packet& inPacket);;

	void AckGateWayGuildChatList(const FGsNet::Packet& inPacket);
	void AckGateWayChatReport(const FGsNet::Packet& InPacket);
	void AckGateWayAddChatBlockList(const FGsNet::Packet& InPacket);
	void AckGateWayRemoveChatBlockList(const FGsNet::Packet& InPacket);
	void AckGateWayCheckUserExist(const FGsNet::Packet& InPacket);

	void NotifyWorldServerChat(const FGsNet::Packet& InPacket);

	void NotifySystemMessage(const FGsNet::Packet& InPacket);
	void NotifySystemMessageItemGetDrop(const FGsNet::Packet& InPacket);
	void NotifySystemMessageItemGetBox(const FGsNet::Packet& InPacket);
	void NotifySystemMessageItemEnchant(const FGsNet::Packet& InPacket);
	void NotifySystemMessageFairyGet(const FGsNet::Packet& InPacket);
	void NotifySystemMessageCostumeGet(const FGsNet::Packet& InPacket);
	void NotifySystemMessageFieldBossItemGet(const FGsNet::Packet& inPacket);
	void NotifySystemMessageItemGetDropOnParty(const FGsNet::Packet& inPacket);
	void NotifySystemMessageTresasureNPC(const FGsNet::Packet& inPacket);

	// 운영 공지사항
	void NotifyGateWayOperatorMessage(const FGsNet::Packet& InPacket);

	// 침공 챗 초기 정보
	void NotifyInvasionChatInfo(const FGsNet::Packet& InPacket);

	// social motion
	void NotifySocialEmotionList(const FGsNet::Packet& inPacket);
	void AckSocialEmotionActive(const FGsNet::Packet& inPacket);
	void SocialEmotionAck(const FGsNet::Packet& inPacket);

	void AckInterWorldCheckUserExist(const FGsNet::Packet& inPacket);
protected:
	TArray<TPair<PD::GC::GCPacketId, FDelegateHandle>> _gateWayPackets;
};

