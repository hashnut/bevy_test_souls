#pragma once

#include "CoreMinimal.h"
#include "Net/GSNet.h"
#include "GsNetMessageHandleInterface.h"

/**
 * 
 */
class T1PROJECT_API FGsNetMessageHandlerBuddy : public IGsNetMessageHandler, FGsNetMessageGateway
{
public:
	FGsNetMessageHandlerBuddy() = default;
	virtual ~FGsNetMessageHandlerBuddy() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// 현재 친구 리스트 v1
	void BuddListAck(const FGsNet::Packet &inPacket);
	// 친구 초대 리스트 v1
	void BuddyRecvInvitationListAck(const FGsNet::Packet &inPacket);
	// 친구 요청 리스트 v1
	void BuddySendInvitationListAck(const FGsNet::Packet &inPacket);
	// 내가 친구를 초대 했을 때 v1
	void BuddyInviteAck(const FGsNet::Packet &inPacket);
	// 내가 친구를 삭제 했을 때 v1
	void BuddyDeleteAck(const FGsNet::Packet &inPacket);
	// 상대방이 친구 요청을 했을 때 초대 알림 v1
	void BuddyInvitedNotify(const FGsNet::Packet &inPacket);
	// 친구 요청을 수락 했을 때 v1
	void BuddyAcceptAck(const FGsNet::Packet &inPacket);
	// 친구가 내 초대에 응했을 때 (맞친구가 됐을때) v1
	void BuddyAcceptedNotify(const FGsNet::Packet &inPacket);
	// 내가 친구의 초대를 거절 또는 삭제 했을 때 v1
	void BuddyDeleteInvitationdAck(const FGsNet::Packet &inPacket);
		// 상대가 초대 거절 또는 초대 취소 알림 v1
	void BuddyInvitationDeletedNotify(const FGsNet::Packet &inPacket);
	// 친구가 나를 친구 삭제 했을 때 v1
	void BuddyDeletedNotify(const FGsNet::Packet &inPacket);

	// 친구가 로그인 했을 때
	void BuddyLoginNotify(const FGsNet::Packet& inPacket);
	// 친구가 로그아웃 했을 때
	void BuddyLogoutNotify(const FGsNet::Packet& inPacket);

	// 현재 친구 리스트 v2
	void BuddListAck_v2(const FGsNet::Packet& inPacket);
	// 친구 초대 리스트 v2
	void BuddyRecvInvitationListAck_v2(const FGsNet::Packet& inPacket);
	// 친구 요청 리스트 v2
	void BuddySendInvitationListAck_v2(const FGsNet::Packet& inPacket);
	// 내가 친구를 초대 했을 때 v2
	void BuddyInviteAck_v2(const FGsNet::Packet& inPacket);
	// 상대방이 친구 요청을 했을 때 초대 알림 v2
	void BuddyInvitedNotify_v2(const FGsNet::Packet& inPacket);
	// 친구 요청을 수락 했을 때 v2
	void BuddyAcceptAck_v2(const FGsNet::Packet& inPacket);
	// 친구가 내 초대에 응했을 때 (맞친구가 됐을때) v2
	void BuddyAcceptedNotify_v2(const FGsNet::Packet& inPacket);
	// 내가 친구의 초대를 거절 또는 삭제 했을 때 v2
	void BuddyDeleteInvitationdAck_v2(const FGsNet::Packet& inPacket);
	// 상대가 초대 거절 또는 초대 취소 알림 v2
	void BuddyInvitationDeletedNotify_v2(const FGsNet::Packet& inPacket);
	// 내가 친구를 삭제 했을 때 v2
	void BuddyDeleteAck_v2(const FGsNet::Packet& inPacket);
	// 친구가 나를 친구 삭제 했을 때 v2
	void BuddyDeletedNotify_v2(const FGsNet::Packet& inPacket);
};
