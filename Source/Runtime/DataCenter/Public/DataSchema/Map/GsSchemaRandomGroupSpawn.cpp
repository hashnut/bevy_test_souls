/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaRandomGroupSpawn.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaRandomGroupSpawn::FGsSchemaRandomGroupSpawn()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaRandomGroupSpawn)
	
	randomGroupSpawnId = 0;
	mapId.DataTable = nullptr;
	randomType = RandomGroupSpawnType::NORMAL;
	selectCount = 0;
}
