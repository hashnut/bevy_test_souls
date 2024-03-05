// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "GsUIMaterialSetGainInfoMarketPriceListEntry.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UGsSwitcherButton;
class FGsItemCollectionCondition;

/**
 * 아이템 컬렉션에서 거래소 최저가 표기 리스트 아이템
 */
UCLASS()
class UGsUIMaterialSetGainInfoMarketPriceListEntry : 
	public UGsUIMarketItemListEntryBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnPurchase, int32/*InIndex*/);

protected:
	// 0: 텍스트, 1: 최저다이아 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockState;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;

	// 0: 일반 거래소 / 1: 월드 거래소 / 2: 거래소바로가기(다른 강화도 존재)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnPurchase;

protected:
	int32 _index = 0;
	const FGsItemCollectionCondition* _data = nullptr;
	MarketPrice _price = 0;
	bool _bIsWorldMarket = false;
	bool _bOtherLevelExist = false;

public:
	FOnPurchase OnPurchase;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetData(int32 InIndex, const FGsItemCollectionCondition* InData, MarketPrice InPrice, bool bIsWorldMarket, bool bOtherLevelExist);

protected:
	void SetState(bool bIsCollected, bool bIsWorldMarket, bool bOtherLevelExist);

protected:
	UFUNCTION()
	void OnClickPurchase();
};
