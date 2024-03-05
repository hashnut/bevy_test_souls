/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassMissionCommon.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaBattlePassMissionCommon::FGsSchemaBattlePassMissionCommon()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaBattlePassMissionCommon)
	
	passMissionId = 0;
	passMissionRewardExp = 0;
	passMissionCondition.DataTable = nullptr;
	passMissionCount = 0;
	detail.DataTable = nullptr;
	rewardDetail.DataTable = nullptr;
}
