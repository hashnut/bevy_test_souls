// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIListEntryAcqItemPanel.h"
#include "GsUIEntryAcqItem.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "Components/PanelWidget.h"

void UGsUIListEntryAcqItemPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _itemPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIEntryAcqItem>())
		{
			child->RemoveFromParent();
		}
	}

	_panelWidgetHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_panelWidgetHelper->Initialize(_entryWidgetClass, _itemPanel);
	_panelWidgetHelper->OnRefreshEntry.AddDynamic(this, &UGsUIListEntryAcqItemPanel::OnRefreshEntry);
}

void UGsUIListEntryAcqItemPanel::OnRefreshEntry(int32 index, UWidget* entry)
{
	if (_dataList.IsValidIndex(index))
	{
		if (auto itemWidget = Cast<UGsUIEntryAcqItem>(entry))
		{
			itemWidget->SetData(index, _dataList[index], 1.f);
		}
	}
}

void UGsUIListEntryAcqItemPanel::ClearData()
{
	_dataList.Reset();
	RefreshUI();
}

void UGsUIListEntryAcqItemPanel::SetData(int index, int composeResCount, FGsSchemaCeilingItemData& data)
{
	static TMap<int, FText> descList;
	if (descList.Num() <= 0)
	{
		FText FairyUI_Compose_FailureReward_4;
		FText FairyUI_Compose_FailureReward_5;
		FText FairyUI_Compose_FailureReward_6;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Compose_FailureReward_4"), FairyUI_Compose_FailureReward_4);
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Compose_FailureReward_5"), FairyUI_Compose_FailureReward_5);
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Compose_FailureReward_6"), FairyUI_Compose_FailureReward_6);

		descList.Emplace(4, FairyUI_Compose_FailureReward_4);
		descList.Emplace(5, FairyUI_Compose_FailureReward_5);
		descList.Emplace(6, FairyUI_Compose_FailureReward_6);
	}

	_dataList.EmplaceAt(index, data); //차례대로 아니면 사이즈 못늘림
	_textResCountTitle = descList.Find(composeResCount) ? descList[composeResCount] : FText::GetEmpty();
}

void UGsUIListEntryAcqItemPanel::RefreshUI()
{
	_panelWidgetHelper->RefreshAll(_dataList.Num());
}
