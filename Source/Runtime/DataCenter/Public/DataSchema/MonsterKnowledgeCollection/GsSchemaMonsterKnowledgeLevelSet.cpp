/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeLevelSet.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaMonsterKnowledgeLevelSet::FGsSchemaMonsterKnowledgeLevelSet()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaMonsterKnowledgeLevelSet)
	
	id = 0;
	knowledgeGainProbability = 0;
	knowledgeGainValue = 0;
	knowledgeSurplusProbability = 0;
	knowledgeSurplusItem.DataTable = nullptr;
	knowledgeSurplusItemAmount = 0;
}
