/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHeal.h"
FGsSchemaSkillNotifyHeal::FGsSchemaSkillNotifyHeal()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSkillNotifyHeal)
	
	healStatType = DamageStatType::HP;
	target = SkillTargetType::CASTER;
	healRatioValueType = HealRatioValueType::NONE;
	healRatioStatValue = 0;
	healFixedStatValue = 0;
}
