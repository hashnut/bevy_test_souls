// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizeHair.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeImageListEntry.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeColorListEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "DataSchema/Customize/GsSchemaCustomizeMenu.h"
#include "DataSchema/Customize/GsSchemaCustomizeColorGroup.h"
#include "DataSchema/Customize/GsSchemaCustomizePartsGroup.h"
#include "GameObject/Customize/GsCustomizeHandler.h"


void UGsUICustomizeHair::BeginDestroy()
{
	if (_slotHelperHair)
	{
		_slotHelperHair->OnRefreshEntry.RemoveDynamic(this, &UGsUICustomizeHair::OnRefreshHair);
		_slotHelperHair = nullptr;
	}

	if (_slotHelperHairColor)
	{
		_slotHelperHairColor->OnRefreshEntry.RemoveDynamic(this, &UGsUICustomizeHair::OnRefreshHairColor);
		_slotHelperHairColor = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUICustomizeHair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperHair = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperHair->Initialize(_hairEntryWidgetClass, _panelHair);
	_slotHelperHair->OnRefreshEntry.AddDynamic(this, &UGsUICustomizeHair::OnRefreshHair);

	_slotHelperHairColor = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperHairColor->Initialize(_colorEntryWidgetClass, _panelHairColor);
	_slotHelperHairColor->OnRefreshEntry.AddDynamic(this, &UGsUICustomizeHair::OnRefreshHairColor);

	_toggleGroupHair.OnSelectChanged.BindUObject(this, &UGsUICustomizeHair::OnSelectHair);
	_toggleGroupHairColor.OnSelectChanged.BindUObject(this, &UGsUICustomizeHair::OnSelectHairColor);
}

void UGsUICustomizeHair::NativeDestruct()
{
	_hairList = nullptr;
	_hairColorList.Empty();

	Super::NativeDestruct();
}

void UGsUICustomizeHair::Show()
{
	if (nullptr == _selectedData)
	{
		return;
	}

	// 머리 모양
	if (const FGsSchemaCustomizeMenu* menu = _selectedData->GetMenu())
	{
		if (const FGsSchemaCustomizePartsGroup* hairGroup = menu->hairPartsGroup.GetRow())
		{
			_hairList = &hairGroup->slotList;
			_slotHelperHair->RefreshAll(hairGroup->slotList.Num());
		}
	}

	_toggleGroupHair.Clear();
	_toggleGroupHair.AddToggleWidgetByParentPanel(_panelHair);
	_toggleGroupHair.SetSelectedIndex(_selectedData->GetValue(EGsUICustomizeType::HAIR_INDEX));


	// 헤어 컬러
	_hairColorList.Empty();
	if (const FGsSchemaCustomizeMenu* menu = _selectedData->GetMenu())
	{
		const FGsSchemaCustomizeColorGroup* colorGroup = menu->hairColorGroup.GetRow();
		if (colorGroup)
		{
			GetBaseColorList(colorGroup->colorIndexList, _hairColorList);
		}
	}
	
	_slotHelperHairColor->RefreshAll(_hairColorList.Num());

	_toggleGroupHairColor.Clear();
	_toggleGroupHairColor.AddToggleWidgetByParentPanel(_panelHairColor);
	_toggleGroupHairColor.SetSelectedIndex(_selectedData->GetValue(EGsUICustomizeType::HAIR_COLOR_INDEX));
}

void UGsUICustomizeHair::OnRefreshHair(int32 InIndex, UWidget* InEntry)
{
	if (_hairList && 
		_hairList->IsValidIndex(InIndex))
	{
		UGsUICustomizeImageListEntry* entry = Cast<UGsUICustomizeImageListEntry>(InEntry);
		if (entry)
		{
			entry->SetData(_hairList->GetData()[InIndex].iconPath);
		}
	}
}

void UGsUICustomizeHair::OnRefreshHairColor(int32 InIndex, UWidget* InEntry)
{
	if (_hairColorList.IsValidIndex(InIndex))
	{
		UGsUICustomizeColorListEntry* entry = Cast<UGsUICustomizeColorListEntry>(InEntry);
		if (entry)
		{
			entry->SetColor(_hairColorList[InIndex]);
		}
	}
}

void UGsUICustomizeHair::OnSelectHair(int32 InIndex)
{
	if (_selectedData)
	{
		_selectedData->SetValue(EGsUICustomizeType::HAIR_INDEX, InIndex);

		if (_customizeHandler)
		{
			_customizeHandler->UpdateCustomizeUIData(_selectedData);
			_customizeHandler->SetHairChange(_selectedData->GetHairId());

			// 주의: 현재 선택된 컬러로 다시 세팅해주어야 한다.(모델이 바뀌며 금발로 리셋됨)
			/*int32 colorIndex = _selectedData->GetHairColorIndex();
			if (0 <= colorIndex)
			{
				_customizeHandler->SetHairColorChange(colorIndex);
			}*/
		}
	}
}

void UGsUICustomizeHair::OnSelectHairColor(int32 InIndex)
{
	if (_selectedData)
	{
		_selectedData->SetValue(EGsUICustomizeType::HAIR_COLOR_INDEX, InIndex);

		if (_customizeHandler)
		{
			_customizeHandler->UpdateCustomizeUIData(_selectedData);
			int32 colorIndex = _selectedData->GetHairColorIndex();
			if (0 <= colorIndex)
			{
				_customizeHandler->SetHairColorChange(colorIndex);
			}
		}
	}
}
