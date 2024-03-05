// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBMShopSmallBanner.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnClickSmallBanner, BMProductId);

class USizeBox;
class UGsButton;
class UImage;
class UTextBlock;
class UWidgetSwitcher;
class FGsBMShopProductData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopSmallBanner : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOnClick;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _saleTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _limitTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _image;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USizeBox* _sizebox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _timerSwitcher; // 0 : 판매종료 , 1 : 남은시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _couponWidget;

private:
	BMProductId _productID = 0;
	FDateTime _popupShopTimer;
	bool _isPopupShopProduct = false;

public:
	FOnClickSmallBanner OnClickSmallBanner;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetProductID(const BMProductId InProductID);
	void SetImage(FGsBMShopProductData* InData);

public:
	float GetWidthSize();


protected:
	UFUNCTION()
	void OnBtnProduct();

};
