#pragma once

#include "GsMessageParam.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "Shared/Client/SharedEnums/SharedRankEnum.h"

struct FGsBattleArenaStateInfo final : public IGsMessageParam
{
	MapId _mapId = INVALID_MAP_ID;
	EBattleArenaGuildWarState _state = EBattleArenaGuildWarState::ENone;
	uint64 _endTime = 0;

	int32 _redTeamScore = 0;
	int32 _blueTeamScore = 0;

public:
	FGsBattleArenaStateInfo() = default;
	virtual ~FGsBattleArenaStateInfo() = default;
	explicit FGsBattleArenaStateInfo(MapId InMapId, EBattleArenaGuildWarState InState, uint64 InEndTime)
		: _mapId(InMapId), _state(InState), _endTime(InEndTime), _redTeamScore(0), _blueTeamScore(0)
	{
	}

	explicit FGsBattleArenaStateInfo(MapId InMapId, EBattleArenaGuildWarState InState, uint64 InEndTime, int32 InRedTeamScore, int32 InBlueTeamScore)
		: _mapId(InMapId), _state(InState), _endTime(InEndTime), _redTeamScore(InRedTeamScore), _blueTeamScore(InBlueTeamScore)
	{
	}
};



/************************************************************************/
/* Battle Arena Season Rank Data                                        */
/************************************************************************/
struct FGsBattleArenaRankResetData final : public IGsMessageParam
{
	RankSearchType _data = RankSearchType::MAX;
public:
	FGsBattleArenaRankResetData() = default;
	virtual ~FGsBattleArenaRankResetData() = default;
	explicit FGsBattleArenaRankResetData(RankSearchType InData) : _data(InData)
	{
	}
};

struct FGsBattleArenaRankCreateData final : public IGsMessageParam
{
	RankSearchType _type = RankSearchType::MAX;
public:
	FGsBattleArenaRankCreateData() = default;
	virtual ~FGsBattleArenaRankCreateData() = default;
	explicit FGsBattleArenaRankCreateData(RankSearchType InType) : _type(InType)
	{
	}
};

struct FGsBattleArenaRankUpdateData final : public IGsMessageParam
{
	RankSearchType _type = RankSearchType::MAX;
	GuildWarTierDataId _tierDataId = INVALID_GUILD_WAR_TIER_DATA_ID;
	TArray<int32> _updateIndexSet;
public:
	FGsBattleArenaRankUpdateData() = default;
	virtual ~FGsBattleArenaRankUpdateData() = default;
	explicit FGsBattleArenaRankUpdateData(RankSearchType InType, GuildWarTierDataId InTierDataId, const TArray<int32>& InIndexSet)
		: _type(InType), _tierDataId(InTierDataId), _updateIndexSet(InIndexSet)
	{
	}
};