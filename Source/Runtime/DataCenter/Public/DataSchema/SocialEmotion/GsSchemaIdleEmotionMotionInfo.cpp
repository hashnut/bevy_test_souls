/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/SocialEmotion/GsSchemaIdleEmotionMotionInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaIdleEmotionMotionInfo::FGsSchemaIdleEmotionMotionInfo()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaIdleEmotionMotionInfo)
	
	emotion = UserSocialEmotion::IDLE_EMOTION;
	motion.DataTable = nullptr;
	randRate = 1.000000f;
	useIdleMotion = false;
}
