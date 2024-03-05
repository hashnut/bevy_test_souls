// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/List/GsMarketListViewControlBase.h"

class UGsMarketLogListItem;

/**
 * 거래소 리스트 뷰 동작 정의
 */
class T1PROJECT_API FGsMarketListViewControlLog : public FGsMarketListViewControlBase
{
public:
	using Super = FGsMarketListViewControlBase;

protected:
	TArray<UGsMarketLogListItem*> _dataList;

public:
	explicit FGsMarketListViewControlLog(UListView* InListView, int32 InListItemCountPerPage, UGsPoolUObject* InListItemPool)
		: Super(InListView, InListItemCountPerPage, InListItemPool)
	{
	}
	virtual ~FGsMarketListViewControlLog();

public:
	virtual void ClearData() override;
	virtual void SetData(UGsMarketLogListItem* InData) override;
	virtual bool UpdateList(bool bInResetScroll, int32 InLastIndex) override;
	virtual int32 GetDataCount() const override { return _dataList.Num(); }
};
