// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStatisticsManager.h"
#include "Net/GsNetSendServiceWorld.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "DataSchema/Statistics/GsSchemaStatisticsData.h"


void FGsGameStatisticsManager::Initialize()
{
	if (const UGsTable* table = FGsSchemaStatisticsData::GetStaticTable())
	{
		TArray<const FGsSchemaStatisticsData*> rows;
		if (table->GetAllRows(rows))
		{
			for (const FGsSchemaStatisticsData* iter : rows)
			{
				FGsGameStatisticsData* data = _poolData.Claim();
				data->SetTable(iter);

				_dataMap.Emplace(data->_id, data);

				if (StatisticsDataList* listPtr = _tabMap.Find(iter->tabId))
				{
					listPtr->Emplace(data);
				}
				else
				{
					StatisticsDataList newList;
					newList.Emplace(data);

					_tabMap.Emplace(iter->tabId, newList);
				}
			}
		}
	}

	for (TPair<StatisticsTabId, StatisticsDataList>& itPair : _tabMap)
	{
		_checkerMap.Emplace(itPair.Key, FGsIntervalChecker(10)); // 자주받을 필요 없음
	}
}

void FGsGameStatisticsManager::Finalize()
{
	_startTime = 0;

	_tabMap.Empty();
	_dataMap.Empty();
	_poolData.RemovePool();

	_dailyDataStatisticsId = INVALID_STATISTICS_ID;
	_dailyDataList.Empty();

	_checkerMap.Empty();
}

void FGsGameStatisticsManager::ReleaseDataList(StatisticsDataList& InList)
{
	for (FGsGameStatisticsData* data : InList)
	{
		_poolData.Release(data);
	}

	InList.Empty();
}

void FGsGameStatisticsManager::SetStatisticsInfo(PD::SC::PKT_SC_ACK_STATISTICS_INFO_READ& InPacket)
{
	if (INVALID_STATISTICS_TAB_ID == InPacket.TabId())
	{
		return;
	}

	_startTime = InPacket.StatisticsTime();

	if (StatisticsDataList* dataList = _tabMap.Find(InPacket.TabId()))
	{
		for (FGsGameStatisticsData* iter : *dataList)
		{
			iter->ResetValue();
		}
	}

	StatisticsDataList dataList;

	TArray<StatisticsInfo> tempList;
	InPacket.MakeStatisticsInfoListVector<TArray<StatisticsInfo>, StatisticsInfo>(tempList);
	for (const StatisticsInfo& info : tempList)
	{
		if (FGsGameStatisticsData** data = _dataMap.Find(info.mStatisticsId))
		{
			(*data)->SetData(info);
		}
	}

	// 리셋
	_dailyDataStatisticsId = INVALID_STATISTICS_ID;
}

void FGsGameStatisticsManager::SetStatisticsDailyInfo(PD::SC::PKT_SC_ACK_STATISTICS_DAILY_INFO_READ& InPacket)
{
	_dailyDataStatisticsId = InPacket.StatisticsId();
	_dailyDataList.Empty();

	// 주의: 서버에서 7개 고정값으로 준다. 아닐경우에 대한 체크를 한다.
	InPacket.MakeDailyStatisticsValueListVector<TArray<StatisticsValue>, StatisticsValue>(_dailyDataList);
	if (DAY_OF_THE_WEEK_COUNT != _dailyDataList.Num())
	{
		// 7개가 아니면 앞쪽에 0값을 채움
		int emptyNum = DAY_OF_THE_WEEK_COUNT - _dailyDataList.Num();
		for (int i = 0; i < emptyNum; ++i)
		{
			_dailyDataList.Insert(0, 0);
		}
	}

	// 주의: 서버에서 최신정보를 줘서 그래프 정보는 계속 갱신되어 값이 서로 맞지 않게 나온다.
	// 따라서 오늘 정보는 계속 리스트에서 받았던 값으로 덮어써서 표현한다
	if (FGsGameStatisticsData** dataPtr = _dataMap.Find(_dailyDataStatisticsId))
	{
		// 시작일로부터의 차이값으로 7개의 값 중 오늘의 인덱스를 찾아낸다
		time_t diffSec = FGsTimeSyncUtil::GetServerNowSecond() - _startTime;
		FTimespan diffTime = FTimespan::FromSeconds(diffSec);
		int32 todayIndex = diffTime.GetDays();

		if (_dailyDataList.IsValidIndex(todayIndex))
		{
			_dailyDataList[todayIndex] = (*dataPtr)->_value;
		}
	}
}

bool FGsGameStatisticsManager::SendReqStatisticsInfo(StatisticsTabId InTabId)
{
	// 광클방지 쿨타임 체크
	if (FGsIntervalChecker* checker = _checkerMap.Find(InTabId))
	{
		if (checker->IsTimeOver())
		{
			FGsNetSendServiceWorld::SendReqStatisticsInfo(InTabId);
			checker->UpdateTime();
			return true;
		}
	}

	return false;
}

bool FGsGameStatisticsManager::SendReqStaticsDailyInfo(StatisticsId InId)
{
	// 다른 정보일 때만 요청
	if (InId != _dailyDataStatisticsId)
	{		
		FGsNetSendServiceWorld::SendReqStatisticsDailyInfo(InId);
		return true;
	}

	return false;
}

const TArray<FGsGameStatisticsData*> FGsGameStatisticsManager::GetDataList(StatisticsTabId InTabId) const
{
	if (const StatisticsDataList* list = _tabMap.Find(InTabId))
	{
		return *list;
	}

	// 빈 리스트로 리턴
	StatisticsDataList out;
	return out;
}

bool FGsGameStatisticsManager::GetDailyDataList(StatisticsId InId, OUT TArray<uint64>& OutList)
{
	if (_dailyDataStatisticsId != InId)
	{
		for (int i = 0; i < DAY_OF_THE_WEEK_COUNT; ++i)
		{
			OutList.Emplace(0);
		}

		return false;
	}

	OutList.Append(_dailyDataList);
	return true;
}

bool FGsGameStatisticsManager::GetDateTime(OUT FDateTime& OutStartDate, OUT FDateTime& OutEndDate)
{
	if (0 == _startTime)
	{
		return false;
	}

	OutStartDate = FGsTimeSyncUtil::ConvertToDateTime(_startTime);
	OutEndDate = OutStartDate + FTimespan::FromDays(7);

	return true;
}
