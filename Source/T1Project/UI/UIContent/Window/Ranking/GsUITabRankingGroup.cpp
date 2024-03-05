// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITabRankingGroup.h"

#include "Components/TextBlock.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"


void UGsUITabRankingGroup::SetTabText(const FText& InText)
{
	_textBlockOff->SetText(InText);
	_textBlockOn->SetText(InText);
}

IGsToggleGroupEntry* UGsUITabRankingGroup::GetToggleGroupEntry()
{
	return _btnGroup;
}
