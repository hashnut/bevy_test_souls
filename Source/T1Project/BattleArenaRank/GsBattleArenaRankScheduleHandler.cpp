#include "GsBattleArenaRankScheduleHandler.h"
#include "../UTIL/GsTimeSyncUtil.h"
#include "../Data/GsGlobalConstant.h"
#include "../Data/GsDataContainManager.h"
#include "../Management/ScopeGame/GsBattleArenaSeasonRankManager.h"
#include "../Net/GsNetSendService.h"
#include "T1Project.h"



void FGsBattleArenaRankScheduleHandler::Initialize()
{
	_scheduleSet.Empty();
	_pendingDataSet.Empty();

	_currentDelay = 0.f;
}

void FGsBattleArenaRankScheduleHandler::Finalize()
{
	_scheduleSet.Empty();
	_pendingDataSet.Empty();

	_currentDelay = 0.f;
}

void FGsBattleArenaRankScheduleHandler::Update(float inTick)
{
	UpdateSchedule(inTick);
	UpdatePending(inTick);
}

void FGsBattleArenaRankScheduleHandler::Push(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, const TArray<int32>& InIndexSet)
{
	FGsBattleArenaScheduler schedule;
	schedule._rankSearchType = InRankSearchType;
	schedule._tierDataId = InTierDataId;
	schedule._indexSet = InIndexSet;
	schedule._precessFunc = [](const FGsBattleArenaScheduler& InData)
	{
		FGsNetSendService::SendReqBattleArenaRankInfo(InData._tierDataId, InData._indexSet);
	};
	_scheduleSet.Push(schedule);

	auto StateChange = [InTierDataId, InIndexSet](TMap<GuildWarTierDataId, FGsBattleArenaRankTierData>& InList)
	{
		if (InList.Contains(InTierDataId))
		{
			for (int32 item : InIndexSet)
			{
				if (InList[InTierDataId]._items.IsValidIndex(item))
				{
					InList[InTierDataId]._items[item]._isValidate = ValidRankDataState::Pending;
				}
			}
		}
	};

	StateChange(RankSearchType::KNIGHTS == InRankSearchType ? GsBattleArenaRank()->_guildRankDataSet : GsBattleArenaRank()->_personRankDataSet);
}

void FGsBattleArenaRankScheduleHandler::ClearAll(RankSearchType InRankSearchType)
{
	if (RankSearchType::MAX == InRankSearchType)
	{
		_scheduleSet.Empty();
		_pendingDataSet.Empty();

		_currentDelay = 0.f;
	}
	else
	{
		for (int32 idx = 0; idx < _scheduleSet.Num(); ++idx)
		{
			if (InRankSearchType == _scheduleSet[idx]._rankSearchType)
			{
				_scheduleSet.RemoveAt(idx);	--idx;
				continue;
			}
		}

		for (int32 idx = 0; idx < _pendingDataSet.Num(); ++idx)
		{
			if (InRankSearchType == _pendingDataSet[idx]._rankSearchType)
			{
				_pendingDataSet.RemoveAt(idx); --idx;
				continue;
			}
		}
	}
}

void FGsBattleArenaRankScheduleHandler::RemoveAll(RankSearchType InRankSearchType)
{
	for (int32 idx = 0; idx < _scheduleSet.Num(); ++idx)
	{
		if (InRankSearchType == _scheduleSet[idx]._rankSearchType)
		{
			_scheduleSet.RemoveAt(idx);	--idx;
			continue;
		}
	}
}

void FGsBattleArenaRankScheduleHandler::UpdateSchedule(float InTick)
{
	if (0 < _scheduleSet.Num())
	{
		_currentDelay -= InTick;
		if (0.f >= _currentDelay)
		{
			_currentDelay = 0.f;

			const static int32 SEND_INDEX = 0;
			if (_scheduleSet.IsValidIndex(SEND_INDEX) && _scheduleSet[SEND_INDEX]._precessFunc)
			{	// 2024/1/29 PKT - Send Packet
				_scheduleSet[SEND_INDEX]._precessFunc(_scheduleSet[SEND_INDEX]);
				// 2024/1/29 PKT - Send Data Pending
				AddPendingData(_scheduleSet[SEND_INDEX]);
				// 2024/1/29 PKT - Delete
				_scheduleSet.RemoveAt(SEND_INDEX);
				// 2024/1/29 PKT - Reset Schedule

				float delay = 0.55f;
				if (UGsGlobalConstant* globalConstant = GData()->GetGlobalData())
				{
					delay = globalConstant->_battleArenaRankSendDelayTimeS;
				}
				_currentDelay = delay;
			}
		}
	}

	if (0 >= _scheduleSet.Num())
	{
		_currentDelay = 0.f;
	}
}

void FGsBattleArenaRankScheduleHandler::UpdatePending(float InTick)
{
	auto StateChange = [](GuildWarTierDataId InTierDBId, int32 InIndex, TMap<GuildWarTierDataId, FGsBattleArenaRankTierData>& InList)
	{
		if (InList.Contains(InTierDBId))
		{
			if (InList[InTierDBId]._items.IsValidIndex(InIndex) && ValidRankDataState::Validate != InList[InTierDBId]._items[InIndex]._isValidate)
			{
				InList[InTierDBId]._items[InIndex]._isValidate = ValidRankDataState::Invalidate;
			}
		}
	};

	int32 peddingLifeTime = 3;
	if (UGsGlobalConstant* globalConstant = GData()->GetGlobalData())
	{
		peddingLifeTime = globalConstant->_battleArenaPendingLifeTimeS;
	}
	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();

	for (int32 idx = 0; idx < _pendingDataSet.Num(); ++idx)
	{
		if (peddingLifeTime <= currentTime - _pendingDataSet[idx]._time)
		{
			StateChange(_pendingDataSet[idx]._tierDataId
				, _pendingDataSet[idx]._index
				, RankSearchType::KNIGHTS == _pendingDataSet[idx]._rankSearchType ? GsBattleArenaRank()->_guildRankDataSet : GsBattleArenaRank()->_personRankDataSet);
			_pendingDataSet.RemoveAt(idx); --idx;
			continue;
		}
	}
}

void FGsBattleArenaRankScheduleHandler::AddPendingData(const FGsBattleArenaScheduler& InData)
{
	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
	RankSearchType rankSearchType = InData._rankSearchType;
	GuildWarTierDataId tierDataId = InData._tierDataId;

	for (int32 item : InData._indexSet)
	{
		FGsBattleArenaSchedulePendingData data;
		data._time = FGsTimeSyncUtil::GetServerNowSecond();
		data._rankSearchType = rankSearchType;
		data._tierDataId = tierDataId;
		data._index = item;

		_pendingDataSet.AddUnique(data);
	}
}