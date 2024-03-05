/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/PlayableEvent/GsSchemaPlayableEventQuest.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaPlayableEventQuest::FGsSchemaPlayableEventQuest()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaPlayableEventQuest)
	
	id = 0;
	goalType = EGsPlayableEventQuestGoalType::Move;
	movePos = FVector(0.000000, 0.000000, 0.000000);
	killCount = 0;
	subDialogData.DataTable = nullptr;
	acceptanceRadius = 0.000000f;
	sequenceId = 0;
	waitTimeSecond = 0.000000f;
	lockInput = false;
}
