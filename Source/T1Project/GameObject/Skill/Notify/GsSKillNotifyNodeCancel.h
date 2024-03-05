// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillNotifyNodeBase.h"

/**
 * 1. 켄슬 가능 시점 Event Send
 * 2. 연계 스킬 발동
 */
class T1PROJECT_API FGsSKillNotifyNodeCancel : public FGsSkillNotifyNodeBase
{
	using Super = FGsSkillNotifyNodeBase;

protected:
	//bool IsActive = true;

public:
	FGsSKillNotifyNodeCancel() = default;
	virtual ~FGsSKillNotifyNodeCancel() = default;

public:
	virtual void Initialize(class UGsGameObjectBase* Owner, const class IGsSkillRunnerData* SkillRunnerData,
		const struct FGsSchemaSkillNotify* NotifyInfo, const struct FGsSkillNetData* NetData, float OffsetRate) override;
	virtual void Finalize() override;

	virtual void Action(UGsGameObjectBase* Owner) override;
	//virtual void Cancel(UGsGameObjectBase* Owner) override;

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() override { return TEXT("FGsSKillNotifyNodeCancel"); }
#endif
};
