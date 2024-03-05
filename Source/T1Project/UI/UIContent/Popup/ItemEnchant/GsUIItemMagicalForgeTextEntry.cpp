// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIItemMagicalForgeTextEntry.h"

#include "Components/TextBlock.h"


void UGsUIItemMagicalForgeTextEntry::SetDescText(const FText& InText)
{
	_textBlockDesc->SetText(InText);
}

void UGsUIItemMagicalForgeTextEntry::SetDescColor(const FLinearColor& InColor)
{
	_textBlockDesc->SetColorAndOpacity(InColor);
}
