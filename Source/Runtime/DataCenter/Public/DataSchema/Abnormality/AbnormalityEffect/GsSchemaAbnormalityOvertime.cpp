/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityOvertime.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
FGsSchemaAbnormalityOvertime::FGsSchemaAbnormalityOvertime()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaAbnormalityOvertime)
	
	targetType = SkillTargetType::CASTER;
	effectMethodType = AbnormalityEffectAddSubMethodType::ADD_SUB;
	value = 0;
	damageCategory = DamageStatType::NONE;
	maxTickDamage = 0;
	maxTickDamagePC = 0;
}
