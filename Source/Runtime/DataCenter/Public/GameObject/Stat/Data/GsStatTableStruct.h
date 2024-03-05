#pragma once

#include "GsStatTableStruct.generated.h"

//////////////////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FGsItemRefineOptionPairData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 probability;
};
//////////////////////////////////////////////////////////////////////////////////////
