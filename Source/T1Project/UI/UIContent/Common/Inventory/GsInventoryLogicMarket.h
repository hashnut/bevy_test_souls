// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"


class UGsUIIconBase;
class UGsUIIconItem;
class FGsItem;

/**
 * 거래소 판매등록 탭에서 사용되는 인벤토리 로직
 */
class T1PROJECT_API FGsInventoryLogicMarket final : public FGsInventoryLogicBase
{
public:
	using Super = FGsInventoryLogicBase;

protected:
	TArray<FGsItem*> _dataList;

	ItemGrade _minItemGrade = ItemGrade::NONE;

public:
	FGsInventoryLogicMarket() = default;
	virtual ~FGsInventoryLogicMarket() = default;
	
public:
	virtual void Clear() override;

public:
	virtual void OnInvalidateList() override;
	virtual void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon) override;	
	virtual void OnSelectItemIcon(UGsUIIconItem* InPrevIcon, UGsUIIconItem* InCurrIcon) override;

	virtual int32 GetDataCount() const override { return _dataList.Num(); }
	virtual int32 GetDataIndex(ItemDBId InItemDBId) const override;
	
protected:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);
	void GetItemList(EInventoryTab InTabType, OUT TArray<FGsItem*>& OutList);

	// 1순위: 판매여부
	// 2순위: 장착여부(판매중 아닌 상품)
	// 3순위: 카테고리
	// 4순위: 등급
	// 5순위: 강화도
	bool SortItem(FGsItem& A, FGsItem& B);
	bool SortUnsellableItem(FGsItem& A, FGsItem& B);

public:
	bool CanSellItem(const FGsItem* InItem, bool bShowTicker = false);
	void SetIsWorldMarket(bool bIsWorldMarket);
};
