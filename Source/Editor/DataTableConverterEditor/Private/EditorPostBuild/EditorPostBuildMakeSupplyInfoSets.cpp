#if WITH_DEV_AUTOMATION_TESTS

#include "Runtime/AssetRegistry/Public/AssetRegistry/AssetRegistryModule.h"
#include "Runtime/Core/Public/Misc/AutomationTest.h"

#include "../Plugins/RSQL/Source/RSQLEditor/Public/GsTableManagerEditor.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "DataSchema/BMShop/GsSchemaBMShopTabInfo.h"
#include "DataSchema/ContentsEvent/GsSchemaContentsEventReward.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftRecipe.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "DataSchema/Item/GsSchemaItemEffectSkillBook.h"
#include "DataSchema/Map/Area/GsSchemaAreaData.h"
#include "DataSchema/Map/GsSchemaMapBalance.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/WorldMap/GsSchemaContinentalMapData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "DataSchema/Reward/GsSchemaRewardBox.h"
#include "DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Upgrade/GsSchemaSkillUpgradeSet.h"
#include "DataSchema/Skill/Upgrade/GsSchemaSkillUpgradeStepInfo.h"
#include "DataSchema/SupplyInfo/GsSchemaCraftSupplyInfo.h"
#include "DataSchema/SupplyInfo/GsSchemaCurrencySupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaDropSupplyInfo.h"
#include "DataSchema/SupplyInfo/GsSchemaItemSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSkillSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSupplyInfoConstraint.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Unlock/GsUnlockDefine.h"

/**
 * 최종적으로 필요한 타입은 Array 이지만, 데이터 조작 시 빠른 데이터 조회를 위해 내부적으로 Map 으로 데이터 관리를 수행함.
 */
template<typename TKey, typename TValue>
class FGsUniqueArrayEditor final
{
public:
	using SizeType = typename TArray<TValue>::SizeType;

private:
	TMap<TKey, SizeType> _map;
	TArray<TValue> _array;

public:
	void Reserve(const SizeType InCount)
	{
		_map.Reserve(InCount);
		_array.Reserve(InCount);
	}

	TValue* Find(const TKey& InKey)
	{
		const SizeType* Index = _map.Find(InKey);
		return (nullptr == Index) ? nullptr : &_array[*Index];
	}

	TValue& Add(const TKey& InKey)
	{
		check(nullptr == _map.Find(InKey)); // 동일 키 add 불가

		const SizeType Index = _array.AddDefaulted(1);
		_map.Emplace(InKey, Index);
		return _array[Index];
	}

	const TArray<TValue>& GetArray() const
	{
		return _array;
	}
};

struct FGsSupplyInfoHelperEditor final
{

public:
	bool _isValidIniatialized;

private:
	FAutomationTestBase& _automationTest;

	// 최종 데이터들을 저장할 컨테이너
	FGsUniqueArrayEditor<CurrencyType, FGsSchemaCurrencySupplyInfoSet> _currencySupplyInfoUniqueArray;
	FGsUniqueArrayEditor<ItemId, FGsSchemaItemSupplyInfoSet> _itemSupplyInfoUniqueArray;
	FGsUniqueArrayEditor<SkillId, FGsSchemaSkillSupplyInfoSet> _skillSupplyInfoUniqueArray;

	UGsTable* _currencySupplyInfoSetTable;
	UGsTable* _itemSupplyInfoSetTable;
	UGsTable* _skillSupplyInfoSetTable;

public:
	explicit FGsSupplyInfoHelperEditor(FAutomationTestBase& InAutomationTest)
		: _automationTest(InAutomationTest)
	{
		UGsTableManagerEditor& TableManager = UGsTableManagerEditor::GetInstance();

		_currencySupplyInfoSetTable = TableManager.GetTable(FGsSchemaCurrencySupplyInfoSet::StaticStruct());
		_itemSupplyInfoSetTable = TableManager.GetTable(FGsSchemaItemSupplyInfoSet::StaticStruct());
		_skillSupplyInfoSetTable = TableManager.GetTable(FGsSchemaSkillSupplyInfoSet::StaticStruct());

		_isValidIniatialized = _automationTest.TestNotNull(TEXT("_currencySupplyInfoSetTable"), _currencySupplyInfoSetTable) && _automationTest.TestNotNull(TEXT("_itemSupplyInfoSetTable"), _itemSupplyInfoSetTable) && _automationTest.TestNotNull(TEXT("_skillSupplyInfoSetTable"), _skillSupplyInfoSetTable);
	}

private:
	static bool IsPassConstraint(const FGsSchemaSupplyInfoConstraintRow& InSupplyInfoConstraintRow, const EGsUnlockContentsType InContentsType)
	{
		if (InSupplyInfoConstraintRow.IsNull())
		{
			// 별도 지정한 추가 정보 없을 경우, 무조건 수집
			return true;
		}

		const FGsSchemaSupplyInfoConstraint* SupplyInfoConstraint = InSupplyInfoConstraintRow.GetRow();
		if (nullptr == SupplyInfoConstraint)
		{
			// AddWarning(FString::Printf(TEXT("ItemCommon 테이블에 비정상 SupplyInfoConstraint: ItemCommon->id: %d, InSupplyInfoConstraintRow.RowName:%s"), ItemCommon->id, *InSupplyInfoConstraintRow.RowName.ToString()));
			return false;
		}

		if (EGsContentsShortcutMethod::Inclusive == SupplyInfoConstraint->showContentsTypesMethod)
		{
			// 포괄적 리스트인 경우에는, 모두 가능
			return true;
		}
		else if (EGsContentsShortcutMethod::Exclusive == SupplyInfoConstraint->showContentsTypesMethod)
		{
			// 배타적 리스트인 경우, 일치하는 경우에만 가능
			return SupplyInfoConstraint->showContentsTypes.Contains(InContentsType);
		}

		return false;
	}

private:
	/** 재화를 공급처 테이블에 입력해야 하는 지 여부를 반환. */
	static bool NeedToCollectSupplyInfoCurrencyType(const EGsUnlockContentsType InContentsType, const CurrencyType& InCurrencyType)
	{
		if (CurrencyType::NONE == InCurrencyType || CurrencyType::MAX == InCurrencyType)
		{
			return false;
		}

		const UGsTableCurrencyData* CurrencyDataTable = Cast<const UGsTableCurrencyData>(FGsSchemaCurrencyData::GetStaticTable());
		if (nullptr == CurrencyDataTable)
		{
			return false;
		}

		const FGsSchemaCurrencyData* CurrencyData;
		if (!CurrencyDataTable->FindRowById(InCurrencyType, CurrencyData))
		{
			// 존재하지 않는 재화 스킵
			// FString::Printf(TEXT("CurrencyDataTable->FindRowById(%d, Row)"), StaticCast<int32>(InCurrencyType))
			return false;
		}

		if (!FGsSupplyInfoHelperEditor::IsPassConstraint(CurrencyData->supplyInfoConstraint, InContentsType))
		{
			return false;
		}

		return true;
	}

	/** 아이템을 공급처 테이블에 입력해야 하는 지 여부를 반환. */
	static bool NeedToCollectSupplyInfoItemId(const EGsUnlockContentsType InContentsType, const ItemId& InItemId)
	{
		if (INVALID_ITEM_ID == InItemId)
		{
			return false;
		}

		const UGsTableItemCommon* ItemCommonTable = Cast<const UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
		if (nullptr == ItemCommonTable)
		{
			return false;
		}

		const FGsSchemaItemCommon* ItemCommon;
		if (!ItemCommonTable->FindRowById(InItemId, ItemCommon))
		{
			// 존재하지 않는 아이템 스킵
			return false;
		}

		if (!FGsSupplyInfoHelperEditor::IsPassConstraint(ItemCommon->supplyInfoConstraint, InContentsType))
		{
			return false;
		}

		return true;
	}

	/** 멀티플 아이템을 공급처 테이블에 입력해야 하는 지 여부를 반환. */
	static bool NeedToCollectSupplyInfoSkillId(const EGsUnlockContentsType InContentsType, const SkillId& InSkillId)
	{
		if (INVALID_SKILL_ID == InSkillId)
		{
			return false;
		}

		// MultipleItems 의 경우는 실제로는 아이템 정보를 바탕으로 체크함.

		return true;
	}

public:
	FGsSchemaItemSupplyInfoSet* GetSupplyInfoSetItemId(const ItemId& InItemId)
	{
		return (INVALID_ITEM_ID == InItemId) ? nullptr : _itemSupplyInfoUniqueArray.Find(InItemId);
	}

private:
	FGsSchemaCurrencySupplyInfoSet* GetValidSupplyInfoSetCurrencyType(const CurrencyType& InCurrencyType)
	{
		if (CurrencyType::NONE == InCurrencyType || CurrencyType::MAX == InCurrencyType)
		{
			return nullptr;
		}

		FGsSchemaCurrencySupplyInfoSet* SupplyInfoSet = _currencySupplyInfoUniqueArray.Find(InCurrencyType);
		if (nullptr == SupplyInfoSet)
		{
			SupplyInfoSet = &_currencySupplyInfoUniqueArray.Add(InCurrencyType);
			SupplyInfoSet->currencyType = InCurrencyType;
			SupplyInfoSet->RowName = FName(FString::Printf(TEXT("%d"), StaticCast<int32>(InCurrencyType)));
		}
		return SupplyInfoSet;
	}

	FGsSchemaItemSupplyInfoSet* GetValidSupplyInfoSetItemId(const ItemId& InItemId)
	{
		if (INVALID_ITEM_ID == InItemId)
		{
			return nullptr;
		}

		FGsSchemaItemSupplyInfoSet* SupplyInfoSet = _itemSupplyInfoUniqueArray.Find(InItemId);
		if (nullptr == SupplyInfoSet)
		{
			SupplyInfoSet = &_itemSupplyInfoUniqueArray.Add(InItemId);
			SupplyInfoSet->itemId = InItemId;
			SupplyInfoSet->RowName = FName(FString::Printf(TEXT("%d"), InItemId));
		}
		return SupplyInfoSet;
	}

private:
	FGsSchemaSkillSupplyInfoSet* GetValidSupplyInfoSetSkillId(const SkillId& InSkillId)
	{
		if (INVALID_SKILL_ID == InSkillId)
		{
			return nullptr;
		}

		FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = _skillSupplyInfoUniqueArray.Find(InSkillId);
		if (nullptr == SupplyInfoSet)
		{
			SupplyInfoSet = &_skillSupplyInfoUniqueArray.Add(InSkillId);
			SupplyInfoSet->skillId = InSkillId;
			SupplyInfoSet->RowName = FName(FString::Printf(TEXT("%d"), InSkillId));
		}
		return SupplyInfoSet;
	}

	void AddSupplyInfoSetContentsEventMapCreatureInternal(OUT TMap<int32, FGsSchemaDropSupplyInfo>& OutContentsEventMapCreatureSetMap, const TMap<ContentsEventId, TMap<MapId, TArray<FGsSchemaWorldMapMonsterElementDataRow>>>& InContentsEventMapWorldMapMonsterElementDataRows)
	{
		for (const TPair<ContentsEventId, TMap<MapId, TArray<FGsSchemaWorldMapMonsterElementDataRow>>>& ContentsEventMapWorldMapMonsterElementDataRows : InContentsEventMapWorldMapMonsterElementDataRows)
		{
			const ContentsEventId& CurrentContentsEventId = ContentsEventMapWorldMapMonsterElementDataRows.Key;
			for (const TPair<MapId, TArray<FGsSchemaWorldMapMonsterElementDataRow>>& MapWorldMapMonsterElementDataRowsPair : ContentsEventMapWorldMapMonsterElementDataRows.Value)
			{
				const MapId& CurrentMapId = MapWorldMapMonsterElementDataRowsPair.Key;
				const TArray<FGsSchemaWorldMapMonsterElementDataRow>& MonsterElementDataRows = MapWorldMapMonsterElementDataRowsPair.Value;
				OutContentsEventMapCreatureSetMap.FindOrAdd(CurrentContentsEventId).maps.FindOrAdd(CurrentMapId).worldmapMonsters.Append(MonsterElementDataRows);
			}
		}
	}

public:
	void AddSupplyInfoSetContentsTypeCurrencyType(const CurrencyType InKey, const EGsUnlockContentsType InContentsType)
	{
		if (FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoCurrencyType(InContentsType, InKey))
		{
			FGsSchemaCurrencySupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetCurrencyType(InKey);
			if (nullptr != SupplyInfoSet)
			{
				SupplyInfoSet->contentsTypeSet.Add(InContentsType);
			}
		}
	}
	void AddSupplyInfoSetContentsTypeItemId(const ItemId InKey, const EGsUnlockContentsType InContentsType)
	{
		if (FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoItemId(InContentsType, InKey))
		{
			FGsSchemaItemSupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetItemId(InKey);
			if (nullptr != SupplyInfoSet)
			{
				SupplyInfoSet->contentsTypeSet.Add(InContentsType);
			}
		}
	}
	void AddSupplyInfoSetContentsTypeSkillId(const SkillId InKey, const EGsUnlockContentsType InContentsType)
	{
		if (FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoSkillId(InContentsType, InKey))
		{
			FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetSkillId(InKey);
			if (nullptr != SupplyInfoSet)
			{
				SupplyInfoSet->contentsTypeSet.Add(InContentsType);
			}
		}
	}

	void AddSupplyInfoSetMapCreatureItemId(const ItemId InKey, const MapId InMapId, const CreatureId InCreatureId, const FGsSchemaWorldMapMonsterElementDataRow& InWorldMapMonsterElementDataRow)
	{
		if (FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoItemId(EGsUnlockContentsType::WORLDMAP, InKey))
		{
			FGsSchemaItemSupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetItemId(InKey);
			if (nullptr != SupplyInfoSet)
			{
				//SupplyInfoSet->mapCreatureSetMap.FindOrAdd(InMapId).creatureIds.Add(InCreatureId);
				SupplyInfoSet->mapCreatureSetMap.FindOrAdd(InMapId).worldmapMonsters.Add(InWorldMapMonsterElementDataRow);
			}
		}
	}

	void AddSupplyInfoSetMapCreatureSkillId(const SkillId InKey, const MapId InMapId, const CreatureId InCreatureId, const FGsSchemaWorldMapMonsterElementDataRow& InWorldMapMonsterElementDataRow)
	{
		if (FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoSkillId(EGsUnlockContentsType::WORLDMAP, InKey))
		{
			FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetSkillId(InKey);
			if (nullptr != SupplyInfoSet)
			{
				//SupplyInfoSet->mapCreatureSetMap.FindOrAdd(InMapId).creatureIds.Add(InCreatureId);
				SupplyInfoSet->mapCreatureSetMap.FindOrAdd(InMapId).worldmapMonsters.Add(InWorldMapMonsterElementDataRow);
			}
		}
	}

	void AddSupplyInfoSetContentsEventMapCreatureCurrencyType(const CurrencyType InKey, const TMap<ContentsEventId, TMap<MapId, TArray<FGsSchemaWorldMapMonsterElementDataRow>>>& InContentsEventMapWorldMapMonsterElementDataRows)
	{
		if (FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoCurrencyType(EGsUnlockContentsType::WORLDMAP, InKey))
		{
			FGsSchemaCurrencySupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetCurrencyType(InKey);
			if (nullptr != SupplyInfoSet)
			{
				AddSupplyInfoSetContentsEventMapCreatureInternal(SupplyInfoSet->contentsEventMapCreatureSetMap, InContentsEventMapWorldMapMonsterElementDataRows);
			}
		}
	}

	void AddSupplyInfoSetContentsEventMapCreatureItemId(const ItemId InKey, const TMap<ContentsEventId, TMap<MapId, TArray<FGsSchemaWorldMapMonsterElementDataRow>>>& InContentsEventMapWorldMapMonsterElementDataRows)
	{
		if (FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoItemId(EGsUnlockContentsType::WORLDMAP, InKey))
		{
			FGsSchemaItemSupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetItemId(InKey);
			if (nullptr != SupplyInfoSet)
			{
				AddSupplyInfoSetContentsEventMapCreatureInternal(SupplyInfoSet->contentsEventMapCreatureSetMap, InContentsEventMapWorldMapMonsterElementDataRows);
			}
		}
	}

	void AddSupplyInfoSetContentsEventMapCreatureSkillId(const SkillId InKey, const TMap<ContentsEventId, TMap<MapId, TArray<FGsSchemaWorldMapMonsterElementDataRow>>>& InContentsEventMapWorldMapMonsterElementDataRows)
	{
		if (FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoSkillId(EGsUnlockContentsType::WORLDMAP, InKey))
		{
			FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetSkillId(InKey);
			if (nullptr != SupplyInfoSet)
			{
				AddSupplyInfoSetContentsEventMapCreatureInternal(SupplyInfoSet->contentsEventMapCreatureSetMap, InContentsEventMapWorldMapMonsterElementDataRows);
			}
		}
	}

	void AddSupplyInfoSetCraftIdCurrencyType(const CurrencyType InKey, const CraftId InCraftId)
	{
		if (!FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoCurrencyType(EGsUnlockContentsType::CRAFT, InKey))
		{
			return;
		}

		FGsSchemaCurrencySupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetCurrencyType(InKey);
		if (nullptr == SupplyInfoSet)
		{
			return;
		}

		SupplyInfoSet->craftIdSet.Add(InCraftId);
	}

	void AddSupplyInfoSetCraftIdItemId(const ItemId InKey, const CraftId InCraftId)
	{
		if (!FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoItemId(EGsUnlockContentsType::CRAFT, InKey))
		{
			return;
		}

		FGsSchemaItemSupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetItemId(InKey);
		if (nullptr == SupplyInfoSet)
		{
			return;
		}

		SupplyInfoSet->craftIdSet.Add(InCraftId);
	}

	void AddSupplyInfoSetCraftIdSkillId(const SkillId InKey, const CraftId InCraftId)
	{
		if (!FGsSupplyInfoHelperEditor::NeedToCollectSupplyInfoSkillId(EGsUnlockContentsType::CRAFT, InKey))
		{
			return;
		}

		FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetSkillId(InKey);
		if (nullptr == SupplyInfoSet)
		{
			return;
		}

		SupplyInfoSet->craftIdSet.Add(InCraftId);
	}

	static bool SortBySkillItemId(const int32& InA, const int32& InB)
	{
		const UGsTableItemCommon* ItemCommonTable = Cast<const UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
		if (nullptr != ItemCommonTable)
		{
			const FGsSchemaItemCommon* A = nullptr;
			const FGsSchemaItemCommon* B = nullptr;
			if (ItemCommonTable->FindRowById(InA, A) && ItemCommonTable->FindRowById(InB, B) && nullptr != A && nullptr != B)
			{
				if (A->tradable != B->tradable)
				{
					// 거래 가능 아이템을 맨 앞으로
					return B->tradable;
				}
			}
		}
		return false;
	}

	void AddSupplyInfoSetItemIdsSkillId(const SkillId InKey, const TSet<ItemId>& InItemIdSet)
	{
		FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = GetValidSupplyInfoSetSkillId(InKey);
		if (nullptr == SupplyInfoSet)
		{
			return;
		}

		for (const ItemId CurrentItemId : InItemIdSet)
		{
			const int32 CurrentItemIdInt32 = StaticCast<int32>(CurrentItemId);
			if (!SupplyInfoSet->itemIds.Contains(CurrentItemIdInt32))
			{
				SupplyInfoSet->itemIds.Add(CurrentItemIdInt32);
			}
		}

		SupplyInfoSet->itemIds.Sort();
	}

/*
private:
	template<typename TSupplyInfoSet, typename TKey>
	static void AddSupplyInfoSet(OUT FGsUniqueArrayEditor<TKey, TSupplyInfoSet>& OutSupplyInfoSet, const TKey InKey, const TMap<ContentsEventId, TMap<MapId, TSet<int32>>>& InContentsEventDropSupplyInfo)
	{
	}

	template<typename TSupplyInfoSet, typename TKey>
	static void AddSupplyInfoSet(OUT FGsUniqueArrayEditor<TKey, TSupplyInfoSet>& OutSupplyInfoSet, const TKey InKey, const MapId InMapId, const CreatureId InCreatureId)
	{
		
	}

	template<typename TSupplyInfoSet, typename TKey>
	static void AddSupplyInfoSet(OUT FGsUniqueArrayEditor<TKey, TSupplyInfoSet>& OutSupplyInfoSet, const TKey InKey, const CraftId& InCraftId)
	{
		FGsSupplyInfoHelperEditor::GetValidSupplyInfoSet(OutSupplyInfoSet, InKey).craftSupplyInfo.craftIdSet.Add(InCraftId);
	}

	template<typename TSupplyInfoSet, typename TKey>
	static void AddSupplyInfoSet(OUT FGsUniqueArrayEditor<TKey, TSupplyInfoSet>& OutSupplyInfoSet, const TKey InKey, const int32 InBmshopTabId)
	{
		FGsSupplyInfoHelperEditor::GetValidSupplyInfoSet(OutSupplyInfoSet, InKey).bmshopSupplyInfo.bmshopTabIds.Add(InBmshopTabId);
	}*/

public:
	bool SaveToDataTables()
	{
		if (!_automationTest.TestTrue(TEXT("_isValidIniatialized"), _isValidIniatialized))
		{
			return false;
		}

		_currencySupplyInfoSetTable->Empty();
		for (const FGsSchemaCurrencySupplyInfoSet& iter : _currencySupplyInfoUniqueArray.GetArray())
		{
			_currencySupplyInfoSetTable->AddRow(iter);
		}

		if (!_automationTest.TestTrue(TEXT("_currencySupplyInfoSetTable->SaveDataTable()"), _currencySupplyInfoSetTable->SaveDataTable()))
		{
			return false;
		}

		_itemSupplyInfoSetTable->Empty();
		for (const FGsSchemaItemSupplyInfoSet& iter : _itemSupplyInfoUniqueArray.GetArray())
		{
			_itemSupplyInfoSetTable->AddRow(iter);
		}

		if (!_automationTest.TestTrue(TEXT("_itemSupplyInfoSetTable->SaveDataTable()"), _itemSupplyInfoSetTable->SaveDataTable()))
		{
			return false;
		}

		_skillSupplyInfoSetTable->Empty();
		for (const FGsSchemaSkillSupplyInfoSet& iter : _skillSupplyInfoUniqueArray.GetArray())
		{
			_skillSupplyInfoSetTable->AddRow(iter);
		}

		if (!_automationTest.TestTrue(TEXT("_multipleItemsSupplyInfoSetTable->SaveDataTable()"), _skillSupplyInfoSetTable->SaveDataTable()))
		{
			return false;
		}

		return true;
	}

};

#define GS_UTEST_TRUE(What) UTEST_TRUE(TEXT(#What), What)
#define GS_UTEST_NOT_NULL(What) UTEST_NOT_NULL(TEXT(#What), What)

/**
 * 공급처 테이블 생성
 * WorldMapMonsterData, Craft 테이블 등을 조회하여, 공급처 테이블을 생성함.
 */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EditorPostBuildMakeSupplyInfoSets, TEXT("Raon.EditorPostBuild.5 공급처 테이블 생성")
	, EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriorityAndAbove | EAutomationTestFlags::EngineFilter)
bool EditorPostBuildMakeSupplyInfoSets::RunTest(const FString& Parameters)
{
	AddInfo(TEXT("공급처 테이블 생성 시작"));

	FGsSupplyInfoHelperEditor SupplyInfoHelperEditor(*this);
	GS_UTEST_TRUE(SupplyInfoHelperEditor._isValidIniatialized);
	
	// 전역으로 필요한 테이블 로드
	const UGsTableWorldMapMonsterData* WorldMapMonsterDataTable = Cast<UGsTableWorldMapMonsterData>(FGsSchemaWorldMapMonsterData::GetStaticTable());
	const UGsTableItemCommon* ItemCommonTable = Cast<const UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	GS_UTEST_NOT_NULL(WorldMapMonsterDataTable)
	GS_UTEST_NOT_NULL(ItemCommonTable)

	// 맵수집대상
	const TSet<MapType> TargetMapTypes = { MapType::PUBLIC_FIELD, MapType::PUBLIC_DUNGEON, MapType::WORLD_BOSS, MapType::ELITE_DUNGEON_1, MapType::INTER_FIELD };

	// 유효한 맵 데이터 수집
	TMap<MapId, const FGsSchemaMapData*> ValidMaps;

	// 컨텐츠이벤트 처리에 사용을 위해 맵데이터 테이블에서 모든 MapId 를 수집.
	TMap<MapId, TArray<FGsSchemaWorldMapMonsterElementDataRow>> AllContentsEventMonsterElementsInMaps;

	{
		// 컨티넨탈맵 테이블에 없는 맵 스킵하기 위해 사용
		const UGsTableContinentalMapData* ContinentalMapDataTable = Cast<UGsTableContinentalMapData>(FGsSchemaContinentalMapData::GetStaticTable());
		GS_UTEST_NOT_NULL(ContinentalMapDataTable)

		const UGsTableMapData* MapDataTable = Cast<UGsTableMapData>(FGsSchemaMapData::GetStaticTable());
		GS_UTEST_NOT_NULL(MapDataTable)

		TArray<const FGsSchemaMapData*> MapDatas;
		GS_UTEST_TRUE(MapDataTable->GetAllRows(MapDatas))

		const FGsSchemaContinentalMapData* DummyContinentalMapData = nullptr;

		for (const FGsSchemaMapData* MapData : MapDatas)
		{
			GS_UTEST_NOT_NULL(MapData)

			const MapId CurrentMapId = MapData->id;
			if (INVALID_MAP_ID == CurrentMapId || MapData->isTestLevel)
			{
				AddInfo(FString::Printf(TEXT("INVALID_MAP_ID == %d || MapData->isTestLevel"), CurrentMapId));
				continue;
			}

			const FGsSchemaMapBalance* MapBalance = MapData->balanceId.GetRow();
			if (nullptr != MapBalance)
			{
				if (!MapBalance->canItemDrop)
				{
					AddInfo(FString::Printf(TEXT("!MapBalance->canItemDrop 인 맵 제외. mapId: %d, mapType: %s"), CurrentMapId, *MapTypeEnumToString(MapData->mapType)));
					continue;
				}
			}

			if (!TargetMapTypes.Contains(MapData->mapType))
			{
				AddInfo(FString::Printf(TEXT("허용된 맵 타입이 아닌 맵 스킵. mapId: %d, mapType: %s"), CurrentMapId, *MapTypeEnumToString(MapData->mapType)));
				continue;
			}

			const FGsSchemaAreaData* AreaData = MapData->areaId.GetRow();
			if (nullptr == AreaData)
			{
				AddInfo(FString::Printf(TEXT("nullptr == AreaData 맵 스킵. mapId: %d, mapType: %s"), CurrentMapId, *MapTypeEnumToString(MapData->mapType)));
				continue;
			}

			const ContinentId CurrentContinentId = AreaData->continentId;
			if (INVALID_CONTINENT_ID == CurrentContinentId)
			{
				AddInfo(FString::Printf(TEXT("INVALID_CONTINENT_ID == CurrentContinentId 맵 스킵. mapId: %d, mapType: %s"), CurrentMapId, *MapTypeEnumToString(MapData->mapType)));
				continue;
			}

			if (!ContinentalMapDataTable->FindRowById(AreaData->continentId, DummyContinentalMapData) || nullptr == DummyContinentalMapData)
			{
				AddInfo(FString::Printf(TEXT("ContinentalMapData 테이블에 존재하지 않는 ContinentId 스킵. mapId: %d, continentalId: %d"), CurrentMapId, AreaData->continentId));
				continue;
			}

			// 유효한 맵 목록에 저장. 여기 할당된 맵 외에는 아래 로직에서 모두 제외될 것.
			ValidMaps.Add(CurrentMapId, MapData);
			AddInfo(FString::Printf(TEXT("획득수집가능맵으로 추가. mapId: %d, continentalId: %d, mapType: %s"), CurrentMapId, AreaData->continentId, *MapTypeEnumToString(MapData->mapType)));

			if (MapType::WORLD_BOSS == MapData->mapType)
			{
				AddInfo(TEXT("MapType::WORLD_BOSS == MapData->mapType"));
			}

			// 컨텐츠이벤트에서 사용을 위해 모든 맵의 모든 월드맵몬스터를 수집
			const FGsSchemaWorldMapMonsterData* WorldMapMonsterData = nullptr;
			if (!WorldMapMonsterDataTable->FindRowById(CurrentMapId, WorldMapMonsterData))
			{
				AddInfo(FString::Printf(TEXT("!WorldMapMonsterDataTable->FindRowById(CurrentMapId(%d), WorldMapMonsterData)"), CurrentMapId));
				continue;
			}

			for (const FGsSchemaWorldMapMonsterElementDataRow& WorldMapMonsterElementDataRow : WorldMapMonsterData->monsters)
			{
				const FGsSchemaWorldMapMonsterElementData* WorldMapMonsterElementData = WorldMapMonsterElementDataRow.GetRow();
				if (nullptr == WorldMapMonsterElementData)
				{
					AddInfo(TEXT("nullptr == WorldMapMonsterElementData"));
					continue;
				}

				const FGsSchemaNpcData* NpcData = WorldMapMonsterElementData->id.GetRow();
				if (nullptr == NpcData || INVALID_CREATURE_ID == NpcData->id)
				{
					AddInfo(FString::Printf(TEXT("nullptr == NpcData || INVALID_CREATURE_ID == NpcData->id, mapId:%d"), CurrentMapId));
					continue;
				}

				if (NpcFunctionType::SANDBAG == NpcData->npcFunctionType)
				{
					AddInfo(FString::Printf(TEXT("기획1팀 요청에 의해 SANDBAG 타입은 공급처에서 제외. mapId: %d, continentalId: %d, NpcData->id: %d"), CurrentMapId, AreaData->continentId, NpcData->id));
					continue;
				}

				const FGsSchemaNpcBalance* NpcBalance = NpcData->balanceId.GetRow();
				if (nullptr == NpcBalance)
				{
					continue;
				}

				AddInfo(FString::Printf(TEXT("획득수집가능NPC로 추가. mapId: %d, mapType: %s, NpcData->id: %d, NpcBalance->id: %d"), CurrentMapId, *MapTypeEnumToString(MapData->mapType), NpcData->id, NpcBalance->id));
				AllContentsEventMonsterElementsInMaps.FindOrAdd(CurrentMapId).Add(WorldMapMonsterElementDataRow);
			}
		}
	}

	{
		AddInfo(TEXT("월드맵 몬스터 데이터 테이블 수집"));

		TArray<const FGsSchemaWorldMapMonsterData*> WorldMapMonsterDatas;
		GS_UTEST_TRUE(WorldMapMonsterDataTable->GetAllRows(WorldMapMonsterDatas))

		// 월드맵몬스터 순환하면서 공급처 수집
		for (const FGsSchemaWorldMapMonsterData* WorldMapMonsterData : WorldMapMonsterDatas)
		{
			if (nullptr == WorldMapMonsterData)
			{
				AddWarning(TEXT("nullptr == WorldMapMonsterData"));
				continue;
			}

			const int32 CurrentMapId = WorldMapMonsterData->mapId;
			if (INVALID_MAP_ID == CurrentMapId)
			{
				continue;
			}

			if (!ValidMaps.Contains(CurrentMapId))
			{
				// 유효한 맵 목록에 없으면 스킵.
				continue;
			}

			// 몬스터 순환
			for (const FGsSchemaWorldMapMonsterElementDataRow& MonsterElementDataRow : WorldMapMonsterData->monsters)
			{
				const FGsSchemaWorldMapMonsterElementData* MonsterElementData = MonsterElementDataRow.GetRow();
				if (nullptr == MonsterElementData)
				{
					AddInfo(TEXT("nullptr == MonsterElementData"));
					continue;
				}

				const FGsSchemaNpcData* NpcData = MonsterElementData->id.GetRow();
				if (nullptr == NpcData || INVALID_CREATURE_ID == NpcData->id)
				{
					AddInfo(FString::Printf(TEXT("nullptr == NpcData || INVALID_CREATURE_ID == NpcData->id, mapId:%d"), CurrentMapId));
					continue;
				}

				if (NpcFunctionType::SANDBAG == NpcData->npcFunctionType)
				{
					// 기획1팀 요청에 의해 SANDBAG 타입은 공급처에서 제외
					continue;
				}

				const CreatureId CurrentCretureId = NpcData->id;
				if (INVALID_CREATURE_ID == CurrentCretureId)
				{
					AddInfo(FString::Printf(TEXT("INVALID_CREATURE_ID == CurrentCretureId, mapId:%d"), CurrentMapId));
					continue;
				}

				// 장비아이템 순환
				for (const FGsSchemaItemCommonRow& ItemCommonRow : MonsterElementData->equipItemList)
				{
					const FGsSchemaItemCommon* ItemCommon = ItemCommonRow.GetRow();
					if (nullptr == ItemCommon || INVALID_ITEM_ID == (ItemId)ItemCommon->id)
					{
						continue;
					}

					SupplyInfoHelperEditor.AddSupplyInfoSetMapCreatureItemId(StaticCast<ItemId>(ItemCommon->id), CurrentMapId, CurrentCretureId, MonsterElementDataRow);
				}

				// 기타 아이템 순환
				for (const FGsSchemaItemCommonRow& ItemCommonRow : MonsterElementData->etcItemList)
				{
					const FGsSchemaItemCommon* ItemCommon = ItemCommonRow.GetRow();
					if (nullptr == ItemCommon || INVALID_ITEM_ID == (ItemId)ItemCommon->id)
					{
						continue;
					}

					SupplyInfoHelperEditor.AddSupplyInfoSetMapCreatureItemId(StaticCast<ItemId>(ItemCommon->id), CurrentMapId, CurrentCretureId, MonsterElementDataRow);
				}
			}
		}
	}

	{
		AddInfo(TEXT("컨텐츠이벤트 리워드 획득처 수집"));

		const UGsTableContentsEventReward* ContentsEventRewardTable = Cast<UGsTableContentsEventReward>(FGsSchemaContentsEventReward::GetStaticTable());
		TArray<const FGsSchemaContentsEventReward*> ContentsEventRewards;
		GS_UTEST_TRUE(ContentsEventRewardTable->GetAllRows(ContentsEventRewards))

		// 모든 컨텐츠이벤트리워드 테이블 순환하여 공급처 수집
		for (const FGsSchemaContentsEventReward* ContentsEventReward : ContentsEventRewards)
		{
			if (nullptr == ContentsEventReward)
			{
				AddWarning(TEXT("nullptr == ContentsEventReward"));
				continue;
			}

			const ContentsEventId CurrentContentsEventId = ContentsEventReward->id;

			TSet<MapId> TargetMapIds;
			if (0 == ContentsEventReward->mapDatas.Num())
			{
				// mapDatas 비어있다면 모든 맵이 대상맵
				AllContentsEventMonsterElementsInMaps.GetKeys(TargetMapIds);
			}
			else
			{
				for (const FGsSchemaMapDataRow& MapDataRow : ContentsEventReward->mapDatas)
				{
					const FGsSchemaMapData* MapData = MapDataRow.GetRow();
					if (nullptr == MapData)
					{
						AddWarning(FString::Printf(TEXT("nullptr == MapData, CurrentContentsEventId:%d"), CurrentContentsEventId));
						continue;
					}
					TargetMapIds.Add(MapData->id);
				}
			}

			// 대상 맵이 없다면 스킵.
			if (0 == TargetMapIds.Num())
			{
				AddInfo(FString::Printf(TEXT("0 == TargetMapIds.Num(), CurrentContentsEventId:%d"), CurrentContentsEventId));
				continue;
			}

			// 제외할 NpcBalanceId 목록 생성
			TSet<NpcBalanceId> ExceptNpcBalanceIds;
			for (const FGsSchemaNpcBalanceRow& NpcBalanceRow : ContentsEventReward->exceptNpcBalances)
			{
				const FGsSchemaNpcBalance* NpcBalance = NpcBalanceRow.GetRow();
				if (nullptr == NpcBalance)
				{
					AddWarning(FString::Printf(TEXT("nullptr == NpcBalance, CurrentContentsEventId:%d"), CurrentContentsEventId));
					continue;
				}

				if (INVALID_NPC_BALANCE_ID == NpcBalance->id)
				{
					AddWarning(FString::Printf(TEXT("INVALID_NPC_BALANCE_ID == NpcBalance->id, CurrentContentsEventId:%d"), CurrentContentsEventId));
					continue;
				}

				ExceptNpcBalanceIds.Add(NpcBalance->id);
			}

			// 컨텐츠이벤트 리워드로부터 공급처 데이터 생성
			TMap<ContentsEventId, TMap<MapId, TArray<FGsSchemaWorldMapMonsterElementDataRow>>> ContentsEventMapWorldMapMonsterElementDataRows;
			TMap<ContentsEventId, TMap<MapId, TSet<int32>>> ContentsEventMapCreature;
			for (const MapId& CurrentMapId : TargetMapIds)
			{
				const TArray<FGsSchemaWorldMapMonsterElementDataRow>* WorldMapMonsterElementDataRows = AllContentsEventMonsterElementsInMaps.Find(CurrentMapId);
				if (nullptr == WorldMapMonsterElementDataRows)
				{
					continue;
				}

				for (const FGsSchemaWorldMapMonsterElementDataRow& WorldMapMonsterElementDataRow : *WorldMapMonsterElementDataRows)
				{
					const FGsSchemaWorldMapMonsterElementData* WorldMapMonsterElementData = WorldMapMonsterElementDataRow.GetRow();
					if (nullptr == WorldMapMonsterElementData)
					{
						continue;
					}

					const FGsSchemaNpcData* NpcData = WorldMapMonsterElementData->id.GetRow();
					if (nullptr == NpcData)
					{
						continue;
					}

					const FGsSchemaNpcBalance* NpcBalance = NpcData->balanceId.GetRow();
					if (nullptr == NpcBalance)
					{
						continue;
					}

					if (ExceptNpcBalanceIds.Contains(NpcBalance->id))
					{
						// 제외 NpcBalanceId
						continue;
					}

					ContentsEventMapWorldMapMonsterElementDataRows.FindOrAdd(CurrentContentsEventId).FindOrAdd(CurrentMapId).Emplace(WorldMapMonsterElementDataRow);
				}
			}

			// 리워드 돌면서 획득처에 입력
			for (const FGsSchemaRewardDataRow& RewardDataRow : ContentsEventReward->rewardDatas)
			{
				const FGsSchemaRewardData* RewardData = RewardDataRow.GetRow();
				if (nullptr == RewardData)
				{
					continue;
				}

				SupplyInfoHelperEditor.AddSupplyInfoSetContentsTypeCurrencyType(RewardData->currencyType, EGsUnlockContentsType::WORLDMAP);

				for (const FGsSchemaRewardBox& RewardBox : RewardData->rewardBoxList)
				{
					const FGsSchemaRewardBoxData* RewardBoxData = RewardBox.rewardBoxDataRow.GetRow();
					if (nullptr == RewardBoxData)
					{
						continue;
					}

					const TArray<FGsSchemaRewardItemBagData>& RewardItemBagDatas = RewardBoxData->itemBagList;
					const int32 Count = RewardItemBagDatas.Num();
					for (int32 i = 0; i < Count; i++)
					{
						const FGsSchemaRewardItemBagData& RewardItemBagData = RewardItemBagDatas[i];
						if (0 < RewardItemBagData.currencyAmountMin)
						{
							SupplyInfoHelperEditor.AddSupplyInfoSetContentsEventMapCreatureCurrencyType(RewardItemBagData.currencyType, ContentsEventMapWorldMapMonsterElementDataRows);
						}

						if (0 < RewardItemBagData.itemCountMin)
						{
							SupplyInfoHelperEditor.AddSupplyInfoSetContentsEventMapCreatureItemId(StaticCast<ItemId>(RewardItemBagData.itemId), ContentsEventMapWorldMapMonsterElementDataRows);
						}
					}
				}
			}
		}
	}

	{
		AddInfo(TEXT("제작 획득처 수집"));

		const UGsTableRewardData* RewardDataTable = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
		GS_UTEST_NOT_NULL(RewardDataTable)

		const UGsTable* CraftTargetTable = FGsSchemaItemCraftRecipe::GetStaticTable();
		GS_UTEST_NOT_NULL(CraftTargetTable)

		TArray<const FGsSchemaItemCraftRecipe*> ItemCraftRecipes;
		GS_UTEST_TRUE(CraftTargetTable->GetAllRows(ItemCraftRecipes))

		for (const FGsSchemaItemCraftRecipe* CraftTarget : ItemCraftRecipes)
		{
			if (nullptr == CraftTarget)
			{
				AddWarning(TEXT("ItemCraftRecipe 테이블 itemCraftRecipeId 비정상. nullptr != CraftTarget"));
				continue;
			}

			const CraftId CurrentCraftId = CraftTarget->itemCraftRecipeId;
			const RewardId CurrentRewardId = CraftTarget->rewardDataId;
			if (INVALID_CRAFT_ID == CurrentCraftId)
			{
				AddWarning(FString::Printf(TEXT("ItemCraftRecipe 테이블 itemCraftRecipeId 비정상. RowName:%s"), *CraftTarget->RowName.ToString()));
				continue;
			}

			const FGsSchemaRewardData* RewardData = nullptr;
			if (!RewardDataTable->FindRowById(CurrentRewardId, RewardData))
			{
				AddWarning(FString::Printf(TEXT("CraftTarget 테이블에 리워드 없음: RowName: %s, CurrentRewardId:%d"), *CraftTarget->RowName.ToString(), CurrentRewardId));
				continue;
			}

			if (nullptr == RewardData)
			{
				AddWarning(FString::Printf(TEXT("CraftTarget 테이블에 비정상 리워드: RowName: %s"), *CraftTarget->RowName.ToString()));
				continue;
			}

			SupplyInfoHelperEditor.AddSupplyInfoSetCraftIdCurrencyType(RewardData->currencyType, CurrentCraftId);

			for (const FGsSchemaRewardBox& RewardBox : RewardData->rewardBoxList)
			{
				const FGsSchemaRewardBoxData* RewardBoxData = RewardBox.rewardBoxDataRow.GetRow();
				if (nullptr == RewardBoxData)
				{
					continue;
				}

				const TArray<FGsSchemaRewardItemBagData>& RewardItemBagDatas = RewardBoxData->itemBagList;
				const int32 Count = RewardItemBagDatas.Num();
				for (int32 i = 0; i < Count; i++)
				{
					if (CRAFT_REWARD_FAIL_INDEX == i)
					{
						// 제작 실패 시 획득 요소는 공급처에서 제외함 https://jira.com2us.com/jira/browse/C2URWQ-1355
						continue;
					}

					const FGsSchemaRewardItemBagData& RewardItemBagData = RewardItemBagDatas[i];
					if (0 < RewardItemBagData.currencyAmountMin)
					{
						SupplyInfoHelperEditor.AddSupplyInfoSetCraftIdCurrencyType(RewardItemBagData.currencyType, CurrentCraftId);
					}

					if (0 < RewardItemBagData.itemCountMin)
					{
						SupplyInfoHelperEditor.AddSupplyInfoSetCraftIdItemId(StaticCast<ItemId>(RewardItemBagData.itemId), CurrentCraftId);
					}
				}
			}
		}
	}

	{
		AddInfo(TEXT("상점 획득처 수집"));

		const UGsTable* BMShopTabInfoTable = FGsSchemaBMShopTabInfo::GetStaticTable();
		if (nullptr != BMShopTabInfoTable)
		{
			// 유효한 상점 탭ID
			TSet<int32> ValidBMShopTabIds;
			
			{
				TArray<const FGsSchemaBMShopTabInfo*> BMShopTabInfos;
				GS_UTEST_TRUE(BMShopTabInfoTable->GetAllRows(BMShopTabInfos))

				for (const FGsSchemaBMShopTabInfo* BMShopTabInfo : BMShopTabInfos)
				{
					if (nullptr == BMShopTabInfo)
					{
						continue;
					}

					ValidBMShopTabIds.Add(BMShopTabInfo->id);
				}
			}

			// 아이템 테이블 순환 조회
			{
				TArray<const FGsSchemaItemCommon*> ItemCommons;
				GS_UTEST_TRUE(ItemCommonTable->GetAllRows(ItemCommons))

				for (const FGsSchemaItemCommon* ItemCommon : ItemCommons)
				{
					if (nullptr == ItemCommon)
					{
						AddWarning(TEXT("nullptr == ItemCommon"));
						continue;
					}

					const FGsSchemaSupplyInfoConstraintRow& SupplyInfoConstraintRow = ItemCommon->supplyInfoConstraint;
					if (SupplyInfoConstraintRow.IsNull())
					{
						continue;
					}

					const FGsSchemaSupplyInfoConstraint* SupplyInfoConstraint = SupplyInfoConstraintRow.GetRow();
					if (nullptr == SupplyInfoConstraint)
					{
						continue;
					}

					if (!ValidBMShopTabIds.Contains(SupplyInfoConstraint->bmshopTabId))
					{
						continue;
					}

					SupplyInfoHelperEditor.AddSupplyInfoSetContentsTypeItemId(StaticCast<ItemId>(ItemCommon->id), EGsUnlockContentsType::BMSHOP);
				}
			}

			// 재화 테이블 순환 조회
			{
				const UGsTableCurrencyData* CurrencyDataTable = Cast<const UGsTableCurrencyData>(FGsSchemaCurrencyData::GetStaticTable());
				GS_UTEST_NOT_NULL(CurrencyDataTable)
				
				TArray<const FGsSchemaCurrencyData*> CurrencyDatas;
				GS_UTEST_TRUE(CurrencyDataTable->GetAllRows(CurrencyDatas))

				for (const FGsSchemaCurrencyData* CurrencyData : CurrencyDatas)
				{
					if (nullptr == CurrencyData)
					{
						AddWarning(TEXT("nullptr == CurrencyData"));
						continue;
					}

					const FGsSchemaSupplyInfoConstraintRow& SupplyInfoConstraintRow = CurrencyData->supplyInfoConstraint;
					if (SupplyInfoConstraintRow.IsNull())
					{
						continue;
					}

					const FGsSchemaSupplyInfoConstraint* SupplyInfoConstraint = SupplyInfoConstraintRow.GetRow();
					if (nullptr == SupplyInfoConstraint)
					{
						continue;
					}

					if (!ValidBMShopTabIds.Contains(SupplyInfoConstraint->bmshopTabId))
					{
						continue;
					}

					SupplyInfoHelperEditor.AddSupplyInfoSetContentsTypeCurrencyType(CurrencyData->currencyType, EGsUnlockContentsType::BMSHOP);
				}
			}
		}
	}

	{
		// https://jira.com2us.com/jira/browse/CHR-23573
		AddInfo(TEXT("스킬 획득처 수집"));

		const UGsTableUserSkillSet* UserSkillSetTable = Cast<UGsTableUserSkillSet>(FGsSchemaUserSkillSet::GetStaticTable());
		GS_UTEST_TRUE(nullptr != UserSkillSetTable);

		TArray<const FGsSchemaUserSkillSet*> UserSkillSets;
		if (nullptr != UserSkillSetTable)
		{
			GS_UTEST_TRUE(UserSkillSetTable->GetAllRows(UserSkillSets));
		}

		// 빠른 조회를 위해 <효과ID, 스킬ID> 형태의 맵을 생성해 둠.
		TMap<ItemEffectId, SkillId> EffectIdUserSkillSetIdMap;
		{
			const UGsTable* ItemEffectSkillBookTable = FGsSchemaItemEffectSkillBook::GetStaticTable();
			GS_UTEST_TRUE(nullptr != ItemEffectSkillBookTable);
			if (nullptr != ItemEffectSkillBookTable)
			{
				TArray<const FGsSchemaItemEffectSkillBook*> ItemEffectSkillBooks;
				GS_UTEST_TRUE(ItemEffectSkillBookTable->GetAllRows(ItemEffectSkillBooks));
				for (const FGsSchemaItemEffectSkillBook* ItemEffectSkillBook : ItemEffectSkillBooks)
				{
					GS_UTEST_TRUE(nullptr != ItemEffectSkillBook);
					if (nullptr == ItemEffectSkillBook || ItemEffectSkillBook->effectId ==  INVALID_ITEM_EFFECT_ID || ItemEffectSkillBook->userSkillSetId == INVALID_SKILL_ID)
					{
						continue;
					}
					EffectIdUserSkillSetIdMap.Emplace(ItemEffectSkillBook->effectId, ItemEffectSkillBook->userSkillSetId);
				}
			}
		}

		const UGsTable* ItemConsumeDetailTable = FGsSchemaItemConsumeDetail::GetStaticTable();
		GS_UTEST_TRUE(nullptr != ItemConsumeDetailTable);

		const UGsTable* CostumeCommonTable = FGsSchemaCostumeCommon::GetStaticTable();
		GS_UTEST_TRUE(nullptr != CostumeCommonTable);

		// 각 스킬을 획득할 수 있는 ItemId 목록을 저장해 둠.
		TMap<SkillId, TSet<ItemId>> UserSkillSetIdItemIdSetMap;
		if (nullptr != UserSkillSetTable && nullptr != ItemConsumeDetailTable && nullptr != CostumeCommonTable)
		{
			TArray<const FGsSchemaItemCommon*> ItemCommons;
			GS_UTEST_TRUE(ItemCommonTable->GetAllRows(ItemCommons));

			const TMap<FName, uint8*>* ItemConsumeDetailMap = nullptr;
			GS_UTEST_TRUE(ItemConsumeDetailTable->GetRowMap(ItemConsumeDetailMap));
			
			TArray<const FGsSchemaCostumeCommon*> CostumeCommons;
			GS_UTEST_TRUE(CostumeCommonTable->GetAllRows(CostumeCommons));

			// 모든 스킬의 스킬북아이템 정보 수집
			const FGsSchemaItemConsumeDetail* ItemConsumeDetail = nullptr;
			for (const FGsSchemaItemCommon* ItemCommon : ItemCommons)
			{
				if (nullptr == ItemCommon)
				{
					AddWarning(TEXT("nullptr == ItemCommon"));
					continue;
				}

				if (INVALID_ITEM_ID == ItemCommon->id)
				{
					AddWarning(TEXT("INVALID_ITEM_ID == ItemCommon->id"));
					continue;
				}

				const FName ItemConsumeDetailRowName(FString::Printf(TEXT("%d"), ItemCommon->id));
				if (!ItemConsumeDetailMap->Contains(ItemConsumeDetailRowName))
				{
					// AddWarning(FString::Printf(TEXT("!ItemConsumeDetailMap->Contains(%s)"), *ItemConsumeDetailRowName.ToString()));
					continue;
				}

				ItemConsumeDetail = (const FGsSchemaItemConsumeDetail*)ItemConsumeDetailMap->FindRef(ItemConsumeDetailRowName);

				const FGsSchemaUserSkillSet* UserSkillSetInfo = nullptr;
				for (const ItemEffectId EffectId : ItemConsumeDetail->effectIDList)
				{
					const SkillId* UserSkillSetId = EffectIdUserSkillSetIdMap.Find(EffectId);
					if (nullptr == UserSkillSetId)
					{
						// AddWarning(TEXT("nullptr == UserSkillSetId"));
						continue;
					}

					if (!UserSkillSetTable->FindRowById(StaticCast<int32>(*UserSkillSetId), UserSkillSetInfo) || nullptr == UserSkillSetInfo)
					{
						AddWarning(FString::Printf(TEXT("!UserSkillSetTable->FindRowById(StaticCast<int32>(*UserSkillSetId), UserSkillSetInfo) || nullptr == UserSkillSetInfo"), *UserSkillSetId));
						continue;
					}

					UserSkillSetIdItemIdSetMap.FindOrAdd(*UserSkillSetId).Add(ItemCommon->id);
				}
			}
		}

		// https://jira.com2us.com/jira/browse/CHR-23573
		// 일부 스킬의 경우, 패시브스킬을 획득해야 활성화되는 경우가 있음.
		// 이 경우, 해당 스킬의 직접적인 획득처에 해당되는 스킬북이 존재하지 않으므로,
		// 관련되는 패시브스킬의 스킬북을 대체하여 사용함.
		// 아래 코드에서 해당 경우의 처리를 위해 추가로 데이터를 입력함.
		const UGsTableSkillUpgradeSet* SkillUpgradeSetTable = Cast<UGsTableSkillUpgradeSet>(FGsSchemaSkillUpgradeSet::GetStaticTable());
		GS_UTEST_TRUE(nullptr != SkillUpgradeSetTable);
		if (nullptr != SkillUpgradeSetTable)
		{
			// 패시비티ID가 등록된 UserSkillSetId 맵
			TMap<PassivityId, SkillId> PassivityIdUserSkillSetIdMap;
			for (const FGsSchemaUserSkillSet* UserSkillSet : UserSkillSets)
			{
				if (nullptr == UserSkillSet)
				{
					continue;
				}

				const SkillId UserSkillSetId = UserSkillSet->id;

				const FGsSchemaSkillSet* MaleSkillSet = UserSkillSet->maleSkillId.GetRow();
				if (nullptr != MaleSkillSet)
				{
					for (const FGsSchemaPassivitySetRow& PassivitySetRow : MaleSkillSet->passivityIdList)
					{
						if (!PassivitySetRow.IsNull())
						{
							PassivityIdUserSkillSetIdMap.Add(PassivitySetRow.GetRow()->id, UserSkillSetId);
						}
					}
				}

				const FGsSchemaSkillSet* FemaleSkillSet = UserSkillSet->maleSkillId.GetRow();
				if (nullptr != FemaleSkillSet)
				{
					for (const FGsSchemaPassivitySetRow& PassivitySetRow : FemaleSkillSet->passivityIdList)
					{
						if (!PassivitySetRow.IsNull())
						{
							PassivityIdUserSkillSetIdMap.Add(PassivitySetRow.GetRow()->id, UserSkillSetId);
						}
					}
				}
			}

			// 키: 업그레이드 UserSkillSetId, 값: 패시브 UserSkillSetId
			// TMap<SkillId, SkillId> UpgradeUserSkillSetIdPassiveUserSkillSetIdMap;

			TArray<const FGsSchemaSkillUpgradeSet*> SkillUpgradeSets;
			GS_UTEST_TRUE(SkillUpgradeSetTable->GetAllRows(SkillUpgradeSets));

			for (const FGsSchemaSkillUpgradeSet* SkillUpgradeSet : SkillUpgradeSets)
			{
				if (nullptr == SkillUpgradeSet)
				{
					continue;
				}

				for (const FGsSchemaSkillUpgradeStepInfo& SkillUpgradeStepInfo : SkillUpgradeSet->upgradeInfoList)
				{
					if (SkillUpgradeStepInfo.upgradeSkillId.IsNull())
					{
						continue;
					}

					const FGsSchemaUserSkillSet* UserSkillSet = SkillUpgradeStepInfo.upgradeSkillId.GetRow();
					const SkillId UpgradeUserSkillSetId = UserSkillSet->id;

					if (SkillUpgradeStepInfo.requirePassivity.IsNull())
					{
						continue;
					}

					const FGsSchemaPassivitySet* PassivitySet = SkillUpgradeStepInfo.requirePassivity.GetRow();
					const SkillId *PassiveUserSkillSetId = PassivityIdUserSkillSetIdMap.Find(PassivitySet->id);
					if (nullptr == PassiveUserSkillSetId)
					{
						continue;
					}

					const TSet<ItemId>* ItemIds = UserSkillSetIdItemIdSetMap.Find(*PassiveUserSkillSetId);
					if (nullptr == ItemIds)
					{
						continue;
					}

					for (const ItemId CurrentItemId : *ItemIds)
					{
						UserSkillSetIdItemIdSetMap.FindOrAdd(UpgradeUserSkillSetId).Add(CurrentItemId);
					}
				}
			}
		}

		{
			// 스킬 획득처 수집. 위 단계에서 수집한 아이템 획득처 내용을 참조하여 새로운 획득처 모음을 생성하여 저장함.
			for (const TPair<SkillId, TSet<ItemId>>& UserSkillSetIdItemIdSet : UserSkillSetIdItemIdSetMap)
			{
				const SkillId UserSkillSetId = UserSkillSetIdItemIdSet.Key;

				SupplyInfoHelperEditor.AddSupplyInfoSetItemIdsSkillId(UserSkillSetId, UserSkillSetIdItemIdSet.Value);

				for (const ItemId CurrentItemId : UserSkillSetIdItemIdSet.Value)
				{
					const FGsSchemaItemSupplyInfoSet* SupplyInfoSet = SupplyInfoHelperEditor.GetSupplyInfoSetItemId(CurrentItemId);
					if (nullptr == SupplyInfoSet)
					{
						continue;
					}

					for (const EGsUnlockContentsType& ContentsType : SupplyInfoSet->contentsTypeSet)
					{
						SupplyInfoHelperEditor.AddSupplyInfoSetContentsTypeSkillId(UserSkillSetId, ContentsType);
					}

					for (const TPair<int32, FGsSchemaDropSupplyInfo>& ContentsEventMapCreatureSetPair : SupplyInfoSet->contentsEventMapCreatureSetMap)
					{
						TMap<ContentsEventId, TMap<MapId, TArray<FGsSchemaWorldMapMonsterElementDataRow>>> ContentsEventMapWorldMapMonsterElementDataRows;

						const ContentsEventId CurrentContentsEventId = ContentsEventMapCreatureSetPair.Key;
						for (const TPair<int32, FGsSchemaDropSupplyInfoCreatureIds>& WorldMapMonsterElementDataRowsPair : ContentsEventMapCreatureSetPair.Value.maps)
						{
							const MapId CurrentMapId = WorldMapMonsterElementDataRowsPair.Key;
							for (const FGsSchemaWorldMapMonsterElementDataRow& WorldMapMonsterElementDataRow : WorldMapMonsterElementDataRowsPair.Value.worldmapMonsters)
							{
								ContentsEventMapWorldMapMonsterElementDataRows.FindOrAdd(CurrentContentsEventId).FindOrAdd(CurrentMapId).Emplace(WorldMapMonsterElementDataRow);
							}
						}
						
						SupplyInfoHelperEditor.AddSupplyInfoSetContentsEventMapCreatureSkillId(UserSkillSetId, ContentsEventMapWorldMapMonsterElementDataRows);
					}

					for (const TPair<int32, FGsSchemaDropSupplyInfoCreatureIds>& MapCreatureSetPair : SupplyInfoSet->mapCreatureSetMap)
					{
						const MapId CurrentMapId = MapCreatureSetPair.Key;
						const FGsSchemaDropSupplyInfoCreatureIds& MapCreatureSet = MapCreatureSetPair.Value;
						for (const FGsSchemaWorldMapMonsterElementDataRow& WorldmapMonster : MapCreatureSet.worldmapMonsters)
						{
							SupplyInfoHelperEditor.AddSupplyInfoSetMapCreatureSkillId(UserSkillSetId, CurrentMapId, INVALID_CREATURE_ID, WorldmapMonster);
						}
					}

					for (const CraftId& CurrentCraftId : SupplyInfoSet->craftIdSet)
					{
						SupplyInfoHelperEditor.AddSupplyInfoSetCraftIdSkillId(UserSkillSetId, CurrentCraftId);
					}
				}
			}
		}
	}

	// 저장
	GS_UTEST_TRUE(SupplyInfoHelperEditor.SaveToDataTables());

	AddInfo(TEXT("공급처 테이블 생성 완료"));

	return true;
}

#endif
