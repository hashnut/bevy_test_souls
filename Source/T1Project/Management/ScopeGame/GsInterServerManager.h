#pragma once
#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"

#include "GsInterServerManager.generated.h"

enum class InvasionState : uint8 { NoGo = 0, Invade = 1, Go = 2, InTimeNoGo = 3, Init = Invade };

struct T1PROJECT_API FGsInvadeMap
{
	MapId _id;
	const FGsSchemaInvadeMapData* _data;

	int32 _currentInvaderCount{ 0 };
	int32 _currentPortalCount{ 0 };

	FDateTime _portalEndTime{ 0 };

	bool _activePortal{ false };
	TArray<FGsSchemaSpotInfo> _spawnPortals;

	TArray<SpotId> _enrolledSpot;

	FGsInvadeMap(const FGsSchemaInvadeMapData* data)
		: _data(data)
	{
		_id = (data ? data->mapId.GetRow()->id : INVALID_MAP_ID);
	}

	void SetInvasionEndTImeInMap(FDateTime time) { _portalEndTime = time; }

	void SetInvaderCount(int count) { _currentInvaderCount = count; };
	void SetPortalCount(int count) { _currentPortalCount = count; }

	int32 GetInvaderCount() { return _currentInvaderCount; }
	int32 GetPortalCount()	{ return _currentPortalCount; }
	int32 GetValidLowLevel() { return _data != nullptr ? _data->enterAllowLowLevel : 0; }
	int32 GetValidHighLevel() { return _data != nullptr ? _data->enterAllowHighLevel : 0; }
	int32 GetEnrollingMaxCountInPortal() { return _data != nullptr ? _data->portalUserCount : 0; }
	const FDateTime& GetInvasionEndTime() { return _portalEndTime; }

	FText GetMapName() { return _data && _data->mapId.GetRow() ? _data->mapId.GetRow()->MapStringId : FText::GetEmpty(); }
	bool IsAirRaidEableState(FDateTime curTime) { return _portalEndTime > curTime; }

	void MapInfoReset()
	{
		_activePortal = false;
		_spawnPortals.Reset();
		_enrolledSpot.Reset();
	}
};

struct T1PROJECT_API FGsMatchingWorld
{
	WorldId _targetWorld;
	FText _worldName;

	FGsMatchingWorld(WorldId targetWorldId, const FText worldName)
		:  _targetWorld(targetWorldId), _worldName(worldName) {}

	const FText GetWorldName() { return _worldName; }
};

static const TCHAR* MyPlanet = TEXT("MyPlanet");

UCLASS()
class T1PROJECT_API UGsInterServerManager : public UObject, public IGsManager
{
	GENERATED_BODY()

protected:
	//uint64 _myPlanetId;
	TArray<TSharedPtr<FGsInvadeMap>> _invasionMapData;
	TMap<WorldId, TArray<TSharedPtr<FGsMatchingWorld>>> _invasionWorldData;

	//bool _isEnrolledPortal{ false };

	WorldId	  _currentWorldId;
	FDateTime _invasionStartTime{ FDateTime(0) };

	//현재 침공맵 (valid 하지 않으면 현재 침공맵 아님)
	TWeakPtr<FGsInvadeMap> _curInvasionMapInfo;

	MsgGameObjHandle _handleUserData;

private:
	bool _isInvasionTime{ false };
	bool _isInInvadeMap{ false };
	int  _invadeRunningMinTime{ 0 };

	// 대륙맵 갱신에 사용
	int32 _mapUpdateCount{ 0 }; 
	FTimerHandle _invasionInfoTimeHandle;
	//FTimerHandle _invasionPortalTimeHandle;
	//bool _worldMapUpdate{ false };
	bool _initMapData{ false };

protected:
	int32 _currentInvaderCount{ 0 };	// 현재 침공자 수 - 삭제 예정
	int32 _remainInvasionStaySec{ 0 };	// 남은 침공시간 - 삭제 예정
protected:
	//InvasionState _InvasionTimeState{ InvasionState::Init };  // 인터월드 침공 가능 시간 상태 - 맵마다 달라서 삭제
	InvasionState _InvasionStateInMap{ InvasionState::Init }; // 현재 맵에서의 인터월드 침공 State

	struct InvasionInfo
	{
		MapId	  _mapId;
		FDateTime _invasionEnterTime;
		FDateTime _invasionLeaveTime;
		FDateTime _invasionFinishStayTime;
		FTimespan _invasionOffset;
		int		  _playerKillPoint{ 0 };
	};
	
	TSharedPtr<InvasionInfo> _invasionInfo{ nullptr };
public:
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float inTick) override;

public:
	bool IsValidInvasionMap(MapId mapId);
	bool IsValidInvasionTime(bool reset = false);
	bool IsValidInvasionTimeInMyWorld();
	
	InvasionState  GetWorldInvasionState() const { return _InvasionStateInMap; }	// 인터월드 침공 가능 상태
	TWeakPtr<FGsInvadeMap> GetInvadeMapData(MapId mapId);	// 침공 가능 맵 정보

	TWeakPtr<FGsMatchingWorld> GetAirRaidWorld();

	int32 GetAirRaidMapList(OUT TArray<TWeakPtr<FGsInvadeMap>>& mapList);

	// 현재 침공가능한 내가 등록한 포탈이 있는 맵정보
	int32 GetActiveInvasionPortalMapList(OUT TArray<TWeakPtr<FGsInvadeMap>>& mapList);
public:
	FTimespan GetInvasionRemainStayTime();			// 남은 체류 가능 시간 바꿔서 허드 표시로 사용
	int32 GetTotalInvaderCount();	

public:
	// 맵로드 후 맵의 침공 가능 상태 체크, 미니맵 갱신
	void SetCurrentMap(MapId mapId);	
	TWeakPtr<FGsInvadeMap> GetCurrentInvasionMapInfo() { return _curInvasionMapInfo; }
	FDateTime GetNextInvasionEnableTime();

	//bool IsEnableEnrollingPortal() { return _isEnrolledPortal == false; }

	// 침공 시작/끝. 남은 침공 체류 시간 갱신
	void EnterInvasionState(time_t invadeTime, int remainSec, int addStaySec);
	void LeaveInvasionState(time_t invadeEndTime, int remainSec, int addStaySec);
	void SetInvasionStayTime(int remainSec, int addStaySec, int PlayerKillPoint = 0);

	// 대륙 맵에서 모든 맵의 현재 침공 인원.. 정보 요구 : 침공 인원 갱신
	bool ReqInvasionInfoCheck(bool isCheck = true);				
	void SetMapInvasionInfoList(const TArray<TPair<MapId, int>>& mapInfoList);	

	// 월드맵 포탈 spot 위치 요구
	void ReqInvasionPortalInfo();

	// 침공워프프랍을 경유한 침공 인원 갱신
	//void SetInvaderCountViaPortal(uint64 gameId, int invaderCount);
	void SetInvaderTotalCount(int invaderCount);

	//void NotReadyInvasionPortalSpot();
	void SetInvasionStartTime(time_t startTime = 0);
	void SetInvasionEndTime();
	void ResetSpawnedInvasionPortal(bool reset = false);
	void SetInvasionPortalSpotListInMap(MapId mapId, const TMap<SpotId, bool>& spotInfo);

	//void ReqActiveInvasionPortalList();

	// 현재 내가 등록한 포털이 있는 맵 정보
	void SetActiveInvasionPortalInMap(TArray<ActiveInvadePortalInfo>& activePortalInMaps);

	bool IsInvasionState() { return _InvasionStateInMap == InvasionState::Invade; }
	bool IsValidAirRaidState() { return _InvasionStateInMap == InvasionState::Go; }
	void SetWorldId(WorldId inWroldId){ _currentWorldId = inWroldId;}
	WorldId GetWorldId() { return _currentWorldId; }

	bool IsMapInInvasionTime(MapId mapId);
	int GetInvasionDurationMin() { return _invadeRunningMinTime; }

	void OnNotiOninvasionTime();

	void OnInvasionStart(time_t invadeStartTime, time_t invadeEndTime);
protected:
	void LoadInvadeMapData();
	void LoadInvadeWorldData();
	void UpdateInvasionInfo();

	void OnUserBasicDataChange(const struct IGsMessageParam* Data);
};

#define GSInterServer() UGsScopeHolder::GetGameManagerFType<UGsInterServerManager>(UGsScopeGame::EManagerType::InterServerInvasion)