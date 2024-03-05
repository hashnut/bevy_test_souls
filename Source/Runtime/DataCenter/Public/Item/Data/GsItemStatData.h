#pragma once

#include "Item/GsStatTypeNameSelector.h"
#include "GsItemStatData.generated.h"

USTRUCT(BlueprintType)
struct FGsItemStatData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGsStatTypeNameSelector type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 value;

public:
	FGsItemStatData() = default;
	FGsItemStatData(FGsStatTypeNameSelector inType, int32 inValue)
		: type(inType), value(inValue)
	{

	}
};