/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyMoveLocation.h"
FGsSchemaSkillNotifyMoveLocation::FGsSchemaSkillNotifyMoveLocation()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSkillNotifyMoveLocation)
	
	centerType = SkillAreaCenter::TARGET;
	moveType = SkillNotifyMoveType::NEAR_TARGET;
	postDirectionType = SkillNotifyPostDirectionType::TO_TARGET;
	distance = 0;
	moveSpeed = 0;
	duration = 1.000000f;
}
