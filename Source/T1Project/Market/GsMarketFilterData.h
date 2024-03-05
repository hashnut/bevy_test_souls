// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "DataSchema/GsSchemaEnums.h"

class FGsItem;
class FGsItemEquipment;

/**
 * 거래소 필터링 정보
 */
struct T1PROJECT_API FGsMarketFilterData final
{
	TArray<ItemGrade> _gradeList;
	Level _enchantLevelMin = UINT16_MAX;
	Level _enchantLevelMax = UINT16_MAX;
	EnchantShield _shieldMin = UINT8_MAX;
	EnchantShield _shieldMax = UINT8_MAX;
	TArray<EGsItemEnchantBonusType> _enchantPropList;
	TArray<StatType> _refineStatList;

public:
	void Clear();
	void ClearEnchantLevel();
	void ClearShield();

	void Set(const FGsMarketFilterData& InData);

	bool IsFilter() const;
	bool IsFilterGrade() const;
	bool IsFilterLevel() const;
	bool IsFilterShield() const;

	bool IsSameGradeFilter(const TArray<ItemGrade>& InGradeList);
	
	// T: 대상, F: 대상아님
	bool CheckByGrade(ItemGrade InGrade);	
	bool CheckByLevel(Level InEnchantLevel);
	bool CheckItem(const FGsItem* InItem);

private:
	// T: 대상, F: 대상아님
	bool CheckByShield(const class FGsItemEquipment* InItem);
	bool CheckByEnchantProp(const class FGsItemEquipment* InItem);
	bool CheckByRefineStat(class FGsItemEquipment* InItem);
};
