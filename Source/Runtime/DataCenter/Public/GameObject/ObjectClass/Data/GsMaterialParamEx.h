#pragma once

#include "Animation/AnimNotify/GsMaterialParam.h"
#include "GsMaterialParamEx.generated.h"

// 파라미터 구조체 정보
// 시간 정보가 필요하여 FGsMaterialParam 구조체를 상속받은 구조체 생성
USTRUCT(BlueprintType)
struct DATACENTER_API FGsMaterialParamEx : public FGsMaterialParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float TotalTime = 0.f;
};
