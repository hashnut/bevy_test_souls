#pragma once
#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconBase.h"

#include "GsRegionMapIconFixed.generated.h"

/*
	지역맵 고정형
*/

UCLASS()
class T1PROJECT_API UGsRegionMapIconFixed : public UGsRegionMapIconBase
{
	GENERATED_BODY()

	// 멤버 변수
private:
	// 액터 위치
	FVector2D _actorPos;

	// 가상 함수
public:
	virtual FVector2D GetPos() override;

	// set
public:
	void SetActorPos(FVector2D In_pos)
	{
		_actorPos = In_pos;
	}
};