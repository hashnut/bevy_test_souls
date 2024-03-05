#pragma once

#include "EGsSealState.h"
#include "GsSealNode.h"
#include "Management/ScopeGame/GsSealManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

struct FGsSchemaSealPack;
struct SealBonusData;
struct SealData;
struct SealSlotData;

/**
 * 봉인팩. 봉인노드를 소유하는 객체.
 */
class FGsSealPack final
{
	using MatchCount = FGsSealManager::MatchCount;

private:
	const FGsSchemaSealPack* _data;
	TArray<TWeakPtr<FGsSealNode>> _sealNodes;

	/** 색상별 봉인노드수 */
	TMap<SealSlotColor, MatchCount> _countPerLineColorMap;
	EGsSealState _sealState = EGsSealState::Activated;

public:
    explicit FGsSealPack(const FGsSchemaSealPack* InSealPackData);

public:
    SealPackId GetSealPackId() const;
	FText GetSealPackTitle() const;
	EGsSealState GetSealState() const;
	SealSlotColor GetSealPackColor() const;

    int32 GetSealRerollLockCountMax() const;
	int32 FindNodeIndexByNodeId(const SealId InNodeId) const;
	const FGsSealNode* GetSealNodeByIndex(const int32 InIndex) const;
	const int32 GetSealNodeCount() const;
	const FGsSealNode* FindSealNodeById(const SealId InSealNodeId) const;
	const TMap<SealSlotColor, FGsSealManager::MatchCount>& GetSealLineColorCount() const;
	bool CanDisplay() const;
	const TMap<FGsSealManager::MatchCount, TArray<TPair<StatType, StatValue>>>* GetSealPackAllGainsInColor(const SealSlotColor InLineColor) const;
	const TArray<TPair<StatType, StatValue>>* GetSealLineGainStats(const SealSlotColor InLineColor, const int32 InLineCount = 1) const;
	Exp GetSealNodeExpDiff(const SealData& InSealData) const;
	int32 GetSealNodeLevelDiff(const SealData& InSealData) const;

public:
	void SetSealBunusDatas(const TArray<SealBonusData>& InSealBonusDatas);
	void SetSealState(const EGsSealState InState);
	void UpdateSealLines();

private:
    static TArray<TWeakPtr<FGsSealNode>> MakeSealNodes(const SealPackId InSealPackId);
    static TMap<SealSlotColor, FGsSealManager::MatchCount> MakeDefaultLineColorMap();
};
