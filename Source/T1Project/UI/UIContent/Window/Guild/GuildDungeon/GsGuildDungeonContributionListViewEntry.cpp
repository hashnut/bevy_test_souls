// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGuildDungeonContributionListViewEntry.h"
#include "Components/TextBlock.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

const FTextKey DUNGEON_NRANK = FTextKey(TEXT("Dungeon_NRank"));

void UGsGuildDungeonContributionListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsGuildDungeonContributionListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(GUILD_TEXT_KEY, DUNGEON_NRANK, _dungeonNRankText);
}

void UGsGuildDungeonContributionListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsGuildDungeonContributionListViewEntryData* data = Cast<UGsGuildDungeonContributionListViewEntryData>(inListItemObject);
	if (data)
	{
		SetRank(data->_rank);
		SetNameText(data->_name);
		SetContributionText(data->_contribution);
	}
}

void UGsGuildDungeonContributionListViewEntry::Clear()
{	
	_rankWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
	_rankTextBlock->SetText(FText::GetEmpty());
	_nameTextBlock->SetText(FText::GetEmpty());
	_contributionTextBlock->SetText(FText::GetEmpty());
}

void UGsGuildDungeonContributionListViewEntry::SetRank(int32 inRank)
{	
	if (inRank < 0)
	{
		_rankWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
	}
	else if(inRank <= 3)
	{
		_rankWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		_rankWidgetSwitcher->SetActiveWidgetIndex(inRank - 1);
	}
	else
	{
		_rankWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		_rankWidgetSwitcher->SetActiveWidgetIndex(3);
		FText rankText = FText::Format(_dungeonNRankText, inRank);
		_rankTextBlock->SetText(rankText);
	}
}

void UGsGuildDungeonContributionListViewEntry::SetNameText(const FString& inName)
{
	_nameTextBlock->SetText(FText::FromString(inName));
}

void UGsGuildDungeonContributionListViewEntry::SetContributionText(int32 inContribution)
{	
	_contributionTextBlock->SetText(FText::AsNumber(inContribution));
}