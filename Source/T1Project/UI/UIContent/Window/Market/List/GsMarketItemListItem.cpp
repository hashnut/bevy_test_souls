// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketItemListItem.h"


void UGsMarketItemListItem::Clear()
{
	_bIsWorldMarket = false;
	_data = nullptr;
}

void UGsMarketItemListItem::SetData(bool bIsWorldMarket, const FGsMarketItemData* InData)
{
	_bIsWorldMarket = bIsWorldMarket;
	_data = InData;
}
