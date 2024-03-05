// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUICurrencyDetailInfo.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHideCurrencyDetailInfo);

/**
 * 재화 정보 UI
 */
class UTextBlock;
class UPanelWidget;
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUICurrencyDetailInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAcquire;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUsedfor;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelAcquire;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelUsedfor;

	UUserWidget* _currencyList = nullptr;

public:
	FOnHideCurrencyDetailInfo OnHide;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	void SetData(CurrencyType InType);
	void SetAmount(CurrencyType InType);
	void SetCurrencyList(UUserWidget* InCurrencyList) { _currencyList = InCurrencyList; }

};
