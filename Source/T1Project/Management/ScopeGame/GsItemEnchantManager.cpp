// Fill out your copyright notice in the Description page of Project Settings.


#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "Management/ScopeGame/GsPassivityManager.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemStat.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemIngredient.h"
#include "Item/Data/GsItemStatData.h"

#include "DataSchema/item/GsSchemaItemCommon.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "DataSchema/Item/GsSchemaItemEquipEnchantRate.h"
#include "DataSchema/Item/GsSchemaItemEquipEnchantResult.h"
#include "DataSchema/Item/GsSchemaItemEffectEnchantScroll.h"

#include "Currency/GsCurrencyHelper.h"
#include "Sound/GsSoundPlayer.h"

#include "T1Project.h"



void FGsItemEnchantManager::Initialize()
{
	InitRateData();
	InitEnchantResultDeltaData();

}

void FGsItemEnchantManager::Finalize()
{
	_enchantRateDataMap.Empty();
	_enchantResultDeltaValueMap.Empty();
	_enchantEffectSoundIdMap.Empty();
}

void FGsItemEnchantManager::InitRateData()
{
	if (0 < _enchantRateDataMap.Num())
	{
		_enchantRateDataMap.Empty();
	}

	const UGsTableItemEquipEnchantRate* table = Cast<UGsTableItemEquipEnchantRate>(FGsSchemaItemEquipEnchantRate::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Warning, TEXT("[FGsItemEnchantManager::InitRateData] UGsTableItemEquipEnchantRate is null "));
		return;
	}

	TArray<const FGsSchemaItemEquipEnchantRate*> allrow;
	if (false == table->GetAllRows(allrow))
	{
		GSLOG(Warning, TEXT("[FGsItemEnchantManager::InitRateData] allrow is empty "));
		return;
	}

	for (const FGsSchemaItemEquipEnchantRate* iter : allrow)
	{
		if (nullptr == iter)
			continue;

		if (_enchantRateDataMap.Contains(iter->enchantRateId))
		{
			TMap<int32, const FGsSchemaItemEquipEnchantRate*>& data = _enchantRateDataMap[iter->enchantRateId];
			if (data.Contains(iter->level))
			{
				data[iter->level] = iter;
			}
			else
			{
				data.Emplace(iter->level, iter);
			}
		}
		else
		{
			TMap<int32, const FGsSchemaItemEquipEnchantRate*> data;
			data.Emplace(iter->level, iter);
			_enchantRateDataMap.Emplace(iter->enchantRateId, data);
		}
	}
}

void FGsItemEnchantManager::InitEnchantResultDeltaData()
{
	if (0 < _enchantResultDeltaValueMap.Num())
	{
		_enchantResultDeltaValueMap.Empty();
	}

	const UGsTableItemEquipEnchantResult* table = Cast<UGsTableItemEquipEnchantResult>(FGsSchemaItemEquipEnchantResult::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Warning, TEXT("[FGsItemEnchantManager::InitEnchantResultDeltaData] UGsTableItemEquipEnchantResult is null "));
		return;
	}

	TArray<const FGsSchemaItemEquipEnchantResult*> allrow;
	if (false == table->GetAllRows(allrow))
	{
		GSLOG(Warning, TEXT("[FGsItemEnchantManager::InitEnchantResultDeltaData] allrow is empty ")); 
		return;
	}

	for (const FGsSchemaItemEquipEnchantResult* iter : allrow)
	{
		if (nullptr == iter)
			continue;

		if (false == _enchantResultDeltaValueMap.Contains(iter->enchantResult))
		{
			_enchantResultDeltaValueMap.Emplace(iter->enchantResult, iter->enchantLevelDelta);
		}
	}
}

void FGsItemEnchantManager::GetEnchantExpectRateInfo(TWeakPtr<FGsItem> InEquipItemData, TWeakPtr<FGsItem> InIngredientItemData, OUT TArray<DisplayRateInfo>& outRateInfoList)
{ 
	outRateInfoList.Empty();

	FGsItemEquipment* equipDatap = InEquipItemData.Pin()->GetCastItem<FGsItemEquipment>();
	FGsItemIngredient* ingredientData = InIngredientItemData.Pin()->GetCastItem<FGsItemIngredient>();
	int32 value = GetEnchantResultDeltaValue(ingredientData);
	if (0 == value)
		return;

	TArray<float> rateResultList;
	// 듀랄(펜타클) 아이템인경우, 별도 디스플레이가 필요하여 별도 처리함
	if (ItemCategorySub::DURAL == equipDatap->GetSubCategory())
	{
		if ((equipDatap->GetLevel() + value) > equipDatap->GetMaxLevel())
			return;

		// 성공 
		float succRate = GetEnchantItemRate(ItemEnchantResult::ENCHANT_SUCCEEDED, equipDatap->GetEnchantRateId(), equipDatap->GetLevel());
		outRateInfoList.Emplace(DisplayRateInfo(ItemEnchantResult::ENCHANT_SUCCEEDED, equipDatap->GetLevel(), equipDatap->GetLevel() + 1, succRate));
		
		// 유지
		float preservRate = GetEnchantItemRate(ItemEnchantResult::ENCHANT_PRESERVED, equipDatap->GetEnchantRateId(), equipDatap->GetLevel());
		outRateInfoList.Emplace(DisplayRateInfo(ItemEnchantResult::ENCHANT_PRESERVED, equipDatap->GetLevel(), equipDatap->GetLevel(), preservRate));

		// 레벨 다운
		float downRate = GetEnchantItemRate(ItemEnchantResult::ENCHANT_FAILED_DOWN, equipDatap->GetEnchantRateId(), equipDatap->GetLevel());
		if (0 >= equipDatap->GetLevel() || 0.f >= downRate)
			return;

		outRateInfoList.Emplace(DisplayRateInfo(ItemEnchantResult::ENCHANT_FAILED_DOWN, equipDatap->GetLevel(), equipDatap->GetLevel() - 1, downRate));
	}
	else
	{
		switch (ingredientData->GetItemEnchantType())
		{
		case ItemEnchantType::NORMAL_ENCHANT:
		{
			if ((equipDatap->GetLevel() + value) > equipDatap->GetMaxLevel())
				return;

			rateResultList = GetDisplayEnchantItemRate(equipDatap->GetEnchantRateId(), equipDatap->GetLevel(), ItemEnchantType::NORMAL_ENCHANT);
			if (rateResultList.IsValidIndex(0))
			{
				DisplayRateInfo info;
				info._enchantType = ItemEnchantResult::ENCHANT_SUCCEEDED;
				info._currentLevel = equipDatap->GetLevel();
				info._expectLevel = equipDatap->GetLevel() + 1;
				info._rate = rateResultList[0];

				outRateInfoList.Emplace(info);
			}
			break;
		}
		case ItemEnchantType::BLESSED_ENCHANT:
		{
			if ((equipDatap->GetLevel() + 1) > equipDatap->GetMaxLevel())
				return;

			if ((equipDatap->GetLevel() + value) > equipDatap->GetMaxLevel())
			{
				rateResultList = GetDisplayEnchantItemRate(equipDatap->GetEnchantRateId(), equipDatap->GetLevel(), ItemEnchantType::NORMAL_ENCHANT);
				if (rateResultList.IsValidIndex(0))
				{
					DisplayRateInfo info;
					info._enchantType = ItemEnchantResult::ENCHANT_SUCCEEDED;
					info._currentLevel = equipDatap->GetLevel();
					info._expectLevel = equipDatap->GetMaxLevel();
					info._rate = rateResultList[0];

					outRateInfoList.Emplace(info);
				}
				return;
			}

			rateResultList = GetDisplayEnchantItemRate(equipDatap->GetEnchantRateId(), equipDatap->GetLevel(), ItemEnchantType::BLESSED_ENCHANT);
			for (uint8 i = 0; i < rateResultList.Num(); ++i)
			{
				DisplayRateInfo info;
				info._enchantType = 0 == i ? ItemEnchantResult::ENCHANT_SUCCEEDED : ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED;
				info._currentLevel = equipDatap->GetLevel();
				info._expectLevel = equipDatap->GetLevel() + (i + 1);
				info._rate = rateResultList[i];

				outRateInfoList.Emplace(info);
			}
		}
		break;
		case ItemEnchantType::CURSED_ENCHANT:
		{
			if (0 >= equipDatap->GetLevel())
				return;

			rateResultList = GetDisplayEnchantItemRate(equipDatap->GetEnchantRateId(), equipDatap->GetLevel(), ItemEnchantType::CURSED_ENCHANT);
			if (rateResultList.IsValidIndex(0))
			{
				DisplayRateInfo info;
				info._enchantType = ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED;
				info._currentLevel = equipDatap->GetLevel();
				info._expectLevel = equipDatap->GetLevel() + value;
				info._rate = rateResultList[0];

				outRateInfoList.Emplace(info);
			}
		}
		break;
		}
	}
}

TArray<float> FGsItemEnchantManager::GetDisplayEnchantItemRate(const int32 InRateId, const Level InTargetLevel, ItemEnchantType InEnchantType)
{
	TArray<float> resultList;
	if (_enchantRateDataMap.Contains(InRateId))
	{
		TMap<int32, const FGsSchemaItemEquipEnchantRate*> data = _enchantRateDataMap[InRateId];
		if (data.Contains(InTargetLevel))
		{
			const FGsSchemaItemEquipEnchantRate* rateData = data[InTargetLevel];
			int32 blessRateValue = rateData->blessRate;
			int32 curseRateValue = rateData->curseRate;
			int32 successRateValue = rateData->successRate;
			switch (InEnchantType)
			{
			case ItemEnchantType::NORMAL_ENCHANT:
			{
				if (0 >= successRateValue)
					break;

				float result = (((float)successRateValue / 10000.f) * 100.f) + 0.01f;
				resultList.Emplace(result);
			}
			break;
			case ItemEnchantType::BLESSED_ENCHANT:
			{
				if (0 >= successRateValue)
					break;
				if (0 >= blessRateValue)
					break;

				float blessedDefaultValue = ((float)successRateValue / 10000.f) * ((float)blessRateValue / 10000.f);
				float defaultValue = ((float)successRateValue / 10000.f) - blessedDefaultValue;
				float result = (defaultValue * 100.f) + 0.01f;
				resultList.Emplace(result);
				resultList.Emplace((blessedDefaultValue * 100.f) + 0.01f);
			}
			break;
			case ItemEnchantType::CURSED_ENCHANT:
			{
				if (0 >= curseRateValue)
					break;

				float result = (((float)curseRateValue / 10000.f) * 100.f) + 0.01f;
				resultList.Emplace(result);
			}
			break;
			}
		}
	}
	return resultList;
}

float FGsItemEnchantManager::GetEnchantItemRate(const ItemEnchantResult InResultType, const int32 InRateId, const int32 InItemLevel)
{
	int32 resultRate = 0;
	if (_enchantRateDataMap.Contains(InRateId))
	{
		TMap<int32, const FGsSchemaItemEquipEnchantRate*> data = _enchantRateDataMap[InRateId];
		if (data.Contains(InItemLevel))
		{
			const FGsSchemaItemEquipEnchantRate* tableData = data[InItemLevel];
			if (nullptr != tableData)
			{
				switch (InResultType)
				{
				case ItemEnchantResult::ENCHANT_SUCCEEDED:
					resultRate = tableData->successRate;
					break;
				case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED:
					resultRate = tableData->blessRate;
					break;
				case ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED:
					resultRate = tableData->curseRate;
					break;
				case ItemEnchantResult::ENCHANT_PRESERVED:
					resultRate = tableData->preserveRate;
					break;
				case ItemEnchantResult::ENCHANT_FAILED_DOWN:
					resultRate = tableData->downRate;
					break;
				case ItemEnchantResult::ENCHANT_FAILED:
				case ItemEnchantResult::ENCHANT_FAILED_SHIELDED:
				case ItemEnchantResult::ENCHANT_FAILED_DESTROYED:
					resultRate = tableData->failRate;
					break;
				}
			}

			if (0 < resultRate)
			{
				float rate = (((float)resultRate / 10000.f) * 100.f) + 0.01f;
				return rate;
			}
		}
	}

	return resultRate;
}

int32 FGsItemEnchantManager::GetOriEnchantItemRate(const ItemEnchantResult InResultType, const int32 InRateId, const int32 InItemLevel)
{
	return 0;
}

int32 FGsItemEnchantManager::GetEnchantResultDeltaValue(FGsItemIngredient* InIngredientItem)
{
	if (nullptr == InIngredientItem)
		return 0;

	if (ItemEffectType::ITEM_ENCHANT != InIngredientItem->GetEffectType())
		return 0;

	if (_enchantResultDeltaValueMap.Contains(ConvertResultTypeByEnchantType(InIngredientItem->GetItemEnchantType())))
	{
		return _enchantResultDeltaValueMap[ConvertResultTypeByEnchantType(InIngredientItem->GetItemEnchantType())];
	}
	return 0;
}

const ItemEnchantResult FGsItemEnchantManager::ConvertResultTypeByEnchantType(const ItemEnchantType InEnchantType)
{
	switch (InEnchantType)
	{
	case ItemEnchantType::NORMAL_ENCHANT:
		return ItemEnchantResult::ENCHANT_SUCCEEDED;
	case ItemEnchantType::BLESSED_ENCHANT:
		return ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED;
	case ItemEnchantType::CURSED_ENCHANT:
		return ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED;
	}

	return ItemEnchantResult::ENCHANT_SUCCEEDED;
}

/*
	NONE = 0,	// 강화 발생하지 않음
	ENCHANT_SUCCEEDED = 1,	// 강화 성공
	ENCHANT_SUCCEEDED_CURSED = 2,	// 강화 성공 - 레벨 차감
	ENCHANT_SUCCEEDED_BLESSED = 3,	// 강화 성공 - 대성공
	ENCHANT_FAILED = 4,	// 강화 실패
	ENCHANT_FAILED_SHIELDED = 5,	// 강화 실패 - 내구도 소모
	ENCHANT_FAILED_DESTROYED = 6,	// 강화 실패 - 파괴
*/
// 단일 강화 응답 패킷
void FGsItemEnchantManager::UpdateSingleEnchantItem(const ItemDataEquip* InTargetEquipItemData, const uint64 InScollItemDbid, const ItemEnchantResult InAckResult)
{
	if (nullptr != InTargetEquipItemData)
	{
		UGsItemManager* itemMgr = GItem();
		Level PrevLevel = 0;
		switch (InAckResult)
		{
		case ItemEnchantResult::ENCHANT_SUCCEEDED:
		case ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED:
		case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED:
		case ItemEnchantResult::ENCHANT_FAILED_SHIELDED:
		case ItemEnchantResult::ENCHANT_PRESERVED:
		case ItemEnchantResult::ENCHANT_FAILED_DOWN:
		{
			// 아이템 변경 사항 적용
			TWeakPtr<FGsItem> findItem = itemMgr->FindItem(InTargetEquipItemData->mItemDBId, ItemType::EQUIP);
			if (findItem.IsValid())
			{
				FGsItem* itemData = findItem.Pin().Get();
				if (nullptr != itemData)
				{
					PrevLevel = itemData->GetLevel();
					itemMgr->UpdateEquipItem(TArray<const ItemDataEquip*> { InTargetEquipItemData });
				}
			}
			break;
		}

		case ItemEnchantResult::ENCHANT_FAILED_DESTROYED:
			break;
		}

		FGsUpdateEnchantItem param(InTargetEquipItemData->mItemDBId, InScollItemDbid, InAckResult, PrevLevel);
		GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::INVALIDATE, &param);
	}
	else
	{
		GSLOG(Error, TEXT("[FGsItemEnchantManager::UpdateEquipItemEnchant] nullptr == InTargetEquipItemData"));
	}
}

void FGsItemEnchantManager::UpdateMultiEnchantItem(TArray<ItemEnchantResultData> InEnchantResultDataList, TArray<ItemDBId>& InUseIngredientItemDbidList)
{
	UGsItemManager* itemMgr = GItem();
	TArray<const ItemDataEquip*> updateEquipItemList;
	for (ItemEnchantResultData& iter : InEnchantResultDataList)
	{
		switch (iter.mItemEnchantResult)
		{
		case ItemEnchantResult::ENCHANT_SUCCEEDED:
		case ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED:
		case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED:
		case ItemEnchantResult::ENCHANT_FAILED_SHIELDED:
		case ItemEnchantResult::ENCHANT_PRESERVED:
		case ItemEnchantResult::ENCHANT_FAILED_DOWN:
			updateEquipItemList.Emplace(&iter.mItemDataEquip);		
			break;
		}
	}

	itemMgr->UpdateEquipItem(updateEquipItemList);

	FGsUpdateMultiEnchantItem param(InEnchantResultDataList, InUseIngredientItemDbidList);
	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::INVALIDATE_MULTI, &param);
}

void FGsItemEnchantManager::UpdateRefineOptionItem(const ItemDataEquip* InTargetEquipItemData, const ItemRefineResult InAckResult)
{
	if (ItemRefineResult::NONE == InAckResult)
		return;

	// 아이템 변경 사항 적용
	TArray<const ItemDataEquip*> updateEquipItemList;
	updateEquipItemList.Emplace(InTargetEquipItemData);
	GItem()->UpdateEquipItem(updateEquipItemList);

	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::INVALIDATE_REFINE, nullptr);
}

void FGsItemEnchantManager::UpdateMagicalForgeItem(const ItemDataEquip* InTargetEquipItemData, const ItemMagicalForgeResult InAckResult)
{
	if (ItemMagicalForgeResult::NONE != InAckResult)
	{
		// 아이템 변경 사항 적용
		TArray<const ItemDataEquip*> updateEquipItemList;
		updateEquipItemList.Emplace(InTargetEquipItemData);
		GItem()->UpdateEquipItem(updateEquipItemList);
	}

	FGsUpdateMagicalForgeItem updateMagicalForgeItem(InTargetEquipItemData->mItemDBId, InAckResult);
	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::INVALIDATE_MAGICALFORGE, &updateMagicalForgeItem);
}

void FGsItemEnchantManager::GetEnchantDefaultStatInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, OUT TArray<DisplayDefaultStatData>& outStatDataList)
{
	outStatDataList.Empty();
	if (InEquipData.IsValid() && InIngredientData.IsValid())
	{
		FGsItemEquipment* equipData = InEquipData.Pin()->GetCastItem<FGsItemEquipment>();
		FGsItemIngredient* ingredientData = InIngredientData.Pin()->GetCastItem<FGsItemIngredient>();
		if (equipData && ingredientData)
		{
			TArray<FGsItemStatData> curStatList;
			TArray<FGsItemStatData> expectStatList;
			TArray<FGsItemStatData> expectStatListBlessed;
			if (ItemEnchantType::NORMAL_ENCHANT == ingredientData->GetItemEnchantType()
				|| ItemEnchantType::CURSED_ENCHANT == ingredientData->GetItemEnchantType())
			{
				int32 level = ItemEnchantType::CURSED_ENCHANT == ingredientData->GetItemEnchantType() ?
					(0 < equipData->GetLevel() ? equipData->GetLevel() - 1 : equipData->GetLevel()) : (equipData->GetLevel() < equipData->GetMaxLevel() ? 
						equipData->GetLevel() + 1 : equipData->GetLevel());

				equipData->GetEnchantDisplayStatListByLevel(equipData->GetLevel(), curStatList);
				equipData->GetEnchantDisplayStatListByLevel(level, expectStatList);
				equipData->CheckStatList(curStatList, expectStatList);
				for (int32 i = 0; i < curStatList.Num(); ++i)
				{
					if (expectStatList.IsValidIndex(i))
					{
						DisplayDefaultStatData statDataInfo;
						statDataInfo._isCursedScroll = ItemEnchantType::CURSED_ENCHANT == ingredientData->GetItemEnchantType();
						statDataInfo._type = curStatList[i].type;
						statDataInfo._currentValue = curStatList[i].value;
						statDataInfo._minExpectValue = expectStatList[i].value;
						statDataInfo._maxExpectValue = 0;

						outStatDataList.Emplace(statDataInfo);
					}
				}
			}
			else
			{
				int32 daltaValue = GetEnchantResultDeltaValue(ingredientData);
				int32 level = (equipData->GetLevel() + daltaValue) <= equipData->GetMaxLevel() ?
					equipData->GetLevel() + daltaValue : equipData->GetLevel() + ((equipData->GetLevel() + daltaValue) - equipData->GetMaxLevel());

				equipData->GetEnchantDisplayStatListByLevel(equipData->GetLevel(), curStatList);
				equipData->GetEnchantDisplayStatListByLevel(equipData->GetLevel() + 1, expectStatList);
				equipData->GetEnchantDisplayStatListByLevel(level, expectStatListBlessed);
				// +1 예상스텟 : +n 예상스텟  중, 스텟이 더 많이 있는 리스트 기준으로 맞춤
				equipData->CheckStatList(curStatList, expectStatListBlessed);
				equipData->CheckStatList(expectStatList, expectStatListBlessed);
				for (int32 i = 0; i < curStatList.Num(); ++i)
				{
					if (expectStatList.IsValidIndex(i) && expectStatListBlessed.IsValidIndex(i))
					{
						DisplayDefaultStatData statDataInfo;
						statDataInfo._isCursedScroll = ItemEnchantType::CURSED_ENCHANT == ingredientData->GetItemEnchantType();
						statDataInfo._type = curStatList[i].type;
						statDataInfo._currentValue = curStatList[i].value;
						statDataInfo._minExpectValue = expectStatList[i].value;
						statDataInfo._maxExpectValue = expectStatListBlessed[i].value;

						outStatDataList.Emplace(statDataInfo);
					}
				}
			}
		}
	}
}

bool FGsItemEnchantManager::GetEnchantPropertyStatInfo(FGsItemEquipment* InEquipData, TWeakPtr<FGsItem> InIngredientData, OUT TArray<DisplayPropertyStatData>& outStatDataList)
{
	outStatDataList.Empty();
	if (InEquipData && InIngredientData.IsValid())
	{
		FGsItemIngredient* ingredientData = InIngredientData.Pin()->GetCastItem<FGsItemIngredient>();
		if (ingredientData)
		{
			const FString strTitle = InEquipData->GetBonusStatTitleName();
			if (strTitle.IsEmpty())
			{
				return false;
			}
			FGsItemEnchantManager* enchantMgr = GSItemEnchant();
			TArray<int32> levelList = InEquipData->FindAllBonusStatLevel();
			for (uint8 i = 0; i < levelList.Num(); ++i)
			{
				TArray<FGsItemStatData> statDataList = InEquipData->FindBonusStatDataByLevel(levelList[i]);
				if (0 < statDataList.Num())
				{
					FGsItemStatData& statData = statDataList[0];
					DisplayPropertyStatData propertyStatInfo;
					propertyStatInfo._type = statData.type;
					propertyStatInfo._currentValue = statData.value;
					propertyStatInfo._earnStatLevel = levelList[i];
					propertyStatInfo._currentLevel = InEquipData->GetLevel();
					propertyStatInfo._deltaScrollValue = enchantMgr->GetEnchantResultDeltaValue(ingredientData);
					propertyStatInfo._isCursed = ItemEnchantType::CURSED_ENCHANT == ingredientData->GetItemEnchantType();
					propertyStatInfo._isLoseStat = InEquipData->GetLevel() >= levelList[i];

					outStatDataList.Emplace(propertyStatInfo);
				}
			}
			return true;
		}
	}
	return false;
}

bool FGsItemEnchantManager::GetEnchantSkillInfo(FGsItemEquipment* InEquipData, TWeakPtr<FGsItem> InIngredientData, OUT TOptional<TMap<Level, TArray<FText>>>& outSkillDataMap)
{
	TArray<TPair<uint8, const FGsSchemaPassivitySet*>> allOptionList = InEquipData->GetAllSkilOptionData();

	if (allOptionList.Num() < 1)
	{
		return false;
	}

	FGsPassivityManager* passivityMgr = GSPassivity();

	if (nullptr == passivityMgr)
	{
		GSLOG(Error, TEXT("nullptr == passivityMgr"));
		return false;
	}

	if (!outSkillDataMap.IsSet())
		outSkillDataMap.Emplace();

	for (const auto& pair : allOptionList)
	{
		if (!outSkillDataMap->Contains(pair.Key))
		{
			outSkillDataMap->Add(pair.Key);
		}
		(*outSkillDataMap)[pair.Key].Add(passivityMgr->GetSkillText(pair.Value));
	}

	return true;
}

bool FGsItemEnchantManager::IsMatchedEnchantItem(const ItemDBId InTargetItemDbid, const ItemDBId InComapreItemDbid, const bool InIsMultiEnchant)
{
	UGsItemManager* itemMgr = GItem();
	TWeakPtr<FGsItem> findTargetItem = itemMgr->FindItem(InTargetItemDbid);
	TWeakPtr<FGsItem> findCompareItem = itemMgr->FindItem(InComapreItemDbid);
	if (findTargetItem.IsValid() && findCompareItem.IsValid())
	{
		// 메인이 장착류 아이템 일 때
		if(findTargetItem.Pin()->IsEquipment())
		{
			if (findCompareItem.Pin()->IsIngredient())
			{
				FGsItemIngredient* ingredientItem = findCompareItem.Pin()->GetCastItem<FGsItemIngredient>();
				if (nullptr == ingredientItem)
					return false;

				if (ItemEffectType::ITEM_ENCHANT == ingredientItem->GetEffectType())
				{
					bool state = ingredientItem->IsMatchedEnchantTargetItem(findTargetItem.Pin()->GetSubCategory());
					//if (state)
					//{
					//	ItemEnchantType enchantType = ingredientItem->GetItemEnchantType();
					//	if (ItemEnchantType::CURSED_ENCHANT == enchantType && false == InIsMultiEnchant)
					//	{
					//		return 0 < findTargetItem.Pin()->GetLevel();
					//	}
					//}
					return state;
				}
				else if (ItemEffectType::OPTION_STONE == ingredientItem->GetEffectType() ||
					ItemEffectType::BLESSED_OPTION_STONE == ingredientItem->GetEffectType())
				{
					return ingredientItem->IsMatchedRefineTargetItem(findTargetItem.Pin()->GetSubCategory());
				}
			}
		}
		// 메인이 재료형 아이템
		if (findTargetItem.Pin()->IsIngredient())
		{
			if (findCompareItem.Pin()->IsEquipment())
			{
				FGsItemIngredient* ingredientItem = findTargetItem.Pin()->GetCastItem<FGsItemIngredient>();
				if (nullptr == ingredientItem)
					return false;

				if (ItemEffectType::ITEM_ENCHANT == ingredientItem->GetEffectType())
				{
					bool state = ingredientItem->IsMatchedEnchantTargetItem(findCompareItem.Pin()->GetSubCategory());
					//if (state)
					//{
					//	ItemEnchantType enchantType = ingredientItem->GetItemEnchantType();
					//	if (ItemEnchantType::CURSED_ENCHANT == enchantType && false == InIsMultiEnchant)
					//	{
					//		return 0 < findCompareItem.Pin()->GetLevel();
					//	}
					//}
					return state;
				}
				else if (ItemEffectType::OPTION_STONE == ingredientItem->GetEffectType() ||
					ItemEffectType::BLESSED_OPTION_STONE == ingredientItem->GetEffectType())
				{
					return ingredientItem->IsMatchedRefineTargetItem(findCompareItem.Pin()->GetSubCategory());
				}
			}
		}
	}
	return false;
}

bool FGsItemEnchantManager::IsMatchedEnchantItem(EInventoryTab InCurInvenTabType, const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid, const ItemDBId InInvenItemDbid)
{
	UGsItemManager* itemMgr = GItem();
	TWeakPtr<FGsItem> findEquipItem = itemMgr->FindItem(InTargetItemDbid, ItemType::EQUIP);
	TWeakPtr<FGsItem> findIngredientItem = itemMgr->FindItem(InIngredientItemDbid, ItemType::INGREDIENT);
	TWeakPtr<FGsItem> findInvenItem = itemMgr->FindItem(InInvenItemDbid, EInventoryTab::EquipmentTab == InCurInvenTabType ?
		ItemType::EQUIP : ItemType::INGREDIENT);

	if (false == findInvenItem.IsValid())
		return false;

	if (EInventoryTab::EquipmentTab == InCurInvenTabType) // 장착탭
	{
		if (findEquipItem.IsValid())
		{
			if (false == findIngredientItem.IsValid())
			{
				return true;
			}
			if (findEquipItem.Pin()->GetMainCategory() != findInvenItem.Pin()->GetMainCategory())
			{
				return false;
			}
		}
		if (findIngredientItem.IsValid())
		{
			return IsMatchedEnchantItem(findIngredientItem.Pin()->GetDBID(), findInvenItem.Pin()->GetDBID(),true);
		}
	}
	else // 재료탭
	{
		if (false == findIngredientItem.IsValid())
		{
			return true;
		}
		if (findEquipItem.IsValid())
		{
			if (false == IsMatchedEnchantItem(findEquipItem.Pin()->GetDBID(), findInvenItem.Pin()->GetDBID(), true))
			{
				return false;
			}
		}
	}
	return true;
}

bool FGsItemEnchantManager::IsValidRefine(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid)
{
	UGsItemManager* itemMgr = GItem();
	TWeakPtr<FGsItem> findEqupiItem = itemMgr->FindItem(InTargetItemDbid, ItemType::EQUIP);
	TWeakPtr<FGsItem> findIngredientItem = itemMgr->FindItem(InIngredientItemDbid, ItemType::INGREDIENT);
	if (false == findEqupiItem.IsValid() || false == findIngredientItem.IsValid())
		return false;

	FGsItemEquipment* equipData = findEqupiItem.Pin()->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipData)
		return false;

	if (true == equipData->GetLock())
	{
		// 잠금 처리된 아이템은 재련할 수 없습니다
		FText findText;
		FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_Refinemnet_Ticker_ItemLocked"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	if (true == findIngredientItem.Pin()->IsExpiredTime() || true == equipData->IsExpiredTime() || findIngredientItem.Pin()->GetLock())
	{
		// 사용할 수 없습니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_CannotUse"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	// cost 체크
	uint64 ownedCurrency = FGsCurrencyHelper::GetCurrencyAmount(equipData->GetRefineOptionCostType());
	int32 cost = equipData->GetRefineOptionCostValue();
	if (ownedCurrency < cost)
	{
		// 돈이 없슴
		//FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, cost - ownedCurrency);
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_Notice_NotEnoughGold"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	return true;
}

Level FGsItemEnchantManager::GetEnchantLevel(const ItemDBId InTargetItemDbid)
{
	UGsItemManager* itemMgr = GItem();
	TWeakPtr<FGsItem> findEqupiItem = itemMgr->FindItem(InTargetItemDbid, ItemType::EQUIP);
	if (false == findEqupiItem.IsValid())
	{
		GSLOG(Error, TEXT("false == findEqupiItem.IsValid()"));
		return false;
	}

	FGsItemEquipment* equipData = findEqupiItem.Pin()->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipData)
	{
		GSLOG(Error, TEXT("nullptr == equipData"));
		return false;
	}

	return equipData->GetLevel();
}

bool FGsItemEnchantManager::IsValidSingleEnchant(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid, int32& InExpectLevel)
{
	UGsItemManager* itemMgr = GItem();
	TWeakPtr<FGsItem> findEqupiItem = itemMgr->FindItem(InTargetItemDbid, ItemType::EQUIP);
	TWeakPtr<FGsItem> findIngredientItem = itemMgr->FindItem(InIngredientItemDbid, ItemType::INGREDIENT);
	if (false == findEqupiItem.IsValid() || false == findIngredientItem.IsValid())
		return false;

	FGsItemEquipment* equipData = findEqupiItem.Pin()->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipData)
		return false;

	FGsItemIngredient* ingredientItem = findIngredientItem.Pin()->GetCastItem<FGsItemIngredient>();
	if (nullptr == ingredientItem)
		return false;

	InExpectLevel = 0 >= InExpectLevel ? equipData->GetLevel() + 1 : InExpectLevel;
	
	/*
	// 스턴상태 return 처리
	if (true == itemMgr->GetInvalidUserStatusflag())
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	*/

	int32 curLevel = equipData->GetLevel();
	int32 resultLevel = (ItemEnchantType::BLESSED_ENCHANT == ingredientItem->GetItemEnchantType() || ItemEnchantType::CURSED_ENCHANT == ingredientItem->GetItemEnchantType()) ? 
		1 : InExpectLevel - curLevel;

	if (true == equipData->GetLock())
	{
		// 잠긴 아이템은 강화할수없습니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_Locked"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	if (true == ingredientItem->IsExpiredTime() || true == equipData->IsExpiredTime())
	{
		// 사용할 수 없습니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_CannotUse"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	if (equipData->GetLevel() >= equipData->GetMaxLevel() && ItemEnchantType::CURSED_ENCHANT != ingredientItem->GetItemEnchantType())
	{
		// 더 이상 강화할 수 없습니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_MaxLevel"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	if (equipData->IsMinLevel() && ItemEnchantType::CURSED_ENCHANT == ingredientItem->GetItemEnchantType())
	{
		// 티커 메세지 출력
		// 더 이상 저주받은 주문서를 사용할 수 없습니다.
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_CursedToLevel0"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	// cost 체크
	uint64 ownedCurrency = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
	int32 cost = equipData->GetEnchantCostValue() * resultLevel;
	if (ownedCurrency < cost)
	{
		// 돈이 없슴
		//FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, cost - ownedCurrency);
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_Notice_NotEnoughGold"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	int32 curMaterialAmount = ingredientItem->GetAmount();
	if (resultLevel > curMaterialAmount)
	{
		// 스크롤 갯수가 부족합니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_NotEnoughScroll"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	// 기획의도 변경 되면 주석해지
	// 장착 중인 무기는 안전 강화 이상으로 강화 할 수 없습니다
	//if (equipData->IsEquipped())
	//{
	//	int32 levelcheck = equipData->GetLevel() + resultLevel;
	//	if ((levelcheck > equipData->GetSafeEnchantCount()) && (false == consumData->IsCursedScroll()))
	//	{
	//		// 티커 메세지 출력
	//		// 장착 중인 무기는 안전 강화 이상으로 강화 할 수 없습니다
	//		FText findText;
	//		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EquipWeaponEnchant"), findText);
	//		FGsUIHelper::TrayMessageTicker(findText);
	//		return false;
	//	}
	//}

	return true;
}

bool FGsItemEnchantManager::IsValidMultiEnchant(TWeakPtr<FGsItem> InValidCheckItemData)
{
	if (false == InValidCheckItemData.IsValid())
	{
		// 사용할 수 없습니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_CannotUse"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	/*
	// 스턴상태 return 처리
	if (true == itemMgr->GetInvalidUserStatusflag())
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	*/

	if (true == InValidCheckItemData.Pin()->GetLock())
	{
		// 잠긴 아이템은 강화할수없습니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), InValidCheckItemData.Pin()->IsEquipment() ? TEXT("ItemUi_Enchant_Notice_Locked") : TEXT("ItemUi_Notice_CannotUse"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	if (true == InValidCheckItemData.Pin()->IsExpiredTime())
	{
		// 사용할 수 없습니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_CannotUse"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	return true;
}

// 실제 강화되어야 하는 아이템들만 수집
// 주문서는 선택한 주문서 다 보냄
// 타겟 강화 수치는 강화되는 장착 아이템들중에 레벨 제일 낮은것 기준으로 +1 보내야함
bool FGsItemEnchantManager::PrepareMultiEnchantPacketData(TArray<ItemDBId> InEquipItemDbidList, TMap<ItemDBId, TPair<ItemId, ItemAmount>> InIngredientDataMap, bool InIsCursed, int32 InExpectLevel,
	OUT TArray<ItemDBId>& outEquipItemDbidList, OUT TArray<ItemDBIdAmountPair>& outIngredientItemDbidList, Level& outRealTargetLevel)
{
	UGsItemManager* itemMgr = GItem();
	for (TPair<ItemDBId, TPair<ItemId, ItemAmount>> iter : InIngredientDataMap)
	{
		TWeakPtr<FGsItem> findData = itemMgr->FindItem(iter.Key, ItemType::INGREDIENT);
		if (false == IsValidMultiEnchant(findData))
		{
			return false;
		}

		ItemDBIdAmountPair pairData(iter.Key, findData.Pin()->GetAmount());
		outIngredientItemDbidList.Emplace(pairData);
	}

	int32 realExpectLevel = 0;
	Level minLv = 999;
	Level maxLv = 0;
	TMap<Level, TArray<TWeakPtr<FGsItem>>> equipItemDataMap;
	for (const ItemDBId iter : InEquipItemDbidList)
	{
		TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter, ItemType::EQUIP);
		if(false == findItem.IsValid())
			continue;

		if (InIsCursed)
		{
			if (InExpectLevel >= findItem.Pin()->GetLevel() || 0 >= findItem.Pin()->GetLevel() || findItem.Pin()->GetMinLevel() >= findItem.Pin()->GetLevel())
				continue;
		}
		else
		{
			if (InExpectLevel <= findItem.Pin()->GetLevel() || findItem.Pin()->IsMaxLevel())
				continue;
		}

		if (false == IsValidMultiEnchant(findItem))
			return false;
		
		if (equipItemDataMap.Contains(findItem.Pin()->GetLevel()))
		{
			TArray<TWeakPtr<FGsItem>>& itemList = equipItemDataMap[findItem.Pin()->GetLevel()];
			itemList.Emplace(findItem);
		}
		else
		{
			minLv = FMath::Min(minLv, findItem.Pin()->GetLevel());
			maxLv = FMath::Max(maxLv, findItem.Pin()->GetLevel());

			TArray<TWeakPtr<FGsItem>> list;
			list.Emplace(findItem);
			equipItemDataMap.Emplace(findItem.Pin()->GetLevel(), list);
		}
	}
	equipItemDataMap.KeySort([](Level keyA, Level keyB)
		{
			// 정렬
			return keyA < keyB;
		}
	);
	
	realExpectLevel = InIsCursed ? (maxLv - 1) : (minLv + 1);
	if (0 > realExpectLevel)
	{
		FString temp = "realExpectLevel is Invalid";
		FGsUIHelper::TrayMessageTicker(FText::FromString(temp));
		return false;
	}

	outRealTargetLevel = realExpectLevel;
	if (equipItemDataMap.Contains(InIsCursed ? maxLv : minLv))
	{
		TArray<TWeakPtr<FGsItem>> dataList = equipItemDataMap[InIsCursed ? maxLv : minLv];
		if (0 < dataList.Num())
		{
			for (TWeakPtr<FGsItem> iter : dataList)
			{
				if (iter.IsValid())
				{
					outEquipItemDbidList.Emplace(iter.Pin()->GetDBID());
					//GSLOG(Error, TEXT("outEquipItemDbidList itemLevel : %d"), iter.Pin()->GetLevel());
				}
			}
		}
	}
	if (0 >= outEquipItemDbidList.Num() || 0 >= outIngredientItemDbidList.Num())
	{
		return false;
	}

	//GSLOG(Error, TEXT("realExpectLevel  %d"), realExpectLevel);
	
	return true;
}

bool FGsItemEnchantManager::IsContinueEnchant(TArray<ItemDBId> InEquipItemDbidList, bool InIsCursed, int32 InExpectLevel)
{
	if (0 >= InEquipItemDbidList.Num() || 0 > InExpectLevel)
		return false;

	UGsItemManager* itemMgr = GItem();
	for (const ItemDBId iter : InEquipItemDbidList)
	{
		TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter, ItemType::EQUIP);
		if(findItem.IsValid())
		{
			bool isContinue = InIsCursed ? findItem.Pin()->GetLevel() > InExpectLevel : findItem.Pin()->GetLevel() < InExpectLevel;
			if (isContinue)
			{
				return true;
			}
		}
	}
	return false;
}

bool FGsItemEnchantManager::IsRiskEnchantEffect(TWeakPtr<FGsItem> InTargetItemData, const ItemDBId InIngredientItemDbid)
{
	if (false == InTargetItemData.IsValid())
		return false;

	TWeakPtr<FGsItem> findIngredientItem = GItem()->FindItem(InIngredientItemDbid, ItemType::INGREDIENT);
	if (findIngredientItem.IsValid())
	{
		if (findIngredientItem.Pin()->IsCursedEnchantScrollItem())
		{
			return false;
		}
	}

	if (FGsItemEquipment* equipData = InTargetItemData.Pin()->GetCastItem<FGsItemEquipment>())
	{
		if (0 >= equipData->GetSafeEnchantCount())
		{
			return true;
		}
		else
		{
			return equipData->GetSafeEnchantCount() < equipData->GetLevel();
		}
	}

	return false;
}

// return - 0 : safe / 1 : preserved / 2 : destroy
uint8 FGsItemEnchantManager::IsRiskEnchant(TArray<ItemDBId> InEquipItemDbidList, const int32 InExpectLevel)
{
	if (0 >= InEquipItemDbidList.Num() || 0 > InExpectLevel)
		return 0;

	UGsItemManager* itemMgr = GItem();
	for (ItemDBId iter : InEquipItemDbidList)
	{
		TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter, ItemType::EQUIP);
		if (findItem.IsValid())
		{
			if (FGsItemEquipment* equipData = findItem.Pin()->GetCastItem<FGsItemEquipment>())
			{
				float failRate = GetEnchantItemRate(ItemEnchantResult::ENCHANT_FAILED, equipData->GetEnchantRateId(), InExpectLevel - 1);
				float preservedRate = GetEnchantItemRate(ItemEnchantResult::ENCHANT_PRESERVED, equipData->GetEnchantRateId(), InExpectLevel - 1);
				if (0.f < failRate)
				{
					return 2;
				}
				if (0.f < preservedRate)
				{
					return 1;
				}
				/*
				if (equipData->GetSafeEnchantCount() < InExpectLevel)
				{
					return true;
				}
				*/
			}
		}
	}
	return 0;
}

bool FGsItemEnchantManager::IsRiskEnchant(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid)
{
	UGsItemManager* itemMgr = GItem();
	TWeakPtr<FGsItem> findIngredientItem = itemMgr->FindItem(InIngredientItemDbid, ItemType::INGREDIENT);
	if (findIngredientItem.IsValid())
	{
		if (findIngredientItem.Pin()->IsCursedEnchantScrollItem())
		{
			return false;
		}
	}

	TWeakPtr<FGsItem> findEqupiItem = itemMgr->FindItem(InTargetItemDbid, ItemType::EQUIP);
	if (findEqupiItem.IsValid() && findIngredientItem.IsValid())
	{
		if (FGsItemEquipment* equipData = findEqupiItem.Pin()->GetCastItem<FGsItemEquipment>())
		{
			float failRate = GetEnchantItemRate(ItemEnchantResult::ENCHANT_FAILED, equipData->GetEnchantRateId(), equipData->GetLevel());
			return 0.f < failRate;
		}
	}

	return false;
}

uint64 FGsItemEnchantManager::PlayEffectSound(EnchantEffectSoundResType InResType, const float InSpeed)
{
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return 0;

	// play 하기전에 혹시나 출력중인 사운드가 있다면 중지 시킴
	AllStopSound();

	uint64 resId = soundPlayer->PlaySound2D(GetSoundResName(InResType), InSpeed);
	if (_enchantEffectSoundIdMap.Contains(InResType))
	{
		_enchantEffectSoundIdMap[InResType] = resId;
	}
	else
	{
		_enchantEffectSoundIdMap.Emplace(InResType, resId);
	}
	return resId;
}

void FGsItemEnchantManager::StopEffectSound(EnchantEffectSoundResType InResType)
{
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	if (_enchantEffectSoundIdMap.Contains(InResType))
	{
		uint64 id = _enchantEffectSoundIdMap[InResType];
		if (0 < id && true == soundPlayer->IsPlaying(id))
		{
			soundPlayer->StopSound(id);
			_enchantEffectSoundIdMap[InResType] = 0;
		}
	}
}

FString FGsItemEnchantManager::GetSoundResName(EnchantEffectSoundResType InResType)
{
	FString resultStr = "";
	switch (InResType)
	{
	case EnchantEffectSoundResType::SingleEnchant_Safe:
	case EnchantEffectSoundResType::Refine_Start:
		resultStr = "Enchant_Safe";
		break;
	case EnchantEffectSoundResType::SingleEnchant_Safe_Success:
	case EnchantEffectSoundResType::Refine_Success:
		resultStr = "Enchant_Safe_Success";
		break;
	case EnchantEffectSoundResType::SingleEnchant_NotSafe_Touch:
		resultStr = "Enchant_NotSafe_Touch";
		break;
	case EnchantEffectSoundResType::SingleEnchant_NotSafe_Wait:
		resultStr = "Enchant_Start";
		break;
	case EnchantEffectSoundResType::MultiEnchant_Start:
		resultStr = "UI_Enchant_Multi_Start";
		break;
	case EnchantEffectSoundResType::MultiEnchant_End:
		resultStr = "UI_Enchant_Multi_Result"; 
		break;
	case EnchantEffectSoundResType::Enchant_Process: // 박승훈님 별도 요청으로 참 강화할때만 이 사운드 붙여달라고 요청옴
		resultStr = "SkillEnchant_Process";
		break;
	}

	return resultStr;
}

void FGsItemEnchantManager::AllStopSound()
{
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	StopEffectSound(EnchantEffectSoundResType::SingleEnchant_Safe);
	StopEffectSound(EnchantEffectSoundResType::SingleEnchant_Safe_Success);
	StopEffectSound(EnchantEffectSoundResType::SingleEnchant_NotSafe_Touch);
	StopEffectSound(EnchantEffectSoundResType::SingleEnchant_NotSafe_Wait);
	StopEffectSound(EnchantEffectSoundResType::Refine_Start);
	StopEffectSound(EnchantEffectSoundResType::Refine_Success);
	StopEffectSound(EnchantEffectSoundResType::MultiEnchant_Start);
	StopEffectSound(EnchantEffectSoundResType::MultiEnchant_End);
}

void FGsItemEnchantManager::FindMultiEnchantExpectNumState(TArray<ItemDBId> InItemDbidList, ItemId InIngredientItemTid, const uint8 InNumMaxCount, OUT TArray<bool>& outStateList)
{
	outStateList.Empty();
	for (uint8 i = 0; i < InNumMaxCount; ++i)
	{
		outStateList.Emplace(false);
	}
	uint8 minLv = 0;
	uint8 maxLv = 0;
	TArray<TWeakPtr<FGsItem>> findItemList;
	const FGsSchemaItemCommon* tableData = FGsItem::GetTableDataByID(InIngredientItemTid);
	if (nullptr != tableData)
	{
		UGsItemManager* itemMgr = GItem();
		bool isCursed = tableData->categorySub == ItemCategorySub::CURSED_ENCHANT_ACCESSORY || tableData->categorySub == ItemCategorySub::CURSED_ENCHANT_ARMOR ||
			tableData->categorySub == ItemCategorySub::CURSED_ENCHANT_WEAPON;

		for (ItemDBId iter : InItemDbidList)
		{
			TWeakPtr<FGsItem> findItemData = itemMgr->FindItem(iter, ItemType::EQUIP);
			if (findItemData.IsValid())
			{
				findItemList.Emplace(findItemData);
			}
		}
		//findItemList.Sort([](TWeakPtr<FGsItem> itemA, TWeakPtr<FGsItem> itemB)
		//	{
		//		return itemB.Pin()->GetLevel() > itemA.Pin()->GetLevel();
		//	}
		//);
		//if (0 < findItemList.Num())
		//{
		//	TWeakPtr<FGsItem> itemData = findItemList[0];
		//	minLv = itemData.Pin()->GetLevel();
		//	itemData = findItemList.Last();
		//	maxLv = itemData.Pin()->GetLevel();
		//	for (uint8 i = 0; i < outStateList.Num(); ++i)
		//	{
		//		outStateList[i] = isCursed ? i < maxLv : i >= minLv;
		//	}
		//}

		if (isCursed)
		{
			for (TWeakPtr<FGsItem> itemData : findItemList)
			{
				Level currentLevel = itemData.Pin()->GetLevel();
				Level minLevel = itemData.Pin()->GetMinLevel();

				if (currentLevel > minLevel)
				{
					if (maxLv < currentLevel)
					{
						maxLv = currentLevel;
					}
				}
				
				if (minLv > minLevel)
				{
					minLv = minLevel;
				}
			}

			for (uint8 i = minLv; i < outStateList.Num(); ++i)
			{
				outStateList[i] = i < maxLv;
			}
		}
		else
		{
			minLv = InNumMaxCount;
			for (TWeakPtr<FGsItem> itemData : findItemList)
			{
				Level currentLevel = itemData.Pin()->GetLevel();
				Level maxLevel = itemData.Pin()->GetMaxLevel();
				
				if (currentLevel < maxLevel)
				{
					if (minLv > currentLevel)
					{
						minLv = currentLevel;
					}
				}

				if (maxLv < maxLevel)
				{
					maxLv = maxLevel;
				}
			}

			for (uint8 level = minLv; level < outStateList.Num(); ++level)
			{
				outStateList[level] = level < maxLv;
			}
		}
	}
}

ItemAmount FGsItemEnchantManager::GetMultiEnchantNeedScrollCount(TArray<ItemDBId> InItemDbidList, const bool InIsCursed, const int32 InExpectEnchantCount)
{
	if (0 > InExpectEnchantCount)
		return 0;

	ItemAmount resultCount = 0;
	UGsItemManager* itemMgr = GItem();
	for (ItemDBId iter : InItemDbidList)
	{
		TWeakPtr<FGsItem> itemData = itemMgr->FindItem(iter, ItemType::EQUIP);
		if (itemData.IsValid())
		{
			if (InIsCursed)
			{
				if(InExpectEnchantCount >= itemData.Pin()->GetLevel())
					continue;
				
				resultCount += (itemData.Pin()->GetLevel() - InExpectEnchantCount);

				if (InExpectEnchantCount < itemData.Pin()->GetMinLevel())
				{
					resultCount -= (itemData.Pin()->GetMinLevel() - InExpectEnchantCount);
				}
			}
			else
			{
				if (InExpectEnchantCount <= itemData.Pin()->GetLevel())
					continue;

				resultCount += (InExpectEnchantCount - itemData.Pin()->GetLevel());

				if (InExpectEnchantCount > itemData.Pin()->GetMaxLevel())
				{
					resultCount -= (InExpectEnchantCount - itemData.Pin()->GetMaxLevel());
				}
			}
		}
	}
	return resultCount;
}

ItemAmount FGsItemEnchantManager::GetMultiEnchantNeedScrollCount(TWeakPtr<FGsItem> InItemData, const bool InIsCursed, const int32 InExpectEnchantCount)
{
	if (0 > InExpectEnchantCount)
		return 0;

	if (InItemData.IsValid())
	{
		if (InIsCursed)
		{
			if (InExpectEnchantCount >= InItemData.Pin()->GetLevel())
				return 0;

			ItemAmount resultCount = (InItemData.Pin()->GetLevel() - InExpectEnchantCount);

			if (InExpectEnchantCount < InItemData.Pin()->GetMinLevel())
			{
				resultCount -= (InItemData.Pin()->GetMinLevel() - InExpectEnchantCount);
			}
			return resultCount;
		}
		else
		{
			if (InExpectEnchantCount <= InItemData.Pin()->GetLevel())
				return 0;

			ItemAmount resultCount = (InExpectEnchantCount - InItemData.Pin()->GetLevel());

			if (InExpectEnchantCount > InItemData.Pin()->GetMaxLevel())
			{
				resultCount -= (InExpectEnchantCount - InItemData.Pin()->GetMaxLevel());
			}
			return resultCount;
		}
	}
	return 0;
}

bool FGsItemEnchantManager::IsOpenItemEnchangWidget()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		return uiMgr->IsActiveWidget(TEXT("PopupItemEnchant"));
	}
	return false;
}

bool FGsItemEnchantManager::IsOkInvenCondition()
{
	if (UGsItemManager* itemMgr = GItem())
	{
		if (itemMgr->IsInvenSlotFull() || itemMgr->IsInvenWeightFull())
		{
			return false;
		}
		return true;
	}
	return false;
}

bool FGsItemEnchantManager::IsNotAllowedMultiEnchantItem(const ItemDBId InCheckItemDbid)
{
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InCheckItemDbid);
	if (findItem.IsValid())
	{
		if (findItem.Pin()->IsEquipment())
		{
			return ItemCategorySub::DURAL == findItem.Pin()->GetSubCategory();
		}
		else
		{
			if (FGsItemIngredient* ingredientItem = findItem.Pin()->GetCastItem<FGsItemIngredient>())
			{
				return true == ingredientItem->IsMatchedEnchantTargetItem(ItemCategorySub::DURAL);
			}
		}
	}
	return false;
}






/*
void FGsItemEnchantManager::GetEnchantExpectRateInfo(TWeakPtr<FGsItem> InEquipItemData, TWeakPtr<FGsItem> InIngredientItemData, OUT TMap<TPair<Level, Level>, float>& outRateInfoMap)
{
	outRateInfoMap.Empty();

	FGsItemEquipment* equipDatap = InEquipItemData.Pin()->GetCastItem<FGsItemEquipment>();
	FGsItemIngredient* ingredientData = InIngredientItemData.Pin()->GetCastItem<FGsItemIngredient>();
	int32 value = GetEnchantResultDeltaValue(ingredientData);
	if (0 == value)
		return;

	TArray<float> rateResultList;
	switch (ingredientData->GetItemEnchantType())
	{
	case ItemEnchantType::NORMAL_ENCHANT:
	{
		if ((equipDatap->GetLevel() + value) > equipDatap->GetMaxLevel())
			return;

		rateResultList = GetDisplayEnchantItemRate(equipDatap->GetEnchantRateId(), equipDatap->GetLevel(), ItemEnchantType::NORMAL_ENCHANT);
		if (rateResultList.IsValidIndex(0))
		{
			outRateInfoMap.Emplace(TPair<Level, Level>(equipDatap->GetLevel(), equipDatap->GetLevel() + 1), rateResultList[0]);
		}
	}
	break;
	case ItemEnchantType::BLESSED_ENCHANT:
	{
		if ((equipDatap->GetLevel() + 1) > equipDatap->GetMaxLevel())
			return;

		if ((equipDatap->GetLevel() + value) > equipDatap->GetMaxLevel())
		{
			rateResultList = GetDisplayEnchantItemRate(equipDatap->GetEnchantRateId(), equipDatap->GetLevel(), ItemEnchantType::NORMAL_ENCHANT);
			if (rateResultList.IsValidIndex(0))
			{
				outRateInfoMap.Emplace(TPair<Level, Level>(equipDatap->GetLevel(), equipDatap->GetLevel() + 1), rateResultList[0]);
			}
			return;
		}

		rateResultList = GetDisplayEnchantItemRate(equipDatap->GetEnchantRateId(), equipDatap->GetLevel(), ItemEnchantType::BLESSED_ENCHANT);
		for (uint8 i = 0; i < rateResultList.Num(); ++i)
		{
			outRateInfoMap.Emplace(TPair<Level, Level>(equipDatap->GetLevel(), equipDatap->GetLevel() + (i + 1)), rateResultList[i]);
		}
	}
	break;
	case ItemEnchantType::CURSED_ENCHANT:
	{
		if (0 >= equipDatap->GetLevel())
			return;

		rateResultList = GetDisplayEnchantItemRate(equipDatap->GetEnchantRateId(), equipDatap->GetLevel(), ItemEnchantType::CURSED_ENCHANT);
		if (rateResultList.IsValidIndex(0))
		{
			outRateInfoMap.Emplace(TPair<Level, Level>(equipDatap->GetLevel(), equipDatap->GetLevel() - 1), rateResultList[0]);
		}
	}
	break;
	}
}
*/