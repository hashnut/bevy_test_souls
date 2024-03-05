#include "GsSpiritShotCategoryData.h"
#include "GsSpiritShotStatData.h"

#include "DataSchema/SpiritShot/GsSchemaSpiritShotLevel.h"
#include "DataSchema/SpiritShot/GsSchemaSpiritShotCategory.h"


FGsSpiritShotCategoryData::FGsSpiritShotCategoryData(const FGsSchemaSpiritShotCategory* InSpiritShotCategory, TSharedRef<FGsSpiritShotLevelData> InLevelData, EGsSpiritShotEnchantState InState /* = EGsSpiritShotEnchantState::UnKnown */) :
	_categoryData(InSpiritShotCategory),
	_levelData(InLevelData),
	_currentState(InState)
{
	check(nullptr != InSpiritShotCategory);
}

int32 FGsSpiritShotCategoryData::GetId() const
{
	return _categoryData->id;
}

FText FGsSpiritShotCategoryData::GetCategoryName() const
{
	return _categoryData->categoryName;
}

int32 FGsSpiritShotCategoryData::GetUnlockContentsId() const
{
	if (_categoryData->unlockContentsId.GetRow() != nullptr)
	{
		return _categoryData->unlockContentsId.GetRow()->id;
	}
	else
	{
		return INVALID_UNLOCK_CONTENTS_ID;
	}
}

FSoftObjectPath FGsSpiritShotCategoryData::GetCoreImagePath() const
{
	return _categoryData->coreImage;
}

TSharedRef<FGsSpiritShotLevelData> FGsSpiritShotCategoryData::GetLevelData() const
{
	return _levelData;
}

EGsSpiritShotEnchantState FGsSpiritShotCategoryData::GetCurrentState() const
{
	return _currentState;
}

void FGsSpiritShotCategoryData::SetLevelData(TSharedRef<FGsSpiritShotLevelData> InLevelData)
{
	_levelData = InLevelData;
}

void FGsSpiritShotCategoryData::SetCurrentState(EGsSpiritShotEnchantState InState)
{
	_currentState = InState;
}