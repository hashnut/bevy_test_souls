// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemEquipment.h"
#include "T1Project.h"
#include "Util/GsTableUtil.h"
#include "GsItemStatGroup.h"
#include "GsItemStat.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "DataSchema/Item/GsSchemaItemEquipDetail.h"
#include "DataSchema/Item/GsSchemaItemEquipEnchantBonus.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemEquipConfig.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "GsItemManager.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Item/Data/GsItemStatData.h"

#include "Shared/Shared/SharedInclude/SharedFuncs.h"

FGsItemEquipment::~FGsItemEquipment()
{
	if (nullptr != _itemStatGroup)
	{
		delete _itemStatGroup;
		_itemStatGroup = nullptr;
	}
	
	_itemTableData = nullptr;
	_equipDetailData = nullptr;
	_partsSchemaData = nullptr;
}

FGsItemEquipment::FGsItemEquipment(const FGsSchemaItemCommon* InCommonData)
{
	const FGsSchemaItemEquipDetail* equipDetailData = GetDetailEquipTableData(InCommonData->detailId);
	if (nullptr == equipDetailData)
	{
		check(equipDetailData);
		GSLOG(Warning, TEXT("FGsSchemaItemEquipDetail Data is nullptr - Detail ID : %d"), InCommonData->detailId);
		return;
	}
	_equipDetailData = equipDetailData;

	// _partsSchemaData 가 nullptr 일 수도 있음.
	// 외형 변경이 일어나지 않는 parts 데이터이기 때문에
	const FGsSchemaPartsShapeData* data = UGsTableUtil::FindRowById<UGsTablePartsShapeData, FGsSchemaPartsShapeData>(
		equipDetailData->modelPath);
	_partsSchemaData = data;

	if (ItemCategoryMain::COLLECTABLE != InCommonData->categoryMain)
	{
		_itemStatGroup = new FGsItemStatGroup(_equipDetailData);
	}
}

void FGsItemEquipment::UpdateBy(const ItemDataEquip* In_ServerData)
{
	// level 수정되기 전, 체크하는 로직이 필요하여 Super() 전에 호출
	UpdateStat(In_ServerData->mLevel);

	Super::UpdateBy(In_ServerData);

	_enchantShieldCount = In_ServerData->mEnchantShield;
	_enchantBonusIndex = In_ServerData->mEnchantBonusIndex;
	_itemEquippedSlotIndex = In_ServerData->mItemEquipSlotIndex;

	// 제련 정보 데이터 가공
	if (nullptr != _itemStatGroup)
	{
		_itemStatGroup->UpdateRefineOptionStat(In_ServerData->mRefineOptionArray, In_ServerData->mRefineBonusInfo);
		_itemStatGroup->UpdateMagicalForge(In_ServerData->mMagicalForgeData);
	}
}

void FGsItemEquipment::UpdateBy(const FGsItem* In_Item)
{
	Super::UpdateBy(In_Item);
	FGsItem* itemData = const_cast<FGsItem*>(In_Item);
	FGsItemEquipment* equipData = itemData->GetCastItem<FGsItemEquipment>();
	check(equipData);

	_enchantShieldCount = equipData->GetEnchantShieldCount();
	_enchantBonusIndex = equipData->GetEnchantBonusIndex();
	_itemEquippedSlotIndex = equipData->GetSlotIndex();

	if (nullptr != _itemStatGroup)
	{
		_itemStatGroup->UpdateEquipItemStat(equipData->GetLevel());

		// 제련 정보 데이터 가공
		if (nullptr != equipData->GetRefineBonusInfo())
		{
			_itemStatGroup->UpdateRefineOptionStat(equipData->GetCurrentItemRefineStatValue(), *equipData->GetRefineBonusInfo());
			_itemStatGroup->UpdateMagicalForge(equipData->GetMagicalForgeData());
		}
	}
}

const FGsSchemaItemEquipDetail* FGsItemEquipment::GetDetailEquipTableData(int32 In_ID) const
{
	const UGsTableItemEquipDetail* table = Cast<UGsTableItemEquipDetail>(FGsSchemaItemEquipDetail::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaItemEquipDetail* row = nullptr;
	if (false == table->FindRowById(In_ID, row))
	{
		return nullptr;
	}


	return row;
}

int32 FGsItemEquipment::GetPartsSchemaID() const
{
	if (nullptr != _equipDetailData)
	{
		return _equipDetailData->modelPath;
	}

	return 0;
}

void FGsItemEquipment::UpdateStat(const uint16 InItemLevel)
{
	if (GetLevel() != InItemLevel && nullptr != _itemStatGroup)
	{
		_itemStatGroup->UpdateEquipItemStat(InItemLevel);
	}
}

void FGsItemEquipment::UpdateMagicalForge(MagicalForgeData InMagicalForgeData)
{
	if (_itemStatGroup)
	{
		_itemStatGroup->UpdateMagicalForge(InMagicalForgeData);
	}
}

const FString FGsItemEquipment::GetBonusStatTitleName() const
{
	if (nullptr == _itemStatGroup)
		return "";

	return _itemStatGroup->GetBonusStatTitleName(_enchantBonusIndex);
}

EGsItemEnchantBonusType FGsItemEquipment::GetBonusStatType() const
{
	if (nullptr == _itemStatGroup)
		return EGsItemEnchantBonusType::NONE;

	return _itemStatGroup->GetBonusStatType(_enchantBonusIndex);
}

const int32 FGsItemEquipment::GetOpenBonusStatLevelByStatType(const StatType InStatType) const
{
	if (nullptr == _itemStatGroup)
		return 0;

	return _itemStatGroup->GetOpenBonusStatLevelByStatType(_enchantBonusIndex, InStatType);
}

void FGsItemEquipment::FindAllBonusStatDataList(OUT TMap<int32, TArray<FGsItemStatData>>& outDataMap) const
{
	if (nullptr == _itemStatGroup)
		return;

	const TMap<int32, TArray<FGsItemStatData>> statDataMap = _itemStatGroup->GetBonusStatByIndex(_enchantBonusIndex);
	for (auto& e : statDataMap)
	{
		int32 openLv = e.Key;
		TArray<FGsItemStatData> statDataList = e.Value;
		FGsStatHelper::SortUiOrder(statDataList);
		outDataMap.Emplace(openLv, statDataList);
	}
}

TArray<int32> FGsItemEquipment::FindAllBonusStatLevel() const
{
	TArray<int32> levelList;
	if (nullptr != _itemStatGroup)
	{
		const TMap<int32, TArray<FGsItemStatData>> statDataMap = _itemStatGroup->GetBonusStatByIndex(_enchantBonusIndex);
		for (auto& e : statDataMap)
		{
			int32 openLv = e.Key;
			levelList.Emplace(openLv);
		}
	}
	return levelList;
}

TArray<FGsItemStatData> FGsItemEquipment::FindBonusStatDataByLevel(const int32 InOpenLevel) const
{
	TArray<FGsItemStatData> statDataList;
	if (nullptr != _itemStatGroup)
	{
		const TMap<int32, TArray<FGsItemStatData>> statDataMap = _itemStatGroup->GetBonusStatByIndex(_enchantBonusIndex);
		for (auto& e : statDataMap)
		{
			int32 openLv = e.Key;
			if (InOpenLevel == openLv)
			{
				TArray<FGsItemStatData> dataList = e.Value;
				for (uint8 i = 0; i < dataList.Num(); ++i)
				{
					statDataList.Emplace(dataList[i]);
				}

				FGsStatHelper::SortUiOrder(statDataList);
				break;
			}
		}
	}

	return statDataList;
}

// 현재 및 예상 스텟 리스트중에서 더 많은 스텟을 가진쪽이 있으면 길이를 맞춤
void FGsItemEquipment::CheckStatList(OUT TArray<FGsItemStatData>& InOriList, OUT TArray<FGsItemStatData>& InTargetList) const
{
	int32 oriNum = InOriList.Num();
	int32 expectNum = InTargetList.Num();
	if (oriNum == expectNum)
		return;

	if (oriNum > expectNum)
	{
		for (uint8 i = 0; i < InOriList.Num(); ++i)
		{
			bool found = false;
			for (uint8 j = 0; j < InTargetList.Num(); ++j)
			{
				if (InOriList[i].type == InTargetList[j].type)
					found = true;
			}

			if (false == found)
			{
				FGsItemStatData statData = InOriList[i];
				statData.value = 0;
				InTargetList.Emplace(statData);
			}
		}
		FGsStatHelper::SortUiOrder(InTargetList);
	}

	if (oriNum < expectNum)
	{
		for (uint8 i = 0; i < InTargetList.Num(); ++i)
		{
			bool found = false;
			for (uint8 j = 0; j < InOriList.Num(); ++j)
			{
				if (InTargetList[i].type == InOriList[j].type)
					found = true;
			}

			if (false == found)
			{
				FGsItemStatData statData = InTargetList[i];
				statData.value = 0;
				InOriList.Emplace(statData);
			}
		}
		FGsStatHelper::SortUiOrder(InOriList);
	}
}

TArray<FGsItemStat*> FGsItemEquipment::GetCurrentItemStatValue() const
{
	TArray<FGsItemStat*> originalStatDataList;
	if (nullptr != _itemStatGroup)
	{
		originalStatDataList = _itemStatGroup->GetItemOriginalStatList();
		//FGsStatHelper::SortUiOrder(originalStatDataList);
	}

	return originalStatDataList;
}

void FGsItemEquipment::GetCurrentAddedItemStatValue(OUT TArray<FGsItemStatData>& outDataList) const
{
	if (nullptr == _itemStatGroup)
		return;

	TArray<FGsItemStat*> originalStatDataList = _itemStatGroup->GetItemOriginalStatList();
	for (uint8 i = 0; i < originalStatDataList.Num(); ++i)
	{
		FGsItemStatData outStatData;
		outStatData.type = originalStatDataList[i]->GetStatType();
		outStatData.value = originalStatDataList[i]->GetStatOriValue() + originalStatDataList[i]->GetCalcStatValue();

		outDataList.Emplace(outStatData);
	}
	FGsStatHelper::SortUiOrder(outDataList);
}

void FGsItemEquipment::GetItemOriginalStatValue(OUT TArray<FGsItemStatData>& outDataList) const
{
	if (nullptr == _itemStatGroup)
		return;

	TArray<FGsItemStat*> statData = _itemStatGroup->GetItemOriginalStatList();
	for (uint8 i = 0; i < statData.Num(); ++i)
	{
		FGsItemStatData outStatData;
		outStatData.type = statData[i]->GetStatType();
		outStatData.value = statData[i]->GetStatOriValue();
		
		outDataList.Emplace(outStatData);
	}
	FGsStatHelper::SortUiOrder(outDataList);
}

void FGsItemEquipment::GetEnchantDisplayStatListByLevel(int32 InItemLevel, OUT TArray<FGsItemStatData>& outDataList)
{
	if (nullptr == _itemStatGroup)
		return;

	// 강화 안된 장비 일때
	if (0 >= InItemLevel)
	{
		TArray<FGsItemStat*> statDataList = _itemStatGroup->GetItemOriginalStatList();
		const TArray<FGsItemStatData> statList = _itemStatGroup->GetEnchantExpectStatList(InItemLevel + 1);
		for (FGsItemStat* iter : statDataList)
		{
			if(nullptr == iter)
				continue;

			for (FGsItemStatData iterData : statList)
			{
				if (iter->GetStatType() == iterData.type)
				{
					FGsItemStatData statData;
					statData.type = iterData.type;
					statData.value = iter->GetStatOriValue();
					outDataList.Emplace(statData);
				}
			}
		}
	}
	else
	{
		// expect(level) balance data + equipDetail stat List
		const TArray<FGsItemStatData> statList = _itemStatGroup->GetEnchantExpectStatList(0 >= InItemLevel ? InItemLevel + 1 : InItemLevel);
		for (uint8 i = 0; i < statList.Num(); ++i)
		{
			StatType statType = statList[i].type;
			int32 value = statList[i].value;

			FGsItemStatData statData;
			statData.type = statType;
			statData.value = 0 >= InItemLevel ? 0 : value;
			GetItemDefaultStatInfoByType(statData);
			outDataList.Emplace(statData);
		}
	}

	FGsStatHelper::SortUiOrder(outDataList);
}

void FGsItemEquipment::GetItemDefaultStatInfoByType(OUT FGsItemStatData& outStatData) const
{
	for (uint8 i = 0; i < _equipDetailData->statList.Num(); ++i)
	{
		if (outStatData.type == _equipDetailData->statList[i].type)
		{
			int32 value = outStatData.value + _equipDetailData->statList[i].value;
			outStatData.value = value;
		}
	}
}

void FGsItemEquipment::GetItemDefaultStat(OUT TArray<FGsItemStatData>& outDataList) const
{
	for (uint8 i = 0; i < _equipDetailData->statList.Num(); ++i)
	{
		outDataList.Emplace(_equipDetailData->statList[i]);
	}
}

ItemMaterial FGsItemEquipment::GetMaterialType() const
{
	return _equipDetailData->materialType;
}

uint8 FGsItemEquipment::GetSafeEnchantCount() const
{
	return _equipDetailData->safeEnchantLevel;
}

CreatureWeaponType FGsItemEquipment::GetCreatureWeaponType() const
{
	return _equipDetailData->weaponType;
}

const int32 FGsItemEquipment::GetEnchantCostValue()
{
	return _equipDetailData->enchantCost;
}

const int32 FGsItemEquipment::GetRefineOptionCostValue()
{
	return _equipDetailData->refineOptionCost;
}

const CurrencyType FGsItemEquipment::GetRefineOptionCostType()
{
	return _equipDetailData->refineOptionCostCurrencyType;
}

bool FGsItemEquipment::GetAllRefineOptionStatList(OUT TArray<FGsItemStat*>& outDataList)
{
	if (nullptr == _itemStatGroup)
		return false;

	if (0 >= _itemStatGroup->GetEquipItemRefineStatList().Num())
		return false;

	TArray<FGsItemStat*> statDataList = _itemStatGroup->GetEquipItemRefineStatList();
	for (uint8 i = 0; i < statDataList.Num(); ++i)
	{
		if(nullptr == statDataList[i])
			continue;

		if(StatType::NONE == statDataList[i]->GetStatType())
			continue;

		outDataList.Emplace(statDataList[i]);
	}

	return 0 < outDataList.Num();
}

const bool FGsItemEquipment::GetEquipItemRefineBonusStatInfo(OUT TPair<StatType, int32>& outPairData)
{
	if (nullptr == _itemStatGroup)
		return false;

	return _itemStatGroup->GetEquipItemRefineBonusStatData(outPairData);
}

// TPair<스텟 최소값,스텟 최대값>
void FGsItemEquipment::GetAllExpectRefineOptionStatList(const ItemCategorySub InRefineOptionStoneType, OUT TMap<StatType, TPair<int32, int32>>& outDataMap)
{
	if (nullptr == _itemStatGroup)
		return;

	if (ItemCategorySub::OPTION_STONE == InRefineOptionStoneType || ItemCategorySub::BLESSED_OPTION_STONE == InRefineOptionStoneType)
	{
		_itemStatGroup->GetAllExpectRefineOptionStatList(InRefineOptionStoneType, _equipDetailData->refineOptionId, outDataMap);
	}
}

// TPair<스텟 최소값,스텟 최대값>
void FGsItemEquipment::GetAllExpectRefineOptionStatList(const int32 InRefineOptionID, const ItemCategorySub InRefineOptionStoneType, OUT TMap<StatType, TPair<int32, int32>>& outDataMap)
{
	if (nullptr == _itemStatGroup)
		return;

	if (ItemCategorySub::OPTION_STONE == InRefineOptionStoneType || ItemCategorySub::BLESSED_OPTION_STONE == InRefineOptionStoneType)
	{
		_itemStatGroup->GetAllExpectRefineOptionStatList(InRefineOptionStoneType, TArray<int32> { InRefineOptionID } , outDataMap);
	}
}

// TPair<PassivityID,Value>
void FGsItemEquipment::GetAllExpectBonusRefineOptionStatList(OUT TMap<StatType, TPair<int32, int32>>& outDataMap)
{
	if (nullptr == _itemStatGroup)
		return;

	if (nullptr == _equipDetailData)
		return;

	_itemStatGroup->GetAllExpectBonusRefineOptionStatList(_equipDetailData->refineBonusOptionIds, outDataMap);
}

const uint8 FGsItemEquipment::GetAllSetCount()
{
	if (0 >= _equipDetailData->setId)
		return 0;

	return GItem()->GetMaxSetCountBySetId(_equipDetailData->setId);
}

const uint8 FGsItemEquipment::GetOpenSetCount()
{
	if (0 >= _equipDetailData->setId)
		return 0;

	return GItem()->GetOpenSetCountBySetId(_equipDetailData->setId);
}

bool FGsItemEquipment::GetSetItemStat(UGsItemManager::SetNumType InType, OUT const TArray<FGsItemStatData>*& outInfoList)
{
	if (0 >= _equipDetailData->setId)
		return false;

	return GItem()->GetSetItemStatInfo(_equipDetailData->setId, InType, outInfoList);
}

TArray<TPair<bool, FText>> FGsItemEquipment::GetItemNameTextAndOpenState()
{
	return GItem()->GetAllEquipStatusAndItemNames(_equipDetailData->setId);
}

bool FGsItemEquipment::IsSetItem() const
{
	return 0 < _equipDetailData->setId;
}

void FGsItemEquipment::GetRefineOptionIdList(OUT TArray<int32>& outOptionList)
{
	if (nullptr != _equipDetailData)
	{
		for (int32 i = 0; i < _equipDetailData->refineOptionId.Num(); ++i)
		{
			outOptionList.Emplace(_equipDetailData->refineOptionId[i]);
		}
	}
}

TArray<TPair<uint8, const FGsSchemaPassivitySet*>> FGsItemEquipment::GetAllSkilOptionData()
{
	TArray<TPair<uint8, const FGsSchemaPassivitySet*>> dataList;
	if (nullptr != _itemStatGroup)
	{
		dataList = _itemStatGroup->GetAllSkilOptionData();
	}
	return dataList;
}

TArray<const FGsSchemaPassivitySet*> FGsItemEquipment::GetSkilOptionDataByLevel()
{
	TArray<const FGsSchemaPassivitySet*> optionList;
	TArray<TPair<uint8, const FGsSchemaPassivitySet*>> allOptionList = GetAllSkilOptionData();
	if (0 < allOptionList.Num())
	{
		for (TPair<uint8, const FGsSchemaPassivitySet*> iter : allOptionList)
		{
			if (iter.Key == _itemLevel)
			{
				optionList.Emplace(iter.Value);
			}
		}
	}

	return optionList;
}

bool FGsItemEquipment::IsExistSkillOption() const
{
	if (nullptr == _itemStatGroup)
		return false;

	return 0 < _itemStatGroup->GetAllSkilOptionData().Num();
}

bool FGsItemEquipment::IsUseEnchantBonusStat() const
{
	if (nullptr == _equipDetailData)
		return false;

	return 0 < _equipDetailData->enchantBonusId;
}

bool FGsItemEquipment::IsExistEnchantLevel(int32 InLevel) const
{
	if (nullptr == _itemStatGroup)
		return false;

	return _itemStatGroup->IsExistEnchantLevel(InLevel);
}

const RefineBonusInfo* FGsItemEquipment::GetRefineBonusInfo()
{
	if (nullptr == _itemStatGroup)
		return nullptr;

	return _itemStatGroup->GetRefineBonusInfo();
}

TArray<FGsItemStat*> FGsItemEquipment::GetCurrentItemRefineStatValue() const
{
	TArray<FGsItemStat*> statList;
	if (nullptr != _itemStatGroup)
	{
		statList = _itemStatGroup->GetEquipItemRefineStatList();
	}
	return statList;
}

bool FGsItemEquipment::IsCooldownTimeItem() const
{
	return ItemCategoryMain::WEAPON == GetMainCategory();
}

bool FGsItemEquipment::IsUseItem() const
{
	return false;
}

float FGsItemEquipment::GetMaxCoolTime() const
{
	const UGsTable* tableData = FGsSchemaItemEquipConfig::GetStaticTable();
	if (nullptr == tableData)
	{
		return 0.f;
	}

	TArray<const FGsSchemaItemEquipConfig*> allrow;
	if (tableData->GetAllRows(allrow))
	{
		if (false == allrow.IsValidIndex(0))
			return 0.f;

		const FGsSchemaItemEquipConfig* data = allrow[0];
		return data->weaponChangeCooltime;
	}
	return 0.f;
}

int32 FGsItemEquipment::GetCooldownGroupID() const
{
	return 0;
}

int32 FGsItemEquipment::GetTooltipDurationTime() const
{
	return 0;
}

int32 FGsItemEquipment::GetPickOneTypeRewardID() const
{
	return 0;
}

int32 FGsItemEquipment::GetEquipGroupId() const
{
	return _equipDetailData->equipGroupId;
}

const int32 FGsItemEquipment::GetEnchantRateId() const
{
	if (nullptr != _equipDetailData)
	{
		return _equipDetailData->enchantRateId;
	}
	return 0;
}

int32 FGsItemEquipment::GetSetId() const
{
	if (_equipDetailData)
		return _equipDetailData->setId;

	return 0;
}

const uint64 FGsItemEquipment::GetEnchantShieldId() const
{
	if (nullptr != _equipDetailData)
	{
		return _equipDetailData->enchantShieldId;
	}
	return 0;
}

const uint8 FGsItemEquipment::GetPresetSlotIndex(PresetId InPresetId) const
{
	return SharedFunc::GetEquipSlotLocation(InPresetId, _itemEquippedSlotIndex);
}

const uint8 FGsItemEquipment::GetCurrentPresetSlotIndex() const
{
	if (FGsGameDataManager* gameData = GGameData())
	{
		if (const FGsNetUserData* userData = gameData->GetUserData())
		{
			return GetPresetSlotIndex(userData->mUserPresetId);
		}
	}
	
	return 0;
}

MagicalForgeData FGsItemEquipment::GetMagicalForgeData() const
{
	if (nullptr == _itemStatGroup)
		return MagicalForgeData(0);

	return _itemStatGroup->GetMagicalForgeData();
}