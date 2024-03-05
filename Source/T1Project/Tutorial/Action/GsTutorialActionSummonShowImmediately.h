// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"


/**
 * 바로보기 or 모두보기 버튼에 포커싱
 */
class T1PROJECT_API FGsTutorialActionSummonShowImmediately final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

public:
	FGsTutorialActionSummonShowImmediately() : Super()
	{
	}
	virtual ~FGsTutorialActionSummonShowImmediately() = default;

public:
	virtual bool OnStart() override;
};
