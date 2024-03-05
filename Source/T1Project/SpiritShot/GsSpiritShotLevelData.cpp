#include "GsSpiritShotLevelData.h"
#include "GsSpiritShotStatData.h"

#include "T1Project.h"
#include "DataSchema/SpiritShot/GsSchemaSpiritShotLevel.h"

#include "Currency/GsCostPackage.h"


FGsSpiritShotLevelData::FGsSpiritShotLevelData(const FGsSchemaSpiritShotLevel* InShotLevelData)
    : _levelData(InShotLevelData)
    , _statData(MakeShared<FGsSpiritShotStatData>())
    , _enchantRequireItemList(FGsSpiritShotLevelData::MakeRequireItemList(_levelData))
    , _nextLevelId(INVALID_SPIRIT_SHOT_LEVEL_ID)
    , _maxModifyCount(GetRateModifyValue() != 0 ? (GetMaxSuccessRate() - GetDefaultSuccessRate()) / GetRateModifyValue() : 0)
{
    check(nullptr != InShotLevelData);
}

SpiritShotLevelId FGsSpiritShotLevelData::GetLevelId() const
{
    return _levelData->id;  
}

SpiritShotCategoryId FGsSpiritShotLevelData::GetCategoryId() const
{
    if (_levelData->categoryId.GetRow() != nullptr)
    {
        return _levelData->categoryId.GetRow()->id;
    }
    else
    {
        return INVALID_SPIRIT_SHOT_CATEGORY_ID;
    }
}

int32 FGsSpiritShotLevelData::GetLevel() const
{
    return _levelData->level;
}

int32 FGsSpiritShotLevelData::GetDefaultSuccessRate() const
{
    return _levelData->enchantData.defaultSuccessRate;
}

int32 FGsSpiritShotLevelData::GetMaxSuccessRate() const
{
    return _levelData->enchantData.maxSuccessRate;
}

int32 FGsSpiritShotLevelData::GetLevelFallRate() const
{
    return _levelData->enchantData.levelFallRate;
}

int32 FGsSpiritShotLevelData::GetRateModifyValue() const
{
    return _levelData->enchantData.rateModifyValue;
}

int32 FGsSpiritShotLevelData::GetMaxModifyCount() const
{
    return _maxModifyCount;
}

const TArray<SpiritShotEnchantCost>& FGsSpiritShotLevelData::GetRequireItemList() const
{
    return _enchantRequireItemList;
}

RewardId FGsSpiritShotLevelData::GetFailRewardId() const
{
    if (_levelData->failRewardId.GetRow() != nullptr)
    {
        return _levelData->failRewardId.GetRow()->id;
    }
    else
    {
        return INVALID_REWARD_ID;
    }
}

PassivityId FGsSpiritShotLevelData::GetPassivityEffectStatSetId() const
{
    if (_levelData->passivityId.GetRow() != nullptr)
    {
        return _levelData->passivityId.GetRow()->id;
    }
    else
    {
        return INVALID_PASSIVITY_ID;
    }
}

FSoftObjectPath FGsSpiritShotLevelData::GetStepImagePath() const
{
    return _levelData->stepImagePath;
}

TSharedRef<FGsSpiritShotStatData> FGsSpiritShotLevelData::GetStatData() const
{
    return _statData;
}

SpiritShotLevelId FGsSpiritShotLevelData::GetNextLevelId() const
{
    return _nextLevelId;
}

int32 FGsSpiritShotLevelData::GetRequireGold() const
{
    return _levelData->requireGold;
}

void FGsSpiritShotLevelData::SetNextLevelId(int32 InLevel)
{
    _nextLevelId = InLevel;
}

void FGsSpiritShotLevelData::SetStatData(const TArray<PassivityId>& InIdList)
{
    _statData->SetStatData(InIdList);
}

TSharedRef<FGsCostPackage> FGsSpiritShotLevelData::CreateItemCostPackage(int32 InModifyCount /*= 0*/) const
{
    // InModifyCount 의 경우, 기본값이 0일 것임
    TSharedRef<FGsCostPackage> costPackage = MakeShared<FGsCostPackage>(ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT);
    
        // 재료 후보들을 합산해서 계산할 것인지
    costPackage->SetJointCostCandidateElements(true);

    // 아이템 추가 (골드는 따로 계산)
    for (const SpiritShotEnchantCost& requireItem : _enchantRequireItemList)
    {
        TArray<ItemId> itemIdList;
        for (const int32 requireItemId : requireItem._itemIdList)
        {
            itemIdList.Emplace(StaticCast<ItemId>(requireItemId));
        }

        costPackage->AddCostCandidates(itemIdList, requireItem._itemAmount + requireItem._modifyItemAmount * InModifyCount);
    }

    return MoveTemp(costPackage);
}

TSharedRef<FGsCostPackage> FGsSpiritShotLevelData::CreateTotalCostPackage(int32 InModifyCount /*= 0*/) const
{
    // InModifyCount 의 경우, 기본값이 0일 것임
    TSharedRef<FGsCostPackage> costPackage = CreateItemCostPackage(InModifyCount);

    // 골드 추가
    costPackage->AddCost(CurrencyType::GOLD, GetRequireGold());

    return MoveTemp(costPackage);
}

TArray<SpiritShotEnchantCost> FGsSpiritShotLevelData::MakeRequireItemList(const FGsSchemaSpiritShotLevel* InLevelData)
{
    if (nullptr == InLevelData)
    {
        GSLOG(Error, TEXT("nullptr == InLevelData"));
        return TArray<SpiritShotEnchantCost>();
    }

    TArray<SpiritShotEnchantCost> requireItemList;
    for (const FGsSchemaSpiritShotRequirement& requirement : InLevelData->requireItemList)
    {
        requireItemList.Emplace(SpiritShotEnchantCost(requirement.itemIds, requirement.amount, requirement.modifyItemAmount));
    }

    return MoveTemp(requireItemList);
}