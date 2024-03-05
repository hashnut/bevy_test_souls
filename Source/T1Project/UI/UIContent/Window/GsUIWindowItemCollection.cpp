
#include "GsUIWindowItemCollection.h"

#include "UTIL/GsText.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Item/Collection/EItemCollectionMainCategory.h"
#include "Item/Collection/EItemCollectionSubCategoryAttainmentUnit.h"
#include "Item/Collection/EItemCollectionFilterType.h"
#include "Item/Collection/GsItemCollectionField.h"
#include "Item/Collection/GsItemCollectionFilter.h"

#include "../GameFlow/GsGameFlowGame.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"

#include "Runtime/UMG/Public/Components/EditableTextBox.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/PanelWidget.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/UMG/Public/Components/ListView.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Window/Material/GsUIMaterialGainInfo.h"
#include "UI/UIContent/Window/Material/GsUIMaterialSetGainInfo.h"
#include "UI/UIContent/Window/ItemCollection/GsUIItemCollectionStatus.h"
#include "UI/UIContent/Window/ItemCollection/GsUIItemCollectionListViewEntry.h"
#include "UI/UIContent/Tray/GsUITrayCommonStatFilterInCategory.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/ItemCollection/GsSchemaItemCollection.h"
#include "DataSchema/ItemCollection/GsSchemaItemCollectionMainCategory.h"
#include "DataSchema/ItemCollection/GsSchemaItemCollectionSubCategoryAttainmentUnit.h"
#include "DataSchema/ItemCollection/GsSchemaItemCollectionSubCategoryStatUnit.h"

#include "GsSchemaEnums.h"

#include "T1Project.h"


void UGsUIWindowItemCollection::BeginDestroy()
{
	if (_poolFieldData)
	{
		_poolFieldData->RemovePool();
		_poolFieldData = nullptr;
	}

	if (_slotHelperSubBtnTab)
	{
		_slotHelperSubBtnTab->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowItemCollection::OnRefreshSubTab);
		_slotHelperSubBtnTab = nullptr;
	}

	if (_slotHelperMainBtnTab)
	{
		_slotHelperMainBtnTab->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowItemCollection::OnRefreshMainTab);
	}

	if (_btnStatFilterOpen)
	{
		_btnStatFilterOpen->OnClicked.RemoveDynamic(this, &UGsUIWindowItemCollection::OnTrayOpenStatFilter);
	}

	if (_btnPrevPage)
	{
		_btnPrevPage->OnClicked.RemoveDynamic(this, &UGsUIWindowItemCollection::OnClickedShiftPrevPage);
	}

	if (_btnNextPage)
	{
		_btnNextPage->OnClicked.RemoveDynamic(this, &UGsUIWindowItemCollection::OnClickedShiftNextPage);
	}

	if (_textSearch)
	{
		_textSearch->OnTextCommitted.RemoveDynamic(this, &UGsUIWindowItemCollection::OnTextCommitted);
		_textSearch->OnTextChanged.RemoveDynamic(this, &UGsUIWindowItemCollection::OnTextChangedMsg);
	}

	if (_btnReset)
	{
		_btnReset->OnClicked.RemoveDynamic(this, &UGsUIWindowItemCollection::OnClickedClearSearchWord);
	}

	Super::BeginDestroy();
}

void UGsUIWindowItemCollection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::Collection;

	// 상단 버튼
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowItemCollection::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowItemCollection::OnClickCloseAllStack);
	
	// 2021/08/02 PKT - 메인 탭
	_slotHelperMainBtnTab = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperMainBtnTab->Initialize(_subClassOfMainBtnTabEntry, _scrollBtnTabMainbList);
	_slotHelperMainBtnTab->OnRefreshEntry.AddDynamic(this, &UGsUIWindowItemCollection::OnRefreshMainTab);
	// 2022/12/30 PKT - Main Tab Toggle
	_toggleGroupMainTabs.OnSelectChanged.BindUObject(this, &UGsUIWindowItemCollection::OnChangedToogleMainTab);

	// 2021/08/02 PKT - 서브 탭
	_slotHelperSubBtnTab = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSubBtnTab->Initialize(_subClassOfBtnTabSubSlot, _scrollBtnTabSubList);
	_slotHelperSubBtnTab->OnRefreshEntry.AddDynamic(this, &UGsUIWindowItemCollection::OnRefreshSubTab);
	// 2021/08/02 PKT - 서브 탭 토글
	_toggleGroupSubTabs.OnSelectChanged.BindUObject(this, &UGsUIWindowItemCollection::OnChangedToogleSubTab);

	// 2022/01/04 PKT - item collection list
	_listViewCollectionItems->OnListViewScrolled().AddUObject(this, &UGsUIWindowItemCollection::OnScrollListView);
	_listViewCollectionItems->OnItemClicked().AddUObject(this, &UGsUIWindowItemCollection::OnClickedListViewItem);
	//	
	// 2021/08/02 PKT - 검색
	_textSearch->OnTextCommitted.AddDynamic(this, &UGsUIWindowItemCollection::OnTextCommitted);
	_textSearch->OnTextChanged.AddDynamic(this, &UGsUIWindowItemCollection::OnTextChangedMsg);

	_btnReset->OnClicked.AddDynamic(this, &UGsUIWindowItemCollection::OnClickedClearSearchWord);

	// 2021/08/02 PKT - 필터 검색
	_btnStatFilterOpen->OnClicked.AddDynamic(this, &UGsUIWindowItemCollection::OnTrayOpenStatFilter);

	// 2021/08/02 PKT - page
	_btnPrevPage->OnClicked.AddDynamic(this, &UGsUIWindowItemCollection::OnClickedShiftPrevPage);
	_btnNextPage->OnClicked.AddDynamic(this, &UGsUIWindowItemCollection::OnClickedShiftNextPage);

	// 2022/01/05 PKT - View List Scrollbar Hidden
	_listViewCollectionItems->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	_poolFieldData = NewObject<UGsPoolUObject>(this);

	const static int32 POOL_WARM_UP_SIZE = 1000;
	_poolFieldData->WarmUp<UGsItemCollectionViewEntryData>(POOL_WARM_UP_SIZE);

	// 2022/01/19 PKT - View Type
	_toggleGroupViewTypeBtns.OnSelectChanged.BindUObject(this, &UGsUIWindowItemCollection::OnChangedToogleListViewType);
}

void UGsUIWindowItemCollection::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	BuildMainTab();

	if (false == IsStackRecoverFlag())
	{	
		// 2023/1/10 PKT - 캡쳐 Data 초기화
		_savedCaptureData = FGSCaptureData();
	}

	// 2023/1/16 PKT - move memory
	_currentCaptureData = MoveTemp(_savedCaptureData);
	
	// 2023/1/10 PKT - 검색어 설정
	_textSearch->SetText(_currentCaptureData._searchWord);
	_textFilterWord = _textSearch->GetText().ToString().Replace(TEXT(" "), TEXT(""));

	// 2023/1/9 PKT - 전체 / 달성 / 미달성
	_toggleGroupViewTypeBtns.Clear();
	_toggleGroupViewTypeBtns.AddToggleWidget(_btnViewTypeAll);
	_toggleGroupViewTypeBtns.AddToggleWidget(_btnViewTypeComplected);
	_toggleGroupViewTypeBtns.AddToggleWidget(_btnViewTypeProgress);
	_toggleGroupViewTypeBtns.SetSelectedIndexNoEvent(_currentCaptureData._viewTypeIndex);

	// 2023/1/10 PKT - 메인 Tab Selected
	_toggleGroupMainTabs.SetSelectedIndex(_currentCaptureData._mainTabIndex, true);

	// 2023/1/10 PKT - Field가 선택 되어 있었다면..
	if (INVALID_COLLECTION_ID < _currentCaptureData._selectedFieldId)
	{
		// 2023/1/10 PKT - 조건이 선택 되어 있었다면..
		if (INVALID_COLLECTION_CONDITION_ID < _currentCaptureData._selectedConditionId)
		{
			SelectedCondition(_currentCaptureData._selectedFieldId, _currentCaptureData._selectedConditionId, _currentCaptureData._detailTabIndex);
		}
		else
		{
			SelectedField(_currentCaptureData._selectedFieldId, _currentCaptureData._detailTabIndex);
		}
	}

	// 2023/1/10 PKT - 캡쳐 Data 초기화
	_currentCaptureData._IsRecoverFlag = false;

	if (FGsStatChangeNotifyManager* statChangeNotifyMgr = GSStatChangeNotify())
	{
		statChangeNotifyMgr->Save(StatUpdateReason::ITEM_COLLECTION);
	}
}

void UGsUIWindowItemCollection::NativeDestruct()
{
	if (FGsStatChangeNotifyManager* statChangeNotifyMgr = GSStatChangeNotify())
	{
		statChangeNotifyMgr->CheckNotify(StatUpdateReason::ITEM_COLLECTION);
	}

	UnbindMessages();

	// 2023/1/16 PKT - 검색어 저장
	_currentCaptureData._mainTabIndex = _toggleGroupMainTabs.GetSelectedIndex();
	_currentCaptureData._subTabIndex = _toggleGroupSubTabs.GetSelectedIndex();
	_currentCaptureData._viewTypeIndex = _toggleGroupViewTypeBtns.GetSelectedIndex();
	_currentCaptureData._searchWord = _textSearch->GetText();

	// 2023/2/14 PKT - Detail Paenl
	const int32 activeIndex = _switcherDetailPanel->GetActiveWidgetIndex();
	if (1 == activeIndex)
	{
		TWeakObjectPtr<UGsUIMaterialSetGainInfo> item = Cast<UGsUIMaterialSetGainInfo>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			item->Capture(_currentCaptureData._detailTabIndex);
		}
	}
	else if (2 == activeIndex)
	{
		TWeakObjectPtr<UGsUIMaterialGainInfo> item = Cast<UGsUIMaterialGainInfo>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			item->Capture(_currentCaptureData._detailTabIndex);
		}
	}

	// 2023/1/16 PKT - Saved
	_savedCaptureData = MoveTemp(_currentCaptureData);
	_savedCaptureData._IsRecoverFlag = true;

	Super::NativeDestruct();
}

bool UGsUIWindowItemCollection::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsItemCollection))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_ITEM_COLLECTION);
		return true;
	}

	return false;
}

bool UGsUIWindowItemCollection::OnBack()
{
	if (GUI()->IsActiveWidget(TEXT("TrayCommonStatFilterInCateogry")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("TrayCommonStatFilterInCateogry"), true);
		if (widget.IsValid())
		{
			widget->Close();

			return true;
		}
	}

	return Super::OnBack();
}

void UGsUIWindowItemCollection::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MMarket& msgMarket = msgMgr->GetMarket();
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_ITEM_PRICE,
		this, &UGsUIWindowItemCollection::OnReceiveItemPriceInfo));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_COLLECTION_PRICE_LIST,
		this, &UGsUIWindowItemCollection::OnReceiveCollectionPriceList));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::REQ_REFRESH_COLLECTION,
		this, &UGsUIWindowItemCollection::OnRequestRefreshPrice));
}

void UGsUIWindowItemCollection::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MMarket& msgMarket = msgMgr->GetMarket();
	for (MsgHandleMarket& handle : _msgHandleMarketList)
	{
		msgMarket.Remove(handle);
	}
	_msgHandleMarketList.Empty();
}

void UGsUIWindowItemCollection::BuildMainTab()
{
	const FGsItemCollectionManager* itemCollectionManager = GSItemCollection();
	if (nullptr == itemCollectionManager)
	{
		GSLOG(Error, TEXT("nullptr == itemCollectionManager"));
		return;
	}

	const TArray<FGsItemCollectionCategory>& categoryRootDatas = itemCollectionManager->CategoryRootDatas();
	// 2023/1/3 PKT - New Build
	_toggleGroupMainTabs.Clear();

	// 2023/1/3 PKT - Refresh
	_slotHelperMainBtnTab->RefreshAll(categoryRootDatas.Num());
}

void UGsUIWindowItemCollection::BuildSubCategory()
{
	const FGsItemCollectionManager* itemCollectionManager = GSItemCollection();
	if (nullptr == itemCollectionManager)
	{
		GSLOG(Error, TEXT("nullptr == itemCollectionManager"));
		return;
	}

	// 2023/1/3 PKT - New Build
	_toggleGroupSubTabs.Clear();

	const TArray<FGsItemCollectionCategory>& categoryRootDatas = itemCollectionManager->CategoryRootDatas();
	const int32 selectedMainTabIndex = _toggleGroupMainTabs.GetSelectedIndex();
	if (false == categoryRootDatas.IsValidIndex(selectedMainTabIndex))
	{
		GSLOG(Error, TEXT("false == mainCategoryDatas.IsValidIndex(selectedMainTabIndex) : [%d]"), selectedMainTabIndex);
		return;
	}

	_slotHelperSubBtnTab->RefreshAll(categoryRootDatas[selectedMainTabIndex]._subCategory.Num());

	_scrollBtnTabSubList->EndInertialScrolling();
	_scrollBtnTabSubList->ScrollToStart();
}

void UGsUIWindowItemCollection::BuildItemCollectionViewData()
{
	auto& listFilter = MakeViewListFilter();
	auto propertyFilter = MakePropertyFilter();
	const int32 viewFieldCount = GSItemCollection()->BuildViewFieldSet(_currentCaptureData._IsRecoverFlag, listFilter, propertyFilter);

	// 2021/10/14 PKT - Page 갱신( 페이지 갱신을 미리 하는 이유는 여기서 0으로 초기화 이후 리스트 스크롤 위치에 따른 갱신이 다시 되기 때문에.. )
	InvalidatePage(0, viewFieldCount);

	const static int32 EMPTY_COUNT = 0;
	if (EMPTY_COUNT == viewFieldCount)
	{
		_switcherItemCollectionList->SetActiveWidgetIndex(1);

		_listViewCollectionItems->ClearListItems();
	}
	else
	{
		_switcherItemCollectionList->SetActiveWidgetIndex(0);

		float scrollOffsetValue = _currentCaptureData._scrollValue;

		if (false == _currentCaptureData._IsRecoverFlag)
		{	// 2023/1/10 PKT - Tab이동 시 등 이벤트를 받아 리스트를 다시 만드는 경우는 대체 아이템을 원복 시켜야 함. 윈도우를 나갔다가 들어왔을 경우에만 유지
			GSItemCollection()->ClearConditionEntryMaterials();

			scrollOffsetValue = 0.f;
		}

		// 2023/1/3 PKT - Pool Clear
		_poolFieldData->ReleaseAll();

		TArray<UGsItemCollectionViewEntryData*> dataEntryList;
		const TArray<const FGsItemCollectionField*> viewFieldSet = GSItemCollection()->ViewFieldSet();
		for (auto& item : viewFieldSet)
		{
			UGsItemCollectionViewEntryData* entryItem = _poolFieldData->Claim<UGsItemCollectionViewEntryData>();
			entryItem->Initialze(item);
			entryItem->SetSelectedCondition(INVALID_COLLECTION_CONDITION_ID);
			entryItem->SetSelectedField(false);

			if (false == entryItem->OnClickedCondition.IsBoundToObject(this))
			{
				entryItem->OnClickedCondition.BindUObject(this, &UGsUIWindowItemCollection::OnClickedConditionIcon);
			}

			if (false == entryItem->OnLongPressedIcon.IsBoundToObject(this))
			{
				entryItem->OnLongPressedIcon.BindUObject(this, &UGsUIWindowItemCollection::OnLongPressedConditionIcon);
			}

			dataEntryList.Emplace(entryItem);
		}

		_listViewCollectionItems->SetListItems(dataEntryList);
		_listViewCollectionItems->RegenerateAllEntries();
		_listViewCollectionItems->SetScrollOffset(scrollOffsetValue);
		_listViewCollectionItems->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	}

	// 2023/1/9 PKT - 디테일 판넬 컬렉션 현황으로 전환
	const static int32 SWITCHER_DETAIL_STATUS = 0;
	_switcherDetailPanel->SetActiveWidgetIndex(SWITCHER_DETAIL_STATUS);
}


const TSet<ECollectionSortOrder> UGsUIWindowItemCollection::MakeViewListFilter() const
{
	// 2021/08/06 PKT - 현재 Filter 상황에 맞는 아이템 컬렉션 리스트를 만들어 준다.(InActive 상태는 보여주지 않는다.)
	TSet<ECollectionSortOrder> listFilter;
	if (0 == _toggleGroupViewTypeBtns.GetSelectedIndex())
	{
		// 2023/1/9 PKT - 전체
		listFilter = { ECollectionSortOrder::ImmediatePossible
			, ECollectionSortOrder::ConditionalPossible
			, ECollectionSortOrder::Progressing
			, ECollectionSortOrder::Idel			
			, ECollectionSortOrder::Completed };
	}
	else if (1 == _toggleGroupViewTypeBtns.GetSelectedIndex())
	{
		// 2023/1/9 PKT - 완료
		// 2023/1/9 PKT - 전체
		listFilter = { ECollectionSortOrder::Completed };
	}
	else if (2 == _toggleGroupViewTypeBtns.GetSelectedIndex())
	{
		listFilter = { ECollectionSortOrder::ImmediatePossible
			, ECollectionSortOrder::ConditionalPossible
			, ECollectionSortOrder::Progressing
			, ECollectionSortOrder::Idel };
	}

	return listFilter;
}

TArray<TSharedPtr<UGsUIWindowItemCollection::PropertyFilter>> UGsUIWindowItemCollection::MakePropertyFilter() const
{
	TArray<TSharedPtr<UGsUIWindowItemCollection::PropertyFilter>> outFilters;

	/**
	 * 1. 카테고리 필터
	 * 2. stat 관련 필터
	 * 3. 검색어 설정 관련 필터
	 */
	auto AddFilter = [](TSharedPtr<UGsUIWindowItemCollection::PropertyFilter> filter, TArray<TSharedPtr<UGsUIWindowItemCollection::PropertyFilter>>& outFilters)
	{
		if (filter.IsValid())
		{
			outFilters.Emplace(filter);
		}
	};

	AddFilter(MakeCategoryPropertyFilter(), outFilters);
	AddFilter(MakeStatFilter(), outFilters);
	AddFilter(MakeSearchWordFilter(), outFilters);

	return outFilters;
}


TSharedPtr<UGsUIWindowItemCollection::PropertyFilter> UGsUIWindowItemCollection::MakeCategoryPropertyFilter() const
{
	FGsItemCollectionManager* ItemCollectionManager = GSItemCollection();
	if (nullptr == ItemCollectionManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemCollectionManager"));
		return nullptr;
	}

	const int32 selectedMainTab = _toggleGroupMainTabs.GetSelectedIndex();	

	TSharedPtr<UGsUIWindowItemCollection::PropertyFilter> outFilter = MakeShared<UGsUIWindowItemCollection::PropertyFilter>();

	EItemCollectionMainCategory mainType = StaticCast<EItemCollectionMainCategory>(selectedMainTab + 1);
	switch (mainType)
	{
	case EItemCollectionMainCategory::AttainmentUnit:	
	{
		auto& categoryRoot = ItemCollectionManager->CategoryRootDatas();
		if (false == categoryRoot.IsValidIndex(selectedMainTab))
		{
			GSLOG(Error, TEXT("false == categoryRoot.IsValidIndex(selectedMainTab) : [%d]"), selectedMainTab);
			return nullptr;
		}

		const int32 selectedSubTab = _toggleGroupSubTabs.GetSelectedIndex();
		if (false == categoryRoot[selectedMainTab]._subCategory.IsValidIndex(selectedSubTab))
		{
			GSLOG(Error, TEXT("false == categoryRoot[selectedMainTab]._subCategory.IsValidIndex(selectedSubTab) : [%d]"), selectedSubTab);
			return nullptr;
		}

		const FGsItemCollectionCategory& subCategory = categoryRoot[selectedMainTab]._subCategory[selectedSubTab];

		// 2023/1/3 PKT - 서브 카테고리 정보
		const FGsSchemaItemCollectionSubCategoryAttainmentUnit* schemaAttainmentUnitData
			= StaticCast<const FGsSchemaItemCollectionSubCategoryAttainmentUnit*>(subCategory._category);

		if (EItemCollectionSubCategoryAttainmentUnit::ALL == schemaAttainmentUnitData->type)
		{	// 2023/1/3 PKT -  전체 보여주기에서는 필터가 의미 없음.
			return nullptr;
		}

		outFilter->Condition = [schemaAttainmentUnitData](const FGsItemCollectionField* InField)
		{
			const FGsSchemaItemCollectionSubCategoryAttainmentUnit* subAttainmentUnit = InField->SchemaData()->AttainmentUnit.GetRow();
			if ( subAttainmentUnit && subAttainmentUnit->type == schemaAttainmentUnitData->type)
			{
				return true;
			}

			return false;
		};
	}	break;
	case EItemCollectionMainCategory::StatUnit:
	{
		auto& categoryRoot = ItemCollectionManager->CategoryRootDatas();
		if (false == categoryRoot.IsValidIndex(selectedMainTab))
		{
			GSLOG(Error, TEXT("false == categoryRoot.IsValidIndex(selectedMainTab) : [%d]"), selectedMainTab);
			return nullptr;
		}

		const int32 selectedSubTab = _toggleGroupSubTabs.GetSelectedIndex();
		if (false == categoryRoot[selectedMainTab]._subCategory.IsValidIndex(selectedSubTab))
		{
			GSLOG(Error, TEXT("false == categoryRoot[selectedMainTab]._subCategory.IsValidIndex(selectedSubTab) : [%d]"), selectedSubTab);
			return nullptr;
		}

		const FGsItemCollectionCategory& subCategory = categoryRoot[selectedMainTab]._subCategory[selectedSubTab];

		// 2023/1/3 PKT - 서브 카테고리 정보
		const FGsSchemaItemCollectionSubCategoryStatUnit* schemaStatUnitData
			= StaticCast<const FGsSchemaItemCollectionSubCategoryStatUnit*>(subCategory._category);

		if (EItemCollectionSubCategoryStatUnit::ALL == schemaStatUnitData->type)
		{	// 2023/1/3 PKT -  전체 보여주기에서는 필터가 의미 없음.
			return nullptr;
		}

		// 2021/07/18 PKT - 필터 조건 타입
		const TSet<FGsStatTypeNameSelector>& statTypeConditions = schemaStatUnitData->statTypes;
		outFilter->Condition = [statTypeConditions](const FGsItemCollectionField* InField)
		{
			for (const auto& item : InField->StatTypeSet())
			{
				if (statTypeConditions.Contains(item))
				{
					return true;
				}
			}
			return false;
		};

	}	break;
	case EItemCollectionMainCategory::Event: 
	{		
		outFilter->Condition = [](const FGsItemCollectionField* InField)
		{	// 2023/1/3 PKT - 이벤트 타입이면서 유효한 기간이라면..
			return (InField->IsEventField() && InField->IsEnableFlag());
		};
	}	break;
	case EItemCollectionMainCategory::BookMark:	
	{
		outFilter->Condition = [](const FGsItemCollectionField* InField)
		{
			return InField->IsBookMarkEnable();
		};
	}	break;
	}
	
	return outFilter;
}

TSharedPtr<UGsUIWindowItemCollection::PropertyFilter> UGsUIWindowItemCollection::MakeStatFilter() const
{
	TArray<StatType> filterStatTypes;
	for (const auto& item : _currentCaptureData._selectedStat)
	{
		filterStatTypes.Emplace(item);
	}

	if (0 >= filterStatTypes.Num())
	{	// 2023/1/5 PKT - Filter Empty
		return nullptr;
	}

	TSharedPtr<UGsUIWindowItemCollection::PropertyFilter> outFilter = MakeShared<UGsUIWindowItemCollection::PropertyFilter>();
	outFilter->Condition = [filterStatTypes](const FGsItemCollectionField* InField)
	{
		return nullptr != InField->StatTypeSet().FindByPredicate([filterStatTypes](const StatType InData)
			{
				return filterStatTypes.Contains(InData);
			}
		);
	};
	
	return outFilter;
}

TSharedPtr<UGsUIWindowItemCollection::PropertyFilter> UGsUIWindowItemCollection::MakeSearchWordFilter() const
{
	if (_textFilterWord.IsEmpty())
	{
		return nullptr;
	}

	FString temp = _textFilterWord;

	TSharedPtr<UGsUIWindowItemCollection::PropertyFilter> outFilter = MakeShared<UGsUIWindowItemCollection::PropertyFilter>();
	outFilter->Condition = [temp](const FGsItemCollectionField* InField)
	{
		return  InField->ContainsSerachWord(temp);
	};

	return outFilter;
}

void UGsUIWindowItemCollection::InvalidatePage(int32 InOffset/* =0 */, int32 maxCount/* =0 */)
{	
	if (0 >= maxCount)
	{	
		_currentCaptureData._scrollValue = 0.f;
		// 2023/1/5 PKT - 리스트에 값이 없으면 0으로 표기 하는게 맞음.
		InOffset = 0;		
	}
	else
	{	
		// 2023/1/5 PKT - 리스트에 값이 있을땐 Index가 1부터 시작해야 함.
		InOffset = InOffset + 1;
	}

	_textPage->SetText(FText::FromString(FString::Format(TEXT("{0} / {1}"), { InOffset, maxCount })));

	_btnPrevPage->SetIsEnabled(1 < InOffset);
	_btnNextPage->SetIsEnabled(InOffset <= maxCount);
}

void UGsUIWindowItemCollection::InvalidateRedDot()
{
	FGsItemCollectionManager* itemCollectionManager = GSItemCollection();
	if (nullptr == itemCollectionManager)
	{
		GSLOG(Error, TEXT("nullptr == itemCollectionManager"));
		return;
	}

	bool isRedDot = itemCollectionManager->IsRedDot();
	for (int32 idx = 0; idx < _slotHelperMainBtnTab->GetItemCount(); ++idx)
	{
		TWeakObjectPtr<UGsUITabButtonCommon> mainTab = StaticCast<UGsUITabButtonCommon*>(_slotHelperMainBtnTab->GetEntry(idx));
		if (EItemCollectionMainCategory::AttainmentUnit == StaticCast<EItemCollectionMainCategory>(idx + 1))
		{
			mainTab->SetIsRedDot(EGsIconRedDotType::NORMAL, isRedDot);
		}
		else
		{
			mainTab->SetIsRedDot(EGsIconRedDotType::NORMAL, false);
		}
	}

	// 2023/1/6 PKT - 현재 Tab이 '난이도별' Tab이 아니라면 서브 카테고리에 대한 레드닷 업데이트는 필요 없음.
	if (EItemCollectionMainCategory::AttainmentUnit != StaticCast<EItemCollectionMainCategory>(_toggleGroupMainTabs.GetSelectedIndex() + 1))
	{
		return;
	}

	// 2023/1/6 PKT - 이름이 너무 길어서..ㅠㅠ;
	using SubCategory = EItemCollectionSubCategoryAttainmentUnit;

	for (int32 idx = 0; idx < _slotHelperSubBtnTab->GetItemCount(); ++idx)
	{
		isRedDot = false;

		SubCategory category = StaticCast<SubCategory>(idx + 1);
		// 2023/1/6 PKT - 전체 카테고리는 레드닷 없음.
		if (SubCategory::ALL != category)
		{	
			isRedDot = itemCollectionManager->IsCategoryRedDot(category);
		}
				
		TWeakObjectPtr<UGsUITabButtonCommon> subTab = StaticCast<UGsUITabButtonCommon*>(_slotHelperSubBtnTab->GetEntry(idx));		
		subTab->SetIsRedDot(EGsIconRedDotType::NORMAL, isRedDot);
	}
}

void UGsUIWindowItemCollection::InvalidateViewItems()
{
	const TArray<UUserWidget*>& displayWidgets = _listViewCollectionItems->GetDisplayedEntryWidgets();
	for (UUserWidget* widget : displayWidgets)
	{
		TWeakObjectPtr<UGsUIItemCollectionListViewEntry> item = Cast<UGsUIItemCollectionListViewEntry>(widget);
		if (false == item.IsValid())
		{
			continue;
		}

		item->Refresh();
	}
}

void UGsUIWindowItemCollection::InvalidateDetailPanel()
{
	const int32 ActiveIndex = _switcherDetailPanel->GetActiveWidgetIndex();
	if (0 == ActiveIndex)
	{
		TWeakObjectPtr<UGsUIItemCollectionStatus> item = Cast<UGsUIItemCollectionStatus>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			item->SetData();
		}
	}
	else if (1 == ActiveIndex)
	{
		TWeakObjectPtr<UGsUIMaterialSetGainInfo> item = Cast<UGsUIMaterialSetGainInfo>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			item->Invalidate();
		}
	}
	else if (2 == ActiveIndex)
	{
		TWeakObjectPtr<UGsUIMaterialGainInfo> item = Cast<UGsUIMaterialGainInfo>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			CollectionConditionId collectionConditionId = _currentCaptureData._selectedConditionId;
			const FGsItemCollectionField* field = GSItemCollection()->FindFieldByConditionId(collectionConditionId);
			if (field)
			{
				auto condition = field->Conditions().FindByPredicate([collectionConditionId](const FGsItemCollectionCondition& InData)
					{
						return InData.Id() == collectionConditionId;
					}
				);

				if (condition->EntryMaterialInfo().Key == item->GetMaterialId())
				{
					// 2023/1/11 PKT - 아이템이 변경 된게 아니라면 상태만 갱신..
					item->Invalidate();
				}
				else
				{
					item->SetData(condition);
				}				
			}
		}
	}	
}

void UGsUIWindowItemCollection::InvalidateFieldSet(CollectionId InFieldId /*= INVALID_COLLECTION_ID*/)
{
	if (INVALID_COLLECTION_ID == InFieldId)
	{	// 2023/1/5 PKT - 리스트 전체를 업데이트 해야 할때에는 Field가 'INVALID_COLLECTION_ID'로 넘어온다.
		BuildItemCollectionViewData();
	}
	else
	{
		for (auto entryItem : _listViewCollectionItems->GetDisplayedEntryWidgets())
		{
			TWeakObjectPtr<UGsUIItemCollectionListViewEntry> item = Cast<UGsUIItemCollectionListViewEntry>(entryItem);
			if (false == item.IsValid())
			{
				continue;
			}

			TWeakObjectPtr<UGsItemCollectionViewEntryData> data = Cast<UGsItemCollectionViewEntryData>(item->GetListItem());
			if (data.IsValid() && data->GetData() && data->GetData()->Id() == InFieldId)
			{
				item->Refresh();
			}
		}
	}

	// 2023/1/6 PKT - 레드닷 갱신
	InvalidateRedDot();
	
	// 2023/1/9 PKT - 아이템 컬렉션 상세 정보 갱신
	InvalidateDetailPanel();
}

void UGsUIWindowItemCollection::SearchMaterial(ItemId InMaterialId)
{
	const FGsSchemaItemCommon* itemCommonData = FGsItem::GetTableDataByID(InMaterialId);
	if (nullptr == itemCommonData)
	{
		GSLOG(Error, TEXT("nullptr == itemCommonData"));
		return;
	}

	if (false == _textSearch->GetText().IsEmpty())
	{
		const static int32 isSame = 0;
		if (_listViewCollectionItems && isSame == _textSearch->GetText().CompareTo(itemCommonData->name))
		{
			// 2023/4/11 PKT - https://jira.com2us.com/jira/browse/C2URWQ-3810
			// 2023/4/11 PKT - 같은 아이템을 중복으로 검색 시 스크롤 위치만 초기화 한다.
			_listViewCollectionItems->SetScrollOffset(0.f);
			InvalidatePage(0, _listViewCollectionItems->GetNumItems());

			return;
		}
	}

	_textSearch->SetText(itemCommonData->name);
	_textSearch->OnTextCommitted.Broadcast(itemCommonData->name, ETextCommit::OnEnter);

	if (INVALID_COLLECTION_CONDITION_ID != _currentCaptureData._selectedConditionId)
	{
		SelectedCondition(_currentCaptureData._selectedFieldId, _currentCaptureData._selectedConditionId);
	}
}

void UGsUIWindowItemCollection::ExitMaterialSetDetail()
{
	if (INVALID_COLLECTION_ID == _currentCaptureData._selectedFieldId)
	{
		return;
	}

	_currentCaptureData._selectedFieldId = INVALID_COLLECTION_ID;
	_currentCaptureData._selectedConditionId = INVALID_COLLECTION_CONDITION_ID;

	SelectedField(INVALID_COLLECTION_ID);

	// 2023/1/9 PKT - 디테일 판넬 컬렉션 현황으로 전환
	const static int32 SWITCHER_DETAIL_STATUS = 0;
	_switcherDetailPanel->SetActiveWidgetIndex(SWITCHER_DETAIL_STATUS);

	InvalidateDetailPanel();
}

void UGsUIWindowItemCollection::OnContentsUnlock(EGsUnlockContentsType InType)
{
	const int32 ActiveIndex = _switcherDetailPanel->GetActiveWidgetIndex();
	if (1 == ActiveIndex)
	{
		TWeakObjectPtr<UGsUIMaterialSetGainInfo> item = Cast<UGsUIMaterialSetGainInfo>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			item->ContentsUnlock(InType);
		}
	}
	else if (2 == ActiveIndex)
	{
		TWeakObjectPtr<UGsUIMaterialGainInfo> item = Cast<UGsUIMaterialGainInfo>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			item->ContentsUnlock(InType);
		}
	}
}

void UGsUIWindowItemCollection::OnRefreshMainTab(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITabButtonCommon> tabButton = Cast<UGsUITabButtonCommon>(InEntry);
	if (false == tabButton.IsValid())
	{
		GSLOG(Error, TEXT("false == tabButton.IsValid()"));
		return;
	}

	const FGsItemCollectionManager* itemCollectionManager = GSItemCollection();
	if (nullptr == itemCollectionManager)
	{
		GSLOG(Warning, TEXT("nullptr == itemCollectionManager"));
		return;
	}

	const TArray<FGsItemCollectionCategory>& categoryRootDatas = itemCollectionManager->CategoryRootDatas();
	if (false == categoryRootDatas.IsValidIndex(InIndex))
	{
		GSLOG(Warning, TEXT("false == categoryRootDatas.IsValidIndex(InIndex) [%d]"), InIndex);
		return;
	}

	tabButton->SetTitleText(categoryRootDatas[InIndex]._name);
	tabButton->SetIsRedDot(EGsIconRedDotType::NORMAL, false);

	// 2021/08/02 PKT - bind widget
	_toggleGroupMainTabs.AddToggleWidget(tabButton->GetToggleGroupEntry());
}

void UGsUIWindowItemCollection::OnRefreshSubTab(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITabButtonCommon> tabButton = Cast<UGsUITabButtonCommon>(InEntry);
	if (false == tabButton.IsValid())
	{
		GSLOG(Warning, TEXT("invaild tabButton"));
		return;
	}

	FGsItemCollectionManager* itemCollectionManager = GSItemCollection();
	if (nullptr == itemCollectionManager)
	{
		GSLOG(Warning, TEXT("nullptr == ItemCollectionManager"));
		return;
	}

	const TArray<FGsItemCollectionCategory>& categoryRootDatas = itemCollectionManager->CategoryRootDatas();
	const int32 selectedMainTabIndex = _toggleGroupMainTabs.GetSelectedIndex();
	if (false == categoryRootDatas.IsValidIndex(selectedMainTabIndex))
	{
		GSLOG(Error, TEXT("false == categoryRootDatas.IsValidIndex(selectedMainTabIndex), [%d]"), selectedMainTabIndex);
		return;
	}

	const FGsItemCollectionCategory& rootDatas = categoryRootDatas[selectedMainTabIndex];
	if (false == rootDatas._subCategory.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == rootDatas._subCategory.IsValidIndex(InIndex), [%d]"), InIndex);
		return;
	}

	// 2021/08/02 PKT - name text
	tabButton->SetTitleText(rootDatas._subCategory[InIndex]._name);
	tabButton->SetIsRedDot(EGsIconRedDotType::NORMAL, false);

	// 2021/08/02 PKT - widget bind
	_toggleGroupSubTabs.AddToggleWidget(tabButton->GetToggleGroupEntry());
}

void UGsUIWindowItemCollection::OnClickedField(UUserWidget* InEntry, uint32 InFieldId)
{
	SelectedField(InFieldId);
}

void UGsUIWindowItemCollection::OnClickedConditionIcon(UUserWidget* InEntry, uint32 InCollectionConditionId)
{
	TWeakObjectPtr<const UGsUIItemCollectionListViewEntry> viewEntry = Cast<UGsUIItemCollectionListViewEntry>(InEntry);
	if (false == viewEntry.IsValid())
	{
		GSLOG(Error, TEXT("false == viewEntry.IsValid()"));
		return;
	}

	TWeakObjectPtr<UGsItemCollectionViewEntryData> itemData = Cast<UGsItemCollectionViewEntryData>(viewEntry->GetListItem());
	if (false == itemData.IsValid())
	{
		GSLOG(Error, TEXT("false == itemData.IsValid()"));
		return;
	}

	SelectedCondition(itemData->GetData()->Id(), InCollectionConditionId);
}

void UGsUIWindowItemCollection::OnLongPressedConditionIcon(UGsUIIconItem* InIcon, UUserWidget* InEntry, uint32 InCollectionConditionId)
{
	/**
	 * Field 내 조건 슬롯을 롱프레스드하면 이쪽으로 콜
	 */
	TWeakObjectPtr<const UGsUIItemCollectionListViewEntry> viewEntry = Cast<UGsUIItemCollectionListViewEntry>(InEntry);
	if (false == viewEntry.IsValid())
	{
		GSLOG(Error, TEXT("false == viewEntry.IsValid()"));
		return;
	}

	TWeakObjectPtr<UGsItemCollectionViewEntryData> itemData = Cast<UGsItemCollectionViewEntryData>(viewEntry->GetListItem());
	if (false == itemData.IsValid())
	{
		GSLOG(Error, TEXT("false == itemData.IsValid()"));
		return;
	}

	const FGsItemCollectionField * findField = itemData->GetData();
	if (nullptr == findField)
	{
		GSLOG(Warning, TEXT("nullptr == findField"));
		return;
	}

	auto findCondition = findField->Conditions().FindByPredicate([InCollectionConditionId](const FGsItemCollectionCondition& InData)
		{
			return InCollectionConditionId == InData.Id();
		}
	);
	if (nullptr == findCondition)
	{
		GSLOG(Warning, TEXT("nullptr == findCondition"));
		return;
	}

	TArray<TWeakPtr<FGsItem>> hasMaterials;
	if (ECollectionMaterialActionType::ImmediatePossible == findCondition->ActionType())
	{	// 2023/1/4 PKT - 즉시 등록 가능한 재료 정보
		findCondition->HasCanCollectMaterials(hasMaterials);
	}
	else if (ECollectionMaterialActionType::ConditionalPossible == findCondition->ActionType())
	{	// 2023/1/4 PKT - 조건부 등록 가능한 재료 정보
		findCondition->HasMaterials(hasMaterials);
	}

	TWeakObjectPtr<const UGsUIIconItem> icon = InIcon;
	if (false == icon.IsValid())
	{
		GSLOG(Warning, TEXT("false == InIcon.IsValid()"));
		return;
	}
	auto findMaterial = hasMaterials.FindByPredicate([icon](const TWeakPtr<FGsItem>& InData)
		{
			return icon.Get()->GetTableId() == InData.Pin().Get()->GetTID();
		}
	);

	if (nullptr == findMaterial || false == findMaterial->IsValid() || findMaterial->Pin().Get()->IsDepotItem())
	{
		// 2023/1/4 PKT - 갖고 있든 없든 띄운다.
		const FGsItemDetailOpenMessageParamTID param(icon.Get()->GetTableId(), true, true, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_TID, &param);
	}
	else
	{	// 2023/1/4 PKT - 아이템을 소지 하고 있다.
		FGsItemDetailOpenMessageParam param(findMaterial->Pin().Get(), false, true, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
	}
}

void UGsUIWindowItemCollection::OnTrayOpenStatFilter()
{	
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		return;
	}
	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	FGsStatFilterHandler* statfilterHandler = (hud) ? hud->GetStatFilterHandler() : nullptr;
	
	if (statfilterHandler)
	{	
		TArray<StatType> contentsAllStat;
		const FGsItemCollectionManager* itemCollectionManager = GSItemCollection();
		if (itemCollectionManager)
		{
			contentsAllStat = itemCollectionManager->ContentsStatAll().Array();
		}

		statfilterHandler->OpenWidget(contentsAllStat, _currentCaptureData._selectedStat, [this](const TArray<StatType>& InData)
			{
				this->OnCallbackStatFilterApply(InData);
			}
		);
	}
}

void UGsUIWindowItemCollection::OnCallbackStatFilterApply(const TArray<StatType>& InSelectedStatSet)
{
	bool isRefresh = false;

	if (_currentCaptureData._selectedStat.Num() != InSelectedStatSet.Num())
	{
		isRefresh = true;
	}
	else
	{
		for (const StatType item : InSelectedStatSet)
		{
			if (false == _currentCaptureData._selectedStat.Contains(item))
			{
				isRefresh = true;
				break;
			}
		}
	}
		
	// 2023/1/4 PKT - 닫히면 첫 탭 으로 이동... 기획 의도..
	if (isRefresh)
	{	// 2024/1/17 PKT - 저장
		_currentCaptureData._selectedStat = InSelectedStatSet;
		
		InvalidateFieldSet();
	}	
}

void UGsUIWindowItemCollection::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (ETextCommit::OnEnter != InCommitMethod)
	{
		return;
	}

	FString trimText = _textSearch->GetText().ToString();
	// 2021/10/05 PKT - 공백 제거
	trimText.ReplaceInline(TEXT(" "), TEXT(""));
	// 2021/10/08 PKT - 공백 제거 이후 아무런 검색어가 없다면
	if (trimText.IsEmpty())
	{
		FText textMessageTicker;
		FText::FindText(TEXT("CollectionText"), TEXT("Input_Needed"), textMessageTicker);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);

		return;
	}
	else
	{
		_textFilterWord = trimText;
	}

	// 2023/1/5 PKT - 검색어에 맞게 다시 리스트를 만들어 준다.
	InvalidateFieldSet();
}

void UGsUIWindowItemCollection::OnTextChangedMsg(const FText& InText)
{
	static const int32 maxLength = 50;

	FString changedString;
	bool isTextChanged = FGsUIStringHelper::GetFormatedEditableText(InText.ToString(), changedString, maxLength, false, false, true);
	if (isTextChanged)
	{
		_textSearch->SetText(FText::FromString(changedString));
	}
}

void UGsUIWindowItemCollection::OnClickedClearSearchWord()
{
	_textSearch->SetText(FText::GetEmpty());

	if (true == _textFilterWord.IsEmpty())
	{
		// 2023/1/5 PKT - 무언가 검색을 이전에 한것이 없다면 새롭게 리스트를 갱신 할 필요가 없다.
		return;
	}

	_textFilterWord.Empty();

	// 2023/1/5 PKT - 검색어를 지웠으므로 리스트를 다시 갱신 한다.
	InvalidateFieldSet();
}

void UGsUIWindowItemCollection::OnScrollListView(float InOffsetInItems, float InDistanceRemaining)
{	
	_currentCaptureData._scrollValue = InOffsetInItems;

	InvalidatePage(StaticCast<int32>(_currentCaptureData._scrollValue), _listViewCollectionItems->GetNumItems());
}

void UGsUIWindowItemCollection::OnClickedListViewItem(UObject* InListItem)
{
	UGsItemCollectionViewEntryData* entryData = Cast<UGsItemCollectionViewEntryData>(InListItem);
	if (nullptr == entryData && entryData->GetData())
	{
		return;
	}

	SelectedField(entryData->GetData()->Id());

	// 2023/7/6 PKT - https://jira.com2us.com/jira/browse/CHR-20917
	if (GUI())
	{
		GUI()->PlayPressSound();
	}
}

// 2023/1/5 PKT - 이전 페이지
void UGsUIWindowItemCollection::OnClickedShiftPrevPage()
{
	const static int32 SHIFT_MAX_NUM = 10;
	int32 nextOffset = FMath::Max(0, int32(_currentCaptureData._scrollValue) - SHIFT_MAX_NUM);
	if (nextOffset == _currentCaptureData._scrollValue)
	{
		return;
	}
	_listViewCollectionItems->SetScrollOffset(StaticCast<float>(nextOffset));
}

void UGsUIWindowItemCollection::OnClickedShiftNextPage()
{
	const static int32 SHIFT_MAX_NUM = 10;
	const int32 maxOffset = _listViewCollectionItems->GetNumItems();
	int32 nextOffset = FMath::Min(maxOffset - 1, int32(_currentCaptureData._scrollValue) + SHIFT_MAX_NUM);
	if (nextOffset == int32(_currentCaptureData._scrollValue))
	{
		return;
	}
	_listViewCollectionItems->SetScrollOffset(StaticCast<float>(nextOffset));
}

void UGsUIWindowItemCollection::OnChangedToogleListViewType(int32 InIndex)
{
	InvalidateFieldSet();
}

void UGsUIWindowItemCollection::OnChangedToogleMainTab(int32 InIndex)
{
	BuildSubCategory();

	_toggleGroupSubTabs.SetSelectedIndex(_currentCaptureData._subTabIndex, true);
}

void UGsUIWindowItemCollection::OnChangedToogleSubTab(int32 InIndex)
{
	InvalidateFieldSet();
}

void UGsUIWindowItemCollection::SelectedField(CollectionId InCollectionId, const int32 InSelectedTabIndex /* = -1 */)
{
	const int32 maxCount = _listViewCollectionItems->GetNumItems();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsItemCollectionViewEntryData> item = Cast<UGsItemCollectionViewEntryData>(_listViewCollectionItems->GetItemAt(idx));
		if (false == item.IsValid() && nullptr == item->GetData())
		{
			continue;
		}

		item->SetSelectedField(item->GetData()->Id() == InCollectionId);
		item->SetSelectedCondition(false);

		if (item->GetData()->Id() == InCollectionId)
		{
			const static int32 SWITCHER_FIELD_DETAIL_INDEX = 1;
			_switcherDetailPanel->SetActiveWidgetIndex(SWITCHER_FIELD_DETAIL_INDEX);
			TWeakObjectPtr<UGsUIMaterialSetGainInfo> detailPanel = Cast<UGsUIMaterialSetGainInfo>(_switcherDetailPanel->GetActiveWidget());
			if (detailPanel.IsValid())
			{
				detailPanel->SetData(item->GetData(), InSelectedTabIndex);
			}
		}
	}

	_currentCaptureData._selectedFieldId = InCollectionId;
	_currentCaptureData._selectedConditionId = INVALID_COLLECTION_CONDITION_ID;

	// 2023/1/10 PKT - 리스트 뷰 갱신
	InvalidateViewItems();
}

void UGsUIWindowItemCollection::SelectedCondition(CollectionId InCollectionId, CollectionConditionId InCollectionConditionId, const int32 InSelectedTabIndex /* = -1 */)
{
	const int32 maxCount = _listViewCollectionItems->GetNumItems();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsItemCollectionViewEntryData> item = Cast<UGsItemCollectionViewEntryData>(_listViewCollectionItems->GetItemAt(idx));
		if (false == item.IsValid() && nullptr == item->GetData())
		{
			continue;
		}

		item->SetSelectedField(false);
		item->SetSelectedCondition(InCollectionConditionId);

		if (item->GetData()->Id() == InCollectionId)
		{
			for (const auto& condition : item->GetData()->Conditions())
			{
				if (InCollectionConditionId == condition.Id())
				{
					const static int32 SWITCHER_CONDITION_DETAIL_INDEX = 2;
					_switcherDetailPanel->SetActiveWidgetIndex(SWITCHER_CONDITION_DETAIL_INDEX);

					TWeakObjectPtr<UGsUIMaterialGainInfo> detailPanel = Cast<UGsUIMaterialGainInfo>(_switcherDetailPanel->GetActiveWidget());
					if (detailPanel.IsValid())
					{
						detailPanel->SetData(&condition, InSelectedTabIndex);
					}
				}
			}
		}
	}

	_currentCaptureData._selectedFieldId = InCollectionId;
	_currentCaptureData._selectedConditionId = InCollectionConditionId;

	// 2023/1/10 PKT - 리스트 뷰 갱신
	InvalidateViewItems();
}

void UGsUIWindowItemCollection::OnReceiveItemPriceInfo(const IGsMessageParam*)
{
	UWidget* activeWidget = _switcherDetailPanel->GetActiveWidget();
	if (nullptr == activeWidget)
	{
		return;
	}

	if (activeWidget->IsA<UGsUIMaterialGainInfo>())
	{
		if (UGsUIMaterialGainInfo* activePage = Cast<UGsUIMaterialGainInfo>(activeWidget))
		{
			activePage->OnReceiveItemPriceInfo(nullptr);
		}
	}
}

void UGsUIWindowItemCollection::OnReceiveCollectionPriceList(const IGsMessageParam* InParam)
{
	UWidget* activeWidget = _switcherDetailPanel->GetActiveWidget();
	if (nullptr == activeWidget)
	{
		return;
	}

	if (activeWidget->IsA<UGsUIMaterialGainInfo>())
	{
		if (UGsUIMaterialGainInfo* activePage = Cast<UGsUIMaterialGainInfo>(activeWidget))
		{
			activePage->OnReceiveCollectionPriceList(InParam);
		}
	}
	else if (activeWidget->IsA<UGsUIMaterialSetGainInfo>())
	{
		if (UGsUIMaterialSetGainInfo* activePage = Cast<UGsUIMaterialSetGainInfo>(activeWidget))
		{
			activePage->OnReceiveCollectionPriceList(InParam);
		}
	}
}

void UGsUIWindowItemCollection::OnRequestRefreshPrice(const IGsMessageParam*)
{
	UWidget* activeWidget = _switcherDetailPanel->GetActiveWidget();
	if (nullptr == activeWidget)
	{
		return;
	}

	if (activeWidget->IsA<UGsUIMaterialGainInfo>())
	{
		if (UGsUIMaterialGainInfo* activePage = Cast<UGsUIMaterialGainInfo>(activeWidget))
		{
			activePage->OnRequestRefreshPrice(nullptr);
		}
	}
	else if (activeWidget->IsA<UGsUIMaterialSetGainInfo>())
	{
		if (UGsUIMaterialSetGainInfo* activePage = Cast<UGsUIMaterialSetGainInfo>(activeWidget))
		{
			activePage->OnRequestRefreshPrice(nullptr);
		}
	}
}
