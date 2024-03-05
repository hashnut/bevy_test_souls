// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"

#include "UTIL/GsTimeSyncUtil.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

/**
 *
 */
UENUM()
enum class EGsFieldBossState
{
	Spawn,
	Despawn,
	Ready,
};

struct FGsFieldBossInfo
{
public:
	FDateTime _spawnTime;
	FVector _spawnPos;

public:
	EGsFieldBossState GetState() {
		return (_spawnTime > FGsTimeSyncUtil::GetServerNowDateTimeUTC() ? EGsFieldBossState::Spawn : EGsFieldBossState::Ready);
	}
};

// for auto move, warp
// no spawn info
class FGsFieldBossInfoCooperation
{
public:
	FVector _spawnPos;
	FVector _autoMoveTargetPos;
	int64 _nearTeleportSpotId;
	FText _spotName;
};

//=================== field boss info ====================
struct FGsBossContributionInfo
{
public:
	FString _name;
	int32 _level;
	FString _guildName;
	float _contribution;
	int32 _rank;
};


struct FGsBossChannelInfo
{
public:
	TArray<FGsBossContributionInfo> _bossInfoArray;
};

struct FGsBossMapContribution
{
public:
	FDateTime _killTime;
	TArray<FGsBossChannelInfo> _bossChannelInfoArray;
};

//=================== field boss ranking ====================
struct FGsBossRewardInfo
{
public:
	int32 _itemId;
	int32 _count;

public:
	FGsBossRewardInfo() = default;
	FGsBossRewardInfo(void* ptr);
	
	FGsBossRewardInfo(int32 inItemid, int32 inCount)
	:_itemId(inItemid), _count(inCount)
	{
;
	}
};

struct FGsBossRankInfo
{
public:
	FString _usereName;
	FString _guildName;
	FString _worldPrefix;
	bool _isParty;
	int32 _level;
	float _contributeRate;
	TArray<TSharedPtr<FGsBossRewardInfo>> _rewardList;
	WorldId _worldId;
	SocialMediaInfo _socialMediaInfo;

public:
	FGsBossRankInfo() = default;
	FGsBossRankInfo(void* ptr);	
	virtual ~FGsBossRankInfo()
	{
		_rewardList.Empty();
	}
};

struct FGsChannelDefeatList
{
public:
	int32 _channelId;	
	FDateTime _logDateTime;
	TArray<TSharedPtr<FGsBossRankInfo>> rankInfoArray;

public:
	FGsChannelDefeatList() = default;
	virtual ~FGsChannelDefeatList()
	{
		rankInfoArray.Empty();
	}
	FGsChannelDefeatList(void* Ptr);
	
};

struct FGsResultDepeatFieldBoss
{
public:
	TArray<TSharedPtr<FGsChannelDefeatList>> _channelDefeatArray;

public:
	FGsResultDepeatFieldBoss() = default;
	virtual ~FGsResultDepeatFieldBoss()
	{
		_channelDefeatArray.Empty();
	}
	FGsResultDepeatFieldBoss(PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ* inRankInfo);
	
};

//=================== field boss manager ====================
class T1PROJECT_API FGsFieldBossHandler
{
public:
	void Init(); 
	void Close();

private:
	TMap<FieldBossId, FGsFieldBossInfo> _fieldBossMap;
	TMap<FieldBossId, FGsBossMapContribution> _bossContributionMap;

	TMap<FieldBossId, FGsFieldBossInfoCooperation*> _mapCooperationBossInfo;

private:
	MsgSystemHandleArray _systemMsgList;
	MsgGameObjHandleArray _gameObjectHandleArray;

private:
	FText _fieldBossDespawnText;
	FText _fieldBossExistenceText;
	FText _fieldBossKillText;
	FText _fieldBossKillGetItemText;
	FText _fieldBossSpawnText;
	FText _tickerFieldBossKillText;
	FText _tickerFieldBossSpawnText;

	static int _fieldBossReadyMinutes;

private:
	TMap<int64, int64> _aggroTargetMap;
	TArray<int64> _contributionUserIdArray;

private:
	TMap<FieldBossId, TSharedPtr<FGsResultDepeatFieldBoss>> depeatResultFieldBossMap;

public:
	void AddFieldBossInfo(FieldBossId inFieldBossId, const FVector& inPos, time_t inEndTime);
	void RemoveFieldBossInfo(FieldBossId inFieldBossId);
	void ClearFieldBossInfo();
	void OnWarpReady();
	bool IsBossInfoEmpty();	
	static TArray<const struct FGsSchemaFieldBossData*> GetFieldBossDataByLevelId(int32 inLevelId);
	static TArray<const struct  FGsSchemaNpcData*> GetFieldBossNpcDataArray(int32 inLevelId);
	static const struct FGsSchemaFieldBossData* GetFieldBossData(int32 inMapId, CreatureId inNpcId);
	bool TryGetRemainFieldBossTime(FieldBossId inFieldBossId, FTimespan& outRemainTime);
	bool TryGetFieldBossState(FieldBossId inFieldBossId, EGsFieldBossState& outFieldBossState);
	bool TryGetFieldBossSpawnTime(FieldBossId inFieldBossId, FDateTime& outFieldBossSpawnTime);
	static void TryGetFieldBossInfos(FieldBossId inFieldBossId, TArray<FVector>& outSpawnPosArray, int& outMapId, int& outWarpSpotId);
	TMap<FieldBossId, FGsFieldBossInfo> GetFieldBossMap() { return _fieldBossMap; }
	static bool IsNeedToBeShownWorldMap(FieldBossId inCreatureId);
	static const struct FGsSchemaFieldBossData* FindFieldBossData(FieldBossId inFieldBoss);
	static const struct FGsSchemaFieldBossData* FindFieldBossDataByMutantBossId(FieldBossId inMutantBossldBoss);
	// 필드 보스 디스폰
	// https://jira.com2us.com/jira/browse/CHR-23655
	void FieldBossDespawn(FieldBossId In_fieldBossId);
	

public:
	void OnFieldBossSpawn(const IGsMessageParam* InParam);
	void OnFieldBossDespawn(const IGsMessageParam* InParam);
	void OnFieldBossDie(const IGsMessageParam* InParam);	
	void OnFieldBossItemGet(const IGsMessageParam* InParam);
	TMap<FieldBossId, FGsBossMapContribution> GetBossContribution() {	return _bossContributionMap;}

public:
	void SetDefeatResult(PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ* inDefeatResult);
	TMap<FieldBossId, TSharedPtr<FGsResultDepeatFieldBoss>> GetDefeatResultMap(){return depeatResultFieldBossMap;}

	// static 함수
public:
	// 보스 상태 구하기
	static EGsFieldBossState GetBossState(bool In_isHaveSpawnTime, FDateTime In_spawnTime);
	
	//aggro
public:
	int64 GetAggroTarget(int64 inNpcGameId);
	//boss gameid, target game id
	const TMap<int64, int64>& GetAllAggroTarget(){return _aggroTargetMap;}
	void AddAggroTarget(int64 inNpcGameId, int64 inTargetGameId);
	void OnRemoveAggroTarget(const IGsMessageParam* InParam);
	bool ClearAggroTarget(int64 inNpcGameId);
	void ClearAllAggroTarget();

public:
	TArray<int64> GetContributionUserId(){return _contributionUserIdArray;}
	void SetConstributionUserId(const TArray<int64>& inContribution) {	_contributionUserIdArray = inContribution;}
	void ClearContribution(){ _contributionUserIdArray.Empty();}
	void ClearBossInfoCooperation();

	FGsFieldBossInfoCooperation* FindBossInfoCooperation(FieldBossId In_fieldBossId);

	void MoveToFieldBoss(FieldBossId In_bossId);
	void WarpToFieldBoss(FieldBossId In_bossId);
};
