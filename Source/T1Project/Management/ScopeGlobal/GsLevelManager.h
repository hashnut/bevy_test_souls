// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/GsLevelDefine.h"
#include "Classes/GsManager.h"
#include "T1Project/Json/JsonDefine.h"
#include "T1Project/GameObject/Define/GsGameObjectDefine.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "Map/Area/GsSchemaAreaData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Map/Town/GsSchemaTownData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Camera/PlayerCameraManager.h"
#include "T1Project/Management/GsScopeHolder.h"
#include "GsLevelManager.generated.h"


class UBoxComponent;
class UDataTable;
class ULevelStreaming;
class ULocalPlayer;
class ULevelStreamingDynamic;
class APlayerController;

const FString DESIGN_LEVEL = TEXT("Design");
const FString SERVER_LEVEL = TEXT("Server");

UCLASS(BlueprintType)
class T1PROJECT_API UGsLevelManager :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

		//bak1210 
		//월드컴포지션을 사용하는 레벨에서 카메라가 붙은 액터를 현재 좌표 + 레벨스트리밍거리 보다 먼좌표에 스폰시
		//내케릭터의 좌표가 Unload 될수 있는 상황이 발생한다. (UWorldComposition::UpdateStreamingState 688 라인 참고)
		//그에 따라 내케릭터의 좌표를 Hold할 필요가 있어서 ULocalPlayer를 스폰시켜 위치를 고정시킨다.
		UPROPERTY(Transient)
		ULocalPlayer* _levelStreamingHolder = nullptr;

private:
	bool _changedLevel = false;
	int32 _currentLevelKey;
	FVector _playerSpawnPos;
	bool _bForcedChangeLevel = false;
	bool _spawnMeCompleteFinFlag = false;

	//warp
private:
	int32 _warpMapId;
	FVector _warpPos;
	WarpReason _warpReason = WarpReason::INVALID;
	EGsLoadTopoEffectType _currLoadTopoEffectType = EGsLoadTopoEffectType::NONE;
	// 인게임에서 pc 스폰전에 해당 위치 레벨 로드 되었는지 체크
	// TrySweepToLand 함수 사용하기 위해 실린더 radius, height 값 저장해놓는다	
	// FGsSchemaCreatureCommonInfo 테이블 찾을때 1, 2 가 pc 남, 여 키값인데 
	// 일단은 gender 값이랑 같아서 그걸로 찾자 (로비에서 정확히 가리키는 값있다면 그걸로 변경 필요)
	// https://jira.com2us.com/jira/browse/C2URWQ-5675 
	int32 _localCylinderRadius;
	int32 _localCylinderHeight;

	bool _isSameArea = false;

private:
	TArray<TWeakObjectPtr<AActor>> _cameraOwnerList;

public:
	static TWeakObjectPtr<class UWorld> _world;
	static bool _isPlayerSpawnComplete;

public:
	UGsLevelManager();

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	// 워프 데이터로 레벨 로드 처리	
	bool LoadLevelById(UWorld* inWorld, int32 inKey = 1, FVector inPos = FVector::ZeroVector);
	bool TryGetLevelName(FString& outLevel, int32 inKey);
	bool LoadLauncher();	
	bool LoadLobby();
	bool BackToLauncher();
	bool BackToLobby();

public:
	//월드컴포지션을 사용하는 레벨에서 카메라가 붙은 액터를 현재 좌표 + 레벨스트리밍거리 보다 먼좌표에 스폰시
	//내케릭터의 좌표가 Unload 될수 있는 상황이 발생한다. (UWorldComposition::UpdateStreamingState 688 라인 참고)
	//그에 따라 내케릭터의 좌표를 Hold할 필요가 있어서 ULocalPlayer를 스폰시켜 위치를 고정시킨다.
	bool HoldStreamingLevelByLocation(const FVector& inLocation);
	// 고정된 스트리밍 레벨을 해제 하는 함수
	void UnHoldStreamingLevel();
	// 스트리밍 레벨이 고정된 상태인가?
	bool IsHoldingStreamingLevel();

	//warp
public:
	bool LoadLevelByWarp();

public:
	bool GetChangedLevelFlag() { return _changedLevel; }
	void SetChangedLevelFlag(bool inChangedLevel) { _changedLevel = inChangedLevel; }
	// 현재 맵정보 가져오기	
	const FGsSchemaMapData* GetCurrentMapInfo();
	const FGsSchemaAreaData* GetCurrentAreaData();
	const FVector GetPlayerSpawnPos() { return _playerSpawnPos; }
	int32 GetCurrentLevelId() const { return _currentLevelKey; }
	void SetCurrentLevelId(int32 inLevelID) { _currentLevelKey = inLevelID; }
	const FString GetLevelIdString() { return FString::FromInt(_currentLevelKey); }
	int32 GetTotalChannelCount();
	static const FString GetDesignLevelName(const UWorld* inWorld, int32 inLevelId);
	static const FString GetArtLevelName(const UWorld* inWorld, int32 inLevelId);
	static const FString GetDesignLevelFullPath(const UWorld* inWorld, int32 inLevelId);
	static const FString GetArtLevelFullPath(const UWorld* inWorld, int32 inLevelId);
	static const FString GetLevelFolderPath(const UWorld* inWorld, int32 inLevelId);

	// 동일 레벨은 다시로징하지 않지만, 강제로 재로딩 하고 싶은 경우 LoadTopo 받기 전에 켜주기 
	void SetForcedChangeLevel(bool bInChangeLevel) { _bForcedChangeLevel = bInChangeLevel; }

	bool IsInstanceSingleMap();
	// 던전맵인가
	bool IsDungeonMap();
	// 던전맵인가
	bool IsDungeonMap(int32 InMapId);
	bool IsGuildDungeonMap();
	bool IsPartyDungeonMap();
	bool IsEliteDungeonMap();
	// 현재맵이랑 같은 타입인가
	bool IsMapType(MapType In_type);
	// 시공의 틈새 맵컨텐츠인가
	bool IsSpaceCrackMapContents();

	EGsLoadTopoEffectType GetLoadTopoEffectType() const { return _currLoadTopoEffectType; }
	void SetLoadTopoData(int32 InMapId, FVector InPos, WarpReason InReason);
	// 인게임에서 pc 스폰전에 해당 위치 레벨 로드 되었는지 체크
	// TrySweepToLand 함수 사용하기 위해 실린더 radius, height 값 저장해놓는다	
	// FGsSchemaCreatureCommonInfo 테이블 찾을때 1, 2 가 pc 남, 여 키값인데 
	// 일단은 gender 값이랑 같아서 그걸로 찾자 (로비에서 정확히 가리키는 값있다면 그걸로 변경 필요)
	// https://jira.com2us.com/jira/browse/C2URWQ-5675 
	void SetLocalCylinderSize(int32 In_cylinderRadius, int32 In_cylinderHeight);

	int32 GetLocalCylinderRadius()
	{
		return _localCylinderRadius;
	}
	int32 GetLocalCylinderHeight()
	{
		return _localCylinderHeight;
	}

	bool IsSameArea()
	{
		return _isSameArea;
	}

	// 워프데이터가 다른맵 가는건가
	bool IsWarpDataDifferentMap();
	bool IsSameArea(int32 InMapId1, int32 InMapId2);
	WarpReason GetWarpReason() {return _warpReason;}

	//마을 npc중 clientSpawn만 가져오는 함수
	static bool TryGetClientSpawnNpcInTown(TArray<const FGsSchemaTownData*>& outNpcList);
	static bool TryGetAllSpawnNpcInTown(TArray<const FGsSchemaTownData*>& outNpcList);

public:
	// 홀더의 컨트롤러 구하기
	APlayerController* GetHolderController();
	MapType GetCurrentMapType();

	//world 침공전 데이터
	//WorldId GetLoadWorldId() { return _worldId; }
	//EnterWorldServerType GetWorldServerType() { return _worldServerType; }


public:
	bool OnCheckUnregisterStreamingLevel(const FString& inLevel);
	static bool IsServerLevel(const FString& inLevel);
	bool IsDesignlevel(const FString& inLevel);
	static bool TryParseLevelName(TArray<FString>& outLevel, const FString& inLevel);
	MapType GetMapType();	
	static bool IsCanSpotWarp(int32 inDestLevelId, int32 inSpotIndex = INDEX_NONE, int32 inPlayerLevel = 0/*, QuestId inQuestId = 0*/);
	static bool IsCanMapWarp(int32 inDestLevelId, int32 inPlayerLevel = 0/*, QuestId inQuestId = 0*/);
	static bool IsCanMonsterWarp(int32 inDestLevelId, int32 inPlayerLevel = 0);
	static bool TryGetDestSpot(int32 inLevelId, int64 inWarpId, OUT int32& outDestLevelKey, OUT int32& outDestSpotIndex);

public:
	bool	GetLevelBounds(OUT FBoxSphereBounds& bound);
	TArray<FGsSchemaSpotInfo> GetMapSpot(int32 inLevelKey);
	TArray<FGsSchemaSpotInfo> GetZone(int32 inLevelKey);
	const struct FGsSchemaSpawnData* GetZone(int32 inLevelKey, int32 inGroupId);

private:
	bool IsRegisteredLevel(int32 inLevelKey);	
	const FGsSchemaAreaData* GetAreaData(int32 inLevelKey);

	EGsLoadTopoEffectType GetLoadTopoEffectType(int32 InMapId, WarpReason InReason);

	// 현재 서버 레벨id, 클라 id 씽크가 안맞아서
	// 퀘스트 테스트 진행용으로 접근
	// 나중에 GetCurrentLevelKey 로 바꿔야 함	

public:
	void PushViewTargetWithBlend(TWeakObjectPtr<AActor> NewViewTarget, float BlendTime = 0, enum EViewTargetBlendFunction BlendFunc = VTBlend_Linear, float BlendExp = 0, bool bLockOutgoing = false);
	void PushViewTarget(TWeakObjectPtr<AActor> NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams());
	void PopViewTarget(TWeakObjectPtr<AActor> RemoveViewTarget);
	static const FGsSchemaMapData* GetMapData(int32 inMapKey);
	static void SetGameWorld(UWorld* inWorld);
	static UWorld* GetGameWorld();
	static void ClearGameWorld();
	static void SetIsPlayerSpawnComplete(bool inSpawned);
	static bool GetIsPlayerSpawnComplete();
	static TArray<FVector> GetFloorPointList(class UBoxComponent* inBoxComponent);
	// 현재 view target 체크
	void CheckViewTarget();


};

#define GLevel() UGsScopeHolder::GetGlobalManager<UGsLevelManager>(UGsScopeGlobal::EManagerType::Level)