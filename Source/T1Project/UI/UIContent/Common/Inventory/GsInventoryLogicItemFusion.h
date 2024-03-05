// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

/**
 * 아이템 강화/제련 창에서 쓰일 인벤토리
 */
class T1PROJECT_API FGsInventoryLogicItemFusion : public FGsInventoryLogicBase
{
public:
	using Super = FGsInventoryLogicBase;

protected:
	const class FGsItemFusionHandler* _fusionHandler = nullptr;

	TArray<FGsItem*> _itemDataList;

public:
	virtual void Clear() override;

protected:
	// 리스트 갱신
	virtual void OnInvalidateList() override;

	/**
	 * FGsInventoryLogicBase override
	 */
public:
	virtual void OnCreateItemIcon(UGsUIIconBase* InIcon) override;
	virtual void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon) override;
	virtual void OnSelectItemIcon(UGsUIIconItem* InPrevIcon, UGsUIIconItem* InCurrIcon) override;

	virtual int32 GetDataCount() const override;
	virtual int32 GetDataIndex(ItemDBId InItemDBId) const override;

	void OnLongPressIcon(UGsUIIconItem& InIcon);

public:
	void SetData(const class FGsItemFusionHandler* InFusionHandler);
};