// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStaticPanelSlotHelper.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"

FGsStaticPanelSlotHelper::~FGsStaticPanelSlotHelper()
{

}

void FGsStaticPanelSlotHelper::Clear()
{
	_slotList.Empty();
}

void FGsStaticPanelSlotHelper::RefreshAll(const int InItemCount)
{
	for (int32 i = 0, maxCount = _slotList.Num(); i < maxCount; ++i)
	{
		TWeakObjectPtr<UWidget> entry = _slotList[i];
		if (false == entry.IsValid())
		{
			continue;
		}

		if (InItemCount > i)
		{
			entry->SetVisibility(ESlateVisibility::Visible);

			// 엔트리에 Item 세팅할 수 있게 델리게이트 호출
			OnRefreshEntry.ExecuteIfBound(i, entry.Get());
		}
		else
		{
			// item 개수를 초과하는 entry는 Hide 처리
			entry->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void FGsStaticPanelSlotHelper::RefreshByIndex(const int InIndex)
{
	if (_slotList.IsValidIndex(InIndex))
	{
		if (_slotList[InIndex].IsValid())
		{
			OnRefreshEntry.ExecuteIfBound(InIndex, _slotList[InIndex].Get());
		}
	}
}

void FGsStaticPanelSlotHelper::Initialize(UPanelWidget* InPanel, bool bInUsePanelSlot)
{
	_panel = InPanel;
	_bUsePanelSlot = bInUsePanelSlot;

	if (_bUsePanelSlot)
	{
		if (_panel.IsValid())
		{
			TArray<UWidget*> childList = _panel->GetAllChildren();
			for (int32 i = 0, maxCount = childList.Num(); i < maxCount; ++i)
			{
				AddSlot(childList[i]);
			}
		}
	}
}

void FGsStaticPanelSlotHelper::AddSlot(UWidget* InSlot)
{
	_slotList.Add(InSlot);
}
