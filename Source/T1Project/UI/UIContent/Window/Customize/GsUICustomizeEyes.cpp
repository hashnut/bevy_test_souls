// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizeEyes.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeColorListEntry.h"
#include "DataSchema/Customize/GsSchemaCustomizeMenu.h"
#include "DataSchema/Customize/GsSchemaCustomizeColorGroup.h"
#include "GameObject/Customize/GsCustomizeHandler.h"


void UGsUICustomizeEyes::BeginDestroy()
{
	if (_slotHelperPupilColor)
	{
		_slotHelperPupilColor->OnRefreshEntry.RemoveDynamic(this, &UGsUICustomizeEyes::OnRefreshPupilColor);
		_slotHelperPupilColor = nullptr;
	}

	if (_slotHelperBrowColor)
	{
		_slotHelperBrowColor->OnRefreshEntry.RemoveDynamic(this, &UGsUICustomizeEyes::OnRefreshBrowColor);
		_slotHelperBrowColor = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUICustomizeEyes::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperPupilColor = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperPupilColor->Initialize(_colorEntryWidgetClass, _panelPupilColor);
	_slotHelperPupilColor->OnRefreshEntry.AddDynamic(this, &UGsUICustomizeEyes::OnRefreshPupilColor);

	_slotHelperBrowColor = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperBrowColor->Initialize(_colorEntryWidgetClass, _panelBrowColor);
	_slotHelperBrowColor->OnRefreshEntry.AddDynamic(this, &UGsUICustomizeEyes::OnRefreshBrowColor);

	_toggleGroupPupilColor.OnSelectChanged.BindUObject(this, &UGsUICustomizeEyes::OnSelectPupilColor);
	_toggleGroupBrowColor.OnSelectChanged.BindUObject(this, &UGsUICustomizeEyes::OnSelectBrowColor);
}

void UGsUICustomizeEyes::NativeDestruct()
{
	_pupilColorList.Empty();
	_browColorList.Empty();

	Super::NativeDestruct();
}

void UGsUICustomizeEyes::Show()
{
	if (nullptr == _selectedData)
	{
		return;
	}

	_pupilColorList.Empty();
	_browColorList.Empty();
	if (const FGsSchemaCustomizeMenu* menu = _selectedData->GetMenu())
	{
		if (const FGsSchemaCustomizeColorGroup* pupilColorGroup = menu->pupilColorGroup.GetRow())
		{
			GetBaseColorList(pupilColorGroup->colorIndexList, _pupilColorList);
		}

		if (const FGsSchemaCustomizeColorGroup* browColorGroup = menu->browColorGroup.GetRow())
		{
			GetBaseColorList(browColorGroup->colorIndexList, _browColorList);
		}
	}

	_slotHelperPupilColor->RefreshAll(_pupilColorList.Num());

	_toggleGroupPupilColor.Clear();
	_toggleGroupPupilColor.AddToggleWidgetByParentPanel(_panelPupilColor);
	_toggleGroupPupilColor.SetSelectedIndex(_selectedData->GetValue(EGsUICustomizeType::PUPIL_COLOR_INDEX));

	_slotHelperBrowColor->RefreshAll(_browColorList.Num());

	_toggleGroupBrowColor.Clear();
	_toggleGroupBrowColor.AddToggleWidgetByParentPanel(_panelBrowColor);
	_toggleGroupBrowColor.SetSelectedIndex(_selectedData->GetValue(EGsUICustomizeType::BROW_COLOR_INDEX));
}

void UGsUICustomizeEyes::OnRefreshPupilColor(int32 InIndex, UWidget* InEntry)
{
	if (_pupilColorList.IsValidIndex(InIndex))
	{
		UGsUICustomizeColorListEntry* entry = Cast<UGsUICustomizeColorListEntry>(InEntry);
		if (entry)
		{
			entry->SetColor(_pupilColorList[InIndex]);
		}
	}
}

void UGsUICustomizeEyes::OnRefreshBrowColor(int32 InIndex, UWidget* InEntry)
{
	if (_browColorList.IsValidIndex(InIndex))
	{
		UGsUICustomizeColorListEntry* entry = Cast<UGsUICustomizeColorListEntry>(InEntry);
		if (entry)
		{
			entry->SetColor(_browColorList[InIndex]);
		}
	}
}

void UGsUICustomizeEyes::OnSelectPupilColor(int32 InIndex)
{
	if (_selectedData)
	{
		_selectedData->SetValue(EGsUICustomizeType::PUPIL_COLOR_INDEX, InIndex);

		if (_customizeHandler)
		{
			_customizeHandler->UpdateCustomizeUIData(_selectedData);
			int32 colorIndex = _selectedData->GetPupilColorIndex();
			if (0 <= colorIndex)
			{
				_customizeHandler->SetPupilColorChange(colorIndex);
			}
		}
	}
}

void UGsUICustomizeEyes::OnSelectBrowColor(int32 InIndex)
{
	if (_selectedData)
	{
		_selectedData->SetValue(EGsUICustomizeType::BROW_COLOR_INDEX, InIndex);

		if (_customizeHandler)
		{
			_customizeHandler->UpdateCustomizeUIData(_selectedData);
			int32 colorIndex = _selectedData->GetBrowColorIndex();
			if (0 <= colorIndex)
			{
				_customizeHandler->SetBrowColorChange(colorIndex);
			}
		}
	}
}
