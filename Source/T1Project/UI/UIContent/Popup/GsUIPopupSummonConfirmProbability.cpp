// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/GsUIPopupSummonConfirmProbability.h"
#include "UI/UIContent/Popup/Fairy/GsUIListDataFairySummonProbability.h"
#include "UI/UIContent/Popup/Costume/GsUIListDataCostumeSummonProbability.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

void UGsUIPopupSummonConfirmProbability::SetTitleDesc(FText title, FText desc)
{
	_titleText = title;
	_descText->SetText(desc);
	_descText->SetVisibility(desc.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIPopupSummonConfirmProbability::SetFairyData(const TArray<FGsFairySummonChangeData>& _dataList)
{
	TArray<UGsUIListDataFairySummonProbability*> listItems;
	for (auto& e : _dataList)
	{
		if (UGsUIListDataFairySummonProbability* listItem = NewObject<UGsUIListDataFairySummonProbability>())
		{
			listItem->SetSummonChangeData(e);
			listItems.Add(listItem);
		}
	}

	if (_listView)
	{
		_listView->ClearListItems();
		_listView->SetListItems(listItems);
	}
}

void UGsUIPopupSummonConfirmProbability::SetCostumeData(const TArray<FGsCostumeSummonChangeData>& _dataList)
{
	TArray<UGsUIListDataCostumeSummonProbability*> listItems;
	for (auto& e : _dataList)
	{
		if (UGsUIListDataCostumeSummonProbability* listItem = NewObject<UGsUIListDataCostumeSummonProbability>())
		{
			listItem->SetSummonChangeData(e);
			listItems.Add(listItem);
		}
	}

	if (_listView)
	{
		_listView->ClearListItems();
		_listView->SetListItems(listItems);
	}
}

void UGsUIPopupSummonConfirmProbability::OnClickClose()
{
	Close();
}

void UGsUIPopupSummonConfirmProbability::OnInputCancel()
{
	Close();
}