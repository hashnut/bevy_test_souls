// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsGuildAuctionListItem.generated.h"


/**
 * 기사단 경매 목록 리스트뷰의 데이터
 */
UCLASS()
class T1PROJECT_API UGsGuildAuctionListItem : public UObject
{
	GENERATED_BODY()

public:
	GuildAuctionId _auctionId = INVALID_GUILD_AUCTION_ID;

public:
	void Clear();
	void SetData(GuildAuctionId InAuctionId);
};
