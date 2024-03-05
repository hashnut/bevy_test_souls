// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameWorldBoss.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsBossMessageParam.h"


FGsStageGameWorldBoss::FGsStageGameWorldBoss() : FGsStageGameBase(FGsStageMode::Game::FIELD)
{
}

FGsStageGameWorldBoss::~FGsStageGameWorldBoss()
{
}

void FGsStageGameWorldBoss::Enter()
{
	FGsStageGameBase::Enter();

	MBoss& msgBoss = GMessage()->GetBoss();
	_msgBossHandleList.Emplace(msgBoss.AddRaw(MessageBoss::WORLD_BOSS_SPAWN_READY, this, &FGsStageGameWorldBoss::OnWorldBossSpawnReadyState));
	//월드 보스 스폰되면 Hide
	_msgBossHandleList.Emplace(msgBoss.AddRaw(MessageBoss::ALARM_WORLD_BOSS_SPAWN, this, &FGsStageGameWorldBoss::OnWorldBossSpawn));
}

void FGsStageGameWorldBoss::Exit()
{
	MBoss& msgBoss = GMessage()->GetBoss();
	for (MsgBossHandle& msgBossHandle : _msgBossHandleList)
	{
		msgBoss.Remove(msgBossHandle);
	}
	_msgBossHandleList.Empty();

	GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::OFF_HUD_TIME, nullptr);

	FGsStageGameBase::Exit();
}

void FGsStageGameWorldBoss::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);
}

void FGsStageGameWorldBoss::OnWorldBossSpawnReadyState(const struct IGsMessageParam* InParam)
{
	const FGsWorldBossRoomReadyParam* param = InParam->Cast<const FGsWorldBossRoomReadyParam>();
	if (nullptr == param)
	{
		return;
	}
	
	SetWorldBossId(param->bossId);
}

void FGsStageGameWorldBoss::OnWorldBossSpawn(const struct IGsMessageParam* InParam)
{
	const FGsWorldBossSpawnParam* param = InParam->Cast<const FGsWorldBossSpawnParam>();
	if (nullptr == param)
	{
		return;
	}

	SetWorldBossId(param->bossId);
}
