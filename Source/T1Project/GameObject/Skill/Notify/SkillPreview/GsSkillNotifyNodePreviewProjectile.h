// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GsSkillNotifyNodeBase.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"

/**
 *
 */
class T1PROJECT_API FGsSkillNotifyNodePreviewProjectile : public FGsSkillNotifyNodeBase
{
	using Super = FGsSkillNotifyNodeBase;	

protected:
	IffApplyType _skillIffType = IffApplyType::NONE;
	uint32 _skillId = 0;

public:
	FGsSkillNotifyNodePreviewProjectile(IffApplyType SkillIffType, uint32 SkillId);
	virtual ~FGsSkillNotifyNodePreviewProjectile() = default;

public:
	virtual void Action(class UGsGameObjectBase* Owner) override;

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() { return TEXT("FGsSkillNotifyNodePreviewProjectile"); }
#endif
};