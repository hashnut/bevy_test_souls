// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Public/CollisionQueryParams.h"
#include "NavigationSystem.h"
#include "Map/Spawn/GsSchemaTerritoryNpc.h"
#include "Map/Spawn/GsSchemaSpawnElement.h"
#include "Map/Town/GsSchemaTownData.h"
#include "Map/Spawn/GsSchemaSpawnData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "GsLevelUtil.generated.h"

/**
 *
 */
class AActor;
class UWorld;
class UNavigationSystemV1;
class ACharacter;
class UGsGameObjectBase;
class AGsCharacterBase;
struct FGsSchemaSpawnVisibleData;

const float DEFAULT_NAVMESH_HEIGHT = 20.0f;
const float LEVEL_MAX_HEIGHT = 100000.0f;
const float LEVEL_MIN_HEIGHT = -100000.0f;
const float LEVEL_GAP = 20000.0f;

UENUM(BlueprintType)
enum class EGroundCollisionType : uint8
{
	None,
	Ground,
	WorldStatic,
	Max
};

USTRUCT()
struct  FGsLevelStreaming
{
	GENERATED_BODY()

public:
	int32 TileIndex;
	UPROPERTY()
		ULevelStreaming* StreamingLevel;
};

UCLASS()
class T1PROJECT_API UGsLevelUtil : public UObject
{
	GENERATED_BODY()

public:
	static FTransform GetPlayerStart(UWorld* inWorld);	
	static FTransform GetPlayerTransform();
	static FTransform GetPlayerSpawnTransformFromServer();
	static void SetPlayerPossess(UWorld* inWorld, bool inPoccess);

public:
	UFUNCTION(BlueprintCallable, Category = "Level Utile")
		static FName GetLevelNameFromActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "Level Utile")
		static	int32 GetRandomRange(int32 in_source = 0, int32 in_dest = 100);
	UFUNCTION(BlueprintCallable, Category = "Level Utile")
		static	int32 GetRandomDegree();	

	//Height
public:
	static bool TryLineTraceToLandDebug(FHitResult& outHit, UWorld* inWorld, const FVector& inOrigin, float inMaxHeight = TNumericLimits<int32>::Max(), float inMinHeight = -TNumericLimits<int32>::Max(), const FColor inColor = FColor::Yellow, float inSecond = 300);		
	static bool TryLineTraceToLand(FHitResult& outHit, UWorld* inWorld, const FVector& inOrigin, float inMaxHeight = TNumericLimits<int32>::Max(), float inMinHeight = -TNumericLimits<int32>::Max());
	// TryLineTraceToLand로 하면 gameobject 위로 떨어 질때가 있어서 gameobject 제끼는 처리 추가
	static bool TryLineTraceToLandExceptGameObject(FHitResult& outHit, UWorld* inWorld, const FVector& inOrigin, float inMaxHeight = TNumericLimits<int32>::Max(), float inMinHeight = -TNumericLimits<int32>::Max());
	static bool TryGetClosestNavmeshLocation(FVector& OutNavmesh, UWorld* InWorld, const FVector& InOrigin);
	static bool TrySetCharacterLocationOnGround(UWorld* inWorld, UGsGameObjectBase* inOwner);
	static bool TrySetCharacterLocationOnGround(UWorld* inWorld, AGsCharacterBase* inOwner, float inHeight);
	static bool TryGetNearestLocationOnGround(UWorld* inWorld, FVector& outLocation, AActor* inOwner); 

	static bool TrySweepToLand(FHitResult& outHit, UWorld* inWorld, const FVector& inOrigin, float inRadius, float inHalfHeight);
	static bool TrySweepToLand(FHitResult& outHit, UWorld* inWorld, const FVector& inOrigin, float inRadius, float inHalfHeight, FCollisionQueryParams inQueryParam);

	//Streaming Level
public:	
	static FVector GetStreamingLevelCenter(UWorld* inWorld, int32 inTileIdx);	
	static bool IsExistOverlapedStreamingLevels(UWorld* inWorld, const FVector& inLocation);		
	static bool TryGetOverlpaedStreamingLevels(TArray<ULevelStreaming*>& outStreamingLevels, UWorld* inWorld, const FVector& inLocation);
	static bool TryGetOverlapedGsStreamingLevels(TArray<FGsLevelStreaming>& outLevels, UWorld* inWorld, const FVector& inLocation);
	static bool IsSteamingLevelLoaded(UWorld* inWorld, int32 inTileIndex);
	static bool IsSteamingLevelLoaded(ULevelStreaming* inLevel);

	//Gravity
public:
	static bool TryDeactiveGravity(UGsGameObjectBase* inOwner);
	static bool TryActiveGravity(UGsGameObjectBase* inOwner);
	static bool ActiveGravity(UGsGameObjectBase* inOwner, bool inAcitveGravity = true);	
	static void ActivePlayerGravity();
	static void DeactivePlayerGravity();

public:
	static bool TryGetSpot(int32 inLevel, int32 inSpotIndex, FVector& outLocation, FRotator& outRotator);
	static bool TryGetSpot(int32 inLevel, int32 inSpotIndex, OUT const FGsSchemaSpotInfo*& outSpotInfo);

	//fence
public:
	static bool IsConcaveFence(const TArray<FVector>& inPointList);

public:	
	static bool TryGetRandomLocationOnGround(FVector& outLocation, UWorld* inWorld, const FVector& inCenter, const float inMinRadius, const float inMaxRadius);

	//warp
public:
	static bool CheckContinuityWarp(int32 inSource, int32 inDest);

private: 
	static bool CheckContinuityWarp_Internal(int32 inSource, int32 inDest, TArray<int32>& inMapList);

public:
	static TArray<FGsSchemaSpawnElement> GetSpawnDataElementByTIDInCurrentLevel(CreatureSpawnType inSpawnType, int32 inTID);
	static TArray<FGsSchemaSpawnElement> GetSpawnDataElementByTID(int32 inLevelKey, CreatureSpawnType inSpawnType, int32 inTID);
	static bool TryGetAllSpawnElement(const int32 inLevelKey, const CreatureSpawnType inSpawnType, OUT TArray<FGsSchemaSpawnElement>& outData);
	static bool TryGetSpawnData(int32 inLevelKey, TArray<const FGsSchemaSpawnData*>& outSpawnDataArray);
	static bool TryGetAllInvadePortalSpotArray(MapId inLevelId, OUT TArray<const FGsSchemaSpotInfo*>& outSpotArray);
	static TArray<const FGsSchemaSpawnVisibleData*> GetSpawnVisibleData(int32 inLevelKey);
	static bool IsLandscapeStreamingProxyLoaded(TArray<FString>& outLoadedLandProxy, TArray<FString>& outUnloadedLandProxy, UWorld* inWorld, const TArray<FString>& inSubLevelPackageList);
	static bool IsSubLevel(UWorld* inWorld, const FString& inLongPackage);
	static bool IsCanWarp(int64 inWarp);
	static bool IsCanQuestWarp(QuestId inQuestId);
	static bool IsCanBookmarkWarp(uint64 inBookmarkId);	
	static bool TryFindSpawnInfo(MapId inLevelId, int64& inUniqueId, OUT FGsSchemaSpawnData& outSpawnData, OUT FGsSchemaSpawnElement& outSpawnElement);
	static bool TryFindSpawnInfo(MapId inLevelId, CreatureId inNpcId, OUT FGsSchemaSpawnData& outSpawnData, OUT FGsSchemaSpawnElement& outSpawnElement);
	static int32 GetSpawnLevel(const struct FGsSchemaNpcData* inNpcData, MapId inMapId);
	static void DrawDebugLocation(UWorld* inWorld, const FVector& inLocation, AActor* inActor, const FString& inCallTime, FColor inColor = FColor::Yellow, float inSeconds = 300);
	static bool TryGetAllOverlappedAreaName(class UWorld* inWorld, OUT TArray<class AGsAreaNameHUD*> outAreaArray);	
};
