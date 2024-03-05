// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"
#include "Classes/GsPool.h"
#include "Management/GsScopeHolder.h"
#include "GameStatistics/GsGameStatisticsData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "UTIL/GsIntervalChecker.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"


/**
 * 통계 시스템
 */
class T1PROJECT_API FGsGameStatisticsManager final : public IGsManager
{
public:
	using StatisticsDataList = TArray<FGsGameStatisticsData*>;

private:
	time_t _startTime = 0;
	TGsPoolDefault<FGsGameStatisticsData> _poolData;
	TMap<StatisticsId, FGsGameStatisticsData*> _dataMap; // 원본
	TMap<StatisticsTabId, StatisticsDataList> _tabMap; // 편의를 위해 포인터 저장
	
	
	StatisticsId _dailyDataStatisticsId = INVALID_STATISTICS_ID;
	TArray<uint64> _dailyDataList; // DAY_OF_THE_WEEK_COUNT 갯수 고정

	TMap<StatisticsTabId, FGsIntervalChecker> _checkerMap;

public:
	FGsGameStatisticsManager() = default;
	virtual ~FGsGameStatisticsManager() = default;

public:
	// IGsManager override
	virtual void Initialize() override;
	virtual void Finalize() override;

private:
	void ReleaseDataList(StatisticsDataList& InList);

public:
	void SetStatisticsInfo(PD::SC::PKT_SC_ACK_STATISTICS_INFO_READ& InPacket);
	void SetStatisticsDailyInfo(PD::SC::PKT_SC_ACK_STATISTICS_DAILY_INFO_READ& InPacket);
	
	// 체크 후 이미 데이터가 있으면 보내지않고 false 리턴
	bool SendReqStatisticsInfo(StatisticsTabId InTabId);
	bool SendReqStaticsDailyInfo(StatisticsId InId);

public:
	const TArray<FGsGameStatisticsData*> GetDataList(StatisticsTabId InTabId) const;
	bool GetDailyDataList(StatisticsId InId, OUT TArray<uint64>&OutList);
	bool GetDateTime(OUT FDateTime& OutStartDate, OUT FDateTime& OutEndDate);
};

#define GSGameStatistics() UGsScopeHolder::GetGameManagerFType<FGsGameStatisticsManager>(UGsScopeGame::EManagerType::GameStatistics)
