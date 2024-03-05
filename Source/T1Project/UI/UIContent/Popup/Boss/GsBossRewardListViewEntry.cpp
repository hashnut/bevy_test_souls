// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBossRewardListViewEntry.h"
#include "Components/ListView.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "T1Project/UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "UI/UIContent/Common/Entry/GsUIItemIconListViewEntry.h"

void UGsBossRewardListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_rewardListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
	FText::FindText(TEXT("FieldBossText"), TEXT("UI_Text_NRank"), _rankText);
}

void UGsBossRewardListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{	
	UGsBossRewardListViewEntryData* data = Cast<UGsBossRewardListViewEntryData>(inListItemObject);
	if (data)
	{		
		InvalidReward(data);
	}
}

void UGsBossRewardListViewEntry::InvalidReward(UGsBossRewardListViewEntryData* inData)
{
	SetRank(inData->_minRank, inData->_maxRank);
	SetReward(inData->_rewardArray);
}

void UGsBossRewardListViewEntry::SetRank(int32 inMinRank, int32 inMaxRank)
{
	if (inMinRank == inMaxRank)
	{
		if (inMinRank <= 3)
		{
			_rankWidgetSwitcher->SetActiveWidgetIndex(inMinRank - 1);
		}
		else
		{
			_rankWidgetSwitcher->SetActiveWidgetIndex(3);
			FString rankRange = FString::Printf(TEXT("%d"), inMinRank);
			_rankTextBlock->SetText(FText::FromString(rankRange));
		}
	}
	else
	{
		_rankWidgetSwitcher->SetActiveWidgetIndex(3);
		FString rankRange = FString::Printf(TEXT("%d ~ %d"), inMinRank, inMaxRank);
		_rankTextBlock->SetText(FText::FromString(rankRange));
	}
}

void UGsBossRewardListViewEntry::SetReward(TArray<TSharedPtr<FGsBossRewardInfo>>& inRewardData)
{	
	TArray<UGsUIItemIconListViewEntryData*> iconListViewEntryDataArray;
	UGsUIItemIconListViewEntryData* listViewEntryData = nullptr;
	FGsItemIconData itemIconData;
	for (const TSharedPtr<FGsBossRewardInfo> reward : inRewardData)
	{		
		listViewEntryData = NewObject<UGsUIItemIconListViewEntryData>();	

		itemIconData.Init(reward->_itemId);
		listViewEntryData->_itemDataArray.Add(itemIconData);

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	_rewardListView->SetListItems(iconListViewEntryDataArray);
	_rewardListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}
