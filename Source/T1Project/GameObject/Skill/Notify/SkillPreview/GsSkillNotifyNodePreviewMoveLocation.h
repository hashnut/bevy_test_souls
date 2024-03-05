// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GsSkillNotifyNodeBase.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillNotifyNodePreviewMoveLocation : public FGsSkillNotifyNodeBase
{
	using Super = FGsSkillNotifyNodeBase;

protected:
	IffApplyType _skillIffType = IffApplyType::NONE;

public:
	explicit FGsSkillNotifyNodePreviewMoveLocation(IffApplyType SkillIffType);
	virtual ~FGsSkillNotifyNodePreviewMoveLocation() = default;

public:
	virtual void Action(class UGsGameObjectBase* Owner) override;

	static void Action(const struct FGsSchemaSkillNotifyMoveLocation* MoveLocationData,
		class UGsGameObjectBase* Owner, class UGsGameObjectBase* Target);

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() { return TEXT("FGsSkillNotifyNodePreviewMoveLocation"); }
#endif
};

