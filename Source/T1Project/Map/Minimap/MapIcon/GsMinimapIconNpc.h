#pragma once
#include "CoreMinimal.h"
#include "Map/Minimap/MapIcon/GsMinimapIconBase.h"
#include "Map/GsMapIconType.h"
#include "Engine/Classes/Engine/EngineTypes.h"
#include "GsMinimapIconNpc.generated.h"

/*
	npc 아이콘 처리
	1. 고정형(이동하지 않음)
*/
class UGsUIIconBase;
class UGsMinimapSceneImage;

UCLASS()
class T1PROJECT_API UGsMinimapIconNpc : public UGsMinimapIconBase
{
	GENERATED_BODY()

	// 멤버 변수
private:
	// 타이머
	FTimerHandle _timerHandle;
	// npc 위치
	FVector2D _npcPos;
	// 테이블 id
	int _tableId;
	// 가상 함수
public:
	// 위치 구하기
	virtual FVector2D GetPos() override;
	// 클리어
	virtual void Finalize() override;
	// 테이블 아이디 구하기
	virtual int GetTableId() override
	{
		return _tableId;
	}

	// 로직 함수
public:
	void Initialize(UGsUIIconBase* In_iconWidget,
		EGsMapIconType In_iconType,
		float In_updateTime,
		UGsMinimapSceneImage* In_sceneImage,
		int In_depth, int64 In_gameId, FVector2D In_pos, 
		int In_tableId);

	// 타이머에의한 업데이트
	void UpdateTimmer();

};