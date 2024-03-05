// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GsSkillNotifyNodeBase.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillNotifyNodePreviewEndSection : public FGsSkillNotifyNodeBase
{
	using Super = FGsSkillNotifyNodeBase;

protected:
	IffApplyType _skillIffType = IffApplyType::NONE;

public:
	FGsSkillNotifyNodePreviewEndSection(IffApplyType SkillIffType);
	virtual ~FGsSkillNotifyNodePreviewEndSection() = default;

public:
	virtual void Action(class UGsGameObjectBase* Owner) override;

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() { return TEXT("FGsSkillNotifyNodePreviewEndSection"); }
#endif
};
