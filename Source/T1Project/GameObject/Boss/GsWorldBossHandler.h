// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "UI/UIContent/Common/Entry/GsUIItemIconListViewEntry.h"

//=================== field boss manager ====================
struct FGsWorldBossRankData
{	
	int32 _rank;
	ContributePoint _contributePoint;
	PVPAlignmentGrade _pvpGrade;
	FString _userName;
	FString _guildName;
	GuildDBId _guildDBId;
	GuildEmblemId _guildEmblemId;	
	TArray<FGsItemIconData> _rewardIdList;
	TArray<FGsItemIconData> _basicRewardIdList;
	TArray<FGsItemIconData> _lastHitRewardIdList;
	Currency _basicRewardGold;

public:
	void Init(PktIterator<PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ::Rankinglist> inData);
	void Init(const int32 inRank
		, const GuildDBId inDBId
		, const ContributePoint inContributionPoint
		, const PVPAlignmentGrade inPvpGrade
		, const FString& inUserName
		, const FString& inGuildName
		, const GuildEmblemId inEmblemId
		, const TArray<FGsItemIconData>& inItemArray);
	void Sort();
	void Clear();
	bool IsValid() const { return _rank > 0; }
};

struct FGsWorldBossInfo
{
public:
	WorldBossId _worldBossId;
	FDateTime _mapOpenTime;
	FDateTime _bossSpawnTime;	

public:
	FGsWorldBossInfo() = default;
	virtual ~FGsWorldBossInfo() = default;
	explicit FGsWorldBossInfo(WorldBossId inWorldId, FDateTime inMapOpenTime, FDateTime inBossSpawnTime)
		: _worldBossId(inWorldId), _mapOpenTime(inMapOpenTime), _bossSpawnTime(inBossSpawnTime)
	{

	}
};

class T1PROJECT_API FGsWorldBossHandler
{
public:
	TArray<FGsWorldBossRankData> _worldBossRankDataArray;
	FGsWorldBossRankData _lastHitRankData;
	FGsWorldBossRankData _playerRankData;
	TArray<WorldBossId> _worldBossRoomReadyArray;
	TMap<WorldBossId, FGsWorldBossInfo> _worldBossInfoMap;

public:
	void Init(); 
	void Close();

public:
	//UI에서 보스 룸 시간 요청 
	void WorldBossInfo(PD::SC::PKT_SC_ACK_WORLD_BOSS_INFO_READ* inPkt);
	//허드에 보스 룸 열렸다고 알림 
	void InformWorldMapCreate(PD::SC::PKT_SC_INFORM_WORLD_BOSS_MAP_CREATE_READ* inPkt);
	void WorldBossSpawnReady(PD::SC::PKT_SC_WORLD_BOSS_SPAWN_READY_STATE_READ* inPkt);
	
public:
	//월드 보스 던전에서 월드 보스 스폰
	void AlarmWorldBossSpawn(PD::SC::PKT_SC_ALARM_WORLD_BOSS_SPAWN_READ* inPkt);
	//월드 보스 처치
	void WorldBossClear(PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ* inPkt);
	//월드 보스 처치 실패
	void AlarmWorldBossTimeOver(PD::SC::PKT_SC_ALARM_WORLD_BOSS_TIME_OVER_READ* inPkt);
	void WorldBossNotRecommed(PD::SC::PKT_SC_WORLD_BOSS_NOT_RECOMMEND_READ* inPkt);	

private:
	void InvalidBossInfo(WorldBossId inBossId, const FDateTime& inBossRoomReadyTime, const FDateTime& inBossSpawnTime);

public:
	static const struct FGsSchemaWorldBossData* FindWorldBossData(WorldBossId inId);	
	TArray<FGsWorldBossRankData> GetWorldBossRankDataArray() { return _worldBossRankDataArray; }
	FGsWorldBossRankData GetLastHitRankData() { return _lastHitRankData; }
	FGsWorldBossRankData GetPlayerRankData() { return _playerRankData; }
	TArray<WorldBossId> GetWorldBossRoomArray() {return _worldBossRoomReadyArray;}
	bool TryGetWorldAllBossInfo(OUT TMap<WorldBossId, FGsWorldBossInfo>& outWorldBossMap);
	bool TryGetWorldBossInfo(WorldBossId inBossId, OUT FGsWorldBossInfo& outWorldBossInfo);
	void WarpToWorldBoss(WorldBossId In_bossId);
	bool IsActiveWorldBoss(WorldBossId In_bossId);
};