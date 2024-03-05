// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/GsItem.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "GsUIPopupItemDetail.generated.h"

class UTextBlock;
class UGsButton;
class UPanelWidget;
class UWidgetSwitcher;
class UGsUIItemDefaultInfo;
class UGsUIItemDetailInfo;
class USizeBox;

DECLARE_MULTICAST_DELEGATE(FOnCloseItemDetailDelegate);
DECLARE_MULTICAST_DELEGATE(FOnItemDetailCompareDelegate);
DECLARE_MULTICAST_DELEGATE_FourParams(FOnItemDetailCompareNextDelegate, const EGsEquipSlotType, const bool, const bool, const bool);
DECLARE_DELEGATE_RetVal(const FGsItem*, FGetItemDataDelegate);
DECLARE_DELEGATE_OneParam(FOnChangeOtherItemEnchantCount, int32);
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupItemDetail : public UUserWidget
{
	GENERATED_BODY()	

public:
	enum DisplayModeType : uint8
	{
		ItemDetailMode = 0,
		ItemCompareMode,
		ItemCollectionMode,

		ModeMax,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemDefaultInfo* _itemDefaultInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemDetailInfo* _itemDetailInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _buttonsGroup1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _buttonsGroup2;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textUseBtn;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUse;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSeveralUse;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDelete;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDisableDelete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEnchant;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDisableEnchant;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDecompose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDisableDecompose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItemCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDisableItemCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMarket;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDisableMarket;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCompare;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCompareOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDisableCompare;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChangeCollectionItem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRegisterCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnComparePrev;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCompareNext;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textItemNameCompare;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textEquipped;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItemFusion;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItemFusionDisable;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _equipDeleteBtnSwitcher; //  0 on , 1 off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _equipEnchantBtnSwitcher; //  0 on , 1 off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _equipDecomposeBtnSwitcher; //  0 on , 1 off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _itemCollectionBtnSwitcher; //  0 on , 1 off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _marketBtnSwitcher; //  0 on , 1 off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _compareBtnSwitcher; // 0 normal , 1 on compare , 2 disable compare
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _modeSwitcher; // 0 itemDetail , 1 itemCompare , 2 ItemCollection 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _equipItemFusionSwitcher; //  0 on , 1 off


protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USizeBox* _collectionSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _personItemCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _guildItemCollection;

private:
	bool _isHideButtonsFlag = false; // 강화, 잠금, 착용등의 버튼들을 가림
	bool _isHideButtonsBottomFlag = false; // 버리기, 분해, 사용 버튼들을 가림
	bool _isHideLockButtonFlag = false; // 잠금 설정 버튼
	bool _isOpenCompare = false;
	bool _isCenterPopup = false;
	bool _isHideBg = false;
	bool _isItemCollectionMarking = false;
	bool _isUseLock = true;
	DisplayModeType _modeType = DisplayModeType::ItemDetailMode;

	// 아이템 컬렉션은 상황에 따라 "장착/등록"버튼이 나온다.
	bool _isBtnRegisterType = true;

public:
	FGetItemDataDelegate OnGetItemDataDelegate;
	FOnCloseItemDetailDelegate OnCloseItemDetailDelegate;
	FOnItemDetailCompareDelegate OnItemDetailCompareDelegate;
	FOnItemDetailCompareNextDelegate OnItemDetailCompareNextDelegate;
	FOnChangeOtherItemEnchantCount OnChangeOtherItemEnchantCount;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetItemDefaultData(bool InHideButtons, DisplayModeType InModeType = DisplayModeType::ItemDetailMode, bool InHideButtonsBottom = false, bool InUseLock = true, bool InIsItemCollectionMarking = false);
	void SetItemDetailData(bool InIsItemCollectionMarking = false, bool InSkipUpdateDetailInfo = false);
	void SetItemDefaultData(const FGsItem* InItem, bool InHideButtons, DisplayModeType InModeType = DisplayModeType::ItemDetailMode, bool InHideButtonsBottom = false, bool InUseLock = true, bool InIsItemCollectionMarking = false);
	void SetItemDetailData(const FGsItem* InItem, bool InIsItemCollectionMarking = false, bool InSkipUpdateDetailInfo = false);
public:
	void SetCurrencyData(const CurrencyType InCurrencyType, const Currency InAmount, bool InHideButtons, DisplayModeType InModeType = DisplayModeType::ItemDetailMode);
	void SetItemCompareDetailData(const FGsItem* InCompareItem);
	void SetCompareValueVisibility(bool InState);
	void SetDisplayDetailMode(DisplayModeType InModeType);
	void UpdateItemData(const uint64 InItemDBID, bool InSkipUpdateDetailInfo = false);
	void SetHideVisiblity(bool InState);
	bool IsRemoveItem(const uint64 InItemDBID);
	void SetCompareItemEnchantCount(int32 InCount);

	// 2021/07/23 PKT - 디테일 창은 기본적으로 소유하고 있는 아이템에게 기능을 제공함. 아이템 컬렉션 같은 경우 소유하고 있지 않은 아이템에 기능을 제공해야 함.
	void SetItemCollectionConditionItemOnly(bool InShowBtn, bool InEnchant, bool InChanged, bool InRegister, bool InIsBtnRegisterType);
	void SetItemCollectionRegisterInfoText(const FText& InText);
private:
	void SetDetailModeBtns();
	void SetCompareModeBtns();
	void SetCollectionModeBtns(bool InChanged, bool InRegister);
	void CheckBtnState();
	bool IsEnableUseBtnState();
	bool IsActiveEnableBtnState();
	bool IsActiveSeveralBtnState();
	const FGsItem* GetDisPlayItemData() const;
	
private:
	void OnConfirmItemDeleteStackCount(ItemAmount InCount);
	void OnConfirmSeveralUseCalculator(uint64 InItemDBID, ItemAmount InCount);
	void OpenDeleteCalculatorUI();
	void OpenDeleteConfirmUI(ItemAmount InAmount);

public:
	void SetCompareState(bool InState) {_isOpenCompare = InState;}

public:
	bool GetCompareState() { return _isOpenCompare; }

private:

	bool IsValidItem() const;

	bool IsOwnedItem() const;

	bool CheckOwnedItem() const;

	bool CanEnchantImmediately() const;

	bool CanDeleteImmediately() const;

	bool CanDecomposeImmediately() const;

	bool CanItemFusionImmediately() const;

	bool CanUseMarket() const;

	bool IsExpiredTimeItem() const;

	ItemCategoryMain GetItemCategoryMain() const;

	ItemCategorySub GetItemCategorySub() const;

	ItemType GetItemType() const;

	ItemDBId GetItemDBId() const;

	ItemAmount GetItemAmount() const;

	FText GetItemActionText() const;

public:
	UFUNCTION()
	void OnClickUseItem();
	UFUNCTION()
	void OnClickSeveralUseItem();
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnClickDelete();
	UFUNCTION()
	void OnClickEnchant();
	UFUNCTION()
	void OnClickDisableDelete();
	UFUNCTION()
	void OnClickDisableEnchant();
	UFUNCTION()
	void OnClickDisableItemUse();
	UFUNCTION()
	void OnClickItemDecompose();
	UFUNCTION()
	void OnClickDisableItemDecompose();
	UFUNCTION()
	void OnClickItemCollection();
	UFUNCTION()
	void OnClickDisableItemCollection();
	UFUNCTION()
	void OnClickMarket();
	UFUNCTION()
	void OnClickDisableMarket();
	UFUNCTION()
	void OnClickItemCompare();
	UFUNCTION()
	void OnClickDisableItemCompare();
	UFUNCTION()
	void OnClickNextCompare();
	UFUNCTION()
	void OnClickItemFusion();
	UFUNCTION()
	void OnRegisterCollection();
	UFUNCTION()
	void OnShiftCollectionItem();
	UFUNCTION()
	void OnShotcutPersonItemCollection();
	UFUNCTION()
	void OnShotcutGuildItemCollection();

	void OnChangeEnchantCount(int32 InCount);
};
