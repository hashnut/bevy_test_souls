// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIWindowDepot.generated.h"


class FGsItem;
class UTextBlock;
class UImage;
class UGsButton;
class UScrollBox;
class UProgressBar;
class UGsSimpleTimer;
class UHorizontalBox;
class UWidgetSwitcher;
class UPanelWidget;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsDynamicPanelSlotHelper;
class UGsUIItemSlotLockWidget;
class UGsUIIconBase;
class UGsUIIconItem;
class UGsUIIconItemInventory;
class UGsUIAutoStorageEntryWidget;
struct FGsSchemaAutoDepotConfig;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowDepot : public UGsUIWindow
{
	GENERATED_BODY()

private:
	// 자동으로 특정 슬롯페이지로 이동되는 오프셋 값 (+ , -)
	const float AUTO_SCROLL_TERM_VALUE = 180.f;
	// 한 슬롯당 맥시멈 슬롯 갯수
	const int32 SLOT_MAX_COUNT = 5;
	// 슬롯이 dest에 도착함을 체크할때 term 값
	const float FISNISHED_GOAL_POSITION_VALIDCHECK_VALUE = 50.f;

	const int32 SWITCHER_INDEX_STORAGE = 0;
	const int32 SWITCHER_INDEX_AUTO_DEPOSIT = 1;

private:
	// 각 페이지별 Offset 값 저장에 사용되는 타입
	enum SlotIndexType : uint8
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
	enum SlotPageType : uint8
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

public:
	enum DepotSortType : uint8
	{
		SortDefault = 0,
		SortGrade,
		SortWeight,

		SortMax,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDepotListClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _depotAreaSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItemSlotLock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDepotSlotSortList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _sortItemListPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _depotSlotCountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyItemList;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDefaultSort;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _defaultSortSwitcher; // 0 : selected  1 : unselected
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGradeSort;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _gradeSortSwitcher;  // 0 : selected  1 : unselected
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnWeightSort;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _weightSortSwitcher;  // 0 : selected  1 : unselected
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSetupAutoDeposit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;


protected:
	// setup switcher area
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCloseSetup;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirmAutoSetting;

protected:
	// 자동보관 아이템 리스트 동적 생성 및 관리
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _autoSettingEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _autoSettingPanelSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _autoStorageItemListScrollBox;
	
	FGsToggleGroup _toggleGroupAutoDeposit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIAutoDepositItemList* _autoDepositItemList;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _rootScrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _rootPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _actionModeText; // 찾을물품 / 보관물품
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _priceGuideText; // 찾는 비용 / 보관 비용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _priceText;

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

public:
	UPROPERTY(EditDefaultsOnly, Category = "CurveData")
	UCurveFloat* _curve;
	UPROPERTY()
	UGsSimpleTimer* _timer;
	

public:
	//Toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UPanelWidget* _panelInvenTab;

	FGsToggleGroup _toggleGroup;


private:
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_itemContentsDelegateList;
	TArray<TPair<MessageContentInven, FDelegateHandle>> _inventoryDelegateList;
	TArray<TPair<MessageUserInfo, FDelegateHandle>> _userInfoDelegateList;

private:
	// 출력할 아이템들 DBID 리스트
	TArray<ItemDBId> _displayItemList;

	// 자동보관 설정
	struct AutoDepositSetting
	{
		int32 _groupId = 0;
		ItemGrade _grade = ItemGrade::NONE;
		TArray<TPair<ItemId, ItemGrade>> _itemIdList;
		TMap<ItemId, bool> _autoDepositMap;
		const FGsSchemaAutoDepotConfig* _autoDepositConfig = nullptr;
		
		bool HasAutoDepositItem()
		{
			for (const TPair<ItemId, bool>& autoDepositData : _autoDepositMap)
			{
				if (autoDepositData.Value)
				{
					return true;
				}
			}
			return false;
		}
	};

	TArray<AutoDepositSetting> _autoDepositSettingList;
	bool _isAutoDepositSelectionRefresh = false;
	bool _isAutoDepositGroupGradeRefresh = false;

	TWeakObjectPtr<UGsUIIconItemInventory> _curSelectIcon;

	bool _isDeposit = true;

private:
	DepotSortType _eDepotSortType = DepotSortType::SortDefault;

private:
	// key : BasketndexType , value : slot PositionValue
	TMap<SlotIndexType, float> _basketSlotPositionMap;
	SlotIndexType _eSlotBasketIndexType = SlotIndexType::FifthSlotGroupIndex;
	SlotPageType _eSlotBasketPageType = SlotPageType::FirstPage;
	ScrollDircetionType _eScrollDirectionType = ScrollDircetionType::Scroll_None;
	bool _isScrolling = false;
	bool _isAutoScrolling = false;
	bool _findEndScrollOffeset = false;
	float _endScrollOffset = 0.f;
	int32 _slotMoveCount = 0;

public:
	void OnClickIconSlot(class UGsUIIconItem& InIcon);
	void OnClickDepotIconSlot(class UGsUIIconItem& InIcon);
	void OnClickAutoDepositIcon(UGsUIIconItem& InIcon);

public:
	void OnAllUpdateDepotItems(const IGsMessageParam* InParam);
	void UpdateDepotItem(const IGsMessageParam* InParam);
	void OnChangeBasketItems(const IGsMessageParam* InParam);
	void AddBasketItems(const IGsMessageParam* InParam);
	void AddBasketItemList(const IGsMessageParam* InParam);
	void RemoveBasketItems(const IGsMessageParam* InParam);
	void OnChangeStateDeposit(const IGsMessageParam* InParam);
	void OnChangeStateTake(const IGsMessageParam* InParam);
	void InvalidateDisPlayText(const IGsMessageParam* InParam);
	void ExtendDepotSlot(const IGsMessageParam* InParam);
	void OnUpdateWeight(const struct IGsMessageParam* InParam);
	void OnUpdateCurrency(uint64 InCurrencyType);

private:
	void UpdateDepotSort();
	void InvalidateEmptyItemList();
	void InvalidateWeight();
	void InvalidatePrice();

	void SortDepotItemList(DepotSortType InSortType);

private:
	void AutoScroll(float InCurveData);
	void CurrentFocusingSlotPositionCheck();
	void SetDefaultFocusingPos();
	SlotIndexType GetExpectPosType(bool InIsAfter);
	float GetSlotPositionValueByIndexType(const SlotIndexType InType);
	void SetSlotPositionValueByIndexType(const SlotIndexType InType);
	void SetFocusingIndexSwitcher(const ScrollDircetionType InDirType);

private:
	void CloseCalculatorUI();
	void OpenCalculatorUI(const FGsItem* InTargetItem);
	bool IsOpenedCalculatorUI();
	void CloseItemDetailUI();
	void ConfirmItemStackCount(int64 InCount);
	void CancelCalculatorPopup();
	void ResetSelection();

private:
	void SetSlotMove(int32 InMoveTargetPageIndex, int32 InDestPageIndex);
	void InvalidateBasketScroll(int32 InTargetPageIndex);

private:
	void OnRefreshIconWrapBox_Basket(int32 InStartIndex, int32 InOffset, UGsUIIconBase* InIcon);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void InvalidateAllInternal() override;

private:
	void OnLongPressIcon(UGsUIIconItem& InIcon);
	void OnLongPressAutoDepositIcon(UGsUIIconItem& InIcon);

	void SwitchLeftPopupUI(int32 InIndex);
	void SetExtendButtonVisibility(bool InIsVisible);
	void SetSortPanelVisibility(bool InIsVisible);
	void SetAutoDepositPanelVisibility(bool InIsVisible, int32 InIndex = 0);

public:
	UFUNCTION()
	void OnChangeMainScrollEvent(float InCurOffset);
	UFUNCTION()
	void OnSlotClickedTab(int32 InIndex, bool bInIsSelected);
	UFUNCTION()
	void OnSelectChangedTab(int32 InIndex);
	UFUNCTION()
	void OnClickSetup();
	UFUNCTION()
	void OnClickCloseSetup();
	UFUNCTION()
	void OnClickAutoSetting();
	UFUNCTION()
	void OnClickConfirm();
	UFUNCTION()
	void OnClickDefaultSort();
	UFUNCTION()
	void OnClickGradeSort();
	UFUNCTION()
	void OnClickWeightSort();
	UFUNCTION()
	void OnClickSlotLock();
	UFUNCTION()
	void OnClickDepotSlotSortList();
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void AutoSettingListRefreshEntry(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshIconAutoDeposit(int32 InIndex, UGsUIIconBase* InIcon);
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
	void OnSlotClickedAutoDepositGroup(int32 InIndex, bool bInIsSelected);
	UFUNCTION()
	void OnSelectChangedAutoDepositGroup(int32 InIndex);
	void OnChangeAutoDepositGrade(int32 InIndex, ItemGrade InGrade);
};
