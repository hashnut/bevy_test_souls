// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildDungeonParticipationListViewEntry.h"
#include "Components/TextBlock.h"
#include "UI/UILib/Define/GsUIDefine.h"

const FTextKey DUNGEON_USERLEVEL_TEXT_KEY = FTextKey(TEXT("Dungeon_UserLevel"));

void UGsGuildDungeonParticipationListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsGuildDungeonParticipationListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(GUILD_TEXT_KEY, DUNGEON_USERLEVEL_TEXT_KEY, _dungeonUserLevelText);
}

void UGsGuildDungeonParticipationListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsGuildDungeonParticipationListViewEntryData* data = Cast<UGsGuildDungeonParticipationListViewEntryData>(inListItemObject);
	if (data)
	{
		SetLevelText(data->_level);
		SetNameText(data->_name);		
	}
}

void UGsGuildDungeonParticipationListViewEntry::SetLevelText(int32 inLevel)
{
	FText levelText = FText::Format(_dungeonUserLevelText, inLevel);
	_levelTextBlock->SetText(levelText);
}

void UGsGuildDungeonParticipationListViewEntry::SetNameText(const FString& inName)
{
	_nameTextBlock->SetText(FText::FromString(inName));
}
