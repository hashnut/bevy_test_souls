/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Spawn/GsSchemaSpawnObjectShowHide.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaSpawnData::FGsSchemaSpawnData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSpawnData)
	
	SpawnGroupId = 0;
	SpawnGroupType = NpcSpawnGroupType::NONE;
	groupId = 0;
	byClient = false;
	byDefault = true;
	MapId.DataTable = nullptr;
	randomSpawnMinDistance = 0;
	lineId = 0;
	groupLineMove = true;
}
