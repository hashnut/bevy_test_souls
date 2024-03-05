// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemStatGroup.h"
#include "GsItemStat.h"
#include "T1Project.h"
#include "Util/GsTableUtil.h"
#include "DataSchema/Item/GsSchemaItemEquipRefineOption.h"
#include "DataSchema/Item/GsSchemaRefineOptionData.h"
#include "DataSchema/Item/GsSchemaItemEquipRefineOptionStatBalance.h"
#include "DataSchema/Item/GsSchemaItemEquipRefineBonusOption.h"
#include "DataSchema/Item/GsSchemaItemOptionSkillGroup.h"
#include "DataSchema/Item/GsSchemaItemOptionSkillLevelGroup.h"
#include "DataSchema/Item/GsSchemaLevelGroupList.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Management/GsScopeHolder.h"
#include "Data/GsItemEquipEnchantLoader.h"
#include "GsItemManager.h"
#include "../Management/GsScopeHolder.h"
#include "../Management/GsScopeGlobal.h"
#include "Item/Data/GsItemStatData.h"


FGsItemStatGroup::~FGsItemStatGroup()
{
	RemoveItemStatList();

	for (auto& iter : _itemBalanceStatMap)
	{
		for (uint8 i = 0; i < iter.Value.Num(); ++i)
		{
			delete iter.Value[i];
			iter.Value[i] = nullptr;
		}
	}

	_itemBalanceStatMap.Empty();
	_itemBonusDataMap.Empty();
	_itemSkillOptionList.Empty();
	_equipRefineOptionDataMap.Empty();
	_equipRefineOptionBalanceDataMap.Empty();

	_equipBonusStatData = nullptr;
}

FGsItemStatGroup::FGsItemStatGroup(const FGsSchemaItemEquipDetail* InEquipDetailData)
{
	_equipDetailData = InEquipDetailData;

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return;
	}

	if (0 < _equipDetailData->enchantBonusId)
	{
		const FGsItemEquipEnchantBonus* bonusData = ItemManager->GetItemEquipEnchantBonus(_equipDetailData->enchantBonusId);
		if (nullptr == bonusData)
		{
			GSLOG(Warning, TEXT("nullptr == bonusData, enchantBonusId : %d"), _equipDetailData->enchantBonusId);
			check(!!bonusData);
			return;
		}
		_equipBonusStatData = bonusData;
	}

	const FGsItemEquipEnchantBalance* balanceData = ItemManager->GetItemEquipEnchantBalance(_equipDetailData->enchantBalanceId);
	if (nullptr == balanceData)
	{
		GSLOG(Warning, TEXT("nullptr == balanceData, enchantBalanceId : %d"), _equipDetailData->enchantBalanceId);
		check(!!balanceData);
		return;
	}

	CreateItemEnchantBalanceData(balanceData);
	CreateItemBonusStatData();
	CreateSkillOptionData();

	for (uint8 i = 0; i < _equipDetailData->statList.Num(); ++i)
	{
		CreateItemStat(_equipDetailData->statList[i].type, _equipDetailData->statList[i].value);
	}

	for (uint8 i = 0; i < MAX_ITEM_REFINE_OPTION_COUNT; ++i)
	{
		FGsItemStat* newStatData = new FGsItemStat(StatType::NONE, 0);
		_itemRefineStatList.Emplace(newStatData);
	}

	_equipRefineOptionDataMap.Empty();
	for (int32 equipId : _equipDetailData->refineOptionId)
	{
		const FGsSchemaItemEquipRefineOption* refineOptionData = UGsTableUtil::FindRowById<UGsTableItemEquipRefineOption, FGsSchemaItemEquipRefineOption>(equipId);
		if (nullptr == refineOptionData)
		{
			GSLOG(Warning, TEXT("FGsSchemaItemEquipRefineOption Data is nullptr - refineOptionId : %d"), equipId);
			check(refineOptionData);
			return;
		}
		_equipRefineOptionDataMap.Emplace(refineOptionData->refineOptionId, refineOptionData);
	}

	_equipRefineOptionBalanceDataMap.Empty();
	const UGsTableItemEquipRefineOptionStatBalance* table = Cast<UGsTableItemEquipRefineOptionStatBalance>(FGsSchemaItemEquipRefineOptionStatBalance::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Warning, TEXT("FGsSchemaItemEquipRefineOptionStatBalance Data is nullptr"));
		check(table);
		return;
	}
	TArray<const FGsSchemaItemEquipRefineOptionStatBalance*> allrow;
	if (false == table->GetAllRows(allrow))
	{
		GSLOG(Warning, TEXT("FGsSchemaItemEquipRefineOptionStatBalance false == GetAllRows"));
		check(false);
		return;
	}
	for (const FGsSchemaItemEquipRefineOptionStatBalance* iter : allrow)
	{
		if(nullptr == iter)
			continue;

		_equipRefineOptionBalanceDataMap.Emplace(iter->refineOptionStatId, iter);
	}
}

void FGsItemStatGroup::RemoveItemStatList()
{
	for (uint8 i = 0; i < _itemStatList.Num(); ++i)
	{
		if (nullptr != _itemStatList[i])
		{
			delete _itemStatList[i];
			_itemStatList[i] = nullptr;
		}
	}

	for (uint8 i = 0; i < _itemRefineStatList.Num(); ++i)
	{
		if (nullptr != _itemRefineStatList[i])
		{
			delete _itemRefineStatList[i];
			_itemRefineStatList[i] = nullptr;
		}
	}

	delete _itemRefineBonusInfo;
	_itemRefineBonusInfo = nullptr;

	_itemStatList.Empty();
	_itemRefineStatList.Empty();
}

void FGsItemStatGroup::UpdateEquipItemStat(const uint16 InItemLevel)
{
	// 기존 정보 제거
	for (uint8 i = 0; i < _itemStatList.Num(); ++i)
	{
		if (nullptr != _itemStatList[i])
		{
			delete _itemStatList[i];
			_itemStatList[i] = nullptr;
		}
	}
	_itemStatList.Empty();

	// 기본 스탯
	for (uint8 i = 0; i < _equipDetailData->statList.Num(); ++i)
	{
		CreateItemStat(_equipDetailData->statList[i].type, _equipDetailData->statList[i].value);
	}

	// 강화 스탯
	TArray<FGsItemStat*> statDataList = GetItemStatWithBalanceValue(InItemLevel);
	for (uint8 i = 0; i < statDataList.Num(); ++i)
	{
		if (nullptr == statDataList[i])
			continue;

		CreateItemStat(statDataList[i]->GetStatType(), statDataList[i]->GetStatOriValue());
	}
}

void FGsItemStatGroup::UpdateRefineOptionStat(const StatInfo InStatInfoArray[], RefineBonusInfo InBonusInfo)
{
	//TArray<struct StatInfo*> _itemRefineStatInfoList;
	//struct RefineBonusInfo _itemRefineBonusInfo;
	if (nullptr == _itemRefineBonusInfo)
	{
		_itemRefineBonusInfo = new RefineBonusInfo(InBonusInfo.mBonusId, InBonusInfo.mValue);
	}
	else
	{
		_itemRefineBonusInfo->mBonusId = InBonusInfo.mBonusId;
		_itemRefineBonusInfo->mValue = InBonusInfo.mValue;
	}

	for (uint8 i = 0; i < MAX_ITEM_REFINE_OPTION_COUNT; ++i)
	{
		if (false == _itemRefineStatList.IsValidIndex(i))
			break;

		_itemRefineStatList[i]->SetStatType(InStatInfoArray[i].mType);
		_itemRefineStatList[i]->SetOriStatValue(InStatInfoArray[i].mValue);
	}
}

void FGsItemStatGroup::UpdateRefineOptionStat(const TArray<FGsItemStat*>& InItemStatArray, RefineBonusInfo InBonusInfo)
{
	if (nullptr == _itemRefineBonusInfo)
	{
		_itemRefineBonusInfo = new RefineBonusInfo(InBonusInfo.mBonusId, InBonusInfo.mValue);
	}
	else
	{
		_itemRefineBonusInfo->mBonusId = InBonusInfo.mBonusId;
		_itemRefineBonusInfo->mValue = InBonusInfo.mValue;
	}

	for (uint8 i = 0; i < MAX_ITEM_REFINE_OPTION_COUNT; ++i)
	{
		if (false == _itemRefineStatList.IsValidIndex(i))
			break;

		_itemRefineStatList[i]->SetStatType(InItemStatArray[i]->GetStatType());
		_itemRefineStatList[i]->SetOriStatValue(InItemStatArray[i]->GetStatOriValue());
	}
}

void FGsItemStatGroup::UpdateMagicalForge(const MagicalForgeData& InMagicalForgeData)
{
	_magicalForgeData = InMagicalForgeData;
}

void FGsItemStatGroup::CreateSkillOptionData()
{
	_itemSkillOptionList.Empty();
	if (nullptr == _equipDetailData || 0 >= _equipDetailData->itemOptionSkillGroupId)
		return;	

	const FGsSchemaItemOptionSkillGroup* skillGroupData = UGsTableUtil::FindRowById<UGsTableItemOptionSkillGroup, FGsSchemaItemOptionSkillGroup>(_equipDetailData->itemOptionSkillGroupId);
	if (nullptr == skillGroupData)
	{
		GSLOG(Warning, TEXT("FGsSchemaItemOptionSkillGroup Data is nullptr - itemOptionSkillGroupId : %d"), _equipDetailData->itemOptionSkillGroupId);
		return;
	}

	for (const int32& iter : skillGroupData->levelGroupIdList)
	{
		const FGsSchemaItemOptionSkillLevelGroup* skillLevelData = UGsTableUtil::FindRowById<UGsTableItemOptionSkillLevelGroup, FGsSchemaItemOptionSkillLevelGroup>(iter);
		if (nullptr == skillLevelData)
		{
			GSLOG(Warning, TEXT("FGsSchemaItemOptionSkillLevelGroup Data is nullptr - levelGroupIdList : %d"), iter);
			continue;
		}
		for (const FGsSchemaLevelGroupList& iterLevelData : skillLevelData->levelGroupList)
		{
			const FGsSchemaPassivitySet* passivitySetData = UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(iterLevelData.passivityId);
			if (nullptr == passivitySetData)
				continue;

			_itemSkillOptionList.Emplace(TPair<uint8,const FGsSchemaPassivitySet*>(iterLevelData.level, passivitySetData));
		}
	}
}

TArray<TPair<uint8, const FGsSchemaPassivitySet*>> FGsItemStatGroup::GetAllSkilOptionData()
{
	return _itemSkillOptionList;
}

bool FGsItemStatGroup::IsExistDefaultStat(StatType InType)
{
	for (uint8 i = 0; i < _equipDetailData->statList.Num(); ++i)
	{
		if (_equipDetailData->statList[i].type == InType)
		{
			return true;
		}
	}

	return false;
}

void FGsItemStatGroup::CreateItemStat(const StatType InType, const int32 InValue)
{
	bool isExist = false;
	for (uint8 i = 0; i < _itemStatList.Num(); ++i)
	{
		if (InType == _itemStatList[i]->GetStatType())
		{
			isExist = true;
			break;
		}
	}

	if (true == isExist)
	{
		UpdateItemStat(InType, InValue);
	}
	else
	{
		FGsItemStat* statData = new FGsItemStat(InType, InValue);
		_itemStatList.Emplace(statData);
	}
}

void FGsItemStatGroup::UpdateItemStat(const StatType InType, const int32 InValue)
{
	for (uint8 i = 0; i < _itemStatList.Num(); ++i)
	{
		if (nullptr == _itemStatList[i])
			continue;

		if (InType == _itemStatList[i]->GetStatType())
		{
			if (true == IsExistDefaultStat(InType))
			{
				_itemStatList[i]->SetCalcStatValue(InValue);
			}
			else
			{
				_itemStatList[i]->SetOriStatValue(InValue);
			}
		}
	}
}

void FGsItemStatGroup::CreateItemEnchantBalanceData(const FGsItemEquipEnchantBalance* InBalanceData)
{
	TArray<FGsItemStat*> itemStat;
	for (uint8 i = 0; i < InBalanceData->_levelBonusLists.Num(); ++i)
	{
		itemStat.Empty();
		TArray<FGsItemStatData> statDataList = InBalanceData->_levelBonusLists[i]._statDatas;
		for (uint8 j = 0; j < statDataList.Num(); ++j)
		{
			FGsItemStat* statData = new FGsItemStat(statDataList[j].type, statDataList[j].value);
			itemStat.Emplace(statData);
		}
		_itemBalanceStatMap.Emplace(InBalanceData->_levelBonusLists[i]._level, itemStat);
	}
}

void FGsItemStatGroup::CreateItemBonusStatData()
{
	if (nullptr == _equipBonusStatData)
		return;

	// Bonus Info Data
	// Bonus Stat Data
	uint8 index = 0;
	for (uint8 i = 0; i < _equipBonusStatData->_levelBonusGroups.Num(); ++i)
	{
		FGsLevelBonusGroup bonusGroup = _equipBonusStatData->_levelBonusGroups[i];
		_itemBonusDataMap.Emplace(index, bonusGroup);

		++index;
	}
}

const TMap<int32, TArray<FGsItemStatData>> FGsItemStatGroup::GetBonusStatByIndex(const uint16 InIndex)
{
	TMap<int32, TArray<FGsItemStatData>> resultDataMap;
	if (true == _itemBonusDataMap.Contains(InIndex))
	{
		FGsLevelBonusGroup groupData = _itemBonusDataMap[InIndex];
		for (uint8 i = 0; i < groupData._levelBonusLists.Num(); ++i)
		{
			resultDataMap.Emplace(groupData._levelBonusLists[i]._level, groupData._levelBonusLists[i]._statDatas);
		}
	}

	return resultDataMap;
}

const TArray<FGsItemStat*> FGsItemStatGroup::GetItemStatWithBalanceValue(const int32 InItemLevel)
{
	// DefaultStat + BalanceStat
	TArray<FGsItemStat*> statDataList = FindBalanceDataByItemLevel(InItemLevel);
	return statDataList;
}

const TArray<FGsItemStat*> FGsItemStatGroup::FindBalanceDataByItemLevel(const int32 InItemLevel)
{
	TArray<FGsItemStat*> statDataList;
	statDataList.Empty();

	if (true == _itemBalanceStatMap.Contains(InItemLevel))
	{
		statDataList = _itemBalanceStatMap[InItemLevel];
	}

	return statDataList;
}

bool FGsItemStatGroup::IsExistStat(StatType InType)
{
	bool found = false;
	for (uint8 i = 0; i < _itemStatList.Num(); ++i)
	{
		if (nullptr == _itemStatList[i])
			continue;

		if (InType == _itemStatList[i]->GetStatType())
		{
			found = true;
			break;
		}
	}
	return found;
}

const TArray<FGsItemStat*> FGsItemStatGroup::GetItemOriginalStatList()
{
	return _itemStatList;
}

const TArray<FGsItemStatData> FGsItemStatGroup::GetEnchantExpectStatList(const int32 InExpectItemLevel)
{
	TArray<FGsItemStatData> resultStatList;
	const TArray<FGsItemStat*> expectStatDataList = FindBalanceDataByItemLevel(InExpectItemLevel);
	for (uint8 i = 0; i < expectStatDataList.Num(); ++i)
	{
		FGsItemStat* data = expectStatDataList[i];
		if (nullptr == data)
			continue;

		FGsItemStatData statData;
		statData.type = data->GetStatType();
		statData.value = data->GetStatOriValue();
		resultStatList.Emplace(statData);
	}

	return resultStatList;
}

const FString FGsItemStatGroup::GetBonusStatTitleName(const uint16 InBonusIndex)
{
	if (true == _itemBonusDataMap.Contains(InBonusIndex))
	{
		FGsLevelBonusGroup groupData = _itemBonusDataMap[InBonusIndex];
		return groupData._name.ToString();
	}

	return "";
}

const EGsItemEnchantBonusType FGsItemStatGroup::GetBonusStatType(const uint16 InBonusIndex)
{
	if (FGsLevelBonusGroup* bonusGroup = _itemBonusDataMap.Find(InBonusIndex))
	{
		return bonusGroup->_enchantBonusType;
	}

	return EGsItemEnchantBonusType::NONE;
}

const int32 FGsItemStatGroup::GetOpenBonusStatLevelByStatType(const uint16 InBonusIndex, const StatType InStatType)
{
	if (true == _itemBonusDataMap.Contains(InBonusIndex))
	{
		FGsLevelBonusGroup groupData = _itemBonusDataMap[InBonusIndex];
		for (uint8 i = 0; i < groupData._levelBonusLists.Num(); ++i)
		{
			FGsLevelBonusList bonusListData = groupData._levelBonusLists[i];
			for (uint8 j = 0; j < bonusListData._statDatas.Num(); ++j)
			{
				if (InStatType == bonusListData._statDatas[j].type)
				{
					return bonusListData._level;
				}
			}
		}
	}

	return 0;
}

void FGsItemStatGroup::GetAllExpectRefineOptionStatList(const ItemCategorySub InRefineOptionStoneType, TArray<int32> InRefineOptionID, OUT TMap<StatType, TPair<int32, int32>>& outDataMap)
{
	outDataMap.Empty();
	for (const int32 iter : InRefineOptionID)
	{
		if (_equipRefineOptionDataMap.Contains(iter))
		{
			const FGsSchemaItemEquipRefineOption* optionData = _equipRefineOptionDataMap[iter];
			if (nullptr == optionData)
				continue;

			for (const FGsSchemaRefineOptionData subOptionDataIter : ItemCategorySub::OPTION_STONE == InRefineOptionStoneType ? optionData->refineOptionStatList : optionData->blessedRefineOptionStatList)
			{
				if (false == _equipRefineOptionBalanceDataMap.Contains(subOptionDataIter.refineOptionStatId))
					continue;

				const FGsSchemaItemEquipRefineOptionStatBalance* balanceData = _equipRefineOptionBalanceDataMap[subOptionDataIter.refineOptionStatId];
				if (nullptr == balanceData)
					continue;

				outDataMap.Emplace(balanceData->stat, TPair<int32, int32>(balanceData->displayMinValue, balanceData->displayMaxValue));
			}
		}
	}
}

void FGsItemStatGroup::GetAllExpectBonusRefineOptionStatList(TArray<int32> InBonusIdList, OUT TMap<StatType, TPair<int32,int32>>& outDataMap)
{ 
	outDataMap.Empty();
	for (const int32 iter : InBonusIdList)
	{
		if(0 >= iter)
			continue;

		const FGsSchemaItemEquipRefineBonusOption* bonusOptionData = UGsTableUtil::FindRowById<UGsTableItemEquipRefineBonusOption, FGsSchemaItemEquipRefineBonusOption>(iter);
		if (nullptr == bonusOptionData)
			return;

		StatType statType = bonusOptionData->statType.GetStatType();
		outDataMap.Emplace(statType, TPair<int32,int32>(bonusOptionData->displayMinValue , bonusOptionData->displayMaxValue));
	}
}

const TArray<FGsItemStat*>& FGsItemStatGroup::GetEquipItemRefineStatList()
{
	return _itemRefineStatList;
}

const bool FGsItemStatGroup::GetEquipItemRefineBonusStatData(OUT TPair<StatType, int32>& outPairData)
{
	if (nullptr == _itemRefineBonusInfo)
		return false;;

	const FGsSchemaItemEquipRefineBonusOption* bonusOptionData = UGsTableUtil::FindRowById<UGsTableItemEquipRefineBonusOption, FGsSchemaItemEquipRefineBonusOption>(_itemRefineBonusInfo->mBonusId);
	if (nullptr == bonusOptionData)
		return false;

	outPairData.Key = bonusOptionData->statType.GetStatType();
	outPairData.Value = _itemRefineBonusInfo->mValue;

	return true;
}

const RefineBonusInfo* FGsItemStatGroup::GetRefineBonusInfo()
{
	return _itemRefineBonusInfo;
}

bool FGsItemStatGroup::IsExistEnchantLevel(int32 InLevel)
{
	return _itemBalanceStatMap.Contains(InLevel);
}
