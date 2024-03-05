#pragma once

#include "EGsMailStringParamterType.generated.h"

UENUM(BlueprintType)
enum class EGsMailStringParamterType : uint8
{
	NONE = 0,
	CURRENCY = 1,
	ITEM = 2,
	CREATURE = 3,
	MAP = 4,
};
