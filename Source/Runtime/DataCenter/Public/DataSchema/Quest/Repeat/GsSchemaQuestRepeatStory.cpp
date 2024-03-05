/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaQuestRepeatStory::FGsSchemaQuestRepeatStory()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaQuestRepeatStory)
	
	id = 0;
	repeatStoryType = RepeatStoryType::SCROLL;
	iconRes.DataTable = nullptr;
	iconRes.RowName = FName(TEXT("NewRow"));
	repeatStoryGrade = RepeatStoryGrade::NORMAL;
	probability = 0;
	requireLevel = 0;
	mainRewardId.DataTable = nullptr;
	subRewardId.DataTable = nullptr;
}
