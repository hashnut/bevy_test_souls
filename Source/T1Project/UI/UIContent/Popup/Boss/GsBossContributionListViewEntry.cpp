// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBossContributionListViewEntry.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/ListView.h"
#include "DataSchema/Reward/GsSchemaRewardBox.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossRewardData.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "UI/UIContent/Common/Entry/GsUIItemIconListViewEntry.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Social/GsSocialHelper.h"

const FTextKey UI_TEXT_LEVEL_TEXT_KEY = TEXT("UI_Text_Level");
const FTextKey UI_TEXT_NRANK_TEXT_KEY = TEXT("UI_Text_NRank");
const FTextKey UI_TEXT_PARTY_NAME_TEXT_KEY = TEXT("UI_Text_PartyName");
const FTextKey UI_TEXT_WORLD_PARTY_NAME_TEXT_KEY = TEXT("UI_Text_WorldPartyName");
const FTextKey RESULT_LEVEL_NAME_TEXT_KEY = TEXT("Result_LevelName");
const FTextKey RESULT_LEVEL_WORLD_NAME = TEXT("Result_LevelWorldName");

void UGsBossContributionListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_rewardListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_NRANK_TEXT_KEY, _rankTextFormat);	
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_PARTY_NAME_TEXT_KEY, _partyTextFormat);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_WORLD_PARTY_NAME_TEXT_KEY, _invadePartyTextFormat);
	FText::FindText(INVADE_TEXT_KEY, RESULT_LEVEL_NAME_TEXT_KEY, _resultLevelNameFormat);
	FText::FindText(INVADE_TEXT_KEY, RESULT_LEVEL_WORLD_NAME, _resultLevelWorldNameFormat);
}

void UGsBossContributionListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{	
	UGsBossContributionListViewEntryData* data = Cast<UGsBossContributionListViewEntryData>(inListItemObject);
	if (data)
	{		
		InvalidBossContribution(data);
	}
}

void UGsBossContributionListViewEntry::InvalidBossContribution(UGsBossContributionListViewEntryData* inData)
{
	SetRank(inData->_rank);	

	WorldId currentWorldId = GGameData()->GetUserData()->mHomeWorldId;
	FString worldPrefix = inData->_worldPrefix;
	bool isOtherWorld = (currentWorldId != inData->_worldId);
	bool isVisible = FGsSocialHelper::IsVisible(inData->_socialMediaInfo, inData->_name, inData->_guildName);

	FText resultText;
	if (inData->_isParty)
	{
		if (isOtherWorld)
		{
			resultText = FText::Format(_invadePartyTextFormat, FText::FromString(worldPrefix), FText::FromString(inData->_name));			
		}
		else
		{
			resultText = FText::Format(_partyTextFormat, FText::FromString(inData->_name));
		}		
	}
	else
	{	
		FString strLevel;
		if (isVisible)
		{
			strLevel = FString::Printf(TEXT("%d"), inData->_level);
		}
		else
		{
			strLevel = TEXT("??");
		}

		if (isOtherWorld)
		{
			resultText = FText::Format(_resultLevelWorldNameFormat, FText::FromString(strLevel), FText::FromString(worldPrefix), FText::FromString(inData->_name));
		}
		else
		{
			resultText = FText::Format(_resultLevelNameFormat, FText::FromString(strLevel), FText::FromString(inData->_name));
		}		
	}	

	_nameTextBlock->SetText(resultText);

	if (inData->_guildName == "")
	{
		_guildPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_guildPanel->SetVisibility(ESlateVisibility::Visible);
		_guildNameTextBlock->SetText(FText::FromString(inData->_guildName));
	}

	FString percent = FString::Printf(TEXT("%.2f%%"), inData->_contributionPercent);
	_contributionPercentTextBlock->SetText(FText::FromString(percent));

	SetReward(inData->_rewardArray);
}

void UGsBossContributionListViewEntry::SetRank(int32 inRank)
{
	if (inRank <= 3)
	{
		_rankWidgetSwitcher->SetActiveWidgetIndex(inRank - 1);
	}
	else
	{
		_rankWidgetSwitcher->SetActiveWidgetIndex(3);
		_rankTextBlock->SetText(FText::Format(_rankTextFormat, inRank));
	}
}

void UGsBossContributionListViewEntry::SetReward(TArray<TSharedPtr<FGsBossRewardInfo>>& inRewardData)
{	
	TArray<UGsUIItemIconListViewEntryData*> iconListViewEntryDataArray;
	UGsUIItemIconListViewEntryData* listViewEntryData = nullptr;
	FGsItemIconData itemIconData;
	for (const TSharedPtr<FGsBossRewardInfo> reward : inRewardData)
	{
		listViewEntryData = NewObject<UGsUIItemIconListViewEntryData>();		

		itemIconData.Init(reward->_itemId, reward->_count);

		listViewEntryData->_itemDataArray.Add(itemIconData);		

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	_rewardListView->SetListItems(iconListViewEntryDataArray);
}
