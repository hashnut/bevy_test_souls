// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Tutorial/GsTutorialBase.h"

/**
 * 
 */
class T1PROJECT_API FGsTutorialGuildContents final : public FGsTutorialBase
{
public:
	using Super = FGsTutorialBase;

public:
	FGsTutorialGuildContents() : Super()
	{
	}
	virtual ~FGsTutorialGuildContents() = default;

public:
	virtual bool CheckIsValidToStart() override;

protected:
	virtual bool OnPrevStart() override;
};
