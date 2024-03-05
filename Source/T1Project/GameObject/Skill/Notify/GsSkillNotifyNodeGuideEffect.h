// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillNotifyNodeBase.h"
#include "ActorComponentEx/GsSkillGuideEffectComponent.h"

/**
 * 예시선 처리 Notify
 */
class T1PROJECT_API FGsSkillNotifyNodeGuideEffect : public FGsSkillNotifyNodeBase
{
	using Super = FGsSkillNotifyNodeBase;

private:
	const struct FGsSchemaSkillNotifySkillGuide* _guideEffectData = nullptr;
	const struct FGsSchemaSkillCollision* _collisionData = nullptr;	
	bool _isTargeting = false;

public:
	FGsSkillNotifyNodeGuideEffect() = default;

public:
	virtual void Initialize() override;
	virtual void Initialize(class UGsGameObjectBase* Owner, const class IGsSkillRunnerData* SkillRunnerData,
		const struct FGsSchemaSkillNotify* NotifyInfo, const struct FGsSkillNetData* NetData, float OffsetRate) override;
	virtual void Finalize() override;

	virtual void Action(UGsGameObjectBase* Owner) override;

public:
	void SetData(const FGsSchemaSkillNotifySkillGuide* Data, const FGsSchemaSkillCollision* CollisionInfo);

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() override { return TEXT("FGsSkillNotifyNodeGuideEffect"); }
#endif
};
