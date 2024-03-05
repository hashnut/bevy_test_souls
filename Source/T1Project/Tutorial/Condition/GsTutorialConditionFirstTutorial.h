// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Condition/GsTutorialConditionBase.h"

/**
 * 게임 첫 시작 첫 퀘스트 받을 때
 */
class T1PROJECT_API FGsTutorialConditionFirstTutorial final : public FGsTutorialConditionBase
{
public:
	using Super = FGsTutorialConditionBase;

public:
	StoryId _storyId = 0;

public:
	FGsTutorialConditionFirstTutorial() : Super()
	{
	}
	virtual ~FGsTutorialConditionFirstTutorial() = default;

public:
	virtual void Initialize(const FGsSchemaTutorial * InTable) override;
	virtual bool CheckCondition(const IGsMessageParam * InParam) override;
	virtual bool IsValidToStart() override;
};
