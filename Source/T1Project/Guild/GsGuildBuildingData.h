// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsIntervalChecker.h"


/**
 * 길드 버프 정보
 */
struct T1PROJECT_API FGsGuildPassivityData final
{
public:
	PassivityId _id = 0;
	time_t _buffEndTime = 0; // 서버기준시. 활성화된 버프가 끝나는 시간
	//FDateTime _buffReActivateTime; // 서버기준시. 버프가 재활성화되는 시간 현재없음
};

struct FGsSchemaGuildBuilding;

/**
 * 길드 건물 정보
 */
struct T1PROJECT_API FGsGuildBuildingData final
{
public:
	GuildBuildingId _id;	
	GuildBuildingStateType _buildingState;

	// 업그레이드 종료시간
	time_t _upgradeFinishTime;

	// 버프가 아닌 스탯타입이어도 일단 passivity값이 있으면 온다. (time 비워서)
	TArray<FGsGuildPassivityData> _passivityList;

	const FGsSchemaGuildBuilding* _tableData;

	// 마지막 레벨업 시간
	time_t _lastLevelUpTime = 0;

private:
	GuildBuildingLevel _level;

public:
	FGsGuildBuildingData() = default;

	void Set(const GuildBuildingData& InData);
	void SetUpgradeLevel(GuildBuildingLevel InNextLevel);
	void SetActiveGuildBuff(PassivityId InPassivityId, float InCoolDownTime);

	int32 GetPassivityCount() const { return _passivityList.Num(); }
	PassivityId GetPassivityId(int32 InIndex);

	// 버프가 활성화 중인가
	bool IsBuffActive() const;
	// 건설된 건물인가(건설되지 않았거나 건설중이면 false, 레벨업 중엔 true)
	bool IsBuildingOpened() const;
	GuildBuildingLevel GetCurrBuildingLevel() const;
	
	float GetBuffRemainTime() const;
	// 버프 지속시간
	float GetBuffDuration() const;
};
