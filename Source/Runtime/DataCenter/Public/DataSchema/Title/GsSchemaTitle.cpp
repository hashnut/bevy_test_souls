/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Title/GsSchemaTitle.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaTitle::FGsSchemaTitle()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaTitle)
	
	id = 0;
	grade = TitleGrade::NORMAL;
	equipSlotType = TitleEquipSlot::FRONT;
	isProgressDisplayFlag = false;
	retainType = TitleRetainType::PERMANENT;
	retainCheckType = TitleConditionCheckType::CREATE_CHARACTER;
	conditionType = TitleConditionType::NONE;
	conditionalCount = 0;
}
