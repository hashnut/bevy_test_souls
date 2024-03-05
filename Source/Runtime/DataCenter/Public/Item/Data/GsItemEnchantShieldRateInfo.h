#pragma once

#include "GsItemEnchantShieldRateInfo.generated.h"

USTRUCT(BlueprintType)
struct FGsItemEnchantShieldRateInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 enchantShield;
};
