// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryLevelUtil.h"

//UE4
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/LevelEditorViewport.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "Editor/SceneOutliner/Public/SSceneOutliner.h"
#include "Editor/SceneOutliner/Public/ISceneOutliner.h"
#include "Editor/LevelEditor/Public/ILevelEditor.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

//T1 Project
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsDir.h"

#if TERRITORY_LEVEL_UTIL
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "DataSchema/Map/Spawn/GsSchemaTerritoryNpc.h"
#endif

//Territory editor
#include "Actor/Spot/GsTerritorySpot.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "GsTerritoryEditorSetting.h"
#include "TerritoryEditor.h"
#include "Map/Area/GsSchemaAreaData.h"
#include "GsTerritoryUniqueIDManager.h"
#include "UTIL/GsText.h"
#include "GameObject/Npc/GsSchemaNpcData.h"
#include "GameObject/GsSchemaShapeData.h"
#include "GsTable.h"
#include "GsSchemaBase.h"
#include "GsTableManagerEditor.h"
#include "DrawDebugHelpers.h"
#include "GsTerrEdSlateUtil.h"


EGsTerritoryActorInfoShow UGsTerritoryLevelUtil::_showTerritoryInfoType = EGsTerritoryActorInfoShow::HideAll;

FRotator UGsTerritoryLevelUtil::GetFaceRotationForEditorCamera(const FVector& in_actor)
{
	FLevelEditorViewportClient* client = (FLevelEditorViewportClient*)(GEditor->GetActiveViewport()->GetClient());
	if (client)
	{
		return UKismetMathLibrary::FindLookAtRotation(in_actor, client->GetViewLocation());
	}

	return FRotator::ZeroRotator;
}

bool UGsTerritoryLevelUtil::IsRegisteredLevel(const FString& inLevelKey, const FString& inLevelName)
{
	int32 id = FCString::Atoi(*inLevelKey);
	const UGsTableMapData* castMapTable = FGsTerritoryLevelManager::GetCachedMapData();
	if (castMapTable)
	{
		const FGsSchemaMapData* findMapData = nullptr;
		if (castMapTable->FindRowById(id, findMapData))
		{
			if (nullptr == findMapData->areaId.GetRow())
			{				
				UE_LOG(LogTemp, Error, TEXT("Area id is missing - id : %s"), *inLevelKey);
				return false;
			}

			return findMapData->areaId.GetRow()->levelName.Equals(inLevelName);
		}
	}

	return false;
}

bool UGsTerritoryLevelUtil::IsRegisteredLevel(const FString& inLevelName)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::IsRegisteredLevel"));

	const UGsTableMapData* castMapTable = FGsTerritoryLevelManager::GetCachedMapData();
	TArray<const FGsSchemaMapData*> mapDataArray;	
	castMapTable->GetAllRows<FGsSchemaMapData>(mapDataArray);

	for (const FGsSchemaMapData* mapData : mapDataArray)
	{
		if (mapData)
		{
			if (mapData->areaId.GetRow()
				&& mapData->areaId.GetRow()->levelName.Equals(inLevelName))
			{
				return true;
			}
		}
	}

	return false;
}

bool UGsTerritoryLevelUtil::IsValidServerLevel(const ULevel* inLevel, FString& outPersistantLevel, FString& outLevelKey)
{	
	if (nullptr == inLevel)
	{
		return false;
	}

	const FString& path = inLevel->GetPathName();
	if (false == UGsTerritoryLevelUtil::IsServerLevel(path))
	{
		return false;
	}

	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(path);
	if (false == UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, outPersistantLevel, outLevelKey))
	{
		return false;
	}

	return true;
}

bool UGsTerritoryLevelUtil::IsValidServerLevelName(const FString& inLevelName, FString& outPersistantLevel, FString& outLevelKey)
{
	TArray<FString> parses;
	FString levelName = inLevelName;
	levelName.ParseIntoArray(parses, TEXT("_"), true);
	if (parses.Num() != 3)
	{
		return false;
	}

	if (false == parses[2].Equals("Server"))
	{
		return false;
	}

	outPersistantLevel = parses[0];
	outLevelKey = parses[1];

	return true;
}

bool UGsTerritoryLevelUtil::IsValidDesignLevel(const ULevel* inLevel, FString& outPersistantLevel, FString& outLevelKey)
{
	if (nullptr == inLevel)
	{
		return false;
	}

	const FString& path = inLevel->GetPathName();
	if (false == UGsTerritoryLevelUtil::IsDesignLevel(path))
	{
		return false;
	}

	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(path);
	if (false == UGsTerritoryLevelUtil::IsValidDesignLevelName(levelName, outPersistantLevel, outLevelKey))
	{
		return false;
	}

	return true;
}

bool UGsTerritoryLevelUtil::IsValidDesignLevelName(const FString& inLevelName, FString& outPersistantLevel, FString& outLevelKey)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::IsValidDesignLevelName"));

	TArray<FString> parses;
	FString levelName = inLevelName;
	levelName.ParseIntoArray(parses, TEXT("_"), true);
	if (parses.Num() != 3)
	{
		return false;
	}

	if (false == parses[2].Equals("Design"))
	{
		return false;
	}

	outPersistantLevel = parses[0];
	outLevelKey = parses[1];

	return true;
}

void UGsTerritoryLevelUtil::LoadWorkLevels(UWorld* in_world, EGsTerritoryLevelType in_type)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::LoadWorkLevels"));

	TWeakPtr<FLevelCollectionModel> levelCollection;
	TArray<TSharedPtr<FLevelModel>> levelDesignArray;

	if (TryGetWorkLevels(levelCollection, levelDesignArray, in_world, in_type))
	{
		levelCollection.Pin()->LoadLevels(levelDesignArray);
	}
}

void UGsTerritoryLevelUtil::UnloadWorkLevels(UWorld* in_world, EGsTerritoryLevelType in_type)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::UnloadWorkLevels"));

	TWeakPtr<FLevelCollectionModel> levelCollection;
	TArray<TSharedPtr<FLevelModel>> levelDesignArray;

	if (TryGetWorkLevels(levelCollection, levelDesignArray, in_world, in_type))
	{
		levelCollection.Pin()->UnloadLevels(levelDesignArray);
	}
}

bool UGsTerritoryLevelUtil::TryGetWorkLevels(TWeakPtr<FLevelCollectionModel>& out_levelCollection, TArray<TSharedPtr<FLevelModel>>& out_levelDesignLevelList
	, UWorld* in_world
	, EGsTerritoryLevelType in_type)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::TryGetWorkLevels"));

	//get level collection from level browser
	TWeakPtr<FLevelCollectionModel> levelCollection;
	if (false == TryGetLevelCollection(levelCollection, in_world))
	{
		return false;
	}	

	const FLevelModelList& levelList = levelCollection.Pin()->GetAllLevels();
	out_levelDesignLevelList.Empty();

	for (TSharedPtr<FLevelModel> iter : levelList)
	{
		if (false == iter.IsValid())
		{
			continue;
		}

		const FString& longPakageName = iter.Get()->GetPackageFileName();
		if (in_type == EGsTerritoryLevelType::Design)
		{
			if (IsDesignLevel(longPakageName))
			{
				out_levelDesignLevelList.Add(iter);
			}
		}
		else if (in_type == EGsTerritoryLevelType::Server)
		{
			if (IsServerLevel(longPakageName))
			{
				out_levelDesignLevelList.Add(iter);
			}
		}
		else
		{		
			if (false == IsDesignLevel(longPakageName)
				&& false == IsServerLevel(longPakageName))
			{
				out_levelDesignLevelList.Add(iter);
			}
		}
	}

	out_levelCollection = levelCollection;

	return true;
}

bool UGsTerritoryLevelUtil::TryGetLevelCollection(TWeakPtr<FLevelCollectionModel>& out_levels, UWorld* in_world)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::TryGetLevelCollection"));

	if (in_world)
	{
		FWorldBrowserModule& worldBrowserModule = FModuleManager::LoadModuleChecked<FWorldBrowserModule>("WorldBrowser");
		TWeakPtr<FLevelCollectionModel> levelCollectionModelPtr = worldBrowserModule.SharedWorldModel(in_world);
		if (levelCollectionModelPtr.IsValid())
		{
			out_levels = levelCollectionModelPtr;
			return true;
		}
	}

	return false;
}

bool UGsTerritoryLevelUtil::CheckLevelType(const FName& in_folderName, const FString& in_levelType)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::CheckLevelType"));

	FString path = in_folderName.ToString();

	TArray<FString> stringArray;
	path.ParseIntoArray(stringArray, TEXT("/"), true);

	//
	if (stringArray.Num() > 0)
	{
		int last = stringArray.Num();
		int lastFolder = last - 2;

		if (stringArray.IsValidIndex(lastFolder))
		{
			const FString& folderName = stringArray[lastFolder];
			return (folderName.Equals(in_levelType));
		}
	}

	return false;
}

bool UGsTerritoryLevelUtil::IsActorInServerLevel(AActor* inActor)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::IsActorInServerLevel"));

	const FString& levelName = UGsLevelUtil::GetLevelNameFromActor(inActor).ToString();
	if (levelName.Contains(SERVER_LEVEL))
	{
		return true;
	}

	return false;
}

const TArray<AGsTerritoryZone*> UGsTerritoryLevelUtil::GetAllZones(UWorld* inWorld)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::GetAllZones"));

	TArray<AGsTerritoryZone*> zones;
	TArray<AActor*> currentZones;
	AGsTerritoryZone* zone;

	UGameplayStatics::GetAllActorsOfClass(inWorld, AGsTerritoryZone::StaticClass(), currentZones);
	for (AActor* actor : currentZones)
	{
		if (actor)
		{
			zone = Cast<AGsTerritoryZone>(actor);
			if (zone)
			{
				zones.Add(zone);
			}
		}
	}

	return zones;

}

const TArray<AGsTerritoryZone*> UGsTerritoryLevelUtil::GetAllZones(UWorld* inWorld, FName inLevel)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::GetAllZones"));

	TArray<AGsTerritoryZone*> zones;
	TArray<AActor*> currentZones;
	AGsTerritoryZone* zone;

	UGameplayStatics::GetAllActorsOfClass(inWorld, AGsTerritoryZone::StaticClass(), currentZones);

	for (AActor* actor : currentZones)
	{
		if (actor)
		{
			zone = Cast<AGsTerritoryZone>(actor);
			if (zone)
			{
				zones.Add(zone);
			}
		}
	}

	return zones;
}

const bool UGsTerritoryLevelUtil::TryGetAllTerritoryActor(ULevel* inLevel, EGsTerritoryObjType inType, TArray<AGsTerritoryActor*>& outActors)
{
	if (nullptr == inLevel)
	{
		return false;
	}

	TArray<AActor*>& actors = inLevel->Actors;

	outActors.Empty();

	for (AActor* actor : actors)
	{
		if (AGsTerritoryActor* territoryActor = Cast<AGsTerritoryActor>(actor))
		{
			if (territoryActor->GetTerritoryObjType() == inType)
			{
				outActors.Add(territoryActor);
			}
		}
	}



	/*TArray<ULevelStreaming*> levelArray = inWorld->GetStreamingLevels();

	ULevelStreaming** find = levelArray.FindByPredicate([&inLevel](ULevelStreaming* level)->bool {
		return (level != nullptr ? level->GetLoadedLevel() == inLevel : false);
	});

	if (nullptr == find || nullptr == (*find))
	{
		return false;
	}*/



	return true;
}

void UGsTerritoryLevelUtil::RemoveTerritorySpawnActor()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::RemoveTerritorySpawnActor"));

	AGsTerritorySpawnActor* selectedActor = FGsTerritoryEditorUtil::GetMouseSelectedObject<AGsTerritorySpawnActor>();
	if (selectedActor)
	{
		selectedActor->DestroyManually();
	}

	UE_LOG(LogTerritoryEditor, Error, TEXT("Territory actor aciton's select is null"));	
}

bool UGsTerritoryLevelUtil::IsServerLevel(const FString& inPath)
{
	if (false == IsLevelInServerFolder(inPath))
	{
		return false;
	}

	//경로 확인
	const FString& levelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(inPath));	

	//Check server level format
	FString levelKey;
	FString persistantLevelName;

	//Server레벨 형식에 맞는 이름인가
	if (false == IsValidServerLevelName(levelName, persistantLevelName, levelKey))
	{
		return false;
	}

	//MapData에 등록 되어 있는가?
	if (false == IsRegisteredLevel(levelKey, persistantLevelName))
	{
		return false;
	}

	return true;
}

bool UGsTerritoryLevelUtil::IsDesignLevel(const FString& inPath)
{
	if (false == IsLevelInDesignFolder(inPath))
	{
		return false;
	}

	//경로 확인
	const FString& levelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(inPath));	

	//Check server level format
	FString levelKey;
	FString persistantLevelName;

	//Server레벨 형식에 맞는 이름인가
	if (false == IsValidDesignLevelName(levelName, persistantLevelName, levelKey))
	{
		return false;
	}

	//MapData에 등록 되어 있는가?
	if (false == IsRegisteredLevel(levelKey, persistantLevelName))
	{
		return false;
	}

	return true;
}

bool UGsTerritoryLevelUtil::IsSpawnActorOnZone(AActor* inZone, AActor* inSpawnActor)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::IsSpawnActorOnZone"));

	return true;
}

bool UGsTerritoryLevelUtil::IsSpawnActorSpawnOnNavmesh(AActor* inSpawnActor, OUT FVector& outLocation)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return false;
	}

	AGsTerritoryActor* spawnActor = Cast<AGsTerritoryActor>(inSpawnActor);
	if (nullptr == spawnActor)
	{
		return false;
	}

	bool isOnNavmesh = false;
	const FVector& start = inSpawnActor->GetActorLocation();
	if (IsLocationOnNavmesh(start, outLocation))
	{
		isOnNavmesh = true;
	}	

	spawnActor->SetCanSpawnOnNavmesh(isOnNavmesh);
	spawnActor->InvalidWidgetText();

	return isOnNavmesh;
}

bool UGsTerritoryLevelUtil::IsLocationOnNavmesh(const FVector& inLocation, OUT FVector& outLocation)
{
	outLocation = inLocation;

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return false;
	}

	UNavigationSystemBase* navigaton = world->GetNavigationSystem();
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
	float gap = TNumericLimits<float>::Max();
	if (false == navigationSystemV1->ProjectPointToNavigation(inLocation, navLocaiton, FVector(1, 1, gap)))
	{
		return false;
	}

	outLocation = navLocaiton.Location;

	return true;
}

bool UGsTerritoryLevelUtil::CheckAllSpawnActorCanSpawnOnNavmesh(TArray<AGsTerritorySpawnActor*>& outActorList)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::CheckAllSpawnActorCanSpawnOnNavmesh"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world || nullptr == world->WorldComposition)
	{
		return false;
	}

	ULevel* currentLevel = world->GetCurrentLevel();
	if (nullptr == currentLevel)
	{
		return false;
	}

	if (false == IsServerLevel(currentLevel->GetPathName()))
	{
		return false;
	}

	bool isOk = true;
	TArray<AActor*>& actorArray = currentLevel->Actors;
	for (AActor* actor : actorArray)
	{
		if (nullptr == actor)
		{
			continue;
		}

		if (AGsTerritorySpawnActor* territoryActor = Cast<AGsTerritorySpawnActor>(actor))
		{
			if (territoryActor->IsNeedToCheckOnNavmesh())
			{
				FVector location;
				bool canSpawn = IsSpawnActorSpawnOnNavmesh(territoryActor, location);
				if (false == canSpawn)
				{
					DrawDebugLine(world, territoryActor->GetActorLocation(), location, FColor::Yellow, true);
					DrawDebugSphere(world, location, 100.0f, 12, FColor::Yellow, true);

					outActorList.Add(territoryActor);
					isOk = false;
				}
			}			
		}
	}

	return isOk;
}

void UGsTerritoryLevelUtil::SetSelectedLevelActors(const TArray<AActor*>& inActorList)
{	
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::SetSelectedLevelActors"));

	GEditor->GetSelectedActors()->Modify();

	// Clear the selection.
	GEditor->SelectNone(false, true, true);

	// We'll batch selection changes instead by using BeginBatchSelectOperation()
	GEditor->GetSelectedActors()->BeginBatchSelectOperation();

	const bool bShouldSelect = true;
	const bool bNotifyAfterSelect = false;
	const bool bSelectEvenIfHidden = true;	// @todo outliner: Is this actually OK?	
	for (AActor* actor : inActorList)
	{		
		GEditor->SelectActor(actor, bShouldSelect, bNotifyAfterSelect, bSelectEvenIfHidden);			
	}

	// Commit selection changes
	GEditor->GetSelectedActors()->EndBatchSelectOperation(/*bNotify*/false);

	// Fire selection changed event
	GEditor->NoteSelectionChange();
}

void UGsTerritoryLevelUtil::SetSelectedLevelActors(const TArray<AActor*>& inActorArray, TArray<TSharedPtr<FString>>& inSelectArray)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::SetSelectedLevelActors"));

	GEditor->GetSelectedActors()->Modify();

	// Clear the selection.
	GEditor->SelectNone(false, true, true);

	// We'll batch selection changes instead by using BeginBatchSelectOperation()
	GEditor->GetSelectedActors()->BeginBatchSelectOperation();

	if (inSelectArray.Num() != 0)
	{
		const bool bShouldSelect = true;
		const bool bNotifyAfterSelect = false;
		const bool bSelectEvenIfHidden = true;	// @todo outliner: Is this actually OK?			
		for (AActor* actor : inActorArray)
		{
			for (TSharedPtr<FString> item : inSelectArray)
			{
				if (actor->GetActorLabel().Equals(*item))
				{
					GEditor->SelectActor(actor, bShouldSelect, bNotifyAfterSelect, bSelectEvenIfHidden);					
				}
			}
		}		
	}
	

	// Commit selection changes
	GEditor->GetSelectedActors()->EndBatchSelectOperation(/*bNotify*/false);

	// Fire selection changed event
	GEditor->NoteSelectionChange();
}

void UGsTerritoryLevelUtil::DeselectAll()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::DeselectAll"));

	GEditor->SelectNone(true, true, true);
	
	USelection* select = GEditor->GetSelectedActors();
	if (select)
	{
		select->DeselectAll();
	}
}

bool UGsTerritoryLevelUtil::IsLevelInServerFolder(const FString& inServerLevel)
{
	return IsLevelInFolder(inServerLevel, "Server");
}

bool UGsTerritoryLevelUtil::IsLevelInDesignFolder(const FString& inDesignLevel)
{		
	return IsLevelInFolder(inDesignLevel, "Design");
}

bool UGsTerritoryLevelUtil::IsLevelInFolder(const FString& inLevelPath, const FString& inFolderName)
{
	FString path = inLevelPath;
	TArray<FString> parses;
	path.ParseIntoArray(parses, TEXT("/"), true);
	if (parses.Num() == 0)
	{
		return false;
	}

	if (parses.Num() < 2)
	{
		return false;
	}

	int serverFolderIndex = parses.Num() - 2;
	return parses[serverFolderIndex].Equals(inFolderName);
}

FString UGsTerritoryLevelUtil::GetLevelName(const ULevel* inLevel)
{
	const FString& levelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(inLevel->GetPathName()));
	return GetLevelName(levelName);
}

FString UGsTerritoryLevelUtil::GetLevelName(const FString& inLevelPath)
{
	TArray<FString> parses;
	FString levelPath = inLevelPath;
	levelPath.ParseIntoArray(parses, TEXT("."), true);
	if (parses.Num() > 0)
	{
		return parses[0];
	}

	return FString();
}

FString UGsTerritoryLevelUtil::GetLevelName(const int32 inLevelKey)
{		
	FString findText="";	
	UGsTable* areaTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaAreaData::StaticStruct());
	const UGsTableMapData* castMapTable = FGsTerritoryLevelManager::GetCachedMapData();
	const UGsTableAreaData* castAreaTable = Cast<UGsTableAreaData>(areaTable);

	TArray<const FGsSchemaAreaData*> areaList;
	castAreaTable->GetAllRows(areaList);
	if (castMapTable && castAreaTable)
	{
		const FGsSchemaMapData* findMapData = nullptr;
		const FGsSchemaAreaData* findAreaData = nullptr;
		
		if (castMapTable->FindRowById(inLevelKey, findMapData)
			&& findMapData
			&& findMapData->areaId.GetRow())
		{
			const FGsSchemaAreaData** findAreaName = areaList.FindByPredicate([&](const FGsSchemaAreaData*& area)->bool {
				return area->id == findMapData->areaId.GetRow()->id;
				});

			if (findAreaName)
			{
				return (*findAreaName)->levelName;
			}
		}
	}

	return findText;
}

FString UGsTerritoryLevelUtil::GetServerLevelName(const int32 inLevelKey)
{
	return GetLevelName(inLevelKey) + "_" + FString::FromInt(inLevelKey) + "_" + TEXT("Server");
}

ULevel* UGsTerritoryLevelUtil::GetCurrentServerLevel(UWorld* inWorld, FString& outLevelKey)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::GetCurrentServerLevel"));

	if (nullptr == inWorld)
	{
		return nullptr;
	}

	const FString& path = inWorld->GetPathName();
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	if (nullptr == world->WorldComposition)
	{
		return nullptr;
	}

	if (false == UGsTerritoryLevelUtil::IsServerLevel(path))
	{
		return nullptr;
	}

	ULevel* currentLevel = world->GetCurrentLevel();
	if (nullptr == currentLevel)
	{
		return nullptr;
	}

	FString levelKey;
	FString persistantLevel;
	const FString& levelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(path));

	UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevel, levelKey);

	outLevelKey = levelKey;

	return currentLevel;
}

TArray<AGsTerritoryZone*> UGsTerritoryLevelUtil::GetAllZone(ULevel* inLevel)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("UGsTerritoryLevelUtil::GetAllZone"));

	const TArray<AActor*>& actors = inLevel->Actors;
	TArray<AGsTerritoryZone*> zones;

	for (AActor* actor : actors)
	{
		if (actor)
		{
			if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor))
			{
				zones.Add(zone);
			}
		}
	}

	return zones;
}


const FGsSchemaNpcData* UGsTerritoryLevelUtil::FindNpcData(int32 inNpcId)
{
	const FGsSchemaNpcData* npc = nullptr;
	if (UGsTableNpcData* npcTbl = FGsTerritoryLevelManager::GetCachedNpcData())
	{
		npcTbl->FindRowById(inNpcId, npc);
		return npc;
	}

	return npc;
}

const FGsSchemaShapeData* UGsTerritoryLevelUtil::FindShapeData(int32 inShapeId)
{
	const FGsSchemaShapeData* shape = nullptr;
	if (UGsTableShapeData* shapeData = FGsTerritoryLevelManager::GetCachedShapeData())
	{
		FGsTerritoryLevelManager::GetCachedShapeData()->FindRowById(inShapeId, shape);
		return shape;
	}	

	return shape;
}

const FGsSchemaShapeData* UGsTerritoryLevelUtil::FindShapeDataByNpcId(int32 inNpcId)
{
	const FGsSchemaNpcData* npc = FindNpcData(inNpcId);
	if (npc)
	{
		return npc->shapeId.GetRow();
	}

	return nullptr;
}

const CreatureSpawnType UGsTerritoryLevelUtil::ConvertCreatureSpawnTypeToTerritorySpawnType(EGsTerritorySpawnType inType)
{
	switch (inType)
	{
	case EGsTerritorySpawnType::Npc:
		return CreatureSpawnType::NPC;
		break;
	case EGsTerritorySpawnType::Prop:
		return CreatureSpawnType::PROP;
		break;
	default:
		break;
	}

	return CreatureSpawnType::NONE;
}
