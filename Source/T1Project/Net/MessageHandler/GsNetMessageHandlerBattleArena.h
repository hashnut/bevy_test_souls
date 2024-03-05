// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Message/GsMessageNet.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;
/**
 *
 */
class T1PROJECT_API FGsNetMessageHandlerBattleArena : public IGsNetMessageHandler, FGsNetMessageWorld
{
protected:
	TArray<TPair<PD::GC::GCPacketId, FDelegateHandle>> _gateWayPackets;

public:
	FGsNetMessageHandlerBattleArena() = default;
	virtual ~FGsNetMessageHandlerBattleArena() = default;

protected:
	class FGsBattleArenaGuildWarHandler* GetGuildWarHandler() const;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

public:
	void Nofity_StartAlarm(const FGsNet::Packet& InPacket);
	void Nofity_BattleArenaEnter(const FGsNet::Packet& InPacket);
	void Nofity_BattleArenaLeave(const FGsNet::Packet& InPacket);
	// 2023/11/23 PKT - Arena 상태 정보 갱신
	void Notify_GuildWarInfo(const FGsNet::Packet& InPacket);
	
	// 2023/11/23 PKT - 이 패킷은 잘 이해가 안됨. Arena가 신청 참여 등등의 진행 중일때 로그인 하면 온다는데 Data가 리스트임. 클라에서는 처리 할 방법이 없음.
	void Notify_GuildWarInfoList(const FGsNet::Packet& InPacket);

	// 2023/11/23 PKT - Arena 신청 상태 갱신
	void Ack_RegisterUpdate(const FGsNet::Packet& InPacket);

	void Ack_MatchFailed(const FGsNet::Packet& InPacket);
	void Ack_EnterFailed(const FGsNet::Packet& InPacket);

	// 2023/11/23 PKT - HUD - 팀 점수 정보
	void Notify_GuildWarScoreInfo(const FGsNet::Packet& InPacket);
	// 2023/11/23 PKT - 점수 디테일 정보
	void Ack_GuildWarScoreDetailInfo(const FGsNet::Packet& InPacket);
	
	// 2023/11/28 PKT - 게임 종료 결과
	void Notify_GuildWarEndPlayResult(const FGsNet::Packet& InPacket);

	// 2023/12/27 PKT - 팀원들의 위치 정보
	void Notify_TeamPlayerPositionInfo(const FGsNet::Packet& InPacket);



	/**
	 * Battle Arena Rank
	 */
	void AckBattleArenaLocalGuildRankInfo(const FGsNet::Packet& InPacket);	
	void AckBattleArenaGuildRankInfo(const FGsNet::Packet& InPacket);
};
