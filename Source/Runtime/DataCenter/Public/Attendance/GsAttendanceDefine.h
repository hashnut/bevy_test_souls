#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsAttendanceDefine.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGsAttendanceExpirationData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString end;
};