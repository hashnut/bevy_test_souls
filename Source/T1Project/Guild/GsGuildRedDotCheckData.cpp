// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildRedDotCheckData.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Guild/GsGuildStorageData.h"
#include "T1Project.h"


void FGsGuildRedDotCheckData::Empty()
{
	_noticeCheckTime = 0;
	_buffCheckTime = 0;
	_buildingCheckTime = 0;
	_questCheckTime = 0;
	_diplomacyCheckTime = 0;	
	_invenCheckTime = 0;
	_acquiredCheckTime = 0;
	_distributedCheckTime = 0;

	_diplomacyEnemyCheckTime = 0;
	_diplomacyAllyCheckTime = 0;
	_diplomacyStateCheckTime = 0;

	_auctionCheckTime = 0;
	_divisionCheckTime = 0;
	_calculateCheckTime = 0;

	_sanctumCheckTime = 0;
}

void FGsGuildRedDotCheckData::SetData(const GuildRedDotDetail& InData)
{
	_noticeCheckTime = InData.mNoticeCheckTime;
	_buffCheckTime = InData.mBuffCheckTime;
	_buildingCheckTime = InData.mBuildingCheckTime;
	_questCheckTime = InData.mQuestCheckTime;

	// 편의상 나눠서 저장했음
	_diplomacyCheckTime = InData.mDiplomacyCheckTime;
	_diplomacyEnemyCheckTime = InData.mDiplomacyCheckTime;
	_diplomacyAllyCheckTime = InData.mDiplomacyCheckTime;
	_diplomacyStateCheckTime = InData.mDiplomacyCheckTime;

	_auctionCheckTime = InData.mInvenCheckTime;
	_divisionCheckTime = InData.mAcquiredCheckTime;
	_calculateCheckTime = InData.mDistributedCheckTime;

	_sanctumCheckTime = InData.mSanctumCheckTime;

#if 0
#if WITH_EDITOR
	FDateTime testNoticeTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_noticeCheckTime);
	FDateTime testBuffTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_buffCheckTime);
	FDateTime testBuildingTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_buildingCheckTime);
	FDateTime testQuestTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_questCheckTime);
	FDateTime testDiplomacyTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_diplomacyCheckTime);
	FDateTime testInvenTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_auctionCheckTime);
	FDateTime testAcquiredTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_divisionCheckTime);
	FDateTime testDistributedTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_calculateCheckTime);
	FDateTime testSanctumTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_sanctumCheckTime);

	GSLOG(Log, TEXT("[REDDOT] Rcv Notice: %s"), *(testNoticeTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Rcv Buff: %s"), *(testBuffTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Rcv Building: %s"), *(testBuildingTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Rcv Quest: %s"), *(testQuestTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Rcv Diplomacy: %s"), *(testDiplomacyTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Rcv Inven: %s"), *(testInvenTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Rcv Acquired: %s"), *(testAcquiredTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Rcv Distributed: %s"), *(testDistributedTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Rcv Sanctum: %s"), *(testSanctumTime.ToString()));
#endif
#endif
}

void FGsGuildRedDotCheckData::SetRedDotDetailData(OUT GuildRedDotDetail& OutData)
{
	OutData.mNoticeCheckTime = _noticeCheckTime;
	OutData.mBuffCheckTime = _buffCheckTime;
	OutData.mBuildingCheckTime = _buildingCheckTime;
	OutData.mQuestCheckTime = _questCheckTime;	
	OutData.mDiplomacyCheckTime = _diplomacyCheckTime;
	OutData.mInvenCheckTime = _auctionCheckTime;
	OutData.mAcquiredCheckTime = _divisionCheckTime;
	OutData.mDistributedCheckTime = _calculateCheckTime;
	OutData.mSanctumCheckTime = _sanctumCheckTime;

#if 0
#if WITH_EDITOR
	FDateTime testNoticeTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_noticeCheckTime);
	FDateTime testBuffTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_buffCheckTime);
	FDateTime testBuildingTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_buildingCheckTime);
	FDateTime testQuestTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_questCheckTime);
	FDateTime testDiplomacyTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_diplomacyCheckTime);
	FDateTime testInvenTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_auctionCheckTime);
	FDateTime testAcquiredTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_divisionCheckTime);
	FDateTime testDistributedTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_calculateCheckTime);
	FDateTime testSanctumTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_sanctumCheckTime);

	GSLOG(Log, TEXT("[REDDOT] Send Notice: %s"), *(testNoticeTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Send Buff: %s"), *(testBuffTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Send Building: %s"), *(testBuildingTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Send Quest: %s"), *(testQuestTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Send Diplomacy: %s"), *(testDiplomacyTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Send Inven: %s"), *(testInvenTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Send Acquired: %s"), *(testAcquiredTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Send Distributed: %s"), *(testDistributedTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] Send Sanctum: %s"), *(testSanctumTime.ToString()));
#endif
#endif
}

void FGsGuildRedDotCheckData::UpdateNoticeCheckTime()
{
	_noticeCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildRedDotCheckData::UpdateBuffCheckTime()
{
	_buffCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildRedDotCheckData::UpdateBuildingCheckTime()
{
	_buildingCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildRedDotCheckData::UpdateQuestCheckTime()
{
	_questCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildRedDotCheckData::UpdateDiplomacyEnemyCheckTime()
{
	_diplomacyEnemyCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
	_diplomacyCheckTime = _diplomacyEnemyCheckTime;
}

void FGsGuildRedDotCheckData::UpdateDiplomacyAllyCheckTime()
{
	_diplomacyAllyCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
	_diplomacyCheckTime = _diplomacyAllyCheckTime;
}

void FGsGuildRedDotCheckData::UpdateDiplomacyStateCheckTime()
{
	_diplomacyStateCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
	_diplomacyCheckTime = _diplomacyStateCheckTime;
}

void FGsGuildRedDotCheckData::UpdateStorageCheckTime(const FGsGuildStorageData* InData)
{
	//_invenCheckTime = InData->_lastInvenCheckTime;
	//_acquiredCheckTime = InData->_lastAcquiredCheckTime;
	//_distributedCheckTime = InData->_lastDistributedCheckTime;
}

void FGsGuildRedDotCheckData::UpdateAcquisitionCheckTime()
{
	_acquisitionCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildRedDotCheckData::UpdateDivisionCheckTime()
{
	_divisionCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildRedDotCheckData::UpdateAuctionCheckTime()
{
	_auctionCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildRedDotCheckData::UpdateCalculateCheckTime()
{
	_calculateCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildRedDotCheckData::UpdateSanctumCheckTime()
{
	_sanctumCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
}
