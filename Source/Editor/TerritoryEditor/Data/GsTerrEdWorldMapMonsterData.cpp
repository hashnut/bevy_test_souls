// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdWorldMapMonsterData.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"

//territory editor
#include "DataSchema/Map/GsSchemaMapWarpSpot.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsText.h"
#include "UTIL/GsItemUtil.h"
#include "Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterData.h"
#include "DataSchema/Map/WorldMap/GsSchemaRegionMapData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Drop/GsSchemaDropItemGroup.h"
#include "DataSchema/Drop/GsSchemaDropItemGroupInfo.h"
#include "DataSchema/Drop/GsSchemaDropData.h"
#include "DataSchema/Stat/GsSchemaNpcCorrectionStatData.h"
#include "Private/Helper/GsNotificatorOnDestruction.h"
#include "SNotificationList.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"
#include "DataSchema/WorldBoss/GsSchemaWorldBossData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapLevelData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsData.h"

FGsTerrEdWorldMapMonsterData::FGsTerrEdWorldMapMonsterData()
{
}

FGsTerrEdWorldMapMonsterData::~FGsTerrEdWorldMapMonsterData()
{
}

bool FGsTerrEdWorldMapMonsterData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce)
{
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	_npcTable = tableManager.GetTable(FGsSchemaNpcData::StaticStruct());
	_itemCommonTable = tableManager.GetTable(FGsSchemaItemCommon::StaticStruct());
	_mapTable = tableManager.GetTable(FGsSchemaMapData::StaticStruct());
	_regionTable = tableManager.GetTable(FGsSchemaRegionMapData::StaticStruct());
	_fieldBossTable = tableManager.GetTable(FGsSchemaFieldBossData::StaticStruct());
	_worldBossTable = tableManager.GetTable(FGsSchemaWorldBossData::StaticStruct());
	_dropTable = tableManager.GetTable(FGsSchemaDropData::StaticStruct());
	_invadeMapDataTable = tableManager.GetTable(FGsSchemaInvadeMapData::StaticStruct());
	_worldMapMonsterElementDataTable = tableManager.GetTable(FGsSchemaWorldMapMonsterElementData::StaticStruct());

	//worldmap
	_worldMapMonsterTable = tableManager.GetTable(FGsSchemaWorldMapMonsterData::StaticStruct());
	_worldMapLevelTable = tableManager.GetTable(FGsSchemaWorldMapLevelData::StaticStruct());
	_worldMapSpoilsTable = tableManager.GetTable(FGsSchemaWorldMapSpoilsData::StaticStruct());

	//Get all monster list 
	TMap<int32, TArray<FGsSchemaWorldMapMonsterElementData>> mapData;
	TryGetAllMonsterData(mapData);

	if (inSaveForce)
	{
		SaveWorldMapMonsterData(mapData);
		SaveWorldMapLevelData();
		SaveWorldMapSpoilsData();
	}
	else
	{
		if (IsNeedToSaveWorldMapMonsterData(mapData))
		{
			SaveWorldMapMonsterData(mapData);
			SaveWorldMapLevelData();
			SaveWorldMapSpoilsData();
		}
	}

	return true;
}

bool FGsTerrEdWorldMapMonsterData::IsNeedToSaveWorldMapMonsterData(const TMap<int32, TArray<FGsSchemaWorldMapMonsterElementData>>& inData)
{
	if (nullptr == _worldMapMonsterTable)
	{
		return false;
	}

	TArray<const FGsSchemaWorldMapMonsterData*> worldMapList;
	_worldMapMonsterTable->GetAllRows(worldMapList);

	if (worldMapList.Num() != inData.Num())
	{
		return true;
	}

	int32 num = 0;
	for (const FGsSchemaWorldMapMonsterData* data : worldMapList)
	{
		if (nullptr == data)
		{
			continue;
		}

		if (false == inData.Contains(data->mapId))
		{
			return true;
		}

		if (data->monsters.Num() != inData[data->mapId].Num())
		{
			return true;
		}

		num = data->monsters.Num();
		for (int32 i = 0; i < num; ++i)
		{
			if ((*data->monsters[i].GetRow()) != inData[data->mapId][i])
			{
				return true;
			}
		}
	}

	return false;
}

void FGsTerrEdWorldMapMonsterData::SaveWorldMapMonsterData(const TMap<int32, TArray<FGsSchemaWorldMapMonsterElementData>>& inData)
{
	if (nullptr == _worldMapMonsterTable)
	{
		return;
	}

	if (nullptr == _worldMapMonsterElementDataTable)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("nullptr == _worldMapMonsterElementDataTable"));
		return;
	}

	_worldMapMonsterElementDataTable->Empty();
	UGsDataTable* CachedWorldMapMonsterElementDataTable = _worldMapMonsterElementDataTable->GetCachedDataTable();
	if (nullptr == CachedWorldMapMonsterElementDataTable)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("nullptr == CachedWorldMapMonsterElementDataTable"));
		return;
	}

	_worldMapMonsterTable->Empty();

	for (const TPair<int32, TArray<FGsSchemaWorldMapMonsterElementData>>& pair : inData)
	{
		FGsSchemaWorldMapMonsterData temp;
		temp.RowName = *FString::FromInt(pair.Key);
		temp.mapId = pair.Key;

		TArray<FGsSchemaWorldMapMonsterElementDataRow> Monsters;
		for (const FGsSchemaWorldMapMonsterElementData& WorldMapMonsterElementData : pair.Value)
		{
			const FGsSchemaNpcData* NpcData = WorldMapMonsterElementData.id.GetRow();
			if (nullptr == NpcData)
			{
				UE_LOG(LogTerritoryEditor, Error, TEXT("nullptr == NpcData"));
				continue;
			}

			// RowName 체크
			const FName RowName = WorldMapMonsterElementData.RowName;
			if (CachedWorldMapMonsterElementDataTable->FindRowUnchecked(RowName))
			{
				UE_LOG(LogTerritoryEditor, Warning, TEXT("FGsSchemaWorldMapMonsterElementData 중복 RowName: %s"), *RowName.ToString());
				continue;
			}

			// WorldMapMonsterElementDataTable 에 추가
			CachedWorldMapMonsterElementDataTable->AddRow(RowName, WorldMapMonsterElementData);

			// FGsSchemaWorldMapMonsterData 로 전달할 목록 생성
			FGsSchemaWorldMapMonsterElementDataRow WorldMapMonsterElementDataRow;
			WorldMapMonsterElementDataRow.DataTable = CachedWorldMapMonsterElementDataTable;
			WorldMapMonsterElementDataRow.RowName = RowName;
			Monsters.Emplace(WorldMapMonsterElementDataRow);
		}
		temp.monsters = MoveTemp(Monsters);

		_worldMapMonsterTable->AddRow(temp);
	}

	_worldMapMonsterTable->SaveDataTable();
	_worldMapMonsterElementDataTable->SaveDataTable();
}

void FGsTerrEdWorldMapMonsterData::SaveWorldMapLevelData()
{
	if (nullptr == _worldMapLevelTable
		|| nullptr == _worldMapMonsterTable)
	{
		return;
	}

	TArray<const FGsSchemaWorldMapMonsterData*> worldMapMonsterArray;
	_worldMapMonsterTable->GetAllRows(worldMapMonsterArray);

	_worldMapLevelTable->Empty();

	for (const FGsSchemaWorldMapMonsterData* worldMapMonster : worldMapMonsterArray)
	{
		FGsSchemaWorldMapLevelData data;
		data.RowName = worldMapMonster->RowName;
		data.id = worldMapMonster->mapId;

		TArray<int32> levelArray;
		for (const FGsSchemaWorldMapMonsterElementDataRow& monsterRow : worldMapMonster->monsters)
		{
			const FGsSchemaWorldMapMonsterElementData* monster = monsterRow.GetRow();
			if (nullptr == monster)
			{
				continue;
			}

			if (levelArray.Contains(monster->level))
			{
				continue;
			}

			levelArray.Emplace(monster->level);
		}

		data.levelRange.Min = FMath::Min(levelArray);
		data.levelRange.Max = FMath::Max(levelArray);

		_worldMapLevelTable->AddRow(data);
	}

	_worldMapLevelTable->SaveDataTable();
}

void FGsTerrEdWorldMapMonsterData::SaveWorldMapSpoilsData()
{
	if (nullptr == _worldMapSpoilsTable
		|| nullptr == _worldMapMonsterTable)
	{
		return;
	}

	TArray<const FGsSchemaWorldMapMonsterData*> worldMapMonsterArray;
	_worldMapMonsterTable->GetAllRows(worldMapMonsterArray);

	_worldMapSpoilsTable->Empty();
	for (const FGsSchemaWorldMapMonsterData* worldMapMonster : worldMapMonsterArray)
	{
		FGsSchemaWorldMapSpoilsData spoilsData;
		spoilsData.RowName = worldMapMonster->RowName;
		spoilsData.mapId = worldMapMonster->mapId;

		TArray<FGsSchemaItemCommonRow> equipItemArray;
		//맵 벼롤 모든 아이템 정보 모은다		
		if (TryGetAllItemOnMap(ItemType::EQUIP, worldMapMonster, equipItemArray))
		{
			//각 아이템 별로 npc data를 정리한다. 
			for (FGsSchemaItemCommonRow& item : equipItemArray)
			{
				TArray<FGsSchemaWorldMapSpoilsMonsterLevel> monsterArray;
				TryGetAllMonsterRelatedItem(item, worldMapMonster, monsterArray);

				FGsSchemaWorldMapSpoilsItem spoilsItem;
				spoilsItem.item = item;
				spoilsItem.monsterLevelArray = monsterArray;

				spoilsData.equipSpoils.Add(spoilsItem);
			}
		}

		TArray<FGsSchemaItemCommonRow> etcItemArray;
		//맵 벼롤 모든 아이템 정보 모은다		
		if (TryGetAllItemOnMap(ItemType::ETC, worldMapMonster, etcItemArray))
		{
			//각 아이템 별로 npc data를 정리한다. 
			for (FGsSchemaItemCommonRow& item : etcItemArray)
			{
				TArray<FGsSchemaWorldMapSpoilsMonsterLevel> monsterArray;
				TryGetAllMonsterRelatedItem(item, worldMapMonster, monsterArray);

				FGsSchemaWorldMapSpoilsItem spoilsItem;
				spoilsItem.item = item;
				spoilsItem.monsterLevelArray = monsterArray;

				spoilsData.etcSpoils.Add(spoilsItem);
			}
		}

		_worldMapSpoilsTable->AddRow(spoilsData);
	}

	_worldMapSpoilsTable->SaveDataTable();
}

bool FGsTerrEdWorldMapMonsterData::TryGetAllMonsterData(OUT TMap<int32, TArray<FGsSchemaWorldMapMonsterElementData>>& outAllMonsterMap)
{
	TMap<int32, TArray<FGsSchemaWorldMapMonsterElementData>> allMonsterList;
	TArray<const FGsSchemaMapData*> mapList;
	if (false == TryGetMapData(mapList))
	{
		return false;
	}

	/*드랍 시스템 개선 반영하여 몬스터 아이템 정보를 얻는다.
	예를 들어 플레이어가 1001 맵에서 Npc id가 101 몬스터를 사냥하면
	1. dropType이 Map이 1001 인 맵에서 몬스터 레벨이 20인 조건을 검사해 드랍할 아이템 그룹을 얻는다.
	2. 101몬스터의 종족 타입이 HUMAN이라면 dropType이 Race이고 dropValue가 HUMAN level이 20인 아이템 그룹을 얻는다.
	따라서 이에 맞춰 WorldmapMonsterData를 수정하려면
	1. 모든 레벨 정보를 가져와 SpawnData를 얻은 후
	2. SpawnData에서 Npc id와 레벨 정보를 얻어 온다.
	3. Npc id 별로 레벨이 다르다면 다른 데이터로 처리
	4. 맵 정보에 맞춰 드랍 그룹 정보, Npc 종족에 맞춰 드랍 정보를 가져온다.
	5. 중복된 아이템 정보를 지우고 저장
	*/

	int32 levelKey = 0;
	UGsTable* spawnTable = nullptr;
	FString levelKeyName;
	TArray<FGsSchemaWorldMapMonsterElementData> monsterList;
	for (const FGsSchemaMapData* map : mapList)
	{
		if (nullptr == map)
		{
			continue;
		}

		levelKeyName = FString::FromInt(map->id);
		levelKey = map->id;

		//skipp lobby data
		if (levelKey == LAUNCHER_LEVEL_KEY
			|| levelKey == BEGIN_LEVEL_KEY)
		{
			continue;
		}

		const FString& spawnTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(levelKeyName);
		spawnTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaSpawnData::StaticStruct(), spawnTableName);
		if (nullptr == spawnTable)
		{
			UE_LOG(LogTemp, Error, TEXT("스폰 테이블 없음: %s"), *spawnTableName);
			continue;
		}

		//맵에 존재하는 드랍 아이템 정보 얻는다
		if (TryGetMonsterList(map, spawnTable, monsterList))
		{
			allMonsterList.Add(map->id, monsterList);
		}
	}

	outAllMonsterMap = allMonsterList;
	return true;
}

bool FGsTerrEdWorldMapMonsterData::TryGetMapData(OUT TArray<const FGsSchemaMapData*>& outMapArray)
{
	outMapArray.Empty();

	//미니맵 맵 데이터 얻어 온다. 	
	if (nullptr == _regionTable
		|| nullptr == _mapTable)
	{
		return false;
	}

	TArray<const FGsSchemaRegionMapData*> regionMapArray;
	_regionTable->GetAllRows(regionMapArray);

	TArray<const FGsSchemaMapData*> mapArray;
	_mapTable->GetAllRows(mapArray);

	const FGsSchemaMapData** find = nullptr;
	const FGsSchemaMapData* mapData = nullptr;
	for (const FGsSchemaRegionMapData* regionMapData : regionMapArray)
	{
		int32 id = regionMapData->id;
		find = mapArray.FindByPredicate([&](const FGsSchemaMapData* region)->bool {
			return region->id == id;
			});

		if (find && (*find))
		{
			mapData = (*find);
			outMapArray.Add(mapData);
		}
	}

	return true;
}

bool FGsTerrEdWorldMapMonsterData::TryGetMonsterList(const FGsSchemaMapData* inMapData, UGsTable* inSpawnDataTable, OUT TArray<FGsSchemaWorldMapMonsterElementData>& outWorldMapMonster)
{
	outWorldMapMonster.Empty();
	if (nullptr == inSpawnDataTable)
	{
		return false;
	}

	TArray<const FGsSchemaSpawnData*> spawnList;
	inSpawnDataTable->GetAllRows(spawnList);

	//일반 몬스터 추가, Npc id는 중복되지 않는다
	TMap<CreatureId, const FGsSchemaNpcData*> npcMap;
	const FGsSchemaNpcData* npcData = nullptr;
	const UGsTableNpcData* castNpcTable = Cast<UGsTableNpcData>(_npcTable);
	if (nullptr == castNpcTable)
	{
		return false;
	}

	for (const FGsSchemaSpawnData* spawn : spawnList)
	{
		if (nullptr == spawn)
		{
			continue;
		}

		const TArray<FGsSchemaSpawnElement>& spawnInfoList = spawn->SpawnInfoList;
		for (const FGsSchemaSpawnElement& npc : spawnInfoList)
		{
			if (CreatureSpawnType::NPC != npc.SpawnType)
			{
				continue;
			}

			castNpcTable->FindRowById(npc.CreatureId, npcData);
			if (nullptr == npcData)
			{
				UE_LOG(LogTerritoryEditor, Error, TEXT("NpcData 테이블에서 찾을 수 없음. CreatureId: %d"), npc.CreatureId);
				continue;
			}

			//npc iffFactionType이 몬스터이고
			//npc functionType 타입이 boss거나 몬스터
			//npc drop 정보가 default거나 Drop이면 등록
			if (npcData
				&& IffFactionType::MONSTER == npcData->iffFactionType
				&& (NpcFunctionType::NONE == npcData->npcFunctionType 
					|| NpcFunctionType::FIELD_BOSS == npcData->npcFunctionType || NpcFunctionType::WORLD_BOSS == npcData->npcFunctionType
					|| NpcFunctionType::MIMIC_NORMAL == npcData->npcFunctionType || NpcFunctionType::MIMIC_ELITE == npcData->npcFunctionType)
				&& CanItemDrop::NO_DROP != npcData->canItemDropOverride)
			{
				if (false == npcMap.Contains(npc.CreatureId))
				{
					npcMap.Emplace(npc.CreatureId, npcData);
				}
			}
		}
	}

	//find npc from field boss data	
	if (_fieldBossTable)
	{
		TArray<const FGsSchemaFieldBossData*> fieldBossDataArray;
		_fieldBossTable->GetAllRows(fieldBossDataArray);
		const FGsSchemaNpcData* bossNpc = nullptr;
		for (const FGsSchemaFieldBossData* fieldBoss : fieldBossDataArray)
		{
			if (fieldBoss->mapId.GetRow() != inMapData)
			{
				continue;
			}

			bossNpc = fieldBoss->bossNpcId.GetRow();
			if (nullptr == bossNpc)
			{
				UE_LOG(LogTerritoryEditor, Error, TEXT("필드 보스 없음. MapId: %d, fieldBoss->bossId: %d"), inMapData->id, fieldBoss->bossId);
				continue;
			}

			if (false == npcMap.Contains(bossNpc->id))
			{
				npcMap.Emplace(bossNpc->id, bossNpc);
			}
		}
	}

	//find npc from field boss data	
	if (_worldBossTable)
	{
		TArray<const FGsSchemaWorldBossData*> worldBossDataArray;
		_worldBossTable->GetAllRows(worldBossDataArray);
		const FGsSchemaNpcData* bossNpc = nullptr;
		for (const FGsSchemaWorldBossData* worldBoss : worldBossDataArray)
		{
			if (worldBoss->mapId.GetRow() != inMapData)
			{
				continue;
			}

			bossNpc = worldBoss->bossNpcId.GetRow();
			if (nullptr == bossNpc)
			{
				UE_LOG(LogTerritoryEditor, Error, TEXT("월드 보스 없음. MapId: %d, id: %d"), inMapData->id, worldBoss->id);
				continue;
			}

			if (false == npcMap.Contains(bossNpc->id))
			{
				npcMap.Emplace(bossNpc->id, bossNpc);
			}
		}
	}

	for (TPair<CreatureId, const FGsSchemaNpcData*> npcId : npcMap)
	{
		TArray<FGsSchemaWorldMapMonsterElementData> monsterElementArray;
		if (TryGetMonsterElement(monsterElementArray, inMapData, spawnList, npcId.Value))
		{
			for (const FGsSchemaWorldMapMonsterElementData& monsterElement : monsterElementArray)
			{
				outWorldMapMonster.Add(monsterElement);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("드랍 아이템 없음. MapId: %d, npcId.Key: %lu"), inMapData->id, npcId.Key);
		}
	}

	return true;
}

bool FGsTerrEdWorldMapMonsterData::TryGetMonsterElement(OUT TArray<FGsSchemaWorldMapMonsterElementData>& outData, const FGsSchemaMapData* inMapData, TArray<const FGsSchemaSpawnData*>& inSpawnList, const FGsSchemaNpcData* inNpcData)
{
	if (nullptr == inNpcData)
	{
		UE_LOG(LogTemp, Error, TEXT("Npc id is nullptr - map id : %d"), inMapData->id);
		return false;
	}

	bool isFindItem = false;
	if (NpcFunctionType::FIELD_BOSS == inNpcData->npcFunctionType
		|| NpcFunctionType::WORLD_BOSS == inNpcData->npcFunctionType)
	{
		isFindItem = TryGetBossMonsterDropItem(outData, inMapData, inSpawnList, inNpcData);
	}
	else
	{
		isFindItem = TryGetMonsterDropItem(outData, inMapData, inSpawnList, inNpcData);
	}

	return isFindItem;
}

bool FGsTerrEdWorldMapMonsterData::GetOffensive(TArray<const FGsSchemaSpawnData*>& inSpawnList, const FGsSchemaNpcData* inNpcData)
{
	if (nullptr == inNpcData)
	{
		return false;
	}

	//find npc
	bool findNpc = false;
	if (0 == inSpawnList.Num())
	{
		return false;
	}

	const FGsSchemaSpawnElement* find = nullptr;
	for (const FGsSchemaSpawnData* spawnData : inSpawnList)
	{		
		find = spawnData->SpawnInfoList.FindByPredicate([&](const FGsSchemaSpawnElement& spawnElement)->bool {
			return (spawnElement.SpawnType == CreatureSpawnType::NPC
				&& spawnElement.CreatureId == inNpcData->id);
			});
		if (find)
		{
			break;
		}
	}

	if (nullptr == find)
	{
		return false;
	}

	switch (find->offensiveType)
	{
	case OffensiveType::OFFENSIVE:
	{
		return true;
	}
	break;
	case OffensiveType::USE_NPC_DATA:
	{
		return inNpcData->isOffensive;
	}
	break;
	}

	return false;
}

bool FGsTerrEdWorldMapMonsterData::TryGetDropDataArray(const FGsSchemaMapData* inMapData, const FGsSchemaNpcData* inNpcId, int32 inLevel, OUT TArray<const FGsSchemaDropData*>& outDropData)
{
	/*드랍 시스템 개선 반영하여 몬스터 아이템 정보를 얻는다.
		예를 들어 플레이어가 1001 맵에서 Npc id가 101 몬스터를 사냥하면
		1. dropType이 Map이 1001 인 맵에서 몬스터 레벨이 20인 조건을 검사해 드랍할 아이템 그룹을 얻는다.
		2. 101몬스터의 종족 타입이 HUMAN이라면 dropType이 Race이고 dropValue가 HUMAN level이 20인 아이템 그룹을 얻는다.*/
	if (nullptr == inNpcId
		|| nullptr == inMapData)
	{
		return false;
	}

	TArray<const FGsSchemaDropData*> dropDataArray;
	Cast<UGsTableDropData>(_dropTable)->GetAllRows(dropDataArray);

	TArray<const FGsSchemaDropData*> matchedDropDataArray;
	bool isMatch = false;
	FString stringValue;
	for (const FGsSchemaDropData* data : dropDataArray)
	{
		isMatch = false;
		switch (data->dropType)
		{
		case DropCheckType::MAP:
		{
			stringValue = FString::FromInt(inMapData->id);
			isMatch = (data->dropValue.Equals(stringValue)) && (data->level == inLevel);
		}
		break;
		case DropCheckType::RANK:
		{
			stringValue = FGsTextUtil::GetStringFromEnum(TEXT("CreatureRankType"), inNpcId->grade);
			isMatch = (data->dropValue.Equals(stringValue)) && (data->level == inLevel);
		}
		break;
		case DropCheckType::RACE:
		{
			stringValue = FGsTextUtil::GetStringFromEnum(TEXT("CreatureRaceType"), inNpcId->race);
			isMatch = (data->dropValue.Equals(stringValue)) && (data->level == inLevel);
		}
		break;
		case DropCheckType::ELEMENT:
		{
			stringValue = FGsTextUtil::GetStringFromEnum(TEXT("MonsterElement"), inNpcId->element);
			isMatch = (data->dropValue.Equals(stringValue)) && (data->level == inLevel);
		}
		break;
		case DropCheckType::NPC_FUNCTION_TYPE:
		{
			stringValue = FGsTextUtil::GetStringFromEnum(TEXT("NpcFunctionType"), inNpcId->npcFunctionType);
			isMatch = (data->dropValue.Equals(stringValue)) && (data->level == inLevel);
		}
		break;
		// 신규 추가된 타입
		// https://jira.com2us.com/jira/browse/CHR-23598
		// https://jira.com2us.com/jira/browse/C2URWQ-7032
		case DropCheckType::CORRECTION:
		{
			// 1. 레벨 조건 같고
			if(data->level == inLevel)
			{ 
				// 2. 리스트 내의 id와 같다면
				for (auto iter: inNpcId->npcCorrectionList)
				{
					const FGsSchemaNpcCorrectionStatData* correctionStatData =  iter.GetRow();
					if (correctionStatData == nullptr)
					{
						continue;
					}

					stringValue = FString::FromInt(correctionStatData->id);
					if (data->dropValue.Equals(stringValue))
					{
						isMatch = true;
						break;
					}
				}
			}
		}
		break;
		default:
			break;
		}

		if (isMatch)
		{
			matchedDropDataArray.Add(data);
		}
	}

	outDropData = matchedDropDataArray;

	return matchedDropDataArray.Num() > 0;
}

bool FGsTerrEdWorldMapMonsterData::TryGetDropItemArray(const FGsSchemaMapData* inMapData, const FGsSchemaNpcData* inNpcData, TArray<const struct FGsSchemaSpawnData*>& inSpawnList, const TArray<int32>& inLevelArray, OUT TArray<ItemId>& outDropItemArray)
{
	int32 npcId = inNpcData->id;
	TArray<const FGsSchemaDropData*> dropArray;

	//level에 따른 drop data를 가져온다.
	for (int32 level : inLevelArray)
	{
		TArray<const FGsSchemaDropData*> tempArray;
		if (TryGetDropDataArray(inMapData, inNpcData, level, tempArray))
		{
			for (const FGsSchemaDropData* dropData : tempArray)
			{
				dropArray.Emplace(dropData);
			}
		}
	}

	TArray<ItemId> itemArray;
	const FGsSchemaDropItemGroup* itemGroup = nullptr;
	for (const FGsSchemaDropData* dropData : dropArray)
	{
		itemGroup = dropData->itemGroup.GetRow();
		if (nullptr == itemGroup)
		{
			continue;
		}

		for (const FGsSchemaDropItemGroupInfo& itemInfo : itemGroup->dropItemGroupList)
		{
			if (false == itemArray.Contains(itemInfo.itemId.GetRow()->id))
			{
				itemArray.Add(itemInfo.itemId.GetRow()->id);
			}
		}
	}

	outDropItemArray = itemArray;

	return true;
}

FName MakeFGsSchemaWorldMapMonsterElementDataRowName(const MapId InMapId, const CreatureId InNpcId, const Level InNpcLevel)
{
	return FName(*FString::Printf(TEXT("%d_%d_%d"), InMapId, InNpcId, InNpcLevel));
}

bool FGsTerrEdWorldMapMonsterData::TryGetMonsterDropItem(OUT TArray<FGsSchemaWorldMapMonsterElementData>& outDataArray, const struct FGsSchemaMapData* inMapData, TArray<const struct FGsSchemaSpawnData*>& inSpawnList, const FGsSchemaNpcData* inNpcData)
{
	outDataArray.Empty();
	FGsSchemaNpcDataRow npcIdRow;
	npcIdRow.DataTable = _npcTable->GetCachedDataTable();
	npcIdRow.RowName = inNpcData->RowName;

	TArray<int32> levelArray;
	if (false == TryGetLevelArray(inMapData, inNpcData, inSpawnList, levelArray))
	{
		return false;
	}

	for (int32 level : levelArray)
	{
		FGsSchemaWorldMapMonsterElementData element;
		element.RowName = MakeFGsSchemaWorldMapMonsterElementDataRowName(inMapData->id, inNpcData->id, level);
		element.id = npcIdRow;
		element.mapId = inMapData->id;
		element.isOffensive = GetOffensive(inSpawnList, inNpcData);
		element.rank = inNpcData->grade;
		element.level = level;

		//NPC와 관련된 모든 아이템을 가져온다. 
		TArray<ItemId> itemIdArray;
		TArray<int32> templevelArray;
		templevelArray.Add(level);
		TryGetDropItemArray(inMapData, inNpcData, inSpawnList, templevelArray, itemIdArray);
		//가져온 Item을 FGsSchemaWorldMapMonsterElementData에 정리하여 내보낸다.
		for (int32 i = 0; i < itemIdArray.Num(); ++i)
		{
			const FGsSchemaItemCommon* findItem = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(itemIdArray[i]);
			if (nullptr == findItem)
			{
				continue;
			}

			//일반 몬스터가 드랍하는 장착 아이템 
			if (ItemType::EQUIP == findItem->type)
			{
				//보스 드랍 아이템 중 BOSS_COLLECTION 은 기타 아이템으로 변경 요청옴
				//https://jira.com2us.com/jira/browse/CHR-20491
				if (findItem->categoryMain == ItemCategoryMain::COLLECTABLE)
				{
					FGsSchemaItemCommonRow* findItemCommon = element.etcItemList.FindByPredicate([&](FGsSchemaItemCommonRow& itemCommonRow)->bool {
						return itemCommonRow.RowName.IsEqual(findItem->RowName);
						});
					if (nullptr == findItemCommon)
					{
						FGsSchemaItemCommonRow itemRow;
						itemRow.RowName = findItem->RowName;
						itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
						element.etcItemList.Add(itemRow);
					}
				}
				else
				{
					FGsSchemaItemCommonRow* findItemCommon = element.equipItemList.FindByPredicate([&](FGsSchemaItemCommonRow& itemCommonRow)->bool {
						return itemCommonRow.RowName.IsEqual(findItem->RowName);
						});
					if (nullptr == findItemCommon)
					{						
						FGsSchemaItemCommonRow itemRow;
						itemRow.RowName = findItem->RowName;
						itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
						element.equipItemList.Add(itemRow);
					}
				}			
			}
			//일반 몬스터가 드랍하는 장착 아이템 
			else
			{
				FGsSchemaItemCommonRow* findItemCommon = element.etcItemList.FindByPredicate([&](FGsSchemaItemCommonRow& itemCommonRow)->bool {
					return itemCommonRow.RowName.IsEqual(findItem->RowName);
					});
				if (nullptr == findItemCommon)
				{
					FGsSchemaItemCommonRow itemRow;
					itemRow.RowName = findItem->RowName;
					itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
					element.etcItemList.Add(itemRow);
				}
			}
		}

		//인베이젼 맵이라면 
		if (IsInvadePossibleMap(inMapData->id))
		{
			element.isInvasion = true;

			TArray<const FGsSchemaItemCommon*> invasionEquipItem;
			TArray<const FGsSchemaItemCommon*> invasionEtcItem;
			if (TryGetInvadeDropItem(inNpcData, InvadeDropType::ATTACK, invasionEquipItem, invasionEtcItem))
			{
				for (const FGsSchemaItemCommon* item : invasionEquipItem)
				{
					FGsSchemaItemCommonRow itemRow;
					itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
					itemRow.RowName = item->RowName;

					element.invasionAttackEquipItemList.Add(itemRow);
				}

				for (const FGsSchemaItemCommon* item : invasionEtcItem)
				{
					FGsSchemaItemCommonRow itemRow;
					itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
					itemRow.RowName = item->RowName;

					element.invasionAttackEtcItemList.Add(itemRow);
				}
			}

			if (TryGetInvadeDropItem(inNpcData, InvadeDropType::DEFENSE, invasionEquipItem, invasionEtcItem))
			{
				for (const FGsSchemaItemCommon* item : invasionEquipItem)
				{
					FGsSchemaItemCommonRow itemRow;
					itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
					itemRow.RowName = item->RowName;

					element.invasionDefenceEquipItemList.Add(itemRow);
				}

				for (const FGsSchemaItemCommon* item : invasionEtcItem)
				{
					FGsSchemaItemCommonRow itemRow;
					itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
					itemRow.RowName = item->RowName;

					element.invasionDefenceEtcItemList.Add(itemRow);
				}
			}
		}
		else
		{
			element.isInvasion = false;
		}

		outDataArray.Add(element);
	}

	return outDataArray.Num() > 0;
}

bool FGsTerrEdWorldMapMonsterData::TryGetBossMonsterDropItem(OUT TArray<FGsSchemaWorldMapMonsterElementData>& outData, const struct FGsSchemaMapData* inMapData, TArray<const struct FGsSchemaSpawnData*>& inSpawnList, const FGsSchemaNpcData* inNpcData)
{
	if (nullptr == inNpcData)
	{
		const FString& log = FString::Printf(TEXT("worldmap monster - npc id missing - id : %d"), inNpcData->id);
		FGsNotificatorOnDestruction::ShowNotificationNow(FText::FromString(log)
			, SNotificationItem::ECompletionState::CS_Fail
			, nullptr
			, FGsNotificatorOnDestruction::eHyperLinkType::Select
			, 60);

		return false;
	}

	if (IffFactionType::MONSTER != inNpcData->iffFactionType)
	{
		return false;
	}

	if (nullptr == inNpcData->balanceId.GetRow())
	{
		const FString& log = FString::Printf(TEXT("worldmap monster - npc balance data missing - id : %d"), inNpcData->id);
		FGsNotificatorOnDestruction::ShowNotificationNow(FText::FromString(log)
			, SNotificationItem::ECompletionState::CS_Fail
			, nullptr
			, FGsNotificatorOnDestruction::eHyperLinkType::Select
			, 60);

		return false;
	}

	TArray<int32> levelArray;
	if (false == TryGetLevelArray(inMapData, inNpcData, inSpawnList, levelArray))
	{
		return false;
	}

	FGsSchemaNpcDataRow npcIdRow;
	npcIdRow.DataTable = _npcTable->GetCachedDataTable();
	npcIdRow.RowName = inNpcData->RowName;

	const UGsTableFieldBossData* fieldBossDataTable = Cast<UGsTableFieldBossData>(FGsSchemaFieldBossData::GetStaticTable());

	const TMap<FName, uint8*>* fieldBossDataRowMap = nullptr;
	if (fieldBossDataTable)
	{
		fieldBossDataTable->GetRowMap(fieldBossDataRowMap);
	}

	for (int32 level : levelArray)
	{
		FGsSchemaWorldMapMonsterElementData element;
		element.RowName = MakeFGsSchemaWorldMapMonsterElementDataRowName(inMapData->id, inNpcData->id, level);
		element.id = npcIdRow;
		element.mapId = inMapData->id;
		element.isOffensive = GetOffensive(inSpawnList, inNpcData);
		element.rank = inNpcData->grade;
		element.level = level;

		/**
		 * https://jira.com2us.com/jira/browse/C2URWQ-3822
		 * // 2023/4/12 PKT - JIRA 개선 의견 처리
		 * 이전 코드 주석
		 */

		 /*const FGsSchemaNpcBalanceLevel* findBalance = inNpcData->balanceId.GetRow()->levelList.FindByPredicate([=](const FGsSchemaNpcBalanceLevel& inBalance)->bool {
			 return inBalance.level == level;
			 });

		 if (nullptr == findBalance)
		 {
			 return false;
		 }*/

		 /*if (false == TryGetRewardData(element, findBalance))
		 {
			 return false;
		 }*/

		 /**
		  * https://jira.com2us.com/jira/browse/C2URWQ-3822
		  * // 2023/4/12 PKT - JIRA 개선 의견 처리
		  * 분기 설정 코드 추가
		  */
		bool isHaveRewardData = false;
		if (NpcFunctionType::FIELD_BOSS == inNpcData->npcFunctionType)
		{
			if (fieldBossDataTable)
			{
				if (fieldBossDataRowMap)
				{
					for (const auto& data : (*fieldBossDataRowMap))
					{
						const FGsSchemaFieldBossData* findfieldBossData = reinterpret_cast<const FGsSchemaFieldBossData*>(data.Value);
						if (findfieldBossData)
						{	// 2023/4/12 PKT - 찾는 Data가 있다면..
							const FGsSchemaNpcData* npcData = findfieldBossData->bossNpcId.GetRow();
							if (npcData && npcData->id == inNpcData->id)
							{
								isHaveRewardData = TryGetRewardFieldBossDataData(element, findfieldBossData);
								break;
							}
						}
					}
				}
			}
		}
		else if (NpcFunctionType::WORLD_BOSS == inNpcData->npcFunctionType)
		{
			TArray<const FGsSchemaWorldBossData*> worldBossDataArray;
			_worldBossTable->GetAllRows(worldBossDataArray);

			const FGsSchemaWorldBossData** find = worldBossDataArray.FindByPredicate([&](const FGsSchemaWorldBossData* boss)->bool {
				if(boss->bossNpcId.GetRow())
				{
					return boss->bossNpcId.GetRow()->id == inNpcData->id;
				}

				return false;
				});

			if (find)
			{
				isHaveRewardData = TryGetRewardWorldBossDataData(element, (*find));
			}

		}

		if (isHaveRewardData)
		{
			outData.Add(element);
		}
	}


	return outData.Num() > 0;
}

bool FGsTerrEdWorldMapMonsterData::TryGetRewardData(OUT FGsSchemaWorldMapMonsterElementData& outData, const FGsSchemaNpcBalanceLevel* inFindBalance)
{
	if (nullptr == _itemCommonTable)
	{
		return false;
	}

	const UGsTableRewardBoxData* rewardTable = Cast<UGsTableRewardBoxData>(FGsSchemaRewardBoxData::GetStaticTable());
	if (nullptr == rewardTable)
	{
		return false;
	}

	const FGsSchemaRewardBoxData* findBox = nullptr;
	const FGsSchemaItemCommon* findItem = nullptr;
	int32 itemId = 0;

	FGsSchemaItemCommonRow* findItemCommon = nullptr;
	const FGsSchemaRewardData* rewardData = inFindBalance->rewardId.GetRow();
	if (nullptr == rewardData)
	{
		return false;
	}

	const UGsTableItemCommon* itemCommon = Cast<const UGsTableItemCommon>(_itemCommonTable);
	if (nullptr == itemCommon)
	{
		return false;
	}

	for (const FGsSchemaRewardBox& box : rewardData->rewardBoxList)
	{
		findBox = box.rewardBoxDataRow.GetRow();
		if (nullptr == findBox)
		{
			continue;
		}

		findItem = nullptr;
		itemId = 0;
		int32 num = findBox->itemBagList.Num();
		for (int32 i = 0; i < num; ++i)
		{
			const FGsSchemaRewardItemBagData& itemBagData = findBox->itemBagList[i];
			itemId = itemBagData.itemId;
			itemCommon->FindRowById(itemId, findItem);
			if (nullptr == findItem)
			{
				continue;
			}

			if (findItem->type == ItemType::EQUIP)
			{
				findItemCommon = outData.equipItemList.FindByPredicate([&](FGsSchemaItemCommonRow& itemCommonRow)->bool {
					return itemCommonRow.RowName.IsEqual(findItem->RowName);
					});
				if (nullptr == findItemCommon)
				{
					FGsSchemaItemCommonRow itemRow;
					itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
					itemRow.RowName = findItem->RowName;
					outData.equipItemList.Add(itemRow);
				}
			}
			else
			{
				findItemCommon = outData.etcItemList.FindByPredicate([&](FGsSchemaItemCommonRow& itemCommonRow)->bool {
					return itemCommonRow.RowName.IsEqual(findItem->RowName);
					});
				if (nullptr == findItemCommon)
				{
					FGsSchemaItemCommonRow itemRow;
					itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
					itemRow.RowName = findItem->RowName;
					outData.etcItemList.Add(itemRow);
				}
			}
		}
	}

	return true;
}

bool FGsTerrEdWorldMapMonsterData::TryGetRewardFieldBossDataData(OUT FGsSchemaWorldMapMonsterElementData& outData, const FGsSchemaFieldBossData* inFieldBossData)
{
	if (nullptr == _itemCommonTable 
		|| nullptr == inFieldBossData 
		|| 0 >= inFieldBossData->bossRewardIdList.Num())
	{
		return false;
	}

	bool isHaveRewardData = false;
	for (int32 idx = 0; idx < inFieldBossData->bossRewardIdList.Num(); ++idx)
	{
		const FGsSchemaFieldBossRewardData* schemaFieldBossRewardData = inFieldBossData->bossRewardIdList[idx].GetRow();
		if (nullptr == schemaFieldBossRewardData)
		{
			continue;
		}

		const FGsSchemaRewardData* rewardData = schemaFieldBossRewardData->rewardId.GetRow();
		if (nullptr == rewardData)
		{
			continue;
		}

		for (const FGsSchemaRewardBox& box : rewardData->rewardBoxList)
		{
			const FGsSchemaRewardBoxData* findBox = box.rewardBoxDataRow.GetRow();
			if (nullptr == findBox)
			{
				continue;
			}

			for (const auto& itemBagData : findBox->itemBagList)
			{
				if (TryGetItemFromRewardBox(outData, findBox))
				{
					isHaveRewardData = true;
				}
			}
		}
	}

	return isHaveRewardData;
}

bool FGsTerrEdWorldMapMonsterData::TryGetRewardWorldBossDataData(OUT FGsSchemaWorldMapMonsterElementData& outData, const struct FGsSchemaWorldBossData* inBossData)
{
	if (nullptr == _itemCommonTable 
		|| nullptr == inBossData 
		|| (0 == inBossData->rankRewardIdList.Num() && 0 == inBossData->basicRewardIdList.Num() && 0 == inBossData->lastHitRewardIdList.Num()))
	{
		return false;
	}

	bool isHaveRewardData = false;

	for (int32 idx = 0; idx < inBossData->basicRewardIdList.Num(); ++idx)
	{
		const FGsSchemaRewardData* rewardData = inBossData->basicRewardIdList[idx].GetRow();
		if (nullptr == rewardData)
		{
			continue;
		}

		for (const FGsSchemaRewardBox& box : rewardData->rewardBoxList)
		{
			const FGsSchemaRewardBoxData* findBox = box.rewardBoxDataRow.GetRow();
			if (nullptr == findBox)
			{
				continue;
			}

			if (TryGetItemFromRewardBox(outData, findBox))
			{
				isHaveRewardData = true;
			}
		}
	}

	for (const auto& rankDataRow : inBossData->rankRewardIdList)
	{		
		if (const FGsSchemaRewardByRankData* rewardRankData = rankDataRow.GetRow())
		{
			if (const FGsSchemaRewardData* rewardData = rewardRankData->rewardDataRow.GetRow())
			{
				for (const auto& rewardBox : rewardData->rewardBoxList)
				{
					if (const FGsSchemaRewardBoxData* boxData = rewardBox.rewardBoxDataRow.GetRow())
					{
						if (TryGetItemFromRewardBox(outData, boxData))
						{
							isHaveRewardData = true;
						}
					}
				}
			}
		}
	}

	for (int32 idx = 0; idx < inBossData->lastHitRewardIdList.Num(); ++idx)
	{
		const FGsSchemaRewardData* rewardData = inBossData->lastHitRewardIdList[idx].GetRow();
		if (nullptr == rewardData)
		{
			continue;
		}

		for (const FGsSchemaRewardBox& box : rewardData->rewardBoxList)
		{
			const FGsSchemaRewardBoxData* findBox = box.rewardBoxDataRow.GetRow();
			if (nullptr == findBox)
			{
				continue;
			}

			if (TryGetItemFromRewardBox(outData, findBox))
			{
				isHaveRewardData = true;
			}
		}
	}

	return isHaveRewardData;
}

bool FGsTerrEdWorldMapMonsterData::TryGetItemFromRewardBox(OUT FGsSchemaWorldMapMonsterElementData& outData, const FGsSchemaRewardBoxData* inRewardBox)
{
	if (nullptr == inRewardBox)
	{
		return false;
	}
	
	const UGsTableItemCommon* itemCommon = Cast<const UGsTableItemCommon>(_itemCommonTable);
	if (nullptr == itemCommon)
	{
		return false;
	}

	bool isHaveRewardData = false;
	for (const auto& itemBagData : inRewardBox->itemBagList)
	{
		const FGsSchemaItemCommon* findItem = nullptr;
		if (false == itemCommon->FindRowById(itemBagData.itemId, findItem) || nullptr == findItem)
		{
			continue;
		}

		FGsSchemaItemCommonRow itemRow;
		itemRow.RowName = findItem->RowName;
		itemRow.DataTable = _itemCommonTable->GetCachedDataTable();
		if (findItem->type == ItemType::EQUIP)
		{
			//보스 드랍 아이템 중 BOSS_COLLECTION 은 기타 아이템으로 변경 요청옴
			//https://jira.com2us.com/jira/browse/CHR-20491
			if (findItem->categoryMain == ItemCategoryMain::COLLECTABLE)
			{
				FGsSchemaItemCommonRow* findItemCommon = outData.etcItemList.FindByPredicate([&](FGsSchemaItemCommonRow& itemCommonRow)->bool {
					return itemCommonRow.RowName.IsEqual(findItem->RowName);
					});
				if (nullptr == findItemCommon)
				{		
					outData.etcItemList.Add(itemRow);
					isHaveRewardData = true;
				}
			}
			else
			{
				FGsSchemaItemCommonRow* findItemCommon = outData.equipItemList.FindByPredicate([&](FGsSchemaItemCommonRow& itemCommonRow)->bool {
					return itemCommonRow.RowName.IsEqual(itemRow.RowName);
					});
				if (nullptr == findItemCommon)
				{
					outData.equipItemList.Add(itemRow);

					isHaveRewardData = true;
				}
			}
		}
		else
		{
			FGsSchemaItemCommonRow* findItemCommon = outData.etcItemList.FindByPredicate([&](FGsSchemaItemCommonRow& itemCommonRow)->bool {
				return itemCommonRow.RowName.IsEqual(itemRow.RowName);
				});
			if (nullptr == findItemCommon)
			{
				outData.etcItemList.Add(itemRow);

				isHaveRewardData = true;
			}
		}
	}

	return isHaveRewardData;
}

bool FGsTerrEdWorldMapMonsterData::IsInvadePossibleMap(int inMapId)
{
	const UGsTableInvadeMapData* table = Cast<UGsTableInvadeMapData>(_invadeMapDataTable);
	if (nullptr == table)
	{
		return false;
	}
	TArray<const FGsSchemaInvadeMapData*> arrayData;
	table->GetAllRows<FGsSchemaInvadeMapData>(arrayData);

	bool isInvadePossibleMap = false;

	for (const FGsSchemaInvadeMapData* iter : arrayData)
	{
		if (nullptr == iter)
		{
			continue;
		}
		if (nullptr == iter->mapId.GetRow())
		{
			continue;
		}
		const FGsSchemaMapData* mapData = iter->mapId.GetRow();
		if (mapData == nullptr)
		{
			continue;
		}
		if (mapData->id == inMapId)
		{
			isInvadePossibleMap = true;
			break;
		}
	}

	return isInvadePossibleMap;
}

bool FGsTerrEdWorldMapMonsterData::TryGetInvadeDropItem(const FGsSchemaNpcData* npcData, InvadeDropType inInvadeType, OUT TArray<const struct FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEtcItemArray)
{
	outEquipItemArray.Empty();
	outEtcItemArray.Empty();

	TArray<const FGsSchemaDropData*> dropDataArray;
	if (false == TryGetInvadeDropData(npcData, inInvadeType, dropDataArray))
	{
		return false;
	}

	return TryConvertDropToItemData(dropDataArray, outEquipItemArray, outEtcItemArray);
}

bool FGsTerrEdWorldMapMonsterData::TryGetInvadeDropData(const FGsSchemaNpcData* npcData, InvadeDropType inInvadeType, OUT TArray<const struct FGsSchemaDropData*>& outDropDataArray)
{
	//find server data
	const UGsTableDropData* dropDataTable = Cast<const UGsTableDropData>(_dropTable);
	if (nullptr == dropDataTable)
	{
		return false;
	}

	TArray<const FGsSchemaDropData*> dropDataArray;
	dropDataTable->GetAllRows(dropDataArray);

	TArray<const FGsSchemaDropData*> matchedDropDataArray;
	FString invadeText;
	bool isMatch = false;
	int32 level = npcData->fixedLevel;
	for (const FGsSchemaDropData* dropData : dropDataArray)
	{
		if (DropCheckType::INVADE != dropData->dropType)
		{
			continue;
		}

		if (level == dropData->level)
		{
			invadeText = FGsTextUtil::GetStringFromEnum(TEXT("InvadeDropType"), inInvadeType);
			isMatch = invadeText.Equals(dropData->dropValue);
			if (isMatch)
			{
				outDropDataArray.Add(dropData);
			}
		}
	}

	return outDropDataArray.Num() > 0;
}

bool FGsTerrEdWorldMapMonsterData::TryConvertDropToItemData(TArray<const struct FGsSchemaDropData*>& inDropDataArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEtcItemArray)
{
	int32 num = inDropDataArray.Num();
	TArray<const FGsSchemaItemCommon*> itemArray;
	for (const FGsSchemaDropData* dropData : inDropDataArray)
	{
		const TArray<FGsSchemaDropItemGroupInfo>& dropItemGroupList = dropData->itemGroup.GetRow()->dropItemGroupList;
		for (const FGsSchemaDropItemGroupInfo& itemInfo : dropItemGroupList)
		{
			if (false == itemArray.Contains(itemInfo.itemId.GetRow()))
			{
				itemArray.Add(itemInfo.itemId.GetRow());
			}
		}
	}

	num = itemArray.Num();
	const FGsSchemaItemCommon* findItem = nullptr;
	for (const FGsSchemaItemCommon* item : itemArray)
	{
		if (ItemType::EQUIP == item->type)
		{
			if (false == outEquipItemArray.Contains(item))
			{
				outEquipItemArray.Add(item);
			}
		}
		else
		{
			if (false == outEtcItemArray.Contains(item))
			{
				outEtcItemArray.Add(item);
			}
		}
	}

	return true;
}

bool FGsTerrEdWorldMapMonsterData::TryGetAllItemOnMap(ItemType itemType, const FGsSchemaWorldMapMonsterData* inMonsterData, OUT TArray<FGsSchemaItemCommonRow>& outItemArray)
{
	outItemArray.Empty();
	for (const FGsSchemaWorldMapMonsterElementDataRow& monsterRow : inMonsterData->monsters)
	{
		const FGsSchemaWorldMapMonsterElementData* monster = monsterRow.GetRow();
		if (nullptr == monster)
		{
			continue;
		}

		if (itemType == ItemType::EQUIP)
		{
			for (const FGsSchemaItemCommonRow& item : monster->equipItemList)
			{
				if (outItemArray.Contains(item))
				{
					continue;
				}

				if (nullptr == item.DataTable)
				{
					continue;
				}

				outItemArray.Add(item);
			}
		}
		else
		{
			for (const FGsSchemaItemCommonRow& item : monster->etcItemList)
			{
				if (outItemArray.Contains(item))
				{
					continue;
				}

				if (nullptr == item.DataTable)
				{
					continue;
				}

				outItemArray.Add(item);
			}
		}
	}

	outItemArray.Sort([](const FGsSchemaItemCommonRow& source, const FGsSchemaItemCommonRow& dest)->bool {
		if (nullptr == source.GetRow())
		{
			return false;
		}

		if (nullptr == dest.GetRow())
		{
			return true;
		}

		if (source.GetRow()->grade != dest.GetRow()->grade)
		{
			return (uint8)source.GetRow()->grade > (uint8)dest.GetRow()->grade;
		}

		// 아이템 TID
		return source.GetRow()->id > dest.GetRow()->id;
		});

	return outItemArray.Num() > 0;
}

bool FGsTerrEdWorldMapMonsterData::TryGetAllMonsterRelatedItem(const FGsSchemaItemCommonRow& inItem, const FGsSchemaWorldMapMonsterData* inMonsterData, OUT TArray<FGsSchemaWorldMapSpoilsMonsterLevel>& outNpcData)
{
	outNpcData.Empty();
	const FGsSchemaItemCommon* item = inItem.GetRow();
	if (nullptr == item)
	{
		return false;
	}

	if (ItemType::EQUIP == item->type
		&& item->categoryMain != ItemCategoryMain::COLLECTABLE)
	{
		for (const FGsSchemaWorldMapMonsterElementDataRow& monsterRow : inMonsterData->monsters)
		{
			const FGsSchemaWorldMapMonsterElementData* monster = monsterRow.GetRow();
			if (nullptr == monster)
			{
				continue;
			}

			if (monster->equipItemList.Contains(inItem))
			{
				const FGsSchemaWorldMapSpoilsMonsterLevel* find = outNpcData.FindByPredicate([&monster](const FGsSchemaWorldMapSpoilsMonsterLevel& data)->bool {
					return data.npcId == monster->id
						&& data.level == monster->level;
					});

				if (nullptr == find)
				{
					FGsSchemaWorldMapSpoilsMonsterLevel data;
					data.npcId = monster->id;
					data.level = monster->level;
					data.isOffensive = monster->isOffensive;

					outNpcData.Add(data);
				}
			}
		}
	}
	else
	{
		for (const FGsSchemaWorldMapMonsterElementDataRow& monsterRow : inMonsterData->monsters)
		{
			const FGsSchemaWorldMapMonsterElementData* monster = monsterRow.GetRow();
			if (nullptr == monster)
			{
				continue;
			}

			if (monster->etcItemList.Contains(inItem))
			{
				const FGsSchemaWorldMapSpoilsMonsterLevel* find = outNpcData.FindByPredicate([&monster](const FGsSchemaWorldMapSpoilsMonsterLevel& data)->bool {
					return data.npcId == monster->id
						&& data.level == monster->level;
					});

				if (nullptr == find)
				{
					FGsSchemaWorldMapSpoilsMonsterLevel data;
					data.npcId = monster->id;
					data.level = monster->level;
					data.isOffensive = monster->isOffensive;

					outNpcData.Add(data);
				}
			}
		}
	}

	outNpcData.Sort([&](const FGsSchemaWorldMapSpoilsMonsterLevel& source, const FGsSchemaWorldMapSpoilsMonsterLevel& dest)->bool {
		if (source.npcId.GetRow()->grade != dest.npcId.GetRow()->grade)
		{
			return (uint8)source.npcId.GetRow()->grade > (uint8)dest.npcId.GetRow()->grade;
		}

		if (source.level != dest.level)
		{
			return (uint8)source.level > (uint8)dest.level;
		}

		// 아이템 TID
		return source.npcId.GetRow()->id > dest.npcId.GetRow()->id;
		});

	return outNpcData.Num() > 0;
}

bool FGsTerrEdWorldMapMonsterData::TryGetLevelArray(const FGsSchemaMapData* inMapData, const FGsSchemaNpcData* inNpcData, TArray<const struct FGsSchemaSpawnData*>& inSpawnList, OUT TArray<int32>& outLevelArray)
{
	outLevelArray.Empty();
	if (inNpcData->fixedLevel > 0)
	{
		outLevelArray.Add(inNpcData->fixedLevel);
	}
	else
	{
		for (const FGsSchemaSpawnData* zone : inSpawnList)
		{
			for (const FGsSchemaSpawnElement& zoneChild : zone->SpawnInfoList)
			{
				if (zoneChild.CreatureId != inNpcData->id)
				{
					continue;
				}

				if (outLevelArray.Contains(zoneChild.spawnLevel))
				{
					continue;
				}

				outLevelArray.Emplace(zoneChild.spawnLevel);
			}
		}
	}

	outLevelArray.Sort([](int32 source, int32 dest) {
		return source < dest;
		});

	return outLevelArray.Num() > 0;
}