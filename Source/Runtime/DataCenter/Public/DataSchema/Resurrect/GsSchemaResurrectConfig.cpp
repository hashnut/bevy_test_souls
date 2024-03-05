/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Resurrect/GsSchemaResurrectConfig.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaResurrectConfig::FGsSchemaResurrectConfig()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaResurrectConfig)
	
	freeReviveCount = 5;
	MinimumLevel = 10;
	maxExpPenaltyCount = 30;
	maxItemPenaltyCount = 30;
	autoRespawnSecond = 0;
	InstantResurrectionSecond = 0;
	spaceCrackRespawnSecond = 5;
	resurrectItem.DataTable = nullptr;
	maxChargeCount = 0;
}
