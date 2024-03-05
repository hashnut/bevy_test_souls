/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyCommon.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaFairyCommon::FGsSchemaFairyCommon()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaFairyCommon)
	
	id = 0;
	grade = FairyGrade::NORMAL;
	tier = 0;
	levelId.DataTable = nullptr;
	detailID.DataTable = nullptr;
	detailID.RowName = FName(TEXT("100"));
}
