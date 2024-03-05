/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeCeiling.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaCostumeCeiling::FGsSchemaCostumeCeiling()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCostumeCeiling)
	
	grade = CostumeGrade::NONE;
	needItemId.DataTable = nullptr;
	maxCount = 60;
	successProbabilityId.DataTable = nullptr;
}
