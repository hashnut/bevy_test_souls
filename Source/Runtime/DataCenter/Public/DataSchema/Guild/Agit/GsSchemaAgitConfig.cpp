/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Guild/Agit/GsSchemaAgitConfig.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaAgitConfig::FGsSchemaAgitConfig()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaAgitConfig)
	
	rentalRequireGuildLevel = 0;
	rentalRequireMemberGrade = GuildMemberGradeType::UNKNOWN;
	enterItem.DataTable = nullptr;
	returnItem.DataTable = nullptr;
	rentalGold = 0;
	rentalPeriodD = 0;
	rentalExtensionGold = 0;
	rentalExtensionPeriodD = 0;
	rentalExtensionMinD = 0;
	mapId.DataTable = nullptr;
	exitMapId.DataTable = nullptr;
}
