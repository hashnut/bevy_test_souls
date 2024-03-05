/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Skill/Param/GsSchemaSkillExecuteConditionMethodStat.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
FGsSchemaSkillExecuteConditionMethodStat::FGsSchemaSkillExecuteConditionMethodStat()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSkillExecuteConditionMethodStat)
	
	target = SkillTargetType::CASTER;
	statCondition = SkillExecuteConditionStatConditionMethod::LESS;
	type = SkillExecuteConditionStatConditionType::RATIO;
	param = 0;
}
