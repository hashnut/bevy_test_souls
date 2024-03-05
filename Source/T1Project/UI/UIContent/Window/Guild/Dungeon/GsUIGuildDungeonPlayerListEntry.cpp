// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDungeonPlayerListEntry.h"
#include "Components/TextBlock.h"


void UGsUIGuildDungeonPlayerListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UGsGuildDungeonPlayerListItem* listItem = Cast<UGsGuildDungeonPlayerListItem>(ListItemObject))
	{
		InvalidateAll(listItem);		
	}
}

void UGsUIGuildDungeonPlayerListEntry::RefreshAll()
{
	InvalidateAll(GetListItem<UGsGuildDungeonPlayerListItem>());
}

void UGsUIGuildDungeonPlayerListEntry::InvalidateAll(UGsGuildDungeonPlayerListItem* InListItem)
{
	if (nullptr == InListItem)
	{
		return;
	}

	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Dungeon_UserLevel"), textFormat))
	{
		FText textLevel = FText::Format(textFormat, InListItem->_level);
		_textBlockLevel->SetText(textLevel);
	}
	else
	{
		_textBlockLevel->SetText(FText::AsNumber(InListItem->_level));
	}

	_textBlockName->SetText(FText::FromString(InListItem->_strName));
}
