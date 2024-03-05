// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketPageBase.h"
#include "GsUIMarketPageSettlement.generated.h"

class UScrollBox;
class UPanelWidget;
class UTextBlock;
class UGsButton;

/**
 * 거래소 정산 페이지
 */
UCLASS()
class UGsUIMarketPageSettlement : public UGsUIMarketPageBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMaxCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTotalPrice;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSettlementAll;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	TArray<struct FGsMarketSettlementData*> _dataList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;

protected:
	virtual void Show(bool bIsWorldMarket) override;
	virtual void Hide() override;
	virtual void OnUpdateItemList() override;
	virtual void OnRcvItemDetailInfo(MarketTransactionId InTransactionId) override;

protected:
	void UpdateItemList();

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);
	UFUNCTION()
	void OnClickSettlementAll();
};
