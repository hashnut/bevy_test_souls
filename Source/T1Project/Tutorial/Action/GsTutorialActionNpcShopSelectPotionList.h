// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

class UGsUIWindowNpcShop;
class UWidget;

/**
 * 잡화상점의 HP포션 선택 액션
 */
class T1PROJECT_API FGsTutorialActionNpcShopSelectPotionList final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const struct FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIWindowNpcShop> _window;

public:
	FGsTutorialActionNpcShopSelectPotionList() : Super()
	{
	}
	virtual ~FGsTutorialActionNpcShopSelectPotionList() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnTutorialListItem(UWidget* InWidget);
};
