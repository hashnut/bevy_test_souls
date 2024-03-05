/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaItemCommon::FGsSchemaItemCommon()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaItemCommon)
	
	id = 0;
	type = ItemType::NONE;
	detailId = 0;
	categoryMain = ItemCategoryMain::NONE;
	categorySub = ItemCategorySub::NONE;
	grade = ItemGrade::NORMAL;
	minLevel = 0;
	maxLevel = 0;
	useLevel = 0;
	genderType = CreatureGenderType::ALL;
	maxStackCount = 0;
	sellPrice = 0;
	storeSellable = false;
	tradable = false;
	personalTradable = false;
	warehouseStorable = false;
	decomposable = false;
	weight = 0;
	acquireType = ItemAcquireType::IN_GAME;
	expiredTimeId = 0;
	supplyInfoConstraint.DataTable = nullptr;
	demandInfoConstraint.DataTable = nullptr;
	belongingItemId = 0;
	itemGroupId = 0;
	canDelete = true;
	iconSlotEffectType = EGsItemIconSlotEffectType::None;
}
