#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * 대륙탐사에서, 각 카테고리 레벨업에 필요한 데이터
 */
struct CategoryBalanceData
{
	int32 level;
	int32 mapGroupLevelUpCount;
	PassivityId passivityId;
	bool isGlobal;
};

