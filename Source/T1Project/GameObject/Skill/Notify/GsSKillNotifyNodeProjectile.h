// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillNotifyNodeBase.h"

/**
 * 프로젝타일 타입 Action처리 클래스
 * 
 */
class T1PROJECT_API FGsSKillNotifyNodeProjectile : public FGsSkillNotifyNodeBase
{
	using Super = FGsSkillNotifyNodeBase;

public:
	FGsSKillNotifyNodeProjectile() = default;
	virtual ~FGsSKillNotifyNodeProjectile() = default;

public:
	virtual void Action(class UGsGameObjectBase* Owner) override;

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() override { return TEXT("FGsSkillNotifyNodeCollision"); }
#endif
};
