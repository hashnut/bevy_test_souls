#pragma once

#include "CoreMinimal.h"
#include "PKBook/GsPKBookStruct.h"
#include "Map/GsMapIconType.h"
#include "../Map/Minimap/MapIcon/GsMinimapIconObject.h"
#include "GsMinimapIconDetectionTarget.generated.h"


/**
* file		GsMinimapIconDetectionTarget.h
* @brief	PK 관련 추적 아이템
* @author	PKT
* @date		2022/09/29
**/
UCLASS()
class T1PROJECT_API UGsMinimapIconDetectionTarget : public UGsMinimapIconObject
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	class UGsGameObjectLocalPlayer* _localPlayer = nullptr;

	TArray<FGsPKDetectionEffectInfo> _effectInfoSet;

	float _limitMaxDistance = 0.f;

	float _distance = 0.f;

	float _currentEffectTick = 0.f;

public:
	// 2022/09/29 PKT - 클리어
	virtual void Finalize() override;
	// 2022/09/29 PKT - Target Pos
	virtual FVector2D GetPos() override;
	// 2022/09/30 PKT - Tick
	virtual void Tick(float InDelta) override;
protected:
	// 2022/09/30 PKT - 타겟과의 거리에 의해 차등 효과를 적용 시킨다.
	void ApplyEffect(float InDelta);
	
public:
	void SetData(
		class UGsGameObjectBase* InTargetObj
		, class UGsUIIconBase* InIconWidget
		, class UGsMinimapSceneImage* InSceneImage
		, EGsMapIconType InIconType
		, float InUpdateTime
		, int64 InGameId
		, int32 InDepth
		, const TArray<FGsPKDetectionEffectInfo>& InEffectInfoSet);
};