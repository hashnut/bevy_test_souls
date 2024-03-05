/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyCompose.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaCeilingItemData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaFairyCompose::FGsSchemaFairyCompose()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaFairyCompose)
	
	grade = FairyGrade::NORMAL;
	costGold = 0;
	upgradeRateList = TArray<FGsSchemaFairyComposeRate> ();
	upgradeRateList.AddUninitialized(3);
	FGsSchemaFairyComposeRate::StaticStruct()->InitializeStruct(upgradeRateList.GetData(), 3);
	auto& __Local__0 = upgradeRateList[0];
	auto& __Local__1 = upgradeRateList[1];
	__Local__1.materialCount = 5;
	auto& __Local__2 = upgradeRateList[2];
	__Local__2.materialCount = 6;
	successProbabilityId.DataTable = nullptr;
	failureProbabilityId.DataTable = nullptr;
}
