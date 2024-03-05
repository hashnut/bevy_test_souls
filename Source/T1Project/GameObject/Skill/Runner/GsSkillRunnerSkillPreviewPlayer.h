// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NoExportTypes.h"
#include "GsSkillRunnerCreature.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillRunnerSkillPreviewPlayer : public FGsSkillRunnerCreature
{
	using Super = FGsSkillRunnerCreature;

public:
	FGsSkillRunnerSkillPreviewPlayer(TFunction<void(int, bool)> EndRun);
	virtual ~FGsSkillRunnerSkillPreviewPlayer() = default;

protected:
	virtual void StartRunner(class UGsGameObjectBase* Owner) override;
	virtual TSharedPtr<FGsSkillNotifyNodeBase> CreateSkillNode(const struct FGsSchemaSkillNotifySet* Data) override;
};
