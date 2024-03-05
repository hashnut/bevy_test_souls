// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsGuildStorageData;

/**
 * 길드 레드닷 체크를 위한 시간 정보
 */
struct T1PROJECT_API FGsGuildRedDotCheckData final
{
public:
	// 서버에 저장하는 시간 목록
	time_t _noticeCheckTime = 0;
	time_t _buffCheckTime = 0;
	time_t _buildingCheckTime = 0;
	time_t _questCheckTime = 0;
	time_t _diplomacyCheckTime = 0;
	time_t _divisionCheckTime = 0; // 분배탭 확인 시각
	time_t _auctionCheckTime = 0; // 경매탭 확인 시각
	time_t _calculateCheckTime = 0; // 정산탭 확인 시각
	time_t _sanctumCheckTime = 0; // 성소탭 확인 시각

	// 편의를 위해 확장해서 쓰는 항목
public:
	time_t _diplomacyEnemyCheckTime = 0;
	time_t _diplomacyAllyCheckTime = 0;
	time_t _diplomacyStateCheckTime = 0;
	time_t _acquisitionCheckTime = 0;

	time_t _invenCheckTime = 0;
	time_t _acquiredCheckTime = 0;
	time_t _distributedCheckTime = 0;

public:
	void Empty();
	void SetData(const GuildRedDotDetail& InData);
	void SetRedDotDetailData(OUT GuildRedDotDetail& OutData);

public:
	void UpdateNoticeCheckTime();
	void UpdateBuffCheckTime();
	void UpdateBuildingCheckTime();	
	void UpdateQuestCheckTime();
	void UpdateDiplomacyEnemyCheckTime();
	void UpdateDiplomacyAllyCheckTime();
	void UpdateDiplomacyStateCheckTime();
	void UpdateStorageCheckTime(const FGsGuildStorageData* InData);
	void UpdateAcquisitionCheckTime();
	void UpdateDivisionCheckTime();
	void UpdateAuctionCheckTime();
	void UpdateCalculateCheckTime();
	void UpdateSanctumCheckTime();
};
