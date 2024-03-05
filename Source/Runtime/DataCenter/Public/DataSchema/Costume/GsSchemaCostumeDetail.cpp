/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeDetail.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaCostumeDetail::FGsSchemaCostumeDetail()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCostumeDetail)
	
	portrait = {};
	portrait.Reserve(1);
	portrait.Add(CreatureGenderType::ALL, FSoftObjectPath(TEXT("")));
	applyCustomizedPelvisSize = true;
	shapeInfo.DataTable = nullptr;
}
