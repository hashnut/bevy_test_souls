// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMarketPriceInfo.generated.h"

class UTextBlock;
class UWidgetSwitcher;
struct FGsMarketItemPriceData;

/**
 * 
 */
UCLASS()
class UGsUIMarketPriceInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 0: 일반 거래소 / 1: 월드 거래소
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherServer;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLowestPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockHighestPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAveragePrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLatestPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLowestPriceCurrent;
	
public:
	void SetData(const FGsMarketItemPriceData& InData);
	void SetEmpty();
	void SetTextTarget(MarketPrice InPrice, bool bIsFractionalDigits, OUT UTextBlock* OutTextBlock);
};
