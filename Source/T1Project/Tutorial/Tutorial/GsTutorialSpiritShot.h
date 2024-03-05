// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Tutorial/GsTutorialBase.h"

/**
 * 천마석 튜토리얼. 천마석 최대 레벨일 때 다른계정의 튜토리얼 예외처리
 */
class T1PROJECT_API FGsTutorialSpiritShot final : public FGsTutorialBase
{
public:
	using Super = FGsTutorialBase;

public:
	FGsTutorialSpiritShot() : Super()
	{
	}
	virtual ~FGsTutorialSpiritShot() = default;

protected:
	virtual bool OnPrevStart() override;
};
