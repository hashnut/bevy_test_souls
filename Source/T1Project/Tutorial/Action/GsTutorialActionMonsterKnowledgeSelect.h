// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIWindowMonsterCollection;
class UWidget;

/**
 * 도감 튜토리얼에서 0번 몬스터 슬롯 선택
 */
class T1PROJECT_API FGsTutorialActionMonsterKnowledgeSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	TWeakObjectPtr<UGsUIWindowMonsterCollection> _window;

public:
	FGsTutorialActionMonsterKnowledgeSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionMonsterKnowledgeSelect() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;
};
