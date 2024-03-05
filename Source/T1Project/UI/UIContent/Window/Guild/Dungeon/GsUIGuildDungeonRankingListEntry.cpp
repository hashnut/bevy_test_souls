// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDungeonRankingListEntry.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"


void UGsUIGuildDungeonRankingListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UGsGuildDungeonRankingListItem* listItem = Cast<UGsGuildDungeonRankingListItem>(ListItemObject))
	{
		InvalidateAll(listItem);
	}
}

void UGsUIGuildDungeonRankingListEntry::Clear()
{	
	_switcherRank->SetVisibility(ESlateVisibility::Hidden);
	_textBlockRank->SetText(FText::GetEmpty());
	_textBlockName->SetText(FText::GetEmpty());
	_textBlockPoint->SetText(FText::GetEmpty());
}

void UGsUIGuildDungeonRankingListEntry::RefreshAll()
{
	InvalidateAll(GetListItem<UGsGuildDungeonRankingListItem>());
}

void UGsUIGuildDungeonRankingListEntry::InvalidateAll(UGsGuildDungeonRankingListItem* InListItem)
{
	if (nullptr == InListItem)
	{
		SetRank(0);
		_textBlockName->SetText(FText::GetEmpty());
		_textBlockPoint->SetText(FText::GetEmpty());

		return;
	}

	SetRank(InListItem->_rank);

	_textBlockName->SetText(FText::FromString(InListItem->_strName));
	_textBlockPoint->SetText(FText::AsNumber(InListItem->_point));
}

void UGsUIGuildDungeonRankingListEntry::SetRank(int32 InRank)
{	
	if (0 == InRank)
	{
		_switcherRank->SetActiveWidgetIndex(3);
		_textBlockRank->SetText(FText::GetEmpty());
	}
	else if (3 >= InRank)
	{
		_switcherRank->SetActiveWidgetIndex(InRank - 1);
	}
	else
	{		
		_switcherRank->SetActiveWidgetIndex(3);

		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Dungeon_NRank"), textFormat))
		{
			FText rankText = FText::Format(textFormat, InRank);
			_textBlockRank->SetText(rankText);
		}
		else
		{
			_textBlockRank->SetText(FText::AsNumber(InRank));
		}
	}
}
