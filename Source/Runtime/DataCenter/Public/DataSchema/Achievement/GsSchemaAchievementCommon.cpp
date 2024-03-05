/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Achievement/GsSchemaAchievementCommon.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaAchievementCommon::FGsSchemaAchievementCommon()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaAchievementCommon)
	
	id = 0;
	groupId = 0;
	level = 0;
	completePoint = 0;
	rewardId.DataTable = nullptr;
	achievementCondition.DataTable = nullptr;
	detail.DataTable = nullptr;
	isTheme = false;
}
