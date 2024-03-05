/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/CommunityGirl/Mission/Condition/GsSchemaCommunityEventMissionConditionQuest.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaCommunityEventMissionConditionQuest::FGsSchemaCommunityEventMissionConditionQuest()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCommunityEventMissionConditionQuest)
	
	questId.DataTable = nullptr;
	mapId.DataTable = nullptr;
	questType = QuestType::MAIN;
	repeatQuestGradeMin = RepeatStoryGrade::NORMAL;
	repeatQuestGradeMax = RepeatStoryGrade::NORMAL;
}
