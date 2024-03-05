#include "GsNetMessageHandlerParty.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Party/GsPartyHandler.h"

#include "Party/GsPartyFunc.h"

void FGsNetMessageHandlerParty::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalGateway& protocolGateWay = inManager->GetProtocalGateway();

	InitializeMessageDelegateList(&protocolGateWay);

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_ACK_PARTY_CREATE,
		this, &FGsNetMessageHandlerParty::PartyCreate));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_ACK_PARTY_INVITE,
		this, &FGsNetMessageHandlerParty::PartyInvite));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_INVITED,
		this, &FGsNetMessageHandlerParty::PartyInvited));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_JOINED,
		this, &FGsNetMessageHandlerParty::PartyJoined));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_MEMBER_JOINED,
		this, &FGsNetMessageHandlerParty::PartyMemberJoined));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_MEMBER_UPDATED,
		this, &FGsNetMessageHandlerParty::PartyMemberUpdate));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_ACK_PARTY_REFUSE_INVITATION,
		this, &FGsNetMessageHandlerParty::PartyRefuseInvitation));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_INVITE_FAILED,
		this, &FGsNetMessageHandlerParty::PartyInviteFailed));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_ACK_PARTY_LEAVE,
		this, &FGsNetMessageHandlerParty::PartyLeave));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_MEMBER_LEFT,
		this, &FGsNetMessageHandlerParty::PartyMemberLeft));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_ACK_PARTY_KICK,
		this, &FGsNetMessageHandlerParty::PartyKick));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_KICKED,
		this, &FGsNetMessageHandlerParty::PartyKicked));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_MEMBER_KICKED,
		this, &FGsNetMessageHandlerParty::PartyMemeberKicked));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_ACK_PARTY_CHANGE_LEADER,
		this, &FGsNetMessageHandlerParty::PartyChangeLeader));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_LEADER_CHANGED,
		this, &FGsNetMessageHandlerParty::PartyLeaderChanged));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_ACK_CHANGE_DROP_OWNERSHIP_TYPE,
		this, &FGsNetMessageHandlerParty::PartyChangeDropOwnershipType));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_PARTY_DROP_OWNERSHIP_TYPE_CHANGED,
		this, &FGsNetMessageHandlerParty::PartyDropOwnershipTypeChanged));



	/**
	 * 2022/06/13 PKT - 파티 단위의 컨텐츠 중 준비 단계가 있는 컨텐츠에 한해 해당 패킷이 옴
	 */
	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_NOTIFY_COOP_PARTY_READY_STATUS,
		this, &FGsNetMessageHandlerParty::PartyMemberUpdateReadyState));

	AddMessageDelegate(protocolGateWay.AddRaw(PD::GC::GCPacketId::GC_ACK_COOP_PARTY_READY,
		this, &FGsNetMessageHandlerParty::PartyMyUpdateReadyState));
}

void FGsNetMessageHandlerParty::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}
void FGsNetMessageHandlerParty::PartyCreate(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_PARTY_CREATE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_PARTY_CREATE_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		if (PD::Result::PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY_MEMBER == result || PD::Result::PARTY_DUNGEON_ERROR_CANNOT_INTER_SERVER == result )
		{
			FGsUIHelper::TraySectionMessageTickerNetError(result);
		}
		else
		{
			FGsUIHelper::PopupNetError(result);
		}		
		return;
	}


	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}


	// 아이템 분배 타입 저장
	partyHandler->RecvPartyCreate(pkt->DropOwnershipType());
	GSChat()->OnCheckPartyChat(true);
}

// 초대 보낸 사람이 받는 패킷
void FGsNetMessageHandlerParty::PartyInvite(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_PARTY_INVITE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_PARTY_INVITE_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (false == FGsPartyFunc::ShowTickerPartyInviteError(result))
		{
			if (PD::Result::PARTY_DUNGEON_ERROR_PLAYING == result
				|| PD::Result::PARTY_ERROR_MEMBER_FULL == result
				|| PD::Result::PARTY_ERROR_INVALID_PARTY == result
				|| PD::Result::PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY_MEMBER == result
				|| PD::Result::PARTY_DUNGEON_ERROR_CANNOT_INTER_SERVER == result
				|| PD::Result::PARTY_ERROR_NOT_INVITABLE_OTHER_SERVER_USER == result)
			{	// 2023/3/15 PKT - 파티 던전 에러는 티커로 변경
				FGsUIHelper::TraySectionMessageTickerNetError(result);
				return;
			}
			FGsUIHelper::PopupNetError(result);
		}
		return;
	}
	// 초대 대기 대상 등록
	// 레벨, 채널, 무기 정보 추가 필요함
	GSParty()->RecvPartyInvite(pkt->SlotIndex(), pkt->InviteeName(), pkt->InviteeHomeWorldId());
}

// 초대 받는 사람이 받는 패킷
void FGsNetMessageHandlerParty::PartyInvited(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_INVITED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_INVITED_READ*>(inPacket.Buffer);
	// 초대한 사람 정보 저장
	// 레벨, 채널 정보 추가 필요함
	GSParty()->RecvPartyInvited(pkt->InviterName(), pkt->InviterLevel(), pkt->InviterChannelId(), pkt->InviterHomeWorldId());
}

// 초대 수락(내가 수락해서 들어감)
void FGsNetMessageHandlerParty::PartyJoined(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_JOINED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_JOINED_READ*>(inPacket.Buffer);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}

	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::PARTY_DUNGEON_ERROR_PLAYING == result
			|| PD::Result::PARTY_ERROR_SAME_PARTY == result
			|| PD::Result::PARTY_ERROR_INVALID_PARTY == result
			|| PD::Result::PARTY_ERROR_MEMBER_FULL == result
			)
		{	// https://jira.com2us.com/jira/browse/C2URWQ-3219
			// 1. a 파티 초대
			// 2. a 파티 나감
			// 3. b 파티 수락
			// 4. 이때 실패 (존재하지 않는 파티)일때 초대자 정보 클리어
			if (PD::Result::PARTY_ERROR_SAME_PARTY != result)
			{
				// 2023/5/6 PKT - https://jira.com2us.com/jira/browse/C2URWQ-4486
				// [시스템] [던전] 특정 경로로 파티 던전 시작 시 파티 UI가 사라집니다.
				partyHandler->RecvClearInviterInfo();				
			}			

			// 2023/3/15 PKT - 파티 던전 에러는 티커로 변경
			FGsUIHelper::TraySectionMessageTickerNetError(result);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	TMap<int64, bool> mapSpawnedInfo;
	for (PD::GC::PKT_GC_PARTY_JOINED_READ::PartyMemberInfoListIterator iter =
		pkt->GetFirstPartyMemberInfoListIterator();
		iter != pkt->GetLastPartyMemberInfoListIterator();
		++iter)
	{
		int64 gameId = iter->GameId();
		UGsGameObjectBase* findObj =
			GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, gameId);

		mapSpawnedInfo.Add(gameId, (findObj != nullptr) ? true : false);
	}

	partyHandler->RecvPartyJoined(*pkt, mapSpawnedInfo);
	GSChat()->OnCheckPartyChat(true);
}

// 멤버 추가(초대 수락)
void FGsNetMessageHandlerParty::PartyMemberJoined(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_MEMBER_JOINED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_MEMBER_JOINED_READ*>(inPacket.Buffer);

	int64 gameId = pkt->GameId();
	UGsGameObjectBase* findObj =
		GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, gameId);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}
	// 파티원이 스폰 되어있는지 체크
	partyHandler->RecvPartyMemberJoined(*pkt, (findObj != nullptr) ? true : false);
}

// 멤버 갱신
void FGsNetMessageHandlerParty::PartyMemberUpdate(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_MEMBER_UPDATED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_MEMBER_UPDATED_READ*>(inPacket.Buffer);


	int64 gameId = pkt->GameId();

	if (nullptr == GSGameObject())
	{
		return;
	}

	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localBase == nullptr)
	{
		return;
	}
	// 로컬은 서버정보가 필요 없다(이미 다 알고 있다)
	if (localBase->GetGameId() == gameId)
	{
		return;
	}

	// 나자신이면 무시
	UGsGameObjectBase* findObj =
		GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, gameId);


	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}

	// 파티원이 스폰 되어있는지 체크
	partyHandler->RecvPartyMemberUpdate(*pkt, (findObj != nullptr) ? true : false);
}

// 초대 거절
void FGsNetMessageHandlerParty::PartyRefuseInvitation(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_PARTY_REFUSE_INVITATION_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_PARTY_REFUSE_INVITATION_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
	GSParty()->RecvPartyRefuseInvitation();
}

// 초대 실패
void FGsNetMessageHandlerParty::PartyInviteFailed(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_INVITE_FAILED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_INVITE_FAILED_READ*>(inPacket.Buffer);
	GSParty()->RecvPartyInviteFailed(pkt->SlotIndex());
}

// 파티 나감(내가)
void FGsNetMessageHandlerParty::PartyLeave(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_PARTY_LEAVE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_PARTY_LEAVE_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}
	partyHandler->RecvPartyLeave();
	GSChat()->OnCheckPartyChat(false);
}

// 파티원 나감
void FGsNetMessageHandlerParty::PartyMemberLeft(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_MEMBER_LEFT_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_MEMBER_LEFT_READ*>(inPacket.Buffer);


	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}

	partyHandler->RecvPartyMemberLeft(pkt->LeftSlotIndex(), pkt->LeaderSlotIndex());
}
// 추방시킴 응답
void FGsNetMessageHandlerParty::PartyKick(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_PARTY_KICK_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_PARTY_KICK_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}

	partyHandler->RecvPartyKick(pkt->SlotIndex());
}

// 추방 당함
void FGsNetMessageHandlerParty::PartyKicked(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_KICKED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_KICKED_READ*>(inPacket.Buffer);
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}
	partyHandler->RecvPartyKicked();
	GSChat()->OnCheckPartyChat(false);
}
// 멤버중 하나 추방 당함
void FGsNetMessageHandlerParty::PartyMemeberKicked(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_MEMBER_KICKED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_MEMBER_KICKED_READ*>(inPacket.Buffer);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}

	partyHandler->RecvPartyKick(pkt->KickedSlotIndex());
}

// 리더 변경 응답(내가 요청)
void FGsNetMessageHandlerParty::PartyChangeLeader(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_PARTY_CHANGE_LEADER_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_PARTY_CHANGE_LEADER_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if(false == FGsPartyFunc::ShowTickerPartyChangeLeader(result))
		{ 
			FGsUIHelper::PopupNetError(result);
		}
		return;
	}

	if (GSParty())
	{
		GSParty()->RecvChangeLeader(pkt->SlotIndex());
	}
}

// 리더 변경
void FGsNetMessageHandlerParty::PartyLeaderChanged(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_LEADER_CHANGED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_LEADER_CHANGED_READ*>(inPacket.Buffer);
	
	if (GSParty())
	{
		GSParty()->RecvChangeLeader(pkt->LeaderSlotIndex());
	}
}

// 전리품 분배룰 변경 응답
void FGsNetMessageHandlerParty::PartyChangeDropOwnershipType(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_CHANGE_DROP_OWNERSHIP_TYPE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CHANGE_DROP_OWNERSHIP_TYPE_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
	GSParty()->RecvChangeDropOwnershipType(pkt->DropOwnershipType());
}

// 전리품 분배룰 변경됨
void FGsNetMessageHandlerParty::PartyDropOwnershipTypeChanged(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_PARTY_DROP_OWNERSHIP_TYPE_CHANGED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PARTY_DROP_OWNERSHIP_TYPE_CHANGED_READ*>(inPacket.Buffer);
	GSParty()->RecvChangeDropOwnershipType(pkt->DropOwnershipType());
}

// 2022/06/13 PKT - 파티원의 준비 상태 변경
void FGsNetMessageHandlerParty::PartyMemberUpdateReadyState(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_NOTIFY_COOP_PARTY_READY_STATUS_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_NOTIFY_COOP_PARTY_READY_STATUS_READ*>(inPacket.Buffer);
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}

	partyHandler->RecvPartyMemberReadyState(*pkt);
}

// 2022/06/13 PKT - 자신의 준비 상태 변경
void FGsNetMessageHandlerParty::PartyMyUpdateReadyState(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_COOP_PARTY_READY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_COOP_PARTY_READY_READ*>(inPacket.Buffer);
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}

	partyHandler->RecvPartyReadyState(*pkt);
}

