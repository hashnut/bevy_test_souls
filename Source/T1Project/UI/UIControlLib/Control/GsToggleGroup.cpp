// Fill out your copyright notice in the Description page of Project Settings.


#include "GsToggleGroup.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"


FGsToggleGroup::~FGsToggleGroup()
{
	Clear();
}

void FGsToggleGroup::Clear()
{
	_selectedIndex = 0;
	_toggleList.Empty();
}

void FGsToggleGroup::AddToggleWidgetByParentPanel(UPanelWidget* InPanel, bool bIsDynamic)
{
	TArray<UWidget*> childList = InPanel->GetAllChildren();
	for (int32 i = 0; i < childList.Num(); ++i)
	{		
		if (UWidget* widget = childList[i])
		{
			// 동적 리스트 관리 때문에 보이는것만 그룹에 넣는다
			if (bIsDynamic)
			{
				ESlateVisibility visibility = widget->GetVisibility();
				if (ESlateVisibility::Hidden != visibility &&
					ESlateVisibility::Collapsed != visibility)
				{
					if (IGsToggleGroupEntry* toggle = RecursiveFindToggleWidget(widget))
					{
						AddToggleWidget(toggle);
					}
				}
			}
			else
			{
				if (IGsToggleGroupEntry* toggle = RecursiveFindToggleWidget(widget))
				{
					AddToggleWidget(toggle);
				}
			}
		}
	}
}

IGsToggleGroupEntry* FGsToggleGroup::RecursiveFindToggleWidget(UWidget* InWidget)
{
	if (IGsToggleGroupEntry* toggle = Cast<IGsToggleGroupEntry>(InWidget))
	{
		return toggle;
	}

	if (false == InWidget->IsA<UPanelWidget>())
	{
		return nullptr;
	}

	UPanelWidget* panel = Cast<UPanelWidget>(InWidget);

	for (int i = 0, maxCount = panel->GetChildrenCount(); i < maxCount; ++i)
	{
		UWidget* child = panel->GetChildAt(i);
		if (IGsToggleGroupEntry* toggleFound = RecursiveFindToggleWidget(child))
		{
			return toggleFound;
		}
	}

	return nullptr;
}

void FGsToggleGroup::AddToggleWidget(IGsToggleGroupEntry* InWidget)
{
	if (nullptr == InWidget)
	{
		return;
	}

	UWidget* toggleTarget = Cast<UWidget>(InWidget);
	if (nullptr == toggleTarget)
	{
		return;
	}

	if (_toggleList.Contains(toggleTarget))
	{
		return;
	}

	int32 index = _toggleList.Num();
	InWidget->InitializeToggle();
	InWidget->SetIndexInGroup(index);
	InWidget->SetToggleCallback([this](int32 InIndex, bool bInIsSelected)
		{	
			if (OnCheckBlock.IsBound()
				&& OnCheckBlock.Execute(InIndex))
			{
				return;
			}

			if (IsBlockedIndex(InIndex))
			{
				OnChangeBlocked.ExecuteIfBound(InIndex);
			}
			else
			{
				OnToggleWidgetStateChanged(InIndex, bInIsSelected);
			}

			OnSlotClicked.ExecuteIfBound(InIndex, bInIsSelected);
		});

	_toggleList.Emplace(toggleTarget);
}

IGsToggleGroupEntry* FGsToggleGroup::GetEntry(int32 InIndex)
{
	if (_toggleList.IsValidIndex(InIndex))
	{
		TWeakObjectPtr<UWidget> widget = _toggleList[InIndex];
		if (widget.IsValid())
		{
			return Cast<IGsToggleGroupEntry>(widget.Get());
		}
	}

	return nullptr;
}

void FGsToggleGroup::SetSelectedIndex(int32 InIndex, bool bInForced)
{
	if (IsBlockedIndex(InIndex))
		return;

	int32 prevIndex = _selectedIndex;
	_selectedIndex = InIndex;
	
	// 항상 하나만 선택되도록 처리한다
	ProcessSelectOnlyOne(InIndex);

	// 강제로 이벤트를 발생시킬 때
	if (bInForced)
	{
		if (prevIndex != _selectedIndex)
		{
			OnLeavePreSelected.ExecuteIfBound(prevIndex);
		}

		OnSelectChanged.ExecuteIfBound(_selectedIndex);
	}
	else
	{
		// 달라졌을 때
		if (prevIndex != _selectedIndex)
		{
			OnLeavePreSelected.ExecuteIfBound(prevIndex);
			OnSelectChanged.ExecuteIfBound(_selectedIndex);
		}
	}
}

void FGsToggleGroup::SetSelectedIndexNoEvent(int32 InIndex)
{
	_selectedIndex = InIndex;

	// 항상 하나만 선택되도록 처리한다
	ProcessSelectOnlyOne(InIndex);
}

void FGsToggleGroup::OnToggleWidgetStateChanged(int32 InIndex, bool bIsSelected)
{
	if (bIsSelected)
	{
		if (InIndex != _selectedIndex)
		{
			SetSelectedIndex(InIndex);
		}
	}
	else
	{
		// 동일한 것을 해제 못하게 만든다
		if (InIndex == _selectedIndex)
		{
			if (IGsToggleGroupEntry* toggle = GetEntry(InIndex))
			{
				toggle->SetIsSelected(true);
			}

			OnNotChangeEvent.ExecuteIfBound();
		}
	}
}

void FGsToggleGroup::ProcessSelectOnlyOne(int32 InIndex)
{
	for (int32 i = 0; i < _toggleList.Num(); ++i)
	{
		TWeakObjectPtr<UWidget> widget = _toggleList[i];
		if (widget.IsValid())
		{
			if (IGsToggleGroupEntry* toggle = Cast<IGsToggleGroupEntry>(widget.Get()))
			{
				if (i == InIndex)
				{
					toggle->SetIsSelected(true);
				}
				else
				{
					toggle->SetIsSelected(false);
				}
			}
		}
	}
}

void FGsToggleGroup::AddBlockedIndex(int32 InIndex)
{
	if (_toggleList.IsValidIndex(InIndex))
	{
		if (false == _blockedIndexSet.Contains(InIndex))
		{
			_blockedIndexSet.Emplace(InIndex);
		}
	}
}

void FGsToggleGroup::DeleteBlockedIndex(int32 InIndex)
{
	if (_blockedIndexSet.Contains(InIndex))
	{
		_blockedIndexSet.Remove(InIndex);
	}
}

bool FGsToggleGroup::IsBlockedIndex(int32 InIndex)
{
	if (_blockedIndexSet.Contains(InIndex))
	{
		return true;
	}
	return false;
}

void FGsToggleGroup::SetVisibilityEntry(int32 InIndex, ESlateVisibility InVisibility)
{
	if (_toggleList.IsValidIndex(InIndex))
	{
		_toggleList[InIndex]->SetVisibility(InVisibility);
	}
}
