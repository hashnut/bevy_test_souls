// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIPopupBulkSale.generated.h"

class UPanelWidget;
class UScrollBox;
class UGsButton;
class UTextBlock;
class UGsUIBulkSaleEntry;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupBulkSale : public UGsUIPopup
{
	GENERATED_BODY()

	struct  DisplayCostInfo
	{
		bool _isItemCurrency;
		BMProductId _productId;
		ItemId _itemId;
		CurrencyType _currencyType;
		Currency _currency;

	public:
		DisplayCostInfo(bool isItemCurrency, BMProductId productId, ItemId itemId, CurrencyType currencyType, Currency currency)
			:
			_isItemCurrency(isItemCurrency), _productId(productId), _itemId(itemId), _currencyType(currencyType), _currency(currency)
		{
		}
	};

protected:
	// product List
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _productListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _productListPanelSlotHelper;

	// curreny List
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _currencyListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _currencyListPanelSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _bulkSaleListRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _currencyListRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPurchase;

private:
	TMap<BMProductId, TPair<bool, ItemAmount>> _bulkSaleSettingListMap;		// 서버 저장될 수량 정보
	TMap<BMProductId, TPair<bool, ItemAmount>> _bulkSalePurchaseListMap;	// 구매에 사용될 수량 정보
	TArray<DisplayCostInfo*> _displayCostInfoList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetBulkSaleList(TArray<BMProductIdAmountPair> InList);
	void InvalidateCurrency();
	void InvalidateLevel();

private:
	void CalcCost();
	void SortDisplayCurrency();

private:
	void OnClickAmount(bool InState, BMProductId InProductID, UGsUIBulkSaleEntry* InEntry);
	void OnClickListCheckBox(BMProductId InProductID, ItemAmount InAmount, bool InState);

private:
	bool IsEnoughCost();
	bool IsEnoughLevel();


protected:
	UFUNCTION()
	void OnPurchase();
	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void ProductListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void CurrencyListRefresh(int32 InIndex, UWidget* InEntry);

};
