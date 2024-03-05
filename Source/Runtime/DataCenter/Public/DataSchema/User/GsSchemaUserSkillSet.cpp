/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserSkillSet.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaUserSkillSet::FGsSchemaUserSkillSet()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaUserSkillSet)
	
	id = 0;
	maleSkillId.DataTable = nullptr;
	femaleSkillId.DataTable = nullptr;
	acquisitionId = 0;
	defaultSlotIndex = 0;
	addTargetButton = false;
	tier = 0;
}
