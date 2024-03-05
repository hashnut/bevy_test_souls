#pragma once
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Achievement/GsAchievementData.h"

struct FGsAchievementMsgParamUpdateAchievement : public IGsMessageParam
{
	TWeakPtr<FGsAchievementData> _data;
	uint64 _prePoint;
	uint64 _acqPoint;

	FGsAchievementMsgParamUpdateAchievement() = default;
	virtual ~FGsAchievementMsgParamUpdateAchievement() = default;
	explicit FGsAchievementMsgParamUpdateAchievement(
		TWeakPtr<FGsAchievementData> data, uint64 prePoint, uint64 acqPoint)
		: _data(data), _prePoint(prePoint), _acqPoint(acqPoint)
	{
	}
};

struct FGsAchievementMsgParamChangeAchievement : public IGsMessageParam
{
	TWeakPtr<FGsAchievementData> _data;

	FGsAchievementMsgParamChangeAchievement() = default;
	virtual ~FGsAchievementMsgParamChangeAchievement() = default;
	explicit FGsAchievementMsgParamChangeAchievement(TWeakPtr<FGsAchievementData> data) : _data(data)
	{
	}
};

struct FGsAchievementMsgParamAckAchievementReward : public IGsMessageParam
{
	AchievementId _id;

	FGsAchievementMsgParamAckAchievementReward() = default;
	virtual ~FGsAchievementMsgParamAckAchievementReward() = default;
	explicit FGsAchievementMsgParamAckAchievementReward(AchievementId id) : _id(id)
	{
	}
};

struct FGsAchievementMsgParamAckThemeReward : public IGsMessageParam
{
	AchievementThemeId _id;

	FGsAchievementMsgParamAckThemeReward() = default;
	virtual ~FGsAchievementMsgParamAckThemeReward() = default;
	explicit FGsAchievementMsgParamAckThemeReward(AchievementThemeId id) : _id(id)
	{
	}
};

struct FGsAchievementMsgParamAckRewardAll : public IGsMessageParam
{
	TArray<AchievementId> _achievementIds;

	FGsAchievementMsgParamAckRewardAll() = default;
	virtual ~FGsAchievementMsgParamAckRewardAll() = default;
	explicit FGsAchievementMsgParamAckRewardAll(const TArray<AchievementId>& achievementIds) 
		: _achievementIds(achievementIds)
	{
	}
};