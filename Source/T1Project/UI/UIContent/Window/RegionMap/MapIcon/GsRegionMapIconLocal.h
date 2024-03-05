#pragma once
#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconObject.h"

#include "SlateCore/Public/Layout/Geometry.h"

#include "GsRegionMapIconLocal.generated.h"

/*
	지역맵 local
*/

UCLASS()
class T1PROJECT_API UGsRegionMapIconLocal : public UGsRegionMapIconObject 
{
	GENERATED_BODY()

	// 멤버 변수
private:
	// 로컬 회전 변했는지
	bool _localRotDirty = false;
	// 이전 회전값
	FRotator _oldLocalRot = FRotator::ZeroRotator;


	// 가상 함수
public:
	virtual void Tick(float In_delta)override;
	// 로직함수
public:
	// 회전 갱신
	void UpdateRotation();
};