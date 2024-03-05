/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInvokeSkillNotifyParam.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaPassivityEffectInvokeSkillNotifyParam::FGsSchemaPassivityEffectInvokeSkillNotifyParam()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaPassivityEffectInvokeSkillNotifyParam)
	
	ratePct = 0;
	rateCondition = 0;
	cause = PassivityCauseType::HIT_TARGET_NORMAL;
	cooldownTime = 0.000000f;
	target = PassivityInvokeTargetType::SELF;
	skillNotifyId.DataTable = nullptr;
}
