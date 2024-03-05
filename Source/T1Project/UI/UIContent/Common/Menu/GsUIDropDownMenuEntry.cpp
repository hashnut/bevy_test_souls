// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIDropDownMenuEntry.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "Image.h"
#include "Engine/AssetManager.h"

void UGsUIDropDownMenuEntry::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIDropDownMenuEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_disableIndex = _btnEntry->GetNumWidgets() - 1;
	_disableIndex = _disableIndex <= 2 ? -1 : _disableIndex;
}

void UGsUIDropDownMenuEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_backupMenuTextColor = _menuTextColor;
	_brushTintColor = _iconBrush.TintColor.GetSpecifiedColor();
	_brushImageSize = _iconBrush.GetImageSize();

	_btnEntry->OnClicked.AddDynamic(this, &UGsUIDropDownMenuEntry::OnClickMenu);
}

void UGsUIDropDownMenuEntry::NativeDestruct()
{
	_menuTextColor = _backupMenuTextColor;
	_iconBrush.TintColor = _brushTintColor;
	_iconBrush.SetImageSize(_brushImageSize);
	
	_btnEntry->OnClicked.RemoveDynamic(this, &UGsUIDropDownMenuEntry::OnClickMenu);

	Super::NativeDestruct();
}

void UGsUIDropDownMenuEntry::OnClickMenu()
{
	if (_isUseUnselecdNoti)
		 _btnEntry->SetIsSelected(!_btnEntry->GetIsSelected());
	else _btnEntry->SetIsSelected(true);

	OnSelectedEntry(_btnEntry->GetIsSelected());
}

void UGsUIDropDownMenuEntry::OnSelectedEntry(bool isSelected)
{
	_menuTextColor = isSelected ? _menuSelectColor :  _menuEnableColor ;
	if(isSelected) OnSelectedMenu.ExecuteIfBound(this);
}

void UGsUIDropDownMenuEntry::InitEntry()
{
	_btnEntry->SetIsEnabled(true);
	_menuTextColor = _menuEnableColor;
}

void UGsUIDropDownMenuEntry::RefreshEntry()
{

}

void UGsUIDropDownMenuEntry::SetMenuEnabled(bool set)
{
	_menuTextColor = set ? _menuEnableColor : _menuDisableColor;

	if (!set) _btnEntry->SetIsSelected(false);
	_btnEntry->SetIsEnabled(set);

	if(_disableIndex >= 0) _btnEntry->SetSwitcherIndex(_disableIndex);
}

void UGsUIDropDownMenuEntry::SetIsSelected(bool set)
{
	_menuTextColor = set ? _menuSelectColor : _btnEntry->GetIsEnabled() ? _menuEnableColor : _menuDisableColor;
	_btnEntry->SetIsSelected(set);
}

void UGsUIDropDownMenuEntry::SetMenuIconPaths(FSoftObjectPath path, FVector2D imgSize /*= FVector2D::ZeroVector*/, FLinearColor tintColor /*= FLinearColor::Transparent */)
{
	if (!path.IsValid()) return;

	if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(path))
	{
		_iconBrush.SetImageSize(imgSize == FVector2D::ZeroVector ? _brushImageSize : imgSize);
		_iconBrush.SetResourceObject(loadObject);

		_iconBrush.TintColor = tintColor != FLinearColor::Transparent ? tintColor : _brushTintColor;
	}
}

FVector2D UGsUIDropDownMenuEntry::GetMenuIconImageSize() const
{
	if (_iconBrush.GetResourceObject())
	{
		return _iconBrush.GetImageSize();
	}
	return _brushImageSize;
}

FLinearColor UGsUIDropDownMenuEntry::GetMenuIconTintColor() const
{
	if (_iconBrush.GetResourceObject())
	{
		return _iconBrush.TintColor.GetSpecifiedColor();
	}
	return _brushTintColor;
}
