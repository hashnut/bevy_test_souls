#pragma once

#include "CoreMinimal.h"
#include "GsSchemaEnums.h"
#include "Unlock/GsUnlockDefine.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Item/Collection/GsItemCollectionFilterBase.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageContents.h"
#include "UI/UIContent/Helper/GsRedDotChecker.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Classes/GsPoolUObject.h"
#include "Item/Collection/EItemCollectionMainCategory.h"
#include "../Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "../Item/Collection/EItemCollectionEnumData.h"
#include "../StatFilterSystem/GsStatFilterUnit.h"
#include "GsUIWindowItemCollection.generated.h"


class FGsItem;
class UEditableTextBox;
class UGsButton;
class UListView;
class UGsDynamicPanelSlotHelper;
class UGsUIIconItem;
class UScrollBox;
class UTextBlock;
class UUserWidget;
class UWidget;
class UWidgetSwitcher;
class UGsToggleButton;
struct IGsMessageParam;

/**
 * 아이템 컬렉션 윈도우
 */
UCLASS()
class T1PROJECT_API UGsUIWindowItemCollection : public UGsUIWindow
{
	GENERATED_BODY()

public:
	using MsgHandleMarket = TPair<MessageContentMarket, FDelegateHandle>;

public:

	static constexpr uint8 ITEM_COLLECTION_ENCHANT_MAX_LEVEL = 10;

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
protected:
	// 2021/08/03 PKT - 뒤로 가기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	// 2021/08/03 PKT - 닫기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	// 2021/08/03 PKT - main tab ..
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperMainBtnTab;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfMainBtnTabEntry;

	// 2021/08/03 PKT - main tab
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBtnTabMainbList;

	// 2021/08/03 PKT - sub tab ..
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBtnTabSubList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSubBtnTab;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfBtnTabSubSlot;

	// 2021/08/03 PKT - item collection switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherItemCollectionList;

	// 2022/01/04 PKT - Renewal - 아이템 컬렉션 리스트 갯수가 너무 많아 메모리를 너무 많이 사용함. "UGsDynamicPanelSlotHelper" -> "UListView"로 변경
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsUIItemCollectionBase))
	UListView* _listViewCollectionItems;

	// 2021/08/03 PKT - stat filter open button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStatFilterOpen;

	// 2021/08/03 PKT - text search
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _textSearch;

	// 2021/08/03 PKT - reset button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReset;
	
	// 2021/08/03 PKT - item collectino 상태 확인
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCollectionStatus;

	// 2021/08/03 PKT - page 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	UGsButton* _btnPrevPage;

	// 2021/08/03 PKT - page
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	UGsButton* _btnNextPage;

	// 2021/08/03 PKT - page count
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	UTextBlock* _textPage;

	// 2022/01/19 PKT - 전체
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _btnViewTypeAll;

	// 2022/01/19 PKT - 완료
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _btnViewTypeComplected;

	// 2022/01/19 PKT - 미달성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _btnViewTypeProgress;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetailPanel;

	// 2022/01/05 PKT - collection list Data Entry Buffer
	UPROPERTY()
	UGsPoolUObject* _poolFieldData;

	// 2021/08/02 PKT - main category toggle
	FGsToggleGroup _toggleGroupMainTabs;

	// 2021/08/02 PKT - sub category toggle
	FGsToggleGroup _toggleGroupSubTabs;

	// 2023/1/9 PKT - 
	FGsToggleGroup _toggleGroupViewTypeBtns;

	// 2021/08/05 PKT - 필터 대상 검색 어
	FString _textFilterWord;

protected:
	// 2023/1/10 PKT - 윈도우를 떠나기 전 화면을 캡쳐
	struct FGSCaptureData
	{
		bool _IsRecoverFlag = false;
		int32 _mainTabIndex = 0;
		int32 _subTabIndex = 0;
		int32 _viewTypeIndex = 0;

		FText _searchWord = FText::GetEmpty();
		float _scrollValue = 0.f;

		int32 _detailTabIndex = 0;

		TArray<StatType> _selectedStat;

		CollectionId _selectedFieldId = INVALID_COLLECTION_ID;
		CollectionConditionId _selectedConditionId = INVALID_COLLECTION_CONDITION_ID;
	};

	FGSCaptureData _currentCaptureData;
	FGSCaptureData _savedCaptureData;

protected:
	TArray<MsgHandleMarket> _msgHandleMarketList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	
	virtual bool SyncContentState() override;
	virtual bool OnBack() override;

private:
	void BindMessages();
	void UnbindMessages();

private:
	// 2022/12/30 PKT - Build Main Tab
	void BuildMainTab();
	// 2023/1/3 PKT - Build Sub Category
	void BuildSubCategory();
	// 2023/1/3 PKT - Build Item Collection View Data
	void BuildItemCollectionViewData();
	// 2023/1/9 PKT - Make Filter
	const TSet<ECollectionSortOrder> MakeViewListFilter() const;
	// 2023/1/3 PKT - Build Filter
	using PropertyFilter = FGsItemCollectionManager::PropertyFilter;
	TArray<TSharedPtr<PropertyFilter>> MakePropertyFilter() const;
	// 2023/1/5 PKT - Main/Sub Tab 상황에 맞게 Filter
	TSharedPtr<PropertyFilter> MakeCategoryPropertyFilter() const;
	// 2023/1/5 PKT - Stat Filter
	TSharedPtr<PropertyFilter> MakeStatFilter() const;
	// 2023/1/5 PKT - Word Filter
	TSharedPtr<PropertyFilter> MakeSearchWordFilter() const;

	/**
	 * Invalidate
	 */
	// 2023/1/5 PKT - Page Info
	void InvalidatePage(int32 InOffset = 0, int32 maxCount = 0);
	// 2023/1/9 PKT - RedDot
	void InvalidateRedDot();
	// 2023/1/9 PKT - View List
	void InvalidateViewItems();
	// 2023/1/9 PKT - Detail Panel
	void InvalidateDetailPanel();

public:
	void InvalidateFieldSet(CollectionId InFieldId = INVALID_COLLECTION_ID);

	void SearchMaterial(ItemId InMaterialId);

	void ExitMaterialSetDetail();

	void OnContentsUnlock(EGsUnlockContentsType InType);

	/************************************************************************/
	/* bind 이벤트                                                           */
	/************************************************************************/
private:
	// 2021/08/03 PKT - main tab...
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshMainTab(int32 InIndex, UWidget* InEntry);

	// 2021/08/03 PKT - sub tab...
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshSubTab(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickedField(UUserWidget* InEntry, uint32 InFieldId);

	UFUNCTION()
	void OnClickedConditionIcon(UUserWidget* InEntry, uint32 InCollectionConditionId);

	UFUNCTION()
	void OnLongPressedConditionIcon(UGsUIIconItem* InIcon, UUserWidget* InEntry, uint32 InCollectionConditionId);

	// 2021/08/03 PKT - open tray
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnTrayOpenStatFilter();
	
	// 2023/1/5 PKT - 검색어
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	UFUNCTION()
	void OnTextChangedMsg(const FText& InText);

	// 2021/08/03 PKT - serach clear
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedClearSearchWord();

	UFUNCTION()
	void OnScrollListView(float InOffsetInItems, float InDistanceRemaining);

	UFUNCTION()
	void OnClickedListViewItem(UObject* InListItem);

	// 2021/07/16 PKT - prev page
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedShiftPrevPage();

	// 2021/07/16 PKT - Next page
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedShiftNextPage();

	void OnCallbackStatFilterApply(const TArray<StatType>& InSelectedStatSet);

private:
	// 2021/08/03 PKT - main toggle
	void OnChangedToogleMainTab(int32 InIndex);
	// 2023/1/9 PKT - Sub Category
	void OnChangedToogleSubTab(int32 InIndex);
	// 2023/1/9 PKT - List View Type
	void OnChangedToogleListViewType(int32 InIndex);

	// 2023/1/10 PKT - Selected Field
	void SelectedField(CollectionId InCollectionId, const int32 InSelectedTabIndex = -1);
	// 2023/1/10 PKT - Selected Condition
	void SelectedCondition(CollectionId InCollectionId, CollectionConditionId InCollectionConditionId, const int32 InSelectedTabIndex = -1);

private:
	void OnReceiveItemPriceInfo(const IGsMessageParam*);
	void OnReceiveCollectionPriceList(const IGsMessageParam* InParam);
	void OnRequestRefreshPrice(const IGsMessageParam*);
};