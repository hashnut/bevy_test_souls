// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageItem.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUITrayNavigationBar.generated.h"

/**
 * 화면 상단에 띄우는 항상 표시되는 항목들
 */
UCLASS()
class T1PROJECT_API UGsUITrayNavigationBar : public UGsUITray, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

public:
	enum class EGsNaviBarAlign
	{
		RIGHT = 0,
		CENTER,
		MARKET,
	};

public:	
	using MsgItemHandle = TPair<MessageItem, FDelegateHandle>;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUINavigationBar")
	TSubclassOf<class UGsUICurrencySlot> _currencyEntryClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	class UWrapBox* _panelCurrency;

	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	class UGsUICurrencySlot* _optionalSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	class UGsButton* _btnDropDownCurrency;

	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	class UWidgetSwitcher* _switcherDropDownArrow;

	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	class UGsUICurrencyList* _uiCurrencyList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	class UGsUICurrencyDetailInfo* _currencyInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	class USpacer* _spacerRight;
	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	class USpacer* _spacerExitBtnSpace;
	UPROPERTY(BlueprintReadOnly, Category = "GsUINavigationBar", meta = (BindWidget))
	class USpacer* _spacerMarketSpace;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _currencySlotHelper;
	
	TPair<MessageUserInfo, FDelegateHandle> _handlerUserInfo;
	TArray<MsgItemHandle> _msgItemHandleList;
	MsgUIHandleArray _msgUIHandleList;

	// 표시할 재화 리스트. 0, 1번 인덱스는 항상 표시
	TArray<CurrencyType> _currencyList;

	// 드롭다운 메뉴의 오픈여부
	bool _bIsDropDownOpen = false;

	// 드롭다운 사용 여부
	bool _bEnableDropDown = true;
	
	// 선택적으로 표시할 재화 타입. CostType::MAX 또는 CurrencyType::NONE, MAX_USER_TYPE일 경우 감춤
	CostType _optionalCostType = CostType::MAX;
	CurrencyType _optionalCurrency = CurrencyType::MAX_USER_TYPE;
	int32 _optionalItemId = 0;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;

	virtual void InvalidateAllInternal() override;

public:
	// HUD 가릴 때 함께 가림
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_TRAY_CINEMATIC; }

public:
	// HUD Mode 변경 시(AHUDLocalPlayer 가 호출)
	void ChangeHUDMode(EGsUIHUDMode InMode);

public:
	void InvalidateCurrencyList();
	void SetOptionalCurrency(CurrencyType InType);
	void SetOptionalCurrencyItem(int32 InItemId);	
	void SetNaviBarAlign(EGsNaviBarAlign InType);

protected:
	// 재화 갱신 메시지를 받았을 때
	UFUNCTION()
	void OnUpdateCurrency(uint64 InType);
	
	void OnAddItemList(FGsItemMessageParamBase& InParam);
	void OnUpdateItemList(FGsItemMessageParamBase& InParam);
	void OnRemoveItem(FGsItemMessageParamBase& InParam);

	// _optionalSlot 에 표시할 메시지를 받았을 때. (주로 전체 창 열 때)
	// 0보다 작은 값 : 드롭다운 표시. _optionalSlot 감춤.
	// MAX_MONEY_TYPE : 드롭다움 감춤. _optionalSlot 감춤.
	// 나머지 값 : 드롭다움 감춤. _optionalSlot에 CurrencyType 값 표시.	
	void OnSetOptionalCurrency(const struct IGsMessageParam* InParam);
	void OnSetOptionalCurrencyItem(const struct IGsMessageParam* InParam);

	void OnChangeHUDMode(const struct IGsMessageParam* InParam);

	void OnHideCurrencyList();

	// _currencySlotHelper에서 재화 슬롯을 생성했을 때
	UFUNCTION()
	void OnCreateCurrencyEntry(class UWidget* InEntry);

	// _currencySlotHelper에서 재화 슬롯을 갱신했을 때
	UFUNCTION()
	void OnRefreshCurrencyEntry(int32 InIndex, class UWidget* InEntry);

	// 재화 슬롯을 클릭했을 때
	UFUNCTION()
	void OnClickCurrencyEntry(class UWidget* InEntry);

	// 재화 충전 버튼을 눌렀을 때
	UFUNCTION()
	void OnClickCurrencyCharge(class UWidget* InEntry);

	// 재화 옆 드롭다운 버튼을 눌렀을 때
	UFUNCTION()
	void OnClickDropDownCurrency();

	// _bIsDropDownOpen 값변경. bInInvalidate가 false이면 UI갱신 안함.
	void SetOpenDropDownCurrency(bool bInOpen) { _bIsDropDownOpen = bInOpen; }

	void SetEnableDropDown(bool bInEnable) { _bEnableDropDown = bInEnable; }

public:
	CurrencyType GetCurrentOptionalCurrency() { return _optionalCurrency;}
};
