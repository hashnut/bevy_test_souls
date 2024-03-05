/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyLevelInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaFairyLevelInfo::FGsSchemaFairyLevelInfo()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaFairyLevelInfo)
	
	level = 0;
	addStatPassivityId.DataTable = nullptr;
	vehiclePassivityId.DataTable = nullptr;
	vehicleVisualAbnormality.DataTable = nullptr;
	fairyOnSkillTalk = TArray<FGsSchemaFairyTalk> ();
	fairyOnSkillTalk.AddUninitialized(1);
	FGsSchemaFairyTalk::StaticStruct()->InitializeStruct(fairyOnSkillTalk.GetData(), 1);
	auto& __Local__0 = fairyOnSkillTalk[0];
}
