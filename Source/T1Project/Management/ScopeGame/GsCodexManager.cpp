#include "GsCodexManager.h"
#include "GsUnlockManager.h"
#include "../Data/GsDataContainManager.h"
#include "../Item/GsItemManager.h"
#include "../Currency/GsCostPackage.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "GsTable.h"
#include "GsTableManager.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Item/GsSchemaItemIngredientDetail.h"
#include "DataSchema/Item/GsSchemaItemEffectGainMonsterKnowledgeExp.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "T1Project.h"

#include "DataSchema/Codex/GsSchemaCodexCategory.h"
#include "DataSchema/Codex/GsSchemaCodexCategoryBalance.h"
#include "DataSchema/Codex/SchemaOnly/GsSchemaCodexCategoryBalanceLevelData.h"
#include "DataSchema/Codex/GsSchemaCodexMapGroup.h"
#include "DataSchema/Codex/GsSchemaCodexMapGroupBalance.h"
#include "DataSchema/Codex/GsSchemaLevelExp.h"

#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"

#include "Data/GsGlobalConstant.h"

#include "Message/MessageParam/GsMonsterKnowledgeCollection.h"
#include "Message/GsMessageContents.h"

#include "EffectText/GsEffectTextDefine.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "Algo/Transform.h"
#include "UTIL/GsTableUtil.h"

void FGsCodexManager::BuildMapGroupToNpcList()
{
	const UGsTableWorldMapMonsterElementData* worldMapMonsterElementDataTable = Cast<UGsTableWorldMapMonsterElementData>(FGsSchemaWorldMapMonsterElementData::GetStaticTable());
	if (nullptr == worldMapMonsterElementDataTable)
	{
		GSLOG(Error, TEXT("nullptr == worldMapMonsterElementDataTable"));
		return;
	}

	TArray<const FGsSchemaWorldMapMonsterElementData*> worldMapMonsterElementDataList;
	worldMapMonsterElementDataTable->GetAllRows(worldMapMonsterElementDataList);

	//// 데이터 정렬하여 저장
	//// 1. MapId 순으로 정렬
	//// 2. 보스 -> 엘리트 -> 노멀 순서로 정렬 (등급 내림차순)
	//// 3. 같은 등급일 경우, NpcId 오름차순
	//// 4. NpcId 도 같으면 레벨 오름차순
	//worldMapMonsterElementDataList.Sort([](const FGsSchemaWorldMapMonsterElementData* lhs, const FGsSchemaWorldMapMonsterElementData* rhs) {
	//	if (lhs->mapId != rhs->mapId)
	//		return lhs->mapId < rhs->mapId;
	//	else if (lhs->rank != rhs->rank)
	//		return lhs->rank > rhs->rank;
	//	else if (lhs->id.GetRow()->id != rhs->id.GetRow()->id)
	//		return lhs->id.GetRow()->id < rhs->id.GetRow()->id;
	//	return lhs->level < rhs->level;
	//	});

	// MapId/NpcId 페어를 만들어 동일맵, 동일 NpcId 인 녀석 걸러내기 위해 사용할 Map
	TMap<MapId, TSet<CreatureId>> mapIdToNpcIdSet;
	TMap<CodexMapGroupId, TArray<TTuple<const FGsSchemaNpcData*, MapId, CreatureRankType>>> mapGroupToNpcRankPairList;

	// 특정 맵 그룹 선택 시, 보여줄 몬스터 목록 (Grade 별로 구분) 캐싱
	for (const FGsSchemaWorldMapMonsterElementData* elementData : worldMapMonsterElementDataList)
	{
		const FGsSchemaNpcData* npcData = elementData->id.GetRow();
		if (nullptr == npcData)
			continue;

		// 도감에 들어가면 안되는 녀석에 대한 예외처리
		const FGsSchemaMonsterKnowledgeLevelSet* monsterKnowledgeLevelSet = npcData->monsterKnowledgeLevelSetId.GetRow();
		if (!monsterKnowledgeLevelSet || monsterKnowledgeLevelSet->id == 0)
			continue;

		if (!mapToMapGroup.Contains(elementData->mapId))
			continue;

		// 중복 제거 작업
		if (!mapIdToNpcIdSet.Contains(elementData->mapId))
			mapIdToNpcIdSet.Emplace(elementData->mapId);

		if (mapIdToNpcIdSet[elementData->mapId].Contains(npcData->id))
			continue;
		mapIdToNpcIdSet[elementData->mapId].Emplace(npcData->id);

		CodexMapGroupId mapGroupId = mapToMapGroup[elementData->mapId];

		if (!mapGroupToRankToNpcList.Contains(mapGroupId))
			mapGroupToRankToNpcList.Emplace(mapGroupId);

		if (!mapGroupToRankToNpcList[mapGroupId].Contains(npcData->grade))
			mapGroupToRankToNpcList[mapGroupId].Emplace(npcData->grade);

		mapGroupToRankToNpcList[mapGroupId][npcData->grade].Emplace(npcData);

		if (!mapGroupToNpcRankPairList.Contains(mapGroupId))
			mapGroupToNpcRankPairList.Emplace(mapGroupId);
		mapGroupToNpcRankPairList[mapGroupId].Emplace(npcData, elementData->mapId, npcData->grade);
	}

	for (auto& mapGroupIdListPair : mapGroupToNpcRankPairList)
	{
		// auto : TTuple<const FGsSchemaNpcData*, MapId, CreatureRankType>
		mapGroupIdListPair.Value.StableSort([](auto& lhs, auto& rhs)
			{
				return lhs.template Get<2>() > rhs.template Get<2>();
			});
	}

	for (auto& mapGroupIdToNpcRankPair : mapGroupToNpcRankPairList)
	{
		mapGroupToMapNpcPairList.Emplace(mapGroupIdToNpcRankPair.Key);
		Algo::Transform(mapGroupIdToNpcRankPair.Value, mapGroupToMapNpcPairList[mapGroupIdToNpcRankPair.Key], [](auto& npcMapRankTuple)
			{
				return MakeTuple(npcMapRankTuple.template Get<1>(), npcMapRankTuple.template Get<0>());
			});
	}
}

void FGsCodexManager::BuildGemIdToKnowledGainAmount()
{
	TArray<const FGsSchemaItemCommon*> itemList;
	const UGsTableItemCommon* itemTable = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == itemTable)
	{
		GSLOG(Error, TEXT("nullptr == itemTable"));
		return;
	}
	itemTable->GetAllRows(itemList);

	TArray<TPair<ItemId, int32>> itemIngredientPairList;
	Algo::TransformIf(itemList, itemIngredientPairList, [](const FGsSchemaItemCommon* item)
		{
			return item->categorySub == ItemCategorySub::MONSTER_KNOWLEDGE_GAIN_EXP;
		},[](const FGsSchemaItemCommon* item)
		{
			return MakeTuple(item->id, item->detailId);
		});

	for (const auto& itemIngredientPair : itemIngredientPairList)
	{
		const FGsSchemaItemIngredientDetail* ingredientData = UGsTableUtil::FindRowById<UGsTableItemIngredientDetail, FGsSchemaItemIngredientDetail>(itemIngredientPair.Value);

		if (nullptr == ingredientData)
			continue;

		if (!ingredientData->effectIDList.IsValidIndex(0))
			continue;

		int32 itemEffectGainId = ingredientData->effectIDList[0];

		const FGsSchemaItemEffectGainMonsterKnowledgeExp* gainData = UGsTableUtil::FindRowById<UGsTableItemEffectGainMonsterKnowledgeExp, FGsSchemaItemEffectGainMonsterKnowledgeExp>(itemEffectGainId);

		if (nullptr == gainData)
			continue;

		_knowledgeGemIdToGainAmount.Emplace(itemIngredientPair.Key, gainData->gainAmount);
	}
}

void FGsCodexManager::Initialize()
{
	TArray<const FGsSchemaCodexMapGroup*> mapGroupList;
	TArray<const FGsSchemaCodexCategory*> categoryList;

	const UGsTableCodexMapGroup* mapGroupTable = Cast<UGsTableCodexMapGroup>(FGsSchemaCodexMapGroup::GetStaticTable());
	if (nullptr == mapGroupTable)
	{
		GSLOG(Error, TEXT("nullptr == mapGroupTable"));
		return;
	}
	mapGroupTable->GetAllRows(mapGroupList);

	const UGsTableCodexCategory* categoryTable = Cast<UGsTableCodexCategory>(FGsSchemaCodexCategory::GetStaticTable());
	if (nullptr == categoryTable)
	{
		GSLOG(Error, TEXT("nullptr == categoryTable"));
		return;
	}
	categoryTable->GetAllRows(categoryList);


	auto schemaFromRow = [=](auto InRow) {
		return InRow.GetRow();
		};

	for (const auto mapGroup : mapGroupList)
	{
		mapGroupIdNamePairList.Emplace(mapGroup->id, mapGroup->name);

		for (const auto& mapRow : mapGroup->mapList)
		{
			auto mapSchema = schemaFromRow(mapRow);
			if (nullptr == mapSchema)
				continue;

			auto mapId = mapSchema->id;
			if (mapToMapGroup.Contains(mapId))
			{
				GSLOG(Error, TEXT("Duplicate MapId Found!"));
				continue;
			}
			mapToMapGroup.Emplace(mapId, mapGroup->id);

			if (!mapGroupToMap.Contains(mapGroup->id))
				mapGroupToMap.Emplace(mapGroup->id);
			
			mapGroupToMap[mapGroup->id].Emplace(mapId);
		}

		auto balanceSchema = schemaFromRow(mapGroup->balanceId);
		if (nullptr == balanceSchema)
			continue;

		for (const auto& schemaLevelExp : balanceSchema->levelList)
		{
			if (!mapGroupToAccumulatedExpList.Contains(mapGroup->id))
				mapGroupToAccumulatedExpList.Emplace(mapGroup->id);
			mapGroupToAccumulatedExpList[mapGroup->id].Emplace(schemaLevelExp.exp);
		}

		int32 prevAccumulatedExp = 0;
		for (const auto accumulatedExp : mapGroupToAccumulatedExpList[mapGroup->id])
		{
			if (!mapGroupToExpList.Contains(mapGroup->id))
				mapGroupToExpList.Emplace(mapGroup->id);
			mapGroupToExpList[mapGroup->id].Emplace(accumulatedExp - prevAccumulatedExp);

			prevAccumulatedExp = accumulatedExp;
		}

		for (const auto& ingredientRow : mapGroup->ingredientItemList)
		{
			auto ingredientSchema = schemaFromRow(ingredientRow);
			if (nullptr == ingredientSchema)
				continue;

			auto ingredientId = ingredientSchema->id;
			if (!mapGroupToIngredients.Contains(mapGroup->id))
				mapGroupToIngredients.Emplace(mapGroup->id);
			mapGroupToIngredients[mapGroup->id].Emplace(ingredientId);
		}

		mapGroupToMapGroupName.Emplace(mapGroup->id, mapGroup->name);
		mapGroupIdImagePairList.Emplace(mapGroup->id, mapGroup->mapGroupImage);
		mapGroupToMapGroupImage.Emplace(mapGroup->id, mapGroup->mapGroupImage);
	}
	
	for (auto category : categoryList)
	{
		categoryIdNamePairList.Emplace(category->id, category->name);
		categoryToCategoryName.Emplace(category->id, category->name);

		for (const auto& mapGroupRow : category->mapGroupList)
		{
			auto mapGroupSchema = schemaFromRow(mapGroupRow);
			if (nullptr == mapGroupSchema)
				continue;

			mapGroupToCategory.Emplace(mapGroupSchema->id, category->id);
			if (!categoryToMapGroupList.Contains(category->id))
				categoryToMapGroupList.Emplace(category->id);
			categoryToMapGroupList[category->id].Emplace(mapGroupSchema->id);
		}

		auto balanceSchema = schemaFromRow(category->balanceId);
		if (nullptr == balanceSchema)
			continue;
		
		for (const auto& levelData : balanceSchema->levelList)
		{
			PassivityId passivityId = schemaFromRow(levelData.passivityId) ? schemaFromRow(levelData.passivityId)->id : INVALID_PASSIVITY_ID;

			if (!categoryToCategoryBalanceDataList.Contains(category->id))
				categoryToCategoryBalanceDataList.Emplace(category->id);

			categoryToCategoryBalanceDataList[category->id].Add({
				levelData.level,
				levelData.mapGroupLevelUpCount,
				passivityId,
				levelData.isGlobalPassivity
				});
		}

		// 카테고리 배경 이미지 탐색
		categoryToBG.Emplace(category->id, category->categoryBG);
		
		// categoryToCurLevel 의 경우, 일단 0 레벨로 채워주기는 해야 한다
		categoryToCurLevel.Emplace(category->id, 0);
	}


	// Id 순으로 정렬 (카테고리만. 맵 그룹은 정렬하지 않고 데이터 입력 순서대로 보여준다)
	for (auto& mapGroupingredientSetPair : mapGroupToIngredients)
		mapGroupingredientSetPair.Value.Sort(TLess<>());

	categoryToMapGroupList.KeySort(TLess<>());
	//for (auto& categoryMapGroupSetPair : categoryToMapGroupList)
	//	categoryMapGroupSetPair.Value.Sort(TLess<>());

	categoryToCategoryBalanceDataList.KeySort(TLess<>());

	if (UGsGlobalConstant* globalConstantData = LoadObject<UGsGlobalConstant>(nullptr, TEXT("/Game/Data/GlobalConstant/GlobalConstant.GlobalConstant")))
	{
		_codexKnowledgeThresholdForRedDot = globalConstantData->_codexKnowledgeThresholdForRedDot;
	}

	for (auto& categoryToMapGroupPair : categoryToMapGroupList)
	{
		CodexCategoryId  categoryId = categoryToMapGroupPair.Key;
		categoryToLevelToRequireLevelSum.Emplace(categoryId);
		for (int32 i = 0; i < categoryToCategoryBalanceDataList[categoryId].Num(); ++i)
		{
			if (!categoryToLevelToRequireLevelSum[categoryId].Contains(categoryToCategoryBalanceDataList[categoryId][i].level-1))
			{
				categoryToLevelToRequireLevelSum[categoryId].Emplace(categoryToCategoryBalanceDataList[categoryId][i].level-1);
				//TMap<CodexCategoryLevel, int32> levelToNextLevel;
				//levelToNextLevel.Emplace(categoryToCategoryBalanceDataList[categoryId][i].level, 0);
				//categoryToLevelToRequireLevelSum[categoryId].Emplace(levelToNextLevel);
			}
			categoryToLevelToRequireLevelSum[categoryId][categoryToCategoryBalanceDataList[categoryId][i].level-1] =
				categoryToCategoryBalanceDataList[categoryId][i].mapGroupLevelUpCount;
		}
		// 마지막 레벨의 경우에는, 절대 불가능하게 Max 값으로 설정
		categoryToLevelToRequireLevelSum[categoryId].Emplace(categoryToCategoryBalanceDataList[categoryId].Num());
		categoryToLevelToRequireLevelSum[categoryId][categoryToCategoryBalanceDataList[categoryId].Num()] = INT32_MAX;
	}

	categoryIdNamePairList.Sort([](const auto& A, const auto& B) {
		return A.Key < B.Key;
		});
	categoryToCategoryName.KeySort(TLess<>());

	for (auto& p : categoryToMapGroupList)
	{
		categoryToMap.Emplace(p.Key);
		for (auto& mapGroupId : p.Value)
		{
			categoryToMap[p.Key].Append(mapGroupToMap[mapGroupId]);
		}
	}

	BuildMapGroupToNpcList();
	BuildGemIdToKnowledGainAmount();
}


void FGsCodexManager::Finalize()
{

}

bool FGsCodexManager::CheckRedDot(bool bInUpdate /*= true*/)
{
	if (bInUpdate)
	{
		SetRedDot(CheckRedDotKnowledge() || CheckRedDotCategory());
	}
	
	return IsRedDot();
}

bool FGsCodexManager::CheckRedDotKnowledge()
{
	return _finalMonsterKnowledgeExp >= _codexKnowledgeThresholdForRedDot;
}

bool FGsCodexManager::CheckRedDotCategory()
{
	return GetRedDotCategoryList().Num() > 0;
}

TArray<CodexCategoryId> FGsCodexManager::GetRedDotCategoryList()
{
	TArray<CodexCategoryId> categoryList;

	for (const auto& categoryLevelPair : categoryToCurLevel)
	{
		CodexCategoryId categoryId = categoryLevelPair.Key;
		CodexCategoryLevel categoryLevel = categoryLevelPair.Value;

		if (!categoryToCategoryBalanceDataList.Contains(categoryId))
		{
			GSLOG(Error, TEXT("!categoryToCategoryBalanceDataList.Contains(%d)"), categoryId);
			continue;
		}

		int32 mapGroupLevelSum = 0;
		for (const auto& mapGroup : categoryToMapGroupList[categoryId])
		{
			if (!mapGroupToCurLevelExpPair.Contains(mapGroup))
				continue;

			mapGroupLevelSum += mapGroupToCurLevelExpPair[mapGroup].Key;
		}

		if (categoryId < 0 || categoryLevel < 0)
			return categoryList;

		if (!categoryToLevelToRequireLevelSum.Contains(categoryId))
			return categoryList;

		if (!categoryToLevelToRequireLevelSum[categoryId].Contains(categoryLevel))
			return categoryList;

		if (mapGroupLevelSum >= categoryToLevelToRequireLevelSum[categoryId][categoryLevel])
			categoryList.Emplace(categoryId);
	}

	return categoryList;
}

void FGsCodexManager::InvalidateExplorePassivityCollectionData()
{
	_explorePassivityCollectionData.Empty();

	MapId currentMapId = GLevel()->GetCurrentLevelId();

	for (const TPair<CodexCategoryId, const TArray<CategoryBalanceData>>& pair  : categoryToCategoryBalanceDataList)
	{
		CodexCategoryLevel currentLevel = GetCurrentCategoryLevelWithId(pair.Key);

		for (const CategoryBalanceData& balance : pair.Value)
		{
			if (currentLevel < balance.level)
				break;
			
			if (balance.isGlobal || categoryToMap[pair.Key].Contains(currentMapId))
				_explorePassivityCollectionData.Add(balance.passivityId);
		}
	}

	// 컬렉션 데이터 갱신 요청 (없을 경우 삭제됨)
	FGsPrimitiveInt32 param(static_cast<int32>(PassivityOwnerType::CODEX));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &param);
}

void FGsCodexManager::NotifyCodex(CodexCategoryId InId, CodexCategoryLevel InLevel, TArray<TTuple<int32, int32, int32>>&& InMapGroupToLevelExpPair)
{
	if (!categoryToCurLevel.Contains(InId))
		categoryToCurLevel.Emplace(InId, InLevel);
	else
		categoryToCurLevel[InId] = InLevel;

	for (const auto& tuple : InMapGroupToLevelExpPair)
		mapGroupToCurLevelExpPair.Emplace(tuple.Get<0>(), MakeTuple(tuple.Get<1>(), tuple.Get<2>()));

	CheckRedDot();
	InvalidateExplorePassivityCollectionData();
}

void FGsCodexManager::NotifyCodexMapGroupExpUpdate(int32 InMonsterKnowledgeExp, CodexMapGroupId InId, CodexMapGroupLevel InLevel, CodexMapGroupExp InExp)
{
	// 총 획득 지식 갱신
	_accumulateMonsterKnowledgeExp += InMonsterKnowledgeExp;

	// 변환 가능한 도감 포인트 값도 갱신해 주어야 함
	_finalMonsterKnowledgeExp += InMonsterKnowledgeExp;
	_finalMonsterBookPoint = _finalMonsterKnowledgeExp * 10;

	// 0 레벨일 경우에는, 캐싱된 데이터가 없을 수도 있다
	if (!mapGroupToCurLevelExpPair.Contains(InId))
		mapGroupToCurLevelExpPair.Emplace(InId);

	mapGroupToCurLevelExpPair[InId].Key = InLevel;
	mapGroupToCurLevelExpPair[InId].Value = InExp;

	FGsMessageCodexMapGroupExpUpdate paramData(InMonsterKnowledgeExp);
	GMessage()->GetMonsterKnowledgeCollection().SendMessage(MessageContentMonsterKnowledge::INVALIDATE_CODEX_MAP_GROUP_EXP_UPDATE, &paramData);

	// 사냥을 통해 몬스터 지식을 획득하여 레드닷이 뜨면, 메인 메뉴에 띄워줄 레드닷 UI 즉시 갱신
	if (CheckRedDot())
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MONSTER_COLLECTION, true);
}

void FGsCodexManager::AckCodex(CodexCategoryId InId, CodexCategoryLevel InLevel, TArray<TTuple<int32, int32, int32>>&& InMapGroupToLevelExpPair, MonsterKnowledgeExp InAccumulateExp, MonsterKnowledgeExp InFinalExp, Currency InFinalMonsterBookPoint)
{
	NotifyCodex(InId, InLevel, MoveTemp(InMapGroupToLevelExpPair));
	_accumulateMonsterKnowledgeExp = InAccumulateExp;
	_finalMonsterKnowledgeExp = InFinalExp;
	_finalMonsterBookPoint = InFinalMonsterBookPoint;

	GMessage()->GetMonsterKnowledgeCollection().SendMessage(MessageContentMonsterKnowledge::INVALIDATE_CODEX_MAP_GROUP_EXP_UPDATE, nullptr);
}

void FGsCodexManager::AckMonsterBookPointExchange(Currency InFinalMonsterBookPoint, MonsterKnowledgeExp InFinalMonsterKnowledgeExp)
{
	_finalMonsterBookPoint = InFinalMonsterBookPoint;
	_finalMonsterKnowledgeExp = InFinalMonsterKnowledgeExp;

	if (GGameData())
		GGameData()->SetCodexRedDot(false);

	GMessage()->GetMonsterKnowledgeCollection().SendMessage(MessageContentMonsterKnowledge::INVALIDATE_KNOWLEDGE_CONVERT,nullptr);
}

void FGsCodexManager::AckCodexCategoryLevelUp(CodexCategoryId InId, CodexCategoryLevel InLevel)
{
	if (!categoryToCurLevel.Contains(InId))
		categoryToCurLevel.Emplace(InId, InLevel);
	else
		categoryToCurLevel[InId] = InLevel;

	// C2URWQ-8317 | 티커 제목에 InLevel 값 대신, mapGroupLevelUpCount 가 출력되도록 수정
	TArray<CategoryBalanceData> CategoryBalanceDataList = GetCategoryBalanceDataListWithId(InId);
	int32 MapGroupLevelUpCount = 0;
	if (CategoryBalanceDataList.IsValidIndex(InLevel - 1))
		MapGroupLevelUpCount = CategoryBalanceDataList[InLevel - 1].mapGroupLevelUpCount;

	FGsMessageCodexCategoryLevelUp levelUpMessage(InId, InLevel, MapGroupLevelUpCount);

	GMessage()->GetMonsterKnowledgeCollection().SendMessage(MessageContentMonsterKnowledge::INVALIDATE_CATEGORY_LEVEL_UP, &levelUpMessage);

	InvalidateExplorePassivityCollectionData();
}

void FGsCodexManager::AckMonsterKnowledgeExpGainItemUse(CodexMapGroupId InId, CodexMapGroupLevel InLevel, CodexMapGroupExp InExp)
{
	// 0 레벨일 경우에는, 캐싱된 데이터가 없을 수도 있다
	if (!mapGroupToCurLevelExpPair.Contains(InId))
		mapGroupToCurLevelExpPair.Emplace(InId, MakeTuple(0, 0));

	// 총 지식 획득량도 즉시 갱신해주어야 함
	CodexMapGroupExp oldExp = mapGroupToCurLevelExpPair[InId].Value;
	_accumulateMonsterKnowledgeExp += (InExp - oldExp);

	mapGroupToCurLevelExpPair[InId].Key = InLevel;
	mapGroupToCurLevelExpPair[InId].Value = InExp;


	// 지식의 결정 사용시, 업데이트
	GMessage()->GetMonsterKnowledgeCollection().SendMessage(MessageContentMonsterKnowledge::INVALIDATE_CODEX_MAP_GROUP_GEM, nullptr);
}

MonsterKnowledgeExp FGsCodexManager::GetAccumulateMonsterKnowledgeExp() const
{
	return _accumulateMonsterKnowledgeExp;
}

MonsterKnowledgeExp FGsCodexManager::GetFinalMonsterKnowledgeExp() const
{
	return _finalMonsterKnowledgeExp;
}

CodexCategoryId FGsCodexManager::GetCategoryIdWithIndex(int32 InIndex) const
{
	if (!categoryIdNamePairList.IsValidIndex(InIndex))
		return INVALID_CODEX_CATEGORY_ID;

	return categoryIdNamePairList[InIndex].Key;
}

FText FGsCodexManager::GetCategoryNameWithId(CodexCategoryId InId) const
{
	if (!categoryToCategoryName.Contains(InId))
		return FText::GetEmpty();

	return categoryToCategoryName[InId];
}

TArray<CodexMapGroupId> FGsCodexManager::GetMapGroupListWithId(CodexCategoryId InId) const
{
	if (!categoryToMapGroupList.Contains(InId))
		return TArray<CodexMapGroupId>();

	return categoryToMapGroupList[InId];
}

TArray<CategoryBalanceData> FGsCodexManager::GetCategoryBalanceDataListWithId(CodexCategoryId InId) const
{
	if (!categoryToCategoryBalanceDataList.Contains(InId))
		return TArray<CategoryBalanceData>();

	return categoryToCategoryBalanceDataList[InId];
}

CodexCategoryLevel FGsCodexManager::GetCurrentCategoryLevelWithId(CodexCategoryId InId) const
{
	if (!categoryToCurLevel.Contains(InId))
		return 0;

	return categoryToCurLevel[InId];
}

CodexCategoryLevel FGsCodexManager::GetCurrentCategoryMapGroupLevelSumWithId(CodexCategoryId InId) const
{
	if (!categoryToMapGroupList.Contains(InId))
		return 0;

	TArray<CodexMapGroupId> mapGroupList = categoryToMapGroupList[InId];

	CodexCategoryLevel sumLevel = 0;
	for (auto mapGroup : mapGroupList)
	{
		if (!mapGroupToCurLevelExpPair.Contains(mapGroup))
			continue;

		sumLevel += mapGroupToCurLevelExpPair[mapGroup].Key;
	}

	return sumLevel;
}

CodexMapGroupId FGsCodexManager::GetMapGroupIdWithIndex(int32 InIndex) const
{
	if (!mapGroupIdNamePairList.IsValidIndex(InIndex))
		return INVALID_CODEX_MAP_GROUP_ID;

	return mapGroupIdNamePairList.IsValidIndex(InIndex);
}

bool FGsCodexManager::GetMapGroupIsMaxLevelWithId(CodexMapGroupId InId) const
{
	if (!mapGroupToCurLevelExpPair.Contains(InId))
		return false;

	if (!mapGroupToExpList.Contains(InId))
		return false;

	TPair<int32, int32> curLevelExpPair = mapGroupToCurLevelExpPair[InId];
	return (mapGroupToExpList[InId].Num() == curLevelExpPair.Key);
}

int32 FGsCodexManager::GetMapGroupCurrentLevelWithId(CodexMapGroupId InId) const
{
	if (!mapGroupToCurLevelExpPair.Contains(InId))
		return false;

	TPair<int32, int32> curLevelExpPair = mapGroupToCurLevelExpPair[InId];
	return curLevelExpPair.Key;
}

CodexMapGroupExp FGsCodexManager::GetMapGroupCurrentExpWithId(CodexMapGroupId InId) const
{
	if (!mapGroupToCurLevelExpPair.Contains(InId))
		return 0;

	TPair<int32, int32> curLevelExpPair = mapGroupToCurLevelExpPair[InId];
	return curLevelExpPair.Value;
}

FText FGsCodexManager::GetMapGroupNameWithId(CodexMapGroupId InId) const
{
	if (!mapGroupToMapGroupName.Contains(InId))
		return FText::GetEmpty();

	return mapGroupToMapGroupName[InId];
}

float FGsCodexManager::GetMapGroupCurrentExpPercentWithId(CodexMapGroupId InId) const
{
	if (!mapGroupToCurLevelExpPair.Contains(InId))
		return 0.f;

	if (!mapGroupToExpList.Contains(InId))
		return 0.f;
	
	TPair<int32, int32> curLevelExpPair = mapGroupToCurLevelExpPair[InId];

	// 현재 레벨이 맥스이면, 프로그레스바를 가득 채워준다
	if (curLevelExpPair.Key == mapGroupToExpList[InId].Num())
		return 1.f;

	// 모종의 이유로 목표 레벨이 없다면, 표시하지 말자
	if (!mapGroupToExpList[InId].IsValidIndex(curLevelExpPair.Key))
		return 0.f;

	// 현재 단계 도달을 위해 필요한 누적 경험치를 계산해 보자
	int32 accumulatedExpForCurLevel = 0;
	if (!mapGroupToAccumulatedExpList.Contains(InId))
		return 0.f;

	if (mapGroupToAccumulatedExpList[InId].IsValidIndex(curLevelExpPair.Key - 1))
		accumulatedExpForCurLevel = mapGroupToAccumulatedExpList[InId][curLevelExpPair.Key - 1];

	// (현재 경험치 - 현재 단계 도달을 위한 경험치) / 목표 레벨 단계 도달을 위한 경험치
	return (static_cast<float>(curLevelExpPair.Value - accumulatedExpForCurLevel) / static_cast<float>(mapGroupToExpList[InId][curLevelExpPair.Key]));
}

FSoftObjectPath FGsCodexManager::GetMapGroupImageWithId(CodexMapGroupId InId) const
{
	if (!mapGroupToMapGroupImage.Contains(InId))
		return FSoftObjectPath();

	return mapGroupToMapGroupImage[InId];
}

TArray<const FGsSchemaNpcData*> FGsCodexManager::GetNpcDataListWithIdAndGrade(CodexMapGroupId InId, CreatureRankType InGrade)
{
	if (!mapGroupToRankToNpcList.Contains(InId))
		return TArray<const FGsSchemaNpcData*>();

	if (!mapGroupToRankToNpcList[InId].Contains(InGrade))
		return TArray<const FGsSchemaNpcData*>();

	return mapGroupToRankToNpcList[InId][InGrade];
}

TArray<TPair<MapId, const FGsSchemaNpcData*>> FGsCodexManager::GetNpcDataListWithId(CodexMapGroupId InId)
{
	if (!mapGroupToMapNpcPairList.Contains(InId))
		return TArray<TPair<MapId, const FGsSchemaNpcData*>>();

	return mapGroupToMapNpcPairList[InId];
}

int32 FGsCodexManager::GetTotalCategoryCount()
{
	return categoryToMapGroupList.Num();
}

CodexMapGroupId FGsCodexManager::GetMapGroupIdWithMapId(int32 InMapId)
{
	if (!mapToMapGroup.Contains(InMapId))
		return INVALID_CODEX_MAP_GROUP_ID;

	return mapToMapGroup[InMapId];
}

CodexCategoryId FGsCodexManager::GetCategoryIdWithMapGroupId(CodexMapGroupId InMapGroupId)
{
	if (!mapGroupToCategory.Contains(InMapGroupId))
		return INVALID_CODEX_CATEGORY_ID;

	return mapGroupToCategory[InMapGroupId];
}

CodexCategoryId FGsCodexManager::GetCategoryIdWithMapId(int32 InMapId)
{
	return GetCategoryIdWithMapGroupId(GetMapGroupIdWithMapId(InMapId));
}

int32 FGsCodexManager::GetIndexWithCategoryId(CodexCategoryId InId)
{
	for (int32 i = 0; i < categoryIdNamePairList.Num(); ++i)
		if (categoryIdNamePairList[i].Key == InId)
			return i;

	return 0;
}

int32 FGsCodexManager::GetKnowledgeGainAmountWithItemId(ItemId InId)
{
	if (!_knowledgeGemIdToGainAmount.Contains(InId))
		return 0;

	return _knowledgeGemIdToGainAmount[InId];
}

TArray<ItemId> FGsCodexManager::GetKnowledgeGemListWithMapGroupId(CodexMapGroupId InId)
{
	if (!mapGroupToIngredients.Contains(InId))
		return TArray<ItemId>();

	return mapGroupToIngredients[InId];
}

TArray<int32> FGsCodexManager::GetKnowledgeRequirementListWithMapGroupId(CodexMapGroupId InId)
{
	if (!mapGroupToExpList.Contains(InId))
		return TArray<int32>();

	return mapGroupToExpList[InId];
}

TArray<int32> FGsCodexManager::GetKnowledgeAccumlatedRequirementListWithMapGroupId(CodexMapGroupId InId)
{
	if (!mapGroupToAccumulatedExpList.Contains(InId))
		return TArray<int32>();

	return mapGroupToAccumulatedExpList[InId];
}

TArray<int32> FGsCodexManager::GetAccumulateKnowledgeExpListWithMapGroupId(CodexMapGroupId InId)
{
	if (!mapGroupToAccumulatedExpList.Contains(InId))
		return TArray<int32>();

	return mapGroupToAccumulatedExpList[InId];
}

CodexMapGroupExp FGsCodexManager::GetAccumulateKnowledgeExpWithMapGroupIdAndLevel(CodexMapGroupId InId, int32 InLevel)
{
	if (!mapGroupToAccumulatedExpList.Contains(InId))
		return 0;

	// 0 레벨이면 0 을 리턴한다.
	if (!mapGroupToAccumulatedExpList[InId].IsValidIndex(InLevel - 1))
		return 0;
	
	return mapGroupToAccumulatedExpList[InId][InLevel - 1];
}

CodexMapGroupExp FGsCodexManager::GetMaxExpWithMapGroupId(CodexMapGroupId InId)
{
	if (!mapGroupToAccumulatedExpList.Contains(InId))
		return 0;

	return mapGroupToAccumulatedExpList[InId].Last();
}

const FGsCodexExplorePassivityCollection* FGsCodexManager::GetExplorePassivityCollectionData() const
{
	return &_explorePassivityCollectionData;
}

FSoftObjectPath FGsCodexManager::GetCategoryBGWithCategoryId(CodexCategoryId InId) const
{
	if (!categoryToBG.Contains(InId))
		return FSoftObjectPath();

	return categoryToBG[InId];
}

FText FGsCodexManager::GetPassivityTextWithCategoryIdAndLevel(CodexCategoryId InId, CodexCategoryLevel InLevel)
{
	FGsEffectTextManager* effectTextMgr = GSEffectText();
	if (nullptr == effectTextMgr)
	{
		GSLOG(Error, TEXT("nullptr == effectTextMgr"));
		return FText::GetEmpty();
	}

	TArray<CategoryBalanceData> categoryBalanceDataList = GetCategoryBalanceDataListWithId(InId);

	if (categoryBalanceDataList.Num() == 0)
		return FText::GetEmpty();

	if (!categoryBalanceDataList.IsValidIndex(InLevel - 1))
		return FText::GetEmpty();

	// 스탯 텍스트 파싱
	FGsEffectTextCollection effectCollection;
	effectTextMgr->GetEffectTextColectionPassivity(categoryBalanceDataList[InLevel - 1].passivityId, effectCollection);

	FString statDescString;
	for (TPair<StatType, int32> iter : effectCollection._statMap)
	{
		// 개행처리
		if (false == statDescString.IsEmpty())
			statDescString.Append(TEXT("\n"));
		FGsStatHelper::GetDesplayStatNameAndValue(iter.Key, iter.Value, statDescString);
	}

	return FText::FromString(statDescString);
}
