// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsTargetHandlerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsTargetHandlerNonPlayer : public FGsTargetHandlerBase
{
	using Super = FGsTargetHandlerBase;

public:
	FGsTargetHandlerNonPlayer()				= default;
	virtual ~FGsTargetHandlerNonPlayer()	= default;

public:
	virtual void Initialize(UGsGameObjectBase* owner);
	virtual void Finalize() override;

public:
	virtual bool SetTarget(UGsGameObjectBase* target, bool isRotate = false, 
		SelectTargetFilter selectFilter = SelectTargetFilter::None) override;
	virtual void ClearTarget(bool isAllClear = false) override;
};
