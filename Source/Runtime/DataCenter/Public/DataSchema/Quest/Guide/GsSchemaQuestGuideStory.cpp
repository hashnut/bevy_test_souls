/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Quest/Guide/GsSchemaQuestGuideStory.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaQuestGuideStory::FGsSchemaQuestGuideStory()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaQuestGuideStory)
	
	id = 0;
	requireLevel = 0;
	requireMainStoryId.DataTable = nullptr;
	questId.DataTable = nullptr;
	mainRewardId.DataTable = nullptr;
	subRewardId.DataTable = nullptr;
	guideQuestWindow = EGsGuideQuestWindowType::ACTIVE_HUD;
}
