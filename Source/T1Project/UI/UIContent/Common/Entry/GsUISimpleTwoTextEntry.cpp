// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISimpleTwoTextEntry.h"
#include "Components/TextBlock.h"


void UGsUISimpleTwoTextEntry::SetTextFront(const FText& InText)
{
	_textBlockFront->SetText(InText);
}

void UGsUISimpleTwoTextEntry::SetTextBack(const FText& InText)
{
	_textBlockBack->SetText(InText);
}
