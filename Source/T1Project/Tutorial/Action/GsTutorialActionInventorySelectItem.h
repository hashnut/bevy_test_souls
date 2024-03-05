// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIPopupInventory;

/**
 * 인벤토리의 특정 아이템 선택 
 */
class T1PROJECT_API FGsTutorialActionInventorySelectItem final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIPopupInventory> _popup;
	ItemId _itemId = INVALID_ITEM_ID;

public:
	FGsTutorialActionInventorySelectItem() : Super()
	{
	}
	virtual ~FGsTutorialActionInventorySelectItem() = default;

public:
	virtual void OnPrevStart(const IGsMessageParam* InParam) override;
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnTutorialInventorytListEvent(class UWidget* InTargetWidget);
};
