// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemIngredient.h"
#include "T1Project.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemEffectEnchantScroll.h"
#include "DataSchema/Item/GsSchemaItemEffectRefineOption.h"
#include "DataSchema/Item/GsSchemaItemEffectDiscountCouponData.h"
#include "DataSchema/Item/GsSchemaItemEffectMagicalForge.h"

#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIContent/Define/EGsIngredientUse_UIType.h"

#include "UTIL/GsTableUtil.h"


FGsItemIngredient::FGsItemIngredient(const FGsSchemaItemCommon* InItemCommonData)
{
	const UGsTableItemIngredientDetail* table = Cast<UGsTableItemIngredientDetail>(FGsSchemaItemIngredientDetail::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("[FGsItemIngredient::FGsItemIngredient] UGsTableItemIngredientDetail is nullptr - In_ItemDetailID : %d"), InItemCommonData->detailId);
		return;
	}
	const FGsSchemaItemIngredientDetail* row = nullptr;
	if (false == table->FindRowById(InItemCommonData->detailId, row))
	{
		GSLOG(Error, TEXT("[FGsItemIngredient::FGsItemIngredient] FGsSchemaItemIngredientDetail is nullptr - In_ItemDetailID : %d"), InItemCommonData->detailId);
		return;
	}
	_ingredientTableData = row;

	_effectDetailDataBaseList.Empty();
	if (ItemEffectType::GET_REWARD == _ingredientTableData->effectType)
	{
		SetRewardEffectDetailData(_ingredientTableData->effectIDList);
	}
	else
	{
		switch (InItemCommonData->categorySub)
		{
		case ItemCategorySub::ENCHANT_WEAPON_SCROLL:
		case ItemCategorySub::BLESSED_ENCHANT_WEAPON:
		case ItemCategorySub::CURSED_ENCHANT_WEAPON:
		case ItemCategorySub::ENCHANT_ARMOR:
		case ItemCategorySub::BLESSED_ENCHANT_ARMOR:
		case ItemCategorySub::CURSED_ENCHANT_ARMOR:
		case ItemCategorySub::ENCHANT_ACCESSORY:
		case ItemCategorySub::BLESSED_ENCHANT_ACCESSORY:
		case ItemCategorySub::CURSED_ENCHANT_ACCESSORY:
		case ItemCategorySub::ENCHANT_COLLECTABLE_SCROLL:
		{
			SetEnchantScrollItemDetailData(_ingredientTableData->effectIDList);
			break;
		}
		case ItemCategorySub::OPTION_STONE:
		case ItemCategorySub::BLESSED_OPTION_STONE:
		{
			SetRefineItemDetailData(_ingredientTableData->effectIDList);
			break;
		}
		case ItemCategorySub::DISCOUNT_COUPON:
		{
			// 무조건 배열의 0번째만 입력해달라고 기획팀에서 요청함
			if (_ingredientTableData->effectIDList.IsValidIndex(0))
			{
				SetDiscountCouponItemDetailData(_ingredientTableData->effectIDList[0]);
			}
			break;
		}
		case ItemCategorySub::MAGICAL_FORGE:
		{
			if (_ingredientTableData->effectIDList.IsValidIndex(0))
			{
				SetMagicalForgeDetailData(_ingredientTableData->effectIDList[0]);
			}
			break;
		}
		}
	}
}

void FGsItemIngredient::UpdateBy(const FGsItem* In_Item)
{
	Super::UpdateBy(In_Item);

	int32 detailID = In_Item->GetDetailID();
	const UGsTableItemIngredientDetail* table = Cast<UGsTableItemIngredientDetail>(FGsSchemaItemIngredientDetail::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("[FGsItemIngredient::UpdateBy] UGsTableItemIngredientDetail is nullptr - detailID : %d"), detailID);
		return;
	}
	const FGsSchemaItemIngredientDetail* row = nullptr;
	if (false == table->FindRowById(detailID, row))
	{
		GSLOG(Error, TEXT("[FGsItemIngredient::UpdateBy] FGsSchemaItemIngredientDetail is nullptr - detailID : %d"), detailID);
		return;
	}
	_ingredientTableData = row;
}

const EGsIngredientUse_UIType FGsItemIngredient::GetIngredientUseContentsType() const
{
	if (nullptr == _ingredientTableData)
		return EGsIngredientUse_UIType::ContentsType_Max;

	return _ingredientTableData->contentsType;
}

// 무조건 배열의 0번째만 입력해달라고 기획팀에서 요청함
void FGsItemIngredient::SetDiscountCouponItemDetailData(const int32 InEffectID)
{
	_effectDetailDataBaseList.Empty();

	const UGsTableItemEffectDiscountCouponData* table = Cast<UGsTableItemEffectDiscountCouponData>(FGsSchemaItemEffectDiscountCouponData::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaItemEffectDiscountCouponData* row;
	if (table->FindRowById(InEffectID, row))
	{
		_effectDetailDataBaseList.Emplace(row);
	}
}

void FGsItemIngredient::SetEnchantScrollItemDetailData(const TArray<int32>& InEffectIDList)
{
	_effectDetailDataBaseList.Empty();

	const UGsTableItemEffectEnchantScroll* table = Cast<UGsTableItemEffectEnchantScroll>(FGsSchemaItemEffectEnchantScroll::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	for (int32 iter : InEffectIDList)
	{
		const FGsSchemaItemEffectEnchantScroll* row;
		if (false == table->FindRowById(iter, row))
			continue;

		_effectDetailDataBaseList.Emplace(row);
	}
}

void FGsItemIngredient::SetRefineItemDetailData(const TArray<int32>& InEffectIDList)
{
	_effectDetailDataBaseList.Empty();

	const UGsTableItemEffectRefineOption* table = Cast<UGsTableItemEffectRefineOption>(FGsSchemaItemEffectRefineOption::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	for (int32 iter : InEffectIDList)
	{
		const FGsSchemaItemEffectRefineOption* row;
		if (false == table->FindRowById(iter, row))
			continue;

		_effectDetailDataBaseList.Emplace(row);
	}
}

void FGsItemIngredient::SetRewardEffectDetailData(const TArray<int32>& InEffectIDList)
{
	_effectDetailDataBaseList.Empty();

	const UGsTableRewardData* table = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	for (uint16 i = 0; i < InEffectIDList.Num(); ++i)
	{
		const FGsSchemaRewardData* row = nullptr;
		if (table->FindRowById(InEffectIDList[i], row))
		{
			_effectDetailDataBaseList.Emplace(row);
		}
	}
}

void FGsItemIngredient::SetMagicalForgeDetailData(const int32 InEffectId)
{
	_effectDetailDataBaseList.Empty();

	const FGsSchemaItemEffectMagicalForge* row = UGsTableUtil::FindRowById<UGsTableItemEffectMagicalForge, FGsSchemaItemEffectMagicalForge>(InEffectId);
	if (nullptr == row)
		return;

	_effectDetailDataBaseList.Emplace(row);
}

const ItemEnchantType FGsItemIngredient::GetItemEnchantType()
{
	const FGsSchemaItemEffectEnchantScroll* tableData = GetFirstEffectDetailData<FGsSchemaItemEffectEnchantScroll>();
	if (nullptr == tableData)
		return ItemEnchantType::MAX;

	return tableData->enchantType;
}

bool FGsItemIngredient::IsMatchedEnchantScrollItem(int32 InCompareId) const
{
	if (nullptr == _ingredientTableData)
		return false;

	if (0 >= InCompareId)
		return false;

	if (_ingredientTableData->effectType != ItemEffectType::ITEM_ENCHANT)
		return false;

	if (_ingredientTableData->effectIDList.Num() <= 0)
		return false;

	return INDEX_NONE != _ingredientTableData->effectIDList.Find(InCompareId);
}

bool FGsItemIngredient::IsMatchedEnchantTargetItem(const ItemCategorySub InCategorySub) const
{
	const FGsSchemaItemEffectEnchantScroll* tableData = GetFirstEffectDetailData<FGsSchemaItemEffectEnchantScroll>();
	if (nullptr == tableData)
		return false;

	return INDEX_NONE != tableData->itemCategorySub.Find(InCategorySub);
}

bool FGsItemIngredient::IsMatchedRefineTargetItem(const ItemCategorySub InCategorySub)
{
	const FGsSchemaItemEffectRefineOption* tableData = GetFirstEffectDetailData<FGsSchemaItemEffectRefineOption>();
	if (nullptr == tableData)
		return false;

	for (const ItemCategorySub iter : tableData->itemCategorySub)
	{
		if (iter == InCategorySub)
		{
			return true;
		}
	}
	return false;
}

bool FGsItemIngredient::IsMatchedMagicalForgeTargetItem(const ItemCategorySub InCategorySub) const
{
	if (false == IsMagicalForgeItem())
		return false;

	const FGsSchemaItemEffectMagicalForge* itemEffectData = GetFirstEffectDetailData<FGsSchemaItemEffectMagicalForge>();
	if (nullptr == itemEffectData)
		return false;

	return itemEffectData->itemCategorySubList.Contains(InCategorySub);
}

bool FGsItemIngredient::IsOverlabUseBuffItem(class FGsAbnormalityHandlerBase* InhandlerBase)
{
	if (nullptr == _ingredientTableData)
		return false;

	if (ItemEffectType::ADD_ABNORMALITY_STAT == _ingredientTableData->effectType)
	{
		for (uint8 i = 0; i < _ingredientTableData->effectIDList.Num(); ++i)
		{
			if (true == InhandlerBase->IsAbnormalityId(_ingredientTableData->effectIDList[i]))
			{
				return true;
			}
		}
	}

	return false;
}

int32 FGsItemIngredient::GetPickOneTypeRewardID() const
{
	if (nullptr == _ingredientTableData)
		return 0;

	if (ItemEffectType::GET_REWARD != _ingredientTableData->effectType)
	{
		return 0;
	}

	if (0 >= _effectDetailDataBaseList.Num())
	{
		return 0;
	}

	const FGsSchemaRewardData* rewardData =
		static_cast<const FGsSchemaRewardData*>(_effectDetailDataBaseList[0]);

	if (nullptr == rewardData)
	{
		return 0;
	}

	if (RewardType::PICK_ONE != rewardData->rewardType)
	{
		return 0;
	}

	return rewardData->id;
}

bool FGsItemIngredient::IsAllowedAutoUse() const
{
	if (nullptr == _ingredientTableData)
		return false;

	return _ingredientTableData->isAllowedAutoUse;
}

bool FGsItemIngredient::IsMagicalForgeItem() const
{
	if (nullptr == _ingredientTableData)
		return false;

	if (_ingredientTableData->effectType != ItemEffectType::MAGICAL_FORGE)
		return false;

	if (INVALID_MAGICAL_FORGE_ID == FindFirstEffectId())
		return false;

	return true;
}

void FGsItemIngredient::FindEffectIdList(OUT TArray<int32>& outEffectIdList) const
{
	outEffectIdList.Empty();
	if (nullptr == _ingredientTableData)
		return;

	for (int32 i = 0; i < _ingredientTableData->effectIDList.Num(); ++i)
	{
		outEffectIdList.Emplace(_ingredientTableData->effectIDList[i]);
	}
}

int32 FGsItemIngredient::FindFirstEffectId()
{
	if (nullptr == _ingredientTableData)
		return 0;

	if (false == _ingredientTableData->effectIDList.IsValidIndex(0))
		return 0;

	return _ingredientTableData->effectIDList[0];
}


int32 FGsItemIngredient::FindFirstEffectId() const
{
	if (nullptr == _ingredientTableData)
		return 0;

	if (false == _ingredientTableData->effectIDList.IsValidIndex(0))
		return 0;

	return _ingredientTableData->effectIDList[0];
}

bool FGsItemIngredient::IsCooldownTimeItem() const
{
	if (nullptr == _ingredientTableData)
		return false;

	return 0.0f < _ingredientTableData->cooldownTime;
}

bool FGsItemIngredient::IsUseItem() const
{
	if (nullptr == _ingredientTableData)
		return false;

	return _ingredientTableData->isUseItem;
}

float FGsItemIngredient::GetMaxCoolTime() const
{
	if (nullptr == _ingredientTableData)
		return 0.f;

	return _ingredientTableData->cooldownTime;
}

int32 FGsItemIngredient::GetCooldownGroupID() const
{
	if (nullptr == _ingredientTableData)
		return 0;

	return _ingredientTableData->cooldownGroupId;
}

int32 FGsItemIngredient::GetTooltipDurationTime() const
{
	if (nullptr == _ingredientTableData)
		return 0;

	int32 tableData = _ingredientTableData->tooltipDurationTime;
	if (0 >= tableData)
	{
		return 0;
	}

	return (tableData / 10000) * 100;
}

ItemEffectType FGsItemIngredient::GetEffectType() const
{
	if (nullptr == _ingredientTableData)
		return ItemEffectType::NONE;

	return _ingredientTableData->effectType;
}