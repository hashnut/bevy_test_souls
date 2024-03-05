/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Quest/Sub/GsSchemaQuestSubChapter.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaQuestSubChapter::FGsSchemaQuestSubChapter()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaQuestSubChapter)
	
	id = 0;
	exposeQuest = true;
	npcId.DataTable = nullptr;
	npcIcon.DataTable = nullptr;
	questContentsType = QuestContentsType::NORMAL;
}
