// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBMShopStepUpProductEntry.generated.h"

class UGsButton;
class UTextBlock;
class UPanelWidget;
class UHorizontalBox;
class FGsBMShopProductData;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopStepUpProductEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _productRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _bannerRootPanel;

protected:
	// product List
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _productListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _productListPanelSlotHelper;

	// small banner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _smallBannerListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _smallBannerListPanelSlotHelper;


private:
	BMProductId _bannerId = 0;
	TArray<BMProductId> _productIdList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetProduct(TArray<FGsBMShopProductData*> InProductList);
	void InvalidateProduct();


protected:
	UFUNCTION()
	void ProductListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void SmallBannerListRefresh(int32 InIndex, UWidget* InEntry);

};
