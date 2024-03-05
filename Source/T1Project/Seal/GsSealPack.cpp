#include "GsSealPack.h"

#include "DataSchema/Seal/GsSchemaSealNode.h"
#include "DataSchema/Seal/GsSchemaSealPack.h"
#include "EGsSealState.h"
#include "GsSealNode.h"
#include "Management/ScopeGame/GsSealManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Management/GsScopeHolder.h"
#include "../Management/GsScopeGame.h"

FGsSealPack::FGsSealPack(const FGsSchemaSealPack* InSealPackData)
	: _data(InSealPackData)
	, _sealNodes(FGsSealPack::MakeSealNodes(_data->id))
	, _countPerLineColorMap(FGsSealPack::MakeDefaultLineColorMap())
	, _sealState(EGsSealState::Unknown)
{
	check(nullptr != InSealPackData);
	//UpdateSealLines();
}

SealPackId FGsSealPack::GetSealPackId() const
{
	return _data->id;
}

FText FGsSealPack::GetSealPackTitle() const
{
	return _data->sealPackName;
}

EGsSealState FGsSealPack::GetSealState() const
{
	return _sealState;
}

SealSlotColor FGsSealPack::GetSealPackColor() const
{
	const MatchCount SealNodesCount = _sealNodes.Num();

	for (const TMap<SealSlotColor, MatchCount>::ElementType& CountPerLineColor : _countPerLineColorMap)
	{
		if (SealNodesCount == CountPerLineColor.Value)
		{
			return CountPerLineColor.Key;
		}
	}

	return SealSlotColor::NONE;
}

int32 FGsSealPack::GetSealRerollLockCountMax() const
{
	return _data->slotLockCountMax;
}

int32 FGsSealPack::FindNodeIndexByNodeId(const SealId InNodeId) const
{
	for (int32 i = _sealNodes.Num() - 1; 0 <= i; --i)
	{
		if (_sealNodes[i].IsValid() && InNodeId == _sealNodes[i].Pin()->GetSealNodeId())
		{
			return i;
		}
	}
	return -1;
}

const FGsSealNode* FGsSealPack::GetSealNodeByIndex(const int32 InIndex) const
{
	TWeakPtr<FGsSealNode> SealNodeWeak;

	if (!_sealNodes.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_sealNodes.IsValidIndex(%d)"), InIndex);
		return nullptr;
	}

	return _sealNodes[InIndex].IsValid() ? _sealNodes[InIndex].Pin().Get() : nullptr;
}

const int32 FGsSealPack::GetSealNodeCount() const
{
	return _sealNodes.Num();
}

const FGsSealNode* FGsSealPack::FindSealNodeById(const SealId InSealNodeId) const
{
	for (const TWeakPtr<FGsSealNode>& SealNode : _sealNodes)
	{
		if (!SealNode.IsValid())
		{
			GSLOG(Error, TEXT("!SealNode.IsValid()"));
			continue;
		}

		if (SealNode.Pin()->GetSealNodeId() == InSealNodeId)
		{
			return SealNode.Pin().Get();
		}
	}
	return nullptr;
}

const TMap<SealSlotColor, FGsSealManager::MatchCount>& FGsSealPack::GetSealLineColorCount() const
{
	return _countPerLineColorMap;
}

bool FGsSealPack::CanDisplay() const
{
	return (EGsSealState::Activated == _sealState) || (EGsSealState::Sealed == _sealState);
}

const TMap<FGsSealManager::MatchCount, TArray<TPair<StatType, StatValue>>>* FGsSealPack::GetSealPackAllGainsInColor(const SealSlotColor InLineColor) const
{
	FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		return nullptr;
	}

	return SealManager->GetSealPackAllGainsInColor(_data->sealPackGainColorId, InLineColor);
}

const TArray<TPair<StatType, StatValue>>* FGsSealPack::GetSealLineGainStats(const SealSlotColor InLineColor, const int32 InLineCount /*= 1*/) const
{
	const TMap<MatchCount, TArray<TPair<StatType, StatValue>>>* MatchCountStats = GetSealPackAllGainsInColor(InLineColor);
	if (nullptr == MatchCountStats)
	{
		return nullptr;
	}

	return MatchCountStats->Find(InLineCount);
}

Exp FGsSealPack::GetSealNodeExpDiff(const SealData& InSealData) const
{
	const FGsSealNode* SealNode = FindSealNodeById(InSealData.mSealId);
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode, InSealData.mSealId:%d"), InSealData.mSealId);
		return 0;
	}

	return SealNode->GetSealDataExpDiff(InSealData);
}

int32 FGsSealPack::GetSealNodeLevelDiff(const SealData& InSealData) const
{
	const FGsSealNode* SealNode = FindSealNodeById(InSealData.mSealId);
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode, InSealData.mSealId:%d"), InSealData.mSealId);
		return 0;
	}

	return SealNode->GetSealDataLevelDiff(InSealData);
}

void FGsSealPack::SetSealBunusDatas(const TArray<SealBonusData>& InSealBonusDatas)
{
	//for (const SealBonusData& CurrentSealBonusData : InSealBonusDatas)
	//{
	//	if (!_countPerLineColorMap.Contains(CurrentSealBonusData.mSealSlotColor))
	//	{
	//		GSLOG(Error, TEXT("!_lineCountMap.Contains(%d)"), *SealSlotColorEnumToString(CurrentSealBonusData.mSealSlotColor));
	//		continue;
	//	}

	//	_countPerLineColorMap[CurrentSealBonusData.mSealSlotColor] = CurrentSealBonusData.mCount;
	//}
	//UpdateSealLines();
}

void FGsSealPack::SetSealState(const EGsSealState InState)
{
	_sealState = InState;
}

void FGsSealPack::UpdateSealLines()
{
	// 갯수 리셋
	TSet<SealSlotColor> LineColorKeys;
	_countPerLineColorMap.GetKeys(LineColorKeys);
	for (const SealSlotColor& lineColor : LineColorKeys)
	{
		int32* Count = _countPerLineColorMap.Find(lineColor);
		if (nullptr != Count)
		{
			*Count = 0;
		}
	}

	for (const TWeakPtr<FGsSealNode>& SealNode : _sealNodes)
	{
		if (!SealNode.IsValid())
		{
			GSLOG(Error, TEXT("!SealNode.IsValid()"));
			continue;
		}

		int32* ColorCount = _countPerLineColorMap.Find(SealNode.Pin()->GetSealColor());
		if (nullptr != ColorCount)
		{
			++(*ColorCount);
		}
	}
}

TArray<TWeakPtr<FGsSealNode>> FGsSealPack::MakeSealNodes(const SealPackId InSealPackId)
{
	FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return TArray<TWeakPtr<FGsSealNode>>();
	}

	return SealManager->MakeInitSealNodes(InSealPackId);
}

TMap<SealSlotColor, FGsSealManager::MatchCount> FGsSealPack::MakeDefaultLineColorMap()
{
	return TMap<SealSlotColor, MatchCount> {
		{ SealSlotColor::RARE, 0 },
		{ SealSlotColor::EPIC, 0 },
		{ SealSlotColor::UNIQUE, 0 }
	};
}
