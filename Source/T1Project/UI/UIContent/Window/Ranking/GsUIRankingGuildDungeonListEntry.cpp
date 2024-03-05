// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRankingGuildDungeonListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Guild/GsGuildData.h"
#include "Ranking/GsRankingData.h"
#include "UI/UIContent/Window/Ranking/GsRankingListItem.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Management/ScopeGlobal/GsGuildManager.h"


void UGsUIRankingGuildDungeonListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_textBlockGuild->SetText(FText::GetEmpty());
	_textBlockStage->SetText(FText::GetEmpty());
	_textBlockClearTime->SetText(FText::GetEmpty());
}

void UGsUIRankingGuildDungeonListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	HideAll();

	UGsRankingListItem* listItem = Cast<UGsRankingListItem>(ListItemObject);
	if (nullptr == listItem)
		return;

	const FGsRankingData* data = listItem->_data;
	if (nullptr == data)
		return;

	const FGsRankingDetailGuildDungeon* rankingData = static_cast<FGsRankingDetailGuildDungeon*>(data->_detailData.Get());
	if (nullptr == rankingData)
		return;

	if (INVALID_GUILD_EMBLEM_ID != rankingData->_guildEmblemId)
	{
		SetGuildInfo(rankingData->_guildEmblemId, rankingData->_guildName);
		SetValueInfo(rankingData->_param1, rankingData->_param2);
	}
	else
	{
		HideAll();
	}
}

void UGsUIRankingGuildDungeonListEntry::SetData(FGsRankingMyData* InMyRanking)
{
	Super::SetData(InMyRanking);

	const FGsGuildDataDetail* guildData = GGuild()->GetGuildDetailData(true);
	if (nullptr != InMyRanking && guildData->IsExistGuild())
	{
		SetGuildInfo(guildData->_emblemId, guildData->_guildName);
		SetValueInfo(InMyRanking->_param1, InMyRanking->_param2);
	}
	else
	{
		HideAll();
	}
}

void UGsUIRankingGuildDungeonListEntry::SetGuildInfo(GuildEmblemId InEmblemId, const FString& InGuildName)
{
	_switcherGuild->SetActiveWidgetIndex(0);
	_textBlockGuild->SetText(FText::FromString(InGuildName));
	SetEmblem(_iconEmblem, InEmblemId);
}

void UGsUIRankingGuildDungeonListEntry::SetValueInfo(uint64 InStage, uint64 InClearTime)
{
	FText findText;
	FText::FindText(TEXT("GuildText"), TEXT("Dungeon_Stage"), findText);
	_textBlockStage->SetText(FText::Format(findText, InStage));

	// 경과 시간
	FTimespan clearTime(InClearTime * ETimespan::TicksPerSecond);
	FString strTime;
	FGsTimeStringHelper::GetTimeString(clearTime, strTime);

	_textBlockClearTime->SetText(FText::FromString(strTime));
}

void UGsUIRankingGuildDungeonListEntry::HideAll()
{
	_switcherGuild->SetActiveWidgetIndex(1);
	_iconEmblem->SetVisibility(ESlateVisibility::Collapsed);
	SetValueInfo(0, 0);
}