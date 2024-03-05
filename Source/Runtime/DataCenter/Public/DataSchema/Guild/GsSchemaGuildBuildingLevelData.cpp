/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/Guild/GsSchemaGuildBuildingLevelData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaGuildBuildingLevelData::FGsSchemaGuildBuildingLevelData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaGuildBuildingLevelData)
	
	level = 0;
	needLevel = 0;
	levelUpCost = 0;
	levelUpSecond = 0;
	buffActiveCostType = CurrencyType::NONE;
	buffActiveCost = 0;
	guildStatValue = 0;
}
