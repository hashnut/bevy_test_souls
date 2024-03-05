// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIPopupComposeFailure.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "DataSchema/Fairy/GsSchemaFairyCompose.h"
#include "DataSchema/Costume/GsSchemaCostumeCompose.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "Fairy/GsUIListEntryAcqItemPanel.h"
#include "Components/PanelWidget.h"


void UGsUIPopupComposeFailure::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const UGsTable* table = FGsSchemaFairyCompose::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaFairyCompose>(_composeFairyGradeList);
	}

	if (const UGsTable* table = FGsSchemaCostumeCompose::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaCostumeCompose>(_composeCostumeGradeList);
	}


	TArray<UWidget*> childList = _getItemPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIListEntryAcqItemPanel>())
		{
			child->RemoveFromParent();
		}
	}

	_panelWidgetHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_panelWidgetHelper->Initialize(_entryWidgetClass, _getItemPanel);
	_panelWidgetHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupComposeFailure::OnRefreshEntry);
}

void UGsUIPopupComposeFailure::OnRefreshEntry(int32 index, UWidget* entry)
{
	if (_selectFairyGrade)
	{
		if (_selectFairyGrade->upgradeRateList.IsValidIndex(index))
		{
			if (auto itemWidget = Cast<UGsUIListEntryAcqItemPanel>(entry))
			{
				itemWidget->ClearData();
				int itemIndex = 0;
				for (auto useData : _selectFairyGrade->upgradeRateList[index].ceilingItemDataList)
				{
					itemWidget->SetData(itemIndex++, _selectFairyGrade->upgradeRateList[index].materialCount, useData);
					itemWidget->RefreshUI();
				}
			}
		}
	}

	if (_selectCostumeGrade)
	{
		if (_selectCostumeGrade->upgradeRateList.IsValidIndex(index))
		{
			if (auto itemWidget = Cast<UGsUIListEntryAcqItemPanel>(entry))
			{
				itemWidget->ClearData();
				int itemIndex = 0;
				for (auto useData : _selectCostumeGrade->upgradeRateList[index].ceilingItemDataList)
				{
					itemWidget->SetData(itemIndex++, _selectCostumeGrade->upgradeRateList[index].materialCount, useData);
					itemWidget->RefreshUI();
				}
			}
		}
	}
}

void UGsUIPopupComposeFailure::InitializeData(PopupInitData* param)
{
	_selectFairyGrade = nullptr;
	_selectCostumeGrade = nullptr;
	if (param)
	{
		if (param->composeType == PopupInitData::ComposeType::Fairy)
		{
			auto grade = static_cast<FairyGrade>(param->composeGrade);
			if (auto iter = _composeFairyGradeList.FindByPredicate([grade](const auto e) { return e->grade == grade; }))
			{
				_selectFairyGrade = *iter;
			}
		}
		else if (param->composeType == PopupInitData::ComposeType::Costume)
		{
			auto grade = static_cast<CostumeGrade>(param->composeGrade);
			if (auto iter = _composeCostumeGradeList.FindByPredicate([grade](const auto e) { return e->grade == grade; }))
			{
				_selectCostumeGrade = *iter;
			}
		}
	}
	
	RefreshUI();
}

void UGsUIPopupComposeFailure::RefreshUI()
{
	if(_selectFairyGrade)
		_panelWidgetHelper->RefreshAll(_selectFairyGrade->upgradeRateList.Num());

	if (_selectCostumeGrade)
		_panelWidgetHelper->RefreshAll(_selectCostumeGrade->upgradeRateList.Num());
}

void UGsUIPopupComposeFailure::OnInputCancel()
{
	Close();
}