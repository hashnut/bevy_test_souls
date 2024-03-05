/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuestObjective.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaQuestObjective::FGsSchemaQuestObjective()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaQuestObjective)
	
	id = 0;
	type = QuestObjectiveType::KILL;
	objectiveClearSubDialogId.DataTable = nullptr;
	isTickerHide = false;
	isCountHide = false;
}
