/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeCompose.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaCeilingItemData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaCostumeCompose::FGsSchemaCostumeCompose()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCostumeCompose)
	
	grade = CostumeGrade::NORMAL;
	costGold = 0;
	successProbabilityId.DataTable = nullptr;
	failureProbabilityId.DataTable = nullptr;
}
