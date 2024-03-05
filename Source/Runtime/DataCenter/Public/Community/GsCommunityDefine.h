#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsCommunityDefine.generated.h"

UENUM(BlueprintType)
// 커뮤니티 이벤트 타임 타입
enum class ECommunityEventTimeType : uint8
{
	// 상시
	ALWAYS = 0,

	// 기간
	PERIOD = 1,

	// 유동적
	CONDITION = 2,

	MAX = 3,
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGsEventTimeData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString end;
};