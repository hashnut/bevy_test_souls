// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Tutorial/GsTutorialBase.h"

/**
 * 
 */
class T1PROJECT_API FGsTutorialDefault final : public FGsTutorialBase
{
public:
	using Super = FGsTutorialBase;

public:
	FGsTutorialDefault() : Super()
	{
	}
	virtual ~FGsTutorialDefault() = default;
};
