// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionToggleGroup.h"
#include "GsUIOptionToggle.h"
#include "GsUIOptionButtonGroup.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Option/GsSchemaOptionToggleGroup.h"
#include "Option/GsSchemaOptionButtonGroup.h"

#include "Components/HorizontalBox.h"
#include "Blueprint/UserWidget.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Option/GsOptionEnumType.h"


void UGsUIOptionToggleGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_slotEntryClass, _HorizontalBoxList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIOptionToggleGroup::OnRefreshEntrySlot);
}

void UGsUIOptionToggleGroup::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIOptionToggleGroup::BeginDestroy()
{
	if (nullptr != _slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIOptionToggleGroup::OnRefreshEntrySlot);
		_slotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIOptionToggleGroup::OnRefreshEntrySlot(int32 InIndex, UWidget* InEntry)
{
	if (nullptr == _schemaToggleGroup)
		return;

	int8 listNum = _schemaToggleGroup->toggleList.Num();
	if (listNum >= InIndex)
	{
		UGsUIOptionToggle* button = Cast<UGsUIOptionToggle>(InEntry);
		if (nullptr != button)
		{
			EGsOptionItem type = _schemaToggleGroup->toggleList[InIndex].optionItemType;

			float val = 0.0f;
			bool isVisible = true;
			UGsOptionManager* optManager = GOption();
			if (nullptr != optManager)
			{
				optManager->GetValue(type, val, isVisible);
			}

			bool isVisibility = true;			
			for (auto iter : _schemaToggleGroup->toggleList[InIndex].OptionExceptionPlatformTypeList)
			{
				if (iter == EGsOptionPlatformType::OPTION_WINDOW)
				{
#if WITH_EDITOR || PLATFORM_WINDOWS
					isVisibility = false;
					continue;
#endif
				}
				else if (iter == EGsOptionPlatformType::OPTION_MOBILE_AOS)
				{
#if PLATFORM_ANDROID
					isVisibility = false;
					continue;
#endif
				}
				else if (iter == EGsOptionPlatformType::OPTION_MOBILE_IOS)
				{
#if PLATFORM_IOS
					isVisibility = false;
					continue;
#endif
				}
			}			

			button->SetVisibility(isVisibility? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			button->SetIsEnabled(_isEnabled);
			button->SetData(type, (0 == val) ? false : true);			
			button->SetButtonText(_schemaToggleGroup->toggleList[InIndex].onText, _schemaToggleGroup->toggleList[InIndex].offText);
			// 버튼 이벤트
			button->OnClickSlot.BindUObject(this, &UGsUIOptionToggleGroup::OnClickToggleButton);
		}
	}
}

void UGsUIOptionToggleGroup::OnClickToggleButton(EGsOptionItem InType, bool InIsSelect)
{
	OnClickSlot.ExecuteIfBound(InType, InIsSelect);
}

void UGsUIOptionToggleGroup::SetData(const FGsSchemaOptionToggleGroup* InSchemaToggleGroup, bool InIsEnabled)
{	
	_schemaToggleGroup = InSchemaToggleGroup;
	_isEnabled = InIsEnabled;

	int8 listNum = _schemaToggleGroup->toggleList.Num();
	_slotHelper->RefreshAll(listNum);
}