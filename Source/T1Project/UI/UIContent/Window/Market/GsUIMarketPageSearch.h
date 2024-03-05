// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketPageBase.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/Control/GsStaticPanelSlotHelper.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedMarketEnum.h"
#include "Classes/GsPoolUObject.h"
#include "UTIL/GsIntervalChecker.h"

#include "GsUIMarketPageSearch.generated.h"

class UTextBlock;
class UGsButton;
class UGsSwitcherButton;
class UPanelWidget;
class UListView;
class UEditableText;
class UScrollBox;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
class UGsUIMarketCategoryListEntry;
struct FGsMarketItemGroupData;
struct FGsMarketItemData;
struct FGsSchemaMarketCategoryData;
struct FGsSchemaMarketSubCategoryData;
class UGsMarketItemGroupListItem;
class UGsMarketItemListItem;
class FGsMarketListViewControlBase;
struct FGsSchemaItemCommon;

/**
 * 거래소 검색 페이지
 */
UCLASS()
class UGsUIMarketPageSearch : public UGsUIMarketPageBase
{
	GENERATED_BODY()

public:
	enum EMarketListType
	{
		ITEM_GROUP = 0,
		INSTACKABLE_ITEM,
		STACKABLE_ITEM
	};

	enum EMarketListDepth
	{
		FIRST = 0,
		SECOND, // 쌓을 수 있는 아이템은 2뎁스 없음
		THIRD
	};
	
	struct FGsMarketCategoryData
	{
		int32 _index;
		UGsUIMarketCategoryListEntry* _slot;
		EGsMarketUICategory _uiCategory;
		const FGsSchemaMarketCategoryData* _tableData;

	public:
		FGsMarketCategoryData() = default;
		explicit FGsMarketCategoryData(int32 InIndex, UGsUIMarketCategoryListEntry* InSlot)
			: _index(InIndex), _slot(InSlot)
		{
			// 0이 NONE 값이므로 +1을 한다 
			_uiCategory = static_cast<EGsMarketUICategory>(_index + 1);
			_tableData = nullptr;
		}

	public:
		void InitData();		
		void SetSubCategory(int32 InSubIndex);
		bool IsSubCategory() const;
		const FGsSchemaMarketCategoryData* GetTableData() const;
		const FGsSchemaMarketSubCategoryData* GetSubCategoryData(int32 InSubIndex) const;
		
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelCategory;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSubCategoryRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSubCategory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollSubCategory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subCategoryEntryClass;

	// 리스트 관련
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsMarketItemGroupListItem))
	UListView* _listViewItemGroup; // 모든 그룹들
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsMarketItemListItem))
	UListView* _listViewInstackableItem; // 장비 등
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsMarketItemListItem))
	UListView* _listViewStackableItem; // 주문서 등
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyList;

	// 정렬버튼(0: 정렬안함, 1: 오름차순, 2: 내림차순)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSortByPriceInstackable;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSortByPricePerUnit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSortByPriceStackable;

	// 하단 버튼 관련	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableText* _editableSearch;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPrev;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFilter;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSearch;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReset;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRefresh;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelectTopItems;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMultiplePurchase;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMultiplePurchase;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelperSubCategory;

	UPROPERTY()
	UGsPoolUObject* _poolItemGroup;
	UPROPERTY()
	UGsPoolUObject* _poolItem;

protected:
	FGsStaticPanelSlotHelper _slotHelperCategory;
	FGsToggleGroup _toggleGroupCategory;
	FGsToggleGroup _toggleGroupSubCategory;
	TMap<EGsMarketUICategory, FGsMarketCategoryData> _categoryDataMap;
	TArray<FGsMarketListViewControlBase*> _listViewControlList;
	
	EGsMarketUICategory _currCategory = EGsMarketUICategory::NONE;
	int32 _currSubCategoryIndex = -1;		
	ItemId _selectedItemId = INVALID_ITEM_ID; // 1뎁스에서는 INVALID_ITEM_ID
	int32 _selectedEnchantLevel = 0;
	EMarketListDepth _currDepth = EMarketListDepth::FIRST;
	EGsMarketUICategory _prevClickedCategory = EGsMarketUICategory::NONE;

	bool _bIsFirstOpen = false; // true일 경우, 북마크 리스트를 받은 후 리스트가 비어있을 경우 ALL로 갱신을 함
	bool _bNeedRefresh = false;

	// 멀티 터치로 서버에 여러번 패킷 보내어 꼬이는 현상 방지 위함
	FGsIntervalChecker _checkCooltime;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void Show(bool bIsWorldMarket) override;
	virtual void OnUpdateItemList() override;
	virtual void OnUpdateBookmark(ItemId InItemId) override;
	virtual void OnRcvBuySuccess(MarketTransactionId InTransactionId) override;
	virtual void OnRcvBuyFail() override;
	virtual void OnUpdateSearch(const FString& InStr) override;
	virtual void OnUpdateFilter(bool bIsGradeChanged) override;
	virtual void OnRcvSearchResult() override;
	virtual void OnReqRefresh() override;
	virtual void OnRcvBookmarkList() override;

public:
	void OnAddToPurchaseList(MarketTransactionId InTransactionId);
	void OnRemoveFromPurchaseList(MarketTransactionId InTransactionId);

protected:
	void InitializeCategoryData();

public:
	void SetCategory(EGsMarketUICategory InCategory);
	void ClearSearchData();

protected:
	void SetCategoryInter(EGsMarketUICategory InCategory, int32 InSubCategoryIndex = -1,
		ItemId InSelectedItemId = INVALID_ITEM_ID, int32 InSelectedEnchantLevel = 0,
		EMarketListDepth InListDepth = EMarketListDepth::FIRST);

	void InvalidateTab();
	void InvalidateList(bool bResetScroll);
	void InvalidateMultiplePurchaseInfo();

	void SetListDataFirstDepth(bool bResetScroll);
	void SetListDataSecondDepth(bool bResetScroll);
	void SetListDataThirdDepth(bool bResetScroll);
	void SetVisibleSubCategory(bool bInShow);
	void SetVisibleEmptyList(bool bIsEmpty);
	void SetUISearchString(const FString& InStr);
	void SendRefresh();

	// 관련 팝업들 강제 닫기
	void CloseAllMarketPopups();

protected:
	void OnClickCategory(int32 InIndex, bool bIsSelected);
	void OnClickSubCategory(int32 InIndex, bool bIsSelected);

	UFUNCTION()
	void OnRefreshEntrySubCategory(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickListViewItemGroup(UObject* InListItem);
	UFUNCTION()
	void OnClickListViewItem(UObject* InListItem);
	UFUNCTION()
	void OnScrollListViewItemGroup(float InOffsetInItems, float InDistanceRemaining);
	UFUNCTION()
	void OnScrollListViewItem(float InOffsetInItems, float InDistanceRemaining);

	UFUNCTION()
	void OnClickSortByPriceInstackable();
	UFUNCTION()
	void OnClickSortByPricePerUnit();
	UFUNCTION()
	void OnClickSortByPriceStackable();	

	UFUNCTION()
	void OnClickPrev();
	UFUNCTION()
	void OnClickFilter();
	UFUNCTION()
	void OnClickSearch();
	UFUNCTION()
	void OnClickReset();
	UFUNCTION()
	void OnClickRefresh();
	UFUNCTION()
	void OnClickSelectTopItems();
	UFUNCTION()
	void OnClickMultiplePurchase();

protected:
	EGsMarketUICategory ConvertIndexToCategory(int32 InIndex);

	FGsMarketCategoryData* GetCategoryData(EGsMarketUICategory InCategory);
	FGsMarketListViewControlBase* GetListViewControl(int32 InActiveIndex);	
	MarketCategory GetMarketCategory(EGsMarketUICategory InCategory, int32 InSubCategoryIndex);
	bool IsBookmarkCategory() const;

public:
	EGsMarketUICategory GetCurrCategory() const { return _currCategory; }
	int32 GetCurrSubCategoryIndex() const { return _currSubCategoryIndex; }
};
