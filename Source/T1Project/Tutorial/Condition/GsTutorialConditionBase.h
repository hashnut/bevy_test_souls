// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct IGsMessageParam;
struct FGsSchemaTutorial;

/**
 * 튜토리얼 발동 조건 체크
 */
class T1PROJECT_API FGsTutorialConditionBase
{
public:
	FGsTutorialConditionBase() = default;
	virtual ~FGsTutorialConditionBase() = default;

public:
	virtual void Initialize(const FGsSchemaTutorial* InTable) = 0;
	// 발동 조건 체크
	virtual bool CheckCondition(const IGsMessageParam* InParam) = 0;
	// 튜토리얼 시작 시 문제 없는지 체크(예: 창이 열려 큐에 발동 예약해뒀는데, 창이 닫힌 경우 등) 
	virtual bool IsValidToStart() = 0;
};
