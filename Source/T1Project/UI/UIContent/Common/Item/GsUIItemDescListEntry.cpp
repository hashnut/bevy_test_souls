// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIItemDescListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"


void UGsUIItemDescListEntry::SetTextTitle(const FText& InText)
{
	_textBlockTitle->SetText(InText);
}

void UGsUIItemDescListEntry::SetTextDesc(const FText& InText)
{
	_textBlockDesc->SetText(InText);
}

void UGsUIItemDescListEntry::SetColorTitle(const FLinearColor& InColor)
{
	_textBlockTitle->SetColorAndOpacity(InColor);
}
