#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


enum class ValidRankDataState
{
	Invalidate,		// 2024/1/22 PKT - 유효하지 않음.
	Pending,		// 2024/1/22 PKT - 보류 중
	Validate,		// 2024/1/22 PKT - 유효함.
};

/*
* 추후 개인전이 생기더라도 그대로 사용 하면 될것 같다.
*/
struct FGsBattleArenaRankListItem
{
	ValidRankDataState _isValidate = ValidRankDataState::Invalidate;

	int32 _index = -1;
	int32 _rank = 0;
	int32 _prevRank = 0;
	GuildWarTierDataId _tierDataId = INVALID_GUILD_WAR_TIER_DATA_ID;
	int32 _score = 0;
	FText _guildName = FText::GetEmpty();
	WorldId _worldId = INVALID_WORLD_ID;
	GuildEmblemId _guildEmblemId = INVALID_GUILD_EMBLEM_ID;
	int32 _winCount = 0;
	int32 _loseCount = 0;
	int32 _tieCount = 0;

	// 2024/1/19 PKT - 개인전에서만 사용
	FText _userName = FText::GetEmpty();
};

struct FGsBattleArenaRankTierData
{
	bool _isCompleted = false;
	TArray<FGsBattleArenaRankListItem> _items;
};

/**
 * 개인전 / 길드전 내 데이터
 */
struct FGsBattleArenaRankLocalData
{
	int32 _playCount = 0;
	FGsBattleArenaRankListItem _rankData;

	time_t _nextUpdateTime = 0;

	void Reset()
	{
		_playCount = 0;
		_nextUpdateTime = 0;
		_rankData = FGsBattleArenaRankListItem();
	}

	bool IsValidRankData() const
	{
		return ValidRankDataState::Validate == _rankData._isValidate && 0 < _nextUpdateTime;
	}

	bool IsValidGuildData() const
	{
		return IsValidRankData() && (INVALID_GUILD_EMBLEM_ID != _rankData._guildEmblemId);
	}
};


struct FGsBattleArenaScheduler
{	
	RankSearchType _rankSearchType = RankSearchType::MAX;
	GuildWarTierDataId _tierDataId = INVALID_GUILD_WAR_TIER_DATA_ID;
	TArray<int32> _indexSet;
	TFunction<void(const FGsBattleArenaScheduler& InData )> _precessFunc = nullptr;
};

struct FGsBattleArenaSchedulePendingData
{
	time_t _time = 0;
	RankSearchType _rankSearchType = RankSearchType::MAX;
	GuildWarTierDataId _tierDataId = INVALID_GUILD_WAR_TIER_DATA_ID;
	int32 _index = -1;

	bool operator==(const FGsBattleArenaSchedulePendingData& InData) const {
		return (_rankSearchType == InData._rankSearchType && _tierDataId == InData._tierDataId && _index == InData._index);
	}
};