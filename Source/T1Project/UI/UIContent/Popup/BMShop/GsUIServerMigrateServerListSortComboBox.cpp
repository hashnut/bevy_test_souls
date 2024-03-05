#include "GsUIServerMigrateServerListSortComboBox.h"
#include "../UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "ForEach.h"


void UGsUIServerMigrateServerListSortComboBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnSortMenu)
	{
		_btnSortMenu->OnClicked.AddDynamic(this, &UGsUIServerMigrateServerListSortComboBox::OnClickedMenu);
	}	

	if (_btnNormal)
	{
		_btnNormal->OnClicked.AddDynamic(this, &UGsUIServerMigrateServerListSortComboBox::OnClickedNormal);
	}

	if (_btnRecommended)
	{
		_btnRecommended->OnClicked.AddDynamic(this, &UGsUIServerMigrateServerListSortComboBox::OnClickedRecommended);
	}

	if (_btnSpacious)
	{
		_btnSpacious->OnClicked.AddDynamic(this, &UGsUIServerMigrateServerListSortComboBox::OnClickedSpacious);
	}
}

void UGsUIServerMigrateServerListSortComboBox::NativeConstruct()
{
	Super::NativeConstruct();

	_menuItems.Reset();

	_panelMenu->SetVisibility(ESlateVisibility::Collapsed);	
	_menuItems.Add(MenuType::Normal, _btnNormal);
	_menuItems.Add(MenuType::Recommended, _btnRecommended);
	_menuItems.Add(MenuType::Spacious, _btnSpacious);
}

void UGsUIServerMigrateServerListSortComboBox::OnClickedMenu()
{
	if (_panelMenu)
	{
		if (_btnSortMenu->GetIsSelected())
		{
			CloseMenuPopup();
		}
		else
		{
			OpenMenuPopup();
		}
	}
}

void UGsUIServerMigrateServerListSortComboBox::OnClickedNormal()
{
	_btnNormal->SetIsSelected(!_btnNormal->GetIsSelected());

	OnMenuSelected(MenuType::Normal, _btnNormal);
}

void UGsUIServerMigrateServerListSortComboBox::OnClickedRecommended()
{
	_btnRecommended->SetIsSelected(!_btnRecommended->GetIsSelected());

	OnMenuSelected(MenuType::Recommended, _btnRecommended);
}

void UGsUIServerMigrateServerListSortComboBox::OnClickedSpacious()
{
	_btnSpacious->SetIsSelected(!_btnSpacious->GetIsSelected());

	OnMenuSelected(MenuType::Spacious, _btnSpacious);
}

void UGsUIServerMigrateServerListSortComboBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsOpenPopup() && _mouseLeaveTime > 0.f)
	{
		_mouseLeaveTime -= InDeltaTime;

		if (_mouseLeaveTime <= 0.f)
		{
			CloseMenuPopup();
		}
	}
}

void UGsUIServerMigrateServerListSortComboBox::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (IsOpenPopup()) _mouseLeaveTime = 0.f;
}

void UGsUIServerMigrateServerListSortComboBox::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (IsOpenPopup()) _mouseLeaveTime = 0.5f;
}

void UGsUIServerMigrateServerListSortComboBox::OpenMenuPopup()
{
	_mouseLeaveTime = 0.f;
	_btnSortMenu->SetIsSelected(true);
	_panelMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIServerMigrateServerListSortComboBox::CloseMenuPopup()
{
	_btnSortMenu->SetIsSelected(false);
	_panelMenu->SetVisibility(ESlateVisibility::Collapsed);
}

bool UGsUIServerMigrateServerListSortComboBox::IsOpenPopup()
{
	return (_panelMenu->GetVisibility() != ESlateVisibility::Collapsed &&
			_panelMenu->GetVisibility() != ESlateVisibility::Hidden);
}

void UGsUIServerMigrateServerListSortComboBox::OnMenuSelected(MenuType type, UGsSwitcherButton* btn, bool noti)
{
	if (btn->GetIsSelected())
	{
		Algo::ForEach(_menuItems, [btn](auto& e) { e.Value->SetIsSelected(e.Value == btn); });
		
		if (noti)
		{
			OnSelectedMenu.ExecuteIfBound(type);
			OnClickedMenu();
		}
	}
}

void UGsUIServerMigrateServerListSortComboBox::SetMenuSelect(MenuType type)
{
	_menuItems[type]->SetIsSelected(true);
	OnMenuSelected(type, _menuItems[type], false);

	CloseMenuPopup();
}
