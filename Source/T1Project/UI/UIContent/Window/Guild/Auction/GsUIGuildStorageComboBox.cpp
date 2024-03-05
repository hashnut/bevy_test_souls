// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildStorageComboBox.h"

#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Window/Guild/Auction/GsUIGuildStorageComboBoxEntry.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildStorageComboBoxSideEntry.h"


void UGsUIGuildStorageComboBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperMain = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperMain->Initialize(_entryClassMain, _panelMainList);
	_slotHelperMain->OnRefreshEntry.AddDynamic(this, &UGsUIGuildStorageComboBox::OnRefreshMainEntry);

	_slotHelperSub = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSub->Initialize(_entryClassSub, _panelSubList);
	_slotHelperSub->OnRefreshEntry.AddDynamic(this, &UGsUIGuildStorageComboBox::OnRefreshSubEntry);

	_btnMenu->OnClicked.AddDynamic(this, &UGsUIGuildStorageComboBox::OnClickMenu);
	_toggleGroupMain.OnSlotClicked.BindUObject(this, &UGsUIGuildStorageComboBox::OnClickListMainEntry);
	_toggleGroupSub.OnSlotClicked.BindUObject(this, &UGsUIGuildStorageComboBox::OnClickListSubEntry);
}

void UGsUIGuildStorageComboBox::NativeConstruct()
{
	Super::NativeConstruct();

	Close();
}

void UGsUIGuildStorageComboBox::NativeDestruct()
{
	Clear();

	Super::NativeDestruct();
}

void UGsUIGuildStorageComboBox::Clear()
{
	_mainMenuList.Empty();
	_subMenuListMap.Empty();
	_toggleGroupMain.Clear();
	_toggleGroupSub.Clear();
	_baseMenuText = FText::GetEmpty();
}

void UGsUIGuildStorageComboBox::StartAddMenu()
{
	_mainMenuList.Empty();
	_subMenuListMap.Empty();
}

void UGsUIGuildStorageComboBox::SetBaseText(const FText& InText)
{
	_baseMenuText = InText;
}

void UGsUIGuildStorageComboBox::AddMenu(const FText& InMenuText)
{
	_mainMenuList.Emplace(InMenuText);
}

void UGsUIGuildStorageComboBox::AddMenuWithSubMenu(const FText& InMenuText, const TArray<FText>& InSubMenuTextList)
{
	_mainMenuList.Emplace(InMenuText);
	_subMenuListMap.Emplace(_mainMenuList.Num() - 1, InSubMenuTextList);
}

void UGsUIGuildStorageComboBox::FinishAddMenu()
{
	_slotHelperMain->RefreshAll(_mainMenuList.Num());

	_toggleGroupMain.Clear();
	_toggleGroupMain.AddToggleWidgetByParentPanel(_panelMainList);
	_toggleGroupMain.SetSelectedIndexNoEvent(0);
	_lastSelectedIndex = 0;
	
	SetMainMenuText(0);
}

void UGsUIGuildStorageComboBox::SetSelectedIndex(int32 InMainIndex, int32 InSubIndex)
{
	_lastSelectedIndex = InMainIndex;

	// 사이드 탭을 사용하는 콤보박스
	if (_subMenuListMap.Contains(InMainIndex))
	{
		SetMainMenuText(InMainIndex);
		//SetSubMenuText(InSubIndex);
	}
	// 사이드 탭을 사용하지 않는 콤보박스
	else
	{
		_toggleGroupMain.SetSelectedIndexNoEvent(InMainIndex);
		SetMainMenuText(InMainIndex);
	}
}

void UGsUIGuildStorageComboBox::Close()
{
	ShowList(false);
	ShowSubList(false);
}

void UGsUIGuildStorageComboBox::ShowList(bool bInShow)
{
	if (bInShow)
	{
		_panelMain->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_panelSub->SetVisibility(ESlateVisibility::Collapsed);
		_btnMenu->SetIsSelected(false);
	}
	else
	{
		_panelMain->SetVisibility(ESlateVisibility::Collapsed);
		_panelSub->SetVisibility(ESlateVisibility::Collapsed);
		_btnMenu->SetIsSelected(true);
	}
}

void UGsUIGuildStorageComboBox::ShowSubList(bool bInShow, int32 InMainIndex /*= 0*/)
{
	if (bInShow && _subMenuListMap.Contains(InMainIndex))
	{
		_panelSub->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_slotHelperSub->RefreshAll(_subMenuListMap[InMainIndex].Num());

		_toggleGroupSub.Clear();
		_toggleGroupSub.AddToggleWidgetByParentPanel(_panelSubList);
		_toggleGroupSub.SetSelectedIndexNoEvent(-1);
	}
	else
	{
		_panelSub->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIGuildStorageComboBox::SetMainMenuText(int32 InIndex)
{
	if (_baseMenuText.IsEmpty())
	{
		if (_mainMenuList.IsValidIndex(InIndex))
		{
			_textBlockMenu->SetText(_mainMenuList[InIndex]);
		}
	}
	else
	{
		if (_mainMenuList.IsValidIndex(InIndex))
		{
			FText menuText = FText::Format(_baseMenuText, _mainMenuList[InIndex]);
			_textBlockMenu->SetText(menuText);
		}
		else
		{
			_textBlockMenu->SetText(_baseMenuText);
		}
	}
}

void UGsUIGuildStorageComboBox::SetSubMenuText(int32 InIndex)
{
	int32 selectedMainIndex = _toggleGroupMain.GetSelectedIndex();

	if (_baseMenuText.IsEmpty())
	{
		if (_subMenuListMap.Contains(selectedMainIndex))
		{
			const TArray<FText>& subMenuList = _subMenuListMap[selectedMainIndex];
			if (subMenuList.Num() > 0)
			{
				if (subMenuList.IsValidIndex(InIndex))
				{
					_textBlockMenu->SetText(subMenuList[InIndex]);
				}
			}
			else
			{
				SetMainMenuText(selectedMainIndex);
			}
		}
	}
	else
	{
		if (_subMenuListMap.Contains(selectedMainIndex))
		{
			const TArray<FText>& subMenuList = _subMenuListMap[selectedMainIndex];
			if (subMenuList.Num() > 0)
			{
				if (subMenuList.IsValidIndex(InIndex))
				{
					FText menuText = FText::Format(_baseMenuText, subMenuList[InIndex]);
					_textBlockMenu->SetText(menuText);
				}
			}
			else
			{
				SetMainMenuText(selectedMainIndex);
			}
		}
	}
}

void UGsUIGuildStorageComboBox::OnClickMenu()
{
	_toggleGroupMain.SetSelectedIndexNoEvent(_lastSelectedIndex);

	ShowList(_panelMain->IsVisible() ? false : true);

	OnClickComboMenu.ExecuteIfBound();
}

void UGsUIGuildStorageComboBox::OnRefreshMainEntry(int32 InIndex, class UWidget* InEntry)
{
	if (false == _mainMenuList.IsValidIndex(InIndex))
		return;

	if (UGsUIGuildStorageComboBoxEntry* entry = Cast<UGsUIGuildStorageComboBoxEntry>(InEntry))
	{
		entry->SetNameText(_mainMenuList[InIndex]);
	}
}

void UGsUIGuildStorageComboBox::OnRefreshSubEntry(int32 InIndex, class UWidget* InEntry)
{
	int32 selectedMainIndex = _toggleGroupMain.GetSelectedIndex();
	if (false == _subMenuListMap.Contains(selectedMainIndex))
		return;

	const TArray<FText>& subMenuList = _subMenuListMap[selectedMainIndex];
	if (false == subMenuList.IsValidIndex(InIndex))
		return;

	if (UGsUIGuildStorageComboBoxSideEntry* entry = Cast<UGsUIGuildStorageComboBoxSideEntry>(InEntry))
	{
		entry->SetNameText(subMenuList[InIndex]);
	}
}

void UGsUIGuildStorageComboBox::OnClickListMainEntry(int32 InIndex, bool bInIsSelected)
{
	// 사이드 탭을 사용하는 콤보박스
	if (_subMenuListMap.Contains(InIndex))
	{
		if (_subMenuListMap[InIndex].Num() > 0)
		{
			ShowSubList(true, InIndex);
			_toggleGroupMain.SetSelectedIndexNoEvent(InIndex);
		}
		else
		{
			_lastSelectedIndex = InIndex;
			OnSelectComboBoxSub.ExecuteIfBound(InIndex, 0);
			SetMainMenuText(InIndex);
			Close();
		}
	}
	// 사이드 탭을 사용하지 않는 콤보박스
	else
	{
		if (_mainMenuList.IsValidIndex(InIndex))
		{
			_lastSelectedIndex = InIndex;
			_toggleGroupMain.SetSelectedIndexNoEvent(InIndex);
			OnSelectComboBoxMain.ExecuteIfBound(InIndex);
			SetMainMenuText(InIndex);
			Close();
		}
	}
}

void UGsUIGuildStorageComboBox::OnClickListSubEntry(int32 InIndex, bool bInIsSelected)
{
	int32 selectedMainIndex = _toggleGroupMain.GetSelectedIndex();

	if (_subMenuListMap.Contains(selectedMainIndex))
	{
		if (_subMenuListMap[selectedMainIndex].IsValidIndex(InIndex))
		{
			_lastSelectedIndex = selectedMainIndex;
			OnSelectComboBoxSub.ExecuteIfBound(selectedMainIndex, InIndex);
			//SetSubMenuText(InIndex);
			SetMainMenuText(selectedMainIndex);
		}
	}

	Close();
}
