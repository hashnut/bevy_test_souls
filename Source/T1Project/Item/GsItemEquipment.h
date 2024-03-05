// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsItem.h"
#include "GsItemManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/GsSchemaEnums.h"
#include "T1Project/GameObject/Define/GsGameObjectDefine.h"

struct FGsSchemaItemEquipDetail;
struct FGsSchemaPartsShapeData;
struct FGsSchemaPassivitySet;
struct FGsSchemaItemCommon;
class FString;
class FGsItemStatGroup;
class FGsItemStat;
struct FGsItemStatData;

/**
 *  Equipment Item
 */
class T1PROJECT_API FGsItemEquipment final : public FGsItem
{
	using Super = FGsItem;

protected:
	uint8 _enchantBonusIndex = UNCONFIRMED_ENCHANT_BONUS_INDEX; // 강화특성 ID. 0부터 시작하는 인덱스 값이기 때문에 Max값을 INVALID로 사용
	uint8 _enchantShieldCount = UNCONFIRMED_ENCHANT_SHIELD; // 아이템 내구도 카운트

	// 아이템의 프리셋 별 장착 슬롯 인덱스 (비트 단위 사용)
	// DB 마이그레이션을 하지 않도록 의미를 변경
	// 받은 ItemEquipSlotIndex 는 프리셋 별 정보.
	// 클라에서 보내는 ItemEquipSlotIndex 는 그대로 단일 인덱스 정보
	// https://jira.com2us.com/jira/browse/C2URWQ-6573
	ItemEquipSlotIndex	_itemEquippedSlotIndex = INVALID_OBJECT_ID; 

private:
	const FGsSchemaItemEquipDetail* _equipDetailData = nullptr;
	const FGsSchemaPartsShapeData* _partsSchemaData = nullptr;

private:
	FGsItemStatGroup* _itemStatGroup = nullptr;

public:
	explicit FGsItemEquipment(const FGsSchemaItemCommon* InCommonData);
	FGsItemEquipment() = default;
	virtual ~FGsItemEquipment();

public:
	virtual void UpdateBy(const FGsItem * In_Item) override;
	virtual void UpdateBy(const ItemDataEquip* In_ServerData) override;

public:
	virtual bool IsCooldownTimeItem() const override;
	virtual bool IsUseItem() const override;
	virtual float GetMaxCoolTime() const override;
	virtual int32 GetCooldownGroupID() const override;
	virtual int32 GetTooltipDurationTime() const override;
	virtual int32 GetPickOneTypeRewardID() const override;

private:
	const FGsSchemaItemEquipDetail* GetDetailEquipTableData(int32 In_ID) const;

public:
	void UpdateStat(const uint16 InItemLevel);
	void UpdateMagicalForge(MagicalForgeData InMagicalForgeData);

public:
	// _partsSchemaData 가 nullptr 일 수도 있음 - 실제 눈에 보이는 아이템 파츠가 아니므로
	const FGsSchemaPartsShapeData* GetPartsSchemaData() const { return _partsSchemaData; }
	int32 GetPartsSchemaID() const;
	ItemMaterial GetMaterialType() const;
	CreatureWeaponType GetCreatureWeaponType() const;
	const int32 GetOpenBonusStatLevelByStatType(const StatType InStatType) const;
	const FString GetBonusStatTitleName() const;
	EGsItemEnchantBonusType GetBonusStatType() const;

public:
	bool IsSetItem() const;
	bool IsExistSkillOption() const;
	bool IsUseEnchantBonusStat() const;
	bool IsExistEnchantLevel(int32 InLevel) const;

public:
	const uint8 GetEnchantShieldCount() const { return _enchantShieldCount; }
	const uint8 GetEnchantBonusIndex() const { return _enchantBonusIndex; }
	uint8 GetSafeEnchantCount() const;
	const uint8 GetSlotIndex() const { return _itemEquippedSlotIndex; }
	const uint8 GetPresetSlotIndex(PresetId InPresetId) const;
	const uint8 GetCurrentPresetSlotIndex() const;
	const int32 GetEnchantRateId() const;
	const uint64 GetEnchantShieldId() const;

public:
	void SetSlotIndex(const ItemEquipSlotIndex In_SlotIndex) { _itemEquippedSlotIndex = In_SlotIndex; }
	void SetEnchantShieldCount(const uint8 Incount) { _enchantShieldCount = Incount; }
	void SetEnchantBonusIndex(const uint8 InIndex) { _enchantBonusIndex = InIndex; }

public:
	void FindAllBonusStatDataList(OUT TMap<int32, TArray<FGsItemStatData>>& outDataMap) const;
	TArray<int32> FindAllBonusStatLevel() const;
	TArray<FGsItemStatData> FindBonusStatDataByLevel(const int32 InOpenLevel) const;

public:
	const uint8 GetAllSetCount();
	const uint8 GetOpenSetCount();
	bool GetSetItemStat(UGsItemManager::SetNumType InType, OUT const TArray<FGsItemStatData>*& outInfoList);
	TArray<TPair<bool, FText>> GetItemNameTextAndOpenState();
	const int32 GetEnchantCostValue();
	const int32 GetRefineOptionCostValue();
	const CurrencyType GetRefineOptionCostType();
	void CheckStatList(OUT TArray<FGsItemStatData>& InOriList, OUT TArray<FGsItemStatData>& InTargetList) const;
	void GetItemOriginalStatValue(OUT TArray<FGsItemStatData>& outDataList) const;
	TArray<FGsItemStat*> GetCurrentItemStatValue() const;
	TArray<FGsItemStat*> GetCurrentItemRefineStatValue() const;
	void GetCurrentAddedItemStatValue(OUT TArray<FGsItemStatData>& outDataList) const;
	void GetItemDefaultStat(OUT TArray<FGsItemStatData>& outDataList) const;
	void GetItemDefaultStatInfoByType(OUT FGsItemStatData& outStatData) const;
	void GetEnchantDisplayStatListByLevel(int32 InItemLevel , OUT TArray<FGsItemStatData>& outDataList);
	bool GetAllRefineOptionStatList(OUT TArray<FGsItemStat*>& outDataList);
	const bool GetEquipItemRefineBonusStatInfo(OUT TPair<StatType, int32>& outPairData);
	void GetAllExpectRefineOptionStatList(const ItemCategorySub InRefineOptionStoneType, OUT TMap<StatType, TPair<int32, int32>>& outDataMap);
	void GetAllExpectRefineOptionStatList(const int32 InRefineOptionID, const ItemCategorySub InRefineOptionStoneType, OUT TMap<StatType, TPair<int32, int32>>& outDataMap);
	void GetAllExpectBonusRefineOptionStatList(OUT TMap<StatType, TPair<int32, int32>>& outDataMap);
	void GetRefineOptionIdList(OUT TArray<int32>& outOptionList);
	TArray<TPair<uint8, const FGsSchemaPassivitySet*>> GetAllSkilOptionData();
	TArray<const FGsSchemaPassivitySet*> GetSkilOptionDataByLevel();
	const RefineBonusInfo* GetRefineBonusInfo();
	int32 GetEquipGroupId() const;
	int32 GetSetId() const;
	MagicalForgeData GetMagicalForgeData() const;
};
