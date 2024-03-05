// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStageGameBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "Message/GsMessageContentDungeon.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

/**
* file		GsStageGameDungeon.h
* @brief	던전(일반/특수/파티/정예)등에 입장 해 있을때 각 던전내 상태 및 변경 되는 정보에 대한 처리.
* @author	PKT
* @date		2022/10/11
**/
class T1PROJECT_API FGsStageGameDungeon : public FGsStageGameBase
{
public:
	FGsStageGameDungeon();
	virtual ~FGsStageGameDungeon();

private:
	TWeakObjectPtr<class UGsUIHUDDungeonTime>	_hudDungeonTime;

	MsgDungeonHandleArray _dungeonMessageHandler;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	// 2022/07/27 PKT - 던전 플레이가 시작 되었음.(공통 - 파티 던전 제외)
	void RecvDungeonEnter(PD::SC::PKT_SC_DUNGEON_ENTER_READ& InPacket);
	// 2022/07/27 PKT - 던전 플레이가 끝났음.(공통 - 파티 던전 제외)
	void RecvDungeonExit(PD::SC::PKT_SC_DUNGEON_LEAVE_READ& InPacket);
	// 2022/07/28 PKT - 던전 플레이 중 변경되어지는 상태나 정보등에 대한 정보
	void RecvDungeonInfo(PD::SC::PKT_SC_DUNGEON_INFO_READ& InPacket);
	// 2022/06/19 PKT - 던전 내 진행 상태(현재는 파티 던전에서만 사용. 추후 다른 던전에서도 사용하게 된다면 패킷 이름을 바꾸든...)
	void PartyDungeonProgressState(PD::SC::PKT_SC_PARTY_DUNGEON_INFO_READ& InPacket);
	// 2022/06/19 PKT - 던전 내 플레이 결과(현재는 파티 던전에서만 사용. 추후 다른 던전에서도 사용하게 된다면 패킷 이름을 바꾸든...)
	void DungeonResultNotify(PD::SC::PKT_SC_PARTY_DUNGEON_END_READ& InPacket);
	// 2022/08/12 PKT - 고레벨 던전 내 프랍 포털 정보
	void EliteDungeonPropPotalInfo(PD::SC::PKT_SC_ELITEDUNGEON_PORTAL_INFO_READ& InPacket);
};
