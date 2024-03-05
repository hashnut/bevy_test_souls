// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizeFace.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeImageListEntry.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeColorListEntry.h"
#include "DataSchema/Customize/GsSchemaCustomizeMenu.h"
#include "DataSchema/Customize/GsSchemaCustomizeMenu.h"
#include "DataSchema/Customize/GsSchemaCustomizePartsGroup.h"
#include "DataSchema/Customize/GsSchemaCustomizeColorGroup.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "T1Project.h"


void UGsUICustomizeFace::BeginDestroy()
{
	if (_slotHelperFace)
	{
		_slotHelperFace->OnRefreshEntry.RemoveDynamic(this, &UGsUICustomizeFace::OnRefreshFace);
		_slotHelperFace = nullptr;
	}

	if (_slotHelperSkinColor)
	{
		_slotHelperSkinColor->OnRefreshEntry.RemoveDynamic(this, &UGsUICustomizeFace::OnRefreshSkinColor);
		_slotHelperSkinColor = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUICustomizeFace::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperFace = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperFace->Initialize(_faceEntryWidgetClass, _panelFace);
	_slotHelperFace->OnRefreshEntry.AddDynamic(this, &UGsUICustomizeFace::OnRefreshFace);

	_slotHelperSkinColor = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSkinColor->Initialize(_colorEntryWidgetClass, _panelSkinColor);
	_slotHelperSkinColor->OnRefreshEntry.AddDynamic(this, &UGsUICustomizeFace::OnRefreshSkinColor);

	_toggleGroupFace.OnSelectChanged.BindUObject(this, &UGsUICustomizeFace::OnSelectFace);
	_toggleGroupSkinColor.OnSelectChanged.BindUObject(this, &UGsUICustomizeFace::OnSelectSkinColor);

	_toggleGroupFace.OnSlotClicked.BindUObject(this, &UGsUICustomizeFace::OnClickSlotFace);
	_toggleGroupSkinColor.OnSlotClicked.BindUObject(this, &UGsUICustomizeFace::OnClickSlotSkinColor);
}

void UGsUICustomizeFace::NativeDestruct()
{
	_faceList = nullptr;
	_skinColorList.Empty();

	Super::NativeDestruct();
}

void UGsUICustomizeFace::Show()
{
	if (nullptr == _selectedData)
	{
		return;
	}

	// 얼굴 세팅
	_faceList = nullptr;
	if (const FGsSchemaCustomizeMenu* menu = _selectedData->GetMenu())
	{
		if (const FGsSchemaCustomizePartsGroup* faceGroup = menu->facePartsGroup.GetRow())
		{
			_faceList = &faceGroup->slotList;
		}
	}

	if (_faceList)
	{
		_slotHelperFace->RefreshAll(_faceList->Num());
	}
	else
	{
		_slotHelperFace->RefreshAll(0);
	}

	_toggleGroupFace.Clear();
	_toggleGroupFace.AddToggleWidgetByParentPanel(_panelFace);
	_toggleGroupFace.SetSelectedIndex(_selectedData->GetValue(EGsUICustomizeType::FACE_INDEX));

	// 피부색 세팅
	_skinColorList.Empty();
	if (const FGsSchemaCustomizeMenu* menu = _selectedData->GetMenu())
	{
		if (const FGsSchemaCustomizeColorGroup* skinColorGroup = menu->skinColorGroup.GetRow())
		{
			GetBaseColorList(skinColorGroup->colorIndexList, _skinColorList);
		}
	}

	_slotHelperSkinColor->RefreshAll(_skinColorList.Num());

	_toggleGroupSkinColor.Clear();
	_toggleGroupSkinColor.AddToggleWidgetByParentPanel(_panelSkinColor);
	_toggleGroupSkinColor.SetSelectedIndex(_selectedData->GetValue(EGsUICustomizeType::SKIN_COLOR_INDEX));
}

void UGsUICustomizeFace::OnRefreshFace(int32 InIndex, UWidget* InEntry)
{
	if (_faceList &&
		_faceList->IsValidIndex(InIndex))
	{
		UGsUICustomizeImageListEntry* entry = Cast<UGsUICustomizeImageListEntry>(InEntry);
		if (entry)
		{
			entry->SetData(_faceList->GetData()[InIndex].iconPath);
		}
	}
}

void UGsUICustomizeFace::OnRefreshSkinColor(int32 InIndex, UWidget* InEntry)
{
	if (_skinColorList.IsValidIndex(InIndex))
	{
		UGsUICustomizeColorListEntry* entry = Cast<UGsUICustomizeColorListEntry>(InEntry);
		if (entry)
		{
			entry->SetColor(_skinColorList[InIndex]);
		}
	}
}

void UGsUICustomizeFace::OnSelectFace(int32 InIndex)
{
	if (_selectedData)
	{
		_selectedData->SetValue(EGsUICustomizeType::FACE_INDEX, InIndex);

		if (_customizeHandler)
		{
			_customizeHandler->UpdateCustomizeUIData(_selectedData);
			_customizeHandler->SetFaceChange(_selectedData->GetFaceId());
			
			//// 얼굴 변경 시 색상 적용
			//int32 skinColorIndex = _selectedData->GetSkinColorIndex();
			//if (0 <= skinColorIndex)
			//{
			//	_customizeHandler->SetSkinColorChange(skinColorIndex);
			//}

			//int32 pupilColorIndex = _selectedData->GetPupilColorIndex();
			//if (0 <= pupilColorIndex)
			//{
			//	_customizeHandler->SetPupilColorChange(pupilColorIndex);
			//}

			//int32 browColorIndex = _selectedData->GetBrowColorIndex();
			//if (0 <= browColorIndex)
			//{
			//	_customizeHandler->SetBrowColorChange(browColorIndex);
			//}

			//_customizeHandler->SetTattooChange(_selectedData->GetTattooId());
		}
	}
}

void UGsUICustomizeFace::OnSelectSkinColor(int32 InIndex)
{
	if (_selectedData)
	{
		_selectedData->SetValue(EGsUICustomizeType::SKIN_COLOR_INDEX, InIndex);

		if (_customizeHandler)
		{
			_customizeHandler->UpdateCustomizeUIData(_selectedData);
			int32 colorIndex = _selectedData->GetSkinColorIndex();
			if (0 <= colorIndex)
			{
				_customizeHandler->SetSkinColorChange(colorIndex);
			}
		}
	}
}

void UGsUICustomizeFace::OnClickSlotFace(int32 InIndex, bool bIsSelected)
{
	OnClickSlot.ExecuteIfBound();
}

void UGsUICustomizeFace::OnClickSlotSkinColor(int32 InIndex, bool bIsSelected)
{
	OnClickSlot.ExecuteIfBound();
}