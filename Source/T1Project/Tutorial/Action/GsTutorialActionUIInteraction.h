// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"


/**
 * UI 특정부분 클릭
 */
class T1PROJECT_API FGsTutorialActionUIInteraction final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

public:
	FGsTutorialActionUIInteraction() : Super()
	{
	}
	virtual ~FGsTutorialActionUIInteraction() = default;

public:
	virtual bool OnStart() override;
};
