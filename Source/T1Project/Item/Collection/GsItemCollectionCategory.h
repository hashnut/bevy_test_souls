#pragma once

/**
* file		GsItemCollectionCategory.h
* @brief	ItemCollection 에서 사용 하게 될 Category Data
* @author	PKT
* @date		2021/07/15
* */
#include "CoreMinimal.h"
#include "Item//Collection/EItemCollectionEnumData.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Item/Collection/EItemCollectionSubCategoryStatUnit.h"

/**
 * 2021/07/17 PKT - ItemCollection Window에서 사용
 */
struct FGsItemCollectionCategory
{
	const struct FGsSchemaBase* _category = nullptr;
	FText _name = FText::GetEmpty();
	TArray<FGsItemCollectionCategory> _subCategory;
};


/**
 * 2021/07/17 PKT - Item Collection Tray 에서만 사용
 */
struct FGsItemCollectionCategoryTrayOnly
{
	EItemCollectionSubCategoryStatUnit _type = EItemCollectionSubCategoryStatUnit::None;
	FText _name = FText::GetEmpty();
	TSet<StatType> _statTypes;
};