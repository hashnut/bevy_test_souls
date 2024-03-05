#include "GsSealSlot.h"

#include "DataSchema/Seal/GsSchemaSealSlot.h"
#include "EGsSealState.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

FGsSealSlot::FGsSealSlot(const FGsSchemaSealSlot* InSealSlotData)
	: _data(InSealSlotData)
{
	check(nullptr != InSealSlotData);
}

void FGsSealSlot::SetOpened()
{
	_isOpened = true;
}

bool FGsSealSlot::IsOpened() const
{
	return _isOpened;
}

SealSlotId FGsSealSlot::GetSealSlotId() const
{
	return _data->id;
}

int32 FGsSealSlot::GetSealStatTier() const
{
	return _sealStatTier;
}

SealSlotColor FGsSealSlot::GetSealColor() const
{
	return _color;
}

FText FGsSealSlot::GetStatText() const
{
	FText Text;
	if (_isOpened)
	{
		if (StatType::NONE == _statType)
		{
			FText::FindText(TEXT("SealText"), TEXT("UI_Slot_NeedReroll"), Text);
		}
		else
		{
			FString StatStr;
			if (!FGsStatHelper::GetDesplayStatNameAndValue(_statType, _statValue, StatStr))
			{
				GSLOG(Error, TEXT("!FGsStatHelper::GetDesplayStatNameAndValue(%s, %d, StatStr)"), *StatTypeEnumToString(_statType), _statValue);
			}
			Text = FText::FromString(StatStr);
		}
	}
	else
	{
		FText::FindText(TEXT("SealText"), TEXT("UI_Slot_NotOpened"), Text);
	}
	return Text;
}

bool FGsSealSlot::CanRerollLock() const
{
	// 최초 오픈 시에는 스탯이 미지정이라 리롤 잠금 기능을 사용하지 못하게 하기 위함.
	return _isOpened && StatType::NONE != _statType;
}

bool FGsSealSlot::IsRerollUnlock(const EGsSealRerollUnlockTarget InUnlockTarget) const
{
	if (EGsSealRerollUnlockTarget::Stat == InUnlockTarget)
	{
		return _rerollUnlockedStat;
	}
	else if (EGsSealRerollUnlockTarget::Color == InUnlockTarget)
	{
		return _rerollUnlockedColor;
	}
	return false;
}

void FGsSealSlot::SetSealSlotData(const SealSlotData& InSealSlotData)
{
	check(GetSealSlotId() == InSealSlotData.mSealSlotId);

	_color = InSealSlotData.mSealSlotColor;
	_statType = InSealSlotData.mStatType;
	_statValue = InSealSlotData.mStatValue;
}

void FGsSealSlot::SetRerollUnlock(const EGsSealRerollUnlockTarget InUnlockTarget, const bool InUnlock)
{
	if (EGsSealRerollUnlockTarget::Stat == InUnlockTarget)
	{
		_rerollUnlockedStat = InUnlock;
	}
	else if (EGsSealRerollUnlockTarget::Color == InUnlockTarget)
	{
		_rerollUnlockedColor = InUnlock;
	}
}
