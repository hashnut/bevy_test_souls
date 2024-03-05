// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillRunnerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillRunnerCreature : public FGsSkillRunnerBase
{
	using Super = FGsSkillRunnerBase;

public:
	FGsSkillRunnerCreature(TFunction<void(int, bool)> EndRun);
	virtual ~FGsSkillRunnerCreature() = default;

public:
	virtual bool Update(float DeltaTime) override;

protected:
	virtual TSharedPtr<FGsSkillNotifyNodeBase> CreateSkillNode(const struct FGsSchemaSkillNotifySet* Data) override;
};
