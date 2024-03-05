// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionButton.h"

#include "GsSchemaEnums.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"
#include "UserWidget.h"
#include "Option/GsOptionEnumType.h"


void UGsUIOptionButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btn->OnClicked.AddDynamic(this, &UGsUIOptionButton::OnClick);
}

void UGsUIOptionButton::OnClick()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	OnClickSlot.ExecuteIfBound(_optionType);
}

void UGsUIOptionButton::SetData(EGsOptionItem InType, const FText& InNameTag)
{
	_optionType = InType;
	_textNameTag = InNameTag;
}