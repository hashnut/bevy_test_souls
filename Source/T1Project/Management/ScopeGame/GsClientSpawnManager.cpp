// Fill out your copyright notice in the Description page of Project Settings.

#include "GsClientSpawnManager.h"

#if CLIENTSPAWN_UNIQUE_ID_DEBUG
//c++
#include <iostream>
#include <bitset>
#include <string>

using namespace std;
#endif

//ue4
#include "Engine/Classes/GameFramework/PawnMovementComponent.h"
#include "Misc/PackageName.h"
#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"
#include "Templates/SharedPointer.h"
#include "DrawDebugHelpers.h"

//t1 project
#include "ActorEx/GsCharacterLocalPlayer.h"

#include "ActorComponentEx/GsQueryFloorHeightComponent.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"

#include "GameObject/Movement/GsMovementHandlerBase.h"

#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Define/GsGameObjectStruct.h"

#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "DataSchema/Map/Spawn/GsSchemaTerritoryNpc.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotData.h"
#include "DataSchema/NpcDialog/GsSchemaNpcDialogActorData.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsDir.h"
#include "UTIL/GsMove.h"
#include "UTIL/GsLevelUtil.h"

#include "Runtime/Public/GsTableManager.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"

#include "ActorEx/GsActorPropObject.h"
#include "ActorEx/GsCharacterBase.h"

#include "ControllerEx/GsPlayerController.h"

#include "ActorComponentEx/GsQueryFloorHeightComponent.h"

#include "Net/MessageHandler/GsNetMessageHandlerGameObject.h"
#include "Map/Spawn/GsSchemaClientSpawnData.h"
#include "T1Project.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Quest/Management/GsQuestActionManagement.h"

#include "Cheat/GsCheatManager.h"
#include "GameFramework/Character.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"

//bit 20
const int32 CLIENT_SPAWN_GAME_ID_MAX = 1048575;

FGsClientSpawnManager::~FGsClientSpawnManager()
{

}

void FGsClientSpawnManager::Initialize()
{
	Clear();

	if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
	{
		_cullingDistance = globalData->_clientSpawnDistance;
		_updateRate = globalData->_clientUpdateRate;
	}

	if (FGsMessageHolder* msg = GMessage())
	{
		_msgDelegates.Emplace(
			msg->GetQuest().AddRaw(MessageContentQuest::QUEST_MAIN_COMPLETED, this,
				&FGsClientSpawnManager::OnQuestMainCompleted)
		);
		_msgDelegates.Emplace(
			msg->GetQuest().AddRaw(MessageContentQuest::QUEST_MAIN_ACCEPTED, this,
				&FGsClientSpawnManager::OnQuestMainAccepted)
		);
	}
}

void FGsClientSpawnManager::Update(float inDelta)
{
	if (false == _active)
	{
		return;
	}

	// 대사 연출중에는 하지 않음
	// 시퀀스 연출중에는 막으면 안됨(뽑기 시퀀스 연출중 자동진행시 npc 스폰이 안됨)
	if (GSCameraMode()->GetIsCameraDialogPlay() == true)
	{
		return;
	}

	if (_currentTime > _updateRate)
	{
		_currentTime = 0;

		CullingGameObjectFromPlayer(false);
		UpdateSpawnList();
		UpdateDespawnList();

		UpdateZoneList();
	}

	DrawZone();

	_currentTime += inDelta;
}

void FGsClientSpawnManager::Finalize()
{
	Clear();

	// 바인딩 된 메시지 해제
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_msgDelegates.Num() != 0)
		{
			for (auto iter : _msgDelegates)
			{
				msg->GetQuest().Remove(iter);
			}
			_msgDelegates.Empty();
		}
	}
}

//void FGsClientSpawnManager::Shutdown()
//{
//	Clear();
//
//	//IGsManager::Shutdown();
//}

void FGsClientSpawnManager::InitializeClientSpawnGameObject(AGsCharacterLocalPlayer* inPlayer)
{
	Clear();
	
	_levelKey = GLevel()->GetCurrentLevelId();
	_playerCharacter = inPlayer;
	_world = inPlayer->GetWorld();

#if CLIENT_SPAWN_MANAGER_DEBUG && WITH_EDITOR
	GSLOG(Log, TEXT("FGsClientSpawnManager - InitializeClientSpawnGameObject - level : %d"), _levelKey);
#endif

	InitNpc(_levelKey);
	InitWarp(_levelKey);

	UGsLevelUtil::TryGetSpawnData(_levelKey, _allZoneArray);

	// 데이터 저장 완료후 interaction manager에게 알림
	TArray<TSharedPtr<FGsSpawnParam>> nonPlayers = GetAllClientSpawnParamList(EGsGameObjectType::NonPlayer);
	FGsClientSpawnListLoadFinishMessageParam param(nonPlayers);
	GMessage()->GetGameObject().SendMessage(
		MessageGameObject::CLIENT_SPAWN_LIST_LOAD_FINISH, &param);
}

void FGsClientSpawnManager::InitNpc(int32 inLevelKey)
{
	//스폰 정보
	TSharedPtr<FGsClientSpawnParamContainer> spawnDataList = MakeShareable(new FGsClientSpawnParamContainer());
	_spawnDataMap.Add(EGsGameObjectType::NonPlayer, spawnDataList);

	//거리에 따라 스폰할 액터
	TSharedPtr<FGsSpawnedGameObjContainer> spawnActorList = MakeShareable(new FGsSpawnedGameObjContainer());
	_spawnObjectMap.Add(EGsGameObjectType::NonPlayer, spawnActorList);

	//이벤트에 따라 스폰할 엑터
	TSharedPtr<FGsSpawnedGameObjContainer> eventSpawnActorList = MakeShareable(new FGsSpawnedGameObjContainer());
	_eventSpawnObjectMap.Add(EGsGameObjectType::NonPlayer, eventSpawnActorList);

	//read table
	TArray<const FGsSchemaClientSpawnData*> rowList = GetClientSpawnData(inLevelKey);
	FNavLocation navLoc;
	for (const FGsSchemaClientSpawnData* zone : rowList)
	{
		if (nullptr == zone)
		{
			continue;
		}

		for (const FGsSchemaSpawnElement& npc : zone->SpawnInfoList)
		{
			TSharedPtr<FGsSpawnParam> temp = MakeShareable(new FGsSpawnParam());
			temp->_type = EGsGameObjectType::NonPlayer;
			temp->_gameId = npc.ClientGameId;

			if(UGsGameObjectManager::TryFindNearestNavmesh(navLoc, _world.Get(), npc.Pos))
			{
				temp->_pos = navLoc.Location;
			}			
			else
			{
				temp->_pos = npc.Pos;
				GSLOG(Error, TEXT("Client spawn npc is not on navmesh - game id : %lld\ttid : %d"), npc.ClientGameId, npc.CreatureId);
			}

#if CLIENT_SPAWN_DEBUG
			DrawDebugSphere(_world.Get(), temp->_pos, 50, 24, FColor::Yellow, false, 300, 0, 0);
#endif

			temp->_rot = FGsDir::AngleToRotator(npc.Dir);
			temp->_tId = npc.CreatureId;
			temp->_async = false;
			spawnDataList->_paramList.Add(temp);
			_npcSpawnDataList.Add(npc);
		}
	}
}

void FGsClientSpawnManager::InitWarp(int32 inLevelKey)
{
	const FString levelKeyString = FString::FromInt(inLevelKey);

	//init container
	TSharedPtr<FGsClientSpawnParamContainer> container = MakeShareable(new FGsClientSpawnParamContainer());
	_spawnDataMap.Add(EGsGameObjectType::Warp, container);

	TSharedPtr<FGsSpawnedGameObjContainer> spawnedContainer = MakeShareable(new FGsSpawnedGameObjContainer());
	_spawnObjectMap.Add(EGsGameObjectType::Warp, spawnedContainer);

	//read warp data
	FString mapTerritoryTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelKeyString);
	const UGsTable* mapTerritoryTable = UGsTableManager::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), mapTerritoryTableName);
	if (nullptr == mapTerritoryTable)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* mapTerritoryData;
	mapTerritoryTable->FindRow<FGsSchemaMapTerritoryData>(*levelKeyString, mapTerritoryData);
	if (nullptr == mapTerritoryData)
	{
		return;
	}

	//read spot data
	FString spotTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(levelKeyString);
	const UGsTable* spotTable = UGsTableManager::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), spotTableName);
	if (nullptr == spotTable)
	{
		return;
	}

	const FGsSchemaSpotData* spotData;
	spotTable->FindRow<FGsSchemaSpotData>(*levelKeyString, spotData);
	if (nullptr == spotData)
	{
		return;
	}

	int32 sourceSpotIndex;
	for (int i = 0; i < mapTerritoryData->warpList.Num(); ++i)
	{
		sourceSpotIndex = mapTerritoryData->warpList[i].sourceSpot.SpotIndex;
		const FGsSchemaSpotInfo* find = spotData->SpotList.FindByPredicate([&](const FGsSchemaSpotInfo& spot)->bool
		{
			return sourceSpotIndex == spot.id;
		});

		if (nullptr == find)
		{
			continue;
		}

		TSharedPtr<FGsSpawnParam> temp = MakeShareable(new FGsSpawnParamWarp());
		temp->_type = EGsGameObjectType::Warp;
		temp->_gameId = mapTerritoryData->warpList[i].ClientGameId;
		temp->_pos = find->pos;
		temp->_tId = i;
		temp->_rot = FGsDir::AngleToRotator(find->dir);		
		temp->_async = false;
		container->_paramList.Add(temp);
	}
}

void FGsClientSpawnManager::Clear()
{
	DespawnAll();
	
	_spawnObjectMap.Empty();
	_eventSpawnObjectMap.Empty();

	for (TPair<EGsGameObjectType, TSharedPtr<FGsClientSpawnParamContainer>> pair : _spawnDataMap)
	{
		if (pair.Value.IsValid())
		{
			pair.Value->Clear();
		}
	}

	_spawnDataMap.Empty();	
	_npcSpawnDataList.Empty();
	_playerCharacter.Reset();

#if CLIENT_SPAWN_MANAGER_DEBUG && WITH_EDITOR
	GSLOG(Log, TEXT("FGsClientSpawnManager - Clear - levelKey : %d"), _levelKey);
#endif

	_arrayDialogCutGlobalDespawn.Empty();
	_arrayQuestShowTblId.Empty();
	_arrayQuestHideTblId.Empty();	
	_allZoneArray.Empty();
	_zoneArray.Empty();
}

void FGsClientSpawnManager::DespawnAll()
{
	for (TPair<EGsGameObjectType, TSharedPtr<FGsSpawnedGameObjContainer>>& pair : _spawnObjectMap)
	{
		if (pair.Value && pair.Value.IsValid())
		{
			pair.Value->Clear();
		}
	}

	_spawnList.Empty();
	_despawnList.Empty();

	for (TPair<EGsGameObjectType, TSharedPtr<FGsSpawnedGameObjContainer>>& pair : _eventSpawnObjectMap)
	{
		if (pair.Value && pair.Value.IsValid())
		{
			pair.Value->Clear();
		}
	}	
}

void FGsClientSpawnManager::CullingGameObjectFromPlayer(bool In_isForced, bool In_isCallByQuest)
{
	if (false == _playerCharacter.IsValid())
	{
		return;
	}

	const FVector& playerLocation = _playerCharacter->GetActorLocation();

	_spawnList.Empty();
	_despawnList.Empty();

	for (TPair<EGsGameObjectType, TSharedPtr<FGsClientSpawnParamContainer>> pair : _spawnDataMap)
	{
		if (false == pair.Value.IsValid())
		{
			continue;
		}

		for (TSharedPtr<FGsSpawnParam> param : pair.Value->_paramList)
		{	
			if (false == param.IsValid())
			{
				continue;
			}

			if ((param->_pos - playerLocation).Size() < _cullingDistance)
			{
				//If unique id is not contained in spawn map, it mean npc need to spawn
				if ((false == IsSpawned(param))
					&& IsShow(param) 					
					// 대사연출내에서 디스폰이면 스폰하면 안됨(다음 퀘스트 갱신까지는)
					// 대사연출내 디스폰시 대사연출끝나고 서버에서 quest id 갱신하기전
					// 다시 스폰시키는 이슈때문에 추가
					&& (IsDialogDespawned(param) == false))
				{
					// 시야내에서, 퀘스트갱신에 의한 호출이면 
					// 따로 저장했다가 스폰후 지운다
					// 더좋은 방법이 있으면 그때 바꾸자
					if (In_isCallByQuest == true)
					{
						if (_arrayQuestShowTblId.Contains(param->_tId) == false)
						{
							_arrayQuestShowTblId.Add(param->_tId);
						}
					}

					_spawnList.Add(param);
				}
				// 일반적인 업데이트에서는 필요없고
				// 강제 갱신할때만 하면 될듯
				if (In_isForced == true)
				{
					// 1. 시야내에 있고
					// 2. 스폰되어있는데
					// 3. 조건이 삭제면 디스폰
					if (true == IsSpawned(param) && IsShow(param, true) == false)
					{
						// 시야내에서, 퀘스트갱신에 의한 호출이면 
						// 따로 저장했다가 스폰후 지운다
						// 더좋은 방법이 있으면 그때 바꾸자
						if (In_isCallByQuest == true)
						{
							if (_arrayQuestHideTblId.Contains(param->_tId) == false)
							{
								_arrayQuestHideTblId.Add(param->_tId);
							}
						}
						_despawnList.Add(param);
					}
				}
			}
			else
			{
				//If unique id is contained in spawn map, it mean npc need to despawn
				if (IsSpawned(param))
				{
					_despawnList.Add(param);
				}
			}
		}
	}
}



void FGsClientSpawnManager::UpdateSpawnList()
{
	for (TSharedPtr<FGsSpawnParam> spawn : _spawnList)
	{
		if (false == spawn.IsValid())
		{
			continue;
		}

		SpawnGameObject(spawn);
		AddSpawnedGameObject(spawn);
	}
}

void FGsClientSpawnManager::UpdateDespawnList()
{
	for (TSharedPtr<FGsSpawnParam> despawn : _despawnList)
	{
		if (despawn.IsValid()
			&& IsSpawned(despawn))
		{
			DespawnGameObject(despawn);
		}

		RemoveSpawnedGameObject(despawn);
	}

	_despawnList.Empty();
}

void FGsClientSpawnManager::SpawnGameObject(TSharedPtr<FGsSpawnParam> inSpawnParam)
{
	switch (inSpawnParam->_type)
	{
	case EGsGameObjectType::NonPlayer:
	{
		UGsGameObjectBase* spawnNpc = FGsNetMessageHandlerGameObject::SpawnNpc(*inSpawnParam.Get(), SPAWN_REASON::SUMMON, true, false);				

#if CLIENT_SPAWN_MANAGER_DEBUG && WITH_EDITOR
		GSLOG(Log, TEXT("FGsClientSpawnManager - Spawn npc - level : %d \tnpc tid : %d"), _levelKey, inSpawnParam->_tId);
#endif
		break;
	}
	case EGsGameObjectType::Warp:
		SpawnWarp(inSpawnParam);
#if CLIENT_SPAWN_MANAGER_DEBUG && WITH_EDITOR
		GSLOG(Log, TEXT("FGsClientSpawnManager - Spawn warp - level : %d \tnpc tid : %d"), _levelKey, inSpawnParam->_tId);
#endif
		break;
	default:
		break;
	}
}


void FGsClientSpawnManager::DespawnGameObject(TSharedPtr<FGsSpawnParam> inParam)
{
	switch (inParam->_type)
	{
	case EGsGameObjectType::NonPlayer:
		FGsNetMessageHandlerGameObject::DespawnNpc(inParam->_gameId, SPAWN_REASON::NORMAL);
#if CLIENT_SPAWN_MANAGER_DEBUG && WITH_EDITOR
		GSLOG(Log, TEXT("FGsClientSpawnManager - Despawn npc - level : %d \tnpc tid : %d"), _levelKey, inParam->_tId);
#endif
		break;
	case EGsGameObjectType::Warp:

		if (UGsGameObjectBase* find = FindSpawnedGameObject(inParam))
		{
			GSGameObject()->DespawnObject(find);
#if CLIENT_SPAWN_MANAGER_DEBUG && WITH_EDITOR
			GSLOG(Log, TEXT("FGsClientSpawnManager - Despawn warp - level : %d \tnpc tid : %d"), _levelKey, inParam->_tId);
#endif
		}

		break;
	default:
		break;
	}
}

void FGsClientSpawnManager::SpawnWarp(TSharedPtr<FGsSpawnParam> inSpawnParam)
{
	if (false == inSpawnParam.IsValid())
	{
		return;
	}

	const FGsSchemaMapWarpInfo* warpData = TryGetWarp(GLevel()->GetCurrentLevelId(), inSpawnParam->_tId);
	if (nullptr == warpData)
	{
		return;
	}


	auto castParam = StaticCastSharedPtr<FGsSpawnParamWarp>(inSpawnParam);
	castParam->SetWarpTable(warpData);

	

	
	GSGameObject()->SpawnObject(inSpawnParam.Get());
}

void FGsClientSpawnManager::SpawnEventGameObject(TSharedPtr<FGsSpawnParam> inSpawnParam)
{
	SpawnGameObject(inSpawnParam);

	if (_eventSpawnObjectMap.Contains(inSpawnParam->_type))
	{
		TSharedPtr<FGsSpawnedGameObjContainer> container = _eventSpawnObjectMap[inSpawnParam->_type];
		if (container.IsValid())
		{
			container->_spawnMap.Add(inSpawnParam->_gameId);
		}
	}
}

void FGsClientSpawnManager::DespawnEventGameObject(TSharedPtr<FGsSpawnParam> inSpawnParam)
{
	DespawnGameObject(inSpawnParam);

	if (_eventSpawnObjectMap.Contains(inSpawnParam->_type))
	{
		TSharedPtr<FGsSpawnedGameObjContainer> container = _eventSpawnObjectMap[inSpawnParam->_type];
		if (container.IsValid())
		{
			container->_spawnMap.Remove(inSpawnParam->_gameId);
		}
	}
}

bool FGsClientSpawnManager::IsSpawned(UGsGameObjectBase* inGameObject)
{
	if (nullptr == inGameObject)
	{
		return false;
	}

	EGsGameObjectType type = inGameObject->GetObjectType();
	if (false == _spawnObjectMap.Contains(type))
	{
		return false;
	}

	TSharedPtr<FGsSpawnedGameObjContainer> container = _spawnObjectMap[type];
	if (false == container.IsValid())
	{
		return false;
	}

	return container->_spawnMap.Contains(inGameObject->GetGameId());
}

bool FGsClientSpawnManager::IsSpawned(TSharedPtr<FGsSpawnParam> inParam)
{
	if (false == _spawnObjectMap.Contains(inParam->_type))
	{
		return false;
	}

	TSharedPtr<FGsSpawnedGameObjContainer> container = _spawnObjectMap[inParam->_type];
	if (false == container.IsValid())
	{
		return false;
	}

	return container->_spawnMap.Contains(inParam->_gameId);
}

bool FGsClientSpawnManager::IsShow(TSharedPtr<FGsSpawnParam> inParam,
	bool In_isCheckCompleteNextQuest)
{
	if (inParam->_type == EGsGameObjectType::NonPlayer)
	{
		const FGsSchemaSpawnElement* find = FindSpawnData(inParam->_gameId);
		if (find)
		{
			return GSQuest()->IsVisible(find, In_isCheckCompleteNextQuest);
		}
	}	

	return true;	
}

// 대사연출내에서 디스폰 되었는지
bool FGsClientSpawnManager::IsDialogDespawned(TSharedPtr<FGsSpawnParam> inParam)
{
	if (inParam.IsValid() == false ||
		_arrayDialogCutGlobalDespawn.Num() == 0)
	{
		return false;
	}
	bool isDespawend = 
		_arrayDialogCutGlobalDespawn.Contains(inParam->_tId);
	return isDespawend;
}

const NpcFunctionType FGsClientSpawnManager::GetNpcFunctionTypeByServerSpawnID(const uint32 InServerSpawnID)
{
	const FGsSpawnParam* spawnData = FindNpcSpawnParamDataBySpawnID(InServerSpawnID);
	if (nullptr == spawnData)
	{
		return NpcFunctionType::NONE;
	}

	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>((uint32)spawnData->_gameId);
	if (nullptr == npcData)
	{
		GSLOG(Error, TEXT("FGsClientSpawnManager::GetNpcFunctionTypeByServerSpawnID is EmptyData - (uint32)spawnData->_gameId : %d"), (uint32)spawnData->_gameId);
		return NpcFunctionType::NONE;
	}

	return npcData->npcFunctionType;
}

const FGsSpawnParam* FGsClientSpawnManager::FindNpcSpawnParamDataBySpawnID(const uint32 InSpawnID)
{
	TArray<TSharedPtr<FGsSpawnParam>> allSpawnParamList = GetAllClientSpawnParamList(EGsGameObjectType::NonPlayer);
	for (uint8 i = 0; i < allSpawnParamList.Num(); ++i)
	{
		if (false == allSpawnParamList[i].IsValid())
			continue;

		FGsSpawnParam* spawnParamData = allSpawnParamList[i].Get();
		if (InSpawnID == (uint32)spawnParamData->_gameId)
		{
			return allSpawnParamList[i].Get();
		}
	}

	return nullptr;
}

TArray<TSharedPtr<FGsSpawnParam>> FGsClientSpawnManager::GetAllClientSpawnParamList(EGsGameObjectType inType)
{
	TArray<TSharedPtr<FGsSpawnParam>> empty;
	if (false == _spawnDataMap.Contains(inType))
	{
		return empty;
	}

	return _spawnDataMap[inType]->_paramList;
}

const FGsSchemaMapWarpInfo* FGsClientSpawnManager::TryGetWarp(int32 inLevelKey, int32 inIndex)
{
	FString levelKeyString = FString::FromInt(inLevelKey);
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelKeyString);
	const UGsTable* territoryMapTable = UGsTableManager::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == territoryMapTable)
	{
		return nullptr;
	}

	const FGsSchemaMapTerritoryData* mapTerritoryData;
	territoryMapTable->FindRow<FGsSchemaMapTerritoryData>(*levelKeyString, mapTerritoryData);
	if (nullptr == mapTerritoryData)
	{
		return nullptr;
	}

	if (false == mapTerritoryData->warpList.IsValidIndex(inIndex))
	{
		return nullptr;
	}

	return &mapTerritoryData->warpList[inIndex];

}

bool FGsClientSpawnManager::TryGetSpot(int32 inLevelKey, int64 inIndex, FGsSchemaSpotInfo& outSpot)
{
	FString levelKeyString = FString::FromInt(inLevelKey);
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(levelKeyString);
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return false;
	}

	const FGsSchemaSpotData* data;
	table->FindRow<FGsSchemaSpotData>(*levelKeyString, data);
	if (nullptr == data)
	{
		return false;
	}

	const FGsSchemaSpotInfo* spot = data->SpotList.FindByPredicate([&inIndex](const FGsSchemaSpotInfo& info)->bool {
		return (info.id == inIndex);
	});
	if (nullptr == spot)
	{
		return false;
	}

	outSpot = *spot;

	return true;
}

bool FGsClientSpawnManager::TryGetNpcSpawnData(FGsSchemaSpawnElement& outData, int32 inNpcID)
{
	const FGsSchemaSpawnElement* find = _npcSpawnDataList.FindByPredicate([&](const FGsSchemaSpawnElement& element)->bool {
		return element.CreatureId == inNpcID;
	});

	if (find)
	{
		outData = *find;
		return true;
	}

	return false;
}

bool FGsClientSpawnManager::TryGetNpcSpawnData(FGsSpawnParam& outSpawnParam, int32 inNpcID)
{
	FGsSchemaSpawnElement spawnElement;
	if (TryGetNpcSpawnData(spawnElement, inNpcID))
	{
		outSpawnParam = FGsSpawnParam(spawnElement);
		return true;
	}

	return false;
}
// 스폰용 데이터 가져오기
bool FGsClientSpawnManager::TryGetNpcSpawnParam(TSharedPtr<FGsSpawnParam>& Out_param, int32 In_npcId)
{
	TSharedPtr<FGsClientSpawnParamContainer> spawnDataList = _spawnDataMap.FindRef(EGsGameObjectType::NonPlayer);
	if (spawnDataList.IsValid() == false)
	{
		return false;
	}
	TSharedPtr<FGsSpawnParam>* findElement =
	spawnDataList->_paramList.FindByPredicate([In_npcId](const TSharedPtr<FGsSpawnParam> element)->bool
	{
		return element->_tId == In_npcId;
	});

	if (findElement == nullptr)
	{
		return false;
	}
	Out_param = *findElement;
	return true;
}

// 다음 맵을 가기위한 워프 정보 가져오기
bool FGsClientSpawnManager::TryGetNextMapWarp(int In_startLevelKey, int In_nextLevelKey, TArray<int>& In_exceptionMapIds,
	TArray<const FGsSchemaMapWarpInfo*>& Out_orderWarps)
{
	In_exceptionMapIds.Add(In_startLevelKey);

	FString startLevelKeyString = FString::FromInt(In_startLevelKey);
	FString startTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(startLevelKeyString);
	const UGsTable* territoryMapTable = UGsTableManager::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), startTableName);
	if (nullptr == territoryMapTable)
	{
		return false;
	}

	const FGsSchemaMapTerritoryData* mapTerritoryData;
	territoryMapTable->FindRow<FGsSchemaMapTerritoryData>(*startLevelKeyString, mapTerritoryData);
	if (nullptr == mapTerritoryData)
	{
		return false;
	}

	for (const FGsSchemaMapWarpInfo& iter : mapTerritoryData->warpList)
	{
		// 찾았다면
		if (iter.destSpot.MapId.GetRow()->mapId == In_nextLevelKey)
		{
			Out_orderWarps.Add(&iter);
			return true;
		}
	}

	// 못찾았으면 워프로 간맵안의 워프도 찾는다
	for (const FGsSchemaMapWarpInfo& iter : mapTerritoryData->warpList)
	{
		// 목적지가 시작한 맵인 곳에서는 찾지 않는다
		if (In_exceptionMapIds.Contains(iter.destSpot.MapId.GetRow()->mapId))
		{
			continue;
		}

		if (TryGetNextMapWarp(iter.destSpot.MapId.GetRow()->mapId, In_nextLevelKey, In_exceptionMapIds, Out_orderWarps) == true)
		{
			Out_orderWarps.Add(&iter);
			return true;
		}
	}
	return false;
}

// 다음 맵을 가기위한 워프의 위치 정보 가져오기
bool FGsClientSpawnManager::TryGetNextMapWarpPos(int In_startLevelKey, int In_nextLevelKey,
	FGsSchemaSpotInfo& Out_spotInfo, int& Out_warpId)
{
	TArray<int> exceptionMapIds;
	TArray<const FGsSchemaMapWarpInfo*> warpInfo;
	if (false == TryGetNextMapWarp(In_startLevelKey, In_nextLevelKey, exceptionMapIds, warpInfo) ||
		warpInfo.Num() == 0)
	{
		return false;
	}
	const FGsSchemaMapWarpInfo* lastInfo = warpInfo.Last();
	if (nullptr == lastInfo)
	{
		return false;
	}

	Out_warpId = lastInfo->warpId;
	if (false == TryGetSpot(lastInfo->sourceSpot.MapId.GetRow()->mapId, lastInfo->sourceSpot.SpotIndex, Out_spotInfo))
	{
		return false;
	}

	return true;
}


bool FGsClientSpawnManager::TryGetCurrentMapWarpPos(UWorld* inWorld, int32 inStartLevelKey, FVector& inSourcePos, FVector& inDestPos, TArray<FGsSchemaMapWarpInfo>& outWarpArray)
{
	outWarpArray.Empty();

	//네비 메쉬로 이동 가능한가?
	UNavigationPath* path = nullptr;
	path = UNavigationSystemV1::FindPathToLocationSynchronously(inWorld, inSourcePos, inDestPos);
	if (path)
	{
		//네비 메쉬로 이동 가능 
		return true;
	}

	//모든 워프 정보 가져온다
	FString startLevelKeyString = FString::FromInt(inStartLevelKey);
	FString territoryTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(startLevelKeyString);
	const UGsTable* territoryMapTable = UGsTableManager::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), territoryTableName);
	if (nullptr == territoryMapTable)
	{
		return false;
	}

	const FGsSchemaMapTerritoryData* mapTerritoryData;
	territoryMapTable->FindRow<FGsSchemaMapTerritoryData>(*startLevelKeyString, mapTerritoryData);
	if (nullptr == mapTerritoryData)
	{
		return false;
	}

	//워프 목적지를 판별하기 위해 모든 스팟 정보를 가져온다
	FString spotTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(startLevelKeyString);
	const UGsTable* spotTable = UGsTableManager::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), spotTableName);
	if (nullptr == spotTable)
	{
		return false;
	}

	TArray<const FGsSchemaSpotData*> spotArray;
	spotTable->GetAllRows(spotArray);
	TArray<FGsSchemaSpotInfo> spotInfoArray = spotArray[0]->SpotList;
	FGsSchemaSpotInfo* findWarpSpot = nullptr;
	FGsSchemaSpotInfo* findDestSpot = nullptr;
	TArray<FGsSchemaMapWarpInfo> visitedWarpArray;

	//모든 워프에서 네비 메쉬로 이동 가능한 워프를 얻어온다
	for (const FGsSchemaMapWarpInfo& iter : mapTerritoryData->warpList)
	{
		//같은 맵으로만 가는 워프 정보를 얻어온다
		if (iter.destSpot.MapId.GetRow()->mapId != inStartLevelKey)
		{
			continue;
		}		

		//워프 위치를 얻어온다
		findWarpSpot = spotInfoArray.FindByPredicate([&](FGsSchemaSpotInfo& spotData)->bool {
			return iter.sourceSpot.SpotIndex == spotData.id;
			});		
		if (nullptr == findWarpSpot)
		{
			continue;
		}		

		//Navmesh로 이동 가능한가?
		path = UNavigationSystemV1::FindPathToLocationSynchronously(inWorld, inSourcePos, findWarpSpot->pos);
		if (nullptr == path)
		{
			continue;
		}
		
		//워프의 목적지를 얻어온다. 
		findDestSpot = spotInfoArray.FindByPredicate([&](FGsSchemaSpotInfo& spotData)->bool {
			return iter.sourceSpot.SpotIndex == spotData.id;
			});
		if (nullptr == findDestSpot)
		{
			continue;
		}

		//목적지가 도착점인가 ?
		if (findDestSpot->pos == inDestPos)
		{
			outWarpArray.Add(iter);			
			return true;
		}

		visitedWarpArray.Push(iter);

		//도착지가 아니라면 워프의 목적지에서 다시 목적지로 이동 가능한지 확인
		if (TryGetCurrentMapWarpPosInternal(inWorld, mapTerritoryData->warpList, spotInfoArray, visitedWarpArray, findDestSpot->pos, inDestPos, outWarpArray))
		{
			visitedWarpArray.Empty();
			return true;
		}	

		visitedWarpArray.Pop();
	}

	visitedWarpArray.Empty();

	return false;
}

bool FGsClientSpawnManager::TryGetCurrentMapWarpPosInternal(UWorld* inWorld
	, const TArray<FGsSchemaMapWarpInfo>& inWarpArray, const TArray<FGsSchemaSpotInfo>& inSpotArray
	, TArray<FGsSchemaMapWarpInfo>& inVisitedWarpArray
	, FVector& inSourcePos, FVector& inDestPos
	, TArray<FGsSchemaMapWarpInfo>& outWarpArray)
{
	//시작점에서 목적지까지 Navmesh로 이동 가능한가?
	UNavigationPath* path = nullptr;
	path = UNavigationSystemV1::FindPathToLocationSynchronously(inWorld, inSourcePos, inDestPos);
	if (path || (inSourcePos == inDestPos))
	{
		outWarpArray = inVisitedWarpArray;

		//네비 메쉬로 이동 가능 
		return true;
	}

	//시작점에서 Navemesh로 갈 수 없어 워프로 이동 가능한지 확인 
	const FGsSchemaSpotInfo* findWarpSpot = nullptr;
	const FGsSchemaSpotInfo* findDestSpot = nullptr;
	const FGsSchemaMapWarpInfo* findWarp = nullptr;
	bool isVisitedGroup = false;

	//이미 방문한 워프를 제외한 워프 리스트에서 네비 메쉬로 이동 가능한 워프에서 검사한다
	for (const FGsSchemaMapWarpInfo& warp : inWarpArray)
	{
		//이미 방문한 워프 목록에 포함되어 있다면 제외한다
		findWarp = inVisitedWarpArray.FindByPredicate([&](FGsSchemaMapWarpInfo& visitedWarpData)->bool {
			return warp.warpId == visitedWarpData.warpId;
			});
		if (nullptr == findWarp)
		{
			continue;
		}

		if (warp.groupId != 0)
		{
			//이미 방문한 그룹 warp라면 제외 시킨다.
			isVisitedGroup = false;
			for (FGsSchemaMapWarpInfo& warpInfo : inVisitedWarpArray)
			{
				if (warpInfo.groupId == warp.groupId)
				{
					isVisitedGroup = true;
					break;
				}
			}

			if (isVisitedGroup)
			{
				continue;
			}
		}		
		
		//워프 위치를 가져온다
		findWarpSpot = inSpotArray.FindByPredicate([&](FGsSchemaSpotInfo& spotData)->bool {
			return warp.sourceSpot.SpotIndex == spotData.id;
			});
		if (nullptr == findWarpSpot)
		{
			continue;
		}

		//워프로 Navmesh 안에서 이동 가능한가?
		path = UNavigationSystemV1::FindPathToLocationSynchronously(inWorld, inSourcePos, findWarpSpot->pos);
		if (nullptr == path)
		{
			continue;
		}

		findDestSpot = inSpotArray.FindByPredicate([&](FGsSchemaSpotInfo& spotData)->bool {
			return warp.destSpot.SpotIndex == spotData.id;
			});
		if (nullptr == findDestSpot)
		{
			continue;
		}

		inVisitedWarpArray.Push(warp);

		if (TryGetCurrentMapWarpPosInternal(inWorld, inWarpArray, inSpotArray, inVisitedWarpArray, inSourcePos, inDestPos, outWarpArray))
		{
			return true;
		}

		inVisitedWarpArray.Pop();
	}

	return false;
}

AActor* FGsClientSpawnManager::GetPlayer()
{
	return _playerCharacter.Get();
}

uint32 FGsClientSpawnManager::GetUniqueID(int64 inClientGameID)
{
	return (uint32)inClientGameID;
}

void FGsClientSpawnManager::SetActive(bool inActive)
{
	_active = inActive;
}

UGsGameObjectBase* FGsClientSpawnManager::FindSpawnedGameObject(TSharedPtr<FGsSpawnParam> inParam)
{
	TSharedPtr<FGsSpawnedGameObjContainer> container = _spawnObjectMap[inParam->_type];
	if (false == container.IsValid())
	{
		return nullptr;
	}

	if (false == container->_spawnMap.Contains(inParam->_gameId))
	{
		return nullptr;
	}

	return GSGameObject()->FindObject(inParam->_type, inParam->_gameId);
}

void FGsClientSpawnManager::AddSpawnedGameObject(TSharedPtr<FGsSpawnParam> inParam)
{
	TSharedPtr<FGsSpawnedGameObjContainer> container = _spawnObjectMap[inParam->_type];
	if (false == container.IsValid())
	{
		return;
	}

	container->_spawnMap.Add(inParam->_gameId);
}

void FGsClientSpawnManager::RemoveSpawnedGameObject(TSharedPtr<FGsSpawnParam> inParam)
{
	TSharedPtr<FGsSpawnedGameObjContainer> container = _spawnObjectMap[inParam->_type];
	if (false == container.IsValid())
	{
		return;
	}

	container->_spawnMap.Remove(inParam->_gameId);
}

void FGsClientSpawnManager::UpdateZoneList()
{
	if (false == UGsCheatManager::_isShowZoneRange)
	{
		return;
	}

	if (false == _playerCharacter.IsValid())
	{
		return;
	}

	const FVector& playerLocation = _playerCharacter->GetActorLocation();
	_zoneArray.Empty();
	for (const FGsSchemaSpawnData* spawn : _allZoneArray)
	{
		if ((spawn->Region.Center - playerLocation).Size() < _cullingDistance)
		{
			_zoneArray.Add(spawn);
		}
	}
}

void FGsClientSpawnManager::DrawZone()
{
	if (false == UGsCheatManager::_isShowZoneRange)
	{
		return;
	}

	if (false == _playerCharacter.IsValid())
	{
		return;
	}

	if (false == _world.IsValid())
	{
		return;
	}
	
	for (const FGsSchemaSpawnData* zone : _zoneArray)
	{
		DrawDebugCircle(_world.Get(), zone->Region.Center, zone->Region.Radius, 32, FColor::Yellow, false, 0.02f, 0, 1, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
		DrawDebugString(_world.Get(), zone->Region.Center
			, FString::Printf(TEXT("Zone Group Index : %d"), zone->SpawnGroupId)
			, nullptr
			, FColor::White
			, 0.02f
			, false);
	}	
}

// npc game id 찾기 (spawn id로)
bool FGsClientSpawnManager::FindNpcGameIdBySpawnId(SpawnId In_spawnId, int64 In_gameId, int64& Out_gameId)
{
	Out_gameId = In_gameId;
	if (0 < In_spawnId)
	{
		TArray<TSharedPtr<FGsSpawnParam>> spawnList = GetAllClientSpawnParamList(EGsGameObjectType::NonPlayer);
		if (spawnList.Num() <= 0)
			return false;

		for (TSharedPtr<FGsSpawnParam> iter : spawnList)
		{
			if (GetUniqueID(iter->_gameId) != In_spawnId)
				continue;

			Out_gameId = iter->_gameId;
			break;
		}
	}
	return true;
}

bool FGsClientSpawnManager::FindNpcByNpcTableId(int32 inTableId, OUT int64& Out_gameId)
{
	TArray<TSharedPtr<FGsSpawnParam>> spawnList = GetAllClientSpawnParamList(EGsGameObjectType::NonPlayer);
	for (TSharedPtr<FGsSpawnParam> iter : spawnList)
	{
		if (iter->_tId == inTableId)
		{
			Out_gameId = iter->_gameId;
			break;
		}				
	}

	return true;
}

bool FGsClientSpawnManager::TryGenerateClientGameIDInRuntime(OUT int64& outUniqueID, int32 inInstanceID)
{
	//Territory tool에서도 사용하므로 반드시 확인하고 Object type 할당할 것	
	int64 objType = CLIENT_SPAWN_OBJECT_MAX << 20;

	//클라이언트 spawn id는 음수이고
	//최하위 20 비트가 instance 
	//그다음 8비트가 Object type이다. 
	if (inInstanceID > CLIENT_SPAWN_INSTANCE_MAX)
	{
		return false;
	}

	int64 uniqueID = (objType | inInstanceID);
	int64 clientSpawnUniqueID = CLIENT_PACKET | uniqueID;
	outUniqueID = clientSpawnUniqueID;

	return true;
}

bool FGsClientSpawnManager::IsClientSpawnGameId(int64 In_gameId)
{
	return CLIENT_PACKET & In_gameId;
}

// 강제로 업데이트
// 이슈: 퀘스트 워프 후 바로 대화 시도시 
// npc가 없을 수도 있어서 
// local spawn complete 시점에 스폰 처리 1회 먼저 처리
// In_isCallByQuest: 퀘스트에 의한 호출인지(스폰/디스폰 이펙트 때문에)
void FGsClientSpawnManager::ForcedUpdate(bool In_isCallByQuest)
{
	if (false == _active)
	{
		return;
	}

	CullingGameObjectFromPlayer(true, In_isCallByQuest);
	UpdateSpawnList();
	UpdateDespawnList();
}

// 대사 연출 npc 스폰
// 1. 스폰 정보
// 2. 스폰시 위치 정할때 중심이 될 오브젝트
// 3. 로컬 오브젝트
void FGsClientSpawnManager::SpawnDialogObject(TArray<const FGsSchemaNpcDialogActorData*> In_arrSpawnInfo,
	UGsGameObjectBase* In_originObject, UGsGameObjectBase* In_localObject)
{
	if (In_originObject == nullptr ||
		In_localObject == nullptr)
	{
		return;
	}

	for (auto iter : In_arrSpawnInfo)
	{
		const FGsSchemaNpcData* npcData = iter->npcData.GetRow();
		if (npcData == nullptr)
		{
			continue;
		}

		FVector originPos = In_originObject->GetLocation();

		FVector npcLookVec = FVector::ForwardVector;
		FVector npcDir =
			npcLookVec.RotateAngleAxis(iter->spawnInfo.spawnRotate, FVector::UpVector);

		FVector npcPos = originPos - iter->spawnInfo.spawnRange * npcDir;

		FVector lookVec = In_localObject->GetLocation() - npcPos;
		lookVec.Z = 0;
		lookVec.Normalize();

		FRotator npcRot = lookVec.Rotation();

		int spawnIndex =
			GetDialogSpawnIndex();

		int64 gameId = 0;
		TryGenerateClientGameIDInRuntime(gameId, spawnIndex);

		FGsSpawnParam spawnParam(
			EGsGameObjectType::NonPlayer,
			gameId,
			npcData->id,
			npcPos,
			npcRot
		);

		_mapDialogSpawn.Add(npcData->id, spawnIndex);
		_arrayDialogSpawnAll.Add(npcData->id);

		UGsGameObjectBase* spawnNpc = FGsNetMessageHandlerGameObject::SpawnNpc(spawnParam, SPAWN_REASON::SUMMON, true, false);
	
		// 미리 map에 넣고 없으면 빼는 이유는
		// 스폰 시점에 위의 map 값을 참조하기 때문이다(visible 처리 때문에)
		if (spawnNpc == nullptr)
		{
			_mapDialogSpawn.Remove(npcData->id);
			_arrayDialogSpawnAll.Remove(npcData->id);
		}
		else
		{
			spawnNpc->SetAutoIdleEmotionEnable(false);
		}

	}
}

// 대사 페어리 스폰
void FGsClientSpawnManager::SpawnDialogFairy(const FGsSchemaNpcData* In_tblData, UGsGameObjectBase* In_localObject,
	EGsNpcDialogFairyPosType In_posType)
{
	if (In_localObject == nullptr ||
		In_tblData == nullptr)
	{
		return;
	}

	const FGsSchemaCreatureCommonInfo* commonInfo = In_tblData->commonInfoId.GetRow();
	if (commonInfo == nullptr)
	{
		return;
	}

	AActor* localActor = In_localObject->GetActor();

	if (localActor == nullptr)
	{
		return;
	}

	int fairyTblId = In_tblData->id;

	float fairyScale = In_tblData->scale;
	float height = commonInfo->cylinderHeight;
	float radius = commonInfo->cylinderRadius;
	height *= fairyScale;
	radius *= fairyScale;

	FVector npcPos = 
		GetDialogFairyPos(In_localObject, In_posType, radius, height);

	FVector lookVec = In_localObject->GetLocation() - npcPos;
	lookVec.Z = 0;
	lookVec.Normalize();

	FRotator npcRot = lookVec.Rotation();

	int spawnIndex =
		GetDialogSpawnIndex();

	int64 gameId = 0;
	TryGenerateClientGameIDInRuntime(gameId, spawnIndex);

	FGsSpawnParam spawnParam(
		EGsGameObjectType::NonPlayer,
		gameId,
		fairyTblId,
		npcPos,
		npcRot
	);

	_mapDialogSpawn.Add(fairyTblId, spawnIndex);
	_arrayDialogSpawnAll.Add(fairyTblId);

	// quest show 이펙트 출력용
	if (_arrayQuestShowTblId.Contains(fairyTblId) == false)
	{
		_arrayQuestShowTblId.Add(fairyTblId);
	}

	// quest hide 이펙트 출력용(미리 넣어놓고 나중에 디스폰시 출력)
	if (_arrayQuestHideTblId.Contains(fairyTblId) == false)
	{
		_arrayQuestHideTblId.Add(fairyTblId);
	}

	_dialogSpawnFairyTblId = fairyTblId;
	_dialogSpawnFairyPosType = In_posType;

	UGsGameObjectBase* spawnNpc = FGsNetMessageHandlerGameObject::SpawnNpc(spawnParam, SPAWN_REASON::SUMMON, true, false);
	


	// 미리 map에 넣고 없으면 빼는 이유는
	// 스폰 시점에 위의 map 값을 참조하기 때문이다(visible 처리 때문에)
	if (spawnNpc == nullptr)
	{
		_mapDialogSpawn.Remove(fairyTblId);
		_arrayDialogSpawnAll.Remove(fairyTblId);

		_arrayQuestShowTblId.Remove(fairyTblId);
		_arrayQuestHideTblId.Remove(fairyTblId);
	}	
}

// 대사 페어리 위치 갱신
void FGsClientSpawnManager::RepositionDialogFairy(const FVector& In_lookPos)
{
	if (_dialogSpawnFairyTblId == 0)
	{
		return;
	}

	UGsGameObjectBase* targetNpc =
		GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(_dialogSpawnFairyTblId);

	UGsGameObjectBase* localObject =
		GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (targetNpc == nullptr ||
		localObject == nullptr)
	{
		return;
	}

	UGsGameObjectNonPlayerBase* nonPlayerObject = Cast<UGsGameObjectNonPlayerBase>(targetNpc);
	if (nonPlayerObject == nullptr)
	{
		return;
	}

	const FGsSchemaNpcData* tblData = nonPlayerObject->GetNpcData();
	if (tblData == nullptr)
	{
		return;
	}

	const FGsSchemaCreatureCommonInfo* commonInfo = tblData->commonInfoId.GetRow();
	if (commonInfo == nullptr)
	{
		return;
	}

	float fairyScale = tblData->scale;
	float halfHeight = commonInfo->cylinderHeight;
	float fairyRadius = commonInfo->cylinderRadius;

	halfHeight *= fairyScale;
	fairyRadius *= fairyScale;


	FVector fairyPos =
		GetDialogFairyPos(localObject, _dialogSpawnFairyPosType, fairyRadius, halfHeight);

	// fairy nullptr 참조 될수있어서 수정
	// 	https://com2us-raon.sentry.io/issues/4277031241/?alert_rule_id=12817173&alert_timestamp=1687828698846&alert_type=email&environment=Release&project=4504115468566528&referrer=alert_email
	AActor* fairy = targetNpc->GetActor();
	if(nullptr != fairy)
	{ 
		fairy->SetActorLocation(fairyPos);
	}
	else if (targetNpc->GetActorState() == ActorState::LOADING)
	{
		nonPlayerObject->SetActorLocation(fairyPos);
	}


	targetNpc->LookAtPosition(In_lookPos);
}


FVector FGsClientSpawnManager::GetDialogFairyPos(
	UGsGameObjectBase* In_localGameObject, EGsNpcDialogFairyPosType In_posType,
	float In_fairyRadius, float In_fairyHalfHeiht)
{
	if (In_localGameObject == nullptr)
	{
		return FVector::ZeroVector;
	}

	AActor* localActor = In_localGameObject->GetActor();
	AGsCharacterBase* localCharacter = In_localGameObject->GetCharacter();

	if (localActor == nullptr ||
		localCharacter == nullptr)
	{
		return FVector::ZeroVector;
	}

	FVector originPos = localCharacter->GetMovementComponent()->GetActorFeetLocation();

	FVector localLookVec = localActor->GetActorForwardVector();

	float fairySpawnRot =
		GData()->GetGlobalData()->_camDialogFairySpawnRotate;

	fairySpawnRot *= (In_posType == EGsNpcDialogFairyPosType::RIGHT) ? -1.0f : 1.0f;

	FVector npcDir =
		localLookVec.RotateAngleAxis(fairySpawnRot, FVector::UpVector);

	float fairySpawnRange =
		GData()->GetGlobalData()->_camDialogFairySpawnRange;
	FVector npcPos = originPos - fairySpawnRange * npcDir;

	// recalc floor zpos
	FVector reCalcPos = FVector::ZeroVector;
	FHitResult hit;
	if (UGsLevelUtil::TrySweepToLand(hit, localCharacter->GetWorld(), npcPos, In_fairyRadius, In_fairyHalfHeiht))
	{
		reCalcPos = FVector( hit.ImpactPoint.X, hit.ImpactPoint.Y, hit.ImpactPoint.Z + In_fairyHalfHeiht);
	}

#ifdef TEST_DRAW_POS
	if (UWorld* world = In_localGameObject->GetWorld())
	{
		FVector oldPos = npcPos;
		oldPos.Z += In_fairyHalfHeiht;
		DrawDebugCircle(world, oldPos,
			100.0f, 100, FColor::Red, false, 60.f,
			0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));


		FVector newPos = reCalcPos;
		newPos.Z += In_fairyHalfHeiht;
		DrawDebugCircle(world, newPos,
			100.0f, 100, FColor::Cyan, false, 60.f,
			0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
	}
#endif

	npcPos = reCalcPos;
	return npcPos;
}
// 대사 연출 npc 디스폰
void FGsClientSpawnManager::DespawnDialogObject(TArray<int> In_arrTblId)
{
	for (int tblId : In_arrTblId)
	{
		if (_mapDialogSpawn.Contains(tblId) == false)
		{
			continue;
		}

		int spawnIndex =
			_mapDialogSpawn.FindRef(tblId);

		_mapDialogSpawn.Remove(tblId);
		_arrayDialogSpawnAll.Remove(tblId);

		int64 gameId = 0;
		TryGenerateClientGameIDInRuntime(gameId, spawnIndex);

		FGsNetMessageHandlerGameObject::DespawnNpc(gameId, SPAWN_REASON::NORMAL);
	}
}


// 대사 연출 내에서 npc 스폰
// 연출 끝나도 상태 유지
void FGsClientSpawnManager::SpawnGlobalObject(TArray<int> In_arrTblId)
{
	for (int tblId: In_arrTblId)
	{
		TSharedPtr<FGsSpawnParam> findData;
		// 못찾으면 pass
		if (false == TryGetNpcSpawnParam(findData, tblId))
		{
			continue;
		}
		// 이미 스폰되었거나
		// 유효하지 않으면 pass
		if (false == findData.IsValid() || 
			true == IsSpawned(findData))
		{
			continue;
		}
		_arrayDialogSpawnAll.Add(findData->_tId);

		// quest show 이펙트 출력용
		if (_arrayQuestShowTblId.Contains(findData->_tId) == false)
		{
			_arrayQuestShowTblId.Add(findData->_tId);
		}
		SpawnGameObject(findData);
		AddSpawnedGameObject(findData);

		if (auto obj = FindSpawnedGameObject(findData))
		{
			obj->SetAutoIdleEmotionEnable(false);
		}
	}
}
// 대사 연출 내에서 npc 디스폰
// 연출 끝나도 상태 유지
void FGsClientSpawnManager::DespawnGlobalObject(TArray<int> In_arrTblId)
{
	for (int tblId : In_arrTblId)
	{
		TSharedPtr<FGsSpawnParam> findData;
		// 못찾으면 pass
		if (false == TryGetNpcSpawnParam(findData, tblId))
		{
			continue;
		}
		// 이미 스폰되었거나
		// 유효하지 않으면 pass
		if (false == findData.IsValid() ||
			false == IsSpawned(findData))
		{
			continue;
		}

		// quest hide 이펙트 출력용
		if (_arrayQuestHideTblId.Contains(findData->_tId) == false)
		{
			_arrayQuestHideTblId.Add(findData->_tId);
		}

		DespawnGameObject(findData);
		RemoveSpawnedGameObject(findData);

		// 디스폰 id 추가
		_arrayDialogCutGlobalDespawn.Add(tblId);

		if (_arrayDialogSpawnAll.Contains(findData->_tId))
		{
			_arrayDialogSpawnAll.Remove(findData->_tId);
		}
	}
}
// npc object 디스폰
void FGsClientSpawnManager::DespawnNpcByTblId(int In_tblId)
{
	TArray<int> arrayTblId;
	arrayTblId.Add(In_tblId);
	DespawnGlobalObject(arrayTblId);
}
// 대사 연출 종료
void FGsClientSpawnManager::FinishDialog()
{
	for (auto iter : _mapDialogSpawn)
	{
		int spawnIndex =
			_mapDialogSpawn.FindRef(iter.Key);

		int64 gameId = 0;
		TryGenerateClientGameIDInRuntime(gameId, spawnIndex);

		FGsNetMessageHandlerGameObject::DespawnNpc(gameId, SPAWN_REASON::NORMAL);
	}
	_mapDialogSpawn.Empty();
	_arrayDialogSpawnAll.Empty();
	// 초기화
	_dialogIndexOrder = 0;

	_dialogSpawnFairyTblId = 0;
}

// 현재 대사 스폰 인덱스 구하기
int FGsClientSpawnManager::GetDialogSpawnIndex()
{
	// 인덱스 값 사용뒤 증가
	return _dialogIndexOrder++;
}


TArray<const FGsSchemaClientSpawnData*> FGsClientSpawnManager::GetCurrentClientSpawnData(UWorld* inWorld)
{
	int32 levelKey = GLevel()->GetCurrentLevelId();
	return GetClientSpawnData(levelKey);
}

TArray<const FGsSchemaClientSpawnData*> FGsClientSpawnManager::GetClientSpawnData(int32 inLevelKey)
{
	TArray<const FGsSchemaClientSpawnData*> rowList;
	FString levelKey = FString::FromInt(inLevelKey);
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableClientSpawnData>(levelKey);
	const UGsTableManager& tableManager = UGsTableManager::GetInstance();
	const UGsTable* table = tableManager.GetTable(FGsSchemaClientSpawnData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return rowList;
	}

	table->GetAllRows<FGsSchemaClientSpawnData>(rowList);

	return rowList;
}

bool FGsClientSpawnManager::IsClientSpawn(EGsGameObjectType inType, UGsGameObjectBase* inObject)
{
	return false;
}

const FGsSchemaSpawnElement* FGsClientSpawnManager::FindSpawnData(int64 inClientGameId)
{
	return _npcSpawnDataList.FindByPredicate([&](const FGsSchemaSpawnElement& spawnData)->bool {
		return spawnData.ClientGameId == inClientGameId;
	});	
}
// 퀘스트 완료
void FGsClientSpawnManager::OnQuestMainCompleted(const IGsMessageParam*)
{
	ForcedUpdate(true);

	if (_arrayDialogCutGlobalDespawn.Num() > 0)
	{
		_arrayDialogCutGlobalDespawn.Empty();
	}
}
// 퀘스트 수락
void FGsClientSpawnManager::OnQuestMainAccepted(const IGsMessageParam*)
{
	ForcedUpdate(true);
	if (_arrayDialogCutGlobalDespawn.Num() > 0)
	{
		_arrayDialogCutGlobalDespawn.Empty();
	}
}

// tbl id로 npc 퀘스트 진행에 따른 show인지
bool FGsClientSpawnManager::IsQuestProgressShowNpcByTblId(int In_npdTblId)
{
	TSharedPtr<FGsSpawnParam> findData;
	if (TryGetNpcSpawnParam(findData, In_npdTblId) == false)
	{
		return false;
	}

	return IsShow(findData);
}


// 퀘스트 show 리스트 사용하게 id 삭제
// 있으면 true
bool FGsClientSpawnManager::FindAndRemoveQuestShowListId(int In_tblId)
{
	if (_arrayQuestShowTblId.Contains(In_tblId) == false)
	{
		return false;
	}

	_arrayQuestShowTblId.Remove(In_tblId);
	return true;
}
// 퀘스트 hide 리스트 사용하게 id 삭제
// 있으면 true
bool FGsClientSpawnManager::FindAndRemoveQuestHideListId(int In_tblId)
{
	if (_arrayQuestHideTblId.Contains(In_tblId) == false)
	{
		return false;
	}

	_arrayQuestHideTblId.Remove(In_tblId);
	return true;
}

// map warp info로 위치 구하기
FVector FGsClientSpawnManager::FindSpotPos(int32 In_levelKey, const FGsSchemaMapWarpInfo& In_mapWarpInfo,
	bool In_isSource)
{
	FGsSchemaSpotInfo spotData;
	int spotIndex = (In_isSource == true) ? In_mapWarpInfo.sourceSpot.SpotIndex : In_mapWarpInfo.destSpot.SpotIndex;
	if (false == TryGetSpot(In_levelKey, spotIndex, spotData))
	{
		return FVector::ZeroVector;
	}

	return spotData.pos;
}


// 현재 맵워프정보 구하기(워프 id, 워프 fvector)
bool FGsClientSpawnManager::TryGetCurrentMapWarpInfo(int32 In_startLevelKey, 
	const FVector& In_sourcePos, const FVector& In_destPos,
	int& Out_warpId, FGsSchemaSpotInfo& Out_spotInfo, bool In_multiSameDestWarp)
{
	if (_isShowCurrentWarpPos == true)
	{
		ShowCurrentWarpPos();
	}

	TArray<const FGsSchemaMapWarpInfo*> warpInfo;
	TArray<int > exceptionGroupIds;

	// 목적지가 같은 워프가 복수개 있는지
	// https://jira.com2us.com/jira/browse/CHR-24660
	if(In_multiSameDestWarp == false)
	{ 
		if (false == TryGetCurrentMapWarp(In_startLevelKey, In_sourcePos, In_destPos, exceptionGroupIds,warpInfo) ||
			warpInfo.Num() == 0)
		{
			return false;
		}
	}
	else
	{
		if (false == TryGetCurrentMapWarpShortestDist(In_startLevelKey, In_sourcePos, In_destPos, exceptionGroupIds, warpInfo) ||
			warpInfo.Num() == 0)
		{
			return false;
		}
	}
	const FGsSchemaMapWarpInfo* lastInfo = warpInfo.Last();
	if (nullptr == lastInfo)
	{
		return false;
	}

	Out_warpId = lastInfo->warpId;
	if (false == TryGetSpot(In_startLevelKey, lastInfo->sourceSpot.SpotIndex, Out_spotInfo))
	{
		return false;
	}

	return true;
}


bool FGsClientSpawnManager::TryGetCurrentMapWarp(int32 In_startLevelKey, 
	const FVector& In_sourcePos,const FVector& In_destPos, TArray<int>& In_exceptionGroupIds,
	TArray<const FGsSchemaMapWarpInfo*>& Out_orderWarps)
{
	// 1. 목표 위치를 갈수 있는 워프를 찾는다
	FString startLevelKeyString = FString::FromInt(In_startLevelKey);
	FString startTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(startLevelKeyString);
	const UGsTable* territoryMapTable = UGsTableManager::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), startTableName);
	if (nullptr == territoryMapTable)
	{
		return false;
	}

	const FGsSchemaMapTerritoryData* mapTerritoryData;
	territoryMapTable->FindRow<FGsSchemaMapTerritoryData>(*startLevelKeyString, mapTerritoryData);
	if (nullptr == mapTerritoryData)
	{
		return false;
	}


	UGsGameObjectBase* localBaseObject =
		GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (localBaseObject == nullptr)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* localObject = Cast<UGsGameObjectLocalPlayer>(localBaseObject);

	if (localObject == nullptr)
	{
		return false;
	}

	AGsCharacterLocalPlayer* localChar = localObject->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return false;
	}
	AGsPlayerController* controller = localChar->GetPlayerController();
	if (controller == nullptr)
	{
		return false;
	}

	float capsuleRadius =
		localObject->GetData()->GetScaledCapsuleRadius();

	for (const FGsSchemaMapWarpInfo& iter : mapTerritoryData->warpList)
	{
		// group id가 0이면 skip
		if (iter.groupId == 0)
		{
			continue;
		}

		// 이미 등록된 warp 정보면 skip
		if (true == Out_orderWarps.Contains(&iter))
		{
			continue;
		}

		// 이미 다른 워프의 목적지가 등록된거라면
		// group id는 dest의 섬맵 id 이다
		if (true == In_exceptionGroupIds.Contains(iter.groupId))
		{
			continue;
		}

		// 해당 워프 위치에서 목적지까지 이동 가능한가
		// 워프의 도착 지점으로 찾음
		FVector warpDestPos = FVector::ZeroVector;

		float spotRadius = capsuleRadius;
		FGsSchemaSpotInfo findSpotInfo;
		if (true == TryGetSpot(In_startLevelKey, iter.destSpot.SpotIndex, findSpotInfo))
		{
			spotRadius = findSpotInfo.radius;
			warpDestPos = findSpotInfo.pos;
		}

		float warpDestCalcAcceptanceRadius = (spotRadius - capsuleRadius) * 0.4f;

		// 이동 가능
		if (true == FGsMove::IsMovePossible(controller, 
			localChar,
			In_destPos, warpDestPos, warpDestCalcAcceptanceRadius))
		{
			// 리스트에 추가
			Out_orderWarps.Add(&iter);
			// 그룹 id 추가
			In_exceptionGroupIds.Add(iter.groupId);

			// 2. 목적지 워프 까지 가는길 찾기

			// 한번에 바로 가지는지
			FGsSchemaSpotInfo warpSourceSpotInfo;
			if (false == TryGetSpot(In_startLevelKey, iter.sourceSpot.SpotIndex, warpSourceSpotInfo))
			{
				Out_orderWarps.Remove(&iter);
				In_exceptionGroupIds.Remove(iter.groupId);
				return false;
			}
			float warpSourceCalcAcceptanceRadius = (warpSourceSpotInfo.radius - capsuleRadius) * 0.4f;
			if (true == FGsMove::IsMovePossible(controller, 
				localChar,
				In_sourcePos, warpSourceSpotInfo.pos, warpSourceCalcAcceptanceRadius))
			{
				return true;
			}

			// 한번에 못가면 거쳐서 찾는다
			if (true == TryGetCurrentMapWarp(In_startLevelKey, In_sourcePos, warpSourceSpotInfo.pos,
				In_exceptionGroupIds,
				Out_orderWarps))
			{
				return true;
			}

			Out_orderWarps.Remove(&iter);
			In_exceptionGroupIds.Remove(iter.groupId);
		}
	}

	return false;
}

// 내부 함수(워프 정보 구하기, 복수개중 찾기, 그림자 전장)
bool FGsClientSpawnManager::TryGetCurrentMapWarpShortestDist(int32 In_startLevelKey,
	const FVector& In_sourcePos, const FVector& In_destPos, TArray<int>& In_exceptionGroupIds,
	TArray<const FGsSchemaMapWarpInfo*>& Out_orderWarps)
{
	// 1. 목표 위치를 갈수 있는 워프를 찾는다
	FString startLevelKeyString = FString::FromInt(In_startLevelKey);
	FString startTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(startLevelKeyString);
	const UGsTable* territoryMapTable = UGsTableManager::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), startTableName);
	if (nullptr == territoryMapTable)
	{
		return false;
	}

	const FGsSchemaMapTerritoryData* mapTerritoryData;
	territoryMapTable->FindRow<FGsSchemaMapTerritoryData>(*startLevelKeyString, mapTerritoryData);
	if (nullptr == mapTerritoryData)
	{
		return false;
	}


	UGsGameObjectBase* localBaseObject =
		GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (localBaseObject == nullptr)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* localObject = Cast<UGsGameObjectLocalPlayer>(localBaseObject);

	if (localObject == nullptr)
	{
		return false;
	}

	AGsCharacterLocalPlayer* localChar = localObject->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return false;
	}
	AGsPlayerController* controller = localChar->GetPlayerController();
	if (controller == nullptr)
	{
		return false;
	}

	float capsuleRadius =
		localObject->GetData()->GetScaledCapsuleRadius();

	// 이동 가능한 워프
	TArray<FGsWarpSortInfo> arrayPossibleMoveWarps;

	for (const FGsSchemaMapWarpInfo& iter : mapTerritoryData->warpList)
	{
		// group id가 0이면 skip
		if (iter.groupId == 0)
		{
			continue;
		}

		// 이미 등록된 warp 정보면 skip
		if (true == Out_orderWarps.Contains(&iter))
		{
			continue;
		}

		// 이미 다른 워프의 목적지가 등록된거라면
		// group id는 dest의 섬맵 id 이다
		if (true == In_exceptionGroupIds.Contains(iter.groupId))
		{
			continue;
		}

		// 해당 워프 위치에서 목적지까지 이동 가능한가
		// 워프의 도착 지점으로 찾음
		FVector warpDestPos = FVector::ZeroVector;

		float spotRadius = capsuleRadius;
		FGsSchemaSpotInfo findSpotInfo;
		if (true == TryGetSpot(In_startLevelKey, iter.destSpot.SpotIndex, findSpotInfo))
		{
			spotRadius = findSpotInfo.radius;
			warpDestPos = findSpotInfo.pos;
		}

		float warpDestCalcAcceptanceRadius = (spotRadius - capsuleRadius) * 0.4f;

		// 이동 가능
		if (true == FGsMove::IsMovePossible(controller,
			localChar,
			In_destPos, warpDestPos, warpDestCalcAcceptanceRadius))
		{
			FGsSchemaSpotInfo warpSourceSpotInfo;
			if (false == TryGetSpot(In_startLevelKey, iter.sourceSpot.SpotIndex, warpSourceSpotInfo))
			{
				continue;
			}
			float warpSourceCalcAcceptanceRadius = (warpSourceSpotInfo.radius - capsuleRadius) * 0.4f;


			// 제곱 거리
			float distSquared = FVector::DistSquared2D( In_sourcePos, warpSourceSpotInfo.pos);
			arrayPossibleMoveWarps.Add(FGsWarpSortInfo{ 
				&iter, distSquared, warpSourceSpotInfo.pos , warpSourceCalcAcceptanceRadius });

		}
	}

	// 복수개중 시작점과 제일 가까운순으로 정렬

	// 이동 가능한곳이 없음
	if (arrayPossibleMoveWarps.Num() == 0)
	{
		return false;
	}

	// 정렬
	arrayPossibleMoveWarps.Sort([](const FGsWarpSortInfo& lhs, const FGsWarpSortInfo& rhs)
		{
			return lhs._distSquared < rhs._distSquared;
		});

	for (FGsWarpSortInfo& iter: arrayPossibleMoveWarps)
	{
		if (iter._warpInfo == nullptr)
		{
			continue;
		}

		// 리스트에 추가
		Out_orderWarps.Add(iter._warpInfo);
		// 그룹 id 추가
		In_exceptionGroupIds.Add(iter._warpInfo->groupId);

		// 2. 목적지 워프 까지 가는길 찾기

		// 한번에 바로 가지는지
		if (true == FGsMove::IsMovePossible(controller,
			localChar,
			In_sourcePos, iter._warpSourceSpotPos, iter._warpSourceCalcAcceptanceRadius))
		{
			return true;
		}

		// 한번에 못가면 거쳐서 찾는다
		if (true == TryGetCurrentMapWarpShortestDist(In_startLevelKey, In_sourcePos, iter._warpSourceSpotPos,
			In_exceptionGroupIds,
			Out_orderWarps))
		{
			return true;
		}

		Out_orderWarps.Remove(iter._warpInfo);
		In_exceptionGroupIds.Remove(iter._warpInfo->groupId);
	}


	return false;
}

void FGsClientSpawnManager::ShowCurrentWarpPos()
{
	if (GLevel() == nullptr)
	{
		return;
	}

	int currentMapId = GLevel()->GetCurrentLevelId();

	FString startLevelKeyString = FString::FromInt(currentMapId);
	FString startTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(startLevelKeyString);
	const UGsTable* territoryMapTable = UGsTableManager::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), startTableName);
	if (nullptr == territoryMapTable)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* mapTerritoryData;
	territoryMapTable->FindRow<FGsSchemaMapTerritoryData>(*startLevelKeyString, mapTerritoryData);
	if (nullptr == mapTerritoryData)
	{
		return;
	}

	for (const FGsSchemaMapWarpInfo& iter : mapTerritoryData->warpList)
	{
		FVector warpDestPos = FVector::ZeroVector;

		FGsSchemaSpotInfo findDestSpotInfo;
		if (true == TryGetSpot(currentMapId, iter.destSpot.SpotIndex, findDestSpotInfo))
		{
			warpDestPos = findDestSpotInfo.pos;
		}

		FVector warpSourcePos = FVector::ZeroVector;

		FGsSchemaSpotInfo findSourceSpotInfo;
		if (true == TryGetSpot(currentMapId, iter.sourceSpot.SpotIndex, findSourceSpotInfo))
		{
			warpSourcePos = findSourceSpotInfo.pos;
		}

		DrawDebugSphere(GLevel()->GetWorld(), warpDestPos, 100.0f, 8, FColor::Red, false, 1000.0f);

		DrawDebugSphere(GLevel()->GetWorld(), warpSourcePos, 100.0f, 8, FColor::Green, false, 1000.0f);
	}
}