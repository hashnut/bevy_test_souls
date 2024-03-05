// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerBoss.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsEventManager.h"

#include "Message/GsMessageNet.h"
#include "Message/MessageParam/GsBossMessageParam.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "T1Project.h"



FGsNetMessageHandlerBoss::~FGsNetMessageHandlerBoss()
{

}

void FGsNetMessageHandlerBoss::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& protocolWorld = inManager->GetProtocalWorld();
	InitializeMessageDelegateList(&protocolWorld);

	//field boss
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_RESULT_DEFEAT_FILEDBOSS,
		this, &FGsNetMessageHandlerBoss::ResultDefeatFieldBoss));

	//world boss field
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_WORLD_BOSS_INFO,
		this, &FGsNetMessageHandlerBoss::AckWorldBossInfo));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INFORM_WORLD_BOSS_MAP_CREATE,
		this, &FGsNetMessageHandlerBoss::InformWorldBossMapCreate));

	//world boss dungoen
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_WORLD_BOSS_SPAWN_READY_STATE,
		this, &FGsNetMessageHandlerBoss::WorldBossSpawnReadyState));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ALARM_WORLD_BOSS_SPAWN,
		this, &FGsNetMessageHandlerBoss::AlarmWorldBossSpawn));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_WORLD_BOSS_CLEAR,
		this, &FGsNetMessageHandlerBoss::WorldBossClear));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ALARM_WORLD_BOSS_TIME_OVER,
		this, &FGsNetMessageHandlerBoss::AlarmWorldBossTimeOver));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ALARM_FIELD_BOSS_SPAWN,
		this, &FGsNetMessageHandlerBoss::AlarmFieldBossSpawn));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ALARM_FIELD_BOSS_DIE,
		this, &FGsNetMessageHandlerBoss::AlarmFieldBossDie));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ALARM_FIELD_BOSS_DESPAWN,
		this, &FGsNetMessageHandlerBoss::AlarmFieldBossDespawn));
}

void FGsNetMessageHandlerBoss::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerBoss::ResultDefeatFieldBoss(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsResultDefeatFieldBossMsgParam param = FGsResultDefeatFieldBossMsgParam(pkt);
	GMessage()->GetBoss().SendMessage(MessageBoss::OPEN_BOSS_CONTRIBUTION_POPUP, &param);
}

void FGsNetMessageHandlerBoss::AckWorldBossInfo(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_WORLD_BOSS_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_WORLD_BOSS_INFO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsBossHandler* bossHandler = FGsBossHandler::GetInstance())
	{
		if (FGsWorldBossHandler* worldBossHandler = bossHandler->GetWorldBossHandler())
		{
			worldBossHandler->WorldBossInfo(pkt);
		}
	}
}

void FGsNetMessageHandlerBoss::WorldBossSpawnReadyState(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_WORLD_BOSS_SPAWN_READY_STATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_WORLD_BOSS_SPAWN_READY_STATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsBossHandler* bossHandler = FGsBossHandler::GetInstance())
	{
		if (FGsWorldBossHandler* worldBossHandler = bossHandler->GetWorldBossHandler())
		{
			worldBossHandler->WorldBossSpawnReady(pkt);
		}
	}
}

void FGsNetMessageHandlerBoss::InformWorldBossMapCreate(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INFORM_WORLD_BOSS_MAP_CREATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INFORM_WORLD_BOSS_MAP_CREATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsBossHandler* bossHandler = FGsBossHandler::GetInstance())
	{
		if (FGsWorldBossHandler* worldBossHandler = bossHandler->GetWorldBossHandler())
		{
#if WORLD_BOSS_DEBUG
			GSLOG(Log, TEXT("InformWorldBossMapCreate - boss id : %lld\tboss spawn time : %s")
				, (int64)pkt->BossId()
				, *FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(pkt->ExpireTime()).ToString());
#endif

			worldBossHandler->InformWorldMapCreate(pkt);
		}
	}
}

void FGsNetMessageHandlerBoss::AlarmWorldBossSpawn(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ALARM_WORLD_BOSS_SPAWN_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ALARM_WORLD_BOSS_SPAWN_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsBossHandler* bossHandler = FGsBossHandler::GetInstance())
	{
		if (FGsWorldBossHandler* worldBossHandler = bossHandler->GetWorldBossHandler())
		{
			worldBossHandler->AlarmWorldBossSpawn(pkt);
		}
	}
}

void FGsNetMessageHandlerBoss::WorldBossClear(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsBossHandler* bossHandler = FGsBossHandler::GetInstance())
	{
		if (FGsWorldBossHandler* worldBossHandler = bossHandler->GetWorldBossHandler())
		{
			worldBossHandler->WorldBossClear(pkt);
		}
	}
}

void FGsNetMessageHandlerBoss::AlarmWorldBossTimeOver(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ALARM_WORLD_BOSS_TIME_OVER_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ALARM_WORLD_BOSS_TIME_OVER_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsBossHandler* bossHandler = FGsBossHandler::GetInstance())
	{
		if (FGsWorldBossHandler* worldBossHandler = bossHandler->GetWorldBossHandler())
		{
			worldBossHandler->AlarmWorldBossTimeOver(pkt);
		}
	}
}

// 2023/8/1 PKT - 필드 보스 스폰
void FGsNetMessageHandlerBoss::AlarmFieldBossSpawn(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ALARM_FIELD_BOSS_SPAWN_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ALARM_FIELD_BOSS_SPAWN_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	if (INVALID_EVENT_SUB_DIALOG_ID < pkt->EventId())
	{
		GSEvent()->EventActionPlayImmediate(pkt->EventId());
	}
}

// 2023/8/1 PKT - 필드 보스 다이
void FGsNetMessageHandlerBoss::AlarmFieldBossDie(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ALARM_FIELD_BOSS_DIE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ALARM_FIELD_BOSS_DIE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (INVALID_EVENT_SUB_DIALOG_ID < pkt->EventId())
	{
		GSEvent()->EventActionPlayImmediate(pkt->EventId());
	}
}

// 필드 보스 디스폰
// https://jira.com2us.com/jira/browse/CHR-23655
void FGsNetMessageHandlerBoss::AlarmFieldBossDespawn(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ALARM_FIELD_BOSS_DESPAWN_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ALARM_FIELD_BOSS_DESPAWN_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsBossHandler* bossHandler = FGsBossHandler::GetInstance();
	if (bossHandler)
	{
		FGsFieldBossHandler* handler = bossHandler->GetFieldBossHandler();
		if (handler)
		{
			handler->FieldBossDespawn(pkt->BossId());
		}
	}
}