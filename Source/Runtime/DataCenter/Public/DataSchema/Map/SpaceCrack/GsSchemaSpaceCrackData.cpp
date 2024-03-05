/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackData.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Wave/GsSchemaMonsterSpawnRate.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Wave/GsSchemaMonsterSpawnRateGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Wave/GsSchemaMonsterWave.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaSpaceCrackData::FGsSchemaSpaceCrackData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSpaceCrackData)
	
	id = 0;
	mapId.DataTable = nullptr;
	spawnGroupId = 0;
	spawnLevel = 0;
	duration = 0;
	despawnTime = 0;
	monsterPollutantId.DataTable = nullptr;
}
