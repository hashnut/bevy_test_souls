// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsStatBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStatNonPlayer final : public FGsStatBase
{
	using Super = FGsStatBase;

public:
	FGsStatNonPlayer() = default;
	virtual ~FGsStatNonPlayer() = default;

public:
	virtual void Initialize(UGsGameObjectBase* owner) override;
	virtual void Finalize() override;
	virtual void Update() override;

public:
	virtual void UpdateStatInfo(const TArray<TPair<StatType, int32>>& InStatInfoMap) override;
};
