// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUIMarketItemStackableListEntry.generated.h"

struct FGsMarketItemData;
class UTextBlock;
class UGsBorder;
class UGsCheckBox;

/**
 * 거래소 쌓을 수 있는 아이템 표시. (아이콘, 이름, 개당 판매 금액, 판매금액)
 */
UCLASS()
class UGsUIMarketItemStackableListEntry : 
	public UGsUIMarketItemListEntryBase, 
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPricePerUnit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsBorder* _borderLongPress;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsCheckBox* _checkBoxSelect;

protected:
	MarketTransactionId _transactionId = INVALID_MARKET_TRANSACTION_ID;
	bool _bIsWorldMarket = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	virtual void RefreshAll() override;
	virtual bool IsSlotLongPressed() const override;

protected:
	void InvalidateAll(class UGsMarketItemListItem* InListItem);

protected:
	UFUNCTION()
	void OnFinishedLongPress();

	UFUNCTION()
	void OnClickCheckBox(int32 IndexInGroup, bool bIsChecked);
};
