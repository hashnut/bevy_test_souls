// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIWindowCraft;
class UWidget;

/**
 * 제작 튜토리얼에서 특정 제작 선택 기능(0번째 것 선택, 바로가기로 들어갈 경우 선택된 대상)
 */
class T1PROJECT_API FGsTutorialActionCraftSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIWindowCraft> _window;

public:
	FGsTutorialActionCraftSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionCraftSelect() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;
};
