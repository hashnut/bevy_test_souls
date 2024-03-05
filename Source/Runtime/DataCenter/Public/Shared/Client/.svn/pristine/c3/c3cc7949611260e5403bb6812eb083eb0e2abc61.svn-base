#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedSpawnEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
enum class SPAWN_REASON : uint8
{
	// [USER/NPC/WORK_OBJECT]
	// Spawn : 일반적인 스폰
	// Despawn : 일반적인 디스폰(User만 사용하며, 접속종료/Warp 등)
	NORMAL = 0,

	// [USER/NPC/WORK_OBJECT]
	// Spawn : 거리가 가까워져 안 보이던 OBJECT가 보이는 경우
	// Despawn : 거리가 멀어져 보이던 OBJECT가 안 보이는 경우
	GRID = 1,

	// [USER/NPC]
	// Spawn : 소환으로 스폰(유저의 경우 소환 스킬로 소환당하는 경우/NPC의 경우 말 그대로 소환)
	// Despawn : 역소환으로 디스폰(유저의 경우 소환 스킬로 소환당하는 경우/NPC의 경우 말 그대로 역소환)
	SUMMON = 2,

	// [NPC]
	// Spawn : 변신(NPC에만 사용)
	// Despawn : 사용 안함
	CHANGE = 3,

	// [NPC]
	// Spawn : 사용 안함
	// Despawn : 사망(NPC에만 사용. 유저의 경우 사망시 마을로 귀환하더라도 REASON_NORMAL을 사용)
	DEATH = 4,

	// [NPC]
	// Spawn : 사용 안함
	// Despawn : 퇴각(NPC에만 사용. 기존 스폰된 NPC가 어떤 연유로 인해 Warp 퇴각하는 경우에 사용)
	WITHDRAWAL = 5,

	// [DROP_OBJECT]
	// Spawn : 사용 안함
	// Despawn : 유저의 습득, 유지 시간 초과로 인한 소멸(DropObject만 사용.)
	EXTINCTION = 6,

	// 도물 몬스터 포탈을 소환하고 포탈안으로  점프하여 뛰어든다. 포탈이 닫힌다  
	RUNAWAY = 7,

	MAX = 8,
};

UENUM()
enum class UserSpawnReason : uint8
{
	ENTERWORLD = 0,
	WARP = 1,
	RESURRECT_TOMB = 2,
	RESURRECT_TOWN = 3,
	RESURRECT_QUEST = 4,
	PHASE_CHANGE = 5,
	SUMMON = 6,
	VAPORIZE_OFF = 7,
	VAPORIZE_ON = 8,

	// 자동 부활
	AUTO_RESPAWN = 9,

	// 기사단 던전에서 나가는 경우
	LAVE_GUILD_DUNGEON = 10,

	// 기사단 영지 던전에서 나가는 경우
	LEAVE_AGIT_DUNGEON_RAID = 11,

	// 기사단 영지 나가는 경우
	LEAVE_AGIT = 12,

	// 기사단 영지 랜덤 스크롤 나가는 경우
	LEAVE_AGIT_RANDOM_SCROL = 13,

	MAX = 14,
};

UENUM(BlueprintType)
enum class ResurrectType : uint8
{
	NOW = 0,
	GRAVE = 1,
	TOWN = 2,

	// 즉시 부활
	INSTANT = 3,

	// 마지막 이벤트 위치
	LASTPOSTION = 4,

	// 관전모드
	OBSERVING = 5,

	// 침공전 퇴장
	INVASIONOUT = 6,

	// 서버 즉시 부활 (테스트)
	SERVER_INSTANT = 7,

	// 던전내 부활
	IN_DUNGEON = 8,

	MAX = 9,
};

UENUM()
enum class SpawnStatus : uint8
{

	// 월드 퇴장 - 오직, User만 가능한 Status
	WORLD_LEAVED = 0,

	// 월드 입장 - 오직, User만 가능한 Status
	WORLD_ENTERED = 1,

	// 채널 스폰 중
	SPAWNING = 2,

	// 채널 스폰 완료
	SPAWNED = 3,

	// 채널 디스폰 중
	DESPAWNING = 4,

	// 채널 디스폰 완료
	DESPAWNED = 5,

	// 재연결 대기 준비 완료 - 오직, User만 가능한 Status
	RECONNECT_WAIT = 6,

	MAX = 7,
};

UENUM()
enum class LoadTopoEffectType : uint8
{
	NONE = 0,

	// 다른 맵: 워프연출(인게임, 살아있을때) -> FadeOut -> 로딩창 -> FadeIn -> 스폰연출
	DIFF_MAP = 1,

	// 같은맵 다른좌표: 워프연출(인게임, 살아있을때) -> FadeOut -> FadeIn -> 스폰연출
	SAME_MAP_DIFF_POS = 2,

	// 같은맵 같은좌표: FadeIn -> FadeOut
	SAME_MAP_SAME_POS = 3,

	MAX = 4,
};

UENUM(BlueprintType)
enum class OffensiveType : uint8
{
	// NpcData 의 IsOffensive 데이터에 따라 선공 여부 판단. 선공 시야는 NpcData의 SightRadius 사용
	USE_NPC_DATA = 0,

	// 선공. 선공 시야는 SpawnData의 SightRadius 사용
	OFFENSIVE = 1,

	// 비선공
	NORMAL = 2,

	MAX = 3,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline SPAWN_REASON SPAWN_REASONStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return SPAWN_REASON::NORMAL;
	if (str.Compare(TEXT("GRID"), ESearchCase::IgnoreCase) == 0)	return SPAWN_REASON::GRID;
	if (str.Compare(TEXT("SUMMON"), ESearchCase::IgnoreCase) == 0)	return SPAWN_REASON::SUMMON;
	if (str.Compare(TEXT("CHANGE"), ESearchCase::IgnoreCase) == 0)	return SPAWN_REASON::CHANGE;
	if (str.Compare(TEXT("DEATH"), ESearchCase::IgnoreCase) == 0)	return SPAWN_REASON::DEATH;
	if (str.Compare(TEXT("WITHDRAWAL"), ESearchCase::IgnoreCase) == 0)	return SPAWN_REASON::WITHDRAWAL;
	if (str.Compare(TEXT("EXTINCTION"), ESearchCase::IgnoreCase) == 0)	return SPAWN_REASON::EXTINCTION;
	if (str.Compare(TEXT("RUNAWAY"), ESearchCase::IgnoreCase) == 0)	return SPAWN_REASON::RUNAWAY;
	return SPAWN_REASON::MAX;
}
inline UserSpawnReason UserSpawnReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ENTERWORLD"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::ENTERWORLD;
	if (str.Compare(TEXT("WARP"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::WARP;
	if (str.Compare(TEXT("RESURRECT_TOMB"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::RESURRECT_TOMB;
	if (str.Compare(TEXT("RESURRECT_TOWN"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::RESURRECT_TOWN;
	if (str.Compare(TEXT("RESURRECT_QUEST"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::RESURRECT_QUEST;
	if (str.Compare(TEXT("PHASE_CHANGE"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::PHASE_CHANGE;
	if (str.Compare(TEXT("SUMMON"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::SUMMON;
	if (str.Compare(TEXT("VAPORIZE_OFF"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::VAPORIZE_OFF;
	if (str.Compare(TEXT("VAPORIZE_ON"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::VAPORIZE_ON;
	if (str.Compare(TEXT("AUTO_RESPAWN"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::AUTO_RESPAWN;
	if (str.Compare(TEXT("LAVE_GUILD_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::LAVE_GUILD_DUNGEON;
	if (str.Compare(TEXT("LEAVE_AGIT_DUNGEON_RAID"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::LEAVE_AGIT_DUNGEON_RAID;
	if (str.Compare(TEXT("LEAVE_AGIT"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::LEAVE_AGIT;
	if (str.Compare(TEXT("LEAVE_AGIT_RANDOM_SCROL"), ESearchCase::IgnoreCase) == 0)	return UserSpawnReason::LEAVE_AGIT_RANDOM_SCROL;
	return UserSpawnReason::MAX;
}
inline ResurrectType ResurrectTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NOW"), ESearchCase::IgnoreCase) == 0)	return ResurrectType::NOW;
	if (str.Compare(TEXT("GRAVE"), ESearchCase::IgnoreCase) == 0)	return ResurrectType::GRAVE;
	if (str.Compare(TEXT("TOWN"), ESearchCase::IgnoreCase) == 0)	return ResurrectType::TOWN;
	if (str.Compare(TEXT("INSTANT"), ESearchCase::IgnoreCase) == 0)	return ResurrectType::INSTANT;
	if (str.Compare(TEXT("LASTPOSTION"), ESearchCase::IgnoreCase) == 0)	return ResurrectType::LASTPOSTION;
	if (str.Compare(TEXT("OBSERVING"), ESearchCase::IgnoreCase) == 0)	return ResurrectType::OBSERVING;
	if (str.Compare(TEXT("INVASIONOUT"), ESearchCase::IgnoreCase) == 0)	return ResurrectType::INVASIONOUT;
	if (str.Compare(TEXT("SERVER_INSTANT"), ESearchCase::IgnoreCase) == 0)	return ResurrectType::SERVER_INSTANT;
	if (str.Compare(TEXT("IN_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return ResurrectType::IN_DUNGEON;
	return ResurrectType::MAX;
}
inline SpawnStatus SpawnStatusStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("WORLD_LEAVED"), ESearchCase::IgnoreCase) == 0)	return SpawnStatus::WORLD_LEAVED;
	if (str.Compare(TEXT("WORLD_ENTERED"), ESearchCase::IgnoreCase) == 0)	return SpawnStatus::WORLD_ENTERED;
	if (str.Compare(TEXT("SPAWNING"), ESearchCase::IgnoreCase) == 0)	return SpawnStatus::SPAWNING;
	if (str.Compare(TEXT("SPAWNED"), ESearchCase::IgnoreCase) == 0)	return SpawnStatus::SPAWNED;
	if (str.Compare(TEXT("DESPAWNING"), ESearchCase::IgnoreCase) == 0)	return SpawnStatus::DESPAWNING;
	if (str.Compare(TEXT("DESPAWNED"), ESearchCase::IgnoreCase) == 0)	return SpawnStatus::DESPAWNED;
	if (str.Compare(TEXT("RECONNECT_WAIT"), ESearchCase::IgnoreCase) == 0)	return SpawnStatus::RECONNECT_WAIT;
	return SpawnStatus::MAX;
}
inline LoadTopoEffectType LoadTopoEffectTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return LoadTopoEffectType::NONE;
	if (str.Compare(TEXT("DIFF_MAP"), ESearchCase::IgnoreCase) == 0)	return LoadTopoEffectType::DIFF_MAP;
	if (str.Compare(TEXT("SAME_MAP_DIFF_POS"), ESearchCase::IgnoreCase) == 0)	return LoadTopoEffectType::SAME_MAP_DIFF_POS;
	if (str.Compare(TEXT("SAME_MAP_SAME_POS"), ESearchCase::IgnoreCase) == 0)	return LoadTopoEffectType::SAME_MAP_SAME_POS;
	return LoadTopoEffectType::MAX;
}
inline OffensiveType OffensiveTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("USE_NPC_DATA"), ESearchCase::IgnoreCase) == 0)	return OffensiveType::USE_NPC_DATA;
	if (str.Compare(TEXT("OFFENSIVE"), ESearchCase::IgnoreCase) == 0)	return OffensiveType::OFFENSIVE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return OffensiveType::NORMAL;
	return OffensiveType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString SPAWN_REASONEnumToString(SPAWN_REASON value)
{
	switch (value)
	{
		case SPAWN_REASON::NORMAL: return TEXT("NORMAL");
		case SPAWN_REASON::GRID: return TEXT("GRID");
		case SPAWN_REASON::SUMMON: return TEXT("SUMMON");
		case SPAWN_REASON::CHANGE: return TEXT("CHANGE");
		case SPAWN_REASON::DEATH: return TEXT("DEATH");
		case SPAWN_REASON::WITHDRAWAL: return TEXT("WITHDRAWAL");
		case SPAWN_REASON::EXTINCTION: return TEXT("EXTINCTION");
		case SPAWN_REASON::RUNAWAY: return TEXT("RUNAWAY");
	}
	return FString();
}
inline FString UserSpawnReasonEnumToString(UserSpawnReason value)
{
	switch (value)
	{
		case UserSpawnReason::ENTERWORLD: return TEXT("ENTERWORLD");
		case UserSpawnReason::WARP: return TEXT("WARP");
		case UserSpawnReason::RESURRECT_TOMB: return TEXT("RESURRECT_TOMB");
		case UserSpawnReason::RESURRECT_TOWN: return TEXT("RESURRECT_TOWN");
		case UserSpawnReason::RESURRECT_QUEST: return TEXT("RESURRECT_QUEST");
		case UserSpawnReason::PHASE_CHANGE: return TEXT("PHASE_CHANGE");
		case UserSpawnReason::SUMMON: return TEXT("SUMMON");
		case UserSpawnReason::VAPORIZE_OFF: return TEXT("VAPORIZE_OFF");
		case UserSpawnReason::VAPORIZE_ON: return TEXT("VAPORIZE_ON");
		case UserSpawnReason::AUTO_RESPAWN: return TEXT("AUTO_RESPAWN");
		case UserSpawnReason::LAVE_GUILD_DUNGEON: return TEXT("LAVE_GUILD_DUNGEON");
		case UserSpawnReason::LEAVE_AGIT_DUNGEON_RAID: return TEXT("LEAVE_AGIT_DUNGEON_RAID");
		case UserSpawnReason::LEAVE_AGIT: return TEXT("LEAVE_AGIT");
		case UserSpawnReason::LEAVE_AGIT_RANDOM_SCROL: return TEXT("LEAVE_AGIT_RANDOM_SCROL");
	}
	return FString();
}
inline FString ResurrectTypeEnumToString(ResurrectType value)
{
	switch (value)
	{
		case ResurrectType::NOW: return TEXT("NOW");
		case ResurrectType::GRAVE: return TEXT("GRAVE");
		case ResurrectType::TOWN: return TEXT("TOWN");
		case ResurrectType::INSTANT: return TEXT("INSTANT");
		case ResurrectType::LASTPOSTION: return TEXT("LASTPOSTION");
		case ResurrectType::OBSERVING: return TEXT("OBSERVING");
		case ResurrectType::INVASIONOUT: return TEXT("INVASIONOUT");
		case ResurrectType::SERVER_INSTANT: return TEXT("SERVER_INSTANT");
		case ResurrectType::IN_DUNGEON: return TEXT("IN_DUNGEON");
	}
	return FString();
}
inline FString SpawnStatusEnumToString(SpawnStatus value)
{
	switch (value)
	{
		case SpawnStatus::WORLD_LEAVED: return TEXT("WORLD_LEAVED");
		case SpawnStatus::WORLD_ENTERED: return TEXT("WORLD_ENTERED");
		case SpawnStatus::SPAWNING: return TEXT("SPAWNING");
		case SpawnStatus::SPAWNED: return TEXT("SPAWNED");
		case SpawnStatus::DESPAWNING: return TEXT("DESPAWNING");
		case SpawnStatus::DESPAWNED: return TEXT("DESPAWNED");
		case SpawnStatus::RECONNECT_WAIT: return TEXT("RECONNECT_WAIT");
	}
	return FString();
}
inline FString LoadTopoEffectTypeEnumToString(LoadTopoEffectType value)
{
	switch (value)
	{
		case LoadTopoEffectType::NONE: return TEXT("NONE");
		case LoadTopoEffectType::DIFF_MAP: return TEXT("DIFF_MAP");
		case LoadTopoEffectType::SAME_MAP_DIFF_POS: return TEXT("SAME_MAP_DIFF_POS");
		case LoadTopoEffectType::SAME_MAP_SAME_POS: return TEXT("SAME_MAP_SAME_POS");
	}
	return FString();
}
inline FString OffensiveTypeEnumToString(OffensiveType value)
{
	switch (value)
	{
		case OffensiveType::USE_NPC_DATA: return TEXT("USE_NPC_DATA");
		case OffensiveType::OFFENSIVE: return TEXT("OFFENSIVE");
		case OffensiveType::NORMAL: return TEXT("NORMAL");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString SPAWN_REASONEnumToDisplayKey(SPAWN_REASON value)
{
	switch (value)
	{
		case SPAWN_REASON::NORMAL: return TEXT("");
		case SPAWN_REASON::GRID: return TEXT("");
		case SPAWN_REASON::SUMMON: return TEXT("");
		case SPAWN_REASON::CHANGE: return TEXT("");
		case SPAWN_REASON::DEATH: return TEXT("");
		case SPAWN_REASON::WITHDRAWAL: return TEXT("");
		case SPAWN_REASON::EXTINCTION: return TEXT("");
		case SPAWN_REASON::RUNAWAY: return TEXT("");
	}
	return FString();
}
inline FString UserSpawnReasonEnumToDisplayKey(UserSpawnReason value)
{
	switch (value)
	{
		case UserSpawnReason::ENTERWORLD: return TEXT("");
		case UserSpawnReason::WARP: return TEXT("");
		case UserSpawnReason::RESURRECT_TOMB: return TEXT("");
		case UserSpawnReason::RESURRECT_TOWN: return TEXT("");
		case UserSpawnReason::RESURRECT_QUEST: return TEXT("");
		case UserSpawnReason::PHASE_CHANGE: return TEXT("");
		case UserSpawnReason::SUMMON: return TEXT("");
		case UserSpawnReason::VAPORIZE_OFF: return TEXT("");
		case UserSpawnReason::VAPORIZE_ON: return TEXT("");
		case UserSpawnReason::AUTO_RESPAWN: return TEXT("");
		case UserSpawnReason::LAVE_GUILD_DUNGEON: return TEXT("");
		case UserSpawnReason::LEAVE_AGIT_DUNGEON_RAID: return TEXT("");
		case UserSpawnReason::LEAVE_AGIT: return TEXT("");
		case UserSpawnReason::LEAVE_AGIT_RANDOM_SCROL: return TEXT("");
	}
	return FString();
}
inline FString ResurrectTypeEnumToDisplayKey(ResurrectType value)
{
	switch (value)
	{
		case ResurrectType::NOW: return TEXT("");
		case ResurrectType::GRAVE: return TEXT("");
		case ResurrectType::TOWN: return TEXT("");
		case ResurrectType::INSTANT: return TEXT("");
		case ResurrectType::LASTPOSTION: return TEXT("");
		case ResurrectType::OBSERVING: return TEXT("");
		case ResurrectType::INVASIONOUT: return TEXT("");
		case ResurrectType::SERVER_INSTANT: return TEXT("");
		case ResurrectType::IN_DUNGEON: return TEXT("");
	}
	return FString();
}
inline FString SpawnStatusEnumToDisplayKey(SpawnStatus value)
{
	switch (value)
	{
		case SpawnStatus::WORLD_LEAVED: return TEXT("");
		case SpawnStatus::WORLD_ENTERED: return TEXT("");
		case SpawnStatus::SPAWNING: return TEXT("");
		case SpawnStatus::SPAWNED: return TEXT("");
		case SpawnStatus::DESPAWNING: return TEXT("");
		case SpawnStatus::DESPAWNED: return TEXT("");
		case SpawnStatus::RECONNECT_WAIT: return TEXT("");
	}
	return FString();
}
inline FString LoadTopoEffectTypeEnumToDisplayKey(LoadTopoEffectType value)
{
	switch (value)
	{
		case LoadTopoEffectType::NONE: return TEXT("");
		case LoadTopoEffectType::DIFF_MAP: return TEXT("");
		case LoadTopoEffectType::SAME_MAP_DIFF_POS: return TEXT("");
		case LoadTopoEffectType::SAME_MAP_SAME_POS: return TEXT("");
	}
	return FString();
}
inline FString OffensiveTypeEnumToDisplayKey(OffensiveType value)
{
	switch (value)
	{
		case OffensiveType::USE_NPC_DATA: return TEXT("");
		case OffensiveType::OFFENSIVE: return TEXT("");
		case OffensiveType::NORMAL: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidSPAWN_REASON(SPAWN_REASON value) noexcept
{
	return (value >= SPAWN_REASON::NORMAL && value < SPAWN_REASON::MAX);
}

inline bool IsValidUserSpawnReason(UserSpawnReason value) noexcept
{
	return (value >= UserSpawnReason::ENTERWORLD && value < UserSpawnReason::MAX);
}

inline bool IsValidResurrectType(ResurrectType value) noexcept
{
	return (value >= ResurrectType::NOW && value < ResurrectType::MAX);
}

inline bool IsValidSpawnStatus(SpawnStatus value) noexcept
{
	return (value >= SpawnStatus::WORLD_LEAVED && value < SpawnStatus::MAX);
}

inline bool IsValidLoadTopoEffectType(LoadTopoEffectType value) noexcept
{
	return (value >= LoadTopoEffectType::NONE && value < LoadTopoEffectType::MAX);
}

inline bool IsValidOffensiveType(OffensiveType value) noexcept
{
	return (value >= OffensiveType::USE_NPC_DATA && value < OffensiveType::MAX);
}

