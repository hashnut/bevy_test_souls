/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyBeamParticle.h"
FGsSchemaSkillNotifyBeamParticle::FGsSchemaSkillNotifyBeamParticle()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSkillNotifyBeamParticle)
	
	targetType = SkillTargetType::CASTER;
	targetCreatureType = BP_EGsGameObjectType::Projectile;
	targetCreatureId = 0;
	targetSocketName = FName(TEXT("HitEffectSocket"));
	duration = 0.000000f;
}
