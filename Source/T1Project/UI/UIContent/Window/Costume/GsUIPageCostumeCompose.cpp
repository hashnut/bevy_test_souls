#include "GsUIPageCostumeCompose.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Sound/GsSoundPlayer.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Costume/GsSchemaCostumeCompose.h"
#include "DataSchema/Costume/GsSchemaCostumeComposeRate.h"
#include "Costume/GsCostumeData.h"

#include "Summon/GsSummonDefine.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyStat.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyComposeRes.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyComposeResEffect.h"

#include "UI/UIContent/Popup/GsUIPopupComposeFailure.h"
#include "UI/UIContent/Popup/GsUIPopupCeilingCostume.h" 
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyWait.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyProgress.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyResult.h"

#include "GsUICostumeItem.h"

#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "Components/Image.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsFairyMessageParam.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Summon/GsSummonFunc.h"
#include "Algo/Count.h"
#include "Algo/Transform.h"
#include "ForEach.h"

using ActTaskStateCostume = UGsUIPageCostumeCompose::ActTaskState;

void UGsUIPageCostumeCompose::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPageCostumeCompose::OnCreateEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPageCostumeCompose::OnRefreshEntry);
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

void UGsUIPageCostumeCompose::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUICostumeItem>())
		{
			child->RemoveFromParent();
		}
	}

	auto InitActState = [this]() {

		TWeakObjectPtr<UGsUIPageCostumeCompose> thiz = this;
		_stateManager.pushTask(ActTaskStateCostume::Composition, MakeShared<FGsTaskStateAct>(TEXT("Composition"),
			[thiz]() { if (thiz.IsValid()) thiz.Get()->OnStateCostumeCompose(); }));

		_stateManager.pushTask(ActTaskStateCostume::Ceiling, MakeShared<FGsTaskStateAct>(TEXT("Ceiling"),
			[thiz]() { if (thiz.IsValid()) thiz.Get()->OnStateCostumeCeiling(); }));
	};

	auto initComposeData = [this]() {

		TArray<const FGsSchemaCostumeCompose*> composeGradeList;
		if (const UGsTable* table = FGsSchemaCostumeCompose::GetStaticTable())
		{
			table->GetAllRows<FGsSchemaCostumeCompose>(composeGradeList);
			Algo::TransformIf(composeGradeList, _enableCeilingGrade, [](const auto e) {
				return e->upgradeRateList.FindByPredicate([](auto& re) {
					return re.ceilingItemDataList.Num() >= 1; }) != nullptr; }, [](const auto e) { return e->grade; });
		}

		for (auto e : composeGradeList)
		{
			_gradeData.Add(e->grade);
			_gradeData[e->grade]._costGold = e->costGold;
			_gradeData[e->grade]._successGrade = static_cast<CostumeGrade>(int(e->grade) + 1);

			if (const auto successProbability = e->successProbabilityId.GetRow())
			{
				if (successProbability->costumeList.IsValidIndex(0))
				{
					if (const auto successCostume = successProbability->costumeList[0].costumeId.GetRow())
					{
						_gradeData[e->grade]._successGrade = successCostume->grade;
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
		_gradeTabButtons.Add(CostumeGrade::MAX, _btnGradeAll);
		_gradeTabButtons.Add(CostumeGrade::LEGEND, _btnGradeLegend);
		_gradeTabButtons.Add(CostumeGrade::UNIQUE, _btnGradeUnique);
		_gradeTabButtons.Add(CostumeGrade::EPIC, _btnGradeEpic);
		_gradeTabButtons.Add(CostumeGrade::RARE, _btnGradeRare);
		_gradeTabButtons.Add(CostumeGrade::MAGIC, _btnGradeMagic);
		_gradeTabButtons.Add(CostumeGrade::NORMAL, _btnGradeNormal);

		int32 index = 0;
		for (auto iter : _gradeTabButtons)
		{
			_tabGradeIndex.Add(index, iter.Key);
			_toggleGroupGrade.AddToggleWidget(iter.Value);

			index++;
		}
		//_btnGradeLegend->SetVisibility(ESlateVisibility::Collapsed);
		_toggleGroupGrade.OnSelectChanged.BindUObject(this, &UGsUIPageCostumeCompose::OnGradeTabChanged);
	};

	_gradeTab = -1;
	_selectedGrade = CostumeGrade::NONE;

	InitActState();
	initComposeData();
	initTabButtons();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPageCostumeCompose::OnCreateEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPageCostumeCompose::OnRefreshEntry);

	_lockRes5->OnClicked.AddDynamic(this, &UGsUIPageCostumeCompose::OnClickResource5Toggle);
	_lockRes6->OnClicked.AddDynamic(this, &UGsUIPageCostumeCompose::OnClickResource6Toggle);

	_btnAutoAdd->OnClicked.AddDynamic(this, &UGsUIPageCostumeCompose::OnAutoResourceAdd);
	_btnCompose->OnClicked.AddDynamic(this, &UGsUIPageCostumeCompose::OnStartComposition);
	_btnCeiling->OnClicked.AddDynamic(this, &UGsUIPageCostumeCompose::OnPopupCeilingSummon);
	_btnFailureInfo->OnClicked.AddDynamic(this, &UGsUIPageCostumeCompose::OnAcqFailureItem);

	_btnCompose->SetOnClickLockSec(1.f);

	_addRessources = { TPair<int, UGsSwitcherButton*>{ 4, _lockRes5 }, TPair<int, UGsSwitcherButton*>{ 5, _lockRes6 } };
	_composeCostumePortrait = { _ItemCostumePortrait1, _ItemCostumePortrait2, _ItemCostumePortrait3, _ItemCostumePortrait4, _ItemCostumePortrait5, _ItemCostumePortrait6 };

	int8 portraitindex = 0;
	for (auto entry : _composeCostumePortrait)
	{
		entry->WidgetType = UGsUICostumeItem::UIItemType::RESOURCE;
		entry->OnItemSelected.BindLambda([portraitindex, this](CostumeId id) {
			OnSelectedResourceItem(portraitindex, id);
			});
		portraitindex++;
	}

	FText findText;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Compose_MultiCompose"), findText); /*다중합성{0}회*/
	_multiComposeMsg = findText.ToString();
}

void UGsUIPageCostumeCompose::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_gradeTab < 0)
		_toggleGroupGrade.SetSelectedIndex(0, true);

	_taskManager.doTasks();
}

void UGsUIPageCostumeCompose::NativeDestruct()
{
	if (_taskManager._taskFinished)
	{
		_taskManager.clearTask(true);
		_taskManager._taskFinished();
		_taskManager._taskFinished = nullptr;
	}

	if (auto setting = GGameUserSettings())
	{
		setting->SetCostumeLockOption(_lockRes5->GetIsSelected() == false, _lockRes6->GetIsSelected() == false);
	}

	Super::NativeDestruct();
}

#pragma region IGsSelectedPanelInterface
void UGsUIPageCostumeCompose::InitializeTab(int32 tabIndex)
{
	IGsSelectedPanelInterface::InitializeTab(tabIndex);

	InitUI();

	if (auto setting = GGameUserSettings())
	{
		bool seletableleft = setting->GetCostumeLockOption(true) == false;
		bool seletableright = setting->GetCostumeLockOption(false) == false;
		_lockRes5->SetIsSelected(seletableleft);
		_lockRes6->SetIsSelected(seletableright);

		_switchLockRes5->SetActiveWidgetIndex(seletableleft ? 0 : 1);
		_switchLockRes6->SetActiveWidgetIndex(seletableright ? 0 : 1);
	}
}

void UGsUIPageCostumeCompose::ReleasedTab()
{
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupCeilingCostume")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairyCeilingComposeInfo")).Get())
		popup->Close();
}

void UGsUIPageCostumeCompose::OnSelected()
{
	InitUI();

	//if (_tabGradeIndex.Find(_gradeTab))
	//	SetRefreshCostumeItems(_tabGradeIndex[_gradeTab]);
}

void UGsUIPageCostumeCompose::OnUpdateItemList(TArray<CostumeId>& arr)
{
	if (!_isSelectedTab) return;

	auto selectTab = _tabGradeIndex[_gradeTab];
	bool isRefresh = selectTab == CostumeGrade::MAX;
	if (!isRefresh)
	{
		for (auto e : arr)
		{
			if (auto data = GSCostume()->GetCostumeData(e).Pin())
			{
				isRefresh = data->GetCostumeGrade() == selectTab;
				if (isRefresh) break;
			}
		}
	}
	if (isRefresh) SetRefreshCostumeItems(selectTab);
}
#pragma endregion

void UGsUIPageCostumeCompose::OnStateCostumeCompose()
{
	if (_lockState) return;

	if (_curState == ActTaskStateCostume::Ceiling)
	{
		Algo::ForEach(_gradeTabButtons, [](auto& pair) {pair.Value->SetIsEnabled(true); });
		_btnAutoAdd->SetIsEnabled(true);
	}
	_curState = ActTaskStateCostume::Composition;
}

void UGsUIPageCostumeCompose::OnStateCostumeCeiling()
{
	if (_lockState) return;

	if (_curState == ActTaskStateCostume::Composition)
	{
		Algo::ForEach(_gradeTabButtons, [](auto& pair) {pair.Value->SetIsEnabled(false); });
		_btnAutoAdd->SetIsEnabled(false);

		ClearCompositionResourceItem();
		_toggleGroupGrade.SetSelectedIndex(0, true);

		if (auto popup = Cast<UGsUIPopupCeilingCostume>(GUI()->OpenAndGetWidget(TEXT("PopupCeilingCostume")).Get()))
		{
			_lockState = true;
			popup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (!popup->OnCelingStart.IsBound())
				popup->OnCelingStart.BindUObject(this, &UGsUIPageCostumeCompose::StartCeiling);
			if (!popup->OnCelingCanceled.IsBound())
				popup->OnCelingCanceled.BindUObject(this, &UGsUIPageCostumeCompose::CancelCeiling);
		}
	}

	_curState = ActTaskStateCostume::Ceiling;
}

void UGsUIPageCostumeCompose::InitUI()
{
	_gradeTab = -1;
	_selectedGrade = CostumeGrade::NONE;

	if (auto userData = GGameData()->GetUserData())
		_genderType = userData->mGender;

	CompositionPan composePan;
	_selectCostumeIdList.Empty(0);
	_selectCostumeIdList.Add(composePan);
	for (auto e : _composeCostumePortrait)
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
	_ItemComposeResult->SetItemResult(static_cast<ItemGrade>(_selectedGrade), static_cast<ItemGrade>(_selectedGrade));
	_composeEffect->SetCostumeResult(_selectedGrade);

	_decoSelectGrade->SetActiveWidgetIndex(0);
	_composeGrade->SetActiveWidgetIndex(0);

	_btnAutoAdd->SetIsEnabled(true);

	for (auto& e : _gradeTabButtons)
	{
		e.Value->SetIsEnabled(true);
	}

	_toggleGroupGrade.SetSelectedIndex(0, true);

	_lockState = false;
	_curState = ActTaskStateCostume::None;
	_stateManager._curState = ActTaskStateCostume::None;
	_stateManager.ChangeTask(ActTaskStateCostume::Composition);
}

void UGsUIPageCostumeCompose::OnGradeTabChanged(int32 index)
{
//	if (_gradeTab == index) return;

	_gradeTab = index;
	auto selectTab = _tabGradeIndex[index];
	SetRefreshCostumeItems(selectTab);
}

void UGsUIPageCostumeCompose::OnAutoResourceAdd()
{
	if (_curState == ActTaskStateCostume::Composition)
		SelectAutoComposition();
}

void UGsUIPageCostumeCompose::OnStartComposition()
{
	if (_curState == ActTaskStateCostume::Composition)
	{
		_lockState = true;
		StartComposition();
	}
}
	
void UGsUIPageCostumeCompose::OnPopupCeilingSummon()
{
	_stateManager.ChangeTask(ActTaskStateCostume::Ceiling);
}

void UGsUIPageCostumeCompose::OnAcqFailureItem()
{
	if (auto popup = Cast<UGsUIPopupComposeFailure>(GUI()->OpenAndGetWidget(TEXT("PopupFairyCeilingComposeInfo")).Get()))
	{
		UGsUIPopupComposeFailure::PopupInitData param{ ComposeType::Costume, static_cast<int>(_tabGradeIndex[_gradeTab]) };
		popup->InitializeData(&param);
	}
}

void UGsUIPageCostumeCompose::OnClickResource5Toggle()
{
	_lockRes5->SetIsSelected(!_lockRes5->GetIsSelected());
	_switchLockRes5->SetActiveWidgetIndex(_lockRes5->GetIsSelected() ? 0 : 1);
	
	ClearCompositionResourceItem();
	_scrollBoxHelper->RefreshAll(_gradeList.Num());

	_lockState = false;
	_btnCeiling->SetIsEnabled(true);
}

void UGsUIPageCostumeCompose::OnClickResource6Toggle()
{
	_lockRes6->SetIsSelected(!_lockRes6->GetIsSelected());
	_switchLockRes6->SetActiveWidgetIndex(_lockRes6->GetIsSelected() ? 0 : 1);
	
	ClearCompositionResourceItem();
	_scrollBoxHelper->RefreshAll(_gradeList.Num());

	_lockState = false;
	_btnCeiling->SetIsEnabled(true);
}

void UGsUIPageCostumeCompose::OnCreateEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		entry->WidgetType = UGsUICostumeItem::UIItemType::COMPOSITION;
		entry->OnItemSelected.BindUObject(this, &UGsUIPageCostumeCompose::OnSelectedCostumeItem);
	}
}
//left item refresh
void UGsUIPageCostumeCompose::OnRefreshEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		if (_gradeList.IsValidIndex(index))
		{
			auto data = _gradeList[index].Pin();
			int useCount = 0;
			for (auto& e : _selectCostumeIdList)
			{
				for (auto eid : e._list)
				{
					if (eid == data->_id)  useCount++;
				}
			}

			entry->SetData(_gradeList[index], _genderType, data->isEnableComposingItem() == false ? data->GetAmount() : useCount);
			entry->SetSelected(false);
			entry->RefreshUI();
		}
	}
}
//Left Item Selected
void UGsUIPageCostumeCompose::OnSelectedCostumeItem(CostumeId id)
{
	int32 itemCount = _scrollBoxHelper->GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		UWidget* child = _listPanel->GetChildAt(i);
		if (auto item = Cast<UGsUICostumeItem>(child))
		{
			item->SetSelected(item->GetCostumeID() == id, false);
		}
	}

	auto panelCount = SetResourceToUsableSlot(id);
	if (panelCount < 0) return;

	int countPerPan = maxResCount;
	for (auto& e : _addRessources)
	{
		countPerPan -= (e.Value->GetIsSelected() == false ? 1 : 0);
	}

	int composeCount = 0;
	for (auto eid : _selectCostumeIdList[panelCount - 1]._list)
	{
		if (eid != INVALID_COSTUME_ID) composeCount++;
	}

	if (panelCount <= 1)
	{
		_btnCeiling->SetIsEnabled(composeCount < minResCount);
		_btnCompose->SetIsEnabled(composeCount >= minResCount);
		_bonusRate->SetVisibility(composeCount >= minResCount ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

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
		_bonusRate->SetVisibility(composeCount >= countPerPan ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
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

	_decoSelectGrade->SetActiveWidgetIndex(static_cast<int>(_selectedGrade));
	_multiCompose->SetVisibility(panelCount > 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (auto iter = _gradeData.Find(_selectedGrade))
	{
		_textBonusRate->SetText(FText::FromString(FString::Printf(TEXT("%.02f%%"), (*iter)._upgradeRate[composeCount] * 100.f)));

		bool isEffect = (panelCount > 1 && composeCount >= countPerPan) || (panelCount <= 1 && composeCount >= minResCount);

		_composeEffect->SetCostumeResult((*iter)._successGrade, isEffect);
		_ItemComposeResult->SetItemResult(static_cast<ItemGrade>(_selectedGrade), static_cast<ItemGrade>((*iter)._successGrade));
	}
}

//Right 재료 Selected
void UGsUIPageCostumeCompose::OnSelectedResourceItem(int8 portraitindex, CostumeId id)
{
	_composeCostumePortrait[portraitindex]->SetData(nullptr);
	_composeCostumePortrait[portraitindex]->RefreshUI();

	for (auto e : _composeCostumePortrait)
	{
		e->SetSelected(e->GetCostumeID() == id, false);
	}

	auto panelCount = _selectCostumeIdList.Num();

	_selectCostumeIdList[panelCount - 1]._list[portraitindex] = INVALID_COSTUME_ID;
	_scrollBoxHelper->RefreshAll(_gradeList.Num());
	
	int countPerPan = maxResCount - Algo::CountIf(_addRessources, [](auto& e) { return e.Value->GetIsSelected() == false; });
	int composeCount = Algo::CountIf(_selectCostumeIdList[panelCount - 1]._list, [](auto& e) {return e != INVALID_COSTUME_ID;});

	if (composeCount == 0 && panelCount > 1)
	{
		panelCount--;
		_selectCostumeIdList.RemoveAt(panelCount);

		for (auto& e : _selectCostumeIdList[panelCount - 1]._list)
		{
			if (e != INVALID_COSTUME_ID) composeCount++;
		}

		int index = 0;
		for (auto eid : _selectCostumeIdList[panelCount - 1]._list)
		{
			_composeCostumePortrait[index]->SetData(GSCostume()->GetCostumeData(eid), _genderType); //count가 표시되지 않도록
			_composeCostumePortrait[index]->RefreshUI();
			index++;
		}
	}

	if (auto iter = _gradeData.Find(_selectedGrade))
	{
		bool isEffect = (panelCount > 1 && composeCount >= countPerPan) || (panelCount <= 1 && composeCount >= minResCount);

		_textBonusRate->SetText(FText::FromString(FString::Printf(TEXT("%.02f%%"), (*iter)._upgradeRate[composeCount] * 100.f)));
		_composeEffect->SetCostumeResult((*iter)._successGrade, isEffect);
		_ItemComposeResult->SetItemResult(static_cast<ItemGrade>(_selectedGrade), static_cast<ItemGrade>((*iter)._successGrade));
	}
	_multiCompose->SetVisibility(panelCount > 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (panelCount <= 1)
	{
		_btnCeiling->SetIsEnabled(composeCount < minResCount);
		_btnCompose->SetIsEnabled(composeCount >= minResCount);
		_bonusRate->SetVisibility(composeCount >= minResCount ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

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
			_selectedGrade = CostumeGrade::NONE;
			_composeEffect->SetCostumeResult(_selectedGrade);
			_ItemComposeResult->SetItemResult(static_cast<ItemGrade>(_selectedGrade), static_cast<ItemGrade>(_selectedGrade));

			_decoSelectGrade->SetActiveWidgetIndex(0);
		}	
	}
	else
	{
		_btnCeiling->SetIsEnabled(false);
		_btnCompose->SetIsEnabled(composeCount >= countPerPan);
		_bonusRate->SetVisibility(composeCount >= countPerPan ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
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

void UGsUIPageCostumeCompose::SetRefreshCostumeItems(CostumeGrade grade)
{
	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (auto item = Cast<UGsUICostumeItem>(child))
		{
			item->SetSelected(false);
		}
	}

	TArray<TWeakPtr<FGsCostumeData>> gradelist;
	GSCostume()->GetGradeCostumeDictionary(grade, gradelist);

	_gradeList.Empty(0);
	_gradeList.Append(gradelist.FilterByPredicate([](TWeakPtr<FGsCostumeData> e) {
		return (e.IsValid() && e.Pin()->isCollectedCostume() && e.Pin()->GetAmount() >= 1);
		}));

	_scrollBoxHelper->RefreshAll(_gradeList.Num());

	TArray<int> gradeCounts = { 0, 0, 0, 0, 0, 0, 0, 0 };
	for (auto e : _gradeList)
	{
		auto data = e.Pin();
		auto nGrade = static_cast<int>(data->GetCostumeGrade());
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
void UGsUIPageCostumeCompose::SelectAutoComposition()
{
	TMap<CostumeGrade, int> gradeCounts;
	gradeCounts.Add(CostumeGrade::NONE, 0);

	for (auto e : _gradeList)
	{
		if (e.Pin()->isEnableComposingItem() == false) continue;

		auto grade = e.Pin()->GetCostumeGrade();
		if (gradeCounts.Find(grade) == nullptr)
			gradeCounts.Add(grade, 0);
		gradeCounts[grade] += e.Pin()->GetAmount();
	}

	//6개이상이 있으면 그 grade중 낮은 것이 우선. 없으면 minResCount~maxResCount개는 동일 취급 grade가 낮은 것이 우선
	CostumeGrade usegrade = CostumeGrade::NONE;
	if (gradeCounts.Num() > 1)
	{
		int value = minResCount;
		for (auto key : { CostumeGrade::UNIQUE, CostumeGrade::EPIC, CostumeGrade::RARE, CostumeGrade::MAGIC, CostumeGrade::NORMAL }) 
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

	if (usegrade == CostumeGrade::NONE) return;

	auto NoUseCheck = [this](int index)->bool {
		return _addRessources.FindByPredicate([index](auto& e) {
			return e.Key == index && e.Value->GetIsSelected() == false; }) != nullptr;
	};

	int countPerPan = maxResCount - Algo::CountIf(_addRessources, [](auto& e) { return e.Value->GetIsSelected() == false; });

	CompositionPan composePan;
	_selectCostumeIdList.Empty(0);
	_selectCostumeIdList.Add(composePan);

	//11회 이상의 다중합성 제한
	int fullCount = *gradeCounts.Find(usegrade);
	int useResCount = (int(fullCount / countPerPan) > maxCompositionPanel ? maxCompositionPanel : int(fullCount / countPerPan)) * countPerPan;
	if (useResCount == 0) useResCount = fullCount % countPerPan;

	int index = 0;
	int panelIndex = _selectCostumeIdList.Num() - 1;
	for (auto e : _gradeList)
	{
		auto item = e.Pin();
		if (item->GetCostumeGrade() == usegrade)
		{
			int amount = item->GetAmount();
			amount = useResCount - amount < 0 ? useResCount : amount;
			useResCount -= amount;
			while (amount > 0)
			{
				if (index >= maxResCount)
				{
					_selectCostumeIdList.Add(composePan);
					panelIndex = _selectCostumeIdList.Num() - 1;
					index = 0;
				}
				if (!NoUseCheck(index))
				{
					_selectCostumeIdList[panelIndex]._list[index] = item->_id;
					amount--;
				}
				index++;
			}
			if (useResCount == 0) break;
		}
	}

	int panelCount = _selectCostumeIdList.Num();
	auto& usePanel = _selectCostumeIdList[panelIndex]._list;

	for (int i = 0; i < maxResCount; i++)
	{
		if (usePanel[i] != INVALID_COSTUME_ID)
			 _composeCostumePortrait[i]->SetData(GSCostume()->GetCostumeData(usePanel[i]), _genderType);
		else _composeCostumePortrait[i]->SetData(nullptr);

		_composeCostumePortrait[i]->RefreshUI();
	}

	_selectedGrade = usegrade;
	int gradeTab = _gradeTab;
	for (auto& tabinfo : _tabGradeIndex)
	{
		if (tabinfo.Value == _selectedGrade)
		{
			gradeTab = tabinfo.Key;
			break;
		}
	}

	if (gradeTab != _gradeTab)
		_toggleGroupGrade.SetSelectedIndex(gradeTab, true);
	else _scrollBoxHelper->RefreshAll(_gradeList.Num());

	for (auto& e : _gradeTabButtons)
	{
		e.Value->SetIsEnabled(e.Key == _selectedGrade);
	}

	_textMultiComposeCount->SetText(FText::FromString(FString::Format(*_multiComposeMsg, { panelCount })));
	if (auto iter = _gradeData.Find(_selectedGrade))
	{
		countPerPan = fullCount < countPerPan ? fullCount : countPerPan;

		_textBonusRate->SetText(FText::FromString(FString::Printf(TEXT("%.02f%%"), (*iter)._upgradeRate[countPerPan] * 100.f)));

		_composeEffect->SetCostumeResult((*iter)._successGrade, countPerPan >= minResCount);
		_ItemComposeResult->SetItemResult(static_cast<ItemGrade>(_selectedGrade), static_cast<ItemGrade>((*iter)._successGrade));
	}

	_btnCeiling->SetIsEnabled(false);
	_btnCompose->SetIsEnabled(true);
	_bonusRate->SetVisibility(ESlateVisibility::Visible);
	_multiCompose->SetVisibility(panelCount > 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	_composeGrade->SetActiveWidgetIndex(countPerPan - 3);
}

int32 UGsUIPageCostumeCompose::SetResourceToUsableSlot(CostumeId id)
{
	auto costumeData = GSCostume()->GetCostumeData(id).Pin();
	if (!costumeData->isEnableComposingItem()) //보여주기는 하지만 처리안함
		return -1;

	ItemAmount useCount = 0;
	Algo::ForEach(_selectCostumeIdList, [&useCount, id](auto& e) {useCount += Algo::Count(e._list, id); });

	if (useCount >= costumeData->GetAmount()) return -1;

	auto NoUseCheck = [this](int index)->bool {
		return _addRessources.FindByPredicate([index](auto& e) {
			return e.Key == index && e.Value->GetIsSelected() == false; }) != nullptr;
	};

	// 마지막 판에 사용가능한 빈 슬롯 찾기
	int selectIndex = -1;
	auto panelCount = _selectCostumeIdList.Num();
	auto& pan = _selectCostumeIdList[panelCount - 1]._list;
	for (int i = 0; i < maxResCount; i++)
	{
		if (selectIndex == -1 && !NoUseCheck(i) && pan[i] == INVALID_COSTUME_ID)
		{
			selectIndex = i;
		}
	}

	// 마지막 판에 빈 슬롯이 없을 때 (-1), 최대값(MaxCompositionPanel)을 넘지 않는다면 한장 PUSH
	if (selectIndex == -1)
	{
		if (panelCount >= maxCompositionPanel) return -1;

		CompositionPan composePan;
		_selectCostumeIdList.Add(composePan);

		panelCount++;
		selectIndex = 0;

		for (auto portrait : _composeCostumePortrait)
		{
			portrait->SetData(nullptr);
			portrait->RefreshUI();
		}
	}

	_selectCostumeIdList[panelCount - 1]._list[selectIndex] = id;
	_composeCostumePortrait[selectIndex]->SetData(GSCostume()->GetCostumeData(id), _genderType); //count가 표시되지 않도록
	_composeCostumePortrait[selectIndex]->RefreshUI();

	//전체 탭일때는 선택된 아이템 grade 탭으로 옮겨준다
	if (_selectedGrade == CostumeGrade::NONE)
	{
		auto selectedGrade = _selectedGrade = costumeData->GetCostumeGrade();

		int gradeTab = _gradeTab;
		if (auto findIter = Algo::FindByPredicate(_tabGradeIndex, [&selectedGrade](auto& tabinfo) {
			return tabinfo.Value == selectedGrade; }))
			gradeTab = (*findIter).Key;

		_gradeTab = 0;
		_toggleGroupGrade.SetSelectedIndex(gradeTab, true);

		for (auto& e : _gradeTabButtons)
		{
			e.Value->SetIsEnabled(e.Key == _selectedGrade);
		}
	}
	else
	{
		_scrollBoxHelper->RefreshAll(_gradeList.Num());
	}

	return panelCount;
}

void UGsUIPageCostumeCompose::StartComposition()
{
	// 소환 대기 갯수 최대 인지 체크
	auto isCheckEnableGrade = [](CostumeGrade grade) { return int(grade) > int(CostumeGrade::MAGIC);  };

	//https://jira.com2us.com/jira/browse/C2URWQ-5176
	// stack 갯수 최대 인지 체크
	if (isCheckEnableGrade(_selectedGrade) && FGsSummonFunc::CheckCostumeSummonWaitCount() == false) 
	{
		return;
	}

	TMap<CostumeId, ItemAmount> composeList;
	TArray<uint8> composeAmount;
	for (auto& e : _selectCostumeIdList)
	{
		uint8 amount = 0;
		for (auto eid : e._list)
		{
			if (eid != INVALID_COSTUME_ID)
			{
				if (!composeList.Find(eid))
					composeList.Add(eid, 0);
				composeList[eid] += 1;
				amount++;
			}
		}

		composeAmount.Add(amount);
	}
	if (GSCostume()->IsEnableStackCostume() == false)
	{
		if (GSCostume()->CheckComposeStackableCostume(composeList) == false)
		{
			FText findText;
			FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_StackFull"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
	}

	_lockState = true;
	TArray<CostumeIdAmountPair> composePair;
	for (auto e : composeList)
	{
		composePair.Add(CostumeIdAmountPair{ e.Key, e.Value });
	}

	if (composePair.Num() != 0)
	{
		if (auto data = GSCostume()->GetCostumeData(composePair[0].mCostumeId).Pin())
		{
			CostumeGrade composeGrade = data->GetCostumeGrade();
			GSSummon()->SetCostumeSummonComposeParticleGrade(composeGrade);
			GSSummon()->SetCostumeSummonComposeTryGrade(composeGrade);
		}
	}
	GSSummon()->SetNotUseRetry();
	GSCostume()->SetEffectPlaying(true, true);
	FGsNetSendServiceWorld::SendReqCostumeCompose(composePair, composeAmount);

}

void UGsUIPageCostumeCompose::ClearCompositionResourceItem()
{
	CompositionPan composePan;
	_selectCostumeIdList.Empty(0);
	_selectCostumeIdList.Add(composePan);

	for (int i = 0; i < maxResCount; i++)
	{
		_composeCostumePortrait[i]->SetData(nullptr);
		_composeCostumePortrait[i]->RefreshUI();
	}

	for (auto& e : _gradeTabButtons)
	{
		e.Value->SetIsEnabled(true);
	}

	_selectedGrade = CostumeGrade::NONE;
	_decoSelectGrade->SetActiveWidgetIndex(0);
	_composeEffect->SetCostumeResult(_selectedGrade);
	_ItemComposeResult->SetItemResult(static_cast<ItemGrade>(_selectedGrade), static_cast<ItemGrade>(_selectedGrade));

	_bonusRate->SetVisibility(ESlateVisibility::Hidden);
	_multiCompose->SetVisibility(ESlateVisibility::Hidden);
	_btnCompose->SetIsEnabled(false);

	_composeGrade->SetActiveWidgetIndex(0);
}

void UGsUIPageCostumeCompose::StartCeiling(uint32 gradeIndex, uint8 phase)
{
	CostumeGrade grade = static_cast<CostumeGrade>(gradeIndex);

	_taskManager._taskFinished = []() {
		if (auto uiManager = GUI())
		{
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantWait"));
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantProgress"));
		}
		if (auto costumeManager = GSCostume())
		{
			if (costumeManager->IsEffectPlaying())
			{
				costumeManager->OnSummonComplete(nullptr);
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

			GSCostume()->SetEffectPlaying(true);
			GSSummon()->SetCostumeSummonWaitChangeParticleGrade(grade);
			GSSummon()->SetNotUseRetry();

			FGsNetSendServiceWorld::SendReqCostumeCeiling(grade, phase);
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

	if (auto popup = Cast<UGsUIPopupCeilingCostume>(GUI()->GetWidgetByKey(TEXT("PopupCeilingCostume")).Get()))
	{
		popup->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIPageCostumeCompose::CancelCeiling()
{
	_lockState = false;
	_stateManager.ChangeTask(ActTaskStateCostume::Composition);
}

void UGsUIPageCostumeCompose::OnSummonComplete()
{
	_lockState = false;

	if (_curState == ActTaskStateCostume::Ceiling)
	{
		_stateManager.ChangeTask(ActTaskStateCostume::Ceiling);

		if (auto popup = Cast<UGsUIPopupCeilingCostume>(GUI()->GetWidgetByKey(TEXT("PopupCeilingCostume")).Get()))
		{
			_lockState = true;
			popup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			popup->OnCeilingResult(true);
		}
	}

}

void UGsUIPageCostumeCompose::OnComposeResult(TArray<CostumeId>& dataList)
{
	_lockState = false;
	_btnCeiling->SetIsEnabled(true);

	ClearCompositionResourceItem();
	SetRefreshCostumeItems(_tabGradeIndex[_gradeTab]);
}

void UGsUIPageCostumeCompose::OnCeilingResult(bool success)
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

		if (auto costumeManager = GSCostume())
			costumeManager->SetEffectPlaying(false);
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

					if (auto popup = Cast<UGsUIPopupCeilingCostume>(uiManager->GetWidgetByKey(TEXT("PopupCeilingCostume")).Get()))
					{
						popup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						popup->OnCeilingResult(false);
					}
				}

				if (GSCostume()->IsEffectPlaying())
				{
					GSCostume()->OnSummonComplete(nullptr);
				}
			});
		_taskManager.pushTask(task);
	}
	else
	{
		if (auto popup = Cast<UGsUIPopupCeilingCostume>(GUI()->GetWidgetByKey(TEXT("PopupCeilingCostume")).Get()))
		{
			popup->SetVisibility(ESlateVisibility::Hidden);
		}

		_taskManager.clearTask();
		if (_taskManager._taskFinished) _taskManager._taskFinished();
	}
}

void UGsUIPageCostumeCompose::PlayEffectSound(EffectSoundResType InResType)
{
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	_effectSoundID[(uint8)InResType] = soundPlayer->PlaySound2D(GetSoundResName(InResType));
}

void UGsUIPageCostumeCompose::StopEffectSound(EffectSoundResType InResType)
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
