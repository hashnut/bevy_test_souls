// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupRecoveryPenalty.generated.h"

/**
 * 패널티 복구 팝업
 */

UENUM()
enum class EGsPenaltyType : uint8
{
	Exp,
	Item,
	Max
};

UENUM()
enum class EGsPenaltyCurrencyType : uint8
{
	Gold,
	Dia,
	HolyWater,
	Max
};

UCLASS()
class T1PROJECT_API UGsUIPopupRecoveryPenalty : public UGsUIPopup
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
		class UGsButton* _closeButton;

	//Toggle
private:
	FGsToggleGroup _toggleGroup;
	UPROPERTY(meta = (BindWidget))
		class UGsSwitcherButton* _expSwitcherButton;
	UPROPERTY(meta = (BindWidget))
		class UGsSwitcherButton* _equipItemSwitcherButton;

public:	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _expCountOffText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _expCountOnText;

public:	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _itemCountOffText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _itemCountOnText;

	//ListView
public:
	UPROPERTY(meta = (BindWidget))
		class UListView* _penaltyListView;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _freeReviveCountText;
	UPROPERTY(meta = (BindWidget))
		class UGsButton* _recoveryButton;

public:
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* _currencyTypeSwitcher;

public:	
	UPROPERTY(meta = (BindWidget))
		class UGsButton* _changeCurrencyButton;

public:
	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* _holyWaterPanel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _holyWaterChargeCountTextBlock;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _noPaneltySwitcher;

private:
	MsgUIHandle _msgUIHandle;

private:
	EGsPenaltyType _penaltyType = EGsPenaltyType::Exp;
	EGsPenaltyCurrencyType _paymentCurrentType;
	CurrencyType _currencyType = CurrencyType::DIA;
	int32 _selectedTabIndex = 0;
	int32 _maxExpPenaltyCount = 0;
	int32 _maxItemPenaltyCount = 0;

private:
	TArray<DeathPenaltyExp> _selectedExpPenalty;
	TArray<DeathPenaltyItem> _selectedItemPenalty;

private:
	float _time = 0;
	float _interval = 0.25;	
	FText _countFormat;
	int32 _currencyIndex;	
	FText _inventoryFullText;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry & inMyGeometry,	float inDeltaTime) override;

public:
	void Invalid(const struct IGsMessageParam* InParam = nullptr);
	void InvalidRestoreButton();

private:
	void InvalidExpPenaltyCount();
	void InvalidItemCount();
	void InvalidExpEntryList();
	void InvalidItemEntryList();
	void InvalidFreeRestoreCount();	
	void InvalidLeftTime();
	void InvalidHolyWater();

private:
	void InvalidCurrency();

private:
	UFUNCTION()
		void OnClose();
	UFUNCTION()
		void OnChangeCurrency();	
	UFUNCTION()
		void OnRecovery();
	UFUNCTION()
		void OnClickTab(int32 InIndex);

public:
	virtual void OnInputCancel() override;
	void SetHighlight(class UTextBlock* inWidget, bool inHighLight);

public:
	CurrencyType GetCurrency() { return _currencyType; }
	EGsPenaltyType GetPenaltyType() { return _penaltyType; }
	TArray<DeathPenaltyExp>& GetExpPenalty() { return _selectedExpPenalty; }
	TArray<DeathPenaltyItem>& GetItemPenalty() { return _selectedItemPenalty; }
	void ClearRestorePenaltyList();	
	static CurrencyType GetCurrencyType(EGsPenaltyCurrencyType inCurrencyType);
	static EGsPenaltyCurrencyType GetCurrencyType(CurrencyType inCurrencyType);
};
