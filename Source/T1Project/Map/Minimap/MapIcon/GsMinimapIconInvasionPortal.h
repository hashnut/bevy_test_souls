#pragma once

#include "CoreMinimal.h"
#include "Map/Minimap/MapIcon/GsMinimapIconBase.h"
#include "Map/GsMapIconType.h"
#include "Engine/Classes/Engine/EngineTypes.h"
#include "GsMinimapIconInvasionPortal.generated.h"

/*
* invasion portal icon
*/

class UGsUIIconBase;
class UGsMinimapSceneImage;

UCLASS()
class T1PROJECT_API UGsMinimapIconInvasionPortal : public UGsMinimapIconBase
{
	GENERATED_BODY()

		// 멤버 변수
private:
	// 타이머
	FTimerHandle _timerHandle;
	// portal 위치
	FVector2D _portalPos;
	
	// 가상 함수
public:
	// 위치 구하기
	virtual FVector2D GetPos() override;
	// 클리어
	virtual void Finalize() override;
	

	// 로직 함수
public:
	void Initialize(UGsUIIconBase* In_iconWidget,
		EGsMapIconType In_iconType,
		float In_updateTime,
		UGsMinimapSceneImage* In_sceneImage,
		int In_depth, FVector2D In_pos);

	// 타이머에의한 업데이트
	void UpdateTimmer();

};