// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Tutorial/GsTutorialBase.h"

/**
 * 길드 가입 튜토리얼. 공통로직으로 처리하기 어려운 부분들 추가 구현
 */
class T1PROJECT_API FGsTutorialGuildJoin final : public FGsTutorialBase
{
public:
	using Super = FGsTutorialBase;

public:
	FGsTutorialGuildJoin() : Super()
	{
	}
	virtual ~FGsTutorialGuildJoin() = default;

protected:
	virtual bool OnPrevStart() override;
};
