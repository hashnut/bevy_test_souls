// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIWindowBMShop;
class UWidget;

/**
 * BMShop의 특정id의 탭 선택 액션
 */
class T1PROJECT_API FGsTutorialActionBMShopTabSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIWindowBMShop> _window;

public:
	FGsTutorialActionBMShopTabSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionBMShopTabSelect() {}

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnListEvent(UWidget* InWidget);
};
