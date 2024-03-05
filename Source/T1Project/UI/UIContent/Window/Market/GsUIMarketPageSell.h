// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketPageBase.h"
#include "GsUIMarketPageSell.generated.h"

class UPanelWidget;
class UTextBlock;
class UScrollBox;
class UGsUIInventoryCommon;
class FGsInventoryLogicMarket;
struct FGsMarketItemData;

/**
 * 거래소 판매등록 페이지
 */
UCLASS()
class UGsUIMarketPageSell : public UGsUIMarketPageBase
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
	UGsUIInventoryCommon* _inventoryUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMaxCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTotalPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTax;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTaxSub;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;
	
protected:
	TArray<FGsMarketItemData*> _dataList;
	FGsInventoryLogicMarket* _inventoryLogic = nullptr;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	virtual void Show(bool bIsWorldMarket) override;
	virtual void OnUpdateItemList() override;
	virtual void OnRcvSellRegisterdItem(MarketTransactionId InTransactionId) override;
	virtual void OnRcvSellCancelItem(MarketTransactionId InTransactionId) override;
	virtual void OnRcvItemDetailInfo(MarketTransactionId InTransactionId) override;
	virtual void OnReqRefresh() override;

protected:
	void UpdateItemList();

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	void OnClickInventoryItem(uint64 InItemDBId);
};
