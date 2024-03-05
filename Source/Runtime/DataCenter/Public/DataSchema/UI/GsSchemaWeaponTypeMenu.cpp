/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/UI/GsSchemaWeaponTypeMenu.h"
FGsSchemaWeaponTypeMenu::FGsSchemaWeaponTypeMenu()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaWeaponTypeMenu)
	
	id = 0;
	type = CreatureWeaponType::NONE;
	iconInfo = TArray<FGsSchemaIconImgInfo> ();
	iconInfo.AddUninitialized(1);
	FGsSchemaIconImgInfo::StaticStruct()->InitializeStruct(iconInfo.GetData(), 1);
	auto& __Local__0 = iconInfo[0];
	useMenu = true;
	order = 0;
}
