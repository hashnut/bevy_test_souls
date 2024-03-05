// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsAbnormalityHandlerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsAbnormalityHandlerSkillPreviewPlayer final :
	public FGsAbnormalityHandlerBase
{
	using Super = FGsAbnormalityHandlerBase;

public:
	FGsAbnormalityHandlerSkillPreviewPlayer() = default;
	virtual ~FGsAbnormalityHandlerSkillPreviewPlayer() = default;
	
public:
	virtual void Add(const AbnormalityData& NetData, bool isRefresh = true) override;
	virtual void Update(float Delta) override;
	virtual void Remove(AbnormalityInstanceId InstanceId) override;

	void Cancel();

public:
	virtual bool IsTeamMate() override { return true; }

private:
	void ExecuteNotify(const struct FGsSchemaAbnormalityIndexSet* EffectIndexSet);
};
