/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuest.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaQuest::FGsSchemaQuest()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaQuest)
	
	id = 0;
	type = QuestType::MAIN;
	objectiveProcessType = QuestObjectiveProcessType::PARALLER;
	isTeleportAble = true;
	isAutoPlayAble = true;
	teleportGold = 0;
	isVolatile = false;
	questContentsType = QuestContentsType::ALL;
	questBeginSubDialogId.DataTable = nullptr;
	questClearSubDialogId.DataTable = nullptr;
	questClearNpcDialogId.DataTable = nullptr;
	preQuestEventActionGroupId.DataTable = nullptr;
	postQuestEventActionGroupId.DataTable = nullptr;
	preEventActionGroupId = 0;
	postEventActionGroupId = 0;
}
