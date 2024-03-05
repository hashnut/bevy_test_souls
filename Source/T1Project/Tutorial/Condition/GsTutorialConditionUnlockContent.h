// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Condition/GsTutorialConditionBase.h"

/**
 * 특정 컨텐츠 언락 시
 */
class T1PROJECT_API FGsTutorialConditionUnlockContent final : public FGsTutorialConditionBase
{
public:
	using Super = FGsTutorialConditionBase;

public:
	int64 _unlockContentsId = 0;

public:
	FGsTutorialConditionUnlockContent() : Super()
	{
	}
	virtual ~FGsTutorialConditionUnlockContent() = default;

public:	
	virtual void Initialize(const FGsSchemaTutorial * InTable) override;
	virtual bool CheckCondition(const IGsMessageParam * InParam) override;
	virtual bool IsValidToStart() override;
};
