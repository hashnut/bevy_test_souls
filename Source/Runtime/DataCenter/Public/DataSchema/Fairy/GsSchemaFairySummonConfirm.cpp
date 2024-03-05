/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairySummonConfirm.h"
FGsSchemaFairySummonConfirm::FGsSchemaFairySummonConfirm()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaFairySummonConfirm)
	
	id = 0;
	duration = 0;
	currencyInfo = TArray<FGsSchemaFairySummonChangeCurrencyInfo> ();
	currencyInfo.AddUninitialized(1);
	FGsSchemaFairySummonChangeCurrencyInfo::StaticStruct()->InitializeStruct(currencyInfo.GetData(), 1);
	auto& __Local__0 = currencyInfo[0];
}
