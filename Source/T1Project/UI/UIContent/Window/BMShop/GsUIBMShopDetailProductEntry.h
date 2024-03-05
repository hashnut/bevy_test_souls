// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBMShopDetailProductEntry.generated.h"

class UGsButton;
class UTextBlock;
class USizeBox;
class UImage;
class URichTextBlock;
class UGsUICurrencySlot;
class FGsBMShopProductData;
class FGsBMShopManager;
class UWidgetSwitcher;
class UGsUIBMShopProductDetail;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopDetailProductEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnProduct;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _productNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _remainTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _limitTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USizeBox* _sizebox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _timerSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _externalDescRichText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _highLightImg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currencySlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _mileageCurrencySlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBMShopProductDetail* _detailInfo;

private:
	bool _isRunningTimer = false;
	BMProductId _productID = 0;
	int32 _productTID = 0;
	FGsBMShopManager* _bmShopMgr;
	FDateTime _popupShopTimer;
	bool _isPopupShopProduct = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetProductID(const BMProductId InDataID);
	void InvalidateProduct();

private:
	void SetProductName(const FGsBMShopProductData* InData);
	void SetSaleRemainTime(const FGsBMShopProductData* InData);
	void SetSaleCurrencyInfo(const FGsBMShopProductData* InData);
	void SetExternalDesc(const FGsBMShopProductData* InData);
	void SetHighLightEffect(const FGsBMShopProductData* InData);

public:
	float GetWidthSize();

private:
	void CountDownRemainTime();

protected:
	UFUNCTION()
	void OnLongPressIcon();
	UFUNCTION()
	void OnClickIcon();
	
};
