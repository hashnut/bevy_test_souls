// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIPopupExchangeCostumeSelect.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Costume/GsCostumeData.h"
#include "DataSchema/Item/GsSchemaItemEffectChangeCostume.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Window/Costume/GsUICostumeItem.h"
#include "UI/UIContent/Window/Costume/GsUICostumeDetail.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "PaperSprite.h"
#include "Transform.h"

void UGsUIPopupExchangeCostumeSelect::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPopupExchangeCostumeSelect::OnCreateCostumeItemEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupExchangeCostumeSelect::OnRefreshCostumeItemEntry);
		_scrollBoxHelper = nullptr;
	}
	_tabGradeIndex.Reset();
	_toggleGroupGrade.Clear();

	Super::BeginDestroy();
}

void UGsUIPopupExchangeCostumeSelect::NativeOnInitialized()
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

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPopupExchangeCostumeSelect::OnCreateCostumeItemEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupExchangeCostumeSelect::OnRefreshCostumeItemEntry);

	_gradeTabs.Add(CostumeGrade::LEGEND, _btnGradeLegend);
	_gradeTabs.Add(CostumeGrade::UNIQUE, _btnGradeUnique);
	_gradeTabs.Add(CostumeGrade::EPIC, _btnGradeEpic);
	_gradeTabs.Add(CostumeGrade::RARE, _btnGradeRare);
	_gradeTabs.Add(CostumeGrade::MAGIC, _btnGradeMagic);
	_gradeTabs.Add(CostumeGrade::NORMAL, _btnGradeNormal);
	
	_toggleGroupGrade.OnSelectChanged.BindUObject(this, &UGsUIPopupExchangeCostumeSelect::OnGradeTabChanged);
	_btnSelected->OnClicked.AddDynamic(this, &UGsUIPopupExchangeCostumeSelect::OnClickSelectd);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupExchangeCostumeSelect::OnClickCancel);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupExchangeCostumeSelect::OnClickCancel);
}

void UGsUIPopupExchangeCostumeSelect::NativeConstruct()
{
	Super::NativeConstruct();

	_gradeTab = -1;
}

void UGsUIPopupExchangeCostumeSelect::NativeDestruct()
{
	_selectedCostume.Reset();

	Super::NativeDestruct();
}

void UGsUIPopupExchangeCostumeSelect::OnGradeTabChanged(int32 index)
{
	if (_gradeTab == index || _tabGradeIndex.Find(index) == nullptr) return;

	_gradeTab = index;
	auto selectTab = _tabGradeIndex[index];

	SetRefreshCostumeItems(selectTab);
}

void UGsUIPopupExchangeCostumeSelect::OnClickSelectd()
{
	if (auto selectTab = _tabGradeIndex.Find(_gradeTab))
	{	 
		if (_callbackSelection && _selectedCostume.IsValid())
		{
			CostumeGrade grade = _selectedCostume.Pin()->GetCostumeGrade();
			TArray<Probability> enableList;
			auto listIter = _exchangeEnableList.Find(grade);

			if (listIter)
			{
				enableList.Append(*listIter);
				enableList.RemoveAll([this](auto& pair) { return pair.Key == _selectedCostume; });
			}

			_callbackSelection(_selectedCostume, enableList);
		}
	}

	Close();
}

void UGsUIPopupExchangeCostumeSelect::OnClickCancel()
{
	Close();
}

void UGsUIPopupExchangeCostumeSelect::OnCreateCostumeItemEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		entry->WidgetType = UGsUICostumeItem::UIItemType::EXCHANGE;
		entry->OnItemSelected.BindUObject(this, &UGsUIPopupExchangeCostumeSelect::OnSelectedCostumeItem);
	}
}

void UGsUIPopupExchangeCostumeSelect::OnRefreshCostumeItemEntry(int32 index, UWidget* InEntry)
{
	CostumeId selectedCostumeId = INVALID_COSTUME_ID;
	if (auto selectedCostume = _selectedCostume.Pin())
		selectedCostumeId = selectedCostume->_id;
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		if (_costumeGradeList.IsValidIndex(index))
		{
			entry->SetData(_costumeGradeList[index], _genderType);
			entry->SetSelected(false);
			if (auto data = _costumeGradeList[index].Pin())
			{
				entry->SetSelected(data->_id == selectedCostumeId);
			}
			entry->RefreshUI();
		}
	}
}

void UGsUIPopupExchangeCostumeSelect::SetRefreshCostumeItems(CostumeGrade grade)
{
	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (auto item = Cast<UGsUICostumeItem>(child))
		{
			item->SetSelected(false);
		}
	}

	_costumeGradeList.Reset();
	_scrollBoxHelper->RefreshAll(0);
	_costumeDetail->SetSelectedCostumeDetail(nullptr);
	_switcherInfo->SetActiveWidgetIndex(1);

	if (grade == CostumeGrade::MAX)
	{	
		for (auto tabPair : _tabGradeIndex)
		{
			if (tabPair.Value != grade)
			{
				GSCostume()->GetGradeCostumeDictionary(tabPair.Value, _costumeGradeList, false);
			}
		}
	}
	else
	{
		GSCostume()->GetGradeCostumeDictionary(grade, _costumeGradeList, false);
	}

	if (_excludeGradeList.Find(grade))
	{
		auto excludeCostumeList = _excludeGradeList[grade];
		_costumeGradeList.RemoveAll([&excludeCostumeList](auto e) {
			auto index = excludeCostumeList.Find(e);
			return index != INDEX_NONE && excludeCostumeList[index].IsValid();
			});
	}

	if (_selectedReservedCostumeId != INVALID_COSTUME_ID)
	{
		_selectedCostume = GSCostume()->GetCostumeData(_selectedReservedCostumeId);
	}

	if (auto selectCostume = _selectedCostume.Pin())
	{
		_costumeGradeList.StableSort([selectCostume](TWeakPtr<FGsCostumeData> le, TWeakPtr<FGsCostumeData> re) {
				return le == selectCostume;
			});
	}

	_scrollBoxHelper->RefreshAll(_costumeGradeList.Num());

	if (_selectedReservedCostumeId != INVALID_COSTUME_ID)
	{
		UWidget* selectedItem = nullptr;
		int32 itemCount = _scrollBoxHelper->GetItemCount();
		for (int32 i = 0; i < itemCount; ++i)
		{
			UWidget* child = _listPanel->GetChildAt(i);
			if (auto item = Cast<UGsUICostumeItem>(child))
			{
				if (item->GetCostumeID() == _selectedReservedCostumeId)
				{
					selectedItem = child;
					break;
				}
			}
		}

		if (selectedItem)
		{
			_listCostume->ScrollWidgetIntoView(selectedItem, true, EDescendantScrollDestination::Center);
		}

		_selectedReservedCostumeId = INVALID_COSTUME_ID;
	}
}

void UGsUIPopupExchangeCostumeSelect::InitializeData(PopupInitData* initParam, TFunction<void(TWeakPtr<FGsCostumeData>, const TArray<Probability>&)> cbSelection /*= nullptr*/)
{
	_callbackSelection = cbSelection;
	_selectedCostume = nullptr;
	_gradeTab = -1;

	if (initParam)
	{
		if (_selectItemId != initParam->itemId || initParam->isSelectionWindow)
		{
			_selectItemId = initParam->itemId;
			MakeExchangeCostumeInfoByItemId();
		}

		_popupType = initParam->isSelectionWindow ? Selection : Detail;
		if (initParam->isSelectionWindow == false)
		{
			_callbackSelection = nullptr;
			_typePanel->SetVisibility(ESlateVisibility::Collapsed);
			_btnClose->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			_typePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_btnClose->SetVisibility(ESlateVisibility::Collapsed);
		}

		_selectedReservedCostumeId = initParam->selectCostume;
		_toggleGroupGrade.SetSelectedIndex(0, true);
	}
}

void UGsUIPopupExchangeCostumeSelect::OnSelectedCostumeItem(CostumeId id)
{
	auto UpdateItemSelection = [this](CostumeId id) {
		int32 itemCount = _scrollBoxHelper->GetItemCount();
		for (int i = 0; i < itemCount; ++i)
		{
			UWidget* child = _listPanel->GetChildAt(i);

			if (auto item = Cast<UGsUICostumeItem>(child))
			{
				if (item->IsSelected())
				{
					item->SetSelected(item->GetCostumeID() == id, false);
					item->RefreshUI();
				}
				else
				{
					item->SetSelected(item->GetCostumeID() == id, false);
					if (item->IsSelected())
						item->RefreshUI();
				}
				item->SetBlockSelected();
			}
		}
	};

	UpdateItemSelection(id);

	_btnSelected->SetIsEnabled(false);
	_costumeName->SetText(FText::GetEmpty());
	_costumeGrade->SetText(FText::GetEmpty());
	_costumeDetail->SetSelectedCostumeDetail(nullptr);

	if (auto costumeData = GSCostume()->GetCostumeData(id).Pin())
	{
		_switcherInfo->SetActiveWidgetIndex(0);
		_selectedCostume = _popupType == Selection ? costumeData  : _selectedCostume;

		FLinearColor gradeColor = FGsUIColorHelper::GetColorGradeWidthItemGrade(costumeData->GetCostumeGrade());
		FText gradeText;
		switch (costumeData->GetCostumeGrade())
		{
		case CostumeGrade::NORMAL:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Normal"), gradeText);	break;
		case CostumeGrade::MAGIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Magic"), gradeText);	break;
		case CostumeGrade::RARE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Rare"), gradeText);		break;
		case CostumeGrade::EPIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Epic"), gradeText);		break;
		case CostumeGrade::UNIQUE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Unique"), gradeText);	break;
		case CostumeGrade::LEGEND:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Legend"), gradeText);	break;
		default: break;
		}
		_costumeName->SetText(costumeData->GetCostumeName());
		_costumeGrade->SetText(gradeText);
		_costumeName->SetColorAndOpacity(gradeColor);
		_costumeGrade->SetColorAndOpacity(gradeColor);

		_costumeDetail->SetSelectedCostumeDetail(costumeData);
		_btnSelected->SetIsEnabled(_popupType == Detail || costumeData->isCollectedCostume());
	}
}

void UGsUIPopupExchangeCostumeSelect::OnInputCancel()
{
	Close();
}

void UGsUIPopupExchangeCostumeSelect::MakeExchangeCostumeInfoByItemId()
{
	const FGsSchemaItemEffectChangeCostume* effectData = nullptr;
	bool ownItem = GSCostume()->FindCostumeExchangeTicketEffectData(_selectItemId, effectData);
	if (effectData)
	{
		_toggleGroupGrade.Clear();
		_tabGradeIndex.Reset();
		_excludeGradeList.Reset();
		_exchangeEnableList.Reset();

		int32 index = 0;

		_tabGradeIndex.Add(index++, CostumeGrade::MAX);
		_toggleGroupGrade.AddToggleWidget(_btnGradeAll);

		for (auto& pair : _gradeTabs)
		{
			pair.Value->SetVisibility(ESlateVisibility::Collapsed);
			auto grade = pair.Key;
			if (auto iter = effectData->changeDataList.FindByPredicate([grade](auto e) { return e.recipeGrade == grade; }))
			{
				pair.Value->SetVisibility(ESlateVisibility::Visible);

				_tabGradeIndex.Add(index++, grade);
				_toggleGroupGrade.AddToggleWidget(pair.Value);

				if (const FGsSchemaCostumeProbability* acqCostumeList = iter->gainProbabilityInfo.GetRow())
				{
					TArray<Probability> list;
					Algo::TransformIf(acqCostumeList->costumeList, list, [](auto& e) { return  e.costumeId.GetRow() != nullptr; }, [](auto& e) {
						return Probability(GSCostume()->GetCostumeData(e.costumeId.GetRow()->id), e.probability); });

					_exchangeEnableList.FindOrAdd(grade).Append(list);
				}

				for (auto row : iter->excludeCostumeIdList)
				{
					if (auto costume = row.GetRow())
					{
						auto exCostume = GSCostume()->GetCostumeData(costume->id);
						_excludeGradeList.FindOrAdd(grade).Add(exCostume);
						_excludeGradeList.FindOrAdd(CostumeGrade::MAX).Add(exCostume);
					}
				}
			}
		}
	}
}