#pragma once

#include "GameObject/Skill/Notify/SkillPreview/GsSkillNotifyNodePreviewDamage.h"

class UParticleSystem;
class UGsGameObjectBase;

class FGsSkillNotifyNodePlayableEventDamage : public FGsSkillNotifyPreviewDamage
{
	using Super = FGsSkillNotifyPreviewDamage;
public:
	FGsSkillNotifyNodePlayableEventDamage() = default;
	virtual ~FGsSkillNotifyNodePlayableEventDamage() = default;
	explicit FGsSkillNotifyNodePlayableEventDamage( UParticleSystem* ParticleEffect, const FSoftObjectPath& EffectPath);

public:
	virtual void Action( UGsGameObjectBase* Owner) override;
};