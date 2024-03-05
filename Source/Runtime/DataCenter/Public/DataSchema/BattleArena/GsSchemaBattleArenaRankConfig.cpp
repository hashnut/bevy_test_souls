/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/BattleArena/GsSchemaBattleArenaRankConfig.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaBattleArenaRankConfig::FGsSchemaBattleArenaRankConfig()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaBattleArenaRankConfig)
	
	rankSearchType = RankSearchType::PERSONAL;
	seasonDataId.DataTable = nullptr;
	rankObjectiveId.DataTable = nullptr;
}
