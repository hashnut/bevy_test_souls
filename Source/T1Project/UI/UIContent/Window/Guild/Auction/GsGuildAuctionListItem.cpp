// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildAuctionListItem.h"

#include "Guild/GsGuildAuctionData.h"


void UGsGuildAuctionListItem::Clear()
{
	_auctionId = INVALID_ITEM_DB_ID;
}

void UGsGuildAuctionListItem::SetData(GuildAuctionId InAuctionId)
{
	_auctionId = InAuctionId;
}
