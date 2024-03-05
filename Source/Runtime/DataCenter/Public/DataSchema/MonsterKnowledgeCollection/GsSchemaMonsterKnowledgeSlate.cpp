/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlate.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaMonsterKnowledgeSlate::FGsSchemaMonsterKnowledgeSlate()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaMonsterKnowledgeSlate)
	
	id = 0;
	grade = MonsterKnowledgeSlateGrade::NORMAL;
	beginNode.DataTable = nullptr;
	categoryType = EMonsterKnowledgeSlateCategory::ATTACK;
}
