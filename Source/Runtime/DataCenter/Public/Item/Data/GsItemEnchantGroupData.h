#pragma once

#include "GsItemEnchantGroupData.generated.h"

USTRUCT(BlueprintType)
struct FGsItemEnchantGroupData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rate;
};
