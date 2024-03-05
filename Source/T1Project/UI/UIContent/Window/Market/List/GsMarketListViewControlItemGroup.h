// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/List/GsMarketListViewControlBase.h"

class UGsMarketItemGroupListItem;
struct FGsSchemaItemCommon;
struct FGsMarketItemGroupData;

/**
 * 거래소 리스트 뷰 동작 정의
 */
class T1PROJECT_API FGsMarketListViewControlItemGroup : public FGsMarketListViewControlBase
{
public:
	using Super = FGsMarketListViewControlBase;

protected:
	TArray<UGsMarketItemGroupListItem*> _dataList;

public:
	explicit FGsMarketListViewControlItemGroup(UListView* InListView, int32 InListItemCountPerPage, UGsPoolUObject* InListItemPool)
		: Super(InListView, InListItemCountPerPage, InListItemPool)
	{
	}
	virtual ~FGsMarketListViewControlItemGroup();

public:
	virtual void ClearData() override;
	virtual void SetData(UGsMarketItemGroupListItem* InData) override;
	virtual void SortByEnchantLevel() override;
	virtual bool UpdateList(bool bInResetScroll, int32 InLastIndex = 0) override;
		
	virtual int32 FindDataIndex(ItemId InItemId) override;
	virtual void AddData(class UGsMarketItemGroupListItem* InData) override;
	virtual void RemoveData(ItemId InItemId) override;
	virtual void ChangeData(int32 InIndex, class UGsMarketItemGroupListItem* InData) override;
	virtual int32 GetDataCount() const override { return _dataList.Num(); }
};
