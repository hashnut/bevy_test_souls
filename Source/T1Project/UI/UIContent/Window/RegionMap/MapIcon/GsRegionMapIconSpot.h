#pragma once
#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconFixed.h"

#include "GsRegionMapIconSpot.generated.h"

/*
  지역맵 스팟 아이콘
*/
UCLASS()
class T1PROJECT_API UGsRegionMapIconSpot : public UGsRegionMapIconFixed
{
    GENERATED_BODY()

    // 로직 함수
public:
	// 이름 세팅
	void SetSpotName(FText In_name);
};