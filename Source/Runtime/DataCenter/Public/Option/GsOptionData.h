#pragma once

#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsOptionData.generated.h"

USTRUCT(BlueprintType)
struct DATACENTER_API FGsNoneEquipItemCategoryOptionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	ItemCategorySub itemCategorySub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	ItemGrade minGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	ItemGrade maxGrade;
};