// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionCheckButtonGroup.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/UserWidget.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Option/GsOptionEnumType.h"
#include "Option/GsGameUserSettings.h"
#include "DataSchema/Option/GsSchemaOptionCheckGroup.h"
#include "DataSchema/Option/GsSchemaOptionCheck.h"

#include "GsUIOptionCheckButton.h"


void UGsUIOptionCheckButtonGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_slotEntryClass, _HorizontalBoxList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIOptionCheckButtonGroup::OnRefreshEntrySlot);
}

void UGsUIOptionCheckButtonGroup::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIOptionCheckButtonGroup::BeginDestroy()
{
	if (nullptr != _slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIOptionCheckButtonGroup::OnRefreshEntrySlot);
		_slotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIOptionCheckButtonGroup::OnRefreshEntrySlot(int32 InIndex, UWidget* InEntry)
{
	if (nullptr == _checkButtonGroup)
		return;

	int8 listNum = _checkButtonGroup->checkList.Num();
	if (listNum >= InIndex)
	{
		UGsUIOptionCheckButton* checkButton = Cast<UGsUIOptionCheckButton>(InEntry);
		if (nullptr != checkButton)
		{
			EGsOptionItem item = _checkButtonGroup->checkList[InIndex].optionItemType;
			float val = 0.0f;
			bool isVisible = true;
			UGsOptionManager* optManager = GOption();
			if (nullptr != optManager)
			{
				optManager->GetValue(item, val, isVisible);
			}
			
			checkButton->SetData(item, _checkButtonGroup->checkList[InIndex].nameText, static_cast<bool>(val), _isEnabled, _isVislble);
			checkButton->_onClickButton.BindUObject(this, &UGsUIOptionCheckButtonGroup::OnClickButton);
		}
	}
}

void UGsUIOptionCheckButtonGroup::OnClickButton(EGsOptionItem InType, bool bIsSelected)
{
	if (OnClickCheckBox.IsBound())
	{
		OnClickCheckBox.ExecuteIfBound(InType, bIsSelected);
	}
}

void UGsUIOptionCheckButtonGroup::SetData(EGsOptionItem InType, const FGsSchemaOptionCheckGroup* InSchemaCheckGroup, bool InisEnabled, bool InisVisible)
{
	_itemType = InType;
	_checkButtonGroup = InSchemaCheckGroup;
	_isEnabled = InisEnabled;
	_isVislble = InisVisible;

	int8 buttonCount = _checkButtonGroup->checkList.Num();

	_slotHelper->RefreshAll(buttonCount);
}