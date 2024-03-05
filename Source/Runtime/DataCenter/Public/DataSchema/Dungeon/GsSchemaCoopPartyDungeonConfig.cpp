/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaCoopPartyDungeonConfig.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaCoopPartyDungeonConfig::FGsSchemaCoopPartyDungeonConfig()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCoopPartyDungeonConfig)
	
	weeklyRewardCount = 0;
	autoMatchingTimeS = 0;
	afterReadyKickTimeS = 0;
	callHelpCount = 0;
	resetRTT.DataTable = nullptr;
	entranceBanTimeS = 0;
	afterClearKickTimeS = 0;
	timerEventId.DataTable = nullptr;
	startDelayCount = 0;
}
