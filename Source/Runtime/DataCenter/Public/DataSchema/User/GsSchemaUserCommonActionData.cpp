/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserCommonActionData.h"
#include "Runtime/DataCenter/Public/DataSchema/SocialEmotion/GsSchemaIdleEmotionMotionInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaUserCommonActionData::FGsSchemaUserCommonActionData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaUserCommonActionData)
	
	genderType = CreatureGenderType::ALL;
	commonActionInfo = TArray<FGsSchemaUserCommonActionInfo> ();
	commonActionInfo.AddUninitialized(1);
	FGsSchemaUserCommonActionInfo::StaticStruct()->InitializeStruct(commonActionInfo.GetData(), 1);
	auto& __Local__0 = commonActionInfo[0];
}
