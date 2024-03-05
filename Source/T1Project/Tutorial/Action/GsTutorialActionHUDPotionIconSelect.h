// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"


/**
 * HUD의 자동물약 아이콘
 */
class T1PROJECT_API FGsTutorialActionHUDPotionIconSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

public:
	FGsTutorialActionHUDPotionIconSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionHUDPotionIconSelect() {};

public:
	virtual bool OnStart() override;
};
