/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityEffectInvokeCooldownSetExcel.h"
FGsSchemaPassivityEffectInvokeCooldownSetExcel::FGsSchemaPassivityEffectInvokeCooldownSetExcel()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaPassivityEffectInvokeCooldownSetExcel)
	
	id = 0;
	ratePct = 0;
	cause = PassivityCauseType::NONE;
	CauseInfoDataId = FName();
	cooldownTime = 0.000000f;
	target = PassivityInvokeTargetType::SELF;
	targetAllUserSkillSetId = false;
	targetCooldownRate = 0;
	causeParamId = 0;
	causeParam1 = AbnormalityEffectType::NONE;
	causeParam2 = ItemCategorySub::NONE;
	causeParam3 = ItemGrade::NONE;
}
