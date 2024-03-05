
#include "GsUIWindowTitleSystem.h"
#include "Title/GsTitleFilter.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Classes/GsPoolUObject.h"

#include "Title/GsTitleCollectionReward.h"
#include "Title/GsTitleEquip.h"
#include "Title/GsTitleHelper.h"
//#include "Title/GsUITitleBattleLevelEntry.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsTitleManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
//#include "Management/ScopeGlobal/GsLevelManager.h"

#include "UI/UIContent/Window/Title/GsUITitleListViewEntry.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIContent/Window/Title/GsUITitleEquipSlotEffect.h"
#include "UI/UIContent/Popup/Title/GsUIPopupTitleCollectionData.h"
#include "UI/UIContent/Tray/GsUITrayCommonStatFilterInCategory.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"

#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/Widget.h"
#include "Components/ListView.h"
#include "Components/SizeBox.h"

#include "RedDot/GsRedDotCheck.h"

#include "DataSchema/Title/GsSchemaTitleSubCategory.h"
//#include "DataSchema/InterServer/GsSchemaInvadeBattleLevel.h"
#include "DataSchema/Map/GsSchemaMapData.h"



void UGsUIWindowTitleSystem::NativeOnInitialized()
{
	UGsUIWindow::NativeOnInitialized();

	// 상단 버튼
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowTitleSystem::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowTitleSystem::OnClickCloseAllStack);

	_btnStatFilter->OnClicked.AddDynamic(this, &UGsUIWindowTitleSystem::OnClickedStatFilter);

	_btnCollection->OnClicked.AddDynamic(this, &UGsUIWindowTitleSystem::OnClickedOpenCollection);

	_slotHelperSubBtnTab = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSubBtnTab->Initialize(_subClassOfSubBtnTabEntry, _scrollBoxSubCategory);
	_slotHelperSubBtnTab->OnRefreshEntry.AddDynamic(this, &UGsUIWindowTitleSystem::OnRefreshEntrySubBtnTab);

	// 2022/01/19 PKT - Main Category
	_toggleGroupMainTabs.AddToggleWidget(_switcherBtnFront);
	_toggleGroupMainTabs.AddToggleWidget(_switcherBtnBack);
	_toggleGroupMainTabs.OnSelectChanged.BindUObject(this, &UGsUIWindowTitleSystem::OnSelectedBtnMainCategory);

	FText textTabName;
	FGsTitleHelper::FindText(TEXT("UI_Main_Tab_Name_Front"), textTabName);
	_textMainCategoryFrontOff->SetText(textTabName);
	_textMainCategoryFrontOn->SetText(textTabName);

	FGsTitleHelper::FindText(TEXT("UI_Main_Tab_Name_Back"), textTabName);
	_textMainCategoryBackOff->SetText(textTabName);
	_textMainCategoryBackOn->SetText(textTabName);

	// 2022/01/19 PKT - Sub Category
	_toggleGroupSubTabs.OnSelectChanged.BindUObject(this, &UGsUIWindowTitleSystem::OnSelectedBtnSubCategory);

	_toggleGroupViewTypeBtns.OnSelectChanged.BindUObject(this, &UGsUIWindowTitleSystem::OnSelectedBtnViewType);

	const static int32 POOL_WARM_UP_SIZE = 30;
	_poolTitleData = NewObject<UGsPoolUObject>(this); // UGsMarketItemGroupListItem
	_poolTitleData->WarmUp<UGsTitleListViewEntryData>(POOL_WARM_UP_SIZE);
}

void UGsUIWindowTitleSystem::BeginDestroy()
{
	// 2021/11/23 PKT - Event UnBind
	if (_btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUIWindowTitleSystem::OnClickBack);
		_btnBack = nullptr;
	}	

	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIWindowTitleSystem::OnClickCloseAllStack);
		_btnClose = nullptr;
	}	

	if (_btnStatFilter)
	{
		_btnStatFilter->OnClicked.RemoveDynamic(this, &UGsUIWindowTitleSystem::OnClickedStatFilter);
		_btnStatFilter = nullptr;
	}

	if (_btnCollection)
	{
		_btnCollection->OnClicked.RemoveDynamic(this, &UGsUIWindowTitleSystem::OnClickedOpenCollection);
		_btnCollection = nullptr;
	}

	// 2022/01/20 PKT - Toggle Clear
	_toggleGroupMainTabs.Clear();
	_toggleGroupSubTabs.Clear();
	_toggleGroupViewTypeBtns.Clear();

	if (_poolTitleData)
	{
		_poolTitleData->RemovePool();
		_poolTitleData = nullptr;
	}	

	UGsUIWindow::BeginDestroy();
}

void UGsUIWindowTitleSystem::NativeConstruct()
{
	UGsUIWindow::NativeConstruct();

	if (false == IsStackRecoverFlag())
	{
		_lastMainTab = ETitleMainCategory::FORNT;
		_lastSubCategory = DEFAULT_SUB_CATEGORY_INDEX;
		_lastViewTypeBtn = DEFAULT_VIEWTYPE_INDEX;

		if (FGsTitleManager* titleManager = GTitle())
		{
			const TArray<StatType>& StatList = titleManager->GetToHaveStatAll();

			_statFilterSet.Empty(StatList.Num());
			for (StatType type : StatList)
			{
				_statFilterSet.Emplace(TPair<StatType, bool>(type, false));
			}
		}
	}

	// 2022/01/19 PKT - View Type
	_toggleGroupViewTypeBtns.Clear();
	_toggleGroupViewTypeBtns.AddToggleWidget(_btnViewTypeAll);
	_toggleGroupViewTypeBtns.AddToggleWidget(_btnViewTypeAcquire);
	_toggleGroupViewTypeBtns.AddToggleWidget(_btnViewTypeDotHave);

	// 2022/08/25 PKT - clear	
	_prevMainCategory = ETitleMainCategory::MAX;
	_prevSubCategory = INVALID_CATEGORY_INDEX;

	// 2022/01/19 PKT - Selected Main Category
	_toggleGroupMainTabs.SetSelectedIndex(StaticCast<int32>(_lastMainTab), true);

	// 2022/01/28 PKT - 서브 카테고리 레드닷
	RefreshCategoryRedDot();

	// 2022/01/21 PKT - 보유 현황
	RefreshCollection();

	// 2022/01/24 PKT - 장착 정보
	RefreshEquip(false);
}

void UGsUIWindowTitleSystem::NativeDestruct()
{
	UGsUIWindow::NativeDestruct();

	CheckOutTitleList(_prevMainCategory, _prevSubCategory);

	// 2022/01/20 PKT - saved last main tab index
	_lastMainTab = StaticCast<ETitleMainCategory>(_toggleGroupMainTabs.GetSelectedIndex());
	
	// 2022/01/20 PKT - saved last sub tab index
	_lastSubCategory = _toggleGroupSubTabs.GetSelectedIndex();
	
	// 2022/01/20 PKT - saved last view type index
	_lastViewTypeBtn = _toggleGroupViewTypeBtns.GetSelectedIndex();
}

bool UGsUIWindowTitleSystem::OnBack()
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

bool UGsUIWindowTitleSystem::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsTitle))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_TITLE);
		return true;
	}

	return false;
}

void UGsUIWindowTitleSystem::MakeViewList()
{
	// 2022/08/25 PKT - Main Category Type
	ETitleMainCategory mainCategory = StaticCast<ETitleMainCategory>(_toggleGroupMainTabs.GetSelectedIndex());

	ChangedVisibleWidget(mainCategory);

	MakeTitleView();
}

void UGsUIWindowTitleSystem::MakeTitleView()
{
	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	// 2022/01/20 PKT - Main Category Type
	ETitleMainCategory mainCategory = StaticCast<ETitleMainCategory>(_toggleGroupMainTabs.GetSelectedIndex());

	const static int32 TITLE_SYMBOL_INDEX = 0;
	_switcherSymbol->SetActiveWidgetIndex(TITLE_SYMBOL_INDEX);

	// 2022/01/20 PKT - pool release
	_poolTitleData->ReleaseAll();

	// 2022/08/25 PKT - Slot Type
	TitleEquipSlot equipSlotType = FGsTitleHelper::ConvertCategoryToEquipType(mainCategory);
	// 2022/01/20 PKT - Sub Category Index
	int32 subCategoryIndex = _toggleGroupSubTabs.GetSelectedIndex();
	// 2022/01/20 PKT - View Type
	ETitleViewListType viewType = StaticCast<ETitleViewListType>(_toggleGroupViewTypeBtns.GetSelectedIndex());

	// 2022/01/20 PKT - Make Filters
	const TArray<const FGsTitleData*>& viewList = titleManager->MakeViewList(equipSlotType, subCategoryIndex, MakeViewListFilters());

	// 2022/01/04 PKT - List를 한번에 셋팅
	TArray<UGsTitleListViewEntryData*> entryList;
	for (auto& titleData : viewList)
	{
		UGsTitleListViewEntryData * entry = _poolTitleData->Claim<UGsTitleListViewEntryData>();
		entry->_titleData = titleData;
		if (false == entry->OnClickedTitleEquipChanged.IsBoundToObject(this))
		{
			entry->OnClickedTitleEquipChanged.BindUObject(this, &UGsUIWindowTitleSystem::OnTitleStateChagned);
		}

		// 2022/01/20 PKT - Add..
		entryList.Emplace(entry);
	}

	const static int32 EMPTY_LIST_COUNT = 0;
	if (EMPTY_LIST_COUNT >= entryList.Num())
	{
		const static int32 EMPTY_SWITHCER_INDEX = 1;
		_switcherTitleList->SetActiveWidgetIndex(EMPTY_SWITHCER_INDEX);
	}
	else
	{
		const static int32 NOT_EMPTY_SWITHCER_INDEX = 0;
		_switcherTitleList->SetActiveWidgetIndex(NOT_EMPTY_SWITHCER_INDEX);

		// 2022/01/04 PKT - List 초기화
		_listViewTitle->SetListItems(entryList);
		_listViewTitle->RegenerateAllEntries();
		_listViewTitle->ScrollToTop();
		_listViewTitle->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIWindowTitleSystem::ChangedVisibleWidget(ETitleMainCategory InMainCategory)
{
	/*_scrollBoxSubCategory->SetVisibility(ESlateVisibility::Visible);
	_sizeBoxFilterGroup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);*/
}

TArray<TSharedPtr<IGsTitleFilterConditionBase>> UGsUIWindowTitleSystem::MakeViewListFilters()
{
	TArray<TSharedPtr<IGsTitleFilterConditionBase>> outFilters;

	/**
	 * Make Filters
	 */
	// 2021/07/19 PKT - View Type
	TSharedPtr<FGsTitleViewTypeFilter> viewTypefilter = MakeShared<FGsTitleViewTypeFilter>();
	if (viewTypefilter.IsValid())
	{
		viewTypefilter->SetFilter(StaticCast<ETitleViewListType>(_toggleGroupViewTypeBtns.GetSelectedIndex()));
		outFilters.Emplace(viewTypefilter);
	}


	TArray<StatType> statFilter;
	for (const TPair<StatType, bool>& pair : _statFilterSet)
	{
		if (pair.Value)
		{
			statFilter.Emplace(pair.Key);
		}
	}

	if (0 < statFilter.Num())
	{
		TSharedPtr<FGsTitleStatFilter> statfilter = MakeShared<FGsTitleStatFilter>();
		statfilter->SetFilter(statFilter);
		outFilters.Emplace(statfilter);
	}
	
	return outFilters;
}

void UGsUIWindowTitleSystem::CheckOutTitleList(ETitleMainCategory InPrevMainCategory, int32 InPrevSubCategory)
{
	if (InPrevMainCategory != ETitleMainCategory::FORNT && InPrevMainCategory != ETitleMainCategory::BACK)
	{
		return;
	}

	if (InPrevSubCategory == INVALID_CATEGORY_INDEX)
	{
		return;
	}

	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	TitleEquipSlot equipSlotIndex = FGsTitleHelper::ConvertCategoryToEquipType(InPrevMainCategory);
	titleManager->TitleConfirm(equipSlotIndex, InPrevSubCategory);
}

void UGsUIWindowTitleSystem::RefreshCollection()
{
	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	const FGsTitleCollectionReward* collectionReward = titleManager->GetCollectionRewardData();
	if (nullptr == collectionReward)
	{
		GSLOG(Error, TEXT("nullptr == collectionReward"));
		return;
	}

	const int32 currentAcquire = collectionReward->GetAcquire();
	const int32 maxAcquire = collectionReward->GetMaxAcquire();

	FString textRetention = FString::Format(TEXT("{0}/{1}"), { FString::FormatAsNumber(currentAcquire), FString::FormatAsNumber(maxAcquire) });
	
	// 2022/01/21 PKT - 보유 현황
	_textRetentionStatusCount->SetText(	FText::FromString(textRetention));

	_reddotCollection->SetRedDot(collectionReward->IsRedDot());
}

void UGsUIWindowTitleSystem::RefreshTitleViewList(TitleId InTitleId)
{
	// 2022/01/21 PKT - 현재 보여 주고 있는 것만 갱신 한다.
	const TArray<UUserWidget*>& displayedWidgetList = _listViewTitle->GetDisplayedEntryWidgets();
	for (UUserWidget* displayedWidget : displayedWidgetList)
	{
		TWeakObjectPtr<UGsUITitleListViewEntry> entry = Cast<UGsUITitleListViewEntry>(displayedWidget);
		if (false == entry.IsValid())
		{
			continue;
		}

		const FGsTitleData* titleData = entry->GetData();
		if (titleData && titleData->GetId() == InTitleId)
		{	// 2022/01/21 PKT - 갱신
			entry->Invalidate();
		}
	}
}

void UGsUIWindowTitleSystem::RefreshCategoryRedDot()
{
	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	// 2022/01/24 PKT - front Category RedDot
	_reddotFrontRedDot->SetRedDot(titleManager->IsMainCategoryRedDot(TitleEquipSlot::FRONT));

	// 2022/01/24 PKT - back Category RedDot
	_reddotBackRedDot->SetRedDot(titleManager->IsMainCategoryRedDot(TitleEquipSlot::BACK));

	ETitleMainCategory mainCategory = StaticCast<ETitleMainCategory>(_toggleGroupMainTabs.GetSelectedIndex());

	for (int32 idx = 0; idx < _slotHelperSubBtnTab->GetItemCount(); ++idx)
	{
		bool isRedDot = false;

		TWeakObjectPtr<UGsUITabButtonCommon> tabButton = Cast<UGsUITabButtonCommon>(_slotHelperSubBtnTab->GetEntry(idx));
		if (false == tabButton.IsValid())
		{
			GSLOG(Warning, TEXT("false == tabButton.IsValid()"));
			continue;
		}

		const static int32 ALL_CATEGORY_INDEX = 0;
		if (idx == ALL_CATEGORY_INDEX)
		{
			isRedDot = (ETitleMainCategory::FORNT == mainCategory)
				? titleManager->IsMainCategoryRedDot(TitleEquipSlot::FRONT) : titleManager->IsMainCategoryRedDot(TitleEquipSlot::BACK);
		}
		else
		{
			TitleEquipSlot equipSlotType = FGsTitleHelper::ConvertCategoryToEquipType(mainCategory);
			isRedDot = titleManager->IsSubCategoryRedDot(equipSlotType, idx);
		}
		
		tabButton->SetIsRedDot(EGsIconRedDotType::NORMAL, isRedDot);
	}
}

void UGsUIWindowTitleSystem::RefreshEquip(bool InIsShowEffect)
{
	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	const FGsTitleEquip* equipData = titleManager->GetTitleEquipData();
	if (nullptr == equipData)
	{
		GSLOG(Error, TEXT("nullptr == equipData"));
		return;
	}

	_equipSlotEffect->SetData(equipData->GetData(TitleEquipSlot::FRONT), equipData->GetData(TitleEquipSlot::BACK), InIsShowEffect);
}

// 2022/01/19 PKT - Clicked Main Category
void UGsUIWindowTitleSystem::OnSelectedBtnMainCategory(int32 InIndex)
{
	// 2022/01/20 PKT - 서브 카테고리 Toggle 초기화
	_toggleGroupSubTabs.Clear();

	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	_equipSlotEffect->SetToggle(TitleEquipSlot(InIndex));

	const int32 maxCount = titleManager->GetCategoryCount(StaticCast<TitleEquipSlot>(InIndex));
	_slotHelperSubBtnTab->RefreshAll(maxCount);
	if (0 >= maxCount)
	{
		GSLOG(Error, TEXT("0 >= maxCount"));
		return;
	}
	
	// 2022/03/17 PKT - Scroll To Start
	_scrollBoxSubCategory->EndInertialScrolling();
	_scrollBoxSubCategory->ScrollToStart();

	int32 selectedSubCategorynIndex = _toggleGroupSubTabs.GetSelectedIndex();
	if (DEFAULT_VIEWTYPE_INDEX != _lastSubCategory)
	{
		selectedSubCategorynIndex = _lastSubCategory;
		_lastSubCategory = DEFAULT_SUB_CATEGORY_INDEX;
	}

	_toggleGroupSubTabs.SetSelectedIndex(selectedSubCategorynIndex, true);

	RefreshCategoryRedDot();
}

// 2022/01/19 PKT - Clicked Sub Category
void UGsUIWindowTitleSystem::OnSelectedBtnSubCategory(int32 InIndex)
{
	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	int32 selectedBtnIndex = _toggleGroupViewTypeBtns.GetSelectedIndex();
	if (DEFAULT_VIEWTYPE_INDEX != _lastViewTypeBtn)
	{
		selectedBtnIndex = _lastViewTypeBtn;
		_lastViewTypeBtn = DEFAULT_VIEWTYPE_INDEX;
	}

	// 2022/01/24 PKT - 이전 Tab에 있는 카테고리 레드닷 정보 초기화
	CheckOutTitleList(_prevMainCategory, _prevSubCategory);

	_prevMainCategory = StaticCast<ETitleMainCategory>(_toggleGroupMainTabs.GetSelectedIndex());
	_prevSubCategory = _toggleGroupSubTabs.GetSelectedIndex();

	_toggleGroupViewTypeBtns.SetSelectedIndex(selectedBtnIndex, true);	
}

// 2022/01/19 PKT - Clicked View Type
void UGsUIWindowTitleSystem::OnSelectedBtnViewType(int32 InIndex)
{
	MakeViewList();
}

void UGsUIWindowTitleSystem::OnRefreshEntrySubBtnTab(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITabButtonCommon> tabButton = Cast<UGsUITabButtonCommon>(InEntry);
	if (false == tabButton.IsValid())
	{
		GSLOG(Warning, TEXT("false == tabButton.IsValid()"));
		return;
	}

	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	ETitleMainCategory mainCategory = StaticCast<ETitleMainCategory>(_toggleGroupMainTabs.GetSelectedIndex());
	TitleEquipSlot equipSlotType = FGsTitleHelper::ConvertCategoryToEquipType(mainCategory);

	TArray<const FGsSchemaTitleSubCategory*> subCategoryList;
	if (titleManager->FindCategoryList(equipSlotType, subCategoryList))
	{
		tabButton->SetTitleText(subCategoryList[InIndex]->name);
	}

	// 2022/01/20 PKT - Add Toggle widgett
	_toggleGroupSubTabs.AddToggleWidget(tabButton->GetToggleGroupEntry());
}

void UGsUIWindowTitleSystem::OnTitleStateChagned(int32 InTitleId, bool IsEquip)
{
	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	const FGsTitleData* findData = titleManager->FindTitleDataInViewListByTitleDataId(InTitleId);
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData"));
		return;
	}

	bool currentEquip = findData->IsEquip();
	if (currentEquip == IsEquip)
	{	// 2022/01/20 PKT - 이미 착용 중이거나 이미 착용하지 않았거나!!
		GSLOG(Error, TEXT("currentEquip == IsEquip"));
		return;
	}

	// 2022/01/20 PKT - Send Packet
	TitleEquipSlot equipSlot = findData->GetEquipSlotType();
	TitleId titleId = findData->GetId();
	if (false == IsEquip)
	{	// 2022/01/24 PKT - 장착 해제 일 경우 TitleId를 0으로 해서 보낸다.
		titleId = INVALID_TITLE_ID;
	}
	FGsNetSendServiceWorld::Send_TitleEquip(titleId, equipSlot);	
}

void UGsUIWindowTitleSystem::OnClickedStatFilter()
{
	//FGsTitleManager* titleManager = GTitle();
	//if (nullptr == titleManager)
	//{
	//	GSLOG(Error, TEXT("nullptr == titleManager"));
	//	return;
	//}

	//titleManager->GetStatFilterCategorySet();

	//if (UGsUIManager* uiManager = GUI())
	//{
	//	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayCommonStatFilterInCateogry"));
	//	TWeakObjectPtr<UGsUITrayCommonStatFilterInCategory> trayFilter
	//		= (widget.IsValid()) ? Cast<UGsUITrayCommonStatFilterInCategory>(widget.Get()) : nullptr;
	//	if (true == trayFilter.IsValid())
	//	{
	//		const UGsUITrayCommonStatFilterInCategory::CategoryParamSet& paramSet = titleManager->GetStatFilterCategorySet();
	//		trayFilter->SetData(paramSet, _statFilterSet);
	//		/*if (false == trayFilter->OnApplyStatFilter.IsBoundToObject(this))
	//		{
	//			trayFilter->OnApplyStatFilter.BindUObject(this, &UGsUIWindowTitleSystem::OnApplyStatFilter);
	//		}*/
	//	}
	//}
}

void UGsUIWindowTitleSystem::OnClickedOpenCollection()
{
	// 2022/01/21 PKT - Collectino Detail Popup Open
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIPopupTitleCollectionData> weakPopup =
			Cast<UGsUIPopupTitleCollectionData>(uiManager->OpenAndGetWidget("PopupTitleCollectionData"));
		if (false == weakPopup.IsValid())
		{
			GSLOG(Error, TEXT("false == weakPopup.IsValid()"));
			return;
		}
	}
}

void UGsUIWindowTitleSystem::OnApplyStatFilter(const TArray<StatType>& InData)
{
	for (TPair<StatType, bool>& pair : _statFilterSet)
	{
		pair.Value = InData.Contains(pair.Key);
	}

	const int32 currentSelectedIndex = _toggleGroupViewTypeBtns.GetSelectedIndex();
	_toggleGroupViewTypeBtns.SetSelectedIndex(currentSelectedIndex, true);
}

void UGsUIWindowTitleSystem::InvalidCollection()
{
	// 2022/01/21 PKT - 보유 현황 업뎃..(Main Tab 상태와는 관련이 없음.)
	RefreshCollection();
}

void UGsUIWindowTitleSystem::InvalidTitle(TitleId InTitleId)
{
	// 2022/01/24 PKT - 칭호 리스트 업뎃..
	RefreshTitleViewList(InTitleId);

	// 2022/01/24 PKT - 카테고리 레드닷 업뎃..
	RefreshCategoryRedDot();
} 

void UGsUIWindowTitleSystem::InvalidEquip()
{	
	// 2022/01/26 PKT - 장착 정보 업뎃..
	RefreshEquip(true);
}

void UGsUIWindowTitleSystem::InvalidCategoryRedDot()
{
	// 2022/01/26 PKT - Category RedDot 업뎃
	RefreshCategoryRedDot();
}