// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataCenter/Public/Tutorial/GsTutorialEnumDefine.h"

class FGsTutorialBase;
class FGsTutorialConditionBase;
class FGsTutorialActionBase;
struct FGsSchemaTutorial;
struct FGsSchemaTutorialAction;

/**
 * 튜토리얼 관련 객체 인스턴스를 생성
 */
class T1PROJECT_API FGsTutorialFactory
{
public:
	FGsTutorialFactory() = default;
	~FGsTutorialFactory() {};

public:
	static TSharedPtr<FGsTutorialBase> CreateTutorial(const FGsSchemaTutorial* InTable);
	static TSharedPtr<FGsTutorialConditionBase> CreateCondition(const FGsSchemaTutorial* InTable);
	static TSharedPtr<FGsTutorialActionBase> CreateAction(const FGsSchemaTutorialAction* InTable);
};
