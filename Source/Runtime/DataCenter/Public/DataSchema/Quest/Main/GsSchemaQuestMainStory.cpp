/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Quest/Main/GsSchemaQuestMainStory.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaQuestMainStory::FGsSchemaQuestMainStory()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaQuestMainStory)
	
	exposeQuest = true;
	id = 0;
	requireLevel = 0;
	mainRewardId.DataTable = nullptr;
	subRewardId.DataTable = nullptr;
	storyBeginSubDialogId.DataTable = nullptr;
	storyClearSubDialogId.DataTable = nullptr;
	postStoryEventActionGroupId.DataTable = nullptr;
}
