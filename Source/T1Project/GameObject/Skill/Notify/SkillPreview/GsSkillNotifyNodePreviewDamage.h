// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GsSkillNotifyNodeCollision.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillNotifyPreviewDamage : public FGsSkillNotifyNodeCollision
{
	using Super = FGsSkillNotifyNodeCollision;

protected:
	const struct FGsSchemaSkillSet* _currentSkillData = nullptr;

public:
	FGsSkillNotifyPreviewDamage()			= default;
	virtual ~FGsSkillNotifyPreviewDamage() = default;
	explicit FGsSkillNotifyPreviewDamage(class UParticleSystem* ParticleEffect, const FSoftObjectPath& EffectPath);

public:
	virtual void Initialize(class UGsGameObjectBase* Owner, const class IGsSkillRunnerData* SkillRunnerData,
		const struct FGsSchemaSkillNotify* NotifyInfo, const struct FGsSkillNetData* NetData, float OffsetRate) override;
	virtual void Action(class UGsGameObjectBase* Owner) override;

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() { return TEXT("FGsSkillNotifyPreviewDamage"); }
#endif
};
