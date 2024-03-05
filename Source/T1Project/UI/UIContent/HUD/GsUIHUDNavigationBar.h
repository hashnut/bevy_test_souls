// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageUserInfo.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIHUDNavigationBar.generated.h"

class UGsUICurrencySlot;
class UGsButton;
class UWidgetSwitcher;
class UGsUICurrencyList;
class UGsUICurrencyDetailInfo;

/**
 * HUD 상단에 띄우는 재화 표시
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDNavigationBar : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	UGsUICurrencySlot* _slotDia;
	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	UGsUICurrencySlot* _slotGold;

	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	UGsButton* _btnDropDownCurrency;
	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	UWidgetSwitcher* _switcherDropDownArrow;

	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	UGsUICurrencyList* _uiCurrencyList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	UGsUICurrencyDetailInfo* _currencyInfo;

protected:
	TPair<MessageUserInfo, FDelegateHandle> _handlerUserInfo;

	// 드롭다운 메뉴의 오픈여부
	bool _bIsDropDownOpen = false;
	// 드롭다운 사용 여부
	bool _bEnableDropDown = true;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;

protected:
	virtual void InvalidateAllInternal() override;

protected:
	// 재화 갱신 메시지를 받았을 때
	UFUNCTION()
	void OnUpdateCurrency(uint64 InType);

protected:
	// 재화 슬롯을 클릭했을 때
	UFUNCTION()
	void OnClickCurrencyEntry(class UWidget* InEntry);

	// 재화 충전 버튼을 눌렀을 때
	UFUNCTION()
	void OnClickCurrencyCharge(class UWidget* InEntry);

	// 재화 옆 드롭다운 버튼을 눌렀을 때
	UFUNCTION()
	void OnClickDropDownCurrency();

protected:
	void InvalidateCurrencyList();
	void UpdateCurrency(CurrencyType InType);

	// _bIsDropDownOpen 값변경. bInInvalidate가 false이면 UI갱신 안함.
	void SetOpenDropDownCurrency(bool bInOpen) { _bIsDropDownOpen = bInOpen; }
	void SetEnableDropDown(bool bInEnable) { _bEnableDropDown = bInEnable; }
};
