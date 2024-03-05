#pragma once

#include "EGsStatFormatType.generated.h"

UENUM(BlueprintType)
enum class EGsStatFormatType : uint8
{
	Integer,
	Percent,
	Max,
};