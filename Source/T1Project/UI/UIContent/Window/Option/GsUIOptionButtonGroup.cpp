// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionButtonGroup.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"
#include "UserWidget.h"
#include "Option/GsOptionEnumType.h"


void UGsUIOptionButtonGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnNormal->OnClicked.AddDynamic(this, &UGsUIOptionButtonGroup::OnClickNormal);
	_btnSelected->OnClicked.AddDynamic(this, &UGsUIOptionButtonGroup::OnClickSelected);
}

void UGsUIOptionButtonGroup::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_btnNormal->SetVisibility((bInIsSelected) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	_btnSelected->SetVisibility((bInIsSelected) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UGsUIOptionButtonGroup::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIOptionButtonGroup::OnClickNormal()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, true);
	}
}

void UGsUIOptionButtonGroup::OnClickSelected()
{
	// ToggleGroup에 콜백 전달. 끄기 위해 false 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, false);
	}
}

void UGsUIOptionButtonGroup::SetData(const FText& InNameTag, bool InIsEnabled, bool InisVisible)
{
	_textNameTag = InNameTag;

	_btnNormal->SetIsEnabled(InIsEnabled ? true : false);
	_btnSelected->SetIsEnabled(InIsEnabled ? true : false);
}

void UGsUIOptionButtonGroup::SetData(EGsOptionItem InType, const FText& InNameTag, bool InIsEnabled, bool InisVisible)
{
	_itemType = InType;
	_textNameTag = InNameTag;

	_btnNormal->SetIsEnabled(InIsEnabled ? true : false);
	_btnSelected->SetIsEnabled(InIsEnabled ? true : false);
}


