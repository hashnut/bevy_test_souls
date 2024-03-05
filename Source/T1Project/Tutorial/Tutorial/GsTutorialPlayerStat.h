// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Tutorial/GsTutorialBase.h"

/**
 * 플레이어 스탯 가이드 튜토리얼. 포인트 있을때 없을때 UI 가 달라서, 있을 때만 발동
 */
class T1PROJECT_API FGsTutorialPlayerStat final : public FGsTutorialBase
{
public:
	using Super = FGsTutorialBase;

public:
	FGsTutorialPlayerStat() : Super()
	{
	}
	virtual ~FGsTutorialPlayerStat() = default;

protected:
	virtual bool OnPrevStart() override;
};
