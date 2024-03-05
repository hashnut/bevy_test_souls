// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsTriggerEventBase.h"
#include "DataSchema/Skill/GsSchemaSkillCollision.h"

/**
 * 스킬 영역(FGsSchemaSkillCollision)을 기준으로 In/Out Envet 처리
 */
class T1PROJECT_API FGsTriggerEventSkillCollision : public FGsTriggerEventBase
{
protected:
	class UGsSkillCollisionComponent* _component = nullptr;
	FGsSchemaSkillCollision _collisionData;

	TArray<int64> _listInSideObject;

public:
	FGsTriggerEventSkillCollision() = default;
	virtual ~FGsTriggerEventSkillCollision() = default;

public:
	virtual void Active(class UGsGameObjectBase* Owner, EGsGameObjectType TargetType) override;
	virtual void Deactive() override;

protected:
	virtual bool CheckTrigger() override;

public:
	virtual EGsTriggerType GetType() const override { return EGsTriggerType::Projectile; }
};
