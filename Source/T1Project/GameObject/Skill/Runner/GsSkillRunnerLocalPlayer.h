// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillRunnerCreature.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillRunnerLocalPlayer final : public FGsSkillRunnerCreature
{
	using Super = FGsSkillRunnerCreature;

public:
	FGsSkillRunnerLocalPlayer(TFunction<void(int, bool)> EndRun);
	virtual ~FGsSkillRunnerLocalPlayer() = default;

protected:
	virtual void StartRunner(UGsGameObjectBase* Owner) override;

protected:
	virtual TSharedPtr<FGsSkillNotifyNodeBase> CreateSkillNode(const FGsSchemaSkillNotifySet* Data) override;
	virtual void CreateRuntimeSkillNode() override;

public:
	virtual bool CanMove() const override;
};
