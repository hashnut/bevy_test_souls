#pragma once
#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconObject.h"

#include "Engine/Classes/Engine/EngineTypes.h"
#include "SlateCore/Public/Layout/Geometry.h"

#include "GsRegionMapIconOther.generated.h"

/*
	지역맵 서버스폰 타유저
*/

UCLASS()
class T1PROJECT_API UGsRegionMapIconOther : public UGsRegionMapIconObject
{
	GENERATED_BODY()

	// 가상 함수
public:
	virtual void Tick(float In_delta) override;
	// 타이머에의한 업데이트
	virtual void UpdateTimmer() override;
};