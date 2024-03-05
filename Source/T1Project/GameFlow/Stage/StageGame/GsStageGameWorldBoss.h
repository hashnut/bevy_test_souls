// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStageGameBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageBoss.h"

//------------------------------------------------------------------------------
// 스테이지 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class T1PROJECT_API FGsStageGameWorldBoss : public FGsStageGameBase
{
public:
	FGsStageGameWorldBoss();
	virtual ~FGsStageGameWorldBoss();

public:
	WorldBossId _worldBossId;
	MsgBossHandleArray _msgBossHandleList;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

public:
	//던전 안에서 보스 룸 레디
	void OnWorldBossSpawnReadyState(const struct IGsMessageParam* InParam);
	//던전 안에서 보스 스폰
	void OnWorldBossSpawn(const struct IGsMessageParam* InParam);

public:
	void SetWorldBossId(WorldBossId inWorldBoss) { _worldBossId = inWorldBoss; }
	WorldBossId GetWorldBossId() { return _worldBossId; }
};
