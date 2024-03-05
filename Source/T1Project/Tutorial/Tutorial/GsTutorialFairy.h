// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Tutorial/GsTutorialBase.h"

/**
 * 페어리 튜토리얼. 소환 확정 목록이 꽉찼을 때 바로 종료시키기 위해 별도 생성
 */
class T1PROJECT_API FGsTutorialFairy final : public FGsTutorialBase
{
public:
	using Super = FGsTutorialBase;

public:
	FGsTutorialFairy() : Super()
	{
	}
	virtual ~FGsTutorialFairy() = default;

protected:
	virtual bool OnPrevStart() override;
};
