#include "GsWorldMapManager.h"

#include "Map/WorldMap/GsContinentalInfo.h"
#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/WorldMap/GsRegionMonsterInfo.h"
#include "Map/WorldMap/GsSpaceCrackPortalIconInfo.h"
#include "Map/WorldMap/GsContinentalSpaceCrackMapInfo.h"
#include "Map/WorldMap/GsRegionInvadeIconInfo.h"
#include "Map/GsMapFunc.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "Option/GsServerOption.h"

#include "Net/GsNetSendServiceWorld.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Map/WorldMap/GsSchemaContinentalMapData.h"
#include "DataSchema/Map/WorldMap/GsSchemaRegionMapData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapDungeonData.h"
#include "DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackData.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/Map/Area/GsSchemaAreaData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitConfig.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/State/GsGameObjectStateManager.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/GsAutoMoveInfo.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataCenter/Public/Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "UI/UIContent/Popup/GsUIPopupWarp.h"
#include "UI/UIContent/Window/GsUIWindowRegionMap.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsUIUtil.h"
#include "UTIL/GsMove.h"
#include "UTIL/GsGameObjectUtil.h"

#include "GsSchemaBase.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Bookmark/GsBookmarkHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Currency/GsCostPackage.h"

#include "Sequence/GsSequencePlayer.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

// 초기화
void FGsWorldMapManager::Initialize()
{
	// 테이블 데이터 가공
	ProcessTableData();

	InitializeDatas();

	if (FGsMessageHolder* msg = GMessage())
	{
		_objectDelegates.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
				&FGsWorldMapManager::OnLocalPlayerSpawnComplete)
		);

		_objectDelegates.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_REVIVE, this,
				&FGsWorldMapManager::OnLocalPlayerSpawnComplete)
		);

		_systemMsg.Emplace(
			msg->GetSystemParam().AddRaw(MessageSystem::LOAD_TOPO_START, this,
				&FGsWorldMapManager::OnLoadTopoStart));

		_messageInvasionList.Emplace(
			msg->GetInvasion().AddRaw(MessageInvasion::INVAION_FINISH, this,
				&FGsWorldMapManager::OnInvasionFinish));
	}
}
// 정리
void FGsWorldMapManager::Finalize()
{
	if (_mapContinentalInfo.Num() != 0)
	{
		for (auto iter : _mapContinentalInfo)
		{
			if (nullptr != iter.Value)
			{
				iter.Value->Finalize();
				delete iter.Value;
			}
		}
		_mapContinentalInfo.Empty();
	}

	if (_mapDungeonInfo.Num() != 0)
	{
		for (auto dungeonInfo : _mapDungeonInfo)
		{
			if (nullptr != dungeonInfo.Value)
			{
				dungeonInfo.Value->Finalize();
				delete dungeonInfo.Value;
			}
		}
		_mapDungeonInfo.Empty();
	}

	if (_mapSpaceCrackInfo.Num() != 0)
	{
		for (auto spaceCrackInfo : _mapSpaceCrackInfo)
		{
			if (nullptr != spaceCrackInfo.Value)
			{
				spaceCrackInfo.Value->Finalize();
				delete spaceCrackInfo.Value;
			}
		}
		_mapSpaceCrackInfo.Empty();
	}
	// 기사단 영지 지도 표시
	// https://jira.com2us.com/jira/browse/CHR-24959
	if (_agitInfo != nullptr)
	{
		_agitInfo->Finalize();
		delete _agitInfo;
		_agitInfo = nullptr;
	}

	if (_arraySpaceCrackSortInfo.Num() != 0)
	{
		for (auto spaceCrackSortInfo : _arraySpaceCrackSortInfo)
		{
			if (spaceCrackSortInfo != nullptr)
			{
				delete spaceCrackSortInfo;
			}
		}
		_arraySpaceCrackSortInfo.Empty();
	}

	_arrContinentalOrder.Empty();

	if (_mapRegionCacheData.Num() != 0)
	{
		for (auto regionInfo : _mapRegionCacheData)
		{
			if (nullptr != regionInfo.Value)
			{
				regionInfo.Value->Finalize();
				delete regionInfo.Value;
			}
		}
		_mapRegionCacheData.Empty();
	}

	ClearRegionInvadeIconInfo();

	// 바인딩 된 메시지 해제
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_objectDelegates.Num() != 0)
		{
			for (auto iter : _objectDelegates)
			{
				msg->GetGameObject().Remove(iter);
			}
			_objectDelegates.Empty();
		}

		if (_systemMsg.Num() != 0)
		{
			for (auto iter : _systemMsg)
			{
				msg->GetSystemParam().Remove(iter);
			}
			_systemMsg.Empty();
		}

		if (_messageInvasionList.Num() != 0)
		{
			for (auto iter : _messageInvasionList)
			{
				msg->GetInvasion().Remove(iter);
			}
			_messageInvasionList.Empty();
		}
	}
}
// 로컬 포인터 저장
void FGsWorldMapManager::SetCharacter(UGsGameObjectLocalPlayer* In_player)
{
	_localPlayer = In_player;
}

// 로컬 포인터 참조 제거
void FGsWorldMapManager::RemoveCharacter()
{
	_localPlayer = nullptr;
}
// 테이블 데이터 가공
void FGsWorldMapManager::ProcessTableData()
{
	GSLOG(Error, TEXT("FGsWorldMapManager ProcessTableData start"));
	const UGsTable* mapDataTbl = FGsSchemaMapData::GetStaticTable();

	if (nullptr == mapDataTbl)
	{
		return;
	}

	TArray<const FGsSchemaMapData*> rowsMapData;
	mapDataTbl->GetAllRows<FGsSchemaMapData>(rowsMapData);

	if (rowsMapData.Num() == 0)
	{
		return;
	}
	GSLOG(Error, TEXT("FGsWorldMapManager ProcessTableData rowsMapData for loop"));

	const FGsSchemaAreaData* findAreaTbl;
	int mapId;
	bool isRegionMapShow = false;
	for (const FGsSchemaMapData* iter : rowsMapData)
	{
		if (nullptr == iter)
		{
			continue;
		}
		// public field 가 아니면 표시 안함
		// 던전도 넣어야 할듯-> 던전에서 상위맵 가야 하므로
		// https://jira.com2us.com/jira/browse/CHR-19997
		// 엘리트 던전,월드 보스 도 추가
		// 그림자 전동도 던전쪽으로 처리
		//https://jira.com2us.com/jira/browse/C2URWQ-7778

		// 기사단 영지 지도 표시
		// https://jira.com2us.com/jira/browse/CHR-24959

		if (iter->mapType != MapType::PUBLIC_FIELD &&
			iter->mapType != MapType::INTER_FIELD &&
			iter->mapType != MapType::PUBLIC_DUNGEON &&
			iter->mapType != MapType::ELITE_DUNGEON_1 &&
			iter->mapType != MapType::WORLD_BOSS &&
			iter->mapType != MapType::GUILD_WAR &&
			iter->mapType != MapType::AGIT)
		{
			continue;
		}

		if (nullptr == iter->areaId.GetRow())
		{
			continue;
		}

		findAreaTbl = iter->areaId.GetRow();
		int continentalId = findAreaTbl->continentId;

		// 대륙맵 테이블에 없으면 처리안함
		// 초반 맵추가되었는데 0번: 실제 대륙맵 데이터 없는것이 있음(여기선 지도도 안열림)
		const FGsSchemaContinentalMapData* continentalMapData =
			UGsTableUtil::FindRowById<UGsTableContinentalMapData, FGsSchemaContinentalMapData>(continentalId);

		if (continentalMapData == nullptr)
		{
			continue;
		}

		FGsContinentalInfo** findInfo =	_mapContinentalInfo.Find(continentalId);
		mapId =	UGsTableUtil::ConvertRow<int32>(iter);	
		isRegionMapShow = false;
		if (const FGsSchemaRegionMapData* data = UGsTableUtil::FindRowById<UGsTableRegionMapData, FGsSchemaRegionMapData>(mapId))
		{
			isRegionMapShow = data->regionMapShow;
		}
			

		// 기존에 없다면
		if (findInfo == nullptr)
		{
			FGsContinentalInfo* newData = new FGsContinentalInfo();

			newData->_groupId = continentalId;
			if ((iter->mapType == MapType::PUBLIC_FIELD || iter->mapType == MapType::INTER_FIELD) &&
				isRegionMapShow == true)
			{
				newData->_arrayRegionId.Add(mapId);
			}
			// https://jira.com2us.com/jira/browse/CHR-19997
			// 엘리트 던전,월드 보스 도 추가
			// 그림자 전동도 던전쪽으로 처리
			//https://jira.com2us.com/jira/browse/C2URWQ-7778
			else if (iter->mapType == MapType::PUBLIC_DUNGEON ||
				iter->mapType == MapType::ELITE_DUNGEON_1 ||
				iter->mapType == MapType::WORLD_BOSS ||
				iter->mapType == MapType::GUILD_WAR
			)
			{
				newData->_arrayDungeonRegionId.Add(mapId);
			}
			// 기사단 영지 지도 표시
			// https://jira.com2us.com/jira/browse/CHR-24959
			else if (iter->mapType == MapType::AGIT)
			{
				newData->_agitRegionId = mapId;
			}

			_mapContinentalInfo.Add(continentalId, newData);

			_arrContinentalOrder.Add(continentalId);
		}
		else
		{
			if ((iter->mapType == MapType::PUBLIC_FIELD || iter->mapType == MapType::INTER_FIELD) &&
				isRegionMapShow == true)
			{
				(*findInfo)->_arrayRegionId.Add(mapId);
			}
			// https://jira.com2us.com/jira/browse/CHR-19997
			// 엘리트 던전,월드 보스 도 추가
			// 그림자 전동도 던전쪽으로 처리
			//https://jira.com2us.com/jira/browse/C2URWQ-7778
			else if (iter->mapType == MapType::PUBLIC_DUNGEON ||
				iter->mapType == MapType::ELITE_DUNGEON_1 ||
				iter->mapType == MapType::WORLD_BOSS ||
				iter->mapType == MapType::GUILD_WAR)
			{
				(*findInfo)->_arrayDungeonRegionId.Add(mapId);
			}
			// 기사단 영지 지도 표시
			// https://jira.com2us.com/jira/browse/CHR-24959
			else if (iter->mapType == MapType::AGIT)
			{
				(*findInfo)->_agitRegionId = mapId;
			}
		}


		const FGsSchemaSpaceCrackData* findSpaceCrackData = FindSpaceCrackData(mapId);

		// continental data setting only with space crack data
		if (findSpaceCrackData != nullptr)
		{
			FGsRegionGroupInfoBase** findSpaceCrackInfo = _mapSpaceCrackInfo.Find(continentalId);
			if (findSpaceCrackInfo == nullptr)
			{
				FGsRegionGroupInfoBase* newData = new FGsRegionGroupInfoBase();

				newData->_groupId = continentalId;
				if ((iter->mapType == MapType::PUBLIC_FIELD || iter->mapType == MapType::INTER_FIELD) &&
					isRegionMapShow == true)
				{
					newData->_arrayRegionId.Add(mapId);
				}

				_mapSpaceCrackInfo.Add(continentalId, newData);
			}
			else
			{
				if ((iter->mapType == MapType::PUBLIC_FIELD || iter->mapType == MapType::INTER_FIELD) &&
					isRegionMapShow == true)
				{
					(*findSpaceCrackInfo)->_arrayRegionId.Add(mapId);
				}
			}
		}
		
	}

	GSLOG(Error, TEXT("FGsWorldMapManager ProcessTableData _mapContinentalInfo for sort"));

	// 돌면서 지역정보 리스트 정렬(tbl id 순서)
	for (auto iter: _mapContinentalInfo)
	{
		if (iter.Value == nullptr)
		{
			continue;
		}

		if (iter.Value->_arrayRegionId.Num() != 0)
		{
			iter.Value->_arrayRegionId.Sort();
		}
		if (iter.Value->_arrayDungeonRegionId.Num() != 0)
		{
			iter.Value->_arrayDungeonRegionId.Sort();
		}
	}

	for (auto iter : _mapSpaceCrackInfo)
	{
		if (iter.Value == nullptr)
		{
			continue;
		}

		if (iter.Value->_arrayRegionId.Num() != 0)
		{
			iter.Value->_arrayRegionId.Sort();
		}
	}

	// id 순서로 정렬
	_arrContinentalOrder.Sort();

	const UGsTable* worldmapDungeonTbl = FGsSchemaWorldMapDungeonData::GetStaticTable();

	if (worldmapDungeonTbl == nullptr)
	{
		return;
	}

	// dungeon info
	TArray<const FGsSchemaWorldMapDungeonData*> rowsWorldmapDungeonData;
	worldmapDungeonTbl->GetAllRows<FGsSchemaWorldMapDungeonData>(rowsWorldmapDungeonData);

	if (rowsWorldmapDungeonData.Num() == 0)
	{
		return;
	}
	for (const FGsSchemaWorldMapDungeonData* iter : rowsWorldmapDungeonData)
	{
		if (nullptr == iter)
		{
			continue;
		}

		int dungeonId = iter->groupId;
	
		FGsRegionGroupInfoBase** findInfo =
			_mapDungeonInfo.Find(dungeonId);

		// key already exist
		if (findInfo != nullptr)
		{
			continue;
		}
		
		FGsRegionGroupInfoBase* newData = new FGsRegionGroupInfoBase();
		newData->_groupId = dungeonId;
		newData->_arrayRegionId = iter->regionMapId;

		_mapDungeonInfo.Add(dungeonId, newData);	
	}

	// 아지트 정보 추가
	// https://jira.com2us.com/jira/browse/CHR-24959

	if (_agitInfo != nullptr)
	{
		delete _agitInfo;
	}
	_agitInfo = new FGsRegionGroupInfoBase();
	const FGsSchemaAgitConfig* agitTbl = nullptr;
	if (const UGsTable* table = FGsSchemaAgitConfig::GetStaticTable())
	{
		TArray<const FGsSchemaAgitConfig*> rowList;
		table->GetAllRows(rowList);

		if (0 < rowList.Num())
		{
			agitTbl = rowList[0];
		}
	}

	if (agitTbl != nullptr)
	{
		if (const FGsSchemaMapData* agitMapId = agitTbl->mapId.GetRow())
		{
			if (nullptr != agitMapId->areaId.GetRow())
			{
				const FGsSchemaAreaData* agitAreaTbl = agitMapId->areaId.GetRow();
				int continentalId = agitAreaTbl->continentId;

				_agitInfo->_groupId = continentalId;
				_agitInfo->_arrayRegionId.Add(agitMapId->id);
			}			
		}
	}
	
	

	GSLOG(Error, TEXT("FGsWorldMapManager ProcessTableData finish"));
}

const FGsSchemaSpaceCrackData* FGsWorldMapManager::FindSpaceCrackData(int In_mapId)
{
	const UGsTable* spaceCrackDataTbl = FGsSchemaSpaceCrackData::GetStaticTable();
	if (nullptr == spaceCrackDataTbl)
	{
		return nullptr;
	}

	TArray<const FGsSchemaSpaceCrackData*> rowsSpaceCrackData;
	spaceCrackDataTbl->GetAllRows<FGsSchemaSpaceCrackData>(rowsSpaceCrackData);

	if (rowsSpaceCrackData.Num() == 0)
	{
		return nullptr;
	}

	for (const FGsSchemaSpaceCrackData* iter : rowsSpaceCrackData)
	{
		if (iter == nullptr)
		{
			continue;
		}

		if (iter->mapId.IsNull() == true)
		{
			continue;
		}

		const FGsSchemaMapData* targetMapData = iter->mapId.GetRow();
		if (targetMapData == nullptr)
		{
			continue;
		}
		if (targetMapData->id == In_mapId)
		{
			return iter;
		}
	}

	return nullptr;
}
void FGsWorldMapManager::InitializeDatas()
{
	_continentIdPerMapId.Reset(/*예상갯수*/);
	for (auto iter : _mapContinentalInfo)
	{
		if (nullptr == iter.Value)
		{
			continue;
		}

		for (const MapId CurrentMapId : iter.Value->_arrayRegionId)
		{
			_continentIdPerMapId.Emplace(CurrentMapId, StaticCast<ContinentId>(iter.Value->_groupId));
		}

		for (const MapId CurrentMapId : iter.Value->_arrayDungeonRegionId)
		{
			_continentIdPerMapId.Emplace(CurrentMapId, StaticCast<ContinentId>(iter.Value->_groupId));
		}
	}
}
// 인덱스(리스트 순서)로 맵 id 구하기
int FGsWorldMapManager::GetContinentalMapId(int In_index)
{
	// 배열 보다 크거나
	// 0보다 작으면 리턴
	if (_arrContinentalOrder.Num() <= In_index ||
		In_index < 0)
	{
		return -1;
	}

	return _arrContinentalOrder[In_index];
}

// 인덱스로 대륙맵 데이터 구하기
const FGsSchemaContinentalMapData* FGsWorldMapManager::FindContinentalMapDataByIndex(int In_index)
{
	int continentalMapId = GetContinentalMapId(In_index);
	if (-1 == continentalMapId)
	{
		return nullptr;
	}
	return UGsTableUtil::FindRowById<UGsTableContinentalMapData, FGsSchemaContinentalMapData>(continentalMapId);
}
// 인덱스로 대륙맵 안인지 체크하기
bool FGsWorldMapManager::IsLocalInContinentalMapByIndex(int In_index)
{
	// ui 의 대륙 id
	int continentalMapId = GetContinentalMapId(In_index);
	const FGsSchemaMapData* mapTbl = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(_currentLevelId);
	if (nullptr == mapTbl
	|| nullptr == mapTbl->areaId.GetRow())
	{
		return false;
	}

	return mapTbl->areaId.GetRow()->continentId == continentalMapId;
}

// 포털 스팟 정보 구하기
FGsRegionInfo* FGsWorldMapManager::FindRegionInfo(int In_mapId)
{
	FGsRegionInfo* findInfo =
		_mapRegionCacheData.FindRef(In_mapId);

	return findInfo;	
}

// 포털 스팟 정보 저장하기
void FGsWorldMapManager::SetRegionInfo(int In_mapId, FGsRegionInfo* In_info)
{
	_mapRegionCacheData.Add(In_mapId, In_info);
}

// 지역 맵 데이터 로드
void FGsWorldMapManager::LoadRegionData(int In_mapId)
{
	const FGsSchemaRegionMapData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapData, FGsSchemaRegionMapData>(In_mapId);

	if (data == nullptr)
	{
		return;
	}

	_regionImageHalfSize = data->regionMapSize * 0.5;
}
FGsMovementHandlerLocalPlayer* FGsWorldMapManager::GetMovementHandlerLocalPlayer()
{	
	if (nullptr == _localPlayer)
	{
		return nullptr;
	}

	FGsMovementHandlerLocalPlayer* movement = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	return movement;
}
// 포털 워프 시도
void FGsWorldMapManager::TryPortalWarp(int In_index)
{
	FGsPortalInfo* portalInfo =	GetCurrentRegionPortalInfoByIndex(In_index);
	if (portalInfo == nullptr)
	{
		return;
	}

	int32 level = GGameData()->GetUserData()->mLevel;	
	if (UGsLevelManager::IsCanSpotWarp(_showRegionMapId, portalInfo->_portalSpotId, level))
	{
		// 서버 전송
		FGsNetSendServiceWorld::SendPortalWarp(_showRegionMapId, portalInfo->_portalSpotId);
	}	
}
// 침공 포털 워프 시도
void FGsWorldMapManager::TryInvasionPortalWarp(int64 In_spotId)
{
	int32 level = GGameData()->GetUserData()->mLevel;
	if (UGsLevelManager::IsCanSpotWarp(_showRegionMapId, In_spotId, level))
	{
		// 서버 전송
		FGsNetSendServiceWorld::SendPortalWarp(_showRegionMapId, In_spotId);
	}
}

// 몬스터 스폰 위치 워프 시도
void FGsWorldMapManager::TryMonsterWarp(MapId In_mapId, CreatureId In_npcId, SpawnGroupId In_spawnGroupId)
{
	int32 level = GGameData()->GetUserData()->mLevel;
	if (UGsLevelManager::IsCanMonsterWarp(In_mapId, level))
	{
		// 서버 전송
		FGsNetSendServiceWorld::SendMonsterSpawnWarp(In_mapId, In_npcId, In_spawnGroupId);
	}
}

// 현재 지역맵 id로 상위 맵 table id 구하기
int FGsWorldMapManager::FindContinentalTblIdByCurrentRegionId()
{
	int findContinentalMapId = -1;
	for (auto iter : _mapContinentalInfo)
	{
		if (nullptr == iter.Value)
		{
			continue;
		}

		if (true == iter.Value->_arrayRegionId.Contains(_showRegionMapId))
		{
			findContinentalMapId = iter.Value->_groupId;
			break;
		}

		if (true == iter.Value->_arrayDungeonRegionId.Contains(_showRegionMapId))
		{
			findContinentalMapId = iter.Value->_groupId;
			break;
		}
		// 기사단 영지 지도 표시
		// https://jira.com2us.com/jira/browse/CHR-24959
		if (_showRegionMapId == iter.Value->_agitRegionId)
		{
			findContinentalMapId = iter.Value->_groupId;
			break;
		}
	
	}
	return findContinentalMapId;
}
// get dungeon tbl id by current region id
int FGsWorldMapManager::FindDungeonGroupTblIdByCurrentRegionId()
{
	return FindDungeonGroupTblIdByMapId(_showRegionMapId);
}

// get dungeon tbl id by map id
int FGsWorldMapManager::FindDungeonGroupTblIdByMapId(int In_mapId)
{
	int findDungeonGroupTblId = -1;
	for (auto iter : _mapDungeonInfo)
	{
		if (nullptr == iter.Value)
		{
			continue;
		}
		if (true == iter.Value->_arrayRegionId.Contains(In_mapId))
		{
			findDungeonGroupTblId = iter.Value->_groupId;
			break;
		}
	}
	return findDungeonGroupTblId;
}
// 맵 id로 던전 그룹 테이블 접근
const FGsSchemaDungeonGroupData* FGsWorldMapManager::FindDungeonGroupTblByMapId(int In_mapId)
{
	int dungeonGroupId =  FindDungeonGroupTblIdByMapId(In_mapId);
	if (dungeonGroupId == -1)
	{
		return nullptr;
	}
	const FGsSchemaDungeonGroupData* dungeonGroupTbl =
		UGsTableUtil::FindRowById<UGsTableDungeonGroupData, FGsSchemaDungeonGroupData>(dungeonGroupId);
	return dungeonGroupTbl;
}

ContinentId FGsWorldMapManager::FindContinentalIdBy(const MapId InMapId) const
{
	const ContinentId* CurrentContinentId = _continentIdPerMapId.Find(InMapId);
	return (nullptr == CurrentContinentId) ? INVALID_CONTINENT_ID : *CurrentContinentId;
}

// 현재 지역맵 id로 상위 맵 인덱스 구하기(지역맵에서 대륙맵 갈때 사용)
int FGsWorldMapManager::FindContinentalIndexByCurrentRegionId()
{
	int findContinentalMapId = FindContinentalTblIdByCurrentRegionId();

	if (findContinentalMapId == -1)
	{
		return -1;
	}

	int findIndex = -1;
	for (int i=0; i< _arrContinentalOrder.Num(); ++i)
	{
		if (_arrContinentalOrder[i] == findContinentalMapId)
		{
			findIndex = i;
			break;
		}
	}

	return findIndex;
}

// get group info by current region map id
FGsRegionGroupInfoBase* FGsWorldMapManager::FindGroupInfoByCurrentRegionId()
{
	EGsWorldMapGroupType groupType = GetShowRegionMapGroupType();

	if (groupType == EGsWorldMapGroupType::Field)
	{
		int findContinentalMapId = FindContinentalTblIdByCurrentRegionId();
		return _mapContinentalInfo.FindRef(findContinentalMapId);
	}
	else if(groupType == EGsWorldMapGroupType::Dungeon)
	{
		int findDungeonGroupTblId = FindDungeonGroupTblIdByCurrentRegionId();
		return _mapDungeonInfo.FindRef(findDungeonGroupTblId);
	}
	else if (groupType == EGsWorldMapGroupType::Agit)
	{
		return _agitInfo;
	}

	return nullptr;
}

// 대상 맵이랑 같은지
bool FGsWorldMapManager::IsTargetMapSame(EGsWorldMapChangeType In_changeType)
{
	int targetMapId = -1;
	switch (In_changeType)
	{
	case EGsWorldMapChangeType::TO_REGIONMAP_MYPOS:
		// 현재 내가 속한 맵정보 출력
		targetMapId = _currentLevelId;
		break;
	case EGsWorldMapChangeType::TO_REGIONMAP_AUTOMOVEPOS:	
		{
			// 자동 이동 목적지 맵 출력
			FGsMovementHandlerLocalPlayer* movementHandler = GetMovementHandlerLocalPlayer();
			if (movementHandler != nullptr)
			{
				targetMapId =
					movementHandler->GetStateAutoMove().GetMoveToMapId();
			}		
		}		
		break;	
	}
	return (targetMapId == _showRegionMapId)? true: false;
}

// 자동 이동 표시 가능한 상태인가
bool FGsWorldMapManager::IsUseableShowAutoMovePos()
{
	// 자동 이동중이 아니면 안함
	FGsMovementHandlerLocalPlayer* movementHandler = GetMovementHandlerLocalPlayer();

	if (movementHandler == nullptr ||
		movementHandler->GetState() != EGsMovementState::AUTO)
	{
		return false;
	}

	// 자동 이동 타입이 전투이동이면 안됨
	if (movementHandler->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::MoveToSkill)
	{
		return false;
	}
	return true;
}
// 자동 이동 목적지 위치 구하기
FVector FGsWorldMapManager::GetAutoMovePos()
{
	// 자동 이동중이 아니면 안함
	FGsMovementHandlerLocalPlayer* movementHandler = GetMovementHandlerLocalPlayer();

	if (movementHandler == nullptr)
	{
		return FVector::ZeroVector;
	}

	FVector movePos = FVector::ZeroVector;
	if (movementHandler->GetStateAutoMove().IsMoveNextMap() == true)
	{
		movePos = movementHandler->GetStateAutoMove().GetNextMapPos();
	}
	else
	{
		FAIMoveRequest request = movementHandler->GetStateAutoMove().GetDataRequest();
		movePos = request.GetDestination();
	}
	return movePos;
}
// ui 위치 구하기
FVector2D FGsWorldMapManager::GetUIPos(const FVector2D& In_objPos)
{
	// 배율 곱하기
	FVector2D targetIconPos = In_objPos * _uiSceneImageRate;

	// offset 센터 중심에서 이동
	FVector2D uiPos(targetIconPos.X - _regionImageHalfSize.X, 
		targetIconPos.Y - _regionImageHalfSize.Y);

	uiPos += _uiShiftMapPos;

	return uiPos;
}

// uiscene image pos -> real pos
FVector2D FGsWorldMapManager::GetRealPosFromUILocalPos(const FVector2D& In_uiPos)
{
	// minus by shift ui pos
	//FVector2D realPos = In_uiPos - _uiShiftMapPos;
	// divide by ui size/ real size rate
	FVector2D realPos = In_uiPos /_uiSceneImageRate;

	return realPos;
}
// ui 위치 -> 텍스쳐 위치
FVector2D FGsWorldMapManager::GetTexturePosFromRealUiPos(const FVector2D& In_uiPos)
{
	FVector2D texturePos = In_uiPos * _uiTextureRate;
	return texturePos;
}
// 로컬이 보여지는 지역맵에 있는지
bool FGsWorldMapManager::IsLocalInShowRegionMap()
{
	int currentLocalLevel = _currentLevelId;
	return (_showRegionMapId == currentLocalLevel) ? true : false;
}

FText FGsWorldMapManager::GetPortalName(int In_index)
{
	FGsPortalInfo* portalInfo =
		GetCurrentRegionPortalInfoByIndex(In_index);

	if (portalInfo == nullptr)
	{
		return FText::GetEmpty();
	}

	return portalInfo->_portalName;
}

// 최근 지역의 몬스터 정보 얻기(ui 인덱스로)
FGsRegionMonsterInfo* FGsWorldMapManager::GetCurrentRegionMonsterInfoByIndex(int In_index)
{
	FGsRegionInfo* info = GetCurrentRegionInfo();

	if (info == nullptr)
	{
		return nullptr;
	}

	if (info->_arrayMonsterInfo.Num() <= In_index)
	{
		return nullptr;
	}

	FGsRegionMonsterInfo* monsterInfo = info->_arrayMonsterInfo[In_index];
	return monsterInfo;
}
// 최근 지역의 포털 정보 얻기(인덱스로)
FGsPortalInfo* FGsWorldMapManager::GetCurrentRegionPortalInfoByIndex(int In_index)
{
	FGsPortalInfo* portalInfo = nullptr;

	// 침공전 넘어 갔는지
	bool isInvadeWorld = GGameData()->IsInvadeWorld();

	// 침공전 넘어갔을때는 별도 리스트 참조(마을 제외)
	// https://jira.com2us.com/jira/browse/CHR-23653

	if (_currentRegionInfo == nullptr)
	{
		return nullptr;
	}
	if (isInvadeWorld == false)
	{
		if (_currentRegionInfo->_arrayPortalInfo.Num() <= In_index)
		{
			return nullptr;
		}

		portalInfo =
			_currentRegionInfo->_arrayPortalInfo[In_index];
	}
	else
	{
		if (_currentRegionInfo->_arrayInvasionEnterPortalInfo.Num() <= In_index)
		{
			return nullptr;
		}

		portalInfo =
			_currentRegionInfo->_arrayInvasionEnterPortalInfo[In_index];
	}

	return portalInfo;
}
// 로컬 스폰 끝
void FGsWorldMapManager::OnLocalPlayerSpawnComplete(const IGsMessageParam* In_mapIdParam)
{
	const FGsPrimitiveInt32* param = In_mapIdParam->Cast<const FGsPrimitiveInt32>();
	_currentLevelId = param->_data;
}

// 현재 보고있는 지역맵이 던전인지
EGsWorldMapGroupType FGsWorldMapManager::GetShowRegionMapGroupType()
{
	const FGsSchemaMapData* mapTbl = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(_showRegionMapId);
	if (mapTbl == nullptr || nullptr == mapTbl->areaId.GetRow())
	{
		return EGsWorldMapGroupType::Field;
	}

	// 시련의 전당도 던전 처럼 표시
	// https://jira.com2us.com/jira/browse/C2URWQ-4377
	// 그림자 전장도 던전처럼 표시
	//https://jira.com2us.com/jira/browse/C2URWQ-7778
	if (mapTbl->mapType == MapType::PUBLIC_DUNGEON ||
	mapTbl->mapType == MapType::ELITE_DUNGEON_1||
		mapTbl->mapType == MapType::GUILD_WAR)
	{
		return EGsWorldMapGroupType::Dungeon;
	}
	else if (mapTbl->mapType == MapType::AGIT)
	{
		return EGsWorldMapGroupType::Agit;
	}

	return EGsWorldMapGroupType::Field;
}

void FGsWorldMapManager::SetShowRegionMapType(EGsRegionMapType In_type)
{	
	_showRegionMapType = In_type;
}

// 이벤트 시작
void FGsWorldMapManager::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	_currentProgressType = In_startType;

	// 이름변경 팝업 띄우기
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupWarp"));
	if (widget.IsValid() == false)
	{
		return;
	}
	UGsUIPopupWarp* popupWarp = Cast<UGsUIPopupWarp>(widget.Get());
	if (popupWarp == nullptr)
	{
		return;
	}

	if (In_startType == EGsEventProgressType::UI_POPUP_WARP_REGION_ITEM)
	{
		popupWarp->SetData(_reserveMoveTargetText,
			PopValidReservedCostPackage(),
			[this]()
			{
				if (_reservePopupWarpType == EGsWorldmapPopupWarpType::Bookmark)
				{
					FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
					if (nullptr == gameFlow)
					{
						return;
					}

					FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
					if (nullptr == contents)
					{
						return;
					}

					FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
					if (nullptr == hud)
					{
						return;
					}

					FGsBookmarkHandler* bookmarkHandler = hud->GetBookmarkHandler();

					if (bookmarkHandler == nullptr)
					{
						return;
					}
					bookmarkHandler->SendBookmarkTeleportUI(EGsBookmarkUIType::RegionMap,
						_reserveSelectIndex);
				}
				else if (_reservePopupWarpType == EGsWorldmapPopupWarpType::Portal)
				{
					// 해당 포털 위치로 텔레포트
					TryPortalWarp(_reserveSelectIndex);
				}
				// 침공전 포탈이면
				else if (_reservePopupWarpType == EGsWorldmapPopupWarpType::InvasionPortal)
				{
					TryInvasionPortalWarp(_reserveSpotId);
				}
				// main hud로 이동
				GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
				// 워프 요청했다.
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
				// region map window 닫기
				// 특정 ui 만 닫으면 그전에 있던 ui가 뜨므로 다닫기로 한다
				// ex)지역->대륙->지역에서 이동시
				GUI()->CloseAllStack();
			});
	}
	else if (In_startType == EGsEventProgressType::UI_POPUP_WARP_CONTINENTAL_DUNGEON)
	{
		popupWarp->SetData(_reserveMoveTargetText,
			PopValidReservedCostPackage(),
			[this]()
			{
				// 서버 전송
				int32 level = GGameData()->GetUserData()->mLevel;
				//QuestId questId = GSQuest()->GetCurrentMainQuestId();
				if (UGsLevelManager::IsCanSpotWarp(_reserveMapId, _reserveSpotId, level/*, questId*/))
				{
					FGsNetSendServiceWorld::SendPortalWarp(_reserveMapId, _reserveSpotId);

					// main hud로 이동
					GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
					// 워프 요청했다.
					GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
					// region map window 닫기
					// 특정 ui 만 닫으면 그전에 있던 ui가 뜨므로 다닫기로 한다
					// ex)지역->대륙->지역에서 이동시
					GUI()->CloseAllStack();
				}
			});
	}
	else if (In_startType == EGsEventProgressType::UI_POPUP_WARP_BOOKMARK_SHARE)
	{
		popupWarp->SetData(_reserveMoveTargetText,
			PopValidReservedCostPackage(),
			[this, popupWarp]()
			{
				int32 level = GGameData()->GetUserData()->mLevel;
				//QuestId questId = GSQuest()->GetCurrentMainQuestId();
				if (UGsLevelManager::IsCanMapWarp(_reserveMapId, level/*, questId*/))
				{
					FGsNetSendServiceWorld::SendSharedBookmarkWarp(
						_reserveMapId,
						_reserveSharePos, _reserveItemDBId,
						_reserveChannelId);

					popupWarp->Close();

					// 워프 요청했다.
					GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
				}				
			});
	}
	else if (In_startType == EGsEventProgressType::UI_POPUP_WARP_BOOKMARK_ITEM)
	{
		popupWarp->SetData(_reserveMoveTargetText,
			PopValidReservedCostPackage(),
			[this]()
			{
				FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
				if (nullptr == gameFlow)
				{
					return;
				}

				FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
				if (nullptr == contents)
				{
					return;
				}

				FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
				if (nullptr == hud)
				{
					return;
				}

				FGsBookmarkHandler* bookmarkHandler = hud->GetBookmarkHandler();

				if (bookmarkHandler == nullptr)
				{
					return;
				}
				bookmarkHandler->SendBookmarkTeleportUI(EGsBookmarkUIType::Bookmark, _reserveSelectIndex);

				TWeakObjectPtr<UGsUIWidgetBase> popup = GUI()->GetWidgetByKey(TEXT("PopupBookmark"));
				if (popup.IsValid() == false)
				{
					return;
				}

				popup->Close();

				// 워프 요청했다.
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
			});
	}
	else if (In_startType == EGsEventProgressType::UI_POPUP_WARP_MOVE_TO_PARTY_LEADER)
	{
		popupWarp->SetData(_reserveMoveTargetText,
			PopValidReservedCostPackage(),
			[this]()
			{

				// 시네마틱 플레이중이면 이동하지 않기
				// https://jira.com2us.com/jira/browse/C2URWQ-5782
				bool isSequencePlaying = UGsSequencePlayer::IsSequencePlaying();
				if (true == isSequencePlaying)
				{
					FText findText;
					FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
					FGsUIHelper::TrayMessageTicker(findText);
				}
				
				// 대화 연출 send 후 응답 대기중에는 워프 하지 말기
				bool isLockWarp = UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE);
				if (false == isLockWarp && false == isSequencePlaying )
				{
					// 서버에 요청
					FGsNetSendServiceWorld::SendWarpToPartyLeader();
				}
				
				// 워프 요청했다.
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
				// 다닫기
				GUI()->CloseAllStack();				
			});
	}
	else if (In_startType == EGsEventProgressType::UI_POPUP_WARP_REGION_MONSTER)
	{
		popupWarp->SetData(_reserveMoveTargetText,
			PopValidReservedCostPackage(),
			[this]()
			{
				if (_reservePopupWarpType == EGsWorldmapPopupWarpType::MonsterDrop)
				{
					// 해당 포털 위치로 텔레포트
					TryMonsterWarp(_reserveMapId, _reserveNpcId, _reserveSpawnGroupId);
				}
				// main hud로 이동
				GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
				// 워프 요청했다.
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
				// region map window 닫기
				// 특정 ui 만 닫으면 그전에 있던 ui가 뜨므로 다닫기로 한다
				// ex)지역->대륙->지역에서 이동시
				GUI()->CloseAllStack();
			});
	}
}
// 이벤트 종료
void FGsWorldMapManager::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	GUI()->CloseByKeyName(TEXT("PopupWarp"));
}

// 워프 시도
void FGsWorldMapManager::ReserveWarpRegionItem(
	EGsWorldmapPopupWarpType In_type, 
	FString In_moveTargetText,
	TSharedRef<FGsCostPackage> InCostPackage,
	int In_selectIndex)
{
	_reserveMoveTargetText = In_moveTargetText;
	_reservedCostPackage = MoveTemp(InCostPackage);
	_reservePopupWarpType = In_type;
	_reserveSelectIndex = In_selectIndex;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_WARP_REGION_ITEM, this);
}

// 워프 예약(침공 포털 아이콘)
void FGsWorldMapManager::ReserveWarpInvasionPortal(
	EGsWorldmapPopupWarpType In_type,
	FString In_moveTargetText,
	TSharedRef<FGsCostPackage> InCostPackage,
	int64 In_spotId)
{
	_reserveMoveTargetText = In_moveTargetText;
	_reservedCostPackage = MoveTemp(InCostPackage);
	_reservePopupWarpType = In_type;
	_reserveSpotId = In_spotId;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_WARP_REGION_ITEM, this);
}

// 워프 예약(지역 아이템)
void FGsWorldMapManager::ReserveWarpDungeonSide(
	FString In_moveTargetText,
	TSharedRef<FGsCostPackage> InCostPackage,
	int In_mapId, int64 In_spotId)
{
	_reserveMoveTargetText = In_moveTargetText;
	_reservedCostPackage = MoveTemp(InCostPackage);
	_reserveMapId = In_mapId;
	_reserveSpotId = In_spotId;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_WARP_CONTINENTAL_DUNGEON, this);
}

// 워프 예약(북마크 공유)
void FGsWorldMapManager::ReserveWarpBookmarkShare(
	FString In_moveTargetText,
	TSharedRef<FGsCostPackage> InCostPackage,
	int In_mapId, FVector In_pos, ItemDBId In_itemDBId, int In_channelId)
{
	_reserveMoveTargetText = In_moveTargetText;
	_reservedCostPackage = MoveTemp(InCostPackage);
	_reserveMapId = In_mapId;
	_reserveSharePos = In_pos;
	_reserveItemDBId = In_itemDBId;
	_reserveChannelId = In_channelId;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_WARP_BOOKMARK_SHARE, this);
}

// 워프 예약(지역 아이템)
void FGsWorldMapManager::ReserveWarpBookmarkItem(
	FString In_moveTargetText,
	TSharedRef<FGsCostPackage> InCostPackage,
	int In_selectIndex)
{
	_reserveMoveTargetText = In_moveTargetText;
	_reservedCostPackage = MoveTemp(InCostPackage);
	_reserveSelectIndex = In_selectIndex;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_WARP_BOOKMARK_ITEM, this);
}

// 워프 예약(파티 리더 따라가기)
void FGsWorldMapManager::ReserveWarpMoveToPartyLeader(
	FString In_moveTargetText,
	TSharedRef<FGsCostPackage> InCostPackage)
{
	_reserveMoveTargetText = In_moveTargetText;
	_reservedCostPackage = MoveTemp(InCostPackage);

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_WARP_MOVE_TO_PARTY_LEADER, this);
}

void FGsWorldMapManager::ReserveWarpRegionMonster(EGsWorldmapPopupWarpType In_type, FString In_moveTargetText, TSharedRef<FGsCostPackage> InCostPackage, MapId In_mapId, CreatureId In_npcId, SpawnGroupId In_spawnGroupId)
{
	// [B1] | ejrrb10 | 현재 유저가 침공전 진행중 / 인터서버라면 워프 팝업을 열어주지 않는다 (C2URWQ-4857)
	// 2023/8/29 PKT - https://jira.com2us.com/jira/browse/CHR-21412 인터서버는 활성화되어야 해서 인터서버 막았던것 풀었음.
	if (GGameData()->IsInvadeWorld())
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("GGameData()->IsInvadeWorld()"));
#endif
		// CHR-20745 | 침공전이나 인터서버일 경우 티커 출력 추가
		FText findText;
		FText::FindText(TEXT("WorldMapText"), TEXT("ErrorCannotmove"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return;
	}

	_reservePopupWarpType = In_type;
	_reserveMoveTargetText = In_moveTargetText;
	_reservedCostPackage = MoveTemp(InCostPackage);
	_reserveMapId = In_mapId;
	_reserveNpcId = In_npcId;
	_reserveSpawnGroupId = In_spawnGroupId;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_WARP_REGION_MONSTER, this);
}

TSharedRef<FGsCostPackage> FGsWorldMapManager::PopValidReservedCostPackage()
{
	if (0 == _reservedCostPackage->GetCostElementsCount())
	{
		// 이 에러가 발생하는 경우는, Cost Package 를 넘겼으나, 안에 아무런 비용도 없는 경우이므로, 확인이 필요함.
		// 항상 유효한 값을 내보내기 위해 아무것도 지정되어 있지 않은 경우라도 골드 0원으로 강제 설정하여 반환함.
		GSLOG(Error, TEXT("Unspecified Cost Status Notice. Forced Gold: Return to 0"));
		_reservedCostPackage->AddCost(CurrencyType::GOLD, 0);
	}
	
	return _reservedCostPackage;
}

FText FGsWorldMapManager::GetContinentalName(const ContinentId InContinentId) const
{
	FText ContinentalName;

	const FGsSchemaContinentalMapData* ContinentalMapData = UGsTableUtil::FindRowById<UGsTableContinentalMapData, FGsSchemaContinentalMapData>(InContinentId);
	if (nullptr == ContinentalMapData)
	{
		GSLOG(Error, TEXT("nullptr == ContinentalMapData"));
	}
	else
	{
		ContinentalName = ContinentalMapData->mapStringId;
	}

	return ContinentalName;
}

FText FGsWorldMapManager::GetMapName(const MapId InMapId)
{
	FText MapName;

	const FGsSchemaMapData* MapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(InMapId);
	if (nullptr == MapData)
	{
		GSLOG(Error, TEXT("nullptr == MapData, %d"), InMapId);
	}
	else
	{
		MapName = MapData->MapStringId;
	}
	

	return MapName;
}

MapType FGsWorldMapManager::GetMapType(const MapId InMapId)
{
	MapType MapName = MapType::NONE;

	const FGsSchemaMapData* MapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(InMapId);
	if (nullptr == MapData)
	{
		GSLOG(Error, TEXT("nullptr == MapData, %d"), InMapId);
	}
	else
	{
		MapName = MapData->mapType;
	}

	return MapName;
}

void FGsWorldMapManager::SetTreasureMonsterMapInfo(int64 seperator, MapId id, FVector pos, bool localPlayerOwn /*= false*/)
{
	bool isUpdate = _treasureMonsterMapInfo.Find(seperator) != nullptr && _treasureMonsterMapInfo[seperator]._posMapId == id;

	if(!isUpdate && _treasureMonsterMapInfo.Find(seperator) != nullptr)
		RemoveTreasureMonsterMapInfo(seperator);
	
	_treasureMonsterMapInfo.Emplace(seperator, FGsTreasureMonsterMapInfo{ seperator, id, pos, localPlayerOwn});

	FGsMapTreasureMonsterParam param(true, seperator);
	if (isUpdate)
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::REGIONMAP_UPDATE_TREASUREMONSTER_ICON, &param);
	}
	else
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::REGIONMAP_SPAWN_TREASUREMONSTER_ICON, &param);
	}
}

void FGsWorldMapManager::RemoveTreasureMonsterMapInfo(int64 seperator)
{
	_treasureMonsterMapInfo.Remove(seperator);

	FGsMapTreasureMonsterParam param(false, seperator);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::REGIONMAP_SPAWN_TREASUREMONSTER_ICON, &param);
}

const FGsTreasureMonsterMapInfo& FGsWorldMapManager::GetTreasureMonsterMapInfo(int64 seperator, int mapId)
{
	static FGsTreasureMonsterMapInfo noTreasureMonster{ 0, 0, FVector::ZeroVector, false };

	if (seperator <= 0) return noTreasureMonster;

	if (auto iter = _treasureMonsterMapInfo.Find(seperator))
	{
		if(mapId == 0 || iter->_posMapId == mapId) return (*iter);
	}

	return noTreasureMonster;
}

int FGsWorldMapManager::GetShowRegionTreasureMonsterInfo(TArray<FGsTreasureMonsterMapInfo>& infoList)
{
	auto showRegionMapId = GetShowRegionMapId();

	for (const auto& e : _treasureMonsterMapInfo)
	{
		if (e.Value._posMapId == showRegionMapId)
		{
			infoList.Add(e.Value);
		}
	}
	return infoList.Num();
}

bool FGsWorldMapManager::GetRegionMapIsBackkeyOpen()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = 
		GUI()->GetWidgetByKey(TEXT("WindowRegionMap"));

	if (widget.IsValid() == false)
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWindowRegionMap> regionMapWindow =  Cast<UGsUIWindowRegionMap>(widget.Get());
	if (regionMapWindow.IsValid() == false)
	{
		return false;
	}

	bool backKeyOpen = regionMapWindow->IsStackRecoverFlag();

	return backKeyOpen && _isUIRegionMapConstruct == true;
}

void FGsWorldMapManager::SaveServerOptionRegionMapFilterInfo()
{
	WorldMapFilterInfo& optionData = GServerOption()->GetWorldMapFilter();

	_oldRegionMapFilterServerOption.mParty = optionData.mParty;
	_oldRegionMapFilterServerOption.mTeleportSpot = optionData.mTeleportSpot;
	_oldRegionMapFilterServerOption.mBookMark = optionData.mBookMark;
	_oldRegionMapFilterServerOption.mSpaceCrackPortal = optionData.mSpaceCrackPortal;
	_oldRegionMapFilterServerOption.mFieldBoss = optionData.mFieldBoss;
	_oldRegionMapFilterServerOption.mSanctum = optionData.mSanctum;
}

bool FGsWorldMapManager::IsChangedServerOptionRegionMapFilterInfo()
{
	WorldMapFilterInfo& optionData = GServerOption()->GetWorldMapFilter();

	if (_oldRegionMapFilterServerOption.mParty != optionData.mParty ||
		_oldRegionMapFilterServerOption.mTeleportSpot != optionData.mTeleportSpot ||
		_oldRegionMapFilterServerOption.mBookMark != optionData.mBookMark ||
		_oldRegionMapFilterServerOption.mSpaceCrackPortal != optionData.mSpaceCrackPortal ||
		_oldRegionMapFilterServerOption.mFieldBoss != optionData.mFieldBoss ||
		_oldRegionMapFilterServerOption.mSanctum != optionData.mSanctum)
	{
		return true;
	}
	return false;
}

FGsRegionGroupInfoBase* FGsWorldMapManager::GetSpaceCrackGroupData(int In_condinentalId)
{
	return _mapSpaceCrackInfo.FindRef(In_condinentalId);
}

FGsContinentalSpaceCrackMapInfo* FGsWorldMapManager::FindSpaceCrackSortInfoByIndex(int In_index)
{
	if (In_index > _arraySpaceCrackSortInfo.Num() - 1)
	{
		return nullptr;
	}

	return _arraySpaceCrackSortInfo[In_index];
}

void FGsWorldMapManager::SetRegionInvadeInfoByServerPacket(PD::SC::PKT_SC_INVADE_MINIMAP_INFO_UPDATE_READ* In_serverInvaeInfo)
{
	TArray<EGsRegionInvadeNPCType> arrayAddType;
	TArray<EGsRegionInvadeNPCType> arrayDelType;

	int64 guardianGameId = In_serverInvaeInfo->GuardianGameId();
	FVector guardianPos = In_serverInvaeInfo->GuardianPos();

	EGsRegionInvadeNPCNewInfoResponse responseGuardian =
		SetRegionInvadeInfo(EGsRegionInvadeNPCType::Guardian, guardianGameId, guardianPos);

	if (responseGuardian == EGsRegionInvadeNPCNewInfoResponse::Del)
	{
		arrayDelType.Add(EGsRegionInvadeNPCType::Guardian);
	}
	else if(responseGuardian == EGsRegionInvadeNPCNewInfoResponse::Add)
	{
		arrayAddType.Add(EGsRegionInvadeNPCType::Guardian);
	}

	int64 monsterGameId = In_serverInvaeInfo->MonsterGameId();
	FVector monsterPos = In_serverInvaeInfo->MonsterPos();

	EGsRegionInvadeNPCNewInfoResponse responseMonster = 
		SetRegionInvadeInfo(EGsRegionInvadeNPCType::Monster, monsterGameId, monsterPos);

	if (responseMonster == EGsRegionInvadeNPCNewInfoResponse::Del)
	{
		arrayDelType.Add(EGsRegionInvadeNPCType::Monster);
	}
	else if (responseMonster == EGsRegionInvadeNPCNewInfoResponse::Add)
	{
		arrayAddType.Add(EGsRegionInvadeNPCType::Monster);
	}

	int64 wagonGameId = In_serverInvaeInfo->WagonGameId();
	FVector wagonPos = In_serverInvaeInfo->WagonPos();

	EGsRegionInvadeNPCNewInfoResponse responseWagon =
		SetRegionInvadeInfo(EGsRegionInvadeNPCType::Wagon, wagonGameId, wagonPos);

	if (responseWagon == EGsRegionInvadeNPCNewInfoResponse::Del)
	{
		arrayDelType.Add(EGsRegionInvadeNPCType::Wagon);
	}
	else if (responseWagon == EGsRegionInvadeNPCNewInfoResponse::Add)
	{
		arrayAddType.Add(EGsRegionInvadeNPCType::Wagon);
	}

	if (arrayAddType.Num() != 0)
	{
		FGsMapInvasionNPCParam param(true
			, arrayAddType);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_INVASION_NPC, &param);
	}

	if (arrayDelType.Num() != 0)
	{
		FGsMapInvasionNPCParam param(false
			, arrayDelType);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_INVASION_NPC, &param);
	}

}

EGsRegionInvadeNPCNewInfoResponse FGsWorldMapManager::SetRegionInvadeInfo(EGsRegionInvadeNPCType In_type, int64 In_gameId, FVector& In_pos)
{
	EGsRegionInvadeNPCNewInfoResponse response = EGsRegionInvadeNPCNewInfoResponse::None;
	// game id 0: delete
	if (In_gameId == 0)
	{
		FGsRegionInvadeIconInfo** findInfo = _mapRegionInvadeIconInfo.Find(In_type);
		if (findInfo != nullptr)
		{
			delete *findInfo;
			_mapRegionInvadeIconInfo.Remove(In_type);

			response = EGsRegionInvadeNPCNewInfoResponse::Del;
		}
	}
	// add or update
	else
	{
		FGsRegionInvadeIconInfo** findInfo = _mapRegionInvadeIconInfo.Find(In_type);
		// already exist
		if (findInfo != nullptr)
		{
			(*findInfo)->_gameId = In_gameId;
			(*findInfo)->_pos = In_pos;

			response = EGsRegionInvadeNPCNewInfoResponse::Update;
		}
		// not exist
		else
		{
			FGsRegionInvadeIconInfo* newData = new FGsRegionInvadeIconInfo();

			newData->_gameId = In_gameId;
			newData->_NPCType = In_type;
			newData->_pos = In_pos;

			_mapRegionInvadeIconInfo.Add(In_type, newData);

			response = EGsRegionInvadeNPCNewInfoResponse::Add;
		}
	}

	return response;
}

FGsRegionInvadeIconInfo* FGsWorldMapManager::GetRegionInvadeInfoByNPCType(EGsRegionInvadeNPCType In_NPCType)
{
	return _mapRegionInvadeIconInfo.FindRef(In_NPCType);
}

// load to po start(level load server call)
void FGsWorldMapManager::OnLoadTopoStart(const IGsMessageParam* inParam)
{
	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
	{
		return;
	}

	// if goto other map
	// clear current map data
	if (levelMgr->IsWarpDataDifferentMap() == true)
	{
		ClearRegionInvadeIconInfo();
	}
}

void FGsWorldMapManager::ClearRegionInvadeIconInfo()
{
	if (_mapRegionInvadeIconInfo.Num() != 0)
	{
		for (auto iconInfo : _mapRegionInvadeIconInfo)
		{
			if (nullptr != iconInfo.Value)
			{
				delete iconInfo.Value;
			}
		}
		_mapRegionInvadeIconInfo.Empty();
	}
}

// invasion finish
void FGsWorldMapManager::OnInvasionFinish(const IGsMessageParam*)
{
	// clear data
	ClearRegionInvadeIconInfo();

	// send message
	TArray<EGsRegionInvadeNPCType> arrayDelType{ 
		EGsRegionInvadeNPCType::Guardian,
		EGsRegionInvadeNPCType::Monster,
		EGsRegionInvadeNPCType::Wagon
	};
	FGsMapInvasionNPCParam param(false
		, arrayDelType);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_INVASION_NPC, &param);
}

// https://jira.com2us.com/jira/browse/C2URWQ-4984
// 대륙맵에 던전 입구로 이동할때 세팅
// 던전 획득저에서 사용
void FGsWorldMapManager::SetDungeonSideShow(int In_mapId)
{
	// map id 저장
	_showRegionMapId = In_mapId;

	int dungeonGroupId = FindDungeonGroupTblIdByMapId(_showRegionMapId);

	if (dungeonGroupId == -1)
	{
		return;
	}

	FGsSchemaSpotInfo findSpotInfo;
	int mapId = 0;
	if (false == FGsMapFunc::GetDungeonSpotInfo(dungeonGroupId, findSpotInfo, mapId))
	{
		return;
	}

	SetDungeonEntranceInfo(dungeonGroupId, mapId,
		findSpotInfo.pos, findSpotInfo.id);

	// 던전 메뉴 보이게
	SetIsShowDungeonMenu(true);

	// 대륙맵으로 변경
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_CONTINENTALMAP);
}

// 지역맵 game state 가져오기
FGsGameStateRegionMap* FGsWorldMapManager::GetGameStateRegionMap()
{
	if (GMode() == nullptr)
	{
		return nullptr;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return nullptr;
	}
	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();		
	if (contents == nullptr)
	{
		return nullptr;	
	}

	FGsGameStateRegionMap* regionMap =
		contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap);

	if (regionMap == nullptr)
	{
		return nullptr;
	}
	return regionMap;	
}

// 현재 열린 맵의 지역맵 성소 정보 가져오기(성소 id로)
FGsRegionMapSanctumInfo* FGsWorldMapManager::GetCurrentRegionSanctumInfoBySanctumId(SanctumAreaId In_id)
{
	FGsRegionInfo* info = GetCurrentRegionInfo();

	if (info == nullptr)
	{
		return nullptr;
	}

	if (info->_arrayRegionMapSanctumInfo.Num() == 0)
	{
		return nullptr;
	}
	FGsRegionMapSanctumInfo* findInfo = nullptr;
	for (auto iter: info->_arrayRegionMapSanctumInfo)
	{
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->_sanctumAreaId == In_id)
		{
			findInfo = iter;
			break;
		}

	}
	return findInfo;
}

// 인덱스로 몬스터 정보 세팅
void FGsWorldMapManager::SetSelectMonsterInfoByIndex(int In_selectIndex)
{
	FGsRegionMonsterInfo* monsterInfo =
		GetCurrentRegionMonsterInfoByIndex(In_selectIndex);
	if (nullptr == monsterInfo || monsterInfo->_npcData == nullptr)
	{
		return;
	}

	_selectInfoPopupMonsterId = monsterInfo->_npcData->id;
	_selectInfoPopupMonsterLevel = monsterInfo->level;
}

// 현재 선택된 몬스터 레벨 인덱스로 구하기
int FGsWorldMapManager::GetCurrentSelectMonsterLevelByLevelIndex(int In_levelIndex)
{
	FGsRegionInfo* info = GetCurrentRegionInfo();
	if (info == nullptr)
	{
		return -1;
	}

	FGsRegionMonsterSameIDInfo* findInfo = info->_mapMonsterSameIdInfo.FindRef(_selectInfoPopupMonsterId);
	if (findInfo == nullptr)
	{
		return -1;
	}

	if (findInfo->_arrayLevel.Num() <= In_levelIndex || 0 > In_levelIndex)
	{
		return -1;
	}

	return findInfo->_arrayLevel[In_levelIndex];
}

// 현재 선택된 몬스터(npc tbl id) + 선택된 레벨로 
// 몬스터 인덱스(지역맵 정보 인덱스) 구하기
int FGsWorldMapManager::GetCurrentSelectMonsterIndexByLevelIndex(int In_levelIndex)
{
	FGsRegionInfo* info = GetCurrentRegionInfo();
	if (info == nullptr)
	{
		return -1;
	}

	int mapId = info->_mapId;

	FGsGameStateRegionMap* stateRegionMap = GetGameStateRegionMap();
	if (stateRegionMap == nullptr)
	{
		return -1;
	}
	// 레벨 인덱스로 레벨 구하기
	int selectLevel = GetCurrentSelectMonsterLevelByLevelIndex(In_levelIndex);
	// mapid, 몬스터 id, 몬스터 레벨로 몬스터 인덱스(지역맵 데이터) 구하기
	int regionMapMonsterIndex = 
		stateRegionMap->FindMonsterIndexByIdAndLevel(mapId, _selectInfoPopupMonsterId, selectLevel);

	return regionMapMonsterIndex;
}

// 현재 몬스터 id 기반 정보(몬스터 팝업에서 사용)
FGsRegionMonsterSameIDInfo* FGsWorldMapManager::GetCurrentMonsterSameIdInfo()
{
	FGsRegionInfo* info = GetCurrentRegionInfo();
	if (info == nullptr)
	{
		return nullptr;
	}

	FGsRegionMonsterSameIDInfo* findInfo = info->_mapMonsterSameIdInfo.FindRef(_selectInfoPopupMonsterId);
	if (findInfo == nullptr)
	{
		return nullptr;
	}

	return findInfo;
}

// 현재 보여지는 맵타입
MapType FGsWorldMapManager::GetShowRegionMapTableType()
{
	MapType findMapType = GetMapType(_showRegionMapId);
	return findMapType;
}