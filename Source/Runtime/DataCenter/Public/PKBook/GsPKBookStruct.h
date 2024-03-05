#pragma once

#include "GsPKBookStruct.generated.h"

USTRUCT(BlueprintType)
struct DATACENTER_API FGsPKDetectionEffectInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMapEffect")
	int32 distance=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMapEffect")
	int32 blinkRate=0;
};