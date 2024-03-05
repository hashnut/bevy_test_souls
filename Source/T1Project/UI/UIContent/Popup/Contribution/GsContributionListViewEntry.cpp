// Fill out your copyright notice in the Description page of Project Settings.


#include "GsContributionListViewEntry.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


void UGsContributionListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	if (UGsContributionListViewEntryData* data = Cast<UGsContributionListViewEntryData>(inListItemObject))
	{
		InvalidateAll(data);
	}
}

void UGsContributionListViewEntry::RefreshAll()
{
	InvalidateAll(GetListItem<UGsContributionListViewEntryData>());
}

void UGsContributionListViewEntry::InvalidateAll(UGsContributionListViewEntryData* InListItem)
{
	if (InListItem)
	{
		SetContribution(InListItem->_rank, InListItem->_userName, InListItem->_contribution);
	}
	else
	{
		SetContribution(0, TEXT(""), 0);
	}
}

void UGsContributionListViewEntry::SetContribution(int32 inRank, const FString& inUserName, float inProgress)
{
	if (inRank <= 0)
	{
		_rankWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (inRank <= 3)
	{
		_rankWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		_rankWidgetSwitcher->SetActiveWidgetIndex(inRank - 1);
	}
	else
	{
		_rankWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		_rankWidgetSwitcher->SetActiveWidgetIndex(3);
		_rankTextBlock->SetText(FText::FromString(FString::FromInt(inRank)));
	}

	FString contributionValue = FString::Printf(TEXT("%.2f%%"), inProgress);

	_nameTextBlock->SetText(FText::FromString(inUserName));
	_contributionTextBlock->SetText(FText::FromString(contributionValue));
	_contributionBar->SetPercent(inProgress / 100.0f);
}
