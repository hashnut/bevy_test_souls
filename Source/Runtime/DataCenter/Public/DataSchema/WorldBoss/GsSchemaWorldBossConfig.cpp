/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/WorldBoss/GsSchemaWorldBossConfig.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaWorldBossConfig::FGsSchemaWorldBossConfig()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaWorldBossConfig)
	
	basicRewardMailType.DataTable = nullptr;
	rankRewardMailType.DataTable = nullptr;
	lastHitRewardMailType.DataTable = nullptr;
	consolationRewardMailType.DataTable = nullptr;
	DailyMaxRewardEarned = 0;
}
