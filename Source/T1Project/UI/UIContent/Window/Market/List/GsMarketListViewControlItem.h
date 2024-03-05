// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/List/GsMarketListViewControlBase.h"

class UGsMarketItemListItem;

/**
 * 거래소 리스트 뷰 동작 정의
 */
class T1PROJECT_API FGsMarketListViewControlItem : public FGsMarketListViewControlBase
{
public:
	using Super = FGsMarketListViewControlBase;

protected:
	TArray<UGsMarketItemListItem*> _dataList;

public:
	explicit FGsMarketListViewControlItem(UListView* InListView, int32 InListItemCountPerPage, UGsPoolUObject* InListItemPool)
		: Super(InListView, InListItemCountPerPage, InListItemPool)
	{
	}
	virtual ~FGsMarketListViewControlItem();

public:
	virtual void ClearData() override;
	virtual void SetData(UGsMarketItemListItem* InData) override;
	virtual bool UpdateList(bool bInResetScroll, int32 InLastIndex = 0) override;
	virtual void SortByPrice(bool isDescending) override;
	virtual void SortByPricePerUnit(bool isDescending) override;
	virtual int32 GetDataCount() const override { return _dataList.Num(); }
	virtual void GetTopNTransactionIdList(int32 InCount, OUT TArray<MarketTransactionId>& OutList) override;
};
