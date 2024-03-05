// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicBase.h"


class UGsUIIconBase;
class FGsItem;
/**
* 개인 거래 인벤토리 처리 클래스
**/
class T1PROJECT_API FGsInventoryLogicTrade final : public FGsInventoryLogicBase
{
public:
	using Super = FGsInventoryLogicBase;

protected:
	TArray<FGsItem*> _dataList;
	ItemDBId _selectedItemDBIdCalc;

public:
	FGsInventoryLogicTrade() = default;
	virtual ~FGsInventoryLogicTrade() = default;

public:
	virtual void Clear() override;

public:
	virtual void OnInvalidateList() override;
	virtual void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon) override;
	virtual void OnClickItemIcon(UGsUIIconItem & InIcon) override;
	virtual void OnSelectTab(EInventoryTab InType) override;

	virtual int32 GetDataCount() const override;
	virtual int32 GetDataIndex(ItemDBId InItemDBId) const override;
	
	void OnLongPressIcon(UGsUIIconItem & InIcon);
	void OnFinishCalculate(int64 InValue);

protected:
	void GetItemList(ItemType InItemType, OUT TArray<FGsItem*>& OutList);

	bool IsTradableItem(const FGsItem* InItem);
	bool SortTradable(FGsItem& A, FGsItem& B);
	bool SortNonTradable(FGsItem& A, FGsItem& B);
};