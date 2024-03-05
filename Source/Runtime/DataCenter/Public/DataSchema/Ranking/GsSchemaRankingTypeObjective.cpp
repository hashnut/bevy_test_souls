/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaRankingTypeObjective::FGsSchemaRankingTypeObjective()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaRankingTypeObjective)
	
	index = 0;
	rankingType = RankType::NONE;
	rankingSearchType = RankSearchType::PERSONAL;
	serverRangeType = RankServerRangeType::LOCAL;
	maxRankCount = 0;
	useBadge = false;
}
