// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBattlePassTab.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

void UGsUIBattlePassTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIBattlePassTab::SetToggleInfo(int groupId)
{
	_switchButton->InitializeToggle();
	_switchButton->SetIndexInGroup(groupId);
	_switchButton->SetToggleCallback([this](int32 InIndex, bool bInIsSelected) {
			if (_toggleCallback)
			{
				_toggleCallback(InIndex, bInIsSelected);
			}
		});
}

void UGsUIBattlePassTab::SetIsSelected(bool bInIsSelected)
{
	_isSelected = bInIsSelected;
	GetToggleGroupEntry()->SetIsSelected(bInIsSelected);
}

bool UGsUIBattlePassTab::GetIsSelected() const
{
	return _isSelected;
}

IGsToggleGroupEntry* UGsUIBattlePassTab::GetToggleGroupEntry()
{
	return _switchButton;
}