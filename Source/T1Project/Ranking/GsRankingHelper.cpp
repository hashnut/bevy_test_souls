// Fill out your copyright notice in the Description page of Project Settings.


#include "GsRankingHelper.h"

#include "DataSchema/Ranking/GsSchemaRankingReward.h"

#include "Management/ScopeGame/GsRankingManager.h"


const FSoftObjectPath* FGsRankingHelper::GetBadgeIconPath(int32 InRank, RankType InRankType /*= RankType::SERVER_LEVEL_RANK*/)
{
	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return nullptr;

	const FGsSchemaRankingReward* rewardData = rankingManager->GetRewardDataByRankType(InRankType);
	if (nullptr == rewardData)
		return nullptr;

	for (const FGsSchemaRankingRewardData& reward : rewardData->rewardDataList)
	{
		if (reward.rankingRewardRangeMin <= InRank && InRank <= reward.rankingRewardRangeMax)
		{
			return &reward.rankingRewardBadge;
		}
	}

	return nullptr;
}