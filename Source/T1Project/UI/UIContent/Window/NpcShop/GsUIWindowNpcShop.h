// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUINpcShopProductEntry.h"
#include "GsUINpcShopAutoBuyListEntry.h"
#include "GsUINpcShopAutoSellListEntry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIWindowNpcShop.generated.h"

class UTextBlock;
class UGsButton;
class UImage;
class UWidgetSwitcher;
class UScrollBox;
class UProgressBar;
class UPanelWidget;
class UGsSimpleTimer;
class UHorizontalBox;
class UGsToggleButton;
class UGsDynamicIconSlotHelper;
class UGsWrapBoxIconSelector;
class FGsNpcShopUIMsgReceiver;
class UGsCheckBox;
class UGsUIWeaponFilter;

struct FGsNpcShopQuickSlotItemInfo;
struct FGsNpcShopItemInfo;
/**
 * Window NpcShop
 */
UCLASS()
class T1PROJECT_API UGsUIWindowNpcShop : public UGsUIWindow
{
	GENERATED_BODY()

private:
	// 자동으로 특정 슬롯페이지로 이동되는 오프셋 값 (+ , -)
	const float AUTO_SCROLL_TERM_VALUE = 180.f;
	// 한 슬롯당 맥시멈 슬롯 갯수
	const int32 SHOPPINGBASKET_SLOT_COUNT = 5;
	// 슬롯이 dest에 도착함을 체크할때 term 값
	const float FISNISHED_GOAL_POSITION_VALIDCHECK_VALUE = 50.f;

private:
	enum ProductAreaType : uint8
	{
		DisPlay_Product = 0,
		DisPlay_SetupAutoOrder ,

		DisPlay_Max,
	};

private:
	// 각 페이지별 Offset 값 저장에 사용되는 타입
	enum ShoppingBasketSlotIndexType : uint8
	{
		FirstSlotGroupIndex = 0,
		SecondSlotGroupIndex,
		ThirdSlotGroupIndex,
		FourthSlotGroupIndex,
		FifthSlotGroupIndex,
		SixthSlotGroupIndex,

		SlotGroupMax,
	};
	// 실제 눈에보이는 페이지 타입
	enum ShoppingBasketPage : uint8
	{
		FirstPage = 0,
		SecondPage,
		ThirdPage,
		FourthPage,
		FifthPage,
		SixthPage,

		PageType_Max,
	};

	enum ScrollDircetionType : uint8
	{
		Scroll_None = 0,
		Scroll_Return_Left,
		Scroll_Return_Right,
		Scroll_Left,
		Scroll_Right,
	};

	enum AutoSettingType : uint8
	{
		AutoBuySetting = 0,
		AutoSellSetting,

		SettingMax
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _leftPopupSwitcher; // 0 : ProductList , 1 : Setup AutoOrderList
		
protected:
	// 상품목록 관련 Property
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnProductListClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCloseSetup;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _shopNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyItemList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _productListScrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _autoBuyItemListScrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelAutoBuyItemList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelAutoSellItemList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSetupAutoOrder;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoBuy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;	// 구매/판매 결정 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageDevideLineUnder;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _sizeBoxUnder;


protected:
	//Toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UPanelWidget* _panelInvenTab;
	FGsToggleGroup _toggleGroup;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_All;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_Filter;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_Potion;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_etc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_EquipWeapon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_EquipArmor;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_EquipAccessory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_SkillBookTwoHandSword;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_SkillBookDagger;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_SkillBookBow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsCheckBox* _checkBox_SkillBookWand;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UGsUIWeaponFilter* _uiWeaponFilter;


protected:
	// 구매 리스트 동적 생성 및 관리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _buyListEntryWidgetClass;
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _buyListPanelSlotHelper;

protected:
	//Toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_Toggle", meta = (BindWidget))
	UGsToggleButton* _stupBuyToggleBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_Toggle", meta = (BindWidget))
	UGsToggleButton* _stupSellToggleBtn;

	FGsToggleGroup _toggleGroupSetup;

	// 자동구매 리스트 동적 생성 및 관리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _autoBuyListEntryWidgetClass;
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _autoBuyListPanelSlotHelper;

	// 자동판매 리스트 동적 생성 및 관리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _autoSellListEntryWidgetClass;
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _autoSellListPanelSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoResetSetting;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoConfirmSetting;


protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _shoppingBasketRootPanel;
	// 장바구니(퀵슬롯) 관련 Property
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _shopActionModeText; // 구매목록 / 판매목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _shopPriceGuideText; 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _shopPriceText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _shoppingBasketScrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgCurrencyIcon;

	// 첫번째 페이지
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _firstPageIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _firstPageIconSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _firstSlotPageRootPanel;

	// 두번째 페이지
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _secondPageIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _secondPageIconSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _secondSlotPageRootPanel;

	// 세번째 페이지
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _thirdPageIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _thirdPageIconSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _thirdSlotPageRootPanel;

	// 네번째 페이지
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _fourthPageIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _fourthPageIconSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _fourthSlotPageRootPanel;

	// 다섯번째 페이지
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _fifthPageIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _fifthPageIconSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _fifthSlotPageRootPanel;

	// 여섯번째 페이지
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _sixthPageIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _sixthPageIconSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _sixthSlotPageRootPanel;
	
	// 0 : on , 1 : off 
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _firstPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _secondPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _thirdPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _fourthPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _fifthPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _sixthPageSwitcher;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _expectWeightPerText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _expectWeightProgressBar;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTaxInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTotalTax;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTaxDetail;

public:
	UPROPERTY(EditDefaultsOnly, Category = "CurveData")
	UCurveFloat* _curve;
	UPROPERTY()
	UGsSimpleTimer* _timer;


private:
	TArray<FGsNpcShopItemInfo*> _disPlayItemInfoList;
	UGsUINpcShopProductEntry* _curSelectedEntryInfo = nullptr;
	FGsNpcShopUIMsgReceiver* _uiMsgReceiver;
	TArray<int32> _shopGroupIdList;
	bool _isBuyMode = true;
	AutoSettingType _eAutoSettingType = AutoSettingType::AutoBuySetting;
	NpcFunctionType _shopNpcFunctionType;
	bool _isCurrencyDirty = true;

private:
	TMap<ENpcShopProductListTab, UGsCheckBox*> _shopProductListTabWidgetMap;
	TMap<NpcFunctionType, TArray<ENpcShopProductListTab>> _npcFunctionTypeTabListMap;
	ENpcShopProductListTab _shopTabType;
	int32 _currentInvenTabIndex = 0;

private:
	// ShoppingBasket QuickSlot Member
	// key : ShoppingBasketndexType , value : slot PositionValue
	TMap<ShoppingBasketSlotIndexType, float> _shoppingBasketSlotPositionMap;
	ShoppingBasketSlotIndexType _eShoppingBasketSlotIndexType = ShoppingBasketSlotIndexType::FirstSlotGroupIndex;
	ShoppingBasketPage _eShoppingBasketPageType = ShoppingBasketPage::FirstPage;
	ScrollDircetionType _eScrollDirectionType = ScrollDircetionType::Scroll_None;
	bool _isScrolling = false;
	bool _isAutoScrolling = false;
	bool _findEndScrollOffeset = false;
	float _endScrollOffset = 0.f;
	int32 _slotMoveCount = 0;

	// 튜토리얼 관련 항목
public:
	DECLARE_DELEGATE_OneParam(FOnTutorialListItem, UWidget* /*TargetWidget*/);
	FOnTutorialListItem OnTutorialListItem;
private:
	ItemId _tutorialItemId = INVALID_ITEM_ID;
	bool _slotClickLock = false;

private:
	void OpenCalculatorUI();
	void CloseCalculatorUI();
	void InitCalculatorUI();
	bool IsOpenedCalculatorUI();
	void ConfirmItemStackCount(int64 InCount);
	void ResetSelctionInfo();

private:
	// 상점에 맞는 탭을 설정한다.
	void ChangeShopTab(const NpcFunctionType InType, const int32 InIndex);
	// 상점 유형과 탭 위치에 맞는 탭 유형을 반환한다.
	ENpcShopProductListTab GetProductListTabType(NpcFunctionType InFunctionType, int32 InIndex);

private:	
	void OnClickIconSlot(class UGsUIIconItem& InIcon);
	void AutoScroll(float InCurveData);
	void CurrentFocusingSlotPositionCheck();
	void SetDefaultFocusingPos();
	UGsUIWindowNpcShop::ShoppingBasketSlotIndexType GetExpectPosType(bool InIsAfter);
	float GetSlotPositionValueByIndexType(const ShoppingBasketSlotIndexType InType);
	void SetSlotPositionValueByIndexType(const ShoppingBasketSlotIndexType InType);
	void SetFocusingIndexSwitcher(const ScrollDircetionType InDirType);


private:
	// ShoppingBasket Func
	void SetSlotMove(int32 InMoveTargetPageIndex, int32 InDestPageIndex);
	void OnRefreshIconWrapBox_Basket(int32 InStartIndex, int32 InOffset, UGsUIIconBase* InIcon);

private:
	void OnLongPressIcon(UGsUIIconItem& InIcon);

public:
	void InvalidateText();
	void InvalidateShopType(const NpcFunctionType InNpcFunctionType);
	void InvalidateShopMode(const bool InIsBuyMode);
	void InvalidateAllShoppingBasketUI();
	void InvalidateShoppingBasketScroll(int32 InSlotIndex);
	void InvalidateProductItemList(const NpcFunctionType InNpcFunctionType, const TArray<int32>& InShopGroupIdList, const ENpcShopProductListTab InTabType, const bool InIsBuyMode);
	void InvalidateCurrencyIcon(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId);
	void OnClickEntryWidget(UGsUINpcShopProductEntry* InEntryUI);
	void UpdateShopItemList();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual bool SyncContentState() override;

	virtual bool OnBack() override;

protected:
	UFUNCTION()
	void ItemListRefreshEntry(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void AutoBuyItemListRefreshEntry(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void AutoSellItemListRefreshEntry(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnSlotClickedTab(int32 InIndex, bool bInIsSelected);
	UFUNCTION()
	void OnSelectChangedTab(int32 InIndex);
	UFUNCTION()
	void OnChangeMainScrollEvent(float InCurOffset);
	UFUNCTION()
	void OnClickConfirm();

	void OnWeaponFilterSave();

public:
	UFUNCTION()
	void OnRefreshIconWrapBox_FristPage(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIconWrapBox_SecondPage(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIconWrapBox_ThirdPage(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIconWrapBox_fourthPage(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIconWrapBox_fifthPage(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIconWrapBox_sixthPage(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnClickSetup();
	UFUNCTION()
	void OnClickCloseSetup();
	UFUNCTION()
	void OnClickResetAutoSetting();
	UFUNCTION()
	void OnClickConfirmAutoSetting();
	UFUNCTION()
	void OnClickAutoBuy();
	UFUNCTION()
	void OnSelectSetupTab(int32 index);

public:
	void SetShopName(const NpcFunctionType InType, const struct FGsSchemaNpcData* InNpcData);

	// 튜튜리얼 관련 항목
public:
	void SetTutorial(ItemId InItemId);	
	void SetSlotClickLock(bool InIsLock) { _slotClickLock = InIsLock; }

private:
	bool IsTutorial() const { return (INVALID_ITEM_ID != _tutorialItemId) ? true : false; }

	UFUNCTION()
	void OnScrollProductList(float InCurrentOffset);

private: 
	Currency GetTaxIncludedPriceInBasket();
};
