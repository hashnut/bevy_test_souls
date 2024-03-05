// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

class UGsUIHUDQuest;

/**
 * HUD 스킬버튼 선택
 */
class T1PROJECT_API FGsTutorialActionHUDQuestSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIHUDQuest> _questUI;

public:
	FGsTutorialActionHUDQuestSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionHUDQuestSelect() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnTutorialQuestListEvent(class UWidget* InTargetWidget);
};
