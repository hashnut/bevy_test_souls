/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityStat.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
FGsSchemaAbnormalityStat::FGsSchemaAbnormalityStat()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaAbnormalityStat)
	
	methodType = AbnormalityEffectStatMethodType::NORMAL;
	proportionTargetType = SkillTargetType::TARGET;
	conditionValue = 0;
	affectedValue = 0;
}
