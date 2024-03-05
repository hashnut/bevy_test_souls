// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GsSkillNotifyNodeBeamParticle.h"

/**
 * 빔 파티클 처리 Notify
 */
class T1PROJECT_API FGsSkillNotifyNodePreviewBeamParticle : public FGsSkillNotifyNodeBeamParticle
{
	using Super = FGsSkillNotifyNodeBeamParticle;

public:	
	FGsSkillNotifyNodePreviewBeamParticle(const struct FGsSchemaSkillNotifyBeamParticle* BeamParticleTable);
	virtual ~FGsSkillNotifyNodePreviewBeamParticle() = default;

private:
	virtual class UGsGameObjectBase* FindBeamParticleTarget() override;

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() override { return TEXT("FGsSkillNotifyNodePreviewBeamParticle"); }
#endif
};
