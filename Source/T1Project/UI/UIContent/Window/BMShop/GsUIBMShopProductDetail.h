// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBMShopProductDetail.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class FGsBMShopProductData;
class UGsHorizontalBoxIconSelector;
class UGsDynamicIconSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopProductDetail : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _limitRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _limitText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _tagSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _buyerLimitText;   // 귀속 제한(캐릭터당, 계정당 등)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _resetTimeText;   // 초기화 알림 텍스트 ( 매일 / 매월 등 )
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _quantiyCount; // 구매한 수량 / 맥시멈 수량
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _couponDisplayWidget;


private:
	BMProductId _productID = 0;

public:	
	virtual void BeginDestroy() override;
	virtual void OnShow() override;
	virtual void OnHide() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	void SetProductIcon();
	void SetProductResetText(const FGsBMShopProductData* InData);
	void SetProductLimitBuyerText(const FGsBMShopProductData* InData);
	void SetProductStickerTag(const FGsBMShopProductData* InData);
	void SetProductAmountInfo(const FGsBMShopProductData* InData);
	void SetLimit(const FGsBMShopProductData* InData, const bool InDisableStepupBlock);

public:
	void SetProductData(const FGsBMShopProductData* InProductData, const bool InDisableStepupBlock = false);

private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);
	void OnClickSlot(class UGsUIIconItem& InIcon);


protected:
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateIconWrapBox(class UGsUIIconBase* InIcon);

	// 튜토리얼
protected:
	bool _bIsTutorial = false;

public:
	DECLARE_DELEGATE(FOnTutorialInteraction);
	FOnTutorialInteraction OnTutorialInteraction;

public:
	void SetIsTutorial(bool bIsTutorial);
};
