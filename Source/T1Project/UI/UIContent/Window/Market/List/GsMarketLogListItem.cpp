// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketLogListItem.h"


void UGsMarketLogListItem::Clear()
{
	_data = nullptr;
}

void UGsMarketLogListItem::SetData(const FGsMarketSettlementData* InData)
{
	_data = InData;
}
