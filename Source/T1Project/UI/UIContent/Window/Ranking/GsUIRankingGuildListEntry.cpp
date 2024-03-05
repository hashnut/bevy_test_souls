// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRankingGuildListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Guild/GsGuildHelper.h"
#include "Guild/GsGuildData.h"
#include "Ranking/GsRankingData.h"
#include "UI/UIContent/Window/Ranking/GsRankingListItem.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "PaperSprite.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Management/ScopeGame/GsRankingManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"


void UGsUIRankingGuildListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	UGsRankingListItem* listItem = Cast<UGsRankingListItem>(ListItemObject);
	if (nullptr == listItem)
		return;

	const FGsRankingData* data = listItem->_data;
	if (nullptr == data)
		return;

	const FGsRankingDetailGuild* rankingDetailData = static_cast<FGsRankingDetailGuild*>(data->_detailData.Get());
	if (nullptr == rankingDetailData)
		return;

	if (INVALID_GUILD_EMBLEM_ID != rankingDetailData->_guildEmblemId)
	{
		ShowGuildInfo(rankingDetailData->_guildEmblemId, rankingDetailData->_guildName);
		_switcherValue->SetActiveWidgetIndex(0);
		SetValueText(rankingDetailData->_param1);
	}
	else
	{
		HideGuildInfo();
		_switcherValue->SetActiveWidgetIndex(1);
	}
}

void UGsUIRankingGuildListEntry::SetData(FGsRankingMyData* InMyRanking)
{
	Super::SetData(InMyRanking);

	const FGsGuildDataDetail* guildData = GGuild()->GetGuildDetailData(true);
	
	if (nullptr == InMyRanking || false == guildData->IsExistGuild())
	{
		HideGuildInfo();
		_switcherValue->SetActiveWidgetIndex(1);
	}
	else
	{
		ShowGuildInfo(guildData->_emblemId, guildData->_guildName);
		_switcherValue->SetActiveWidgetIndex(0);
		SetValueText(InMyRanking->_param1);
	}
}

void UGsUIRankingGuildListEntry::ShowGuildInfo(GuildEmblemId InEmblemId, const FString& InGuildName)
{
	_switcherGuild->SetActiveWidgetIndex(0);
	_textBlockGuild->SetText(FText::FromString(InGuildName));
	SetEmblem(_iconEmblem, InEmblemId);
}

void UGsUIRankingGuildListEntry::HideGuildInfo()
{
	_switcherGuild->SetActiveWidgetIndex(1);
	_iconEmblem->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIRankingGuildListEntry::SetValueText(uint64 InValue)
{
	_textBlockValue->SetText(FText::AsNumber(InValue));
	_textBlockPercent->SetVisibility(ESlateVisibility::Collapsed);
}
