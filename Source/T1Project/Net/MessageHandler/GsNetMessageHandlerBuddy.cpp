// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerBuddy.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGame/GsBuddyManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Buddy/GsBuddyHandler.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "T1Project.h"  
#include "Message/GsMessageNet.h"


using namespace PD::GC;

void FGsNetMessageHandlerBuddy::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	::MProtocalGateway& protocalGatewayHandler = inManager->GetProtocalGateway();

	InitializeMessageDelegateList(&protocalGatewayHandler);

	/////// v1 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_LIST, this, &FGsNetMessageHandlerBuddy::BuddListAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_RECV_INVITATION_LIST, this, &FGsNetMessageHandlerBuddy::BuddyRecvInvitationListAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_SEND_INVITATION_LIST, this, &FGsNetMessageHandlerBuddy::BuddySendInvitationListAck
	));	
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_INVITE, this, &FGsNetMessageHandlerBuddy::BuddyInviteAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_DELETE, this, &FGsNetMessageHandlerBuddy::BuddyDeleteAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_INVITED, this, &FGsNetMessageHandlerBuddy::BuddyInvitedNotify
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_ACCEPT, this, &FGsNetMessageHandlerBuddy::BuddyAcceptAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_ACCEPTED, this, &FGsNetMessageHandlerBuddy::BuddyAcceptedNotify
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_DELETE_INVITATION, this, &FGsNetMessageHandlerBuddy::BuddyDeleteInvitationdAck
	));	
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_INVITATION_DELETED, this, &FGsNetMessageHandlerBuddy::BuddyInvitationDeletedNotify
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_DELETED, this, &FGsNetMessageHandlerBuddy::BuddyDeletedNotify
	));
	
	/////// v2 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_LIST_V2, this, &FGsNetMessageHandlerBuddy::BuddListAck_v2
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_RECV_INVITATION_LIST_V2, this, &FGsNetMessageHandlerBuddy::BuddyRecvInvitationListAck_v2
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_SEND_INVITATION_LIST_V2, this, &FGsNetMessageHandlerBuddy::BuddySendInvitationListAck_v2
	));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_INVITE_V2, this, &FGsNetMessageHandlerBuddy::BuddyInviteAck_v2
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_INVITED_V2, this, &FGsNetMessageHandlerBuddy::BuddyInvitedNotify_v2
	));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_ACCEPT_V2, this, &FGsNetMessageHandlerBuddy::BuddyAcceptAck_v2
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_ACCEPTED_V2, this, &FGsNetMessageHandlerBuddy::BuddyAcceptedNotify_v2
	));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_DELETE_INVITATION_V2, this, &FGsNetMessageHandlerBuddy::BuddyDeleteInvitationdAck_v2
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_INVITATION_DELETED_V2, this, &FGsNetMessageHandlerBuddy::BuddyInvitationDeletedNotify_v2
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_ACK_BUDDY_DELETE_V2, this, &FGsNetMessageHandlerBuddy::BuddyDeleteAck_v2
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_DELETED_V2, this, &FGsNetMessageHandlerBuddy::BuddyDeletedNotify_v2
	));

	/////// v1 v2 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_LOGIN, this, &FGsNetMessageHandlerBuddy::BuddyLoginNotify
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		GCPacketId::GC_NOTIFY_BUDDY_LOGOUT, this, &FGsNetMessageHandlerBuddy::BuddyLogoutNotify
	));
}

void FGsNetMessageHandlerBuddy::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

// 현재 친구 리스트
void FGsNetMessageHandlerBuddy::BuddListAck(const FGsNet::Packet & inPacket)
{
	// v1은 사용안함
	return;

	// 친구가 없다면 빈배열
	// 클라이언트가 요청하지 않아도 최초 월드 진입 시 패킷을 1회 내려 줌.
	PKT_GC_ACK_BUDDY_LIST_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_LIST_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
	{
		GGameData()->Set(*pkt);
		return;
	}

	buddyHandler->RecvBuddListAck(*pkt);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_LIST"));
#endif
}

// 친구 초대 리스트
void FGsNetMessageHandlerBuddy::BuddyRecvInvitationListAck(const FGsNet::Packet & inPacket)
{
	// v1은 사용안함
	return;

	// 초대가 없다면 빈배열
	// 클라이언트가 요청하지 않아도 최초 월드 진입 시 패킷을 1회 내려 줌.
	
	PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
	{
		GGameData()->Set(*pkt);
		return;
	}

	buddyHandler->RecvBuddyRecvInvitationListAck(*pkt);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_RECV_INVITATION_LIST"));
#endif
}

// 친구 요청 리스트
void FGsNetMessageHandlerBuddy::BuddySendInvitationListAck(const FGsNet::Packet & inPacket)
{
	// v1은 사용안함
	return;

	// 요청이 없다면 빈배열
	// 클라이언트가 요청하지 않아도 최초 월드 진입 시 패킷을 1회 내려 줌

	PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_READ*>(inPacket.Buffer);
	
	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
	{
		GGameData()->Set(*pkt);
		return;
	}

	buddyHandler->RecvBuddySendInvitationListAck(*pkt);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_SEND_INVITATION_LIST"));
#endif
}

void FGsNetMessageHandlerBuddy::BuddyInviteAck(const FGsNet::Packet & inPacket)
{
	// v1은 사용안함
	return;

	/*
	result:
		PACKET_RESULT_SUCCESS(0) : 성공
		BUDDY_ERROR_INVALID_NAME_LENGTH(701) : 이름이 빈 문자열
		BUDDY_ERROR_REQUEST_MY_SELF(702) : 자신을 초대 요청
		BUDDY_ERROR_NOT_FOUND_USER(703) : 유저를 찾을 수 없음
		BUDDY_ERROR_DB_LOAD_FAIL(704) : 유저 정보 로딩 실패
		BUDDY_ERROR_DB_OPERATION_ERROR(706) : 디비 에러
		BUDDY_ERROR_ALREADY_BUDDY(708) : 이미 등록 된 친구
		BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY(709) : 수신 자의 친구 수 초과
		BUDDY_ERROR_ALREADY_INVITATION_SENT(711) : 이미 친구 요청 상태
		BUDDY_ERROR_ALREADY_INVITATION_RECV(712) : 이미 내가 요청을 받은 대상
		BUDDY_ERROR_SEND_INVITATION_FULL(713) : 송신 자의 친구 수 초과
	*/

	PKT_GC_ACK_BUDDY_INVITE_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_INVITE_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIMsgParamBuddyError param = FGsUIMsgParamBuddyError(static_cast<PD::Result>(result));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BUDDY_PACKET_RESULT_ERROR, &param);

		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyInviteAck(pkt->UserDBId(), pkt->UserName());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_INVITE- ID: %llu, NAME : %s"), pkt->UserDBId(), *pkt->UserName());
#endif
}

// 내가 친구를 삭제 했을 때
void FGsNetMessageHandlerBuddy::BuddyDeleteAck(const FGsNet::Packet &inPacket)
{
	// v1은 사용안함
	return;

	PKT_GC_ACK_BUDDY_DELETE_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_DELETE_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIMsgParamBuddyError param = FGsUIMsgParamBuddyError(static_cast<PD::Result>(result));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BUDDY_PACKET_RESULT_ERROR, &param);

		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyDeletAck(pkt->UserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_DELETE- ID: %llu"), pkt->UserDBId());
#endif
}

// 상대방이 친구 요청을 했을 때 초대 알림
void FGsNetMessageHandlerBuddy::BuddyInvitedNotify(const FGsNet::Packet & inPacket)
{
	// v1은 사용안함
	return;

	PKT_GC_NOTIFY_BUDDY_INVITED_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_INVITED_READ*>(inPacket.Buffer);
		
	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyInvitedNotify(pkt->UserDBId(), pkt->UserName());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_INVITED- ID: %llu, NAME : %s"), pkt->UserDBId(), *pkt->UserName());
#endif
}

// 친구 요청을 수락 했을 때
void FGsNetMessageHandlerBuddy::BuddyAcceptAck(const FGsNet::Packet & inPacket)
{
	// v1은 사용안함
	return;

	/*
	result:
		PACKET_RESULT_SUCCESS(0) : 성공
		BUDDY_ERROR_REQUEST_MY_SELF(702) : 자신을 수락 요청
		BUDDY_ERROR_NOT_FOUND_USER(703) : 유저 조회 실패
		BUDDY_ERROR_INVALID_USER_ID(705) : UserDBId가 0
		BUDDY_ERROR_DB_OPERATION_ERROR(706) : 디비 에러
		BUDDY_ERROR_INVALID_INVITATION(707) : 유효하지 않은 요청에 대한 친구 수락 시도
		BUDDY_ERROR_ALREADY_BUDDY(708) : 이미 친구인 대상에 대한 친구 수락 시도
		BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY(709) : 송신 자의 친구 수가 초과
		BUDDY_ERROR_REACHED_MAX_BUDDY(714) : 수신자의 친구 수가 초과되어 실패
	*/

	PKT_GC_ACK_BUDDY_ACCEPT_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_ACCEPT_READ*>(inPacket.Buffer);	

	int32_t result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIMsgParamBuddyError param = FGsUIMsgParamBuddyError(static_cast<PD::Result>(result));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BUDDY_PACKET_RESULT_ERROR, &param);

		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	time_t lastLoginTime = pkt->LastLoginTime();
	buddyHandler->RecvBuddyAcceptAck(pkt->UserDBId(), pkt->UserName(), (lastLoginTime == 0) ? true : false, lastLoginTime);
	
#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV ACK_BUDDY_ACCEPT- ID: %llu, NAME : %s"), pkt->UserDBId(), *pkt->UserName());
#endif
}

// 친구가 내 초대에 응했을 때 (맞친구가 됐을때)
void FGsNetMessageHandlerBuddy::BuddyAcceptedNotify(const FGsNet::Packet & inPacket)
{
	// v1은 사용안함
	return;

	PKT_GC_NOTIFY_BUDDY_ACCEPTED_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_ACCEPTED_READ*>(inPacket.Buffer);
	
	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	time_t lastLoginTime = pkt->LastLoginTime();
	buddyHandler->RecvBuddyAddNotify(pkt->UserDBId(), pkt->UserName(), (lastLoginTime == 0) ? true : false, lastLoginTime);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_ACCEPTED- ID: %llu, NAME : %s"), pkt->UserDBId(), *pkt->UserName());
#endif
}	

// 친구 요청을 거절 했을 때, 내가 친구 요청을 철회 했을 때
void FGsNetMessageHandlerBuddy::BuddyDeleteInvitationdAck(const FGsNet::Packet & inPacket)
{
	// v1은 사용안함
	return;

	PKT_GC_ACK_BUDDY_DELETE_INVITATION_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_DELETE_INVITATION_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIMsgParamBuddyError param = FGsUIMsgParamBuddyError(static_cast<PD::Result>(result));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BUDDY_PACKET_RESULT_ERROR, &param);

		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyDeleteInvitationdAck(pkt->ReceiverUserDBId(), pkt->SenderUserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_ACCEPTED- ReceiverUserDBId: %llu, SenderUserDBId : %llu"), 
		pkt->ReceiverUserDBId(), pkt->SenderUserDBId());
#endif
}

// 친구가 로그인 했을 때
void FGsNetMessageHandlerBuddy::BuddyLoginNotify(const FGsNet::Packet &inPacket)
{
	PKT_GC_NOTIFY_BUDDY_LOGIN_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_LOGIN_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyLoginNotify(pkt->UserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_LOGIN- ID: %llu"), pkt->UserDBId());
#endif
}

// 친구가 로그아웃 했을 때
void FGsNetMessageHandlerBuddy::BuddyLogoutNotify(const FGsNet::Packet &inPacket)
{
	PKT_GC_NOTIFY_BUDDY_LOGOUT_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_LOGOUT_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyLogoutNotify(pkt->UserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_LOGOUT- ID: %llu"), pkt->UserDBId());
#endif
}

// 상대가 초대 거절 또는 초대 취소 알림
void FGsNetMessageHandlerBuddy::BuddyInvitationDeletedNotify(const FGsNet::Packet &inPacket)
{
	// v1은 사용안함
	return;

	PKT_GC_NOTIFY_BUDDY_INVITATION_DELETED_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_INVITATION_DELETED_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyInvitationDeletedNotify(pkt->ReceiverUserDBId(), pkt->SenderUserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_ACCEPTED- ReceiverUserDBId: %llu, SenderUserDBId : %llu"),
		pkt->ReceiverUserDBId(), pkt->SenderUserDBId());
#endif
}

// 친구가 나를 친구 삭제 했을 때 
void FGsNetMessageHandlerBuddy::BuddyDeletedNotify(const FGsNet::Packet &inPacket)
{
	// v1은 사용안함
	return;

	PKT_GC_NOTIFY_BUDDY_DELETED_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_DELETED_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyDeletedNotify(pkt->UserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_DELETED- ID: %llu"), pkt->UserDBId());
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 현재 친구 리스트
void FGsNetMessageHandlerBuddy::BuddListAck_v2(const FGsNet::Packet& inPacket)
{
	// 친구가 없다면 빈배열
	// 클라이언트가 요청하지 않아도 최초 월드 진입 시 패킷을 1회 내려 줌.
	PKT_GC_ACK_BUDDY_LIST_V2_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_LIST_V2_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
	{
		GGameData()->Set(*pkt);
		return;
	}

	buddyHandler->RecvBuddListAck_v2(*pkt);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_LIST"));
#endif
}

// 친구 초대 리스트
void FGsNetMessageHandlerBuddy::BuddyRecvInvitationListAck_v2(const FGsNet::Packet& inPacket)
{
	// 초대가 없다면 빈배열
	// 클라이언트가 요청하지 않아도 최초 월드 진입 시 패킷을 1회 내려 줌.

	PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_V2_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_V2_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
	{
		GGameData()->Set(*pkt);
		return;
	}

	buddyHandler->RecvBuddyRecvInvitationListAck_v2(*pkt);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_RECV_INVITATION_LIST"));
#endif
}

// 친구 요청 리스트
void FGsNetMessageHandlerBuddy::BuddySendInvitationListAck_v2(const FGsNet::Packet& inPacket)
{
	// 요청이 없다면 빈배열
	// 클라이언트가 요청하지 않아도 최초 월드 진입 시 패킷을 1회 내려 줌

	PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_V2_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_V2_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
	{
		GGameData()->Set(*pkt);
		return;
	}

	buddyHandler->RecvBuddySendInvitationListAck_v2(*pkt);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_SEND_INVITATION_LIST"));
#endif
}

void FGsNetMessageHandlerBuddy::BuddyInviteAck_v2(const FGsNet::Packet& inPacket)
{
	/*
	result:
		PACKET_RESULT_SUCCESS(0) : 성공
		BUDDY_ERROR_INVALID_NAME_LENGTH(701) : 이름이 빈 문자열
		BUDDY_ERROR_REQUEST_MY_SELF(702) : 자신을 초대 요청
		BUDDY_ERROR_NOT_FOUND_USER(703) : 유저를 찾을 수 없음
		BUDDY_ERROR_DB_LOAD_FAIL(704) : 유저 정보 로딩 실패
		BUDDY_ERROR_DB_OPERATION_ERROR(706) : 디비 에러
		BUDDY_ERROR_ALREADY_BUDDY(708) : 이미 등록 된 친구
		BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY(709) : 수신 자의 친구 수 초과
		BUDDY_ERROR_ALREADY_INVITATION_SENT(711) : 이미 친구 요청 상태
		BUDDY_ERROR_ALREADY_INVITATION_RECV(712) : 이미 내가 요청을 받은 대상
		BUDDY_ERROR_SEND_INVITATION_FULL(713) : 송신 자의 친구 수 초과
	*/

	PKT_GC_ACK_BUDDY_INVITE_V2_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_INVITE_V2_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIMsgParamBuddyError param = FGsUIMsgParamBuddyError(static_cast<PD::Result>(result));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BUDDY_PACKET_RESULT_ERROR, &param);

		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyInviteAck(pkt->UserDBId(), pkt->UserName(), pkt->WorldId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_INVITE- ID: %llu, NAME : %s, WorldId : %d"), pkt->UserDBId(), *pkt->UserName(), pkt->WorldId());
#endif
}

// 내가 친구를 삭제 했을 때
void FGsNetMessageHandlerBuddy::BuddyDeleteAck_v2(const FGsNet::Packet& inPacket)
{
	PKT_GC_ACK_BUDDY_DELETE_V2_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_DELETE_V2_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIMsgParamBuddyError param = FGsUIMsgParamBuddyError(static_cast<PD::Result>(result));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BUDDY_PACKET_RESULT_ERROR, &param);

		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyDeletAck(pkt->UserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV BUDDY_DELETE- ID: %llu"), pkt->UserDBId());
#endif
}

// 상대방이 친구 요청을 했을 때 초대 알림
void FGsNetMessageHandlerBuddy::BuddyInvitedNotify_v2(const FGsNet::Packet& inPacket)
{
	PKT_GC_NOTIFY_BUDDY_INVITED_V2_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_INVITED_V2_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyInvitedNotify(pkt->UserDBId(), pkt->UserName(), pkt->WorldId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_INVITED- ID: %llu, NAME : %s, WorldId : %d"), pkt->UserDBId(), *pkt->UserName(), pkt->WorldId());
#endif
}

// 친구 요청을 수락 했을 때
void FGsNetMessageHandlerBuddy::BuddyAcceptAck_v2(const FGsNet::Packet& inPacket)
{
	/*
	result:
		PACKET_RESULT_SUCCESS(0) : 성공
		BUDDY_ERROR_REQUEST_MY_SELF(702) : 자신을 수락 요청
		BUDDY_ERROR_NOT_FOUND_USER(703) : 유저 조회 실패
		BUDDY_ERROR_INVALID_USER_ID(705) : UserDBId가 0
		BUDDY_ERROR_DB_OPERATION_ERROR(706) : 디비 에러
		BUDDY_ERROR_INVALID_INVITATION(707) : 유효하지 않은 요청에 대한 친구 수락 시도
		BUDDY_ERROR_ALREADY_BUDDY(708) : 이미 친구인 대상에 대한 친구 수락 시도
		BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY(709) : 송신 자의 친구 수가 초과
		BUDDY_ERROR_REACHED_MAX_BUDDY(714) : 수신자의 친구 수가 초과되어 실패
	*/

	PKT_GC_ACK_BUDDY_ACCEPT_V2_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_ACCEPT_V2_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIMsgParamBuddyError param = FGsUIMsgParamBuddyError(static_cast<PD::Result>(result));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BUDDY_PACKET_RESULT_ERROR, &param);

		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	time_t lastLoginTime = pkt->LastLoginTime();
	buddyHandler->RecvBuddyAcceptAck(pkt->UserDBId(), pkt->UserName(), lastLoginTime, pkt->WorldId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV ACK_BUDDY_ACCEPT- ID: %llu, NAME : %s, WorldId : %d"), pkt->UserDBId(), *pkt->UserName(), pkt->WorldId());
#endif
}

// 친구가 내 초대에 응했을 때 (맞친구가 됐을때)
void FGsNetMessageHandlerBuddy::BuddyAcceptedNotify_v2(const FGsNet::Packet& inPacket)
{
	PKT_GC_NOTIFY_BUDDY_ACCEPTED_V2_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_ACCEPTED_V2_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	time_t lastLoginTime = pkt->LastLoginTime();
	buddyHandler->RecvBuddyAddNotify(pkt->UserDBId(), pkt->UserName(), lastLoginTime, pkt->WorldId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_ACCEPTED- ID: %llu, NAME : %s, WorldId : %d"), pkt->UserDBId(), *pkt->UserName(), pkt->WorldId());
#endif
}

// 친구 요청을 거절 했을 때, 내가 친구 요청을 철회 했을 때
void FGsNetMessageHandlerBuddy::BuddyDeleteInvitationdAck_v2(const FGsNet::Packet& inPacket)
{
	PKT_GC_ACK_BUDDY_DELETE_INVITATION_V2_READ* pkt = reinterpret_cast<PKT_GC_ACK_BUDDY_DELETE_INVITATION_V2_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIMsgParamBuddyError param = FGsUIMsgParamBuddyError(static_cast<PD::Result>(result));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BUDDY_PACKET_RESULT_ERROR, &param);

		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyDeleteInvitationdAck(pkt->ReceiverUserDBId(), pkt->SenderUserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_ACCEPTED- ReceiverUserDBId: %llu, SenderUserDBId : %llu"),
		pkt->ReceiverUserDBId(), pkt->SenderUserDBId());
#endif
}

// 상대가 초대 거절 또는 초대 취소 알림
void FGsNetMessageHandlerBuddy::BuddyInvitationDeletedNotify_v2(const FGsNet::Packet& inPacket)
{
	PKT_GC_NOTIFY_BUDDY_INVITATION_DELETED_V2_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_INVITATION_DELETED_V2_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyInvitationDeletedNotify(pkt->ReceiverUserDBId(), pkt->SenderUserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_ACCEPTED- ReceiverUserDBId: %llu, SenderUserDBId : %llu"),
		pkt->ReceiverUserDBId(), pkt->SenderUserDBId());
#endif
}

// 친구가 나를 친구 삭제 했을 때 
void FGsNetMessageHandlerBuddy::BuddyDeletedNotify_v2(const FGsNet::Packet& inPacket)
{
	PKT_GC_NOTIFY_BUDDY_DELETED_V2_READ* pkt = reinterpret_cast<PKT_GC_NOTIFY_BUDDY_DELETED_V2_READ*>(inPacket.Buffer);

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyDeletedNotify(pkt->UserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_DELETED- ID: %llu"), pkt->UserDBId());
#endif
}