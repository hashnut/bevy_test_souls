// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdMonsterToMapData.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"

//territory editor
#include "DataSchema/Map/GsSchemaMapWarpSpot.h"
#include "UTIL/GsTableUtil.h"
#include "Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/MonsterDropItem/GsSchemaMonsterToMapData.h"
#include "DataSchema/MonsterDropItem/GsSchemaMonsterInMapData.h"
#include "DataSchema/MonsterDropItem/GsSchemaMonsterInRegionData.h"

#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Data/GsGlobalConstant.h"

FGsTerrEdMonsterToMapData::FGsTerrEdMonsterToMapData()
{
}

FGsTerrEdMonsterToMapData::~FGsTerrEdMonsterToMapData()
{
}

bool FGsTerrEdMonsterToMapData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce)
{
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	_npcTable = tableManager.GetTable(FGsSchemaNpcData::StaticStruct());
	_mapTable = tableManager.GetTable(FGsSchemaMapData::StaticStruct());
	_worldMapMonsterTable = tableManager.GetTable(FGsSchemaWorldMapMonsterData::StaticStruct());
	_monsterToMapTable = tableManager.GetTable(FGsSchemaMonsterToMapData::StaticStruct());

	//Get all monster list 
	// Save monster's spawnGroupId using monster's 'npcData id' and 'spawnLevel'
	TMap<int32, TMap<int32, TArray<FGsSchemaMonsterInMapData>>> npcLevelMapData;
	TryGetAllMonsterToMapData(npcLevelMapData);

	if (inSaveForce)
	{
		SaveMonsterToMapData(npcLevelMapData);
	}
	else
	{
		if (IsNeedToSaveMonsterToMapData(npcLevelMapData))
		{
			SaveMonsterToMapData(npcLevelMapData);
		}
	}

	return true;
}

bool FGsTerrEdMonsterToMapData::IsNeedToSaveMonsterToMapData(TMap<int32, TMap<int32, TArray<FGsSchemaMonsterInMapData>>>& inData)
{
	if (nullptr == _monsterToMapTable)
	{
		return false;
	}

	TArray<const FGsSchemaMonsterToMapData*> monsterToMapList;
	_monsterToMapTable->GetAllRows(monsterToMapList);

	// 스폰 정보를 저장할 <몬스터, 레벨> 페어의 총 갯수를 비교
	int32 monsterLevelPairLen = 0;
	for (const auto& monsterLevelMapPair : inData)
	{
		for (const auto& levelMapPair : monsterLevelMapPair.Value)
		{
			++monsterLevelPairLen;
		}
	}

	if (monsterToMapList.Num() != monsterLevelPairLen)
	{
		return true;
	}

	int32 num = 0;
	for (const FGsSchemaMonsterToMapData* data : monsterToMapList)
	{
		if (nullptr == data)
		{
			continue;
		}

		if (inData.Contains(data->Id) == false)
		{
			return true;
		}

		if (inData[data->Id].Contains(data->level) == false)
		{
			return true;
		}

		if (data->MonsterInMapDataList.Num() != inData[data->Id][data->level].Num())
		{
			return true;
		}

		num = data->MonsterInMapDataList.Num();
		int32 numRegion = 0;
		for (int32 i = 0; i < num; ++i)
		{
			numRegion = data->MonsterInMapDataList[i].MonsterInRegionDataList.Num();
			for (int32 j = 0; j < numRegion; ++j)
			{
				// 아래 코드로 한번에 비교를 하려고 하면, "피연산자로 'const FGsSchemaMonsterInRegionInData' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다" 라는 에러 메시지가 뜬다.
				// operator== 가 const 로 오버로딩 되어 있는데 왜 그러한 메시지가 뜨는지 이해 불가
				//if (data->MonsterInMapDataList[i].MonsterInRegionDataList[j] !=
				//	inData[data->CreatureId][i].MonsterInRegionDataList[j])
				//{
				//	return true;
				//}

				if (data->MonsterInMapDataList[i].MonsterInRegionDataList[j].SpawnGroupId !=
					inData[data->Id][data->level][i].MonsterInRegionDataList[j].SpawnGroupId ||
					data->MonsterInMapDataList[i].MonsterInRegionDataList[j].MonsterCount !=
					inData[data->Id][data->level][i].MonsterInRegionDataList[j].MonsterCount)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void FGsTerrEdMonsterToMapData::SaveMonsterToMapData(TMap<int32, TMap<int32, TArray<FGsSchemaMonsterInMapData>>>& inData)
{
	TArray<const FGsSchemaMonsterToMapData*> monsterToMapList;
	_monsterToMapTable->Empty();

	FGsSchemaMonsterToMapData temp;
	for (TPair<int32, TMap<int32, TArray<FGsSchemaMonsterInMapData>>>& npcLevelMapPair : inData)
	{
		int32 npcId = npcLevelMapPair.Key;

		for (TPair<int32, TArray<FGsSchemaMonsterInMapData>>& levelMapPair : npcLevelMapPair.Value)
		{
			int32 level = levelMapPair.Key;

			temp.RowName = FName(*FString::Format(TEXT("{0}_{1}"), { FString::FromInt(npcId), FString::FromInt(level) }));
			temp.Id = npcId;
			temp.level = level;

			if (const FGsSchemaNpcData* findNpc = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(npcId))
			{
				FString monsterName = (findNpc->nameText).ToString();

				// Primary Key 인 MonsterName 을 '몬스터 이름_레벨_Id' 로 설정
				temp.MonsterName = FName(*FString::Format(TEXT("{0}_{1}_{2}"), { *monsterName, FString::FromInt(level), FString::FromInt(npcId) }));
			}
			else
			{
				// npcData 테이블에 조회한 NPC 가 없다...
				UE_LOG(LogTemp, Error, TEXT("There is no npc with id : %d"), npcId);
				continue;
			}

			temp.MonsterInMapDataList = levelMapPair.Value;

			_monsterToMapTable->AddRow(temp);
		}
	}

	_monsterToMapTable->SaveDataTable();
}

bool FGsTerrEdMonsterToMapData::TryGetAllMonsterToMapData(OUT TMap<int32, TMap<int32, TArray<FGsSchemaMonsterInMapData>>>& outDataMap)
{
	TMap<int32, TMap<int32, TArray<FGsSchemaMonsterInMapData>>> allMonsterLevelToMap;

	/*
	* 1. WorldMapMonsterData 에서, 각 MapId 마다 존재하는 몬스터를 가지고, 각 몬스터 별로 등장하는 맵 리스트를 저장.
	* 2. CreatureId / level / MapId 페어로 해당하는 맵의 SpawnData_MapId 테이블에서 해당 몬스터의 상위 3개 SpawnData 배열 만들기
	*/

	TMap<int32, TMap<int32, TSet<int32>>> monsterLevelToMapId;

	TArray<const FGsSchemaWorldMapMonsterData*> worldMapMonsterDataArray;
	_worldMapMonsterTable->GetAllRows(worldMapMonsterDataArray);

	// WorldMapMonsterData 에서 맵별로 순회
	for (const FGsSchemaWorldMapMonsterData* worldMapMonsterData : worldMapMonsterDataArray)
	{
		int32 mapId = worldMapMonsterData->mapId;

		// 해당 맵이 PUBLIC FIELD 일 경우에만 탐색
		const FGsSchemaMapData* mapData;
		if (_mapTable->FindRow(FName(*FString::FromInt(mapId)), mapData))
		{
			// 월드맵 개선
			// 던전도 몬스터 위치 표시
			// https://jira.com2us.com/jira/browse/CHR-23756
			//if (MapType::PUBLIC_FIELD != mapData->mapType && MapType::INTER_FIELD != mapData->mapType)
			//{
			//	continue;
			//}
		}
		else
		{
			continue;
		}

		// 맵 별로, monsters 라는 리스트에 맵_아이디_레벨 형식으로 WorldMapMonsterElement 가 저장되어 있음
		for (const FGsSchemaWorldMapMonsterElementDataRow& elementRow : worldMapMonsterData->monsters)
		{
			const FGsSchemaWorldMapMonsterElementData* element = elementRow.GetRow();
			if (nullptr == element)
			{
				continue;
			}

			FGsSchemaNpcDataRow npcDataRow = element->id;
			const FGsSchemaNpcData* npcData = npcDataRow.GetRow();

			if (nullptr == npcData)
			{
				continue;
			}

			// 몬스터 타입만 체크한다
			if (IffFactionType::MONSTER == npcData->iffFactionType)
			{
				if (monsterLevelToMapId.Contains(npcData->id) == false)
				{
					monsterLevelToMapId.Add(npcData->id);
				}

				if (monsterLevelToMapId[npcData->id].Contains(element->level) == false)
				{
					monsterLevelToMapId[npcData->id].Add(element->level);
				}

				monsterLevelToMapId[npcData->id][element->level].Add(mapId);
			}
		}
	}

	// monsterLevelToMapId 를 가지고, 각 (CreatureId / level) 별로 실제 SpawnData 배열을 저장하자

	// CreatureId 별 순회
	for (const auto& monsterLevelToMapIdPair : monsterLevelToMapId)
	{
		int32 creatureId = monsterLevelToMapIdPair.Key;

		FGsSchemaMonsterInMapData monsterInMapData;

		// level 별 순회
		for (const auto& levelToMapIdPair : monsterLevelToMapIdPair.Value)
		{
			int32 level = levelToMapIdPair.Key;

			// mapId 별 순회
			for (const auto& mapId : levelToMapIdPair.Value)
			{
				// 맵에 존재하는 몬스터의 스폰 정보를 저장한다
				if (TryGetAllMonsterInMapData(creatureId, level, mapId, monsterInMapData))
				{
					if (allMonsterLevelToMap.Contains(creatureId) == false)
					{
						allMonsterLevelToMap.Add(creatureId);
					}

					if (allMonsterLevelToMap[creatureId].Contains(level) == false)
					{
						allMonsterLevelToMap[creatureId].Add(level);
					}

					allMonsterLevelToMap[creatureId][level].Add(monsterInMapData);
				}
			}
		}
	}

	outDataMap = allMonsterLevelToMap;
	return true;
}

bool FGsTerrEdMonsterToMapData::TryGetAllMonsterInMapData(int32 inCreatureId, int32 inLevel, int32 inMapId, OUT FGsSchemaMonsterInMapData& outMonsterInMapData)
{
	UGsTable* spawnTable = nullptr;
	FString levelKeyName;

	levelKeyName = FString::FromInt(inMapId);

	//skipp lobby data
	if (inMapId == LAUNCHER_LEVEL_KEY_MAP
		|| inMapId == BEGIN_LEVEL_KEY_MAP)
	{
		return false;
	}

	const FString& spawnTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(levelKeyName);
	spawnTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaSpawnData::StaticStruct(), spawnTableName);
	if (nullptr == spawnTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn table missing : %s"), *spawnTableName);
		return false;
	}

	// monsterInRegionList 에는 최대 상위 3개의 Region 만 저장되어야 한다
	TArray<FGsSchemaMonsterInRegionData> monsterInRegionList;
	TryGetAllMonsterInRegionDataList(inCreatureId, inLevel, inMapId, spawnTable, monsterInRegionList);
	
	// 만약 스폰 위치가 찍히지 않는다면, 이는 일반적으로 에러라고 생각해야 할 것이다.
	if (monsterInRegionList.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No Spawn Pos for NpcId : %d / MapId : %d / Level : %d"), inCreatureId, inMapId, inLevel);
	}


	outMonsterInMapData.MapId = inMapId;
	outMonsterInMapData.MonsterInRegionDataList = monsterInRegionList;

	return true;
}

bool FGsTerrEdMonsterToMapData::TryGetAllMonsterInRegionDataList(int32 inCreatureId, int32 inLevel, int32 inMapId, UGsTable* inSpawnDataTable, OUT TArray<FGsSchemaMonsterInRegionData>& outRegionDataArray)
{
	outRegionDataArray.Empty();
	if (nullptr == inSpawnDataTable)
	{
		return false;
	}

	TArray<FGsSchemaMonsterInRegionData> sortedRegionDataArray;

	TArray<const FGsSchemaSpawnData*> spawnList;
	inSpawnDataTable->GetAllRows(spawnList);

	const FGsSchemaNpcData* findNpc = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(inCreatureId);
	if (nullptr == findNpc)
	{
		UE_LOG(LogTemp, Error, TEXT("nullptr == findNpc, NpcId : %d"), inCreatureId);
		return false;
	}

	for (const FGsSchemaSpawnData* spawn : spawnList)
	{
		if (nullptr == spawn)
		{
			continue;
		}

		FGsSchemaMonsterInRegionData regionData;
		regionData.MonsterCount = 0;

		const TArray<FGsSchemaSpawnElement>& spawnInfoList = spawn->SpawnInfoList;
		for (const FGsSchemaSpawnElement& npc : spawnInfoList)
		{
			// CreatureId 와 Level 까지 같아야 함 -> 보스는 같지 않아도 되는 예외처리 추가
			if ( (inCreatureId == npc.CreatureId) && (findNpc->npcFunctionType == NpcFunctionType::FIELD_BOSS || inLevel == npc.spawnLevel))
			{
				regionData.MonsterCount += 1;
			}
		}

		if (regionData.MonsterCount > 0)
		{
			FGsSchemaSpawnDataRow spawnIdRow;
			spawnIdRow.DataTable = inSpawnDataTable->GetCachedDataTable();
			spawnIdRow.RowName = spawn->RowName;

			regionData.SpawnGroupId = spawnIdRow;

			sortedRegionDataArray.Add(regionData);
		}
	}

	// sortedRegionDataArray 를 MonsterCount 기준으로 정렬
	sortedRegionDataArray.Sort([](const FGsSchemaMonsterInRegionData& left, const FGsSchemaMonsterInRegionData& right) {
		//return (left.MonsterCount < right.MonsterCount);
		if (left.MonsterCount > right.MonsterCount)
		{
			return true;
		}
		else if (left.MonsterCount == right.MonsterCount)
		{
			if (nullptr != left.SpawnGroupId.GetRow() && nullptr != right.SpawnGroupId.GetRow())
			{
				return (left.SpawnGroupId.GetRow()->SpawnGroupId < right.SpawnGroupId.GetRow()->SpawnGroupId);
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
		});

	// 정렬 후, 보여줄 최대 Marker 갯수로 잘라 주기. 기본값은 3으로 설정
	// OutRegionDataArray 에 들어간 Region 들과 비교하여  X, Y 좌표 차이가 최소값 이상을 넘겨야 새로 삽입
	int iconCount = 3;
	float iconDistanceMin = 4000.f;

	if (UGsGlobalConstant* globalConstantData = LoadObject<UGsGlobalConstant>(nullptr, TEXT("/Game/Data/GlobalConstant/GlobalConstant.GlobalConstant")))
	{
		// 안정성을 위해, 만약 GlobalConstant 가 망가졌는지 체크(Serialize 과정에서 코드와 에셋 사이에 오차가 생겼을 수 있다).
		// 여기서 문제가 있다는 것은 클라이언트 빌드를 다시 해야 함을 의미한다.		
		if (!std::is_same<decltype(iconCount), decltype(globalConstantData->_monsterMapSpawnIconShowCount)>::value)
		{
			checkf(false, TEXT("globalConstantData->_monsterMapSpawnIconShowCount is not int! You have to rebuild client!"));
		}
		if (iconCount >= 100)
		{
			checkf(false, TEXT("globalConstantData->_monsterMapSpawnIconShowCount is spoiled! You have to rebuild client!"));
		}

		if (!std::is_same<decltype(iconDistanceMin), decltype(globalConstantData->MonsterSpawnDistCheckConfig)>::value)
		{
			checkf(false, TEXT("globalConstantData->MonsterSpawnDistCheckConfig is not float! You have to rebuild client!"));
		}
		if (iconDistanceMin == 0.f)
		{
			checkf(false, TEXT("globalConstantData->MonsterSpawnDistCheckConfig is spoiled! You have to rebuild client!"));
		}

		iconDistanceMin = globalConstantData->MonsterSpawnDistCheckConfig;
		iconCount = globalConstantData->_monsterMapSpawnIconShowCount;
	}

	// Icon 몇개 찍히는지 로그 보강
	//UE_LOG(LogTemp, Warning, TEXT("iconCount : %d"), iconCount);
	//UE_LOG(LogTemp, Warning, TEXT("iconDistanceMin : %f"), iconDistanceMin);

	for (const FGsSchemaMonsterInRegionData& regionData : sortedRegionDataArray)
	{
		if (0 == outRegionDataArray.Num())
		{
			outRegionDataArray.Add(regionData);
		}
		else if (iconCount <= outRegionDataArray.Num())
		{
			break;
		}
		else
		{
			float isFarFromIcons = true;

			const FGsSchemaSpawnData* regionSpawnData = regionData.SpawnGroupId.GetRow();

			for (const FGsSchemaMonsterInRegionData& outRegionData : outRegionDataArray)
			{
				const FGsSchemaSpawnData* outSpawnData = outRegionData.SpawnGroupId.GetRow();

				if (abs(regionSpawnData->Region.Center.X - outSpawnData->Region.Center.X) < iconDistanceMin &&
					abs(regionSpawnData->Region.Center.Y - outSpawnData->Region.Center.Y) < iconDistanceMin)
				{
					isFarFromIcons = false;
					break;
				}
			}

			if (isFarFromIcons)
			{
				outRegionDataArray.Add(regionData);
			}
		}
	}

	return true;
}