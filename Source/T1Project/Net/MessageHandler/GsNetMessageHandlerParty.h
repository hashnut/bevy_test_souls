#pragma once
#include "CoreMinimal.h"
#include "Net/GSNet.h"
#include "Net/MessageHandler/GsNetMessageHandleInterface.h"

// 파티 패킷 수신부
class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerParty : public IGsNetMessageHandler, FGsNetMessageGateway
{
public:
	FGsNetMessageHandlerParty() = default;
	virtual ~FGsNetMessageHandlerParty() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// ack	
public:

	void PartyCreate(const FGsNet::Packet& inPacket);
	// 초대 보낸 사람이 받는 패킷
	void PartyInvite(const FGsNet::Packet& inPacket);
	// 초대 받는 사람이 받는 패킷
	void PartyInvited(const FGsNet::Packet& inPacket);
	// 초대 수락
	void PartyJoined(const FGsNet::Packet& inPacket);
	// 멤버 추가(초대 수락)
	void PartyMemberJoined(const FGsNet::Packet& inPacket);
	// 멤버 갱신
	void PartyMemberUpdate(const FGsNet::Packet& inPacket);
	// 초대 거절
	void PartyRefuseInvitation(const FGsNet::Packet& inPacket);
	// 초대 실패
	void PartyInviteFailed(const FGsNet::Packet& inPacket);
	// 파티 나감(내가)
	void PartyLeave(const FGsNet::Packet& inPacket);
	// 파티원 나감
	void PartyMemberLeft(const FGsNet::Packet& inPacket);
	// 추방시킴 응답
	void PartyKick(const FGsNet::Packet& inPacket);
	// 추방 당함
	void PartyKicked(const FGsNet::Packet& inPacket);
	// 멤버중 하나 추방 당함
	void PartyMemeberKicked(const FGsNet::Packet& inPacket);
	// 리더 변경 응답(내가 요청)
	void PartyChangeLeader(const FGsNet::Packet& inPacket);
	// 리더 변경
	void PartyLeaderChanged(const FGsNet::Packet& inPacket);
	// 전리품 분배룰 변경 응답
	void PartyChangeDropOwnershipType(const FGsNet::Packet& inPacket);
	// 전리품 분배룰 변경됨
	void PartyDropOwnershipTypeChanged(const FGsNet::Packet& inPacket);


	// 2022/06/13 PKT - 파티 단위의 컨텐츠 중 준비 단계가 있는 컨텐츠에 한해 해당 패킷이 옴
	void PartyMemberUpdateReadyState(const FGsNet::Packet& inPacket);
	void PartyMyUpdateReadyState(const FGsNet::Packet& inPacket);
};