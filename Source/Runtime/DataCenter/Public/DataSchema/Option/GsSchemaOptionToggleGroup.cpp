/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionToggleGroup.h"
#include "Runtime/DataCenter/Public/Option/GsOptionEnumType.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
FGsSchemaOptionToggleGroup::FGsSchemaOptionToggleGroup()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaOptionToggleGroup)
	
	optionItemType = EGsOptionItem::None;
	toggleList = TArray<FGsSchemaOptionToggle> ();
	toggleList.AddUninitialized(1);
	FGsSchemaOptionToggle::StaticStruct()->InitializeStruct(toggleList.GetData(), 1);
	auto& __Local__0 = toggleList[0];
	isTooltip = false;
}
