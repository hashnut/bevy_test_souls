// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Condition/GsTutorialConditionBase.h"

/**
 * 특정 UI를 열었을 때 발동 조건 체크
 */
class T1PROJECT_API FGsTutorialConditionOpenUICheckParam final : public FGsTutorialConditionBase
{
public:
	using Super = FGsTutorialConditionBase;

public:
	FName _widgetKey;
	FString _paramStr;
	int32 _paramInt;

public:
	FGsTutorialConditionOpenUICheckParam() : Super()
	{
	}
	virtual ~FGsTutorialConditionOpenUICheckParam() {};
	
public:
	virtual void Initialize(const FGsSchemaTutorial* InTable) override;
	virtual bool CheckCondition(const IGsMessageParam* InParam) override;
	virtual bool IsValidToStart() override;

public:
	FName GetWidgetKey() const { return _widgetKey; }
	FString GetParamString() const { return _paramStr; }
};
