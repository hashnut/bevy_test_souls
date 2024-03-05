// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildDivisionListItem.h"


void UGsGuildDivisionListItem::Clear()
{
	_divisionItemDBId = INVALID_ITEM_DB_ID;
}

void UGsGuildDivisionListItem::SetData(ItemDBId InItemDBId)
{
	_divisionItemDBId = InItemDBId;
}
