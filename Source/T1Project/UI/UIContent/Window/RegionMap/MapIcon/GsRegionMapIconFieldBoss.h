#pragma once
#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconFixed.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "GameObject/Boss/GsFieldBossHandler.h"

#include "GsRegionMapIconFieldBoss.generated.h"

/*
	지역맵 필드 보스 아이콘
*/

UCLASS()
class T1PROJECT_API UGsRegionMapIconFieldBoss : public UGsRegionMapIconFixed
{
	GENERATED_BODY()

private:
	// 스폰 시간
	FDateTime _spawnTime;
	// 보스 상태
	EGsRegionFieldBossState _bossState = EGsRegionFieldBossState::NotSpawn;
	// 이전 초
	float _oldTotalSecond;
	// 스폰 시간 있는지
	bool _isHaveSpawnTime = false;

	// 가상 함수
public:
	virtual void Tick(float In_deltal) override;

	// 로직 함수
public:
	// 남은시간 갱신
	void UpdateRemainTime(bool In_isForced = false);
	// 상태 갱신
	void UpdateState(bool In_isForeced = false);

	// set
public:
	void SetSpawnTime(bool In_isHaveSpawnTime , FDateTime In_time);
	// 보스 상태 스위칭
	void SetBossState(EGsRegionFieldBossState In_bossState);
	// 남은 시간 세팅
	void SetRemainTime(FText In_remainTime);

	EGsRegionFieldBossState GetUIState(EGsFieldBossState In_state);

	void SetClickCallback(TFunction<void()> In_callback);
};