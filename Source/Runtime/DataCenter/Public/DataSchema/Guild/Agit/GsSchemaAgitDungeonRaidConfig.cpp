/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Guild/Agit/GsSchemaAgitDungeonRaidConfig.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaAgitDungeonRaidConfig::FGsSchemaAgitDungeonRaidConfig()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaAgitDungeonRaidConfig)
	
	requiredGuildLevel = 0;
	raidMaxClearCount = 0;
	raidTicketCount = 0;
	raidDurationTimeM = 0;
	newMemberRestrictTimeM = 0;
	raidRewardPersonMailTypeId.DataTable = nullptr;
	raidOpenMinGrade = GuildMemberGradeType::UNKNOWN;
	tickerSoundStart.DataTable = nullptr;
	tickerSoundClear.DataTable = nullptr;
}
