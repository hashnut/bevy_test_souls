#pragma once
#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconBase.h"

#include "Map/GsMapIconType.h"

#include "GsRegionMapIconObject.generated.h"

/*
	지역맵 object 단위
*/

class UGsGameObjectBase;
class UGsUIIconBase;

UCLASS()
class T1PROJECT_API UGsRegionMapIconObject :public UGsRegionMapIconBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UGsGameObjectBase* _targetObject = nullptr;

	// 가상 함수
public:
	// 정리
	virtual void Finalize() override;
	virtual FVector2D GetPos() override;

	// 로직 함수
public:
	// 타겟 오브젝트 세팅
	void SetTargetObject(UGsGameObjectBase* In_targetObj);
};