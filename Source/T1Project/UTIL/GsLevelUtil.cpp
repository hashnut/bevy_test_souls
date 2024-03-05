// Fill out your copyright notice in the Description page of Project Settings.
#define DRAW_HEIGHT false

#include "GsLevelUtil.h"
#include "Runtime/Core/Public/Math/RandomStream.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LevelStreaming.h"
#include "Engine/WorldComposition.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "DrawDebugHelpers.h"

#include "UTIL/GsDir.h"
#include "Cheat/GsCheatManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "ActorEx/GsCharacterBase.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "UTIL/GsTableUtil.h"
#include "DataSchema/Map/Spot/GsSchemaSpotData.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Map/Town/GsSchemaTownData.h"
#include "Map/Spawn/GsSchemaClientSpawnData.h"
#include "Map/Spawn/GsSchemaTerritoryNpc.h"
#include "Map/Spawn/GsSchemaSpawnElement.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "Map/GsSchemaMapData.h"
#include "Map/GsSchemaMapWarpInfo.h"
#include "Map/Spawn/GsSchemaSpawnData.h"
#include "Map/Spawn/GsSchemaSpawnVisibleData.h"
#include "LandscapeStreamingProxy.h"
#include "Quest/GsQuestData.h"
#include "Quest/Table/GsQuestTableCache.h"
#include "Map/Bookmark/GsBookmarkData.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "T1Project.h"
#include "Components/CapsuleComponent.h"
#include "Object.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterData.h"
#include "UI/GsAreaNameHUD.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"

const FString ONLY_WORLDSTATIC_COLLISION = TEXT("OnlyWorldStatic");
const FVector DEFUALT_QUERY_EXTENT = FVector(50.0f, 50.0f, 5000.0f);

FTransform UGsLevelUtil::GetPlayerStart(UWorld* inWorld)
{
	FTransform temp;
	TArray<AActor*> actors;

	UGameplayStatics::GetAllActorsOfClass(inWorld, APlayerStart::StaticClass(), actors);

	if (actors.Num() > 0)
	{
		temp = actors[0]->GetActorTransform();
	}	

	return temp;
}

FTransform UGsLevelUtil::GetPlayerTransform()
{
	FTransform temp;

	if (UGsGameObjectBase * player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (AActor* actor = player->GetActor())
		{
			temp = actor->GetActorTransform();
		}
	}

	return temp;
}

FTransform UGsLevelUtil::GetPlayerSpawnTransformFromServer()
{
	FTransform temp;

	if (GLevel())
	{
		temp.SetLocation(GLevel()->GetPlayerSpawnPos());
	}

	return temp;
}

void UGsLevelUtil::SetPlayerPossess(UWorld* inWorld, bool inPoccess)
{
	if (GSGameObject())
	{
		if (UGsGameObjectBase * player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (AActor * actor = player->GetActor())
			{
				if (APawn * pawn = Cast<APawn>(actor))
				{
					if (APlayerController * controller = UGameplayStatics::GetPlayerController(inWorld, 0))
					{
						if (inPoccess)
						{
							controller->Possess(pawn);
						}
						else
						{
							controller->UnPossess();
						}
					}
				}
			}
		}
	}
}

FName UGsLevelUtil::GetLevelNameFromActor(AActor* in_actor)
{
	if (in_actor)
	{
		ULevel* level = in_actor->GetLevel();

		if (level)
		{
			UObject* outer = level->GetOuter();

			if (outer)
			{
				return outer->GetFName();
			}
		}
	}

	return NAME_None;
}

int32 UGsLevelUtil::GetRandomRange(int32 in_source, int32 in_dest)
{
	FRandomStream rand;
	rand.GenerateNewSeed();

	return rand.RandRange(in_source, in_dest);
}

int32 UGsLevelUtil::GetRandomDegree()
{
	return GetRandomRange(0, 360);
}

bool UGsLevelUtil::TrySweepToLand(FHitResult& outHit, UWorld* inWorld, const FVector& inOrigin, float inRadius, float inHalfHeight, FCollisionQueryParams inQueryParam)
{
	if (nullptr == inWorld)
	{
		return false;
	}

	static FCollisionResponseParams responseParams(ECR_Ignore);
	responseParams.CollisionResponse.SetResponse(ECC_WorldStatic, ECR_Block);

	static FCollisionShape collisionShape;
	collisionShape.SetCapsule(inRadius, inHalfHeight);
	if (inWorld->SweepSingleByChannel(outHit, inOrigin + FVector(0, 0, inHalfHeight), inOrigin + FVector(0, 0, -inHalfHeight), 
		FQuat::Identity, ECollisionChannel::ECC_WorldStatic, collisionShape, inQueryParam, responseParams))
	{
		if (false == outHit.Actor.IsValid())
		{
			// GSLOG(Error, TEXT("Terrain - set invalide character on land : %s"), *result.Actor->GetName());
			return false;
		}

#if SPAWN_DEBUG
		DrawDebugLine(inWorld, inOrigin + FVector(0, 0, inHalfHeight), inOrigin + FVector(0, 0, -inHalfHeight), FColor::Red, false, 30, 0, 5);
		FString log = FString::Printf(TEXT("TrySweepToLand location : %s"), *outHit.Actor->GetActorLabel());
		DrawDebugString(inWorld, outHit.Location, log, NULL, FColor::Red, 30);
		DrawDebugSphere(inWorld, outHit.Location, 25, 8, FColor::Red, false, 30);
		GSLOG(Log, TEXT("%s"), *log);
#endif

		return true;
	}

	return false;
}

bool UGsLevelUtil::TrySweepToLand(FHitResult& outHit, UWorld* inWorld, const FVector& inOrigin, float inRadius, float inHalfHeight)
{
	static FCollisionQueryParams queryParam;
	return TrySweepToLand(outHit, inWorld, inOrigin, inRadius, inHalfHeight, queryParam);
}

bool UGsLevelUtil::TryLineTraceToLand(FHitResult& outHit, UWorld* inWorld, const FVector& inOrigin, float inMaxHeight, float inMinHeight)
{
	if (nullptr == inWorld)
	{
		return false;
	}

	FCollisionQueryParams queryParam;
	queryParam.bIgnoreBlocks = false;
	queryParam.bIgnoreTouches = true;

	// blocked by world static and optionally world dynamic
	FCollisionResponseParams responseParams(ECR_Ignore);
	responseParams.CollisionResponse.SetResponse(ECC_WorldStatic, ECR_Block);	

	FHitResult result;
	FVector start = FVector(inOrigin.X, inOrigin.Y, inMaxHeight);
	FVector end = FVector(inOrigin.X, inOrigin.Y, inMinHeight);
	if (inWorld->LineTraceSingleByChannel(result, start, end, ECollisionChannel::ECC_WorldStatic, queryParam, responseParams))
	{
		if (false == result.Actor.IsValid())
		{
			// GSLOG(Error, TEXT("Terrain - set invalide character on land : %s"), *result.Actor->GetName());
			return false;
		}		

		outHit = result;

		return true;
	}

	return false;
}

// TryLineTraceToLand로 하면 gameobject 위로 떨어 질때가 있어서 gameobject 제끼는 처리 추가
bool UGsLevelUtil::TryLineTraceToLandExceptGameObject(
	FHitResult& outHit, UWorld* inWorld,
	const FVector& inOrigin, float inMaxHeight, float inMinHeight)
{
	if (nullptr == inWorld)
	{
		return false;
	}

	TArray<FHitResult> arrayHitResult;	
	FVector start = FVector(inOrigin.X, inOrigin.Y, inMaxHeight);
	FVector end = FVector(inOrigin.X, inOrigin.Y, inMinHeight);


	bool isFind = false;
	if (inWorld->LineTraceMultiByObjectType(arrayHitResult, start, end, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)))
	{		
		for (const FHitResult& result: arrayHitResult)
		{
			if (false == result.Actor.IsValid())
			{
				continue;
			}

			if (result.Actor->IsA(AGsCharacterBase::StaticClass()) == true)
			{
				continue;
			}

			isFind = true;
			outHit = result;
			break;
		}		
	}

	return isFind;
}

bool UGsLevelUtil::TryGetClosestNavmeshLocation(FVector& OutNavmesh, UWorld* InWorld, const FVector& InOrigin)
{
	OutNavmesh = InOrigin;

	if (nullptr == InWorld)
	{
		return false;
	}

	UNavigationSystemBase* navigaton = InWorld->GetNavigationSystem();
	if (nullptr == navigaton)
	{
		return false;
	}

	UNavigationSystemV1* navigationSystemV1 = Cast<UNavigationSystemV1>(navigaton);
	if (nullptr == navigationSystemV1)
	{
		return false;
	}

	FNavLocation navLocaiton;			
	if (false == navigationSystemV1->ProjectPointToNavigation(InOrigin, navLocaiton, DEFUALT_QUERY_EXTENT))
	{
		return false;
	}

	OutNavmesh = navLocaiton.Location;

	return true;
}

bool UGsLevelUtil::TrySetCharacterLocationOnGround(UWorld* inWorld, AGsCharacterBase* inOwner, float inHeight)
{
	FVector origin = inOwner->GetActorLocation() + FVector(0, 0, inHeight);
	FVector landLocation;
	FHitResult hit;
	/*TArray<AActor*> exceptActors;
	exceptActors.Add(inOwner);*/

	if (false == UGsLevelUtil::TryLineTraceToLand(hit, inWorld, origin, origin.Z))
	{
		return false;
	}

	landLocation = hit.Location;
	
	FVector offset = FVector(0, 0, inHeight + 20.0f);
	inOwner->SetActorLocation(landLocation + offset);

#if WITH_EDITOR
	if (UGsCheatManager::IsShowSpawnLocation)
	{
		//Origin				
		FString log = TEXT("Linetrace start - name : ") + inOwner->GetName();

		DrawDebugString(inWorld, origin, log, NULL, FColor::White, 30.0f);
		DrawDebugSolidBox(inWorld, origin, FVector(100, 100, 0), FColor::Yellow, false, 30.0f);

		log = TEXT("Character location - name : ") + inOwner->GetName();

		//Spawn location
		DrawDebugString(inWorld, landLocation, log, NULL, FColor::White, 30.0f);
		DrawDebugSolidBox(inWorld, landLocation, FVector(100, 100, 0), FColor::Blue, false, 30.0f);
	}	
#endif

	return true;
}

bool UGsLevelUtil::TrySetCharacterLocationOnGround(UWorld* inWorld, UGsGameObjectBase* inOwner)
{
	if (nullptr == inOwner)
	{
		return false;
	}

	AActor* actor = inOwner->GetActor();

	if (nullptr == actor)
	{
		return false;
	}

	AGsCharacterBase* character = Cast<AGsCharacterBase>(actor);

	if (character)
	{
		float height = inOwner->GetData()->GetScaledCapsuleHalfHeight();

		return TrySetCharacterLocationOnGround(inWorld, character, height);
	}	

	return false;
}

bool UGsLevelUtil::TryGetNearestLocationOnGround(UWorld* inWorld, FVector& outLocation, AActor* inOwner)
{	
	FVector origin = inOwner->GetActorLocation();
	FHitResult hit;
	/*TArray<AActor*> exceptActors;
	exceptActors.Add(inOwner);*/

	if (false == UGsLevelUtil::TryLineTraceToLand(hit, inWorld, origin, origin.Z))
	{
		return false;
	}

	outLocation = hit.Location;	

	return true;
}

FVector UGsLevelUtil::GetStreamingLevelCenter(UWorld* inWorld, int32 inTileIdx)
{
	if (nullptr == inWorld)
	{
		return FVector::ZeroVector;
	}

	if (nullptr == inWorld->WorldComposition)
	{
		return FVector::ZeroVector;
	}

	TArray<FWorldCompositionTile>& tileArray = inWorld->WorldComposition->GetTilesList();

	if (false == tileArray.IsValidIndex(inTileIdx))
	{
		return FVector::ZeroVector;
	}

	const FBox& relatvieBound = tileArray[inTileIdx].Info.Bounds;
	const FIntVector& absolutePosition = tileArray[inTileIdx].Info.AbsolutePosition;

	FVector min;
	FVector max;

	min = relatvieBound.Min + FVector(absolutePosition.X, absolutePosition.Y, absolutePosition.Z);
	max = relatvieBound.Max + FVector(absolutePosition.X, absolutePosition.Y, absolutePosition.Z);

	return (min + max) / 2;
}

bool UGsLevelUtil::IsExistOverlapedStreamingLevels(UWorld* inWorld, const FVector& inLocation)
{
	TArray<ULevelStreaming*> levels;

	return TryGetOverlpaedStreamingLevels(levels, inWorld, inLocation);
}

bool UGsLevelUtil::TryGetOverlpaedStreamingLevels(TArray<ULevelStreaming*>& outLevels, UWorld* inWorld, const FVector& inLocation)
{
	TArray<FGsLevelStreaming> levels;
	outLevels.Empty();

	if (TryGetOverlapedGsStreamingLevels(levels, inWorld, inLocation))
	{
		for (FGsLevelStreaming& level : levels)
		{
			outLevels.Add(level.StreamingLevel);
		}
	}

	return outLevels.Num() > 0;
}

bool UGsLevelUtil::TryGetOverlapedGsStreamingLevels(TArray<FGsLevelStreaming>& outLevels, UWorld* inWorld, const FVector& inLocation)
{
	outLevels.Empty();

	if (nullptr == inWorld)
	{
		return false;
	}	

	if (nullptr == inWorld->WorldComposition)
	{
		return false;
	}
	
	TArray<FDistanceVisibleLevel> visibleLevels;
	TArray<FDistanceVisibleLevel> hiddenLevels;

	inWorld->WorldComposition->GetDistanceVisibleLevels(inLocation, visibleLevels, hiddenLevels);

	TArray<FWorldCompositionTile>& tileArray = inWorld->WorldComposition->GetTilesList();
	FVector min;
	FVector max;
	FBox realBound;
	FWorldCompositionTile tile;

	for (FDistanceVisibleLevel& level : visibleLevels)
	{
		if (nullptr == level.StreamingLevel)
		{
			continue;
		}			

		if (false == tileArray.IsValidIndex(level.TileIdx))
		{
			continue;
		}			

		tile = tileArray[level.TileIdx];

#if WITH_EDITOR
		if (tile.Info.Layer.Name.Equals(SERVER_LEVEL))
		{
			continue;
		}
#endif

		const FBox& relatvieBound = tileArray[level.TileIdx].Info.Bounds;
		const FIntVector& absolutePosition = tileArray[level.TileIdx].Info.AbsolutePosition;
		min = relatvieBound.Min + FVector(absolutePosition.X, absolutePosition.Y, 0);
		max = relatvieBound.Max + FVector(absolutePosition.X, absolutePosition.Y, 0);
		realBound = FBox(min, max);

		if (realBound.IsValid)
		{
			if (realBound.IsInsideOrOn(inLocation))
			{				
				FGsLevelStreaming addLevel;

				addLevel.TileIndex = level.TileIdx;
				addLevel.StreamingLevel = level.StreamingLevel;

				outLevels.Add(addLevel);
			}
		}
	}

	return (outLevels.Num() > 0);
}

bool UGsLevelUtil::IsSteamingLevelLoaded(UWorld* inWorld, int32 inTileIndex)
{
	if (nullptr == inWorld)
	{
		return false;
	}

	if (nullptr == inWorld->WorldComposition)
	{
		return false;
	}

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(inWorld, 0);

	if (nullptr == player)
	{
		return false;
	}

	TArray<FDistanceVisibleLevel> visibleLevels;
	TArray<FDistanceVisibleLevel> hiddenLevels;

	inWorld->WorldComposition->GetDistanceVisibleLevels(
		player->GetActorLocation(), visibleLevels, hiddenLevels
	);

	for (FDistanceVisibleLevel& level : visibleLevels)
	{
		if (level.TileIdx == inTileIndex)
		{
			if (level.StreamingLevel)
			{
				return IsSteamingLevelLoaded(level.StreamingLevel);
			}

			return false;
		}
	}

	GSLOG(Error, TEXT("There is no tile index : %d"), inTileIndex);

	return false;
}

bool UGsLevelUtil::IsSteamingLevelLoaded(ULevelStreaming* inLevel)
{
	if (inLevel)
	{
		return (inLevel->GetCurrentState() == ULevelStreaming::ECurrentState::LoadedVisible);
	}

	return false;
}

bool UGsLevelUtil::TryDeactiveGravity(UGsGameObjectBase* inOwner)
{
	return ActiveGravity(inOwner, false);
}

bool UGsLevelUtil::TryActiveGravity(UGsGameObjectBase* inOwner)
{
	return ActiveGravity(inOwner);
}

bool UGsLevelUtil::ActiveGravity(UGsGameObjectBase* inOwner, bool inAcitveGravity)
{	
	if (UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(inOwner))
	{		
		creature->ActiveGravity(inAcitveGravity);

		return true;
	}

	return false;
}

void UGsLevelUtil::ActivePlayerGravity()
{
	if (UGsGameObjectBase * player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsLevelUtil::TryActiveGravity(player);
	}
}

void UGsLevelUtil::DeactivePlayerGravity()
{
	if (UGsGameObjectBase * player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsLevelUtil::TryDeactiveGravity(player);
	}
}

bool UGsLevelUtil::TryGetSpot(int32 inLevel, int32 inSpotIndex, FVector& outLocation, FRotator& outRotator)
{
	const FGsSchemaSpotInfo* spotData = nullptr;
	if (false == TryGetSpot(inLevel, inSpotIndex, spotData))
	{
		return false;
	}

	outLocation = spotData->pos;
	outRotator = FGsDir::AngleToRotator(spotData->dir);
	return true;
}

bool UGsLevelUtil::TryGetSpot(int32 inLevel, int32 inSpotIndex, OUT const FGsSchemaSpotInfo*& outSpotInfo)
{
	FString levelKeyString = FString::FromInt(inLevel);
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(levelKeyString);
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return false;
	}

	const FGsSchemaSpotData* find;
	table->FindRow<FGsSchemaSpotData>(*levelKeyString, find);
	if (nullptr == find)
	{
		return false;
	}

	const FGsSchemaSpotInfo* findSpot = find->SpotList.FindByPredicate([&](const FGsSchemaSpotInfo& spot)->bool
		{
			return inSpotIndex == spot.id;
		});
	if (nullptr == findSpot)
	{
		return false;
	}

	outSpotInfo = findSpot;
	return true;
}

bool UGsLevelUtil::IsConcaveFence(const TArray<FVector>& inPointList)
{
	int32 num = inPointList.Num();	
	int32 currentIndex;
	int32 nextIndex;	
	int32 doubleNextIndex;

	//FVector2D v1;
	//FVector2D v2;
	//FVector2D v3;

	FVector v1;
	FVector v2;
	FVector v3;

	for (int32 i = 0; i < num; ++i)
	{
		currentIndex = i % num;
		nextIndex = (i + 1) % num;
		doubleNextIndex = (i + 2) % num;

		/*v1 = (FVector2D(inPointList[currentIndex]) - FVector2D(inPointList[nextIndex])).GetSafeNormal();
		v2 = (FVector2D(inPointList[doubleNextIndex]) - FVector2D(inPointList[nextIndex])).GetSafeNormal();*/

		v1 = (inPointList[doubleNextIndex] - inPointList[nextIndex]).GetSafeNormal();
		v2 = (inPointList[currentIndex] - inPointList[nextIndex]).GetSafeNormal();
		
		v3 = v1 ^ v2;

		if (v3.Z < 0)
		{
			return true;
		}
	}

	return false;
}

bool UGsLevelUtil::TryGetRandomLocationOnGround(FVector& outLocation, UWorld* inWorld, const FVector& inCenter, const float inMinRadius, const float inMaxRadius)
{
	//rand direciton
	FRandomStream randStream(FDateTime::Now().GetSecond());

	int32 degree = randStream.RandRange(0, 360);
	int32 distance = randStream.RandRange(inMinRadius, inMinRadius);

	//rand distance
	FVector direction = FVector::ForwardVector;
	FVector destLocation = direction.RotateAngleAxis(degree, FVector::UpVector) * distance + inCenter;
	FVector finalDestLocation;
	FHitResult hit;

	/*TArray<AActor*> exceptActors;*/
	if(UGsLevelUtil::TryLineTraceToLand(hit, inWorld, destLocation))
	{
		outLocation = hit.Location;
		
		return true;
	}

	return false;
}

bool UGsLevelUtil::CheckContinuityWarp(int32 inSource, int32 inDest)
{
	if (inSource == inDest)
	{
		GSLOG(Log, TEXT("Check continuity warp - same map - source : %d\tdest : %d"), inSource, inDest);

		return false;
	}

	TArray<int32> mapList;
	mapList.Add(inSource);
	if (false == CheckContinuityWarp_Internal(inSource, inDest, mapList))
	{
		GSLOG(Log, TEXT("Check continuity warp - fail - source : %d\tdest : %d"), inSource, inDest);

		return false;
	}

	GSLOG(Log, TEXT("Check continuity warp - success - source : %d\tdest : %d"), inSource, inDest);

	return true;
}

bool UGsLevelUtil::CheckContinuityWarp_Internal(int32 inSource, int32 inFinalDest, TArray<int32>& inMapList)
{
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaMapData::StaticStruct());
	if (nullptr == table)
	{
		return false;
	}

	FName mapId = *FString::FromInt(inSource);
	const FGsSchemaMapData* mapData;
	if (false == table->FindRow<FGsSchemaMapData>(mapId, mapData))
	{
		return false;
	}

	//success		
	if (const FGsSchemaMapTerritoryData* territory = mapData->territoryInfo.GetRow())
	{
		for (const FGsSchemaMapWarpInfo& warp : territory->warpList)
		{
			int32 destId = warp.destSpot.MapId.GetRow()->mapId;

			if (inMapList.Contains(destId))
			{
				continue;
			}

			if (destId == inFinalDest)
			{
				return true;
			}
			else
			{
				inMapList.Add(destId);
				if (CheckContinuityWarp_Internal(destId, inFinalDest, inMapList))
				{
					return true;
				}
			}
		}
	}

	return false;
}

TArray<FGsSchemaSpawnElement> UGsLevelUtil::GetSpawnDataElementByTIDInCurrentLevel( CreatureSpawnType inSpawnType, int32 inTID)
{
	int32 levelKey = GLevel()->GetCurrentLevelId();
	return GetSpawnDataElementByTID(levelKey, inSpawnType, inTID);
}

TArray<FGsSchemaSpawnElement> UGsLevelUtil::GetSpawnDataElementByTID(int32 inLevelKey, CreatureSpawnType inSpawnType, int32 inTID)
{
	TArray<FGsSchemaSpawnElement> findList;
	TArray<const FGsSchemaSpawnData*> zoneList;
	
	TryGetSpawnData(inLevelKey, zoneList);
	for (const FGsSchemaSpawnData* zone : zoneList)
	{
		if (zone)
		{
			for(const FGsSchemaSpawnElement& spawnElement : zone->SpawnInfoList)
			{ 
				if (spawnElement.SpawnType == inSpawnType
				&& spawnElement.CreatureId == inTID)
				{
					findList.Add(spawnElement);
				}				
			}
		}
	}

	return findList;	 
}

bool UGsLevelUtil::TryGetAllSpawnElement(const int32 inLevelKey, const CreatureSpawnType inSpawnType, OUT TArray<FGsSchemaSpawnElement>& outSpawnElementArray)
{
	TArray<const FGsSchemaSpawnData*> spawnDataArray;
	if (false == TryGetSpawnData(inLevelKey, spawnDataArray))
	{
		return false;
	}

	outSpawnElementArray.Empty();
	for (const FGsSchemaSpawnData* spawnData : spawnDataArray)
	{
		for (const FGsSchemaSpawnElement& spawnElement : spawnData->SpawnInfoList)
		{
			if (inSpawnType == spawnElement.SpawnType)
			{
				outSpawnElementArray.Add(spawnElement);
			}
		}		
	}

	return outSpawnElementArray.Num() > 0;
}

bool UGsLevelUtil::TryGetSpawnData(int32 inLevelKey, TArray<const FGsSchemaSpawnData*>& outSpawnDataArray)
{
	outSpawnDataArray.Empty();
	TArray<const FGsSchemaSpawnData*> rowList;
	FString levelKey = FString::FromInt(inLevelKey);
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(levelKey);	
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaSpawnData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return false;
	}	

	table->GetAllRows<FGsSchemaSpawnData>(rowList);
	outSpawnDataArray = rowList;

	return true;
}

bool UGsLevelUtil::TryGetAllInvadePortalSpotArray(MapId inLevelId, OUT TArray<const FGsSchemaSpotInfo*>& outSpotArray)
{
	outSpotArray.Empty();	
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaInvadeMapData::StaticStruct());
	if (nullptr == table)
	{
		return false;
	}

	TArray<const FGsSchemaInvadeMapData*> invadeMapArray;
	table->GetAllRows<FGsSchemaInvadeMapData>(invadeMapArray);
	const FGsSchemaInvadeMapData** invadeMapData = invadeMapArray.FindByPredicate([&](const FGsSchemaInvadeMapData* mapData)->bool {
		return mapData->mapId.GetRow()->id == inLevelId;
		});	
	if (nullptr == invadeMapData)
	{
		return false;
	}	

	for (int64 id : (*invadeMapData)->portalSpotList)
	{
		const FGsSchemaSpotInfo* spotInfo = nullptr;
		if (UGsLevelUtil::TryGetSpot(inLevelId, id, spotInfo))
		{
			outSpotArray.Add(spotInfo);
		}
	}

	return outSpotArray.Num() > 0;
}

bool UGsLevelUtil::TryFindSpawnInfo(MapId inLevelId, int64& inUniqueId, OUT FGsSchemaSpawnData& outSpawnData, OUT FGsSchemaSpawnElement& outSpawnElement)
{
	TArray<const FGsSchemaSpawnData*> spawnDataArray;	
	TryGetSpawnData(inLevelId, spawnDataArray);

	for (const FGsSchemaSpawnData* spawnData : spawnDataArray)
	{
		for (const FGsSchemaSpawnElement& spawnElement : spawnData->SpawnInfoList)
		{
			if (spawnElement.SpawnId == inUniqueId)
			{
				outSpawnData = *spawnData;
				outSpawnElement = spawnElement;
				return true;
			}
		}		
	}

	return false;
}


bool UGsLevelUtil::TryFindSpawnInfo(MapId inLevelId, CreatureId inNpcId, OUT FGsSchemaSpawnData& outSpawnData, OUT FGsSchemaSpawnElement& outSpawnElement)
{
	TArray<const FGsSchemaSpawnData*> spawnDataArray;
	TryGetSpawnData(inLevelId, spawnDataArray);

	for (const FGsSchemaSpawnData* spawnData : spawnDataArray)
	{
		for (const FGsSchemaSpawnElement& spawnElement : spawnData->SpawnInfoList)
		{
			if (spawnElement.SpawnType == CreatureSpawnType::NPC
				&& spawnElement.CreatureId == inNpcId)
			{
				outSpawnData = *spawnData;
				outSpawnElement = spawnElement;
				return true;
			}
		}
	}

	return false;
}

int32 UGsLevelUtil::GetSpawnLevel(const FGsSchemaNpcData* inNpcData, MapId inMapId)
{
	if (inNpcData->fixedLevel > 0)
	{
		return inNpcData->fixedLevel;
	}

	FGsSchemaSpawnData spawnData;
	FGsSchemaSpawnElement spawnElement;
	if (UGsLevelUtil::TryFindSpawnInfo(inMapId, inNpcData->id, spawnData, spawnElement))
	{
		return spawnElement.spawnLevel;
	}

	return 0;
}


void UGsLevelUtil::DrawDebugLocation(UWorld* inWorld, const FVector& inLocation, AActor* inActor, const FString& inCallTime, FColor inColor, float inSecond)
{

	FString actorLabel;

#if WITH_EDITOR
	if (inActor)
	{
		actorLabel = inActor->GetActorLabel();
	}	
#endif

	FVector origin = inLocation;
	FHitResult hit;
	FString log;
	float halfHeight = 1000;
	float radius = 100;
	AGsCharacterBase* character = Cast<AGsCharacterBase>(inActor);
	if(character)
	{
		halfHeight = character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		radius = character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}	

	if (UGsLevelUtil::TryLineTraceToLand(hit, inWorld, origin, origin.Z, origin.Z - 1000))
	{
#if WITH_EDITOR
		log = TEXT("Hit - success - CallTime : ") + inCallTime + TEXT("\tname : ") + actorLabel + TEXT("\tlocation : ") + hit.Location.ToString() + TEXT("\thit actor : ") + hit.GetActor()->GetActorLabel();
#else
		log = TEXT("Hit - success - CallTime : ") + inCallTime + TEXT("\tlocaation : ") + hit.Location.ToString();
#endif
	}
	else
	{
		log = TEXT("Hit - fail - CallTime : ") + inCallTime + TEXT("\torigin location : ") + inLocation.ToString();
	}

	if (hit.Actor.IsValid())
	{
		DrawDebugString(inWorld, hit.Location, log, NULL, inColor, inSecond);
		DrawDebugLine(inWorld, origin, hit.Location, inColor, false, inSecond);

		if (character)
		{
			DrawDebugCapsule(inWorld, hit.Location, halfHeight, radius, FQuat::Identity, inColor, false, inSecond);
		}
		else
		{
			DrawDebugSolidBox(inWorld, hit.Location, FVector(100, 100, 0), inColor, false, inSecond);
		}
	}	

}

bool UGsLevelUtil::TryGetAllOverlappedAreaName(UWorld* inWorld, OUT TArray<AGsAreaNameHUD*> outAreaArray)
{
	outAreaArray.Empty();
	TArray<AActor*> areaNameArray;
	UGameplayStatics::GetAllActorsOfClass(inWorld, AGsAreaNameHUD::StaticClass(), areaNameArray);
	if (0 == areaNameArray.Num())
	{
		return false;
	}

	AGsAreaNameHUD* castAreaName = nullptr;
	for (AActor* area : areaNameArray)
	{
		if (nullptr == area)
		{
			continue;
		}

		castAreaName = Cast<AGsAreaNameHUD>(area);
		if (nullptr == castAreaName)
		{
			continue;
		}

		if (false == castAreaName->GetIsInCollider())
		{
			continue;
		}

		outAreaArray.Add(castAreaName);
	}

	return outAreaArray.Num() > 0;
}

TArray<const FGsSchemaSpawnVisibleData*> UGsLevelUtil::GetSpawnVisibleData(int32 inLevelKey)
{
	TArray<const FGsSchemaSpawnVisibleData*> rowList;
	FString levelKey = FString::FromInt(inLevelKey);
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnVisibleData>(levelKey);
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaSpawnVisibleData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return rowList;
	}

	table->GetAllRows<FGsSchemaSpawnVisibleData>(rowList);
	return rowList;
}

bool UGsLevelUtil::IsLandscapeStreamingProxyLoaded(TArray<FString>& outLoadedLandProxy, TArray<FString>& outUnloadedLandProxy, UWorld* inWorld, const TArray<FString>& inSubLevelPackageList)
{
	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(inWorld, ALandscapeStreamingProxy::StaticClass(), actorList);
	bool isFullyLoad = true;
	outLoadedLandProxy.Empty();
	outUnloadedLandProxy.Empty();
	for (const FString& packageNameToLoad : inSubLevelPackageList)
	{		
		const FString& visibleLevelName = FPackageName::GetShortName(packageNameToLoad);

		bool isLoad = false;
		for (AActor* actor : actorList)
		{
			ALandscapeStreamingProxy* landscapeStreamingProxy = Cast<ALandscapeStreamingProxy>(actor);
			if (landscapeStreamingProxy)
			{
				const FString& landscapelongLevelName = landscapeStreamingProxy->GetLevel()->GetOuter()->GetName();
				const FString landscapeShortLevelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(landscapelongLevelName));

				if (visibleLevelName.Equals(landscapeShortLevelName))
				{
					isLoad = true;
					outLoadedLandProxy.Add(visibleLevelName);
					break;
				}
			}
		}

		if (false == isLoad)
		{
			outUnloadedLandProxy.Add(visibleLevelName);
			isFullyLoad = false;
		}
	}

	return isFullyLoad;
}

bool UGsLevelUtil::IsSubLevel(UWorld* inWorld, const FString& inLongPackage)
{
	const FString& visibleLevelName = FPackageName::GetShortName(inLongPackage);
	TArray<FString> parseList;
	if (3 > visibleLevelName.ParseIntoArray(parseList, TEXT("_")))
	{
		return false;
	}

	int32 last = parseList.Num();
	if (false == parseList[last - 2].Contains(TEXT("x")) && false == parseList[last - 2].Contains(TEXT("X")))
	{
		return false;
	}

	if (false == parseList[last - 1].Contains(TEXT("y")) && false == parseList[last - 1].Contains(TEXT("Y")))
	{
		return false;
	}

	/*const FString& currentLevelName = UGameplayStatics::GetCurrentLevelName(inWorld);	
	if (false == parseList[0].Contains(currentLevelName))
	{
		return false;
	}*/

	return true;
}

bool UGsLevelUtil::IsCanWarp(int64 inWarp)
{
	int32 currentLevelId = GLevel()->GetCurrentLevelId();
	int32 level = GGameData()->GetUserData()->mLevel;
	//int32 questId = GSQuest()->GetCurrentMainQuestId();
	int32 destLevelKey;
	int32 destSpotIndex;	
	if (UGsLevelManager::TryGetDestSpot(currentLevelId, inWarp, destLevelKey, destSpotIndex))
	{
		return UGsLevelManager::IsCanSpotWarp(destLevelKey, destSpotIndex, level/*, questId*/);
	}

	return false;
}

bool UGsLevelUtil::IsCanQuestWarp(QuestId inQuestId)
{
	int32 currentLevelId = GLevel()->GetCurrentLevelId();
	int32 level = GGameData()->GetUserData()->mLevel;
	int32 questId = GSQuest()->GetCurrentMainQuestId();
	TArray<const FGsSchemaQuest*> questList;
	GSQuest()->GetSchemaQuestList(questList);
	const FGsSchemaQuest** findQuestData = questList.FindByPredicate([&](const FGsSchemaQuest* questData) {
		return questData->id == inQuestId; });
	
	if (nullptr == findQuestData
		|| nullptr == (*findQuestData))
	{
		GSLOG(Error, TEXT("Quest missing - id : %ull"), inQuestId);
		return false;
	}	

	const TArray<FGsSchemaQuestObjectiveRow>& questObjectiveArray = (*findQuestData)->objectiveList;
	for (const FGsSchemaQuestObjectiveRow& questObjective : questObjectiveArray)
	{
		const FGsSchemaQuestObjective* quest = questObjective.GetRow();
		if (quest == nullptr)
		{
			continue;
		}
		
		const TArray<FGsSchemaQuestObjectiveAreaPos>& areaPosList = quest->posList;
		for (const FGsSchemaQuestObjectiveAreaPos& areaPos : areaPosList)
		{
			if (false == UGsLevelManager::IsCanSpotWarp(areaPos.mapId, areaPos.spotId, level/*, questId*/))
			{
				return false;
			}
		}
	}

	return true;
}

bool UGsLevelUtil::IsCanBookmarkWarp(uint64 inBookmarkId)
{
	FGsBookmarkData* bookmark = GSBookmark()->GetBookmarkDataById(inBookmarkId);
	if (nullptr == bookmark)
	{
		GSLOG(Error, TEXT("Bookmark missing - id : %ull"),inBookmarkId);
		return false;
	}

	int32 mapId = bookmark->GetMapId();
	int32 level = GGameData()->GetUserData()->mLevel;
	//QuestId questId = GSQuest()->GetCurrentMainQuestId();	

	return UGsLevelManager::IsCanMapWarp(mapId, level/*, questId*/);
}

