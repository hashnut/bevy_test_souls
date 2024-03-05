// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsAbnormalityHandlerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsAbnormalityHandlerNonPlayer final :
	public FGsAbnormalityHandlerBase
{
	using Super = FGsAbnormalityHandlerBase;

public:
	FGsAbnormalityHandlerNonPlayer() = default;
	virtual ~FGsAbnormalityHandlerNonPlayer() = default;

protected:
	virtual void OnActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData, bool IsNewDataAdded, bool isSpawnEffect = true) override;
	virtual void OnDeActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData) override;
};
