// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPayIncrease.h"
#include "GsUIPopupCommonPayInventory.generated.h"

class UTextBlock;
class UGsButton;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCommonPayInventory : public UGsUIPopupCommonPayIncrease
{
	GENERATED_BODY()

protected:
	CurrencyType _currencyType;
	Currency _amountPerLevel;

	Currency _amountHave;
	Currency _amountNeeded;

	// 창고의 경우에는, 각 레벨마다 요구 재화 수치가 달라진다
	bool _isDepot = false;

protected:
	virtual void OnClickYes() override;

	virtual void OnClickMinus() override;
	virtual void OnClickPlus() override;

public:
	void SetData(const FText& InContent, Level InPastLevel, Level InMaxLevel, bool InIsDepot = false, TFunction<void(bool, bool, Level)> InCallback = nullptr, TFunction<void(bool)> InDestructCallback = nullptr);

	virtual void RefreshUI() override;
};
