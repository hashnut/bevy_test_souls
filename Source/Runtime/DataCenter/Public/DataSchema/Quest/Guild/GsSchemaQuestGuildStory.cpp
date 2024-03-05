/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Quest/Guild/GsSchemaQuestGuildStory.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaQuestGuildStory::FGsSchemaQuestGuildStory()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaQuestGuildStory)
	
	id = 0;
	grade = GuildQuestGrade::NORMAL;
	minLevel = 0;
	maxLevel = 0;
	questId.DataTable = nullptr;
	zealPointAmount = 0;
	probability = 0;
	mainRewardId.DataTable = nullptr;
	subRewardId.DataTable = nullptr;
}
