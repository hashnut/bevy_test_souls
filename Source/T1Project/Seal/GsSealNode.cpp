#include "GsSealNode.h"

#include "Currency/GsCostPackage.h"
#include "DataSchema/Seal/GsSchemaSealNode.h"
#include "DataSchema/Seal/GsSchemaSealNodeLevel.h"
#include "DataSchema/Seal/GsSchemaSealNodeRerollCost.h"
#include "DataSchema/Seal/GsSchemaSealSlot.h"
#include "EGsSealState.h"
#include "GsSealSlot.h"
#include "GsTable.h"
#include "Management/ScopeGame/GsSealManager.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

FGsSealNode::FGsSealNode(const FGsSchemaSealNode* InSealNodeData)
	: _data(InSealNodeData)
	, _levelRange(FGsSealManager::FindSealNodeAllLevelRange(_data->sealNodeLevelId))
	, _sealSlots(FGsSealNode::MakeSealSlots(_data->id))
{
	check(nullptr != InSealNodeData);
}

SealId FGsSealNode::GetSealNodeId() const
{
	return _data->id;
}

int32 FGsSealNode::GetSealNodeLevelId() const
{
	return _data->sealNodeLevelId;
}

FText FGsSealNode::GetTitle() const
{
	return _data->sealNodeName;
}

Exp FGsSealNode::GetTotalExp() const
{
	const FGsSchemaSealNodeLevel* SealNodeLevel = FGsSealManager::FindSealNodeLevel(GetSealNodeLevelId(), _level);
	if (nullptr == SealNodeLevel)
	{
		GSLOG(Error, TEXT("nullptr == SealNodeLevel"));
		return 0;
	}
	else
	{
		return SealNodeLevel->needExp;
	}
}

Exp FGsSealNode::GetCurrentExp() const
{
	return _exp;
}

Exp FGsSealNode::GetRandomExpMin() const
{
	const FGsSchemaSealNodeLevel* SealNodeLevel = FGsSealManager::FindSealNodeLevel(GetSealNodeLevelId(), _level);
	if (nullptr == SealNodeLevel)
	{
		GSLOG(Error, TEXT("nullptr == SealNodeLevel"));
		return 0;
	}

	return SealNodeLevel->gainExpRandomMin;
}

Exp FGsSealNode::GetRandomExpMax() const
{
	const FGsSchemaSealNodeLevel* SealNodeLevel = FGsSealManager::FindSealNodeLevel(GetSealNodeLevelId(), _level);
	if (nullptr == SealNodeLevel)
	{
		GSLOG(Error, TEXT("nullptr == SealNodeLevel"));
		return 0;
	}

	return SealNodeLevel->gainExpRandomMax;
}

Level FGsSealNode::GetLevel() const
{
	return _level;
}

Level FGsSealNode::GetLevelMax() const
{
	return _levelRange.GetUpperBoundValue();
}

bool FGsSealNode::IsLevelMax() const
{
	return _levelRange.GetUpperBoundValue() <= _level;
}

Level FGsSealNode::GetRerollAvailableLevel() const
{
	return _data->openSealSlotOpenLevel;
}

bool FGsSealNode::CanLinkLine() const
{
	return SealSlotColor::NONE != GetSealColor();
}

SealSlotColor FGsSealNode::GetSealColor() const
{
	TSet<SealSlotColor> SealSlotColorSet;
	for (const TSharedRef<FGsSealSlot>& SealSlot : _sealSlots)
	{
		SealSlotColorSet.Add(SealSlot->GetSealColor());
	}
	return (1 == SealSlotColorSet.Num()) ? *SealSlotColorSet.CreateConstIterator() : SealSlotColor::NONE;
}

int32 FGsSealNode::GetSealSlotCount() const
{
	return _sealSlots.Num();
}

TWeakPtr<const FGsSealSlot> FGsSealNode::GetSealSlotByIndex(const int32 InIndex) const
{
	if (!_sealSlots.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_sealSlots.IsValidIndex(%d)"), InIndex);
		return nullptr;
	}

	return _sealSlots[InIndex];
}

TWeakPtr<FGsSealSlot> FGsSealNode::FindSealSlotById(SealSlotId InSealSlotId)
{
	for (TSharedRef<FGsSealSlot> SealSlot : _sealSlots)
	{
		if (SealSlot->GetSealSlotId() == InSealSlotId)
		{
			return SealSlot;
		}
	}
	return nullptr;
}

Exp FGsSealNode::GetSealDataExpDiff(const SealData& InSealData) const
{
	check(_data->id == InSealData.mSealId);

	Exp ExpDiff = 0;
	for (Level i = _level; i < InSealData.mLevel; ++i)
	{
		const FGsSchemaSealNodeLevel* SealNodeLevel = FGsSealManager::FindSealNodeLevel(GetSealNodeLevelId(), i);
		if (nullptr == SealNodeLevel)
		{
			GSLOG(Error, TEXT("nullptr == SealNodeLevel"));
			return 0;
		}

		ExpDiff += StaticCast<Exp>(SealNodeLevel->needExp);
	}

	return FMath::Clamp(ExpDiff - _exp + InSealData.mExp, StaticCast<Exp>(0), TNumericLimits<Exp>::Max());
}

int32 FGsSealNode::GetSealDataLevelDiff(const SealData& InSealData) const
{
	check(_data->id == InSealData.mSealId);
	return FMath::Clamp(InSealData.mLevel - _level, TNumericLimits<int32>::Min(), TNumericLimits<int32>::Max());
}

bool FGsSealNode::IsAnyRerollUnlocked(const EGsSealRerollUnlockTarget InUnlockTarget) const
{
	bool IsAnyRerollColorUnlocked = false;
	for (const TSharedRef<FGsSealSlot>& SealSlot : _sealSlots)
	{
		if (SealSlot->IsRerollUnlock(InUnlockTarget))
		{
			IsAnyRerollColorUnlocked = true;
			break;
		}
	}
	return IsAnyRerollColorUnlocked;
}

void FGsSealNode::SetSealData(const SealData& InSealData)
{
	check(_data->id == InSealData.mSealId);

	_level = InSealData.mLevel;
	_exp = InSealData.mExp;

	if (CanReroll())
	{
		for (const TSharedRef<FGsSealSlot>& SealSlot : _sealSlots)
		{
			SealSlot->SetOpened();
		}
	}
}

void FGsSealNode::SetSealSlotDatas(const SealSlotData& InSealSlotData)
{
	TWeakPtr<FGsSealSlot> SealSlotPtr = FindSealSlotById(InSealSlotData.mSealSlotId);
	if (!SealSlotPtr.IsValid())
	{
		GSLOG(Error, TEXT("SealSlotPtr.IsValid(), CurrentSealSlotData.mSealSlotId:%d"), InSealSlotData.mSealSlotId);
		return;
	}

	SealSlotPtr.Pin()->SetSealSlotData(InSealSlotData);
}

void FGsSealNode::SetRerollUnlock(const SealSlotId InSealSlotId, const EGsSealRerollUnlockTarget InUnlockTarget, const bool InUnlock)
{
	TWeakPtr<FGsSealSlot> SealSlot = FindSealSlotById(InSealSlotId);
	if (!SealSlot.IsValid())
	{
		GSLOG(Error, TEXT("!SealSlot.IsValid()"));
		return;
	}

	SealSlot.Pin()->SetRerollUnlock(InUnlockTarget, InUnlock);
}

TSharedPtr<FGsCostPackage> FGsSealNode::CreateLevelUpCostPackage(const bool IsCurrency /*= false*/) const
{
	TSharedPtr<FGsCostPackage> CostPackage = nullptr;
	if (CanLevelUp())
	{
		const FGsSchemaSealNodeLevel* SealNodeLevel = FGsSealManager::FindSealNodeLevel(GetSealNodeLevelId(), _level);
		if (nullptr == SealNodeLevel)
		{
			GSLOG(Error, TEXT("nullptr == SealNodeLevel"));
		}
		else
		{
			// 비용이 정상인지 체크 후 반환
			CostPackage = MakeShared<FGsCostPackage>(ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT);
			CostPackage->SetJointCostCandidateElements(true);
			
			// 재화 타입
			if (IsCurrency && 0 < SealNodeLevel->costCurrencyAmount)
			{
				CostPackage->AddCost(SealNodeLevel->costCurrencyType, SealNodeLevel->costCurrencyAmount);
			}
			// 아이템 타입
			else if (!IsCurrency && 0 <= SealNodeLevel->costItemAmount.Num())
			{
				TArray<ItemId> CostItemIds;
				for (int i = 0; i < SealNodeLevel->costItemId.Num(); ++i)
				{
					ItemId CostItemId = StaticCast<ItemId>(SealNodeLevel->costItemId[i]);
					Currency CostAmount = 0;
					if (SealNodeLevel->costItemAmount.IsValidIndex(i))
						CostAmount = SealNodeLevel->costItemAmount[i];

					CostPackage->AddCost(CostItemId, CostAmount, true);
				}
			}
			else
			{
				return nullptr;
			}
		}
	}
	return CostPackage;
}

TSharedPtr<FGsCostPackage> FGsSealNode::CreateRerollCostPackage(const int32 InLockCount, const bool IsCurrency /*= false*/) const
{
	TSharedPtr<FGsCostPackage> CostPackage = nullptr;
	if (CanReroll())
	{
		const FGsSchemaSealNodeLevel* SealNodeLevel = FGsSealManager::FindSealNodeLevel(GetSealNodeLevelId(), _level);
		if (nullptr == SealNodeLevel)
		{
			GSLOG(Error, TEXT("nullptr == SealNodeLevel"));
		}
		else
		{
			const int32 RerollCostId = SealNodeLevel->rerollCostId;
			const UGsTable* RerollCostTable = FGsSchemaSealNodeRerollCost::GetStaticTable();
			TArray<const FGsSchemaSealNodeRerollCost*> RerollCosts;
			if (!RerollCostTable->GetAllRows(RerollCosts))
			{
				GSLOG(Error, TEXT("!RerollCostTable->GetAllRows(RerollCosts)"));
			}
			else
			{
				// TODO: 최적화 필요
				for (const FGsSchemaSealNodeRerollCost* RerollCost : RerollCosts)
				{
					if (nullptr == RerollCost)
					{
						GSLOG(Error, TEXT("nullptr == RerollCost"));
						continue;
					}

					if (RerollCost->rerollCostId == RerollCostId && RerollCost->slotLockCount == InLockCount)
					{
						CostPackage = MakeShared<FGsCostPackage>(ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT);
						CostPackage->SetJointCostCandidateElements(true);

						if (IsCurrency)
						{
							CostPackage->AddCost(RerollCost->currencyType, RerollCost->currencyAmount);
						}
						else
						{
							TArray<ItemId> CostItemIds;
							for (int i = 0; i < RerollCost->itemId.Num(); ++i)
							{
								ItemId CostItemId = StaticCast<ItemId>(RerollCost->itemId[i]);
								Currency CostAmount = 0;
								if (RerollCost->itemAmount.IsValidIndex(i))
									CostAmount = RerollCost->itemAmount[i];

								CostPackage->AddCost(CostItemId, CostAmount, true);
							}
						}
					}
				}
			}
		}
	}
	return CostPackage;
}

bool FGsSealNode::CanLevelUp() const
{
	return !IsLevelMax();
}

bool FGsSealNode::TryLevelUp()
{
	return false;
}

bool FGsSealNode::CanReroll() const
{
	return GetRerollAvailableLevel() <= _level;
}

bool FGsSealNode::TryReroll() const
{
	return false;
}

TArray<TSharedRef<FGsSealSlot>> FGsSealNode::MakeSealSlots(const SealId InSealNodeId)
{
	TArray<TSharedRef<FGsSealSlot>> SealSlots;

	const UGsTable* SealSlotTable = FGsSchemaSealSlot::GetStaticTable();
	if (nullptr == SealSlotTable)
	{
		GSLOG(Error, TEXT("nullptr == SealSlotTable"));
	}
	else
	{
		TArray<const FGsSchemaSealSlot*> SealSlotDatas;
		if (!SealSlotTable->GetAllRows(SealSlotDatas))
		{
			GSLOG(Error, TEXT("!SealSlotTable->GetAllRows(SealSlotDatas)"));
		}
		else
		{
			for (const FGsSchemaSealSlot* SealSlotData : SealSlotDatas)
			{
				if (InSealNodeId != SealSlotData->sealNodeId)
				{
					continue;
				}

				TSharedRef<FGsSealSlot> SealSlot = MakeShared<FGsSealSlot>(SealSlotData);
				SealSlots.Emplace(SealSlot);

				GSSeal()->SetSealSlotData(SealSlotData->id, SealSlot);
			}
		}
	}

	return SealSlots; //rvo
}
