
#include "GsInterServerManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"

#include "User/GsSchemaUserData.h"
#include "DataSchema/InterServer/GsSchemaInvadeWorldTargetData.h"
#include "DataSchema/InterServer/GsSchemaInvadeWorldConfigDataEx.h"

#include "Net/GsNetSendServiceWorld.h"

#include "UTIL/GsCronUtil.h"
#include "GsTable.h"

#include "Algo/Transform.h"
#include "Hercules_App.h"

//static bool reqPacketSend = false;

void UGsInterServerManager::Initialize()
{
	_initMapData = false;
	_invadeRunningMinTime = 0;
	_invasionStartTime = FDateTime(0);

	if (FGsMessageHolder* msg = GMessage())
	{
		_handleUserData = msg->GetGameObject().AddUObject(MessageGameObject::UPDATE_USER_DATA, this, &UGsInterServerManager::OnUserBasicDataChange);
	}
}

void UGsInterServerManager::Finalize()
{	
	if (FGsMessageHolder* msg = GMessage())
	{
		msg->GetGameObject().Remove(_handleUserData);
	}

	_invasionMapData.Reset(0);
	IsValidInvasionTime(true);
}

void UGsInterServerManager::Update(float inTick)
{	
	if (GSGameObject()->GetIsSpawnMeComplete())
	{
		if (_initMapData)
		{
			IsValidInvasionTimeInMyWorld();
		}
	}
}

bool UGsInterServerManager::IsValidInvasionMap(MapId mapId)
{
	auto InvasionState = GGameData()->IsInvadeWorld() ? InvasionState::Invade :
		IsMapInInvasionTime(mapId) ? InvasionState::Go : IsValidInvasionTime() ? InvasionState::InTimeNoGo : InvasionState::NoGo;

	bool isInvadableMap = GetInvadeMapData(mapId).IsValid();

	return InvasionState == InvasionState::Go && isInvadableMap;
}

bool UGsInterServerManager::IsValidInvasionTime(bool reset /*= false */)
{
	static FDateTime enableInvasionTime = FDateTime(0);
	static FDateTime disableInvationTime = FDateTime(0);

	static bool checkTime = false;
	static auto lastDay = FGsTimeSyncUtil::GetServerNowDateTime().GetDay();

	static auto invadeTimeCheck = [](FDateTime svrTime) {
		return svrTime >= enableInvasionTime && svrTime < disableInvationTime;
	};

	auto curTime = FGsTimeSyncUtil::GetServerNowDateTime();

	if (disableInvationTime.GetTicks() != 0 && disableInvationTime <= curTime) reset = true; //setTime 시 다시 읽도록

	//if (enableInvasionTime.GetTicks() != 0 && checkTime != invadeTimeCheck(curTime))
	//{
	//	reset = true;
	//}

	if (_invasionStartTime != enableInvasionTime && _invasionStartTime.GetTicks() != 0) //server에서 start를 내리면 무조건 start로 변경
	{
		lastDay = curTime.GetDay();

		if (_invadeRunningMinTime == 0)
		{
			LoadInvadeMapData();
		}

		reset = false;
		enableInvasionTime = _invasionStartTime;
		disableInvationTime = enableInvasionTime + FTimespan(0, _invadeRunningMinTime, 0);
		checkTime = !_isInvasionTime; // invadeTimeCheck(curTime);
	}

	if (reset)
	{
		_invasionStartTime = FDateTime(0);
		enableInvasionTime = FDateTime(0);
		disableInvationTime = FDateTime(0);
		return false;
	}

	if (enableInvasionTime.GetTicks() == 0 || curTime.GetDay() != lastDay) // 하루가 지났을 때
	{
		lastDay = curTime.GetDay();

		FString invadeStartCron;
		if (const auto table = Cast<UGsTableInvadeWorldConfigDataEx>(FGsSchemaInvadeWorldConfigDataEx::GetStaticTable()))
		{
			TArray<const FGsSchemaInvadeWorldConfigDataEx*> invasionWorldConfigs;
			table->GetAllRows(invasionWorldConfigs);

			if (invasionWorldConfigs.IsValidIndex(0))
			{
				const auto invasionWorldConfig = invasionWorldConfigs[0];
				auto runTime = _invadeRunningMinTime = invasionWorldConfig->InvadePlayTimeMinute;

				invadeStartCron = invadeStartCron.IsEmpty() ? invasionWorldConfig->invadeStartCron : invadeStartCron;

				FTimespan checkRange(0, runTime, 0);
				enableInvasionTime = FGsCronUtil::GetValidDateTime(invadeStartCron, curTime, checkRange);

				if (enableInvasionTime + FTimespan(0, runTime, 0) <= curTime)
				{
					curTime = curTime + FTimespan(0, 1, 0);
					enableInvasionTime = FGsCronUtil::GetValidDateTime(invadeStartCron, curTime, checkRange);
				}

				_invasionStartTime = enableInvasionTime;
				disableInvationTime = enableInvasionTime + FTimespan(0, runTime, 0);

				for (auto invasionMap : _invasionMapData)
				{
					invasionMap->SetInvasionEndTImeInMap(enableInvasionTime + FTimespan(0, runTime, 0));
				}
			}
		}
	}

	//_isInvasionTime = invadeTimeCheck(curTime); // TEST 빠질 예정

	if (_isInvasionTime != checkTime)  
	{
		checkTime = _isInvasionTime;

		FGsUIMsgParamBool param(_isInvasionTime);
		GMessage()->GetUI().SendMessage(MessageUI::INVADE_INVASIONTIME_UPDATE, &param); //미니맵 uo
	}

	return _isInvasionTime; 
}

bool UGsInterServerManager::IsValidInvasionTimeInMyWorld()
{	
	return !GGameData()->IsInvadeWorld() && IsValidInvasionTime();
}

FTimespan UGsInterServerManager::GetInvasionRemainStayTime()
{		
	if (IsInvasionState())
	{	
		if (auto mapInfo = _curInvasionMapInfo.Pin()) //내가 침공했던 맵의 정보
		{
			auto curTime = FGsTimeSyncUtil::GetServerNowDateTime();
			auto portalEndTIme = mapInfo->GetInvasionEndTime();

			if(portalEndTIme >= curTime)
			{
				return portalEndTIme - curTime;
			}
		}
	}
	return FTimespan::Zero();
}

TWeakPtr<FGsInvadeMap> UGsInterServerManager::GetInvadeMapData(MapId mapId)
{
	if (auto iter = _invasionMapData.FindByPredicate([mapId](auto e) { return e->_id == mapId; }))
	{
		FDateTime endTime = (*iter)->GetInvasionEndTime();
		bool overEndTime = endTime != 0 && (endTime <= FGsTimeSyncUtil::GetServerNowDateTime());

		if ((endTime <= FGsTimeSyncUtil::GetServerNowDateTime()) || !IsValidInvasionTimeInMyWorld())
		{
			(*iter)->_activePortal = false;
		}
		return *iter;
	}
	return nullptr;
}

int32 UGsInterServerManager::GetTotalInvaderCount()
{
	return _currentInvaderCount;
}


TWeakPtr<FGsMatchingWorld> UGsInterServerManager::GetAirRaidWorld()
{
	auto myWorldId = GGameData()->GetUserData()->mHomeWorldId;
	if (auto iter = _invasionWorldData.Find(myWorldId))
	{
		if (auto airRadeWorld = (*iter).FindByPredicate([this](auto e) {
			return IsValidInvasionTimeInMyWorld();
			}))
		{
			return *airRadeWorld;
		}
	}
	return nullptr;
}

int32 UGsInterServerManager::GetAirRaidMapList(OUT TArray<TWeakPtr<FGsInvadeMap>>& mapList)
{
	mapList.Append(_invasionMapData.FilterByPredicate([this](auto mapData) {
		return mapData && mapData->_enrolledSpot.Num() > 0 && IsMapInInvasionTime(mapData->_id); }));

	if(_curInvasionMapInfo != nullptr && GGameData()->IsInvadeWorld()) 
		mapList.Add(_curInvasionMapInfo);

	return mapList.Num();
}

int32 UGsInterServerManager::GetActiveInvasionPortalMapList(OUT TArray<TWeakPtr<FGsInvadeMap>>& mapList)
{
	mapList.Append(_invasionMapData.FilterByPredicate([this](auto mapData) {
		return mapData && IsMapInInvasionTime(mapData->_id) && mapData->_enrolledSpot.Num() > 0; }));
	return mapList.Num();
}

void UGsInterServerManager::SetCurrentMap(MapId mapID)
{
	if (GGameData()->IsInvadeWorld() == false)
	{
		_isInInvadeMap = false;
		_curInvasionMapInfo = nullptr;

		if (auto data = GetInvadeMapData(mapID).Pin())
		{
			_isInInvadeMap = true;
			_curInvasionMapInfo = data;
		}
	}
}

FDateTime UGsInterServerManager::GetNextInvasionEnableTime()
{
	if (IsValidInvasionTimeInMyWorld() == false)
	{
		return _invasionStartTime;
	}
	return FDateTime(0);
}

void UGsInterServerManager::EnterInvasionState(time_t invadeTime, int remainSec, int addStaySec)
{
	auto invasionMap = GLevel()->GetCurrentLevelId();

	_invasionInfo = MakeShared<InvasionInfo>(InvasionInfo{ invasionMap, 0, 0, 0 });
	auto enterTime = _invasionInfo->_invasionEnterTime = FGsTimeSyncUtil::ConvertToDateTime(invadeTime);
	_invasionInfo->_invasionOffset = FGsTimeSyncUtil::GetServerNowDateTime() - enterTime;

	FGsUIMsgParamInvadeState param(FGsUIMsgParamInvadeState::InvadeState::Start,
		invasionMap, enterTime, FDateTime(0), remainSec, addStaySec);

	GMessage()->GetUI().SendMessage(MessageUI::INVADE_AIRRAID_STATE, &param);

	SetInvasionStayTime(remainSec, addStaySec);
}

void UGsInterServerManager::LeaveInvasionState(time_t invadeEndTime, int remainSec, int addStaySec)
{
	if (_invasionInfo)
	{
		auto leaveTime = _invasionInfo->_invasionLeaveTime = FGsTimeSyncUtil::ConvertToDateTime(invadeEndTime);

		FGsUIMsgParamInvadeState param(FGsUIMsgParamInvadeState::InvadeState::End,
			_invasionInfo->_mapId, _invasionInfo->_invasionEnterTime, leaveTime, remainSec, addStaySec);
	
		GMessage()->GetUI().SendMessage(MessageUI::INVADE_AIRRAID_STATE, &param);
	}

	_invasionInfo = nullptr;
}

void UGsInterServerManager::SetInvasionStayTime(int remainSec, int addStaySec, int playerKillPoint /* 0 */)
{
	_remainInvasionStaySec = remainSec + addStaySec;
	if (IsInvasionState() && _invasionInfo)
	{	
		_invasionInfo->_invasionFinishStayTime = _invasionInfo->_invasionEnterTime + FTimespan(0, 0, remainSec + addStaySec);
		_invasionInfo->_playerKillPoint = playerKillPoint;
	}
}

bool UGsInterServerManager::ReqInvasionInfoCheck(bool isCheck)
{
	if (isCheck)
	{
		_mapUpdateCount = 0;
		if (IsValidInvasionTimeInMyWorld())
		{	
			FGsNetSendServiceWorld::Send_ReqInvadeInfo();
		}

		if (_invasionInfoTimeHandle.IsValid() == false)
		{
			GetWorld()->GetTimerManager().SetTimer(_invasionInfoTimeHandle, FTimerDelegate::CreateWeakLambda(this, [this]() {
					if (IsValidInvasionTimeInMyWorld()) FGsNetSendServiceWorld::Send_ReqInvadeInfo();
				}), 1.f, true, 0.f);
		}
		return true;
	}
	else 
	{	
		GetWorld()->GetTimerManager().ClearTimer(_invasionInfoTimeHandle);
	}
	return false;
}

void UGsInterServerManager::SetMapInvasionInfoList(const TArray<TPair<MapId, int>>& mapInfoList)
{
	TArray<MapId> updateMapInfo;
	for (auto e : mapInfoList)
	{
		if (auto data = GetInvadeMapData(e.Key).Pin())
		{
			data->SetInvaderCount(e.Value);
			updateMapInfo.Add(e.Key);
		}
	}
	_mapUpdateCount++;
	FGsUIMsgParamInvadeMapInfo param(updateMapInfo, _mapUpdateCount);
	GMessage()->GetUI().SendMessage(MessageUI::INVADE_INVADER_COUNT, &param);
}

void UGsInterServerManager::ReqInvasionPortalInfo()
{
	bool isActive = Algo::AllOf(_invasionMapData, [](auto e) { return e->_activePortal; });

	if (IsValidInvasionTime() != _isInvasionTime)
	{
		if (_isInvasionTime == false)
		{
			Algo::ForEach(_invasionMapData, [](auto e) { e->MapInfoReset(); });
		}
	}

	GMessage()->GetInvasion().SendMessage(MessageInvasion::INVALID_INVASION_ACTIVEPORTAL, nullptr);
}

void UGsInterServerManager::SetInvaderTotalCount(int invaderCount)
{
	_currentInvaderCount = invaderCount;
}

// 사용하지 않는다 23Y10M4W 에 등록 절차 제거
//void UGsInterServerManager::ReqActiveInvasionPortalList()
//{
//	FGsNetSendServiceWorld::Send_ReqActiveInvasionPortalList();
//}

// 사용하지 않는다 23Y10M4W 에 등록 절차 제거
void UGsInterServerManager::SetActiveInvasionPortalInMap(TArray<ActiveInvadePortalInfo>& activePortalInMaps)
{
	if (IsValidInvasionTimeInMyWorld())
	{
		Algo::ForEach(_invasionMapData, [](auto e) { e->MapInfoReset(); });
	}
	else
	{
		Algo::ForEachIf(_invasionMapData, [](auto e) {
			return (e->GetInvasionEndTime() <= FGsTimeSyncUtil::GetServerNowDateTime());
			}, [](auto e) { e->MapInfoReset(); });
	}

	GMessage()->GetInvasion().SendMessage(MessageInvasion::INVALID_INVASION_ACTIVEPORTAL, nullptr);
	GMessage()->GetUI().SendMessage(MessageUI::INVADE_PORTAL_SPAWNSPOT_UPDATE, nullptr);
}

//void UGsInterServerManager::NotReadyInvasionPortalSpot()
//{	
//	_worldMapUpdate = false;
//	reqPacketSend = false;
//
//	if (_invasionPortalTimeHandle.IsValid())
//	{
//		GetWorld()->GetTimerManager().ClearTimer(_invasionPortalTimeHandle);
//	}
//}

void UGsInterServerManager::SetInvasionStartTime(time_t startTime /*= 0*/)
{
	_isInvasionTime = true;

	if(startTime != 0) _invasionStartTime = FGsTimeSyncUtil::ConvertToDateTime(startTime);
	IsValidInvasionTime(true);

	UpdateInvasionInfo();
}

void UGsInterServerManager::SetInvasionEndTime()
{
	_isInvasionTime = false;

	IsValidInvasionTime(true);

	UpdateInvasionInfo();
}

void UGsInterServerManager::ResetSpawnedInvasionPortal(bool reset/*= false*/)
{
	if (reset)
	{
		for (auto mapData : _invasionMapData)
		{
			mapData->_activePortal = false;
			mapData->_enrolledSpot.Reset();
			mapData->_spawnPortals.Reset();
		}
	}
	else
	{
		if (Algo::AnyOf(_invasionMapData, [](auto e) { return e->_activePortal; }))
		{
			SetInvasionStartTime();

			GMessage()->GetInvasion().SendMessage(MessageInvasion::INVALID_INVASION_ACTIVEPORTAL, nullptr);
			GMessage()->GetUI().SendMessage(MessageUI::INVADE_PORTAL_SPAWNSPOT_UPDATE, nullptr);
		}
		else
		{
			SetInvasionEndTime();
		}
	}
}

void UGsInterServerManager::SetInvasionPortalSpotListInMap(MapId mapId, const TMap<SpotId, bool>& spotInfo)
{	
	if (auto mapData = GetInvadeMapData(mapId).Pin())
	{
		FString spotKey = FString::FromInt(mapId);
		FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(spotKey);
		if (const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), tableName))
		{
			const FGsSchemaSpotData* spotData;
			if (table->FindRow<FGsSchemaSpotData>(*spotKey, spotData))
			{
				TArray<SpotId> list;
				spotInfo.GenerateKeyArray(list);
				//spotInfo
				TArray<FGsSchemaSpotInfo> spawnList = spotData->SpotList.FilterByPredicate([&list](auto& e) { return list.Find(e.id) != INDEX_NONE; });

				mapData->_activePortal = spawnList.Num() > 0;
				mapData->_enrolledSpot.Reset();
				mapData->_spawnPortals.Reset();
				mapData->_spawnPortals.Append(spawnList);
				
				Algo::Transform(spotInfo, mapData->_enrolledSpot, [](auto& e) { return e.Key; });
			}
		}
	}
}

bool UGsInterServerManager::IsMapInInvasionTime(MapId mapId)
{
	if (IsValidInvasionTimeInMyWorld())
	{
		if (auto data = GetInvadeMapData(mapId).Pin())
		{
			auto curTime = FGsTimeSyncUtil::GetServerNowDateTime();
			auto portalEndTIme = data->GetInvasionEndTime();

			return portalEndTIme >= curTime;
		}
	}
	return false;
}

void UGsInterServerManager::OnNotiOninvasionTime()
{
	GUI()->OpenAndGetWidget(TEXT("TrayTickerInvasionNoti"));
}

void UGsInterServerManager::LoadInvadeMapData()
{
	if (const auto table = Cast<UGsTableInvadeMapData>(FGsSchemaInvadeMapData::GetStaticTable()))
	{
		TArray<const FGsSchemaInvadeMapData*> data;
		table->GetAllRows<FGsSchemaInvadeMapData>(data);

		for (const auto e : data)
		{
			_invasionMapData.Add(MakeShared<FGsInvadeMap>(e));
		}
	}
}

void UGsInterServerManager::LoadInvadeWorldData()
{
	uint64 planetId = CHerculesPrefs::GetInt(MyPlanet, 0);

	_invasionWorldData.Reset();
	if (const auto worldTable = Cast<UGsTableWorldData>(FGsSchemaWorldData::GetStaticTable()))
	{
		if (auto table = FGsSchemaInvadeWorldTargetData::GetStaticTable())
		{
			TArray<const FGsSchemaInvadeWorldTargetData*> rows;
			if (table->GetAllRows(rows))
			{
				for (auto e : rows)
				{
					if (auto myPlanet = e->worldMatching.FindByPredicate([planetId](auto& data) { return data.planetId == planetId; }))
					{
						for (auto& matchInfo : myPlanet->worldMatchingData)
						{
							if (matchInfo.worldId.GetRow() == nullptr) continue;

							auto key = matchInfo.worldId.GetRow()->worldId;

							const FGsSchemaWorldData* worldData = nullptr;
							WorldId targetWorldId = matchInfo.targetWorldId.GetRow() ? matchInfo.targetWorldId.GetRow()->worldId : 0;

							if (targetWorldId > 0 && worldTable->FindRowById(targetWorldId, worldData))
							{
								_invasionWorldData.FindOrAdd(key).Add(MakeShared<FGsMatchingWorld>(targetWorldId, worldData->worldName));
							}
						}
					}
				}
			}
		}
	}	
}

void UGsInterServerManager::UpdateInvasionInfo()
{
	//현재 맵을 기준으로 바꾼다
	auto curTime = FGsTimeSyncUtil::GetServerNowDateTime();
	auto curInvasionTime = IsValidInvasionTimeInMyWorld();
	bool isInvasionMap = curInvasionTime &&
		_curInvasionMapInfo.IsValid() && _curInvasionMapInfo.Pin()->GetInvasionEndTime() >= curTime;

	InvasionState InvasionStateInMap = GGameData()->IsInvadeWorld() ? InvasionState::Invade :
		isInvasionMap ? InvasionState::Go : IsValidInvasionTime() ? InvasionState::InTimeNoGo : InvasionState::NoGo;

	if (_InvasionStateInMap != InvasionStateInMap)
	{
		_InvasionStateInMap = InvasionStateInMap;
		// 현재 상태와 현재 로드된 맵이 침공 가능 맵인지 - 미니맵 갱신
		FGsUIMsgParamInt param(static_cast<int>(_InvasionStateInMap));
		GMessage()->GetUI().SendMessage(MessageUI::INVADE_MAPSTATE_UPDATE, &param);

		ReqInvasionPortalInfo();
	}
}

void UGsInterServerManager::OnUserBasicDataChange(const struct IGsMessageParam* Data)
{
	if (_initMapData) return;

	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		PlanetId planetId = SharedFunc::GetPlanetId(userData->mUserDBId);

		CHerculesPrefs::SetInt(MyPlanet, planetId);
		_initMapData = planetId > 0;
	}

	LoadInvadeMapData();
	LoadInvadeWorldData();

	_isInvasionTime = IsValidInvasionTime(true);
}

void UGsInterServerManager::OnInvasionStart(time_t invadeStartTime, time_t invadeEndTime)
{
	_invasionStartTime = FGsTimeSyncUtil::ConvertToDateTime(invadeStartTime);
	for (auto invasionMap : _invasionMapData)
	{
		invasionMap->SetInvasionEndTImeInMap(_invasionStartTime + FTimespan(0, _invadeRunningMinTime, 0));
	}

	GSLOG(Log, TEXT("OnInvasionStart Time %s"), *_invasionStartTime.ToIso8601());
}
