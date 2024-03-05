

#include "GsUITrayCommonStatFilterInCategory.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../StatFilterSystem/GsStatFilterHandler.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Common/GsUICheckBoxCommon.h"
#include "UI/UIContent/Common/GsUICheckBoxCommonText.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "../Runtime/UMG/Public/Components/ScrollBox.h"
#include "../Runtime/UMG/Public/Components/PanelWidget.h"
#include "../Runtime/UMG/Public/Components/EditableTextBox.h"
#include "../Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "../Runtime/UMG/Public/Components/TextBlock.h"


void UGsUITrayCommonStatFilterInCategory::BeginDestroy()
{
	if (_btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnClickedBtnClose);
		_btnBack = nullptr;
	}

	if (_btnApply)
	{
		_btnApply->OnClicked.RemoveDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnClickedBtnApply );
		_btnApply = nullptr;
	}

	if (_btnReset)
	{
		_btnReset->OnClicked.RemoveDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnClickedBtnReset);
		_btnReset = nullptr;
	}

	if (_btnSerachTextReset)
	{
		_btnSerachTextReset->OnClicked.RemoveDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnClickedBtnSerachClear);
		_btnSerachTextReset = nullptr;
	}

	if (_textSearchStat)
	{
		_textSearchStat->OnTextCommitted.RemoveDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnTextCommitted);
		_textSearchStat->OnTextChanged.RemoveDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnTextChangedMsg);
	}	

	if (_slotHelperCategoryBtnTab)
	{
		_slotHelperCategoryBtnTab->OnRefreshEntry.RemoveDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnRefreshEntryTabButton);
		_slotHelperCategoryBtnTab = nullptr;
	}

	if (_slotHelperCheckBoxSet)
	{
		_slotHelperCheckBoxSet->OnCreateEntry.RemoveDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnCreateEntryCheckBox);
		_slotHelperCheckBoxSet->OnRefreshEntry.RemoveDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnRefreshEntryCheckBox);

		_slotHelperCheckBoxSet = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUITrayCommonStatFilterInCategory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperCategoryBtnTab = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperCategoryBtnTab->Initialize(_categoryTabButtonbEntry, _scrollBoxTabButton);
	_slotHelperCategoryBtnTab->OnRefreshEntry.AddDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnRefreshEntryTabButton);

	_slotHelperCheckBoxSet = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperCheckBoxSet->Initialize(_statUnitEntry, _panelCheckBoxStatSet);
	_slotHelperCheckBoxSet->OnCreateEntry.AddDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnCreateEntryCheckBox);
	_slotHelperCheckBoxSet->OnRefreshEntry.AddDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnRefreshEntryCheckBox);

	_btnBack->OnClicked.AddDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnClickedBtnClose);

	_btnApply->OnClicked.AddDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnClickedBtnApply);

	_btnReset->OnClicked.AddDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnClickedBtnReset);

	_btnSerachTextReset->OnClicked.AddDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnClickedBtnSerachClear);

	_textSearchStat->OnTextCommitted.AddDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnTextCommitted);
	_textSearchStat->OnTextChanged.AddDynamic(this, &UGsUITrayCommonStatFilterInCategory::OnTextChangedMsg);

	_toggleGroupCategory.OnSelectChanged.BindUObject(this, &UGsUITrayCommonStatFilterInCategory::OnSelectedCategory);
}

void UGsUITrayCommonStatFilterInCategory::NativeConstruct()
{
	Super::NativeConstruct();

	_contentsStatAllSet.Empty();
	_selectedStatSet.Empty();

	_panelCheckBoxStatSet->ClearChildren();

	_toggleGroupCategory.Clear();

	_textSearchStat->SetText(FText::GetEmpty());

	_textSerachWord = _textSearchStat->GetText().ToString();

	_callbackApply = nullptr;
}

void UGsUITrayCommonStatFilterInCategory::NativeDestruct()
{
	_contentsStatAllSet.Empty();
	_selectedStatSet.Empty();

	_callbackApply = nullptr;

	Super::NativeDestruct();
}

void UGsUITrayCommonStatFilterInCategory::OnClickedBtnClose()
{
	Close();
}

void UGsUITrayCommonStatFilterInCategory::OnClickedBtnApply()
{
	if (_callbackApply)
	{
		_callbackApply(_selectedStatSet);
	}

	_callbackApply = nullptr;

	Close();
}

void UGsUITrayCommonStatFilterInCategory::OnClickedBtnReset()
{
	// 2024/1/17 PKT - 선택되어진 스탯 초기화
	_selectedStatSet.Empty();

	// 2022/02/11 PKT - 검색 초기화
	_textSearchStat->SetText(FText::GetEmpty());
	_textSerachWord = _textSearchStat->GetText().ToString();

	class FGsStatFilterHandler* handler = GetHandler();
	if (handler)
	{	
		_slotHelperCategoryBtnTab->RefreshAll(handler->CategoryMaxCount());
		
		MakeViewList();
	}	
}

void UGsUITrayCommonStatFilterInCategory::OnClickedBtnSerachClear()
{
	_textSearchStat->SetText(FText::GetEmpty());
	_textSerachWord = _textSearchStat->GetText().ToString();

	MakeViewList();
}

void UGsUITrayCommonStatFilterInCategory::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (ETextCommit::OnEnter != InCommitMethod)
	{
		return;
	}

	FString tempSerachWord = InText.ToString();
	tempSerachWord.ReplaceInline(TEXT(" "), TEXT(""));
	if (tempSerachWord.IsEmpty())
	{	// 2022/02/11 PKT - 1글자 이상입력 해야 합니다.
		FText textTickerMsg;
		FText::FindText(TEXT("StatFilterText"), TEXT("UI_Msg_Input_Needed"), textTickerMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textTickerMsg);
		return;
	}

	_textSerachWord = tempSerachWord;

	MakeViewList();
}

void UGsUITrayCommonStatFilterInCategory::OnTextChangedMsg(const FText& InText)
{
	static const int32 maxLength = 50;

	FString changedString;
	bool isTextChanged = FGsUIStringHelper::GetFormatedEditableText(InText.ToString(), changedString, maxLength, false, false, true);
	if (isTextChanged)
	{
		_textSearchStat->SetText(FText::FromString(changedString));
	}
}


void UGsUITrayCommonStatFilterInCategory::OnRefreshEntryTabButton(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITabButtonCommon> item = Cast<UGsUITabButtonCommon>(InEntry);
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("false == item.IsValid()"));
		return;
	}

	FGsStatFilterHandler* statFilterHandler = GetHandler();
	if (nullptr == statFilterHandler)
	{
		return;
	}

	const FGsStatFilterCategoryItem* categoryItem = statFilterHandler->FindCatgegoryItem(InIndex);
	if (nullptr == categoryItem)
	{
		return;
	}

	int32 count = 0;
	for (StatType stat : _selectedStatSet)
	{
		if (categoryItem->_statSet.Contains(stat))
		{	// 2024/1/17 PKT - 선택되어진 갯수 파악
			count += 1;
		}
	}

	item->SetTitleText(categoryItem->_categoryName);
	item->SetSubText((0 < count) ? FText::AsNumber(count) : FText::GetEmpty());

	// 2022/02/10 PKT - Add toggle group
	_toggleGroupCategory.AddToggleWidget(item->GetToggleGroupEntry());
}

void UGsUITrayCommonStatFilterInCategory::OnCreateEntryCheckBox(UWidget* InEntry)
{
	TWeakObjectPtr<UGsUICheckBoxCommonText> item = Cast<UGsUICheckBoxCommonText>(InEntry);
	if (false == item.IsValid())
	{
		GSLOG(Warning, TEXT("false == item.IsValid()"));
		return;
	}

	if (false == item->_onClickSlot.IsBoundToObject(this))
	{
		item->_onClickSlot.BindUObject(this, &UGsUITrayCommonStatFilterInCategory::OnClickedCheckBox);
	}
}

void UGsUITrayCommonStatFilterInCategory::OnRefreshEntryCheckBox(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUICheckBoxCommonText> item = Cast<UGsUICheckBoxCommonText>(InEntry);
	if (false == item.IsValid())
	{
		GSLOG(Warning, TEXT("false == item.IsValid()"));
		return;
	}

	if (false == _viewListStatItemSet.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == _viewListStatItemSet.IsValidIndex(InIndex) : %d"), InIndex);
		return;
	}

	// 2022/02/10 PKT - Index / StatName / Check State
	item->SetIndexInGroup(InIndex);
	item->SetText(_viewListStatItemSet[InIndex].Value);
	item->SetIsSelected(_selectedStatSet.Contains(_viewListStatItemSet[InIndex].Key));
}

void UGsUITrayCommonStatFilterInCategory::OnClickedCheckBox(bool bIsChecked, int32 InIndexInGroup)
{
	if (false == _viewListStatItemSet.IsValidIndex(InIndexInGroup))
	{
		GSLOG(Error, TEXT("false == _viewListStatItemSet.IsValidIndex(InIndexInGroup) : %d"), InIndexInGroup);
		return;
	}

	const StatType targetStat = _viewListStatItemSet[InIndexInGroup].Key;

	if (bIsChecked && false == _selectedStatSet.Contains(targetStat))
	{
		_selectedStatSet.Emplace(targetStat);
	}
	else if (false == bIsChecked && _selectedStatSet.Contains(targetStat))
	{
		_selectedStatSet.Remove(targetStat);
	}

	FGsStatFilterHandler* statFilterHandler = GetHandler();
	if (statFilterHandler)
	{
		_slotHelperCategoryBtnTab->RefreshAll(statFilterHandler->CategoryMaxCount());
	}
}

void UGsUITrayCommonStatFilterInCategory::OnSelectedCategory(int32 InIndex)
{
	MakeViewList();
}

void UGsUITrayCommonStatFilterInCategory::MakeViewList()
{
	_viewListStatItemSet.Empty();

	FGsStatFilterHandler* statfilterHandler = GetHandler();
	if (nullptr == statfilterHandler)
	{
		return;
	}

	const FGsStatFilterCategoryItem* categoryItem = statfilterHandler->FindCatgegoryItem(_toggleGroupCategory.GetSelectedIndex());
	if (nullptr == categoryItem)
	{
		return;
	}

	for (StatType item : categoryItem->_statSet)
	{
		const FGsStatFilterItem* findData = statfilterHandler->FindOrAddStatFilterItem(item);
		
		if (_textSerachWord.IsEmpty() || findData->_cachedTrimName.Contains(_textSerachWord))
		{
			if (_contentsStatAllSet.Contains(item))
			{	// 2024/1/17 PKT - 해당 컨텐츠에 있는 Stat 만 화면에 보인다.
				_viewListStatItemSet.Emplace(TPair<StatType, FText>(item, findData->_name));
			}			
		}
	}
	
	const int32 maxCount = _viewListStatItemSet.Num();
	if (0 >= maxCount)
	{
		static const int32 EMPTY_STAT_LIST = 1;
		_switcherStatList->SetActiveWidgetIndex(EMPTY_STAT_LIST);
	}
	else
	{
		static const int32 NOT_EMPTY_STAT_LIST = 0;
		_switcherStatList->SetActiveWidgetIndex(NOT_EMPTY_STAT_LIST);
		
		_slotHelperCheckBoxSet->RefreshAll(maxCount);
	}	
}

FGsStatFilterHandler* UGsUITrayCommonStatFilterInCategory::GetHandler() const
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		return nullptr;
	}
	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	return (hud) ? hud->GetStatFilterHandler() : nullptr;
}

void UGsUITrayCommonStatFilterInCategory::SetData(const TArray<StatType>& InContentsAllStatSet, const TArray<StatType>& InSelectedStatSet, TFunction<void(TArray<StatType>&)> InCallback)
{
	FGsStatFilterHandler* statfilterHandler = GetHandler();
	if (nullptr == statfilterHandler)
	{
		return;
	}
	
	_callbackApply = InCallback;

	// 2024/1/17 PKT - 해당 컨텐츠에 있는 모든 Stat
	_contentsStatAllSet = InContentsAllStatSet;
	// 2024/1/17 PKT - 선택된 Stat 정보
	_selectedStatSet = InSelectedStatSet;

	// 2024/1/17 PKT - 카테고리 생성
	const int32 maxCount = statfilterHandler->CategoryMaxCount();
	_slotHelperCategoryBtnTab->RefreshAll(maxCount);

	// 2024/1/17 PKT - '전체' 카테고리 선택
	static const int32 DEFAULT_SELECTED_INDEX = 0;
	_toggleGroupCategory.SetSelectedIndex(DEFAULT_SELECTED_INDEX, true);
}