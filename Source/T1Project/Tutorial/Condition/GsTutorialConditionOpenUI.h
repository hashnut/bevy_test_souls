// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Condition/GsTutorialConditionBase.h"

/**
 * 특정 UI를 열었을 때 발동 조건 체크
 */
class T1PROJECT_API FGsTutorialConditionOpenUI final : public FGsTutorialConditionBase
{
public:
	using Super = FGsTutorialConditionBase;

public:
	FName _nameWidgetKey;

public:
	FGsTutorialConditionOpenUI() : Super()
	{
	}
	virtual ~FGsTutorialConditionOpenUI() = default;
	
public:
	virtual void Initialize(const FGsSchemaTutorial* InTable) override;
	virtual bool CheckCondition(const IGsMessageParam* InParam) override;
	virtual bool IsValidToStart() override;
};
