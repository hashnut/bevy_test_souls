// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketFilterData.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemStat.h"


void FGsMarketFilterData::Clear()
{	
	_gradeList.Empty();

	ClearEnchantLevel();
	ClearShield();

	_enchantPropList.Empty();
	_refineStatList.Empty();
}

void FGsMarketFilterData::ClearEnchantLevel()
{
	_enchantLevelMin = UINT16_MAX;
	_enchantLevelMax = UINT16_MAX;
}

void FGsMarketFilterData::ClearShield()
{
	_shieldMin = UINT8_MAX;
	_shieldMax = UINT8_MAX;
}

void FGsMarketFilterData::Set(const FGsMarketFilterData& InData)
{
	_gradeList.Empty();
	_gradeList.Append(InData._gradeList);

	_enchantLevelMin = InData._enchantLevelMin;
	_enchantLevelMax = InData._enchantLevelMax;

	_shieldMin = InData._shieldMin;
	_shieldMax = InData._shieldMax;

	_enchantPropList.Empty();
	_enchantPropList.Append(InData._enchantPropList);

	_refineStatList.Empty();
	_refineStatList.Append(InData._refineStatList);
}

bool FGsMarketFilterData::IsFilter() const
{
	if (IsFilterGrade())
	{
		return true;
	}

	if (IsFilterLevel())
	{
		return true;
	}

	if (IsFilterShield())
	{
		return true;
	}

	if (0 < _enchantPropList.Num())
	{
		return true;
	}

	if (0 < _refineStatList.Num())
	{
		return true;
	}

	return false;
}

bool FGsMarketFilterData::IsFilterGrade() const
{
	if (0 < _gradeList.Num())
	{
		return true;
	}

	return false;
}

bool FGsMarketFilterData::IsFilterLevel() const
{
	if (UINT16_MAX != _enchantLevelMin ||
		UINT16_MAX != _enchantLevelMax)
	{
		return true;
	}

	return false;
}

bool FGsMarketFilterData::IsFilterShield() const
{
	if (UINT8_MAX != _shieldMin ||
		UINT8_MAX != _shieldMax)
	{
		return true;
	}

	return false;
}

bool FGsMarketFilterData::IsSameGradeFilter(const TArray<ItemGrade>& InGradeList)
{
	if (_gradeList.Num() != InGradeList.Num())
	{
		return false;
	}

	for (int32 i = 0; i < InGradeList.Num(); ++i)
	{
		if (false == _gradeList.Contains(InGradeList[i]))
		{
			return false;
		}
	}

	return true;
}

bool FGsMarketFilterData::CheckByGrade(ItemGrade InGrade)
{
	if (0 < _gradeList.Num())
	{
		if (false == _gradeList.Contains(InGrade))
		{
			return false;
		}
	}

	return true;
}

bool FGsMarketFilterData::CheckByLevel(Level InEnchantLevel)
{
	if (IsFilterLevel())
	{
		if (_enchantLevelMin > InEnchantLevel ||
			_enchantLevelMax < InEnchantLevel)
		{
			return false;
		}
	}

	return true;
}

bool FGsMarketFilterData::CheckItem(const FGsItem* InItem)
{
	if (false == CheckByGrade(InItem->GetGradeType()))
	{
		return false;
	}

	if (false == CheckByLevel(InItem->GetLevel()))
	{
		return false;
	}

	if (ItemType::EQUIP == InItem->GetType())
	{
		FGsItem* item = const_cast<FGsItem*>(InItem);
		FGsItemEquipment* itemEquip = item->GetCastItem<FGsItemEquipment>();
		if (nullptr == itemEquip)
		{
			return false;
		}

		if (false == CheckByShield(itemEquip))
		{
			return false;
		}

		if (false == CheckByEnchantProp(itemEquip))
		{
			return false;
		}

		if (false == CheckByRefineStat(itemEquip))
		{
			return false;
		}
	}	

	return true;
}

bool FGsMarketFilterData::CheckByShield(const FGsItemEquipment* InItem)
{
	if (IsFilterShield())
	{
		if (_shieldMin > InItem->GetEnchantShieldCount() ||
			_shieldMax < InItem->GetEnchantShieldCount())
		{
			return false;
		}
	}

	return true;
}

bool FGsMarketFilterData::CheckByEnchantProp(const FGsItemEquipment* InItem)
{
	if (0 < _enchantPropList.Num())
	{
		return _enchantPropList.Contains(InItem->GetBonusStatType());
	}

	return true;
}

bool FGsMarketFilterData::CheckByRefineStat(FGsItemEquipment* InItem)
{
	if (0 < _refineStatList.Num())
	{
		TArray<FGsItemStat*> statDataList;
		if (InItem->GetAllRefineOptionStatList(statDataList))
		{
			for (FGsItemStat* iter : statDataList)
			{
				if (_refineStatList.Contains(iter->GetStatType()))
				{
					return true;
				}
			}
		}

		return false;
	}

	return true;
}