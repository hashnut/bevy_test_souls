/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/NeverCook/Title/GsSchemaTitleExcel.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaTitleExcel::FGsSchemaTitleExcel()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaTitleExcel)
	
	id = 0;
	subCategoryRowId.DataTable = nullptr;
	grade = TitleGrade::NORMAL;
	equipSlotType = TitleEquipSlot::FRONT;
	passivitySetId.DataTable = nullptr;
	isProgressDisplayFlag = false;
	retainType = TitleRetainType::PERMANENT;
	retainCheckType = TitleConditionCheckType::CREATE_CHARACTER;
	conditionType = TitleConditionType::NONE;
	conditionalCount = 0;
}
