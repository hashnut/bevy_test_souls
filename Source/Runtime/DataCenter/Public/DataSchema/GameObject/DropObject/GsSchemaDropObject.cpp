/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/GameObject/DropObject/GsSchemaDropObject.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "GameObject/Define/EGsDropItemCategory.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaDropObject::FGsSchemaDropObject()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaDropObject)
	
	id = EGsDropItemCategory::Weapon;
}
