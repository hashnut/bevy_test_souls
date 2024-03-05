#pragma once

#include "EItemCollectionMainCategory.generated.h"

/**
 * Main Category Enum 정의
 */
UENUM(BlueprintType)
enum class EItemCollectionMainCategory : uint8
{
	None = 0,
	AttainmentUnit,
	StatUnit,
	Event,
	BookMark,

	Max,
};
