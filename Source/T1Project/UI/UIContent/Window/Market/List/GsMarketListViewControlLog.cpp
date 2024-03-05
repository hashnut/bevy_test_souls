// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketListViewControlLog.h"
#include "UI/UIContent/Window/Market/List/GsMarketLogListItem.h"


FGsMarketListViewControlLog::~FGsMarketListViewControlLog()
{
	ClearData();
}

void FGsMarketListViewControlLog::ClearData()
{
	if (_listItemPool.IsValid())
	{
		for (UGsMarketLogListItem* listItem : _dataList)
		{
			_listItemPool->ReleaseOne(listItem);
		}
	}
	_dataList.Empty();
	
	_listView->ClearListItems();
}

void FGsMarketListViewControlLog::SetData(UGsMarketLogListItem* InData)
{
	_dataList.Emplace(InData);
}

bool FGsMarketListViewControlLog::UpdateList(bool bInResetScroll, int32 InLastIndex)
{
	_listView->SetListItems(_dataList);
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	return Super::UpdateList(bInResetScroll, InLastIndex);
}
