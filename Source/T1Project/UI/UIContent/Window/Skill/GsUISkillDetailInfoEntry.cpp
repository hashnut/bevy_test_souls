// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillDetailInfoEntry.h"
#include "Components/TextBlock.h"


void UGsUISkillDetailInfoEntry::SetTitle(const FText& InText)
{
	_textBlockTitle->SetText(InText);
}

void UGsUISkillDetailInfoEntry::SetDesc(const FText& InText)
{
	_textBlockDesc->SetText(InText);
}
