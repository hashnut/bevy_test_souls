#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

struct FGsStatFilterCategoryItem
{
	int32 _order = 0;
	FText _categoryName = FText::GetEmpty();
	TArray<StatType> _statSet;
};

struct FGsStatFilterItem
{
	StatType _type = StatType::NONE;		// 2024/1/16 PKT - 스탯 타입
	FText _name = FText::GetEmpty();		// 2024/1/16 PKT - 스탯 이름(화면 출력 용)
	FString _cachedTrimName;				// 2024/1/16 PKT - 스탯 이름(검색 용)
	//bool _isChecked = false;				// 2024/1/16 PKT - 선택
};

struct FGsStatFilterSimpleData
{
	StatType _type = StatType::NONE;		// 2024/1/16 PKT - 스탯 타입
	bool _isChecked = false;				// 2024/1/16 PKT - 선택
};