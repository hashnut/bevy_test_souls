#pragma once
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsBattlePassMsgParamPass : public IGsMessageParam
{
	BattlePassId _passId;
	bool _isAddRewardActive{ false };

	FGsBattlePassMsgParamPass() = default;
	virtual ~FGsBattlePassMsgParamPass() = default;
	explicit FGsBattlePassMsgParamPass(BattlePassId passId)
		: _passId(passId)
	{
	}
};

struct FGsBattlePassMsgParamLevel : public IGsMessageParam
{
	BattlePassId _passId;
	Level _level;

	FGsBattlePassMsgParamLevel() = default;
	virtual ~FGsBattlePassMsgParamLevel() = default;
	explicit FGsBattlePassMsgParamLevel(BattlePassId passId, Level level)
		: _passId(passId), _level(level)
	{
	}
};

struct FGsBattlePassMsgParamLevelReward : public IGsMessageParam
{
	BattlePassId _passId;
	Level _level;
	bool _isAddReward;

	FGsBattlePassMsgParamLevelReward() = default;
	virtual ~FGsBattlePassMsgParamLevelReward() = default;
	explicit FGsBattlePassMsgParamLevelReward(BattlePassId passId, Level level, bool isAddReward)
		: _passId(passId), _level(level), _isAddReward(isAddReward)
	{
	}
};

struct FGsBattlePassMsgParamLevelRewardAll : public IGsMessageParam
{
	BattlePassId	_passId;
	TArray<Level>	_rewardedLevel;

	FGsBattlePassMsgParamLevelRewardAll() = default;
	virtual ~FGsBattlePassMsgParamLevelRewardAll() = default;
	explicit FGsBattlePassMsgParamLevelRewardAll(BattlePassId passId, TArray<Level>& levels)
		: _passId(passId)
	{
		_rewardedLevel.Append(levels);
	}
};

struct FGsBattlePassMsgParamMissionTab : public IGsMessageParam
{
	TArray<TPair<BattlePassId, BattlePassMissionTabId>> _tabId;

	FGsBattlePassMsgParamMissionTab() = default;
	virtual ~FGsBattlePassMsgParamMissionTab() = default;
	explicit FGsBattlePassMsgParamMissionTab(TArray<TPair<BattlePassId, BattlePassMissionTabId>>& tabId)
	{
		_tabId.Reset();
		_tabId.Append(tabId);
	}
};

struct FGsBattlePassMsgParamMission : public IGsMessageParam
{
	BattlePassId _passId;
	BattlePassMissionTabId _tabId;
	BattlePassMissionId _missionId;

	FGsBattlePassMsgParamMission() = default;
	virtual ~FGsBattlePassMsgParamMission() = default;
	explicit FGsBattlePassMsgParamMission(BattlePassId passId, BattlePassMissionTabId tabId, BattlePassMissionId missionId)
		: _passId(passId), _tabId(tabId), _missionId(missionId)
	{
	}
};

struct FGsBattlePassMsgParamPassModify : public IGsMessageParam
{
	bool _isAddedPass{ false };
	bool _isClosedPass{ false };

	FGsBattlePassMsgParamPassModify() = default;
	virtual ~FGsBattlePassMsgParamPassModify() = default;
	explicit FGsBattlePassMsgParamPassModify(bool isAddedPass, bool isClosedPass)
		: _isAddedPass(isAddedPass), _isClosedPass(isClosedPass)
	{
	}
};