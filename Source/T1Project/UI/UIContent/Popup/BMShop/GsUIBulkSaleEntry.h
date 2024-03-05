// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBulkSaleEntry.generated.h"


DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnClickAmount, bool, BMProductId, UGsUIBulkSaleEntry*);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnClickCheckBox, BMProductId, ItemAmount, bool);


class UTextBlock;
class UGsButton;
class UGsCheckBox;
class UCanvasPanel;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBulkSaleEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsCheckBox* _selectCheckBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMin;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPlus;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _amountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _limitRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _limitReasonText;


private:
	bool _isSelected = false;
	BMProductId _productID = 0;
	ItemAmount _amount = 0;
	ItemAmount _maximumAmount = 0;

public:
	FOnClickAmount OnClickAmount;
	FOnClickCheckBox OnClickCheckBox;


protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;

protected:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetProductInfo(const BMProductId InProductID, const bool InState, const ItemAmount InAmount);

private:
	void UpdateAmountCount();
	void UpdateDimmed();

private:
	void ValidCheck();

public:
	BMProductId GetProductID() { return _productID; }
	ItemAmount GetAmount() { return _amount; }
	bool IsSelectedCheckBox() { return _isSelected; }


protected:
	UFUNCTION()
	void OnRefreshItemIcon(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnChangeCheckBox(int32 InIndexGroup, bool InState);
	UFUNCTION()
	void OnPlus();
	UFUNCTION()
	void OnMin();
};
