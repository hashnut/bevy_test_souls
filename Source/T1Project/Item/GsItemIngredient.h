// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsItem.h"
#include "DataSchema/item/GsSchemaItemIngredientDetail.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "UI/UIContent/Define/EGsIngredientUse_UIType.h"


struct FGsSchemaItemCommon;
struct FGsSchemaItemEffectRefineOption;
struct FGsSchemaItemEffectEnchantScroll;
struct FGsSchemaItemEffectDiscountCouponData;
/**
 * 재료성 아이템
 */
class T1PROJECT_API FGsItemIngredient : public FGsItem
{
	using Super = FGsItem;

private:
	const FGsSchemaItemIngredientDetail* _ingredientTableData = nullptr;

private:
	TArray<const FGsSchemaBase*> _effectDetailDataBaseList;
	

public:
	explicit FGsItemIngredient(const FGsSchemaItemCommon* InItemCommonData);
	FGsItemIngredient() = default;
	virtual ~FGsItemIngredient() = default;

public:
	virtual void UpdateBy(const FGsItem* In_Item) override;

public:
	virtual bool IsCooldownTimeItem() const override;
	virtual bool IsUseItem() const override;
	virtual float GetMaxCoolTime() const override;
	virtual int32 GetCooldownGroupID() const override;
	virtual int32 GetTooltipDurationTime() const override;
	virtual int32 GetPickOneTypeRewardID() const override;

private:
	void SetEnchantScrollItemDetailData(const TArray<int32>& InEffectIDList);	
	void SetRefineItemDetailData(const TArray<int32>& InEffectIDList);
	void SetDiscountCouponItemDetailData(const int32 InEffectID);
	void SetRewardEffectDetailData(const TArray<int32>& InEffectIDList);
	void SetMagicalForgeDetailData(const int32 InEffectId);

public:
	bool IsMatchedEnchantScrollItem(int32 InCompareId) const;
	bool IsMatchedEnchantTargetItem(const ItemCategorySub InCategorySub) const;
	bool IsMatchedRefineTargetItem(const ItemCategorySub InCategorySub);
	bool IsMatchedMagicalForgeTargetItem(const ItemCategorySub InCategorySub) const;
	bool IsOverlabUseBuffItem(class FGsAbnormalityHandlerBase* InhandlerBase);
	bool IsMagicalForgeItem() const;
	bool IsAllowedAutoUse() const;

public:
	ItemEffectType GetEffectType() const;
	const ItemEnchantType GetItemEnchantType();
	const EGsIngredientUse_UIType GetIngredientUseContentsType() const;

	void FindEffectIdList(OUT TArray<int32>& outEffectIdList) const;
	int32 FindFirstEffectId();
	int32 FindFirstEffectId() const;

	template <typename tSchema>
	const tSchema* GetFirstEffectDetailData() const
	{
		if (nullptr == _ingredientTableData)
			return nullptr;

		if (0 >= _effectDetailDataBaseList.Num())
			return nullptr;

		const tSchema* detailData = static_cast<const tSchema*>(_effectDetailDataBaseList[0]);

		return detailData;
	}
};
