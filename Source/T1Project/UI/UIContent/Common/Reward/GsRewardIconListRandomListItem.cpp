// Fill out your copyright notice in the Description page of Project Settings.


#include "GsRewardIconListRandomListItem.h"


void UGsRewardIconListRandomListItem::Clear()
{
	_iconData = nullptr;
	_bCenterPopup = false;
	_bEnableDetailPopup = false;
}

void UGsRewardIconListRandomListItem::SetData(FGsRewardIconDataWrapper* InIconData,
	bool bInCenterPopup, bool bInEnableDetailPopup)
{
	_iconData = InIconData;
	_bCenterPopup = bInCenterPopup;
	_bEnableDetailPopup = bInEnableDetailPopup;
}
