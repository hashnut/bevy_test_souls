#pragma once
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"

struct FGsBossSelectMsgParam : public IGsMessageParam
{
public:	
	const FGsSchemaFieldBossData* _bossData;
	const FGsSchemaFieldBossData* _mutantBossData;

public:
	FGsBossSelectMsgParam() = default;
	virtual ~FGsBossSelectMsgParam() = default;
	explicit FGsBossSelectMsgParam(const FGsSchemaFieldBossData* inBossInfo, const FGsSchemaFieldBossData* inMutant)
		: _bossData(inBossInfo), _mutantBossData(inMutant)
	{
	}
};

struct FGsResultDefeatFieldBossMsgParam : public IGsMessageParam
{
public:
	FieldBossId _bossId;
	TSharedPtr<FGsResultDepeatFieldBoss> _resultDepeatBoss;

public:
	FGsResultDefeatFieldBossMsgParam() = default;
	virtual ~FGsResultDefeatFieldBossMsgParam() = default;
	explicit FGsResultDefeatFieldBossMsgParam(PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ* inData)
	{
		_bossId = inData->FieldBossId();		
		_resultDepeatBoss = MakeShareable(new FGsResultDepeatFieldBoss(inData));
	}
};

struct FGsCompetitionFieldBossLastDead : public IGsMessageParam
{
public:
	FieldBossId bossId;
	time_t lastDeadTime;

public:
	FGsCompetitionFieldBossLastDead() = default;
	virtual ~FGsCompetitionFieldBossLastDead() = default;
	//explicit FGsCompetitionFieldBossLastDead(PD::SC::PKT_SC_ACK_COMPETITION_FIELD_BOSS_LAST_DEAD_TIME_READ* inPacket)
	//{
	//	bossId = inPacket->FieldBossId();
	//	lastDeadTime = inPacket->LastDeadTime();
	//}
	explicit FGsCompetitionFieldBossLastDead(PD::GC::PKT_GC_ACK_COMPETITION_FIELD_BOSS_LAST_DEAD_TIME_READ* inPacket)
	{
		bossId = inPacket->FieldBossId();
		lastDeadTime = inPacket->LastDeadTime();
	}
};

struct FGsWorldBossParam : public IGsMessageParam
{
public:
	WorldBossId bossId;	

public:
	FGsWorldBossParam() = default;
	virtual ~FGsWorldBossParam() = default;
	explicit FGsWorldBossParam(WorldBossId inWorldBossId)
		: bossId(inWorldBossId)
	{
		
	}
};

struct FGsWorldBossRoomReadyParam : public IGsMessageParam
{
public:
	WorldBossId bossId;
	FDateTime bossSpawnTime;

public:
	FGsWorldBossRoomReadyParam() = default;
	virtual ~FGsWorldBossRoomReadyParam() = default;
	explicit FGsWorldBossRoomReadyParam(WorldBossId inWorldBossId, FDateTime inBossSpawnTime)
		: bossId(inWorldBossId), bossSpawnTime(inBossSpawnTime)
	{
		
	}
};

struct FGsWorldBossSpawnParam : public IGsMessageParam
{
public:
	WorldBossId bossId;
	FDateTime bossEndTime;

public:
	FGsWorldBossSpawnParam() = default;
	virtual ~FGsWorldBossSpawnParam() = default;
	explicit FGsWorldBossSpawnParam(WorldBossId inWorldBossId, FDateTime inBossEndTime)
		: bossId(inWorldBossId), bossEndTime(inBossEndTime)
	{

	}
};

struct FGsWorldBossTimeOverParam : public IGsMessageParam
{
public:
	WorldBossId bossId;
	bool isConsolation;

public:
	FGsWorldBossTimeOverParam() = default;
	virtual ~FGsWorldBossTimeOverParam() = default;
	explicit FGsWorldBossTimeOverParam(WorldBossId inWorldBossId, bool inIsConsolation)
		: bossId(inWorldBossId), isConsolation(inIsConsolation)
	{

	}
};
