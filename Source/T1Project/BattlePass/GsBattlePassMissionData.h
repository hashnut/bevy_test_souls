#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassMissionTab.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassMissionCommon.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassDetail.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassTabDetail.h"
#include "UTIL/GsTimeSyncUtil.h"

struct T1PROJECT_API FGsBattlePassMissionData
{
	const FGsSchemaBattlePassMissionCommon* _missionData;
	const FGsSchemaBattlePassDetail* _detail{ nullptr };
	const FGsSchemaBattlePassDetail* _rewardDetail{ nullptr };

	BattlePassMissionId	 _id{ INVALID_BATTLE_PASS_ID };
	uint64 _achievePoint{ 0 };

	BattlePassMissionId GetMissionId() { return _id; }

	bool IsCompleteMission() { return _achievePoint >= GetCompletePoint(); }
	uint64 GetAchievePoint() { return _achievePoint; }
	uint64 GetCompletePoint() { return _missionData != nullptr ? _missionData->passMissionCount : 0; }
	int64 GetPassMissionRewardExp() { return _missionData != nullptr ? _missionData->passMissionRewardExp : 0;  }
	FText GetMissionName()  { return _detail ? _detail->name : FText::GetEmpty(); } //detail Ã³¸®
	FText GetMissionExpDecription() { return _detail ? _detail->desc : FText::GetEmpty(); }
	FSoftObjectPath GetMissionIcon() { return _detail != nullptr ? _detail->icon : FSoftObjectPath(); }

	FSoftObjectPath GetMissionRewardIcon() { return _rewardDetail != nullptr ? _rewardDetail->icon : FSoftObjectPath();  }
	FText GetMissionRewardName()		{ return _rewardDetail ? _rewardDetail->name : FText::GetEmpty(); }
	FText GetMissionRewardDescription() { return _rewardDetail ? _rewardDetail->desc : FText::GetEmpty(); }

	FGsBattlePassMissionData() = default;
	FGsBattlePassMissionData(const FGsSchemaBattlePassMissionCommon* data) 
		: _missionData(data)
	{
		if (data)
		{
			_id = data->passMissionId;
			_detail = data->detail.GetRow();
			_rewardDetail = data->rewardDetail.GetRow();
		}
	}

	void SetAchieveMissionPoint(uint64 achievePoint)
	{
		_achievePoint = achievePoint;
	}

	void ResetMission() { _achievePoint = 0; }
};

struct T1PROJECT_API FGsBattlePassMissionTab
{
	const FGsSchemaBattlePassMissionTab* _data{ nullptr };
	const FGsSchemaBattlePassTabDetail* _tabDetail{ nullptr };

	bool  _isCompleteTab{ false };
	bool  _openPass{ false };
	uint64 _completeExp{ 0 };
	FText _missionTabDesc;

	TArray<TWeakPtr<FGsBattlePassMissionData>> _missions;

	FGsBattlePassMissionTab() = default;
	FGsBattlePassMissionTab(const FGsSchemaBattlePassMissionTab* tabData);

	virtual ~FGsBattlePassMissionTab()
	{
		_missions.Reset();
	}

	int64	GetTabId()		{ return _data ? _data->tabId : 0; }
	uint8	GetTabIndex()		{ return _tabDetail ? _tabDetail->tabIndex - 1 : 0; }
	FText	GetMissionTabName() { return _tabDetail ? _tabDetail->tabName : FText::GetEmpty(); }
	FText	GetMissionTabDesc() { return _missionTabDesc.IsEmpty() == false ? _missionTabDesc : _tabDetail ? _tabDetail->tabDesc : FText::GetEmpty(); }
	ResetTimeTicket GetMissionTabResetType() { return _data ? _data->resetType : ResetTimeTicket::RTT_NONE;  }
	uint64	GetTabCurrentExp();
	bool	IsOpenTab();
	bool	IsMissionCompleteTab();
	
	int GetMissionData(OUT TArray<TWeakPtr<FGsBattlePassMissionData>>& missions);
	TWeakPtr<FGsBattlePassMissionData> GetMission(BattlePassMissionId missionId);

	void SetPassOpenNotiToMissionTab() { _openPass = true; };
	void UpdateMissionTab();
	void OnMissionInfoChanged(BattlePassMissionId id, uint64 missionPoint);

	void ResetMissionTab();
};