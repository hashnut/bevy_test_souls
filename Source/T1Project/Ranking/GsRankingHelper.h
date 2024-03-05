// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Shared/Client/SharedEnums/SharedRankEnum.h"

/**
 * ·©Å· Helper Å¬·¡½º
 */
class T1PROJECT_API FGsRankingHelper
{
public:
	static const FSoftObjectPath* GetBadgeIconPath(int32 InRank, RankType InRankType = RankType::SERVER_LEVEL_RANK);

};
