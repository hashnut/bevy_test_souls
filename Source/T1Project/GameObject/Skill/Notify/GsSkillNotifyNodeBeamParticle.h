// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillNotifyNodeBase.h"
#include "GameObject/Define/BP_EGsGameObjectType.h"

/**
 * 빔 파티클 처리 Notify
 */
class T1PROJECT_API FGsSkillNotifyNodeBeamParticle : public FGsSkillNotifyNodeBase
{
	using Super = FGsSkillNotifyNodeBase;

protected:
	const struct FGsSchemaSkillNotifyBeamParticle* _beamParticleEffect = nullptr;
	class UGsGameObjectBase* _owner = nullptr;

public:
	FGsSkillNotifyNodeBeamParticle() = default;

public:
	virtual void Initialize(class UGsGameObjectBase* Owner, const struct FGsSchemaSkillNotifySet* NotifySetData,
		const struct FGsSkillNetData* NetData) override;
	virtual void Finalize() override;

	virtual void Action(UGsGameObjectBase* Owner) override;

public:
	void SetData(const struct FGsSchemaSkillNotifyBeamParticle* BeamParticleTable);

protected:
	virtual UGsGameObjectBase* FindBeamParticleTarget();
	void RemoveTargetBeamParticle();

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() override { return TEXT("FGsSkillNotifyNodeBeamParticle"); }
#endif
};
