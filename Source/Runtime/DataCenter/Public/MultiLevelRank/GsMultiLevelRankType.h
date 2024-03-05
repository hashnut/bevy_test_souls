#pragma once

#include "GsMultiLevelRankType.generated.h"


UENUM(BlueprintType)
enum class EGsMultiLevelRewardType : uint8
{
	NONE,		// 미선택
	COSTUME,	// 코스튬 보상
	FAIRY,		// 페어리 보상
	STAT,		// 스탯 보상
	ITEM,		// 아이템 보상
};

UENUM(BlueprintType)
enum class EGsMultiLevelRankPledgePosition : uint8
{
	NONE,
	LEFT,
	MIDDLE,
	RIGHT,
};