// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Condition/GsTutorialConditionBase.h"

/**
 * 특정 id의 퀘스트를 받을 시
 */
class T1PROJECT_API FGsTutorialConditionQuestId final : public FGsTutorialConditionBase
{
public:
	using Super = FGsTutorialConditionBase;

public:
	//TutorialId _tutorialId = 0;
	QuestId _questId = 0;

public:
	FGsTutorialConditionQuestId() : Super()
	{
	}
	virtual ~FGsTutorialConditionQuestId() = default;

public:
	virtual void Initialize(const FGsSchemaTutorial * InTable) override;
	virtual bool CheckCondition(const IGsMessageParam * InParam) override;
	virtual bool IsValidToStart() override;
};
