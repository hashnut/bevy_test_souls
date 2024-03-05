#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsManager.h"
#include "Management/GsScopeHolder.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassMissionTab.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassMissionGroup.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassMissionCommon.h"
#include "BattlePass/GsBattlePassData.h"
#include "GsBattlePassManager.generated.h"

struct BattlePassItem
{
	BattlePassId passId;
	Level		 level;
	Exp			 exp;

	bool isUseAdditionalReward;

	TArray<Level> rewardedLevels;

	TArray<Level> additionalRewardedLevels;
	TMap<BattlePassMissionTabId, TArray<TPair<BattlePassMissionId, uint64>>> passMissionIdPoints;
};

/**
 *  배틀 패스 매니저
 */
UCLASS()
class T1PROJECT_API UGsBattlePassManager : public UObject, public IGsManager
{
	GENERATED_BODY()

protected:
	TMap<BattlePassId, TSharedPtr<FGsBattlePassData>> _battlePass { TMap<BattlePassId, TSharedPtr<FGsBattlePassData>>()};  // 현재 보이는 배틀 패스
	FTimerHandle _gameTimeHandle;

public:
	void Initialize() override;
	void Finalize() override;

private:
	void LoadOpenTabBattlePass();
	void ReloadOpenTabBattlePass();

private:
	TWeakPtr<FGsBattlePassData> AddDisplayPassData(const FGsSchemaBattlePassCommon* pass);

	bool GetOpenBattlePassCheck(const TArray<const FGsSchemaBattlePassCommon*>& battlePassList, OUT FDateTime& passCheckTime);
	bool GetCloseBattlePassCheck(IN const FDateTime& passOpenTime, OUT FDateTime& passCheckTime, bool check = true);

public:
	TWeakPtr<FGsBattlePassData> GetBattlePassData(BattlePassId passId);
	int GetTabOpendBattlePassList(TArray<TWeakPtr<FGsBattlePassData>>& openBattlePass);

public:
	void UpdateBattlePassLevel(BattlePassId passId, Level level, Exp levelExp = 0);
	void UpdateBattlePassLevelRewarded(BattlePassId passId, Level level, bool isAdditionalReward = false);
	void UpdatePassLevelAdditionalRewardUnlock(BattlePassId passId);

	void UpdatePassMissionPoint(BattlePassId passId, BattlePassMissionTabId missionTabId, BattlePassMissionId passMissionId, uint64 missionPoint);

	void UpdateBattlePassList(TArray<BattlePassItem>& items);
	void UpdateBattlePassData(BattlePassItem& item);

	void ResetTimeContents(ResetTimeTicket rttType);

	void OnAckBattlePassRewardAllList(BattlePassId passId, const TArray<CurrencyDeltaPair>& currencyPairList, const TArray<ItemIdAmountPair>& itemAmountPairList);
	void OnAckBattlePassReceiveLevelRewardAll(BattlePassItem& rewardResult);

public:
	bool IsRedDot();
	bool IsRedDotPass(BattlePassId id);
	bool IsOpenBattlePass() { return _battlePass.Num() > 0; }

	friend class FGsBattlePassData;
};

#define GSBattlePass() UGsScopeHolder::GetGameManager<UGsBattlePassManager>(UGsScopeGame::EManagerType::BattlePass)