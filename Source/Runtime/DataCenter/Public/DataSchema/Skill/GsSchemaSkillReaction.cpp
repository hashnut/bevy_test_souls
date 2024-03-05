/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillReaction.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaSkillReaction::FGsSchemaSkillReaction()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSkillReaction)
	
	id = 0;
	category = EReactionType::HIT;
	totalRuntime = 0.000000f;
	rootMotionId.DataTable = nullptr;
	physicsAnimation = true;
}
