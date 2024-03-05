#if WITH_DEV_AUTOMATION_TESTS

#include "Runtime/Core/Public/Misc/AutomationTest.h"

#include "../Plugins/RSQL/Source/RSQLEditor/Public/GsTableManagerEditor.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "DataSchema/BMShop/GsSchemaBMShopTabInfo.h"
#include "DataSchema/Costume/GsSchemaCostumeCeiling.h"
#include "DataSchema/DemandInfo/GsSchemaCurrencyDemandInfoSet.h"
#include "DataSchema/DemandInfo/GsSchemaDemandInfoConstraint.h"
#include "DataSchema/DemandInfo/GsSchemaItemDemandInfoSet.h"
#include "DataSchema/Fairy/GsSchemaFairyCeiling.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftMaterial.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftRecipe.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "DataSchema/Item/GsSchemaItemEffectSpawnTreasureMonster.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/WorldMap/GsSchemaContinentalMapData.h"
#include "DataSchema/Seal/GsSchemaSealNodeLevel.h"
#include "DataSchema/Seal/GsSchemaSealNodeRerollCost.h"
#include "DataSchema/Skill/Cost/GsSchemaSkillUpCostSet.h"
#include "DataSchema/Skill/Cost/GsSchemaSkillUpCurrencyInfo.h"
#include "DataSchema/Skill/Cost/GsSchemaSkillUpItemInfo.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
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

struct FGsDemandInfoHelperEditor final
{

public:
	bool _isValidIniatialized;

private:
	FAutomationTestBase& _automationTest;

	// 최종 데이터들을 저장할 컨테이너
	FGsUniqueArrayEditor<CurrencyType, FGsSchemaCurrencyDemandInfoSet> _currencyDemandInfoUniqueArray;
	FGsUniqueArrayEditor<ItemId, FGsSchemaItemDemandInfoSet> _itemDemandInfoUniqueArray;

	UGsTable* _currencyDemandInfoSetTable;
	UGsTable* _itemDemandInfoSetTable;

public:
	explicit FGsDemandInfoHelperEditor(FAutomationTestBase& InAutomationTest)
		: _automationTest(InAutomationTest)
	{
		UGsTableManagerEditor& TableManager = UGsTableManagerEditor::GetInstance();

		_currencyDemandInfoSetTable = TableManager.GetTable(FGsSchemaCurrencyDemandInfoSet::StaticStruct());
		_itemDemandInfoSetTable = TableManager.GetTable(FGsSchemaItemDemandInfoSet::StaticStruct());

		_isValidIniatialized = _automationTest.TestNotNull(TEXT("_currencyDemandInfoSetTable"), _currencyDemandInfoSetTable) && _automationTest.TestNotNull(TEXT("_itemDemandInfoSetTable"), _itemDemandInfoSetTable);
	}

private:
	static bool IsPassConstraint(const FGsSchemaDemandInfoConstraintRow& InDemandInfoConstraintRow, const EGsUnlockContentsType InContentsType)
	{
		if (InDemandInfoConstraintRow.IsNull())
		{
			// 별도 지정한 추가 정보 없을 경우, 무조건 수집
			return true;
		}

		const FGsSchemaDemandInfoConstraint* DemandInfoConstraint = InDemandInfoConstraintRow.GetRow();
		if (nullptr == DemandInfoConstraint)
		{
			// AddWarning(FString::Printf(TEXT("ItemCommon 테이블에 비정상 DemandInfoConstraint: ItemCommon->id: %d, InDemandInfoConstraintRow.RowName:%s"), ItemCommon->id, *InDemandInfoConstraintRow.RowName.ToString()));
			return false;
		}

		if (EGsContentsShortcutMethod::Inclusive == DemandInfoConstraint->showContentsTypesMethod)
		{
			// 포괄적 리스트인 경우에는, 모두 가능
			return true;
		}
		else if (EGsContentsShortcutMethod::Exclusive == DemandInfoConstraint->showContentsTypesMethod)
		{
			// 배타적 리스트인 경우, 일치하는 경우에만 가능
			return DemandInfoConstraint->showContentsTypes.Contains(InContentsType);
		}

		return false;
	}

private:
	/** 재화를 사용처 테이블에 입력해야 하는 지 여부를 반환. */
	static bool NeedToCollectDemandInfo(const EGsUnlockContentsType InContentsType, const CurrencyType& InCurrencyType)
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
			return false;
		}

		if (!FGsDemandInfoHelperEditor::IsPassConstraint(CurrencyData->demandInfoConstraint, InContentsType))
		{
			return false;
		}

		return true;
	}

	/** 아이템을 사용처 테이블에 입력해야 하는 지 여부를 반환. */
	static bool NeedToCollectDemandInfo(const EGsUnlockContentsType InContentsType, const ItemId& InItemId)
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

		if (!FGsDemandInfoHelperEditor::IsPassConstraint(ItemCommon->demandInfoConstraint, InContentsType))
		{
			return false;
		}

		return true;
	}

private:
	FGsSchemaCurrencyDemandInfoSet* GetValidDemandInfoSet(const CurrencyType& InCurrencyType)
	{
		if (CurrencyType::NONE == InCurrencyType || CurrencyType::MAX == InCurrencyType)
		{
			return nullptr;
		}

		FGsSchemaCurrencyDemandInfoSet* CurrencyDemandInfoSet = _currencyDemandInfoUniqueArray.Find(InCurrencyType);
		if (nullptr == CurrencyDemandInfoSet)
		{
			CurrencyDemandInfoSet = &_currencyDemandInfoUniqueArray.Add(InCurrencyType);
			CurrencyDemandInfoSet->currencyType = InCurrencyType;
			CurrencyDemandInfoSet->RowName = FName(FString::Printf(TEXT("%d"), StaticCast<int32>(InCurrencyType)));
		}
		return CurrencyDemandInfoSet;
	}

	FGsSchemaItemDemandInfoSet* GetValidDemandInfoSet(const ItemId& InItemId)
	{
		if (INVALID_ITEM_ID == InItemId)
		{
			return nullptr;
		}

		FGsSchemaItemDemandInfoSet* ItemDemandInfoSet = _itemDemandInfoUniqueArray.Find(InItemId);
		if (nullptr == ItemDemandInfoSet)
		{
			ItemDemandInfoSet = &_itemDemandInfoUniqueArray.Add(InItemId);
			ItemDemandInfoSet->itemId = InItemId;
			ItemDemandInfoSet->RowName = FName(FString::Printf(TEXT("%d"), InItemId));
		}
		return ItemDemandInfoSet;
	}

public:
	template<typename TKey>
	void AddDemandInfoSetContentsType(const TKey InKey, const EGsUnlockContentsType InDemandInfoType)
	{
		if (!FGsDemandInfoHelperEditor::NeedToCollectDemandInfo(InDemandInfoType, InKey))
		{
			return;
		}

		auto* DemandInfoSet = GetValidDemandInfoSet(InKey);
		if (nullptr == DemandInfoSet)
		{
			return;
		}

		DemandInfoSet->contentsTypeSet.Add(InDemandInfoType);
	}

	template<typename TKey>
	void AddDemandInfoSetMapId(const TKey InKey, const MapId InMapId)
	{
		if (!FGsDemandInfoHelperEditor::NeedToCollectDemandInfo(EGsUnlockContentsType::WORLDMAP, InKey))
		{
			return;
		}

		auto* DemandInfoSet = GetValidDemandInfoSet(InKey);
		if (nullptr == DemandInfoSet)
		{
			return;
		}

		DemandInfoSet->mapIds.Add(InMapId);
	}

	template<typename TKey>
	void AddDemandInfoSetCraftId(const TKey InKey, const CraftId InCraftId)
	{
		if (!FGsDemandInfoHelperEditor::NeedToCollectDemandInfo(EGsUnlockContentsType::CRAFT, InKey))
		{
			return;
		}

		auto* DemandInfoSet = GetValidDemandInfoSet(InKey);
		if (nullptr == DemandInfoSet)
		{
			return;
		}

		DemandInfoSet->craftIds.Add(InCraftId);
	}

public:
	void CollectDemandInfoSetByItemCategorySub(const FGsSchemaItemCommon& InItemCommon)
	{
		const ItemId CurrentItemId = StaticCast<ItemId>(InItemCommon.id);
		if (0 >= CurrentItemId)
		{
			_automationTest.AddInfo(FString::Printf(TEXT("0 >= CurrentItemId(%d)"), CurrentItemId));
			return;
		}

		switch (InItemCommon.categorySub)
		{

		// 몬스터도감
		case ItemCategorySub::MONSTER_KNOWLEDGE_GAIN_EXP:
		{
			AddDemandInfoSetContentsType(CurrentItemId, EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION);
		}
		break;

		// 보물몬스터
		case ItemCategorySub::SPAWN_TREASURE_MONSTER:
		{
			if (0 >= InItemCommon.detailId)
			{
				return;
			}

			const UGsTableItemConsumeDetail* ItemConsumDetailTable = Cast<const UGsTableItemConsumeDetail>(FGsSchemaItemConsumeDetail::GetStaticTable());
			const UGsTableItemEffectSpawnTreasureMonster* spawnEffectTable = Cast<const UGsTableItemEffectSpawnTreasureMonster>(FGsSchemaItemEffectSpawnTreasureMonster::GetStaticTable());
			if (nullptr == ItemConsumDetailTable || nullptr == spawnEffectTable)
			{
				return;
			}

			const FGsSchemaItemConsumeDetail* consumDetailData = nullptr;
			if (false == ItemConsumDetailTable->FindRowById(InItemCommon.detailId, consumDetailData) || nullptr == consumDetailData)
			{
				_automationTest.AddInfo(FString::Printf(TEXT("false == ItemConsumDetailTable->FindRowById(%d) || nullptr == consumDetailData"), InItemCommon.detailId));
				return;
			}

			const FGsSchemaItemEffectSpawnTreasureMonster* spawnTreasureData = nullptr;
			for (const int32 effectId : consumDetailData->effectIDList)
			{
				if (0 >= effectId || false == spawnEffectTable->FindRowById(effectId, spawnTreasureData) || nullptr == spawnTreasureData)
				{
					return;
				}

				// allowedLocation 의 id 는 mapData 테이블의 id 임
				for (const int32 mapId : spawnTreasureData->allowedLocation)
				{
					AddDemandInfoSetMapId(CurrentItemId, StaticCast<MapId>(mapId));
				}
			}
		}
		break;

		// 페어리 강화
		case ItemCategorySub::ENCHANT_FAIRY:
		{
			AddDemandInfoSetContentsType(CurrentItemId, EGsUnlockContentsType::FAIRY);
		}
		break;

		}
	}

public:
	bool SaveToDataTables()
	{
		if (!_automationTest.TestTrue(TEXT("_isValidIniatialized"), _isValidIniatialized))
		{
			return false;
		}

		_currencyDemandInfoSetTable->Empty();
		for (const FGsSchemaCurrencyDemandInfoSet& iter : _currencyDemandInfoUniqueArray.GetArray())
		{
			_currencyDemandInfoSetTable->AddRow(iter);
		}

		if (!_automationTest.TestTrue(TEXT("_currencyDemandInfoSetTable->SaveDataTable()"), _currencyDemandInfoSetTable->SaveDataTable()))
		{
			return false;
		}

		_itemDemandInfoSetTable->Empty();
		for (const FGsSchemaItemDemandInfoSet& iter : _itemDemandInfoUniqueArray.GetArray())
		{
			_itemDemandInfoSetTable->AddRow(iter);
		}

		if (!_automationTest.TestTrue(TEXT("_itemDemandInfoSetTable->SaveDataTable()"), _itemDemandInfoSetTable->SaveDataTable()))
		{
			return false;
		}

		return true;
	}

};

#define GS_UTEST_TRUE(What) UTEST_TRUE(TEXT(#What), What)
#define GS_UTEST_NOT_NULL(What) UTEST_NOT_NULL(TEXT(#What), What)

/**
 * 사용처 테이블 생성
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(EditorPostBuildMakeUtilizeInfoSets, TEXT("Raon.EditorPostBuild.5 사용처 테이블 생성")
	, EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriorityAndAbove | EAutomationTestFlags::EngineFilter)
	bool EditorPostBuildMakeUtilizeInfoSets::RunTest(const FString& Parameters)
{
	AddInfo(TEXT("사용처 테이블 생성 시작"));

	FGsDemandInfoHelperEditor DemandInfoHelperEditor(*this);
	GS_UTEST_TRUE(DemandInfoHelperEditor._isValidIniatialized);

	// 메인 테이블 로드
	const UGsTableItemCommon* ItemCommonTable = Cast<const UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	const UGsTableMapData* MapDataTable = Cast<UGsTableMapData>(FGsSchemaMapData::GetStaticTable());
	GS_UTEST_NOT_NULL(ItemCommonTable)
	GS_UTEST_NOT_NULL(MapDataTable)

	AddInfo(TEXT("유효한 맵 데이터 수집"));
	TMap<MapId, const FGsSchemaMapData*> ValidMaps;

	{
		// 컨티넨탈맵 테이블에 없는 맵 스킵하기 위해 사용
		const UGsTableContinentalMapData* ContinentalMapDataTable = Cast<UGsTableContinentalMapData>(FGsSchemaContinentalMapData::GetStaticTable());
		GS_UTEST_NOT_NULL(ContinentalMapDataTable)

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

			if (MapType::PUBLIC_FIELD != MapData->mapType && MapType::PUBLIC_DUNGEON != MapData->mapType && MapType::INTER_FIELD != MapData->mapType)
			{
				AddInfo(FString::Printf(TEXT("맵 타입 PUBLIC_FIELD, PUBLIC_DUNGEON 이 아닌 맵 스킵. mapType: %d. mapId: %d"), MapData->mapType, CurrentMapId));
				continue;
			}

			const FGsSchemaAreaData* AreaData = MapData->areaId.GetRow();
			if (nullptr == AreaData)
			{
				AddInfo(FString::Printf(TEXT("nullptr == AreaData 맵 스킵. mapId: %d"), CurrentMapId));
				continue;
			}

			const ContinentId CurrentContinentId = AreaData->continentId;
			if (INVALID_CONTINENT_ID == CurrentContinentId)
			{
				AddInfo(FString::Printf(TEXT("INVALID_CONTINENT_ID == CurrentContinentId 맵 스킵. mapId: %d"), CurrentMapId));
				continue;
			}

			if (!ContinentalMapDataTable->FindRowById(AreaData->continentId, DummyContinentalMapData) || nullptr == DummyContinentalMapData)
			{
				AddInfo(FString::Printf(TEXT("ContinentalMapData 테이블에 존재하지 않는 ContinentId 스킵. mapId: %d, continentalId: %d"), CurrentMapId, AreaData->continentId));
				continue;
			}

			// 유효한 맵 목록에 저장. 여기 할당된 맵 외에는 아래 로직에서 모두 제외될 것.
			ValidMaps.Add(CurrentMapId, MapData);
		}
	}

	{
		AddInfo(TEXT("ItemCategorySub 별 사용처 수집"));

		TArray<const FGsSchemaItemCommon*> itemDataList;
		GS_UTEST_TRUE(ItemCommonTable->GetAllRows(itemDataList))
		for (const FGsSchemaItemCommon* ItemCommon : itemDataList)
		{
			if (nullptr == ItemCommon)
			{
				GS_UTEST_NOT_NULL(ItemCommon)
				continue;
			}

			// ItemCategorySub 별 사용처 수집
			DemandInfoHelperEditor.CollectDemandInfoSetByItemCategorySub(*ItemCommon);
		}
	}

	{
		AddInfo(TEXT("제작 사용처 수집"));

		const UGsTable* CraftRecipeTable = FGsSchemaItemCraftRecipe::GetStaticTable();
		GS_UTEST_NOT_NULL(CraftRecipeTable)

		TArray<const FGsSchemaItemCraftRecipe*> ItemCraftRecipes;
		GS_UTEST_TRUE(CraftRecipeTable->GetAllRows(ItemCraftRecipes))

		// 대체아이템 목록을 위해 실제 재료 아이템의 ID 는 아래 테이블에 저장함.
		const UGsTableItemCraftMaterial* ItemCraftMaterialTable = Cast<UGsTableItemCraftMaterial>(FGsSchemaItemCraftMaterial::GetStaticTable());
		const FGsSchemaItemCraftMaterial* ItemCraftMaterial = nullptr;
		for (const FGsSchemaItemCraftRecipe* CraftRecipe : ItemCraftRecipes)
		{
			if (nullptr == CraftRecipe)
			{
				AddWarning(TEXT("ItemCraftRecipe 테이블 itemCraftRecipeId 비정상. nullptr != CraftRecipe"));
				continue;
			}

			const CraftId CurrentCraftId = CraftRecipe->itemCraftRecipeId;
			if (INVALID_CRAFT_ID == CurrentCraftId)
			{
				AddWarning(FString::Printf(TEXT("ItemCraftRecipe 테이블 itemCraftRecipeId 비정상. RowName:%d"), *CraftRecipe->RowName.ToString()));
				continue;
			}

			const TArray<int32> MaterialKeys = {
				CraftRecipe->material1,
				CraftRecipe->material2,
				CraftRecipe->material3,
				CraftRecipe->material4,
				CraftRecipe->material5,
				CraftRecipe->material6,
				CraftRecipe->material7,
				CraftRecipe->material8,
				CraftRecipe->material9,
				CraftRecipe->material10,
			};

			// 제작 재화/아이템 사용처 수집
			DemandInfoHelperEditor.AddDemandInfoSetCraftId(CraftRecipe->currencyType, CurrentCraftId);
			
			for (const int32& MaterialKey : MaterialKeys)
			{
				if (ItemCraftMaterialTable->FindRowById(MaterialKey, ItemCraftMaterial) && nullptr != ItemCraftMaterial)
				{
					//const FGsSchemaItemCommon* ItemCommon = nullptr;
					for (const int32 MaterialItemId : ItemCraftMaterial->itemIds)
					{
						if (INVALID_ITEM_ID != MaterialItemId)
						{
							const ItemId CurrentItemId = StaticCast<ItemId>(MaterialItemId);
							//if (!ItemCommonTable->FindRowById(CurrentItemId, ItemCommon))
							//{
							//	AddWarning(TEXT("!ItemCommonTable->FindRowById(CurrentItemId, ItemCommon), MaterialItemId:%d"), CurrentItemId);
							//	continue;
							//}

							DemandInfoHelperEditor.AddDemandInfoSetCraftId(CurrentItemId, CurrentCraftId);
						}
					}
				}
			}
		}
	}

	{
		AddInfo(TEXT("스킬 강화 사용처 수집"));

		const UGsTable* SkillUpCostSetTable = FGsSchemaSkillUpCostSet::GetStaticTable();
		GS_UTEST_NOT_NULL(SkillUpCostSetTable)
		
		TArray<const FGsSchemaSkillUpCostSet*> SkillUpCostSets;
		GS_UTEST_TRUE(SkillUpCostSetTable->GetAllRows(SkillUpCostSets))

		for (const FGsSchemaSkillUpCostSet* SkillUpCostSet : SkillUpCostSets)
		{
			if (nullptr == SkillUpCostSet)
			{
				AddWarning(TEXT("nullptr == SkillUpCostSet"));
				continue;
			}

			for (const FGsSchemaSkillUpCurrencyInfo& SkillUpCurrencyInfo : SkillUpCostSet->currencyList)
			{
				DemandInfoHelperEditor.AddDemandInfoSetContentsType(SkillUpCurrencyInfo.currencyType, EGsUnlockContentsType::SKILL);
			}

			for (const FGsSchemaSkillUpItemInfo& SkillUpItemInfo : SkillUpCostSet->itemList)
			{
				DemandInfoHelperEditor.AddDemandInfoSetContentsType(StaticCast<ItemId>(SkillUpItemInfo.itemId), EGsUnlockContentsType::SKILL);
			}
		}
	}

	{
		AddInfo(TEXT("코스튬 사용처 수집"));

		const UGsTable* CostumeCeilingTable = FGsSchemaCostumeCeiling::GetStaticTable();
		GS_UTEST_NOT_NULL(CostumeCeilingTable)

		TArray<const FGsSchemaCostumeCeiling*> CostumeCeilings;
		GS_UTEST_TRUE(CostumeCeilingTable->GetAllRows(CostumeCeilings));

		for (const FGsSchemaCostumeCeiling* CostumeCeiling : CostumeCeilings)
		{
			if (nullptr == CostumeCeiling)
			{
				AddWarning(TEXT("nullptr == CostumeCeiling"));
				continue;
			}

			const FGsSchemaItemCommonRow& ItemCommonRow = CostumeCeiling->needItemId;
			if (ItemCommonRow.IsNull())
			{
				continue;
			}

			DemandInfoHelperEditor.AddDemandInfoSetContentsType(StaticCast<ItemId>(ItemCommonRow.GetRow()->id), EGsUnlockContentsType::COSTUME);
		}
	}

	{
		AddInfo(TEXT("페어리 사용처 수집"));

		const UGsTable* FairyCeilingTable = FGsSchemaFairyCeiling::GetStaticTable();
		GS_UTEST_NOT_NULL(FairyCeilingTable)

		TArray<const FGsSchemaFairyCeiling*> FairyCeilings;
		GS_UTEST_TRUE(FairyCeilingTable->GetAllRows(FairyCeilings));

		for (const FGsSchemaFairyCeiling* FairyCeiling : FairyCeilings)
		{
			if (nullptr == FairyCeiling)
			{
				AddWarning(TEXT("nullptr == FairyCeiling"));
				continue;
			}

			const FGsSchemaItemCommonRow& ItemCommonRow = FairyCeiling->needItemId;
			if (ItemCommonRow.IsNull())
			{
				continue;
			}

			DemandInfoHelperEditor.AddDemandInfoSetContentsType(StaticCast<ItemId>(ItemCommonRow.GetRow()->id), EGsUnlockContentsType::FAIRY);
		}
	}
	
	{
		AddInfo(TEXT("상점 정보 데이터테이블 수집"));

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

					const FGsSchemaDemandInfoConstraintRow& DemandInfoConstraintRow = ItemCommon->demandInfoConstraint;
					if (DemandInfoConstraintRow.IsNull())
					{
						continue;
					}

					const FGsSchemaDemandInfoConstraint* DemandInfoConstraint = DemandInfoConstraintRow.GetRow();
					if (nullptr == DemandInfoConstraint)
					{
						continue;
					}

					if (!ValidBMShopTabIds.Contains(DemandInfoConstraint->bmshopTabId))
					{
						continue;
					}

					DemandInfoHelperEditor.AddDemandInfoSetContentsType(StaticCast<ItemId>(ItemCommon->id), EGsUnlockContentsType::BMSHOP);
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

					const FGsSchemaDemandInfoConstraintRow& DemandInfoConstraintRow = CurrencyData->demandInfoConstraint;
					if (DemandInfoConstraintRow.IsNull())
					{
						continue;
					}

					const FGsSchemaDemandInfoConstraint* DemandInfoConstraint = DemandInfoConstraintRow.GetRow();
					if (nullptr == DemandInfoConstraint)
					{
						continue;
					}

					if (!ValidBMShopTabIds.Contains(DemandInfoConstraint->bmshopTabId))
					{
						continue;
					}

					DemandInfoHelperEditor.AddDemandInfoSetContentsType(CurrencyData->currencyType, EGsUnlockContentsType::BMSHOP);
				}
			}
		}
	}

	{
		AddInfo(TEXT("봉인 사용처 수집"));

		const UGsTable* SealNodeLevelTable = FGsSchemaSealNodeLevel::GetStaticTable();
		GS_UTEST_NOT_NULL(SealNodeLevelTable)

		TArray<const FGsSchemaSealNodeLevel*> SealNodeLevels;
		GS_UTEST_TRUE(SealNodeLevelTable->GetAllRows(SealNodeLevels));

		for (const FGsSchemaSealNodeLevel* SealNodeLevel : SealNodeLevels)
		{
			if (nullptr == SealNodeLevel)
			{
				AddWarning(TEXT("nullptr == SealNodeLevel"));
				continue;
			}

			for (const ItemId CurrentItemId : SealNodeLevel->costItemId)
			{
				DemandInfoHelperEditor.AddDemandInfoSetContentsType(CurrentItemId, EGsUnlockContentsType::SEAL);
			}

			DemandInfoHelperEditor.AddDemandInfoSetContentsType(SealNodeLevel->costCurrencyType, EGsUnlockContentsType::SEAL);
		}

		const UGsTable* SealNodeRerollCostTable = FGsSchemaSealNodeRerollCost::GetStaticTable();
		GS_UTEST_NOT_NULL(SealNodeRerollCostTable)

		TArray<const FGsSchemaSealNodeRerollCost*> SealNodeRerollCosts;
		GS_UTEST_TRUE(SealNodeRerollCostTable->GetAllRows(SealNodeRerollCosts));

		for (const FGsSchemaSealNodeRerollCost* SealNodeRerollCost : SealNodeRerollCosts)
		{
			if (nullptr == SealNodeRerollCost)
			{
				AddWarning(TEXT("nullptr == SealNodeRerollCost"));
				continue;
			}

			for (const ItemId CurrentItemId : SealNodeRerollCost->itemId)
			{
				DemandInfoHelperEditor.AddDemandInfoSetContentsType(CurrentItemId, EGsUnlockContentsType::SEAL);
			}

			DemandInfoHelperEditor.AddDemandInfoSetContentsType(SealNodeRerollCost->currencyType, EGsUnlockContentsType::SEAL);
		}
	}

	GS_UTEST_TRUE(DemandInfoHelperEditor.SaveToDataTables());

	return true;
}

#endif
