#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMission.h"
#include "DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMissionCommon.h"
#include "DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMissionTab.h"
#include "Reward/GsRewardHelper.h"
#include "UTIL/GsTableUtil.h"
#include "T1Project.h"


struct T1PROJECT_API FGsCommunityEventMissionCommon
{
	CommunityEventMissionCommonId _missionCommonId = INVALID_COMMUNITY_EVENT_MISSION_COMMON_ID;

	const FGsSchemaCommunityEventMissionCommon* _schemaMissionCommon = nullptr;
	int32 _achievePoint = 0;
	bool _isRewarded = false;
	FGsRewardUnboxingData _rewardData;
	int8 _commonIndex = 0;

public:
	FGsCommunityEventMissionCommon() = default;
	explicit FGsCommunityEventMissionCommon(CommunityEventMissionCommonId inId, int32 inCount, bool inIsRewarded, int8 inCommonIndex)
		:_missionCommonId(inId),
		_achievePoint(inCount),
		_isRewarded(inIsRewarded),
		_commonIndex(inCommonIndex)
	{
	}

public:
	void ApplyData()
	{
		_schemaMissionCommon = UGsTableUtil::FindRowById<UGsTableCommunityEventMissionCommon, FGsSchemaCommunityEventMissionCommon>(_missionCommonId);
		if (nullptr == _schemaMissionCommon)
		{
#if WITH_EDITOR
			GSLOG(Error, TEXT("[CommunityEventMissionCommon] CommunityEventMissionCommon nullptr = Id: %d"), _missionCommonId);
#endif
			return;
		}
		
		if (const FGsSchemaRewardData* rewardTable = _schemaMissionCommon->rewardId.GetRow())
		{
			FGsRewardHelper::UnboxingRewardData(rewardTable->id, _rewardData);
		}
	}

	CommunityEventMissionCommonId GetMissionCommonId() { return _schemaMissionCommon ? _schemaMissionCommon->id : 0; }
	bool IsCompleteMission() { return _achievePoint >= GetCompletePoint(); }
	uint64 GetAchievePoint() { return _achievePoint; }
	uint64 GetCompletePoint() { return _schemaMissionCommon != nullptr ? _schemaMissionCommon->eventMissionCount : 0; }
	FText GetMissionCommonText() { return _schemaMissionCommon ? _schemaMissionCommon->eventMissionText : FText::GetEmpty(); }
	bool IsRedDot() 
	{
		if (nullptr == _schemaMissionCommon)
			return false;
		return false == _isRewarded && _achievePoint == _schemaMissionCommon->eventMissionCount;
	}
	bool IsRewarded() { return _isRewarded; }
	int8 GetCommonIndex() { return _commonIndex; }

	FGsRewardUnboxingData& GetRewardData() { return _rewardData; }

	void SetAchieveMissionPoint(uint64 achievePoint)
	{
		_achievePoint = achievePoint;
	}

	void SetIsRewarded(bool inIsRewarded) { _isRewarded = inIsRewarded; }

	void ResetMission() { _achievePoint = 0; }
};

struct T1PROJECT_API FGsCommunityEventMissionTab
{
	CommunityEventMissionTabId _missionTabId = INVALID_COMMUNITY_EVENT_MISSION_TAB_ID;
	int8 _tabIndex = 0;

	const FGsSchemaCommunityEventMissionTab* _schemaMissionTab = nullptr;
	TMap<CommunityEventMissionCommonId, TSharedPtr<FGsCommunityEventMissionCommon>>  _missionCommons;
	bool _isTimeLock = true;

	FGsCommunityEventMissionTab() = default;
	FGsCommunityEventMissionTab(CommunityEventMissionTabId inId, int8 inTabIndex)
		: _missionTabId(inId),
		_tabIndex(inTabIndex)
	{		
	}
	virtual ~FGsCommunityEventMissionTab()
	{
		_missionCommons.Empty();
	}

	void ApplyData()
	{
		_schemaMissionTab = UGsTableUtil::FindRowById<UGsTableCommunityEventMissionTab, FGsSchemaCommunityEventMissionTab>(_missionTabId);
		if (nullptr == _schemaMissionTab)
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("[CommunityEventMissionTab] CommunityEventMissionTabData nullptr = Id: %d"), _schemaMissionTab);
#endif
		}

		_missionCommons.ValueSort([](const TSharedPtr<FGsCommunityEventMissionCommon>& A, const TSharedPtr<FGsCommunityEventMissionCommon>& B)
			{
				return (A->GetCommonIndex() < B->GetCommonIndex()) ? true : false;
			});
	}

	CommunityEventMissionTabId GetMissionTabId() { return _schemaMissionTab ? _schemaMissionTab->id : 0; }
	int8 GetTabIndex() { return _tabIndex; }
	int32 GetTabOpenTime() { return _schemaMissionTab ? _schemaMissionTab->tabOpenTime : 0; }	
	FText GetTabNameText() { return _schemaMissionTab ? _schemaMissionTab->tabNameText : FText::GetEmpty(); }

	int GetCommonCount() { return _missionCommons.Num(); }
	bool GetIsTimeLock() { return _isTimeLock; }
	void SetIsTimeLock(bool inValue) { _isTimeLock = inValue; }
	const TMap<CommunityEventMissionCommonId, TSharedPtr<FGsCommunityEventMissionCommon>>&
		GetCommunityEventMissionCommonList() const { return _missionCommons; }

	void AddMissionCommonData(CommunityEventMissionCommonId inCommonId, TSharedPtr<FGsCommunityEventMissionCommon> inCommonData)
	{
		_missionCommons.Emplace(inCommonId, inCommonData);
	}

	TSharedPtr<FGsCommunityEventMissionCommon> GetCommunityEventMissionCommonData(CommunityEventMissionCommonId inCommonId)
	{
		return _missionCommons.FindRef(inCommonId);
	}

	bool IsRedDot()
	{
		if (true == _isTimeLock)
		{
			return false;
		}

		for (auto& iter: _missionCommons)
		{
			TSharedPtr<FGsCommunityEventMissionCommon> missionCommon = iter.Value;
			if (!missionCommon.IsValid())
			{
				continue;
			}

			if (true == missionCommon->IsRedDot())
				return true;
		}

		return false;
	}

	bool IsLock()
	{
		return false;
	}

	bool IsAllMissionCommonCompleted()
	{
		for (auto& iter : _missionCommons)
		{
			TSharedPtr<FGsCommunityEventMissionCommon> missionCommon = iter.Value;
			if (!missionCommon.IsValid())
			{
				continue;
			}

			if (false == missionCommon->IsCompleteMission())
				return false;
		}

		return true;
	}

	bool IsAllMissionCommonRewarded()
	{
		for (auto& iter : _missionCommons)
		{
			TSharedPtr<FGsCommunityEventMissionCommon> missionCommon = iter.Value;
			if (!missionCommon.IsValid())
			{
				continue;
			}

			if (false == missionCommon->IsRewarded())
				return false;
		}

		return true;
	}
};

class FGsCommunityEventMissionData
{
	CommunityEventMissionId _missionId = INVALID_COMMUNITY_EVENT_MISSION_ID;

	const FGsSchemaCommunityEventMission* _schemaMission = nullptr;
	TMap<CommunityEventMissionTabId, TSharedPtr<FGsCommunityEventMissionTab>>  _missionTabs;
	FGsRewardUnboxingData _bonusRewardData;
	bool _isRewardedBonus = false;
	bool _isHasRewardBonus = false;

public:
	FGsCommunityEventMissionData() = default;
	explicit FGsCommunityEventMissionData(CommunityEventMissionId inId, bool inIsRewardedBonus)
		:_missionId(inId),
		_isRewardedBonus(inIsRewardedBonus)
	{
	}
	virtual ~FGsCommunityEventMissionData()
	{
		_missionTabs.Empty();
	}

	void ApplyData()
	{
		_schemaMission = UGsTableUtil::FindRowById<UGsTableCommunityEventMission, FGsSchemaCommunityEventMission>(_missionId);
		if (nullptr == _schemaMission)
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("[CommunityEventMission] CommunityEventMissionData nullptr = Id: %d"), _missionId);
#endif
			return;
		}

		if (true == _schemaMission->isBonus)
		{
			if (const FGsSchemaRewardData* rewardTable = _schemaMission->bonusRewardId.GetRow())
			{
				_isHasRewardBonus = true;
				FGsRewardHelper::UnboxingRewardData(rewardTable->id, _bonusRewardData);
			}
			else
			{
				_isHasRewardBonus = false;
			}
		}
		else
		{
			_isHasRewardBonus = false;
		}
	}
	CommunityEventMissionId GetMissionId() { return _missionId; }
	bool IsHasRewardBonus() { return _isHasRewardBonus; }
	bool IsRewardedBonus() { return _isRewardedBonus; }
	FGsRewardUnboxingData& GetBonusRewardData() { return _bonusRewardData; }

	int GetTabCount() { return _missionTabs.Num(); }
	int GetCompletedTabCount()
	{
		int completedCount = 0;
		for (auto& iter : _missionTabs)
		{
			TSharedPtr<FGsCommunityEventMissionTab> missionTab = iter.Value;
			if (!missionTab.IsValid())
			{
				continue;
			}

			if (true == missionTab->IsAllMissionCommonCompleted())
			{
				++completedCount;
			}
		}
		return completedCount;
	}

	const TMap<CommunityEventMissionTabId, TSharedPtr<FGsCommunityEventMissionTab>>&
		GetCommunityEventMissionTabDataList() const { return _missionTabs; }

	TSharedPtr<FGsCommunityEventMissionTab> GetCommunityEventMissionTabData(CommunityEventMissionTabId inTabId)
	{
		_missionTabs.ValueSort([](const TSharedPtr<FGsCommunityEventMissionTab>& A, const TSharedPtr<FGsCommunityEventMissionTab>& B)
			{
				return (A->GetTabIndex() < B->GetTabIndex()) ? true : false;
			});

		return _missionTabs.FindRef(inTabId);
	}

	void AddMissionTabData(CommunityEventMissionTabId inTabId, TSharedPtr<FGsCommunityEventMissionTab> inTabData)
	{
		_missionTabs.Emplace(inTabId, inTabData);
	}

	void SetIsRewardedBonus(bool inIsRewardedBonus)
	{
		_isRewardedBonus = inIsRewardedBonus;
	}

	bool IsBonusRedDot()
	{
		if (false == IsHasRewardBonus())
		{
			return false;
		}

		return !IsRewardedBonus() && IsAllMissionCompleted();
	}

	bool IsAllMissionCompleted()
	{
		for (auto& iter : _missionTabs)
		{
			TSharedPtr<FGsCommunityEventMissionTab> missionTab = iter.Value;
			if (!missionTab.IsValid())
			{
				continue;
			}

			if (false == missionTab->IsAllMissionCommonCompleted())
				return false;
		}
		return true;
	}

	bool IsAllMissionRewarded()
	{
		for (auto& iter : _missionTabs)
		{
			TSharedPtr<FGsCommunityEventMissionTab> missionTab = iter.Value;
			if (!missionTab.IsValid())
			{
				continue;
			}

			if (false == missionTab->IsAllMissionCommonRewarded())
				return false;
		}
		return true;
	}

	bool IsRedDot()
	{
		for (auto& iter : _missionTabs)
		{
			TSharedPtr<FGsCommunityEventMissionTab> missionTab = iter.Value;
			if (!missionTab.IsValid())
			{
				continue;
			}
			
			if (true == missionTab->GetIsTimeLock())
				return false;
			if (true == missionTab->IsRedDot())
				return true;
		}
		return false;
	}

	bool IsAllMissionAndBonusRewarded()
	{
		return IsRewardedBonus() && IsAllMissionRewarded();
	}
};
