/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaUserLookInfo.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaCostumeCommon::FGsSchemaCostumeCommon()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCostumeCommon)
	
	id = 0;
	grade = CostumeGrade::NONE;
	genderType = CreatureGenderType::ALL;
	statPassivityId.DataTable = nullptr;
	tier = 0;
	detail.shapeInfo.DataTable = nullptr;
	useAppearance = true;
	weaponType = TArray<CreatureWeaponType> ();
	weaponType.Reserve(1);
	weaponType.Add(CreatureWeaponType::NONE);
}
