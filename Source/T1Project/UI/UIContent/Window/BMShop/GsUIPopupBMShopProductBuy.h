// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Reward/GsRewardHelper.h"
#include "GsUIPopupBMShopProductBuy.generated.h"

class FGsItem;
class UTextBlock;
class UScrollBox;
class UGsButton;
class UVerticalBox;
class UHorizontalBox;
class UWidgetSwitcher;
class UGsUIBMShopBuyPrice;
class FGsBMShopProductData;
class UGsUIBMShopContractGuide;
class UGsUIBMShopBuyCalculator;
class UGsUIBMShopProductDetail;
class UGsUIBMShopProductTextEntry;
struct FGsSharedParam;
/**
 * 상품 구매 상세창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupBMShopProductBuy : public UGsUIPopup
{
	GENERATED_BODY()

public:
	static const TCHAR* ASSET_NAME;

	enum BottomType : uint8
	{
		OnlyPrice = 0,
		WithCalculator,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnProbability;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCoupon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _detailTextRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _provideGroupRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _groupItemListRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBMShopProductDetail* _productDetailInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _bottomSwitcher; // 0 : 가격만 출력 , 1 : 계산기 및 가격 , 2 : 안내문
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBMShopBuyPrice* _priceOnly;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBMShopBuyCalculator* _calculator;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBMShopContractGuide* _contractGuide;


protected:
	// 상품 설명 text Entry
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _textListEntryWidgetClass;
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _textListPanelSlotHelper;

protected:
	// 상품 구성 아이템 그룹 Entry
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _provideGroupEntryWidgetClass;
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _provideGroupPanelSlotHelper;


private:
	BMProductId _productID = 0;
	uint16 _currentAmountCount = 1;
	TMap<FString, FString> _displayStringMap;
	CurrencyPair _cashRebateInfo;
	TArray<FGsRewardUnboxingData> _rewardItemDataList;
	TPair<MessageContentStoreBM, FDelegateHandle> _bmShopMsgDelegate;


private:
	void SetItemGroup(const FGsBMShopProductData* InProductData);
	void SetBottomInfo(const FGsBMShopProductData* InProductData);
	void SetOnlyPrice(const FGsBMShopProductData* InProductData);
	void SetWithCalculator(const FGsBMShopProductData* InProductData);
	void OnChangeCalculatorCountEvent(const uint16 InCount);
	void InvalidateCoupontPrice(const FGsBMShopProductData* InProductData);
	bool IsShowContractNotice() const;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void OnFinishedRemainTime(FGsSharedParam& InParamData);

public:
	void InvalidateCurrency();
	void InvalidateLevel();
	void InvalidateCoupontPrice(FGsBMShopProductData* InProductData, FGsItem* InCouponItemData);
	void ExpiredCouponItem();

public:
	void SetProductBuyInfo(const FGsBMShopProductData* InProductData);

public:
	BMProductId GetProductID() { return _productID; }

protected:
	UFUNCTION()
	void OnRefreshText(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshItemGroup(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnOk();
	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void OnProbability();
	UFUNCTION()
	void OnCoupon();

	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;
};
