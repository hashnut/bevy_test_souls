/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaFieldBossData::FGsSchemaFieldBossData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaFieldBossData)
	
	bossId = 0;
	bossType = BossType::COOPERATION;
	bossNpcId.DataTable = nullptr;
	mapId.DataTable = nullptr;
	worldMapMark = false;
	WarpSpotId = 0;
	spawnType = BossSpawnType::ALL;
	spawnChannelType = BossSpawnChannel::ALL;
	contributionMin = 0;
	spawnIntervalM = 0;
	spawnIntervalDeviationM = 0;
	spawnProbability = 0;
	despawnTimeM = 0;
	cameraRot = FRotator(0.000000, 0.000000, 0.000000);
	cameraMinDist = 250.000000f;
	cameraMaxDist = 500.000000f;
	tickerEventSpawnId.DataTable = nullptr;
	tickerEventDieId.DataTable = nullptr;
}
