#include "GSUIMonsterCollectionSlatePage.h"
#include "GsUIMonsterCollectionSlate.h"
#include "GsUIMonsterCollectionSlateInfo.h"
#include "GsUIMonsterCollectionSlateInfoAll.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeUICaptureData.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIContent/Common/GsUITabButtonCommon.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../UI/UIContent/Tray/GsUITrayCommonStatFilterInCategory.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlateCategory.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"

#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../StatFilterSystem/GsStatFilterHandler.h"

#include "T1Project.h"

void UGSUIMonsterCollectionSlatePage::BeginDestroy()
{
	if (_btnFilter)
	{
		_btnFilter->OnClicked.RemoveDynamic(this, &UGSUIMonsterCollectionSlatePage::OnClickedOpenFilter);
	}
	_btnFilter = nullptr;

	if (_slotHelperCategory)
	{
		_slotHelperCategory->OnRefreshEntry.RemoveDynamic(this, &UGSUIMonsterCollectionSlatePage::OnRefreshCategoryTab);		
	}
	_slotHelperCategory = nullptr;

	if (_slotHelperSlateSet)
	{
		_slotHelperSlateSet->OnRefreshEntry.RemoveDynamic(this, &UGSUIMonsterCollectionSlatePage::OnRefreshSlateSet);
	}
	_slotHelperSlateSet = nullptr;

	Super::BeginDestroy();
}

void UGSUIMonsterCollectionSlatePage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_toggleGroupCategoryTab.Clear();
	_toggleGroupCategoryTab.OnSelectChanged.BindUObject(this, &UGSUIMonsterCollectionSlatePage::OnSelectedCategoryTab);

	_toggleGroupSlateSet.Clear();
	_toggleGroupSlateSet.OnSelectChanged.BindUObject(this, &UGSUIMonsterCollectionSlatePage::OnSelectedSlate);

	_slotHelperCategory = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperCategory->Initialize(_subClassOfCategoryEntry, _scrollBoxCategory);
	_slotHelperCategory->OnRefreshEntry.AddDynamic(this, &UGSUIMonsterCollectionSlatePage::OnRefreshCategoryTab);

	_slotHelperSlateSet = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSlateSet->Initialize(_subClassOfSlateEntry, _scrollBoxSlateSet);
	_slotHelperSlateSet->OnRefreshEntry.AddDynamic(this, &UGSUIMonsterCollectionSlatePage::OnRefreshSlateSet);

	_btnFilter->OnClicked.AddDynamic(this, &UGSUIMonsterCollectionSlatePage::OnClickedOpenFilter);
}

void UGSUIMonsterCollectionSlatePage::NativeConstruct()
{
	Super::NativeConstruct();

	_reservedId = 0;
	_prevSelectedSlateIndex = 0;
	_prevScrollValue = 0.f;
	_prevStatFilterSet.Empty();
	_categorySet.Empty();
	_toggleGroupCategoryTab.Clear();
	
	// 2023/2/21 PKT - 전체 카테고리를 포함 하기 위해 +1를 한다.
	const int32 maxCount = GsMonsterCollection()->BuildSlateCategory() + 1;
	_slotHelperCategory->RefreshAll(maxCount);
}

void UGSUIMonsterCollectionSlatePage::NativeDestruct()
{
	_slotHelperCategory->AllClearChildren();
	_slotHelperSlateSet->AllClearChildren();

	Super::NativeDestruct();
}

void UGSUIMonsterCollectionSlatePage::Enter(FGsMonsterKnowledgeUICaptureData* InData)
{
	Super::Enter(InData);

	_reservedId = InData->_reservedId;
	if (0 < _reservedId)
	{
		for (int32 idx = 0; idx < GsMonsterCollection()->SlateMaxCount(); ++idx)
		{
			const FGsMonsterKnowledgeSlate* item = GsMonsterCollection()->SlateDataAt(idx);
			if (_reservedId == item->Id())
			{
				InData->_categoryTab = 0;
				for (int32 i=0; i < _categorySet.Num(); ++i)
				{
					if (_categorySet[i] == item->CategoryType())
					{
						InData->_categoryTab = i;
						break;
					}
				}
				break;
			}
		}
	}	

	_prevStatFilterSet = InData->_StatFilterSet;

	// 2023/3/17 PKT - Capture Data
	_prevSelectedSlateIndex = StaticCast<int32>(InData->_lastViewParam);
	_prevScrollValue = InData->_categoryScrollOffset;
	_prevSelectedNodeId = InData->_selectedNodeId;
	_prevCheckedNodeAll = InData->_isSelectedNodeAll;
	_prevSelectedSpecialNodeId = InData->_selectedSpecialNodeId;


	_toggleGroupCategoryTab.SetSelectedIndex(StaticCast<int32>(InData->_categoryTab), true);

	_reservedId = 0;
	_prevSelectedSlateIndex = 0;
	_prevScrollValue = 0.f;
	_prevSelectedNodeId = 0;
	_prevCheckedNodeAll = false;
	_prevSelectedSpecialNodeId = 0;

	InvalidateRedDot();
}

void UGSUIMonsterCollectionSlatePage::Leave()
{
	Super::Leave();
}

void UGSUIMonsterCollectionSlatePage::Capture(FGsMonsterKnowledgeUICaptureData* OutData)
{
	Super::Capture(OutData);

	OutData->_categoryTab = StaticCast<uint32>(_toggleGroupCategoryTab.GetSelectedIndex());
	OutData->_categoryScrollOffset = _scrollBoxSlateSet->GetScrollOffset();
	OutData->_lastViewParam = StaticCast<uint32>(_toggleGroupSlateSet.GetSelectedIndex());;
	OutData->_StatFilterSet = _prevStatFilterSet;
	OutData->_selectedNodeId = INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID;
	OutData->_isSelectedNodeAll = false;
	OutData->_selectedSpecialNodeId = INVALID_SLATE_SPECIAL_NODE_ID;

	const static int32 SWITCHER_SLATE_NODE = 1;
	if (SWITCHER_SLATE_NODE == _switcherDetailPanel->GetActiveWidgetIndex())
	{
		TWeakObjectPtr<UGsUIMonsterCollectionSlateInfo> item = Cast<UGsUIMonsterCollectionSlateInfo>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			OutData->_selectedNodeId = item.Get()->SelectedNodeId();
			OutData->_isSelectedNodeAll = item.Get()->IsCheckedSelectedNodeAll();
			OutData->_selectedSpecialNodeId = item.Get()->SelectedSpecialNodeId();
		}
	}
}

bool UGSUIMonsterCollectionSlatePage::CloseChildWidget()
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

	return Super::CloseChildWidget();
}

void UGSUIMonsterCollectionSlatePage::InvalidateSlate(MonsterKnowledgeSlateId InId)
{
	Super::InvalidateSlate(InId);

	TWeakObjectPtr<UWidget> activeWidget = _switcherDetailPanel->GetActiveWidget();
	if (false == activeWidget.IsValid())
	{
		GSLOG(Error, TEXT("false == activeWidget.IsValid()"));
		return;
	}

	const static int32 VIEW_ALL_INDEX = 0;
	const static int32 VIEW_NODE_INDEX = 1;
	const static int32 VIEW_EMPTY_INDEX = 2;

	const int32 activeIndex = _switcherDetailPanel->GetActiveWidgetIndex();

	if (VIEW_ALL_INDEX == activeIndex)
	{
		TWeakObjectPtr<UGsUIMonsterCollectionSlateInfoAll> item = Cast<UGsUIMonsterCollectionSlateInfoAll>(activeWidget);
		if (item.IsValid())
		{
			item->Invalidate();
		}
	}
	else if (VIEW_NODE_INDEX == activeIndex)
	{
		TWeakObjectPtr<UGsUIMonsterCollectionSlateInfo> item = Cast<UGsUIMonsterCollectionSlateInfo>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid() && InId == item->GetData()->Id())
		{
			item->Invalidate();
		}
	}
	else if (VIEW_EMPTY_INDEX == activeIndex)
	{
		// None
	}
}

void UGSUIMonsterCollectionSlatePage::InvalidateSlateCurrency()
{
	// 2023/2/21 PKT - 전체 Tab이 아닌 경우에만 갱신 한다.
	const int32 currentTab = _toggleGroupCategoryTab.GetSelectedIndex();
	if (_categorySet.IsValidIndex(currentTab) && EMonsterKnowledgeSlateCategory::MAX != _categorySet[currentTab])
	{
		TWeakObjectPtr<UGsUIMonsterCollectionSlateInfo> item = Cast<UGsUIMonsterCollectionSlateInfo>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			item->InvalidateCurrency();
		}
	}
}

void UGSUIMonsterCollectionSlatePage::InvalidateRedDot()
{
	Super::InvalidateRedDot();

	int32 maxCount = _slotHelperCategory->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUITabButtonCommon> tabButton = Cast<UGsUITabButtonCommon>(_slotHelperCategory->GetEntry(idx));
		if (false == tabButton.IsValid())
		{
			GSLOG(Error, TEXT("false == tabButton.IsValid()"));
			continue;
		}

		bool isRedDot = false;
		if (0 != idx)
		{	// 2023/4/24 PKT - 카테고리가 '전체'에서 '요약'으로 변경 됨에 따라 요약은 레드닷 켜지 않음.
			isRedDot = _categorySet.IsValidIndex(idx) ? GsMonsterCollection()->CheckRedDot(_categorySet[idx]) : false;
		}		
		tabButton->SetIsRedDot(EGsIconRedDotType::NORMAL, isRedDot);
	}

	maxCount = _slotHelperSlateSet->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUIMonsterCollectionSlate> slateEntry = Cast<UGsUIMonsterCollectionSlate>(_slotHelperSlateSet->GetEntry(idx));
		if (false == slateEntry.IsValid())
		{
			GSLOG(Error, TEXT("false == slateEntry.IsValid()"));
			continue;
		}
		slateEntry->Invalidate();
	}
}

void UGSUIMonsterCollectionSlatePage::BuildData(int32 InCategoryIndex)
{
	Super::BuildData(InCategoryIndex);

	EMonsterKnowledgeSlateCategory currentCategory 
		= _categorySet.IsValidIndex(InCategoryIndex) ? _categorySet[InCategoryIndex] : EMonsterKnowledgeSlateCategory::MAX;

	if (EMonsterKnowledgeSlateCategory::MAX == currentCategory)
	{
		_panelSlateSet->SetVisibility(ESlateVisibility::Collapsed);

		const static int32 EMPTY_INDEX = 0;
		_slotHelperSlateSet->RefreshAll(EMPTY_INDEX);

		const static int32 SLATE_ALL_VIEW_INDEX = 0;
		_switcherDetailPanel->SetActiveWidgetIndex(SLATE_ALL_VIEW_INDEX);

		TWeakObjectPtr<UGsUIMonsterCollectionSlateInfoAll> item = Cast<UGsUIMonsterCollectionSlateInfoAll>(_switcherDetailPanel->GetActiveWidget());
		if (item.IsValid())
		{
			item->BuildView();
		}
	}
	else
	{
		_panelSlateSet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// 2023/2/20 PKT - Build Slate Set
		const int32 maxCount = GsMonsterCollection()->BuildSlateViewSet(currentCategory, _prevStatFilterSet);

		const static int32 SLATE_VIEW_INDEX = 1;
		const static int32 SLATE_EMPTY_INDEX = 2;

		int32 selectedSwitcher = (0 >= maxCount) ? SLATE_EMPTY_INDEX : SLATE_VIEW_INDEX;
		_switcherDetailPanel->SetActiveWidgetIndex(selectedSwitcher);

		_slotHelperSlateSet->RefreshAll(maxCount);

		if (SLATE_VIEW_INDEX == selectedSwitcher)
		{
			UWidget* focusWidget = nullptr;
			int32 selectedIndex = _prevSelectedSlateIndex;
			if (0 < _reservedId)
			{
				for (int32 idx = 0; idx < maxCount; ++idx)
				{
					if (false == GsMonsterCollection()->SlateViewSet().IsValidIndex(idx))
					{
						continue;
					}

					if (_reservedId == GsMonsterCollection()->SlateViewSet()[idx]->Id())
					{
						selectedIndex = idx;
						focusWidget = _slotHelperSlateSet->GetEntry(selectedIndex);
						break;
					}
				}
			}

			_toggleGroupSlateSet.Clear();
			_toggleGroupSlateSet.AddToggleWidgetByParentPanel(_scrollBoxSlateSet);
			_toggleGroupSlateSet.SetSelectedIndex(selectedIndex, true);

			// 2023/2/20 PKT - 미리 선택되어진 Data가 있으면 Focus를 맞춰 준다.
			if (nullptr == focusWidget)
			{
				_scrollBoxSlateSet->SetScrollOffset(_prevScrollValue);
			}
			else
			{
				_scrollBoxSlateSet->ScrollWidgetIntoView(focusWidget, false, EDescendantScrollDestination::IntoView);
			}
		}
	}
}

void UGSUIMonsterCollectionSlatePage::OnSelectedCategoryTab(int32 InIndex)
{
	BuildData(InIndex);
}

void UGSUIMonsterCollectionSlatePage::OnSelectedSlate(int32 InIndex)
{
	if (false == GsMonsterCollection()->SlateViewSet().IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("Invalid Index : [%d]"), InIndex);
		return;
	}

	TWeakObjectPtr<UGsUIMonsterCollectionSlateInfo> item = Cast<UGsUIMonsterCollectionSlateInfo>(_switcherDetailPanel->GetActiveWidget());
	if (item.IsValid())
	{
		UGsUIMonsterCollectionSlateInfo::FGsCaptureSmallOption option;
		option._isSelectedNodeId = StaticCast<MonsterKnowledgeSlateSlotId>(_prevSelectedNodeId);
		option._isCheckNodeAll = _prevCheckedNodeAll;
		option._isSelectedSpecialNodeId = StaticCast<SlateSpecialNodeId>(_prevSelectedSpecialNodeId);

		item->SetData(GsMonsterCollection()->SlateViewSet()[InIndex], option);
	}
}

void UGSUIMonsterCollectionSlatePage::OnRefreshCategoryTab(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITabButtonCommon> tabButton = Cast<UGsUITabButtonCommon>(InEntry);
	if (false == tabButton.IsValid())
	{
		GSLOG(Error, TEXT("false == tabButton.IsValid()"));
		return;
	}

	FText categoryName;
	EMonsterKnowledgeSlateCategory currentCategory = EMonsterKnowledgeSlateCategory::MAX;
	if (0 >= InIndex)
	{
		// 2023/2/21 PKT - 요약 카테고리
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("UI_Slate_Category_Name_Summary"), categoryName);
		currentCategory = EMonsterKnowledgeSlateCategory::MAX;
	}
	else
	{
		int32 categoryIndex = InIndex - 1;
		if (GsMonsterCollection()->SlateCategoryData(categoryIndex))
		{
			currentCategory = GsMonsterCollection()->SlateCategoryData(categoryIndex)->Key;
			categoryName = GsMonsterCollection()->SlateCategoryData(categoryIndex)->Value;
		}		
	}

	tabButton->SetTitleText(categoryName);
	tabButton->SetIsRedDot(EGsIconRedDotType::NORMAL, false);

	_categorySet.Emplace(currentCategory);
	_toggleGroupCategoryTab.AddToggleWidget(tabButton->GetToggleGroupEntry());
}

void UGSUIMonsterCollectionSlatePage::OnRefreshSlateSet(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIMonsterCollectionSlate> item = Cast<UGsUIMonsterCollectionSlate>(InEntry);
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("false == item.IsValid()"));
		return;
	}

	if (false == GsMonsterCollection()->SlateViewSet().IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == GsMonsterCollection()->SlateViewSet().IsValidIndex(InIndex)"));
		return;
	}

	item->SetData(GsMonsterCollection()->SlateViewSet()[InIndex]);
}

void UGSUIMonsterCollectionSlatePage::OnClickedOpenFilter()
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
		statfilterHandler->OpenWidget(GsMonsterCollection()->ContentsStatAll().Array(), _prevStatFilterSet, [this](const TArray<StatType>& InData)
			{
				this->OnCallbackStatFilterApply(InData);
			}
		);
	}
}

void UGSUIMonsterCollectionSlatePage::OnCallbackStatFilterApply(const TArray<StatType>& InSelectedStatSet)
{
	_prevStatFilterSet = InSelectedStatSet;

	BuildData(_toggleGroupCategoryTab.GetSelectedIndex());
}