// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Condition/GsTutorialConditionBase.h"

/**
 * 퀘스트 EventAction에서 튜토리얼 발동 시
 */
class T1PROJECT_API FGsTutorialConditionQuest final : public FGsTutorialConditionBase
{
public:
	using Super = FGsTutorialConditionBase;

public:
	TutorialId _tutorialId = 0;

public:
	FGsTutorialConditionQuest() : Super()
	{
	}
	virtual ~FGsTutorialConditionQuest() = default;

public:
	// EventAction의 발동 메시지를 받고 수행하기 때문에 그냥 무조건 false임
	virtual void Initialize(const FGsSchemaTutorial * InTable) override;
	virtual bool CheckCondition(const IGsMessageParam * InParam) override;
	virtual bool IsValidToStart() override { return true; }
};
