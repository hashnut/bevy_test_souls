/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInvokeCooldownParam.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaPassivityEffectInvokeCooldownParam::FGsSchemaPassivityEffectInvokeCooldownParam()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaPassivityEffectInvokeCooldownParam)
	
	ratePct = 0;
	cause = PassivityCauseType::NONE;
	cooldownTime = 0.000000f;
	target = PassivityInvokeTargetType::SELF;
	targetAllUserSkillSetId = false;
	targetCooldownRate = 0;
}
