#pragma once

#include "EItemCollectionFilterType.generated.h"

UENUM(BlueprintType)
enum class EItemCollectionFilterType : uint8
{
	None = 0,
	/************************************************************************/
	/* by Category                                                          */
	/************************************************************************/
	AttainmentUnit,
	StatUnit,
	Event,
	BookMark,

	/************************************************************************/
	/* by Contents                                                          */
	/************************************************************************/
	Name,
	StatType,

	Max,
};