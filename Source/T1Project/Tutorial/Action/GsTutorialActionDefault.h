// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

/**
 * 기본적인 튜토리얼 기능으로 타겟없이 설명글만 출력하고 끝내는 경우에 사용
 */
class T1PROJECT_API FGsTutorialActionDefault final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

public:
	FGsTutorialActionDefault() : Super()
	{

	}
	virtual ~FGsTutorialActionDefault() = default;

public:
	virtual bool OnStart() override;
};
