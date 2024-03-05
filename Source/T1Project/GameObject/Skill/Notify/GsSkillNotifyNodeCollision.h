// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillNotifyNodeBase.h"

struct FGsSchemaSkillSet;

/**
 * 
 */
class T1PROJECT_API FGsSkillNotifyNodeCollision : public FGsSkillNotifyNodeBase
{
	using Super = FGsSkillNotifyNodeBase;

protected:
	const FGsSchemaSkillSet* _currentSkillData = nullptr;
	
	bool _isTargeting = false;

	// 발동 효과
	class UParticleSystem* _effect = nullptr;
	FSoftObjectPath _effectPath;

	// 판정영역 스케일 정보
	float _collisionAddRange = 0.f;
	float _collisionScale = 1.f;

	// 특수 스킬 처리
	//bool _isSpecialSkill = false;

public:
	FGsSkillNotifyNodeCollision()			= default;
	explicit FGsSkillNotifyNodeCollision(class UParticleSystem* ParticleEffect, const FSoftObjectPath& EffectPath);
	virtual ~FGsSkillNotifyNodeCollision()	= default;

public:
	virtual void Initialize(class UGsGameObjectBase* Owner, const class IGsSkillRunnerData* SkillRunnerData,
		const struct FGsSchemaSkillNotify* NotifyInfo, const struct FGsSkillNetData* NetData, float OffsetRate) override;

	virtual void Action(class UGsGameObjectBase* Owner) override;

public:
	void SetData(class UParticleSystem* ParticleEffect, const FSoftObjectPath& EffectPath);

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() override { return TEXT("FGsSkillNotifyNodeCollision"); }
#endif
};
