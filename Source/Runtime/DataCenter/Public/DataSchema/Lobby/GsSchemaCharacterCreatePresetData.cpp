/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Lobby/GsSchemaCharacterCreatePresetData.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaCharacterCreatePresetData::FGsSchemaCharacterCreatePresetData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCharacterCreatePresetData)
	
	id = 0;
	genderType = CreatureGenderType::ALL;
	customizePreset.DataTable = nullptr;
	bodyPartsShapeId = 0;
}
