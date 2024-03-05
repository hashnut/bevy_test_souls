// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"
#include "GsUIMarketSettlementListEntry.generated.h"

class UTextBlock;
class UGsButton;
struct FGsMarketSettlementData;

/**
 * 거래소 정산 리스트 아이템
 */
UCLASS()
class UGsUIMarketSettlementListEntry : public UGsUIMarketItemListEntryBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStat;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockEarnings;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSaleDate;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSaleTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSettlement;

protected:
	MarketTransactionId _transactionId = INVALID_MARKET_TRANSACTION_ID;
	int32 _diaEarnings = 0;
	bool _bIsWorldMarket = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetData(const FGsMarketSettlementData* InData);

public:
	UFUNCTION(BlueprintCallable)
	void OnClickSettlement();
};
