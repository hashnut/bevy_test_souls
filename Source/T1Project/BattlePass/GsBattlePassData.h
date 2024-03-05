#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassCommon.h"
#include "GsBattlePassLevelData.h"
#include "GsBattlePassMissionData.h"

class T1PROJECT_API FGsBattlePassData 
{
public:
	struct FGsBattlePassLevelGroup
	{
		BattlePassLevelGroupId _levelGroupId{ INVALID_BATTLE_PASS_LEVEL_GROUP_ID };
		TArray<TSharedPtr<FGsBattlePassLevelData>>  _levels;
		TSharedPtr<FGsBattlePassLevelBonusData> _bonusLevel;

		TWeakPtr<FGsBattlePassLevelData> _currentLevel;
	};

	struct FGsBattlePassMissionGroup
	{
		BattlePassMissionGroupId _missionGroupId{ INVALID_BATTLE_PASS_MISSION_GROUP_ID };
		TArray<TSharedPtr<FGsBattlePassMissionTab>>  _missionTabs;
		TArray<TSharedPtr<FGsBattlePassMissionData>> _missions;
	};

protected:
	BattlePassId _id{ INVALID_BATTLE_PASS_ID };

	const FGsSchemaBattlePassCommon* _tblData{ nullptr };
	const FGsSchemaBattlePassTabDetail* _tabDetail{ nullptr };
	const FGsSchemaBattlePassDetail* _detail{ nullptr };

	FGsBattlePassLevelGroup   _passLevelGroup;
	FGsBattlePassMissionGroup _passMissionGroup;

protected:
	// 추가보상 오픈되어 있는가
	bool _isUnlockAdditionalReward{ false };

	FDateTime _passOpenTime;
	FDateTime _passEndTime;

	bool _isExpose{ true };   // tab에 노출
	bool _isOpen{ true }; // 실제 오픈된 패스

private:
	void MakeBattlePassMissionData(const FGsSchemaBattlePassMissionGroup* missionGroup);
	void MakeBattlePassLevelData(const FGsSchemaBattlePassLevel* levelGroup);

public:
	FGsBattlePassData() = default;
	FGsBattlePassData(const FGsSchemaBattlePassCommon* data);
	virtual ~FGsBattlePassData();

public:
	void SetExposePassTab();
	bool IsExposePassTab() { return _isExpose; }

	void SetOpenPass() { _isOpen = true; }
	bool IsOpenPass();
	bool IsFinishedPass();

	bool IsUnlockAdditionalReward() { return _isUnlockAdditionalReward; }
	
public:
	void SetPassLevelInfo(Level curLevel, Exp curExp);
	void SetPassLevelRewarded(Level level, bool rewarded = true);
	void SetPassLevelAdditionalRewarded(Level level, bool rewarded = true);
	void SetAdditionalRewardUnlock(bool unlock = true);

	void OnMissionInfoChanged(BattlePassMissionTabId missionTabId, BattlePassMissionId missionId, uint64 missionPoint);

public:
	bool  IsSetRedDot() { return IsEligibleForRemainRewards(); }

	BattlePassId GetBattlePassId() { return _id; }
	FString GetPassAvailablePeriod();
	FSoftObjectPath GetCurrentIconPath();
	FText GetPassName() { return _detail ? _detail->name : FText::GetEmpty(); }
	FText GetPassDesc() { return _detail ? _detail->desc : FText::GetEmpty(); }
	FSoftObjectPath GetExpIconPath() { return _detail ? _detail->icon : FSoftObjectPath(); }

	int GetPassTabIndex() { return _tabDetail ? _tabDetail->tabIndex : -1;}
	FText GetPassTabName() { return _tabDetail ? _tabDetail->tabName : FText::GetEmpty(); }
	FText GetPassTabDesc() { return _tabDetail ? _tabDetail->tabDesc : FText::GetEmpty(); }
	
	FSoftObjectPath GetPassBannerPath() { return _tblData ? _tblData->passBanner : FSoftObjectPath(); }

	FGsBattlePassOpenPay GetLevelOpenPay(uint8 level);
	FGsBattlePassOpenPay GetAdditionalRewardOpenPay();
public:
	FDateTime GetBattlePassEndTime() { return _passEndTime; }

public:
	BattlePassMissionGroupId GetBattlePassMissionGroupId() { return _passMissionGroup._missionGroupId; }
	BattlePassLevelGroupId GetBattlePassLevelGroupId()	{ return _passLevelGroup._levelGroupId; }

	TWeakPtr<FGsBattlePassMissionTab> GetMissionTabInfoByIndex(int32 index);
	TWeakPtr<FGsBattlePassMissionTab> GetMissionTabInfo(BattlePassMissionTabId tabId);

public:
	TWeakPtr<FGsBattlePassLevelData> FindLevelData(Level level);
	TWeakPtr<FGsBattlePassLevelData> GetProgressLevelData() { return _passLevelGroup._currentLevel; }

	Level GetMaxLevel();
	bool IsLastLevel();
	bool IsBonusLevel();

	bool IsEligibleForRemainRewards();

	int GetPassLevelList(OUT TArray<TWeakPtr<FGsBattlePassLevelData>>& levels);

	int GetMissionTabList(OUT TArray<TWeakPtr<FGsBattlePassMissionTab>>& missionTabs);
	int GetMissionList(IN BattlePassMissionTabId tabId, OUT TArray<TWeakPtr<FGsBattlePassMissionData>>& missions);

	TWeakPtr<FGsBattlePassLevelBonusData> GetPassLevelBonusData();
};
