// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIDropDownMenuCommon.h"
#include "GsUIDropDownMenuEntry.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib//ContentWidget/GsButton.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "ForEach.h"
#include "Transform.h"

using MenuEntry = TPair<int, const UGsUIDropDownMenuEntry*>;

void UGsUIDropDownMenuCommon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RemoveAll();
}

void UGsUIDropDownMenuCommon::NativeConstruct()
{
	Super::NativeConstruct();

	_btnMenuOn->OnClicked.AddDynamic(this, &UGsUIDropDownMenuCommon::OnMenuPopup);
}

void UGsUIDropDownMenuCommon::NativeDestruct()
{
	RemoveAll();

	_btnMenuOn->OnClicked.RemoveDynamic(this, &UGsUIDropDownMenuCommon::OnMenuPopup);

	Super::NativeDestruct();
}

void UGsUIDropDownMenuCommon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UGsUIDropDownMenuCommon::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (IsOpenPopup()) _mouseLeaveTime = 0.f;
}

void UGsUIDropDownMenuCommon::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (_useOutClose && IsOpenPopup()) _mouseLeaveTime = 0.5f;
}

void UGsUIDropDownMenuCommon::RemoveAll()
{
	Algo::ForEach(_reservedData, [this](auto& e) { OnRemovedEntry.ExecuteIfBound(e); });

	_menuPanel = _menuPanel == nullptr ? FindMenuPanel(_usePanel) : _menuPanel;

	TArray<UWidget*> menuItems = _menuPanel->GetAllChildren();
	for (auto child : menuItems)
	{
		if (auto widget = Cast<UGsUIDropDownMenuEntry>(child))
		{
			widget->OnSelectedMenu.Unbind();
			child->RemoveFromParent();
		}
	};

	_reservedData.Reset();
	_menuSlots.Reset();
}


UPanelWidget* UGsUIDropDownMenuCommon::FindMenuPanel(UPanelWidget* panel)
{
	if (panel == nullptr) return nullptr;

	TArray<UWidget*> menuItems = panel->GetAllChildren();
	for (auto child : menuItems)
	{
		if (child->IsA<UGsUIDropDownMenuEntry>())
		{
			return panel;
		}
		else if (child->IsA<UPanelWidget>())
		{
			return FindMenuPanel(Cast<UPanelWidget>(child));
		}
	};

	return _usePanel;
}

void UGsUIDropDownMenuCommon::SetMenuEntryColorProperty(UGsUIDropDownMenuEntry* Entry, FLinearColor enableColor, FLinearColor selectColor /*= FLinearColor::Transparent*/, FLinearColor disableColor /*= FLinearColor::Transparent*/)
{
#if WITH_EDITOR
	if (Entry)
	{
		Entry->SetMenuNormalColor(enableColor);

		if (selectColor != FLinearColor::Transparent)
			Entry->SetMenuSelectColor(selectColor);
		if (disableColor != FLinearColor::Transparent)
			Entry->SetMenuDisableColor(disableColor);
	}
#endif
}

void UGsUIDropDownMenuCommon::OnMenuPopup()
{
	if (_useMenuPanel)
	{
		if (_btnMenuOn->GetIsSelected())
		{
			CloseMenuPopup();
		}
		else
		{
			OpenMenuPopup();
		}
	}
}

void UGsUIDropDownMenuCommon::InitializeMenu(int32 Count, int32 init)
{
	if (_entryWidget == nullptr) return;

	RemoveAll();

	_menuPanel = _menuPanel == nullptr ? FindMenuPanel(_usePanel) : _menuPanel;

	auto slots = _menuPanel->GetSlots();
	auto slotCount = slots.Num();

	for (int index = 0; index < Count; index++)
	{
		if (auto widget = Cast<UGsUIDropDownMenuEntry>(CreateWidget(GetWorld(), _entryWidget)))
		{
			_reservedData.EmplaceAt(index, FMenuData());
			OnCreateEntry.ExecuteIfBound(index, _reservedData[index]);

			widget->SetMenuName(_reservedData[index]._menuName);
			if (_reservedData[index]._menuColor != FLinearColor::Transparent)
			{
				widget->SetMenuNormalColor(_reservedData[index]._menuColor);
			}
			else
			{
				_reservedData[index]._menuColor = widget->GetMenuColor();
			}

			if (_reservedData[index]._menuSelectColor != FLinearColor::Transparent)
				widget->SetMenuSelectColor(_reservedData[index]._menuSelectColor);
			if (_reservedData[index]._menuDisableColor != FLinearColor::Transparent)
				widget->SetMenuDisableColor(_reservedData[index]._menuDisableColor);

			widget->SetMenuIconPaths(_reservedData[index]._menuIcon, _reservedData[index]._imgSize, _reservedData[index]._imgTintColor);
			widget->SetMenuData(_reservedData[index]._data);
			widget->OnSelectedMenu.BindUObject(this, &UGsUIDropDownMenuCommon::OnSelectedMenuEntry);
			widget->InitEntry();

			_menuSlots.Emplace(_menuPanel->AddChild(widget));
			_reservedData[index]._content = widget;
		}
	}

	_selectedIndex = -1;
	_btnMenuOn->SetIsSelected(false);
	if (init >= 0)
	{
		SetSelectMenuEntry(init, true);
	}
	CloseMenuPopup();
}

void UGsUIDropDownMenuCommon::RefreshUI()
{
	auto slots = _menuPanel->GetSlots();
	auto slotCount = slots.Num();

	for (int index = 0; index < slotCount; index++)
	{
		if (!_reservedData.IsValidIndex(index)) continue;

		if (auto widget = Cast<UGsUIDropDownMenuEntry>(_reservedData[index]._content))
		{
			widget->SetMenuName(_reservedData[index]._menuName);
			widget->SetMenuNormalColor(_reservedData[index]._menuColor);

			if (_reservedData[index]._menuSelectColor != FLinearColor::Transparent)
				widget->SetMenuSelectColor(_reservedData[index]._menuSelectColor);
			if (_reservedData[index]._menuDisableColor != FLinearColor::Transparent)
				widget->SetMenuDisableColor(_reservedData[index]._menuDisableColor);

			widget->SetMenuIconPaths(_reservedData[index]._menuIcon, _reservedData[index]._imgSize, _reservedData[index]._imgTintColor);
			widget->SetMenuData(_reservedData[index]._data); 

			widget->RefreshEntry();
		}
	}
}

void UGsUIDropDownMenuCommon::OnSelectedMenuEntry(const UGsUIDropDownMenuEntry* Entry)
{
	auto menuIndex = _menuSlots.IndexOfByKey(Entry->Slot);
	menuIndex = OnSelectedEntry.IsBound() ? (OnSelectedEntry.Execute(menuIndex, false) ? menuIndex : _selectedIndex) : menuIndex;

	if (menuIndex != _selectedIndex)
	{
		_selectedIndex = menuIndex;

		Algo::ForEach(_menuSlots, [Entry](auto e) {
			auto widget = Cast<UGsUIDropDownMenuEntry>(e->Content);
			widget->SetIsSelected(Entry->Slot == e);
			});

		_selectedName = Entry->GetMenuName();
		_selectedColor = Entry->GetMenuColor();
		if (const auto obj = Entry->GetMenuIcon().GetResourceObject())
		{
			_selectediconBrush.TintColor = Entry->GetMenuIconTintColor();
			_selectediconBrush.SetImageSize(Entry->GetMenuIconImageSize());
			_selectediconBrush.SetResourceObject(obj);
		}
	}

	OnMenuPopup();
}

void UGsUIDropDownMenuCommon::SetSelectMenuEntry(int32 index, bool init)
{
	if (_menuSlots.IsValidIndex(index))
	{
		_selectedIndex = index;
		auto slot = _menuSlots[index];

		auto menuItems = _menuPanel->GetAllChildren();
		Algo::ForEach(_menuSlots, [slot](auto e) {
			auto widget = Cast<UGsUIDropDownMenuEntry>(e->Content);
			widget->SetIsSelected(slot == e);
			});

		if (auto widget = Cast<UGsUIDropDownMenuEntry>(slot->Content))
		{
			_selectedName = widget->GetMenuName();
			_selectedColor = widget->GetMenuColor();

			if (const auto obj = widget->GetMenuIcon().GetResourceObject())
			{
				_selectediconBrush.TintColor = widget->GetMenuIconTintColor();
				_selectediconBrush.SetImageSize(widget->GetMenuIconImageSize());
				_selectediconBrush.SetResourceObject(obj);
			}
		}

		if (OnSelectedEntry.IsBound() && !init) OnSelectedEntry.Execute(index, true);
	}
	else if (index == -1)
	{
		auto menuItems = _menuPanel->GetAllChildren();
		Algo::ForEach(_menuSlots, [](auto e) {
			auto widget = Cast<UGsUIDropDownMenuEntry>(e->Content);
			widget->SetIsSelected(false);
		});
	}
}

void UGsUIDropDownMenuCommon::SetEnableMenuEntry(int32 index, bool set, bool collapsed /*= false*/)
{
	if (_menuSlots.IsValidIndex(index))
	{
		if (!set && _selectedIndex == index) _selectedIndex = -1;

		auto slot = _menuSlots[index];
		if (auto widget = Cast<UGsUIDropDownMenuEntry>(slot->Content))
		{
			if (set == false && collapsed)
				 widget->SetVisibility(ESlateVisibility::Collapsed);
			else widget->SetVisibility(ESlateVisibility::Visible);

			widget->SetMenuEnabled(set);
		}
	}
}

const FMenuData& UGsUIDropDownMenuCommon::GetMenuEntryData(int32 index) const
{
	static FMenuData emptyEntry;

	if (_reservedData.IsValidIndex(index))
		return _reservedData[index];

	return emptyEntry;
}

const FText UGsUIDropDownMenuCommon::GetMenuEntryName(int32 index) const
{
	if (_reservedData.IsValidIndex(index))
	{
		return _reservedData[index]._menuName;
	}

	return FText::GetEmpty();
}

const FLinearColor UGsUIDropDownMenuCommon::GetMenuEntryColor(int32 index) const
{
	if (_reservedData.IsValidIndex(index))
	{
		return _reservedData[index]._menuColor;
	}

	return FLinearColor::Transparent;
}

const FSoftObjectPath UGsUIDropDownMenuCommon::GetMenuEntryIconPath(int32 index) const
{
	if (_reservedData.IsValidIndex(index))
	{
		return _reservedData[index]._menuIcon;
	}

	return FSoftObjectPath();
}

TWeakPtr<IMenuData> UGsUIDropDownMenuCommon::GetMenuEntryOptionData(int32 index)
{
	if (_reservedData.IsValidIndex(index))
	{
		return _reservedData[index]._data;
	}

	return nullptr;
}

void UGsUIDropDownMenuCommon::SetMenuEntryName(int32 index, FText name)
{
	if (_menuSlots.IsValidIndex(index))
	{
		auto slot = _menuSlots[index];
		if (auto widget = Cast<UGsUIDropDownMenuEntry>(slot->Content))
		{
			if(_reservedData.IsValidIndex(index))
				_reservedData[index]._menuName = name;
			widget->SetMenuName(name);
		}
	}
}

void UGsUIDropDownMenuCommon::SetMenuEntryColor(int32 index, FLinearColor enableColor, FLinearColor selectColor /*= FLinearColor::Transparent*/, FLinearColor disableColor /*= FLinearColor::Transparent*/)
{
	if (_menuSlots.IsValidIndex(index))
	{
		auto slot = _menuSlots[index];
		if (auto widget = Cast<UGsUIDropDownMenuEntry>(slot->Content))
		{
			if (enableColor != FLinearColor::Transparent)  _reservedData[index]._menuColor = enableColor;
			if (selectColor != FLinearColor::Transparent) _reservedData[index]._menuSelectColor = selectColor;
			if (disableColor != FLinearColor::Transparent) _reservedData[index]._menuDisableColor = disableColor;

			if (_reservedData[index]._menuColor != FLinearColor::Transparent)
				widget->SetMenuNormalColor(_reservedData[index]._menuColor);

			if (_reservedData[index]._menuSelectColor != FLinearColor::Transparent)
				widget->SetMenuSelectColor(_reservedData[index]._menuSelectColor);

			if (_reservedData[index]._menuDisableColor != FLinearColor::Transparent)
				widget->SetMenuDisableColor(_reservedData[index]._menuDisableColor);
		}
	}
}

void UGsUIDropDownMenuCommon::SetMenuEntryIconPath(int32 index, FSoftObjectPath path, FVector2D imgSize /*= FVector2D::ZeroVector*/, FLinearColor tintColor /*= FLinearColor::Transparent*/)
{
	if (_menuSlots.IsValidIndex(index))
	{
		auto slot = _menuSlots[index];
		if (auto widget = Cast<UGsUIDropDownMenuEntry>(slot->Content))
		{
			_reservedData[index]._menuIcon = path;
			_reservedData[index]._imgSize = imgSize;
			_reservedData[index]._imgTintColor = tintColor;
		}
	}
}

void UGsUIDropDownMenuCommon::SetMenuEntryOptionData(int32 index, TSharedPtr<IMenuData> data)
{
	if (_menuSlots.IsValidIndex(index))
	{
		auto slot = _menuSlots[index];
		if (auto widget = Cast<UGsUIDropDownMenuEntry>(slot->Content))
		{
			if (_reservedData.IsValidIndex(index))
				_reservedData[index]._data = data;
			widget->SetMenuData(data);
		}
	}
}

void UGsUIDropDownMenuCommon::OpenMenuPopup()
{
	_mouseLeaveTime = 0.f;
	_btnMenuOn->SetIsSelected(true);
	_usePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIDropDownMenuCommon::CloseMenuPopup()
{
	_btnMenuOn->SetIsSelected(false);
	_usePanel->SetVisibility(ESlateVisibility::Collapsed);
}

bool UGsUIDropDownMenuCommon::IsOpenPopup()
{
	return (_usePanel->GetVisibility() != ESlateVisibility::Collapsed &&
		_usePanel->GetVisibility() != ESlateVisibility::Hidden);
}