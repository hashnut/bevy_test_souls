// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicBase.h"


class UGsUIIconBase;
class UGsUIIconItem;
class FGsItem;

/**
 * 아이템 강화/제련 창에서 쓰일 인벤토리
 */
class T1PROJECT_API FGsInventoryLogicEnchantInven final : public FGsInventoryLogicBase
{
public:
	using Super = FGsInventoryLogicBase;

protected:
	ItemEffectType _ingredientEffectType = ItemEffectType::NONE;
	TArray<FGsItem*> _dataList;
	TArray<ItemDBId> _selectedItemDbidList;

public:
	FGsInventoryLogicEnchantInven() = default;
	~FGsInventoryLogicEnchantInven() = default;

public:
	virtual void Clear() override;

public:
	virtual void OnInvalidateList() override;
	virtual void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase * InIcon) override;
	virtual void OnSelectItemIcon(UGsUIIconItem * InPrevIcon, UGsUIIconItem * InCurrIcon) override;

	virtual int32 GetDataCount() const override { return _dataList.Num(); }
	virtual int32 GetDataIndex(ItemDBId InItemDBId) const override;

protected:
	void GetItemList(EInventoryTab InTabType, OUT TArray<FGsItem*>&OutList);

private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

public:
	void SetSeletedItemDbidList(TArray<ItemDBId> InItemDbidList);
	void SetIngredientEffectType(ItemEffectType InEffectType) { _ingredientEffectType = InEffectType; }

public:
	TArray<FGsItem*> GetDataList() { return _dataList; }

//----------------------------------------------------------------------------------------------------------------------
	// Tutorial
private:
	ItemDBId _tutorialItemDBId = INVALID_ITEM_DB_ID;
public:
	void SetTutorial(ItemDBId InItemDBId);
	bool IsTutorial() const { return (INVALID_ITEM_DB_ID != _tutorialItemDBId) ? true : false; }
};
