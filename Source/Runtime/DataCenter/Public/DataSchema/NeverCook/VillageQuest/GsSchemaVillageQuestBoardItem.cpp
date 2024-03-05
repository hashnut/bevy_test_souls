/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/NeverCook/VillageQuest/GsSchemaVillageQuestBoardItem.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaVillageQuestBoardItem::FGsSchemaVillageQuestBoardItem()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaVillageQuestBoardItem)
	
	id = 0;
	ParentID = 0;
	QuestMainStory.DataTable = nullptr;
	QuestBoardType = QuestBoardType::NONE;
}
