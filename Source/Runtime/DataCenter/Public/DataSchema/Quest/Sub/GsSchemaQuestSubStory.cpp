/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Quest/Sub/GsSchemaQuestSubStory.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaQuestSubStory::FGsSchemaQuestSubStory()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaQuestSubStory)
	
	exposeQuest = true;
	id = 0;
	requireLevel = 0;
	mainRewardId.DataTable = nullptr;
	subRewardId.DataTable = nullptr;
	questBeginNpcDialogId.DataTable = nullptr;
	requireMainStoryId = 0;
	requireSubStoryId = 0;
	isGoToNpcTeleportAble = true;
}
