#include "GsNetMessageHandlerCommunity.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageSystem.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsCommunityEventManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"


void FGsNetMessageHandlerCommunity::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& protocolWorld = inManager->GetProtocalWorld();

	InitializeMessageDelegateList(&protocolWorld);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_COMMUNITY_EVENT_LIST,
		this, &FGsNetMessageHandlerCommunity::CommunityListAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_CHECK_ATTENDANCE,
		this, &FGsNetMessageHandlerCommunity::ChackAttendanceAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PURCHASE_ATTENDANCE,
		this, &FGsNetMessageHandlerCommunity::PurchaseAttendanceAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_COMMUNITY_EVENT_MISSION,
		this, &FGsNetMessageHandlerCommunity::CommunityEventMission)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_COMMUNITY_EVENT_MISSION_REWARD,
		this, &FGsNetMessageHandlerCommunity::CommunityEventMissionReward)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INFORM_COMMUNITY_EVENT_MISSION_PROGRESS,
		this, &FGsNetMessageHandlerCommunity::CommunityEventMissionProgress)
	);
}

void FGsNetMessageHandlerCommunity::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

// 커뮤니티 이벤트 리스트 요청 응답
void FGsNetMessageHandlerCommunity::CommunityListAck(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[Attendance] RECV SC_ACK_COMMUNITY_EVENT_LIST"));
#endif
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_COMMUNITY_EVENT_LIST_READ*>(inPacket.Buffer))
	{
		GSCommunityEvent()->Set(*pkt);
	}

	FGsUIHelper::HideBlockUI();
}

// 출석 체크 결과
void FGsNetMessageHandlerCommunity::ChackAttendanceAck(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[Attendance] RECV SC_ACK_CHECK_ATTENDANCE"));
#endif

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_CHECK_ATTENDANCE_READ*>(inPacket.Buffer))
	{
		GSCommunityEvent()->Set(*pkt);
	}

	FGsUIHelper::HideBlockUI();
}

// 출석부 구매 요청 응답
void FGsNetMessageHandlerCommunity::PurchaseAttendanceAck(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[Attendance] RECV SC_ACK_PURCHASE_ATTENDANCE"));
#endif

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PURCHASE_ATTENDANCE_READ*>(inPacket.Buffer))
	{
		GSCommunityEvent()->Set(*pkt);
	}

	FGsUIHelper::HideBlockUI();
}

// 커뮤니티 이벤트 미션 목록
void FGsNetMessageHandlerCommunity::CommunityEventMission(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[CommunityEventMission] RECV SC_COMMUNITY_EVENT_MISSION"));
#endif

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_COMMUNITY_EVENT_MISSION_READ*>(inPacket.Buffer))
	{
		GSCommunityEvent()->Set(*pkt);
	}
}

// 커뮤니티 이벤트 미션 보상 요청 응답
void FGsNetMessageHandlerCommunity::CommunityEventMissionReward(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[CommunityEventMission] RECV SC_ACK_COMMUNITY_EVENT_MISSION_REWARD"));
#endif

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_COMMUNITY_EVENT_MISSION_REWARD_READ*>(inPacket.Buffer))
	{
		GSCommunityEvent()->Set(*pkt);
	}

	FGsUIHelper::HideBlockUI();
}

// 커뮤니티 이벤트 미션 임무 진행도
void FGsNetMessageHandlerCommunity::CommunityEventMissionProgress(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[CommunityEventMission] RECV SC_INFORM_COMMUNITY_EVENT_MISSION_PROGRESS"));
#endif

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_INFORM_COMMUNITY_EVENT_MISSION_PROGRESS_READ*>(inPacket.Buffer))
	{
		GSCommunityEvent()->Set(*pkt);
	}
}