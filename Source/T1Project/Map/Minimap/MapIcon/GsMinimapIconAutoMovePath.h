#pragma once

#include "CoreMinimal.h"
#include "Map/Minimap/MapIcon/GsMinimapIconBase.h"
#include "Map/GsMapIconType.h"
#include "GsMinimapIconAutoMovePath.generated.h"

// 자동 이동 경로 표시 아이콘

class UGsUIIconBase;
class UGsMinimapSceneImage;

UCLASS()
class T1PROJECT_API UGsMinimapIconAutoMovePath : public UGsMinimapIconBase
{
	GENERATED_BODY()

	// 멤버 변수
private:
	// 타이머
	FTimerHandle _timerHandle;
	// 경로 위치
	FVector2D _pathPos;

	// 삭제할 거리(로컬과의 거리)
	float _deleteDist;
	// 삭제할 제곱 거리
	float _deleteSquaredDist;

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
		int In_depth, FVector2D In_pos,
		float In_deleteDist);

	// 타이머에의한 업데이트
	void UpdateTimmer();

	// get
public:
	// 아이콘 삭제해야하는가
	bool IsDeleteIcon(FVector2D In_localPos);
};