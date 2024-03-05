/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/Resurrect/GsSchemaResurrectInfo.h"
#include "DataSchema/Resurrect/GsSchemaResurrectPaneltyInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaResurrectInfo::FGsSchemaResurrectInfo()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaResurrectInfo)
	
	ResurrectType = ResurrectType::NOW;
	currencyType = CurrencyType::NONE;
	cost = 0;
	hpRate = 0.000000f;
	mpRate = 0.000000f;
	invncibleTime = 0;
	canRecovery = false;
	recoveryTime = 0;
}
