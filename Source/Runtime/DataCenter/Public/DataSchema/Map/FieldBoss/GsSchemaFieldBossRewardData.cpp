/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossRewardData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaFieldBossRewardData::FGsSchemaFieldBossRewardData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaFieldBossRewardData)
	
	bossRewardId = 0;
	mailTypeId = 0;
	giveType = BossRewardGiveType::NONE;
	contributionRankMin = 1;
	contributionRankMax = 100;
}
