// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICheckBoxCommonText.h"
#include "Components/TextBlock.h"


void UGsUICheckBoxCommonText::SetText(const FText& InText)
{
	_textBlockNameOff->SetText(InText);
	_textBlockNameOn->SetText(InText);
}
