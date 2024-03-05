// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIChatTabUseSubTab.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UGsUIChatTabUseSubTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIChatTabUseSubTab::NativeConstruct()
{
	Super::NativeConstruct();

	_selectedName = _textTabName;
	_mainBtnSwitcher->SetActiveWidgetIndex(0);
	SetUseMouseOutClose(false);
}

void UGsUIChatTabUseSubTab::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIChatTabUseSubTab::SynchronizeProperties()
{
	Super::SynchronizeProperties();

#if WITH_EDITOR

	_selectedName = _textTabName;
	if (_textSelect)
	{
	//	_textSelect->SetText(_selectedName); // chat bindingÀÌ ±úÁø´Ù
		_textSelect->SetColorAndOpacity(_tabSelectColor);
		_textSelect->SynchronizeProperties();
	}
	if (_textBase)
	{
	//	_textBase->SetText(_selectedName);
		_textBase->SetColorAndOpacity(_tabBaseColor);
		_textBase->SynchronizeProperties();
	}
	if (_textDisable)
	{
	//	_textDisable->SetText(_selectedName);
		_textDisable->SetColorAndOpacity(_tabDisableColor);
		_textDisable->SynchronizeProperties();
	}
	
	_usePanel->SetVisibility(UseSubTab ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_mainBtnSwitcher->SetVisibility(UseSubTab ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (UseSubTab)
	{
		_menuPanel = _menuPanel == nullptr ? FindMenuPanel(_usePanel) : _menuPanel;
	}

	if (_menuPanel)
	{
		auto children = _menuPanel->GetAllChildren();

		int index = 0;
		for (auto child : children)
		{
			if (auto entry = Cast<UGsUIDropDownMenuEntry>(child))
			{
				SetMenuEntryColorProperty(entry, _tabBaseColor, _tabSelectColor, _tabDisableColor);
				entry->SynchronizeProperties();
			}
		};
	}
#endif

	_btnMenuOn->_slotIndexSelected = _slotIndexSelected;
	_btnMenuOn->_slotIndexUnselected = _slotIndexUnselected;
	_btnMenuOn->SynchronizeProperties();
}

void UGsUIChatTabUseSubTab::OnMenuPopup()
{
	if (UseSubTab == false)
	{
		if (_toggleCallback)
		{
			_toggleCallback(_indexInGroup, !_btnMenuOn->GetIsSelected());
		}
	}
	else
	{
		int curIndex = _btnMenuOn->GetSwitcherIndex();

		if (curIndex == _slotIndexSelected) //tab selected
		{
			if (IsOpenPopup())
			{
				_mainBtnSwitcher->SetActiveWidgetIndex(0);
				CloseMenuPopup();
			}
			else
			{
				_mainBtnSwitcher->SetActiveWidgetIndex(1);
				OpenMenuPopup();
			}

			SetIsSelected(true);
		}
		else if (curIndex == _slotIndexUnselected) //tab unselected, Menu Off
		{
			if (_selectedIndex < 0)
			{
				_mainBtnSwitcher->SetActiveWidgetIndex(1);
				OpenMenuPopup();
			}

			SetIsSelected(true);
		}

		if (_toggleCallback)
		{
			curIndex = _btnMenuOn->GetSwitcherIndex();
			_toggleCallback(_indexInGroup, curIndex == _slotIndexSelected);
		}
	}
}

void UGsUIChatTabUseSubTab::SetIsSelected(bool bInIsSelected)
{
	if (UseSubTab && _useMenuPanel && !bInIsSelected)
	{
		_mainBtnSwitcher->SetActiveWidgetIndex(0);
		CloseMenuPopup();
	}
	_btnMenuOn->SetIsSelected(bInIsSelected);
}

bool UGsUIChatTabUseSubTab::GetIsSelected() const
{
	return _btnMenuOn->GetIsSelected();
}

void UGsUIChatTabUseSubTab::InitializeMenu(int32 Count, int32 init)
{
	Super::InitializeMenu(Count, init);

	_selectedIndex = init >= 0 ? _selectedIndex : -1;
	_selectedName  = _textTabName;
	_selectedColor = _tabBaseColor;

	auto count = _menuSlots.Num();
	for (auto index = 0; index < count; index++)
	{
		SetMenuEntryColor(index, _tabBaseColor, _tabSelectColor, _tabDisableColor);
	}

	SetSelectMenuEntry(-1);

	SetMenuPanelActive(Count > 0);
}

void UGsUIChatTabUseSubTab::RefreshUI()
{
	Super::RefreshUI();
}

void UGsUIChatTabUseSubTab::SetSwitcherIndex(int32 InIndex)
{
	_btnMenuOn->SetSwitcherIndex(InIndex);
}

int32 UGsUIChatTabUseSubTab::GetSwitcherIndex() const
{
	return _btnMenuOn->GetSwitcherIndex();
}

FLinearColor UGsUIChatTabUseSubTab::GetTabColor()
{
	if (GetIsEnabled()) return _tabDisableColor;
	else if (GetIsSelected()) return _tabSelectColor;

	return _tabBaseColor;
}
