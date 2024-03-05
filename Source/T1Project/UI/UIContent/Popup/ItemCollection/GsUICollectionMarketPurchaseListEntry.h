// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "Item/Collection/EItemCollectionSlotState.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUICollectionMarketPurchaseListEntry.generated.h"

class UTextBlock;
class UGsButton;
class UGsSwitcherButton;
class UWidgetSwitcher;
class UImage;
struct FGsMarketCollectionItemData;

/**
 * 아이템 컬렉션에서 거래소 구매 팝업, 거래 결과 팝업
 */
UCLASS()
class UGsUICollectionMarketPurchaseListEntry : 
	public UGsUIMarketItemListEntryBase, 
	public IGsToggleGroupEntry
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_ThreeParams(FOnCheckItem, MarketTransactionId /*InTransactionId*/, bool /*bIsWorldMarket*/, bool/*bIsChecked*/);

public:
	enum class EGsMarketPurchaseResult
	{
		WAIT = 0,
		SUCCESS = 1,
		FAIL = 2,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCheck;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

	// 0: 구매중, 1: 성공, 2: 실패 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherResult;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockFail;

protected:
	MarketTransactionId _transactionId = INVALID_MARKET_TRANSACTION_ID;
	bool _bIsWorldMarket = false;
	bool _bIsSelected = false;

public:
	FOnCheckItem OnCheckItem;

protected:
	virtual void NativeOnInitialized() override;

protected:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	void SetData(const FGsMarketCollectionItemData* InData, bool bIsResult);
	void SetIsChecked(bool bIsChecked);

public:
	void SetPurchaseResult(EGsMarketPurchaseResult InResult);
	void SetTextFailResult(int32 InResult);

protected:
	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnClickCheck();
};
