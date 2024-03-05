/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyComposeRate.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaFairyComposeRate::FGsSchemaFairyComposeRate()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaFairyComposeRate)
	
	materialCount = 4;
	upgradeRate = 0.000000f;
	ceilingItemDataList = TArray<FGsSchemaCeilingItemData> ();
	ceilingItemDataList.AddUninitialized(2);
	FGsSchemaCeilingItemData::StaticStruct()->InitializeStruct(ceilingItemDataList.GetData(), 2);
	auto& __Local__0 = ceilingItemDataList[0];
	__Local__0.ceilingItemCount = 3;
	__Local__0.ceilingItemRate = 1.000000f;
	auto& __Local__1 = ceilingItemDataList[1];
	__Local__1.ceilingItemCount = 1;
	__Local__1.ceilingItemRate = 0.300000f;
}
