// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIPopupItemEnchantMain;

/**
 * 아이템 강화 인벤토리의 장착중인 무기 아이템 선택
 */
class T1PROJECT_API FGsTutorialActionItemEnchantSelectItem final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIPopupItemEnchantMain> _popup;

public:
	FGsTutorialActionItemEnchantSelectItem() : Super()
	{
	}
	virtual ~FGsTutorialActionItemEnchantSelectItem() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnTutorialListEvent(class UWidget* InTargetWidget);
};
