#pragma once

#include "EGsSealState.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaSealSlot;
struct SealSlotData;

/** 봉인슬롯레벨업 시 잠금 타입 */
enum class EGsSealRerollUnlockTarget
{
    Stat,
    Color,
};

/**
 * 봉인슬롯. 봉인노드에 포함되는 객체.
 */
class FGsSealSlot final
{
    friend class FGsSealNode;

public:
	const FGsSchemaSealSlot* _data = nullptr;

private:
    bool _isOpened = false;
    int32 _sealStatTier = 0;
    SealSlotColor _color = SealSlotColor::NONE;
    StatType _statType = StatType::NONE;
    StatValue _statValue = 0;
    bool _rerollUnlockedStat = true;
    bool _rerollUnlockedColor = true;

public:
	explicit FGsSealSlot(const FGsSchemaSealSlot* InSealSlotData);
	void SetOpened();
    bool IsOpened() const;
    SealSlotId GetSealSlotId() const;
    EGsSealState GetSealState() const;
    int32 GetSealStatTier() const;
    SealSlotColor GetSealColor() const;
	FText GetStatText() const;
	bool CanRerollLock() const;
    bool IsRerollUnlock(const EGsSealRerollUnlockTarget InUnlockTarget) const;
    void SetRerollUnlock(const EGsSealRerollUnlockTarget InUnlockTarget, const bool InUnlock);

private:
    void SetSealSlotData(const SealSlotData& InSealSlotData);
};
