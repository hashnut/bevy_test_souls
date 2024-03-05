#pragma once

#include "EGsSealState.h"
#include "GsSealSlot.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsCostPackage;
class FGsSealSlot;
struct FGsSchemaSealNode;
struct SealData;
struct SealSlotData;

/**
 * 봉인노드. 봉인팩에 포함되고, 봉인슬롯을 소유하는 객체.
 */
class FGsSealNode final
{
public:
	const FGsSchemaSealNode* _data;

private:
	const TRange<Level> _levelRange;
	const TArray<TSharedRef<FGsSealSlot>> _sealSlots;

private:
    SealSlotColor _sealColor = SealSlotColor::NONE;
	Level _level = INVALID_LEVEL;
    Exp _exp = 0;

public:
    explicit FGsSealNode(const FGsSchemaSealNode* InSealNodeData);

public:
    SealId GetSealNodeId() const;
    int32 GetSealNodeLevelId() const;
    FText GetTitle() const;
    Exp GetTotalExp() const;
    Exp GetCurrentExp() const;
    Exp GetRandomExpMin() const;
    Exp GetRandomExpMax() const;
    Level GetLevel() const;
    Level GetLevelMax() const;
    bool IsLevelMax() const;
    Level GetRerollAvailableLevel() const;
    bool CanLinkLine() const;
    SealSlotColor GetSealColor() const;
    int32 GetSealSlotCount() const;
    TWeakPtr<const FGsSealSlot> GetSealSlotByIndex(const int32 InIndex) const;
    TWeakPtr<FGsSealSlot> FindSealSlotById(SealSlotId InSealSlotId);
    Exp GetSealDataExpDiff(const SealData& InSealData) const;
    int32 GetSealDataLevelDiff(const SealData& InSealData) const;
    bool IsAnyRerollUnlocked(const EGsSealRerollUnlockTarget InUnlockTarget) const;

public:
    void SetSealData(const SealData& InSealData);
    void SetSealSlotDatas(const SealSlotData& InSealSlotData);
    void SetRerollUnlock(const SealSlotId InSealSlotId, const EGsSealRerollUnlockTarget InUnlockTarget, const bool InUnlock);
    
public:
    TSharedPtr<FGsCostPackage> CreateLevelUpCostPackage(const bool IsCurrency = false) const;
    TSharedPtr<FGsCostPackage> CreateRerollCostPackage(const int32 InLockCount, const bool IsCurrency = false) const;
    bool CanLevelUp() const;
    bool TryLevelUp();
    
public:
    bool CanReroll() const;
    bool TryReroll() const;

private:
    static TArray<TSharedRef<FGsSealSlot>> MakeSealSlots(const SealId InSealNodeId);
};
