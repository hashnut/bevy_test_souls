#pragma once

#include "CoreMinimal.h"

#include "GsSummonCardSlotInfo.generated.h"

USTRUCT(BlueprintType)
struct T1PROJECT_API FGsSummonCardSlotInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Summon, Meta = (Tooltip = "slot Á¤º¸"))
	TArray<int> _slotInfo;
};