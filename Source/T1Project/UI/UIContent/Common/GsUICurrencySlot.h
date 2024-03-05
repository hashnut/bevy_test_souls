// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "T1Project/UI/UIContent/Define/GsUIContentDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "GsUICurrencySlot.generated.h"


class FGsCostElement;
class UWidgetSwitcher;
class FGsBMShopProductData;
/**
 * 기본적인 재화 정보를 표시하는 슬롯.
 * - WBP_CurrencySlot : 네비게이션바 재화 슬롯
 * - WBP_CurrencyCommon : 일반 재화 표시 슬롯
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUICurrencySlot : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickCurrencySlot, class UWidget*);
	DECLARE_DELEGATE_OneParam(FOnClickCurrencyCharge, class UWidget*);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockAmount;

	// 재화 이름. 없을 수도 있음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	class UTextBlock* _textBlockName = nullptr;

	// 버튼이 있을 수도 없을 수도 있음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	class UGsButton* _btnSlot = nullptr;

	// 버튼이 있을 수도 없을 수도 있음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	class UGsButton* _btnCharge = nullptr;

	// 쿠폰 가격표 출력에 사용됌 없을수도 있음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	class UTextBlock* _textBlockDiscountAmount;

	// 쿠폰 가격표 출력에 사용됌 없을수도 있음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UWidgetSwitcher* _discountSwitcher;


protected:
	uint64 _limitAmount = 0; // 0일 때는 설정안함
	CostType _costType = CostType::CURRENCY;
	CurrencyType _currencyType = CurrencyType::NONE;
	ItemId _currencyItemId = 0;
	Currency _amount = 0;

public:
	// 주의 NativeDestruct 시 바인딩을 비운다. 동적인 데이터에 사용하자.
	FOnClickCurrencySlot OnClickCurrencySlot;
	FOnClickCurrencyCharge OnClickCurrencyCharge;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetDataLocalPlayer(CurrencyType InType);
	void SetDataLocalPlayer(ItemId InItemId);
	void SetData(CurrencyType InType);
	void SetData(CurrencyType InType, Currency InAmount);
	void SetRebateCurrencyData(CurrencyType InType, Currency InAmount);
	void SetData(CurrencyType InType, FText InAmountText);
	void SetData(ItemId InItemId, Currency InAmount);
	void SetData(ItemId InItemId, FText InAmountText);
	void SetData(FText InAmountText);
	void SetData(const FGsCostElement& InCostElements);
	void SetDisCountOriData(CurrencyType InType, Currency InAmount);
	void SetDisCountOriData(uint32 ItemTid, Currency InAmount);
	void SetDisCountData(CurrencyType InType, Currency InDiscountAmount);
	void SetBMShopCostData(const FGsBMShopProductData* InProductData);
	void SetAmountTextColor(const EGsUIColorType InColorType);
	// 특정 갯수 이상일 때, +표기하고 싶을 때 사용 (FGsUIStringHelper::GetAmountText)
	void SetLimitAmount(uint64 InLimitAmount) { _limitAmount = InLimitAmount; }

	// 유저 재화 보유량 비교해서 텍스트 컬러 세팅
	void UpdateAmountTextColorLocalPlayer();
	void UpdateDiscountAmountTextColorLocalPlayer(Currency InAmount);

	void SetChangeDisplayType(bool InIsDiscountFlag);
	void SetVisibilityBtnCharge(ESlateVisibility InVisibility);

public:
	CurrencyType GetCurrencyType() const { return _currencyType; }
	Currency GetCurrencyAmount() const { return _amount; }
	ItemId GetItemId() const { return _currencyItemId; }

protected:
	class UPaperSprite* GetIcon(CurrencyType InType);
	class UPaperSprite* GetIcon(ItemId InItemId);	
	
	// _btnSlot을 클릭했을 때
protected:
	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnClickCharge();
};
