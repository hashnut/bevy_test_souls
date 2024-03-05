/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/InterServer/GsSchemaInvadeMapData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaInvadeMapData::FGsSchemaInvadeMapData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaInvadeMapData)
	
	mapId.DataTable = nullptr;
	mapId.RowName = FName(TEXT("0"));
	portalUserCount = 0;
	portalSpawnCount = 0;
	portalSpawnProbability = 0;
	enterAllowLowLevel = 0;
	enterAllowHighLevel = 0;
	invadePassivityIdListIndexEnv = 0;
	defensePassivityIdListIndexEnv = 0;
	invadeGuardianSpawnGroupId.DataTable = nullptr;
	invadeMonsterSpawnCheckStartTimeS = 0;
	invadeMonsterSpawnCheckIntervalTimeS = 0;
	invadeMonsterSpawnSuccessRate = 0;
	InvadeMonsterRandomGroupSpawnId.DataTable = nullptr;
	invadeTreasureBoxSpawnCheckStartTimeS = 0;
	invadeTreasureBoxSpawnCheckIntervalTimeS = 0;
	invadeTreasureBoxRandomGroupSpawnId.DataTable = nullptr;
	invadeWagonRandomGroupSpawnId.DataTable = nullptr;
	invadeGuardianPassivityId.DataTable = nullptr;
	invadeGuardianDeathPenaltyAbnormalityId.DataTable = nullptr;
	invadeWagonRandomTableTotalCount = 0;
	invadeWagonRandomTableSpawnCount = 0;
	maxInvaderCount = 100;
}
