#include "GsUIPageFairyCompose.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Sound/GsSoundPlayer.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Fairy/GsSchemaFairyCompose.h"
#include "DataSchema/Fairy/GsSchemaFairyComposeRate.h"
#include "Fairy/GsFairyData.h"

#include "Summon/GsSummonDefine.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyResult.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "GsUIFairyItem.h"
#include "GsUIFairyStat.h"
#include "GsUIFairyComposeRes.h"
#include "GsUIFairyComposeResEffect.h"

#include "UI/UIContent/Popup/GsUIPopupComposeFailure.h"
#include "UI/UIContent/Popup/GsUIPopupCeilingFairy.h" 
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyWait.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyProgress.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyResult.h"

#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "Components/Image.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsFairyMessageParam.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Summon/GsSummonFunc.h"
#include "Algo/Count.h"
#include "Algo/Transform.h"

using ActTaskStateCompo = UGsUIPageFairyCompose::ActTaskState;

void UGsUIPageFairyCompose::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPageFairyCompose::OnCreateEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPageFairyCompose::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	_tabGradeIndex.Reset();
	_toggleGroupGrade.Clear();

	_stateManager.clearTask(true);
	if (_taskManager._taskFinished)
	{
		_taskManager.clearTask();
		_taskManager._taskFinished();
		_taskManager._taskFinished = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPageFairyCompose::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIFairyItem>())
		{
			child->RemoveFromParent();
		}
	}

	auto InitActState = [this]() {

		TWeakObjectPtr<UGsUIPageFairyCompose> thiz = this;
		_stateManager.pushTask(ActTaskStateCompo::Composition, MakeShared<FGsTaskStateAct>(TEXT("Composition"),
			[thiz]() { if (thiz.IsValid()) thiz.Get()->OnStateFairyCompose(); }));

		_stateManager.pushTask(ActTaskStateCompo::Ceiling, MakeShared<FGsTaskStateAct>(TEXT("Ceiling"),
			[thiz]() { if (thiz.IsValid()) thiz.Get()->OnStateFairyCeiling(); }));
	};

	auto initComposeData = [this]() {

		TArray<const FGsSchemaFairyCompose*> composeGradeList;
		if (const UGsTable* table = FGsSchemaFairyCompose::GetStaticTable())
		{
			table->GetAllRows<FGsSchemaFairyCompose>(composeGradeList);
			Algo::TransformIf(composeGradeList, _enableCeilingGrade, [](const auto e) {
				return e->upgradeRateList.FindByPredicate([](auto re) {
					return re.ceilingItemDataList.Num() >= 1; }) != nullptr; }, [](const auto e) { return e->grade; });
		}

		for (auto e : composeGradeList)
		{
			_gradeData.Add(e->grade);
			_gradeData[e->grade]._costGold = e->costGold;
			_gradeData[e->grade]._successGrade = static_cast<FairyGrade>(int(e->grade) + 1);

			if (const auto successProbability = e->successProbabilityId.GetRow())
			{
				if (successProbability->fairyList.IsValidIndex(0))
				{
					if (const auto successFairy = successProbability->fairyList[0].fairyId.GetRow())
					{
						_gradeData[e->grade]._successGrade = successFairy->grade;
					}
				}
			}

			for (auto re : e->upgradeRateList)
			{
				if (re.materialCount <= maxResCount)
					_gradeData[e->grade]._upgradeRate[re.materialCount] = re.upgradeRate;
			}
		}
	};

	auto initTabButtons = [this]() {
		_gradeTabButtons.Add(FairyGrade::MAX, _btnGradeAll);
		_gradeTabButtons.Add(FairyGrade::LEGEND, _btnGradeLegend);
		_gradeTabButtons.Add(FairyGrade::UNIQUE, _btnGradeUnique);
		_gradeTabButtons.Add(FairyGrade::EPIC, _btnGradeEpic);
		_gradeTabButtons.Add(FairyGrade::RARE, _btnGradeRare);
		_gradeTabButtons.Add(FairyGrade::MAGIC, _btnGradeMagic);
		_gradeTabButtons.Add(FairyGrade::NORMAL, _btnGradeNormal);

		int32 index = 0;
		for (auto iter : _gradeTabButtons)
		{
			_tabGradeIndex.Add(index, iter.Key);
			_toggleGroupGrade.AddToggleWidget(iter.Value);

			index++;
		}
		//_btnGradeLegend->SetVisibility(ESlateVisibility::Collapsed);
		_toggleGroupGrade.OnSelectChanged.BindUObject(this, &UGsUIPageFairyCompose::OnGradeTabChanged);
	};

	_gradeTab = -1;
	_selectedFairyGrade = FairyGrade::NONE;

	InitActState();
	initComposeData();
	initTabButtons();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPageFairyCompose::OnCreateEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPageFairyCompose::OnRefreshEntry);

	_btnAutoAdd->OnClicked.AddDynamic(this, &UGsUIPageFairyCompose::OnAutoResourceAdd);
	_btnCompose->OnClicked.AddDynamic(this, &UGsUIPageFairyCompose::OnStartComposition);
	_btnFailureInfo->OnClicked.AddDynamic(this, &UGsUIPageFairyCompose::OnAcqFailureItem);

	_btnCeiling->OnClicked.AddDynamic(this, &UGsUIPageFairyCompose::OnPopupCeilingSummon);

	_lockRes5->OnClicked.AddDynamic(this, &UGsUIPageFairyCompose::OnClickResource5Toggle);
	_lockRes6->OnClicked.AddDynamic(this, &UGsUIPageFairyCompose::OnClickResource6Toggle);

	//lock되는 리소스
	_addRessources = { TPair<int, UGsSwitcherButton*>{ 4, _lockRes5 }, TPair<int, UGsSwitcherButton*>{ 5, _lockRes6 } };
	_composeFairyPortrait = { _ItemFairyPortrait1, _ItemFairyPortrait2, _ItemFairyPortrait3, _ItemFairyPortrait4, _ItemFairyPortrait5, _ItemFairyPortrait6};
	
	int8 portraitindex = 0;
	for (auto entry : _composeFairyPortrait)
	{
		entry->WidgetType = UGsUIFairyItem::UIItemType::RESOURCE;
		entry->OnItemSelected.BindLambda([portraitindex, this](FairyId id) {
				OnSelectedResourceItem(portraitindex, id);
			});
		portraitindex++;
	}

	FText findText;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Compose_MultiCompose"), findText); /*다중합성{0}회*/
	_multiComposeMsg = findText.ToString();
}

void UGsUIPageFairyCompose::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_gradeTab < 0)
		_toggleGroupGrade.SetSelectedIndex(0, true);

	_taskManager.doTasks();
}

void UGsUIPageFairyCompose::NativeDestruct()
{
	if (_taskManager._taskFinished)
	{
		_taskManager.clearTask(true);
		_taskManager._taskFinished();
		_taskManager._taskFinished = nullptr;
	}

	if (auto setting = GGameUserSettings())
	{
		setting->SetFairyLockOption(_lockRes5->GetIsSelected () == false, _lockRes6->GetIsSelected() == false);
	}

	Super::NativeDestruct();
}

#pragma region IGsSelectedPanelInterface
void UGsUIPageFairyCompose::InitializeTab(int32 tabIndex)
{
	IGsSelectedPanelInterface::InitializeTab(tabIndex);

	InitUI();

	if (auto setting = GGameUserSettings())
	{
		bool seletableleft = setting->GetFairyLockOption(true) == false;
		bool seletableright = setting->GetFairyLockOption(false) == false;
		_lockRes5->SetIsSelected(seletableleft);
		_lockRes6->SetIsSelected(seletableright);

		_switchLockRes5->SetActiveWidgetIndex(seletableleft ? 0 : 1);
		_switchLockRes6->SetActiveWidgetIndex(seletableright ? 0 : 1);
	}
}

void UGsUIPageFairyCompose::ReleasedTab()
{
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupCeilingFairy")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairyCeilingComposeInfo")).Get())
		popup->Close();
}

void UGsUIPageFairyCompose::OnSelected()
{
	InitUI();
}

void UGsUIPageFairyCompose::OnUpdateItemList(TArray<FairyId>& arr)
{
	if (!_isSelectedTab) return;

	auto selectTab = _tabGradeIndex[_gradeTab];
	bool isRefresh = selectTab == FairyGrade::MAX;
	if (!isRefresh)
	{
		for (auto e : arr)
		{
			if (auto data = GSFairy()->GetFairyData(e).Pin())
			{
				isRefresh = data->GetFairyGrade() == selectTab;
				if (isRefresh) break;
			}
		}
	}
	if (isRefresh) SetRefreshFairyItems(selectTab, _stateManager._curState);
}
#pragma endregion
#pragma region StateChange
void UGsUIPageFairyCompose::OnStateFairyCompose()
{
	if (_lockState) return;

	if (_curState == ActTaskStateCompo::Ceiling)
	{
		Algo::ForEach(_gradeTabButtons, [](auto& pair) {pair.Value->SetIsEnabled(true); });
		_btnAutoAdd->SetIsEnabled(true);
	}
	_curState = ActTaskStateCompo::Composition;
}

void UGsUIPageFairyCompose::OnStateFairyCeiling()
{
	if (_lockState) return;

	if (_curState == ActTaskStateCompo::Composition)
	{
		Algo::ForEach(_gradeTabButtons, [](auto& pair) {pair.Value->SetIsEnabled(false); });
		_btnAutoAdd->SetIsEnabled(false);

		ClearCompositionResourceItem();
		_toggleGroupGrade.SetSelectedIndex(0, true);

		if (auto popup = Cast<UGsUIPopupCeilingFairy>(GUI()->OpenAndGetWidget(TEXT("PopupCeilingFairy")).Get()))
		{
			_lockState = true;
			popup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if(!popup->OnCelingStart.IsBound())
				popup->OnCelingStart.BindUObject(this, &UGsUIPageFairyCompose::StartCeiling);
			if(!popup->OnCelingCanceled.IsBound())
				popup->OnCelingCanceled.BindUObject(this, &UGsUIPageFairyCompose::CancelCeiling);
		}
	}

	_curState = ActTaskStateCompo::Ceiling;
}
#pragma endregion

void UGsUIPageFairyCompose::InitUI()
{
	_gradeTab = -1;
	_selectedFairyGrade = FairyGrade::NONE;

	CompositionPan composePan;
	_selectFairyIdList.Empty(0);
	_selectFairyIdList.Add(composePan);
	for (auto e : _composeFairyPortrait)
	{
		e->SetData(nullptr);
		e->SetSelected(false);
		e->RefreshUI();
	}
	_scrollBoxHelper->RefreshAll(0);

	_btnFailureInfo->SetIsEnabled(false);
	_btnCompose->SetIsEnabled(false);
	_bonusRate->SetVisibility(ESlateVisibility::Hidden);
	_multiCompose->SetVisibility(ESlateVisibility::Hidden);
	_composeEffect->SetFairyResult(_selectedFairyGrade);
	_ItemFairyComposeResult->SetResult(_selectedFairyGrade, _selectedFairyGrade);

	_decoSelectGrade->SetActiveWidgetIndex(0);
	_composeGrade->SetActiveWidgetIndex(0);

	_btnAutoAdd->SetIsEnabled(true);

	for (auto& e : _gradeTabButtons)
	{
		e.Value->SetIsEnabled(true);
	}

	_toggleGroupGrade.SetSelectedIndex(0, true);

	_lockState = false;
	_curState = ActTaskStateCompo::None;
	_stateManager._curState = ActTaskStateCompo::None;
	_stateManager.ChangeTask(ActTaskStateCompo::Composition);
}

void UGsUIPageFairyCompose::OnGradeTabChanged(int32 index)
{
	//if (_gradeTab == index) return;

	_gradeTab = index;
	auto selectTab = _tabGradeIndex[index];
	SetRefreshFairyItems(selectTab);
}

void UGsUIPageFairyCompose::OnAutoResourceAdd()
{
	if (_curState == ActTaskStateCompo::Composition)
		SelectAutoComposition();
}

void UGsUIPageFairyCompose::OnStartComposition()
{
	if (_curState == ActTaskStateCompo::Composition)
	{
		_lockState = true;
		StartComposition();
	}
}

void UGsUIPageFairyCompose::OnPopupCeilingSummon()
{
	_stateManager.ChangeTask(ActTaskStateCompo::Ceiling);
}

void UGsUIPageFairyCompose::OnAcqFailureItem()
{
	if (auto popup = Cast<UGsUIPopupComposeFailure>(GUI()->OpenAndGetWidget(TEXT("PopupFairyCeilingComposeInfo")).Get()))
	{
		UGsUIPopupComposeFailure::PopupInitData param{ ComposeType::Fairy, static_cast<int>(_tabGradeIndex[_gradeTab]) };
		popup->InitializeData(&param);
	}
}

void UGsUIPageFairyCompose::OnClickResource5Toggle()
{	
	_lockRes5->SetIsSelected(!_lockRes5->GetIsSelected());
	_switchLockRes5->SetActiveWidgetIndex(_lockRes5->GetIsSelected() ? 0 : 1);

	ClearCompositionResourceItem();
	_scrollBoxHelper->RefreshAll(_fairyGradeList.Num());

	_lockState = false;
	_btnCeiling->SetIsEnabled(true);
}

void UGsUIPageFairyCompose::OnClickResource6Toggle()
{
	_lockRes6->SetIsSelected(!_lockRes6->GetIsSelected());
	_switchLockRes6->SetActiveWidgetIndex(_lockRes6->GetIsSelected() ? 0 : 1);

	ClearCompositionResourceItem();
	_scrollBoxHelper->RefreshAll(_fairyGradeList.Num());

	_lockState = false;
	_btnCeiling->SetIsEnabled(true);
}

void UGsUIPageFairyCompose::OnCreateEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairyItem>(InEntry))
	{
		entry->WidgetType = UGsUIFairyItem::UIItemType::COMPOSITION;
		entry->OnItemSelected.BindUObject(this, &UGsUIPageFairyCompose::OnSelectedFairyItem);
	}
}

void UGsUIPageFairyCompose::OnRefreshEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairyItem>(InEntry))
	{
		if (_fairyGradeList.IsValidIndex(index))
		{
			auto data = _fairyGradeList[index].Pin();
			int fairyCount = 0;
			for (auto& e :  _selectFairyIdList)
			{
				for (auto eid : e._list)
				{
					if (eid == data->_id)  fairyCount++;
				}
			}
			
			entry->SetData(_fairyGradeList[index], data->isEnableComposingItem() == false ? data->GetAmount() : fairyCount);
			entry->SetSelected(false);
			entry->RefreshUI();
		}
	}
}
//Left Item Selected
void UGsUIPageFairyCompose::OnSelectedFairyItem(FairyId id)
{
	int32 itemCount = _scrollBoxHelper->GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		UWidget* child = _listPanel->GetChildAt(i);
		if (auto item = Cast<UGsUIFairyItem>(child))
		{
			item->SetSelected(item->GetFairyId() == id, false);
		}
	}

	auto panelCount = SetResourceToUsableSlot(id);
	if (panelCount < 0) return;

	int composeCount = 0;
	for (auto eid : _selectFairyIdList[panelCount - 1]._list)
	{
		if (eid != INVALID_FAIRY_ID) composeCount++;
	}

	int countPerPan = maxResCount;
	for (auto& e : _addRessources)
	{
		countPerPan -= (e.Value->GetIsSelected() == false ? 1 : 0);
	}
	
	if (panelCount <= 1)
	{
		_btnCeiling->SetIsEnabled(composeCount < minResCount);
		_btnCompose->SetIsEnabled(composeCount >= minResCount);
		_bonusRate->SetVisibility(composeCount >= minResCount ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		if (composeCount >= minResCount)
		{
			_composeGrade->SetActiveWidgetIndex(composeCount - 3);
		}
		else
		{
			_composeGrade->SetActiveWidgetIndex(0);
		}
	}
	else
	{
		_btnCeiling->SetIsEnabled(false);
		_btnCompose->SetIsEnabled(composeCount >= countPerPan);
		_bonusRate->SetVisibility(composeCount >= countPerPan ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		_textMultiComposeCount->SetText(FText::FromString(FString::Format(*_multiComposeMsg, { panelCount})));

		if (composeCount >= countPerPan)
		{
			_composeGrade->SetActiveWidgetIndex(countPerPan - 3);
		}
		else
		{
			_composeGrade->SetActiveWidgetIndex(0);
		}
	}

	_decoSelectGrade->SetActiveWidgetIndex(static_cast<int>(_selectedFairyGrade));
	_multiCompose->SetVisibility(panelCount > 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (auto iter = _gradeData.Find(_selectedFairyGrade))
	{
		_textBonusRate->SetText(FText::FromString(FString::Printf(TEXT("%.02f%%"), (*iter)._upgradeRate[composeCount] * 100.f)));

		bool isEffect = (panelCount > 1 && composeCount >= countPerPan) || (panelCount <= 1 && composeCount >= minResCount);
		_composeEffect->SetFairyResult((*iter)._successGrade, isEffect);
		_ItemFairyComposeResult->SetResult(_selectedFairyGrade, (*iter)._successGrade);
	}
}
//Right 재료 Selected
void UGsUIPageFairyCompose::OnSelectedResourceItem(int8 portraitindex, FairyId id)
{
	_composeFairyPortrait[portraitindex]->SetData(nullptr);
	_composeFairyPortrait[portraitindex]->RefreshUI();

	for (auto e : _composeFairyPortrait)
	{
		e->SetSelected(e->GetFairyId() == id, false);
	}

	auto panelCount = _selectFairyIdList.Num();

	_selectFairyIdList[panelCount - 1]._list[portraitindex] = INVALID_FAIRY_ID;
	_scrollBoxHelper->RefreshAll(_fairyGradeList.Num());

	int countPerPan = maxResCount - Algo::CountIf(_addRessources, [](auto& e) { return e.Value->GetIsSelected() == false; });
	int composeCount = Algo::CountIf(_selectFairyIdList[panelCount - 1]._list, [](auto& e) {return e != INVALID_FAIRY_ID;});

	if (composeCount == 0 && panelCount > 1)
	{
		panelCount--;
		_selectFairyIdList.RemoveAt(panelCount);

		for (auto& e : _selectFairyIdList[panelCount - 1]._list)
		{
			if (e != INVALID_FAIRY_ID) composeCount++;
		}

		int index = 0;
		for (auto eid : _selectFairyIdList[panelCount - 1]._list)
		{
			_composeFairyPortrait[index]->SetData(GSFairy()->GetFairyData(eid)); //count가 표시되지 않도록
			_composeFairyPortrait[index]->RefreshUI();
			index++;
		}
	}
	if (auto iter = _gradeData.Find(_selectedFairyGrade))
	{
		bool isEffect = (panelCount > 1 && composeCount >= countPerPan) || (panelCount <= 1 && composeCount >= minResCount);

		_textBonusRate->SetText(FText::FromString(FString::Printf(TEXT("%.02f%%"), (*iter)._upgradeRate[composeCount] * 100.f)));
		_composeEffect->SetFairyResult((*iter)._successGrade, isEffect);
		_ItemFairyComposeResult->SetResult(_selectedFairyGrade, (*iter)._successGrade);
	}

	if (auto iter = _gradeData.Find(_selectedFairyGrade))
	_textBonusRate->SetText(FText::FromString(FString::Printf(TEXT("%.02f%%"), (*iter)._upgradeRate[composeCount] * 100.f)));
	_multiCompose->SetVisibility(panelCount > 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (panelCount <= 1)
	{
		_btnCeiling->SetIsEnabled(composeCount < minResCount);
		_btnCompose->SetIsEnabled(composeCount >= minResCount);
		_bonusRate->SetVisibility(composeCount >= minResCount ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		if (composeCount >= minResCount)
		{
			_composeGrade->SetActiveWidgetIndex(composeCount - 3);
		}
		else
		{
			_composeGrade->SetActiveWidgetIndex(0);
		}

		if (composeCount == 0)
		{
			for (auto& e : _gradeTabButtons)
			{
				e.Value->SetIsEnabled(true);
			}
			_selectedFairyGrade = FairyGrade::NONE;
			_composeEffect->SetFairyResult(_selectedFairyGrade);
			_ItemFairyComposeResult->SetResult(_selectedFairyGrade, _selectedFairyGrade);

			_decoSelectGrade->SetActiveWidgetIndex(0);
		}
	}
	else
	{	
		_btnCeiling->SetIsEnabled(false);
		_btnCompose->SetIsEnabled(composeCount >= countPerPan);
		_bonusRate->SetVisibility(composeCount >= countPerPan ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		_textMultiComposeCount->SetText(FText::FromString(FString::Format(*_multiComposeMsg, { panelCount })));

		if (composeCount >= countPerPan)
		{
			_composeGrade->SetActiveWidgetIndex(countPerPan - 3);
		}
		else
		{
			_composeGrade->SetActiveWidgetIndex(0);
		}
	}
}

void UGsUIPageFairyCompose::SetRefreshFairyItems(FairyGrade grade, UGsUIPageFairyCompose::ActTaskState state)
{
	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (auto item = Cast<UGsUIFairyItem>(child))
		{
			item->SetSelected(false);
		}
	}

	TArray<TWeakPtr<FGsFairyData>> gradelist;
	GSFairy()->GetGradeFairyDictionary(grade, gradelist);

	_fairyGradeList.Empty(0);
	_fairyGradeList.Append(gradelist.FilterByPredicate([](TWeakPtr<FGsFairyData> e) {
		return (e.IsValid() && e.Pin()->isCollectedFairy() && e.Pin()->GetAmount() >= 1);
		}));

	_scrollBoxHelper->RefreshAll(_fairyGradeList.Num());

	TArray<int> gradeCounts = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (auto e : _fairyGradeList)
	{
		auto data = e.Pin();
		auto nGrade = static_cast<int>(data->GetFairyGrade());
		gradeCounts[nGrade] += (data->isEnableComposingItem() ? data->GetAmount() : 0);
	}

	bool countCheck = gradeCounts.FindByPredicate([](auto e) { return e >= minResCount; }) != nullptr;

	_btnCeiling->SetIsEnabled(true);
	_btnCompose->SetIsEnabled(false);
	_btnAutoAdd->SetIsEnabled(countCheck);
	_btnFailureInfo->SetIsEnabled(_enableCeilingGrade.Find(grade) != INDEX_NONE);
	_btnFailureInfo->SetVisibility(_enableCeilingGrade.Find(grade) != INDEX_NONE ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

//Auto 재료 등록
void UGsUIPageFairyCompose::SelectAutoComposition()
{
	TMap<FairyGrade, int> gradeCounts;
	gradeCounts.Add(FairyGrade::NONE, 0);

	for (auto e : _fairyGradeList)
	{
		if (!e.Pin()->isEnableComposingItem()) continue;

		auto grade = e.Pin()->GetFairyGrade();
		if (gradeCounts.Find(grade) == nullptr)
			gradeCounts.Add(grade, 0);
		gradeCounts[grade] += e.Pin()->GetAmount();
	}

	//6개이상이 있으면 그 grade중 낮은 것이 우선. 없으면 minResCount~maxResCount개는 동일 취급 grade가 낮은 것이 우선
	FairyGrade usegrade = FairyGrade::NONE;
	if (gradeCounts.Num() > 1)
	{
		int value = minResCount;
		for (auto key : { FairyGrade::UNIQUE, FairyGrade::EPIC, FairyGrade::RARE, FairyGrade::MAGIC, FairyGrade::NORMAL })
		{ 
			auto val = gradeCounts.Find(key);
			if (val && value <= *val)
			{
				value = *val < maxResCount ? minResCount : maxResCount;
				usegrade = key;
			}
		}
	}
	else
	{
		if ((*gradeCounts.begin()).Value >= minResCount)
			usegrade = (*gradeCounts.begin()).Key;
	}

	if (usegrade == FairyGrade::NONE) return;

	auto NoUseCheck = [this](int index)->bool {
		return _addRessources.FindByPredicate([index](auto& e) {
			return e.Key == index && e.Value->GetIsSelected() == false; }) != nullptr;
	};

	int countPerPan = maxResCount - Algo::CountIf(_addRessources, [](auto& e) { return e.Value->GetIsSelected() == false; });

	CompositionPan composePan;
	_selectFairyIdList.Empty(0);
	_selectFairyIdList.Add(composePan);

	//11회 이상의 다중합성 제한
	int fullCount = *gradeCounts.Find(usegrade);
	int useResCount = (int(fullCount / countPerPan) > maxCompositionPanel ? maxCompositionPanel : int(fullCount / countPerPan)) * countPerPan;
	if (useResCount == 0) useResCount = fullCount % countPerPan;

	int index = 0;
	int panelIndex = _selectFairyIdList.Num() - 1;
	for (auto e : _fairyGradeList)
	{
		auto item = e.Pin();
		if (item->GetFairyGrade() == usegrade)
		{
			int amount = item->GetAmount();
			amount = useResCount - amount < 0 ? useResCount : amount;
			useResCount -= amount;
			while (amount > 0)
			{
				if (index >= maxResCount)
				{
					_selectFairyIdList.Add(composePan);
					panelIndex = _selectFairyIdList.Num() - 1;
					index = 0;
				}

				if (!NoUseCheck(index))
				{
					_selectFairyIdList[panelIndex]._list[index] = item->_id;
					amount--;
				}
				index++;
			}
			if (useResCount == 0) break;
		}
	}

	int panelCount = _selectFairyIdList.Num();
	auto& usePanel = _selectFairyIdList[panelIndex]._list;

	for(int i = 0; i < maxResCount; i++)
	{
		if (usePanel[i] != INVALID_FAIRY_ID)
			 _composeFairyPortrait[i]->SetData(GSFairy()->GetFairyData(usePanel[i]));
		else _composeFairyPortrait[i]->SetData(nullptr);

		_composeFairyPortrait[i]->RefreshUI();
	}

	_selectedFairyGrade = usegrade;
	int gradeTab = _gradeTab;
	for (auto& tabinfo : _tabGradeIndex)
	{
		if (tabinfo.Value == _selectedFairyGrade)
		{
			gradeTab = tabinfo.Key;
			break;
		}
	}

	if(gradeTab != _gradeTab)
		 _toggleGroupGrade.SetSelectedIndex(gradeTab, true);
	else _scrollBoxHelper->RefreshAll(_fairyGradeList.Num());

	for (auto& e : _gradeTabButtons)
	{
		e.Value->SetIsEnabled(e.Key == _selectedFairyGrade);
	}

	//int composeCount = fullCount >= maxResCount ? maxResCount : fullCount % maxResCount;

	_textMultiComposeCount->SetText(FText::FromString(FString::Format(*_multiComposeMsg, { panelCount })));
	if (auto iter = _gradeData.Find(_selectedFairyGrade))
	{
		countPerPan = fullCount < countPerPan ? fullCount : countPerPan;

		_textBonusRate->SetText(FText::FromString(FString::Printf(TEXT("%.02f%%"), (*iter)._upgradeRate[countPerPan] * 100.f)));

		_composeEffect->SetFairyResult((*iter)._successGrade, countPerPan >= minResCount);
		_ItemFairyComposeResult->SetResult(_selectedFairyGrade, (*iter)._successGrade);
	}

	_btnCeiling->SetIsEnabled(false);
	_btnCompose->SetIsEnabled(true);
	_bonusRate->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_multiCompose->SetVisibility(panelCount > 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	
	_composeGrade->SetActiveWidgetIndex(countPerPan - 3);
}

void UGsUIPageFairyCompose::ClearCompositionResourceItem()
{
	CompositionPan composePan;
	_selectFairyIdList.Empty(0);
	_selectFairyIdList.Add(composePan);

	for (int i = 0; i < maxResCount; i++)
	{
		_composeFairyPortrait[i]->SetData(nullptr);
		_composeFairyPortrait[i]->RefreshUI();
	}

	for (auto& e : _gradeTabButtons)
	{
		e.Value->SetIsEnabled(true);
	}

	_selectedFairyGrade = FairyGrade::NONE;
	_decoSelectGrade->SetActiveWidgetIndex(0);
	_composeEffect->SetFairyResult(_selectedFairyGrade);
	_ItemFairyComposeResult->SetResult(_selectedFairyGrade, _selectedFairyGrade);

	_bonusRate->SetVisibility(ESlateVisibility::Hidden);
	_multiCompose->SetVisibility(ESlateVisibility::Hidden);
	_btnCompose->SetIsEnabled(false);

	_composeGrade->SetActiveWidgetIndex(0);
}

int32 UGsUIPageFairyCompose::SetResourceToUsableSlot(FairyId id)
{	
	auto fairyData = GSFairy()->GetFairyData(id).Pin();
	if (!fairyData->isEnableComposingItem()) //if (fairyData->GetFairyGrade() == FairyGrade::LEGEND) //보여주기는 하지만 처리안함
		return -1;

	ItemAmount useCount = 0;
	Algo::ForEach(_selectFairyIdList, [&useCount, id](auto& e) {useCount += Algo::Count(e._list, id); });

	if (useCount >= fairyData->GetAmount()) return -1;

	auto NoUseCheck = [this](int index)->bool {
		return _addRessources.FindByPredicate([index](auto& e) {
			return e.Key == index && e.Value->GetIsSelected() == false; }) != nullptr;
	};

	int selectIndex = -1;
	auto panelCount = _selectFairyIdList.Num();
	auto& pan = _selectFairyIdList[panelCount - 1]._list;
	for(int i = 0; i < maxResCount; i++)
	{
		if (selectIndex == -1 && !NoUseCheck(i) && pan[i] == INVALID_FAIRY_ID)
		{
			selectIndex = i;
		}
	}

	// 마지막 판에 빈 슬롯이 없을 때 (-1), 최대값(MaxCompositionPanel)을 넘지 않는다면 한장 PUSH
	if (selectIndex == -1)
	{
		if (panelCount >= maxCompositionPanel) return -1;

		CompositionPan composePan;
		_selectFairyIdList.Add(composePan);
		
		panelCount++;
		selectIndex = 0;

		for (auto portrait : _composeFairyPortrait)
		{
			portrait->SetData(nullptr);
			portrait->RefreshUI();
		}
	}

	_selectFairyIdList[panelCount - 1]._list[selectIndex] = id;
	_composeFairyPortrait[selectIndex]->SetData(GSFairy()->GetFairyData(id)); //count가 표시되지 않도록
	_composeFairyPortrait[selectIndex]->RefreshUI();

	//전체 탭일때는 선택된 아이템 grade 탭으로 옮겨준다
	if (_selectedFairyGrade == FairyGrade::NONE)
	{
		auto selectedGrade = _selectedFairyGrade = fairyData->GetFairyGrade();

		int gradeTab = _gradeTab;
		if (auto findIter = Algo::FindByPredicate(_tabGradeIndex, [&selectedGrade](auto& tabinfo) {
			return tabinfo.Value == selectedGrade; }))
			gradeTab = (*findIter).Key;

		_gradeTab = 0;
		_toggleGroupGrade.SetSelectedIndex(gradeTab, true);

		for (auto& e : _gradeTabButtons)
		{
			e.Value->SetIsEnabled(e.Key == _selectedFairyGrade);
		}

	}
	else
	{
		_scrollBoxHelper->RefreshAll(_fairyGradeList.Num());
	}

	int composeCount = 0;
	for (auto eid : _selectFairyIdList[panelCount - 1]._list)
	{
		if (eid != INVALID_FAIRY_ID) composeCount++;
	}
	
	return panelCount;
}

void UGsUIPageFairyCompose::StartComposition()
{
	auto isCheckEnableGrade = [](FairyGrade grade) { return int(grade) > int(FairyGrade::MAGIC);  };

	//https://jira.com2us.com/jira/browse/C2URWQ-5176
	// stack 갯수 최대 인지 체크
	if (isCheckEnableGrade(_selectedFairyGrade) && FGsSummonFunc::CheckFairySummonWaitCount() == false)
	{
		return;
	}

	TMap<FairyId, ItemAmount> fairyList;
	TArray<uint8> composeAmount;
	for (auto& e : _selectFairyIdList)
	{
		uint8 amount = 0;
		for (auto eid : e._list)
		{
			if (eid != INVALID_FAIRY_ID)
			{
				if (!fairyList.Find(eid))
					fairyList.Add(eid, 0);
				fairyList[eid] += 1;
				amount++;
			}
		}

		composeAmount.Add(amount);
	}

	if (GSFairy()->IsEnableStackFairy() == false)
	{
		if (GSFairy()->CheckComposeStackableFairy(fairyList) == false)
		{
			FText findText;
			FText::FindText(TEXT("FairyText"), TEXT("Alert_Ticker_StackFull"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
	}

	_lockState = true;

	TArray<FairyIdAmountPair> composefairyList;
	for (auto e : fairyList)
	{
		composefairyList.Add(FairyIdAmountPair{ e.Key, e.Value });
	}

	if (composefairyList.Num() != 0)
	{
		if (auto data = GSFairy()->GetFairyData(composefairyList[0].mFairyId).Pin())
		{
			FairyGrade composeGrade = data->GetFairyGrade();
			GSSummon()->SetFairySummonComposeParticleGrade(composeGrade);
			GSSummon()->SetFairySummonComposeTryGrade(composeGrade);
		}
	}
	
	GSFairy()->SetEffectPlaying(true, true);
	GSSummon()->SetNotUseRetry();
	FGsNetSendServiceWorld::SendReqFairyCompose(composefairyList, composeAmount);
}

void UGsUIPageFairyCompose::CancelCeiling()
{
	_lockState = false;
	_stateManager.ChangeTask(ActTaskStateCompo::Composition);
}

void UGsUIPageFairyCompose::StartCeiling(uint32 gradeIndex, uint8 phase)
{
	FairyGrade grade = static_cast<FairyGrade>(gradeIndex);

	_taskManager._taskFinished = []() {
		if (auto uiManager = GUI())
		{
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantWait"));
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantProgress"));
		}
		if (auto fairyManager = GSFairy())
		{
			if (fairyManager->IsEffectPlaying())
			{
				fairyManager->OnSummonComplete(nullptr);
			}
		}
	};

	auto waitTask = MakeShared<FGsTaskTrayUI>(TEXT("TrayFairyCeilingWait"), TEXT("TrayFairyEnchantWait"), [this]() {
		if (auto uiManager = GUI())
		{
			if (auto widget = Cast<UGsUITrayEnchantFairyBase>(uiManager->OpenAndGetWidget(TEXT("TrayFairyEnchantWait"))))
			{
				UGsUITrayEnchantFairyWait::EnchantWaitEffectInitData param(1.2f, true);
				widget->InitializeData(&param);
			}
			PlayEffectSound(EffectSoundResType::Enchant_NotSafe_Touch);
		}
		}, [grade, phase]() {

			GSFairy()->SetEffectPlaying(true);
			GSSummon()->SetFairySummonWaitChangeParticleGrade(grade);
			GSSummon()->SetNotUseRetry();

			FGsNetSendServiceWorld::SendReqFairyCeiling(grade, phase);
		});

	auto progressTask = MakeShared<FGsTaskTrayUI>(TEXT("TrayFairyCeilintProgress"), TEXT("TrayFairyEnchantProgress"), []() {
		if (auto uiManager = GUI())
		{
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantWait"));
			if (auto widget = Cast<UGsUITrayEnchantFairyBase>(uiManager->OpenAndGetWidget(TEXT("TrayFairyEnchantProgress"))))
			{
				widget->InitializeData(nullptr);
			}
		}
		});

	_taskManager.pushTask(waitTask);
	_taskManager.pushTask(progressTask);

	if (auto popup = Cast<UGsUIPopupCeilingFairy>(GUI()->GetWidgetByKey(TEXT("PopupCeilingFairy")).Get()))
	{
		popup->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIPageFairyCompose::OnSummonComplete()
{
	_lockState = false;
	GSFairy()->SetEffectPlaying(false);

	if (_curState == ActTaskStateCompo::Ceiling)
	{
		_stateManager.ChangeTask(ActTaskStateCompo::Ceiling);

		if (auto popup = Cast<UGsUIPopupCeilingFairy>(GUI()->GetWidgetByKey(TEXT("PopupCeilingFairy")).Get()))
		{
			_lockState = true;
			popup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			popup->OnCeilingResult(true);
		}
	}
}

void UGsUIPageFairyCompose::OnComposeFairyResult(TArray<FairyId>& dataList)
{
	_lockState = false;
	_btnCeiling->SetIsEnabled(true);

	ClearCompositionResourceItem();
	SetRefreshFairyItems(_tabGradeIndex[_gradeTab]);
}

void UGsUIPageFairyCompose::OnCeilingFairyResult(bool success)
{
	_taskManager._taskFinished = [this]() {
		if (auto uiManager = GUI())
		{
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantWait"));
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantProgress"));
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantFailure"));
		}
		StopEffectSound(EffectSoundResType::Enchant_NotSafe_Touch);
		StopEffectSound(EffectSoundResType::Enchant_NotSafe_Fail);

		if (auto fairyManager = GSFairy())
			fairyManager->SetEffectPlaying(false);
	};

	if (success == false)
	{
		auto task = MakeShared<FGsTaskTrayUI>(TEXT("TrayFairyCeilingResultFailure"), TEXT("TrayFairyEnchantFailure"), [this]() {
			if (auto uiManager = GUI())
			{
				uiManager->CloseByKeyName(TEXT("TrayFairyEnchantProgress"));
				if (auto widget = Cast<UGsUITrayEnchantFairyBase>(uiManager->OpenAndGetWidget(TEXT("TrayFairyEnchantFailure"))))
				{
					FText title;
					FText desc;
					FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_SceneFailure_Title"), title);
					FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_SceneFailure_Info"), desc);
					PlayEffectSound(EffectSoundResType::Enchant_NotSafe_Fail);

					UGsUITrayEnchantFairyResult::EnchantResultEffectInitData initparam(nullptr, title, desc, false);
					widget->InitializeData(&initparam);
				}
			}
			}, []() {

				if (auto uiManager = GUI())
				{
					uiManager->CloseByKeyName(TEXT("TrayFairyEnchantFailure"));

					if (auto popup = Cast<UGsUIPopupCeilingFairy>(uiManager->GetWidgetByKey(TEXT("PopupCeilingFairy")).Get()))
					{
						popup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						popup->OnCeilingResult(false);
					}
				}

				if (GSFairy()->IsEffectPlaying())
				{
					GSFairy()->OnSummonComplete(nullptr);
				}
			});
		_taskManager.pushTask(task);
	}
	else 
	{
		if (auto popup = Cast<UGsUIPopupCeilingFairy>(GUI()->GetWidgetByKey(TEXT("PopupCeilingFairy")).Get()))
		{
			popup->SetVisibility(ESlateVisibility::Hidden);
		}

		_taskManager.clearTask();
		if (_taskManager._taskFinished) _taskManager._taskFinished();
	}
}

void UGsUIPageFairyCompose::PlayEffectSound(EffectSoundResType InResType)
{
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	_effectSoundID[(uint8)InResType] = soundPlayer->PlaySound2D(GetSoundResName(InResType));
}

void UGsUIPageFairyCompose::StopEffectSound(EffectSoundResType InResType)
{
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	uint64 soundID = _effectSoundID[(uint8)InResType];
	if (true == soundPlayer->IsPlaying(soundID))
	{
		soundPlayer->StopSound(soundID);
	}
}
