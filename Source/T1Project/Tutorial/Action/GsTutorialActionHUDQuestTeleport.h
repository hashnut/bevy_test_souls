// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

class UGsUIHUDQuest;

/**
 * HUD 퀘스트 슬롯은 튜토리얼일 때 강제로 끄지만, 이 액션에서는 살려둔다
 */
class T1PROJECT_API FGsTutorialActionHUDQuestTeleport final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIHUDQuest> _questUI;

public:
	FGsTutorialActionHUDQuestTeleport() : Super()
	{
	}
	virtual ~FGsTutorialActionHUDQuestTeleport() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnTutorialQuestListEvent(class UWidget* InTargetWidget);
};
