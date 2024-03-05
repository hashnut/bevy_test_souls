// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDynamicPanelSlotHelper.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "T1Project.h"


void UGsDynamicPanelSlotHelper::Initialize(TSubclassOf<UUserWidget> InEntryClass, class UPanelWidget* InPanel)
{
	_entryWidgetClass = InEntryClass;
	_panel = InPanel;
}

void UGsDynamicPanelSlotHelper::RefreshAll(const int InItemCount)
{
	if (false == _panel.IsValid())
	{
		return;
	}

	_itemCount = InItemCount;

	// 이미 Panel에 자손으로 있는 것보다 많을 경우에 생성
	int32 createEntryCount = InItemCount - _panel->GetChildrenCount();
	if (0 < createEntryCount)
	{
		for (int i = 0; i < createEntryCount; ++i)
		{
			// 생성하여 Panel에 Add
			UUserWidget* newEntry = CreateWidget(GetWorld(), _entryWidgetClass);
			_panel->AddChild(newEntry);

			OnCreateEntry.Broadcast(newEntry);
		}
	}

	TArray<UWidget*> childList = _panel->GetAllChildren();
	for (int32 i = 0, maxCount = childList.Num(); i < maxCount; ++i)
	{
		UWidget* entry = childList[i];
		if (nullptr == entry)
		{
			continue;
		}

		if (InItemCount > i)
		{
			// 보이도록 켜준다
			entry->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (IGsEntryInterface* slot = Cast<IGsEntryInterface>(entry))
			{
				slot->OnShow();
			}
			
			// 엔트리에 Item 세팅할 수 있게 델리게이트 호출
			OnRefreshEntry.Broadcast(i, entry);
		}
		else
		{
			// 안보이도록 꺼준다
			entry->SetVisibility(ESlateVisibility::Collapsed);
			if (IGsEntryInterface* slot = Cast<IGsEntryInterface>(entry))
			{
				slot->OnHide();
			}
		}
	}

	if (_bUseEmptyPanel && _emptyPanel.IsValid())
	{
		_emptyPanel->SetVisibility(0 >= _itemCount ?
			ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UGsDynamicPanelSlotHelper::RefreshRange(const int32 InBegin, const int32 InRange)
{
	if (false == _panel.IsValid())
	{
		return;
	}

	_itemCount = (InBegin + InRange);

	// 이미 Panel에 자손으로 있는 것보다 많을 경우에 생성
	int32 createEntryCount = _itemCount -_panel->GetChildrenCount();
	if (0 < createEntryCount)
	{
		for (int i = 0; i < createEntryCount; ++i)
		{
			// 생성하여 Panel에 Add
			UUserWidget* newEntry = CreateWidget(GetWorld(), _entryWidgetClass);
			_panel->AddChild(newEntry);

			OnCreateEntry.Broadcast(newEntry);
		}
	}

	TArray<UWidget*> childList = _panel->GetAllChildren();
	for (int32 idx = InBegin; idx < childList.Num(); ++idx)
	{
		UWidget* entry = (childList.IsValidIndex(idx)) ? childList[idx] : nullptr;
		if (nullptr == entry)
		{
			continue;
		}

		if ( _itemCount > idx )
		{
			// 보이도록 켜준다
			entry->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (IGsEntryInterface* slot = Cast<IGsEntryInterface>(entry))
			{
				slot->OnShow();
			}

			// 엔트리에 Item 세팅할 수 있게 델리게이트 호출
			OnRefreshEntry.Broadcast(idx, entry);
		}
		else
		{
			// 안보이도록 꺼준다
			entry->SetVisibility(ESlateVisibility::Collapsed);
			if (IGsEntryInterface* slot = Cast<IGsEntryInterface>(entry))
			{
				slot->OnHide();
			}
		}
	}

	if (_bUseEmptyPanel && _emptyPanel.IsValid())
	{
		_emptyPanel->SetVisibility(0 >= _itemCount ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UGsDynamicPanelSlotHelper::RefreshByIndex(const int InIndex)
{
	if (UWidget* entry = _panel->GetChildAt(InIndex))
	{
		OnRefreshEntry.Broadcast(InIndex, entry);
	}
}

UWidget* UGsDynamicPanelSlotHelper::GetEntry(const int InIndex)
{
	if (_panel.IsValid())
	{
		if (UWidget* entry = _panel->GetChildAt(InIndex))
		{
			return entry;
		}
	}

	return nullptr;
}

void UGsDynamicPanelSlotHelper::SetEmptyListPanel(UPanelWidget* emptyPanel, bool useEmptyPanel)
{
	_emptyPanel = emptyPanel;
	_bUseEmptyPanel = useEmptyPanel;

	if(_emptyPanel.IsValid())
	{
		_emptyPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

int32 UGsDynamicPanelSlotHelper::GetChildrenCount() const
{
	return (_panel.IsValid()) ? _panel->GetChildrenCount() : 0;
}

void UGsDynamicPanelSlotHelper::AllClearChildren()
{
	if (false == _panel.IsValid())
	{
		return;
	}

	TArray<UWidget*> childList = _panel->GetAllChildren();
	_panel->ClearChildren();
}