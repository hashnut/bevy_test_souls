#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsItemCollectionCommonData.generated.h"


USTRUCT(BlueprintType)
struct DATACENTER_API FGsItemCollectionSubSituteItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubSituteItemId")
	int32 itemId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubSituteEnchantLevcel")
	int32 enchantLevel = 0;
};




