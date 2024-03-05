// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildSanctumNexusStateText.h"
#include "Components/TextBlock.h"


void UGsUIGuildSanctumNexusStateText::SetTextState(const FText& InText)
{
	_textBlockState->SetText(InText);
}
