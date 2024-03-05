// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionRadioButtonGroup.h"
#include "GsUIOptionButtonGroup.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Option/GsSchemaOptionButtonGroup.h"

#include "Components/HorizontalBox.h"
#include "Blueprint/UserWidget.h"
#include "Option/GsOptionEnumType.h"
#include "Option/GsGameUserSettings.h"


void UGsUIOptionRadioButtonGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 버튼그룹 토글 이벤트
	_toggleGroup.OnSlotClicked.BindUObject(this, &UGsUIOptionRadioButtonGroup::OnClickButton);
	_toggleGroup.SetSelectedIndex(0);

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_slotEntryClass, _HorizontalBoxList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIOptionRadioButtonGroup::OnRefreshEntrySlot);
}

void UGsUIOptionRadioButtonGroup::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIOptionRadioButtonGroup::BeginDestroy()
{
	_toggleGroup.Clear();

	if (nullptr != _slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIOptionRadioButtonGroup::OnRefreshEntrySlot);
		_slotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIOptionRadioButtonGroup::OnRefreshEntrySlot(int32 InIndex, UWidget* InEntry)
{
	if (nullptr == _buttonGroup)
		return;

	int8 listNum = _buttonGroup->buttonTextList.Num();
	if (listNum >= InIndex)
	{
		UGsUIOptionButtonGroup* button = Cast<UGsUIOptionButtonGroup>(InEntry);
		if (nullptr != button)
		{
			button->SetData(_buttonGroup->buttonTextList[InIndex], _isEnabled, _isVislble);
		}
	}
}

void UGsUIOptionRadioButtonGroup::OnClickButton(int32 InIndex, bool bIsSelected)
{
	// 예외처리
	if (EGsOptionItem::Window_Resolution == _itemType)
	{		
		OnClickSlot.ExecuteIfBound(_itemType, InIndex);
		return;
	}

	if (bIsSelected)
	{
		OnClickSlot.ExecuteIfBound(_itemType, InIndex);
	}
}

void UGsUIOptionRadioButtonGroup::SetData(EGsOptionItem InType, const FGsSchemaOptionButtonGroup* InSchemaButtonGroup, 
	int InOptionIndex, bool InisEnabled, bool InisVisible)
{
	_itemType = InType;
	_buttonGroup = InSchemaButtonGroup;
	_isEnabled = InisEnabled;
	_isVislble = InisVisible;

	int8 buttonCount = _buttonGroup->buttonList.Num();

	_slotHelper->RefreshAll(buttonCount);
	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel((UPanelWidget*)_HorizontalBoxList);

	_toggleGroup.SetSelectedIndex(InOptionIndex, false);
}

void UGsUIOptionRadioButtonGroup::SetValueIndex(int InOptionIndex)
{		
	_toggleGroup.SetSelectedIndexNoEvent(InOptionIndex);
}