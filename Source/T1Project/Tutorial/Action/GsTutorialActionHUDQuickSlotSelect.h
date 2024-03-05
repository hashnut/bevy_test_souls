// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

class UGsUIReuseScrollHUD;

/**
 * HUD 스킬버튼 선택
 */
class T1PROJECT_API FGsTutorialActionHUDQuickSlotSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	TWeakObjectPtr<UGsUIReuseScrollHUD> _quickSlotSetUI;

public:
	FGsTutorialActionHUDQuickSlotSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionHUDQuickSlotSelect() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;
};
