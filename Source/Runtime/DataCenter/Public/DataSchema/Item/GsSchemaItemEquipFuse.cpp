/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemEquipFuse.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaItemEquipFuse::FGsSchemaItemEquipFuse()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaItemEquipFuse)
	
	fuseItemCategorySub = ItemCategorySub::NONE;
	fuseItemId.DataTable = nullptr;
	fuseMinEnchantLevel = 0;
	fuseResultItemId.DataTable = nullptr;
	fuseCost = 0;
}
