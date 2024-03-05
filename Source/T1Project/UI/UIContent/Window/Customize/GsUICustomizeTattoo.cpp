// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizeTattoo.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeImageListEntry.h"
#include "DataSchema/Customize/GsSchemaCustomizeMenu.h"
#include "DataSchema/Customize/GsSchemaCustomizeTattooGroup.h"
#include "GameObject/Customize/GsCustomizeHandler.h"


void UGsUICustomizeTattoo::BeginDestroy()
{
	if (_slotHelperTattoo)
	{
		_slotHelperTattoo->OnRefreshEntry.RemoveDynamic(this, &UGsUICustomizeTattoo::OnRefreshTattoo);
	}
	_slotHelperTattoo = nullptr;

	Super::BeginDestroy();
}

void UGsUICustomizeTattoo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperTattoo = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperTattoo->Initialize(_tattooEntryWidgetClass, _panelTattoo);
	_slotHelperTattoo->OnRefreshEntry.AddDynamic(this, &UGsUICustomizeTattoo::OnRefreshTattoo);

	_toggleGroupTattoo.OnSelectChanged.BindUObject(this, &UGsUICustomizeTattoo::OnSelectTattoo);
}

void UGsUICustomizeTattoo::NativeDestruct()
{
	_tattooList = nullptr;

	Super::NativeDestruct();
}

void UGsUICustomizeTattoo::Show()
{
	if (nullptr == _selectedData)
	{
		return;
	}

	_tattooList = nullptr;
	if (const FGsSchemaCustomizeMenu* menu = _selectedData->GetMenu())
	{
		if (const FGsSchemaCustomizeTattooGroup* tattooGroup = menu->tattooGroup.GetRow())
		{
			_tattooList = &tattooGroup->slotList;
		}
	}
	
	UWidget* selectedWidget = nullptr;
	int32 selectedIndex = _selectedData->GetValue(EGsUICustomizeType::TATTOO_INDEX);
	if (_tattooList)
	{
		_slotHelperTattoo->RefreshAll(_tattooList->Num());

		selectedWidget = _slotHelperTattoo->GetEntry(selectedIndex);
	}
	else
	{
		_slotHelperTattoo->RefreshAll(0);		
	}	

	_toggleGroupTattoo.Clear();
	_toggleGroupTattoo.AddToggleWidgetByParentPanel(_panelTattoo);
	_toggleGroupTattoo.SetSelectedIndex(selectedIndex);

	if (selectedWidget)
	{
		_scroll->ScrollWidgetIntoView(selectedWidget);
	}
	else
	{
		_scroll->ScrollToStart();
	}
}

void UGsUICustomizeTattoo::OnRefreshTattoo(int32 InIndex, UWidget* InEntry)
{
	if (_tattooList &&
		_tattooList->IsValidIndex(InIndex))
	{
		UGsUICustomizeImageListEntry* entry = Cast<UGsUICustomizeImageListEntry>(InEntry);
		if (entry)
		{
			entry->SetData(_tattooList->GetData()[InIndex].iconPath);
		}
	}
}

void UGsUICustomizeTattoo::OnSelectTattoo(int32 InIndex)
{
	if (_selectedData)
	{
		_selectedData->SetValue(EGsUICustomizeType::TATTOO_INDEX, InIndex);
		
		if (_customizeHandler)
		{
			_customizeHandler->UpdateCustomizeUIData(_selectedData);
			_customizeHandler->SetTattooChange(_selectedData->GetTattooId());
		}
	}
}
