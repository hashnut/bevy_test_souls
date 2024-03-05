#pragma once

#include "EStatImportanceType.generated.h"

UENUM(BlueprintType)
enum class EStatImportanceType : uint8
{
	Important,
	Assistant,
	Hide,
};