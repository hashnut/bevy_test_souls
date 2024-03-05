// Fill out your copyright notice in the Description page of Project Settings.


#include "GsRankingListItem.h"


void UGsRankingListItem::Clear()
{
	_data = nullptr;
}

void UGsRankingListItem::SetData(const FGsRankingData* InData)
{
	_data = InData;
}
