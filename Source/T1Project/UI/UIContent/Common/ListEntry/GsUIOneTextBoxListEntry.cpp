// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOneTextBoxListEntry.h"
#include "Components/TextBlock.h"


void UGsUIOneTextBoxListEntry::SetData(const FText& InText)
{
	_textBlockDesc->SetText(InText);
}
