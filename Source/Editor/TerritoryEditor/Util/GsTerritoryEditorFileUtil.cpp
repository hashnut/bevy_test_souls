// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryEditorFileUtil.h"

//UE4
#include "Engine/Engine.h"
#include "Editor/EditorEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Public/HAL/FileManagerGeneric.h"
#include "AssetRegistryModule.h"
#include "LogMacros.h"
#include "IAssetTypeActions.h"

//T1 Project
#include "Level/GsLevelDefine.h"

#include "RSQLEditor/Private/AssetTypeActions/GsAssetTypeActionsGsDataTable.h"
#include "RSQLEditor/Public/GsTableManagerEditor.h"

#include "UTIL/GsDir.h"
#include "UTIL/GsTableUtil.h"

#include "DataSchema/Guild/GuildDungeon/GsSchemaGuildDungeon.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcMapInfo.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcMapData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/Map/Spawn/GsSchemaClientSpawnData.h"
#include "DataSchema/Map/Spawn/GsSchemaEventSpawnData.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackData.h"

//Territory editor
#include "Actor/Spot/GsTerritorySpot.h"
#include "GsTerritoryEditorSetting.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "UTIL/GsTableUtil.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"

const FString MAP_DIRECTORY = TEXT("Maps/");
const FString SEVER = TEXT("/Server");
const FString NEVER_COOK_SEVER_PRE = TEXT("(Path=\"/Game/Maps/");
const FString NEVER_COOK_SERVER_POST = TEXT("/Server\")");
const FString DEFAULT_GAME_PATH = FPaths::ProjectConfigDir() + TEXT("DefaultGame.ini");
const FString PACKAGE_SECTION_NAME = TEXT("/Script/UnrealEd.ProjectPackagingSettings");
const FName DIRECTORY_TO_NEVER_COOK = FName("+DirectoriesToNeverCook");
const FName MAPS_TO_COOK = FName("+MapsToCook");
const FString MAPS_TO_COOK_PRE = TEXT("(FilePath=\"/Game/Maps/");


void UGsTerritoryEditorFileUtil::SaveSpawnData(ULevel* inLevel, const FString& inFileName)
{
	//if (nullptr == inLevel)
	//{
	//	return;
	//}

	//UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	//UGsTable* table = tableManager.CreateCustomTable(FGsSchemaSpawnData::StaticStruct(), inFileName, true);

	//if(nullptr == table)
	//{
	//	return;
	//}

	//const TArray<AActor*>& actors = inLevel->Actors;
	//const TArray<AGsTerritoryZone*> zones;

	//table->Empty();

	//for(AActor* actor : actors)
	//{
	//	if(actor)
	//	{
	//		if(const AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor))
	//		{
	//			FGsSchemaSpawnData temp;
	//			FGsSchemaTerritoryZone newZone;
	//			FGsSchemaTerritoryZoneShape shape;

	//			shape.Center = zone->GetActorLocation();
	//			shape.Radius = static_cast<int32>(zone->_radius);

	//			newZone.Region = shape;
	//			//set zone uniuqe id
	//			newZone.SpawnGroupId = zone->GetTerritoryUniqueID();

	//			newZone.clientSpawn = zone->_clientSpawn;

	//			SaveZoneChild(newZone, zone);			

	//			table->AddRow(temp);
	//		}
	//	}
	//}


	//Load all server level
	//UGsTerritoryLevelUtil::LoadWorkLevels(inWorld, EGsTerritoryLevelType::Server);

	//persistant level name
	/*FName persistantLevelName = inWorld->GetLevel(0)->GetOuter()->GetFName();
	FString persistantLevelText = persistantLevelName.ToString();*/

	//Get manager
	/*UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();
	UGsTable * table = tableMgr.GetTable(FGsSchemaSpawnData::StaticStruct());

	if (nullptr == table)
	{
		return;
	}

	int32 levelKey = GetLevelIndex(persistantLevelText);

	if (INDEX_NONE == levelKey)
	{
		return;
	}

	table->RemoveRow(persistantLevelName);

	FGsSchemaSpawnData spawnData;
	spawnData.RowName = FName(*FString::FromInt(levelKey));

	if (TrySaveSpawnData(inWorld, spawnData))
	{
		table->AddRow(spawnData);
		table->SaveDataTable();
	}*/
}

#if TERRITORY_EDITOR_FILE_UTIL
bool UGsTerritoryEditorFileUtil::TrySaveSpawnData(UWorld* inWorld, FGsSchemaSpawnData& inTerritory)
{
	//Get zones
	const TArray<AGsTerritoryZone*>& zones = UGsTerritoryLevelUtil::GetAllZones(inWorld);

	if (zones.Num() == 0)
	{
		return false;
	}

	SaveZone(inTerritory, zones);

	return true;
}

void UGsTerritoryEditorFileUtil::SaveZone(FGsSchemaSpawnData& inTerritory, const TArray<AGsTerritoryZone*>& inZones)
{
	for (const AGsTerritoryZone* zone : inZones)
	{
		if (zone)
		{
			FGsSchemaTerritoryZone newZone;
			FGsSchemaTerritoryZoneShape shape;

			shape.Center = zone->GetActorLocation();
			shape.Radius = static_cast<int32>(zone->_radius);

			newZone.Region = shape;
			//set zone uniuqe id
			newZone.SpawnGroupId = zone->GetTerritoryUniqueID();

			newZone.byClient = zone->_byClient;

			SaveZoneChild(newZone, zone);

			inTerritory.SpawnGroupList.Add(newZone);
		}
	}
}

void UGsTerritoryEditorFileUtil::SaveZoneChild(FGsSchemaTerritoryZone& inRSQLZone, const AGsTerritoryZone* inZone)
{
	if (nullptr == inZone)
	{
		return;
	}

	for (const AGsTerritorySpawnActor* zoneChild : inZone->_spawnActors)
	{
		if (zoneChild)
		{
			if (const AGsTerritoryNpc * npc = Cast<AGsTerritoryNpc>(zoneChild))
			{
				FGsSchemaTerritoryNpc spawnInfo;
				FRotator rot = npc->GetActorRotation();

				spawnInfo.SpawnId = npc->GetTerritoryUniqueID();
				spawnInfo.Pos = npc->GetActorLocation();
				spawnInfo.Dir = FGsDir::RotatorToAngle(rot);
				spawnInfo.CreatureId = npc->_spawnInfo.TargetID;
				spawnInfo.SpawnCount = npc->_spawnInfo.MaxSpawnNum;
				spawnInfo.RespawnInterval = FTimespan::FromSeconds(npc->_spawnInfo.RespawnInterval).GetTotalMilliseconds();
				spawnInfo.groupLimitRange = npc->_spawnInfo.groupLimitRange;
				spawnInfo.RandomPos = npc->_spawnInfo.CanRandomLocationSpawn;				

				inRSQLZone.SpawnInfoList.Add(spawnInfo);
			}
		}
	}
}
#endif

void UGsTerritoryEditorFileUtil::ExportSpawnData()
{
#if TERRITORY_EDITOR_FILE_UTIL
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();

	if (UGsTable * table = tableMgr.GetTable(FGsSchemaSpawnData::StaticStruct()))
	{
		if (UGsTerritoryEditorSetting* setting = GetMutableDefault<UGsTerritoryEditorSetting>())
		{
			TArray<TSoftObjectPtr<UGsDataTable>> rsqlArray;

			rsqlArray.Add(table->GetCachedDataTable());

			FString root = FPaths::ProjectDir();
			FString absolutePath = FPaths::ConvertRelativePathToFull(root);

			FString path = absolutePath + setting->_raonJsonSavePath.Path;

			const FString OverrideGroupName(TEXT(""));
			UGsTableManagerEditor::ExportTerritoryDataTableJson(path, OverrideGroupName, &rsqlArray);
		}
	}
#endif
}

void UGsTerritoryEditorFileUtil::ImportSpawnData()
{
#if TERRITORY_EDITOR_FILE_UTIL
	FTerritoryEditor::_isCanSaveTerritoryData = false;

	UGsTerritoryLevelUtil::DestroyAllTerritoryActors();

	UWorld* world = GEditor->GetEditorWorldContext().World();

	if (nullptr == world)
	{
		return;
	}

	FString levelName = world->GetLevel(0)->GetOuter()->GetFName().ToString();

	//Check current server level index	
	int32 levelIndex = UGsTerritoryEditorFileUtil::GetLevelIndex(levelName);

	TWeakPtr<FLevelCollectionModel> serverLevelCollection;
	TArray<TSharedPtr<FLevelModel>> serverLevelModel;

	if (false == UGsTerritoryLevelUtil::TryGetWorkLevels(serverLevelCollection, serverLevelModel, world, EGsTerritoryLevelType::Server))
	{
		return;
	}

	ULevel* serverLevel = nullptr;

	if (serverLevelModel.Num() > 0)
	{
		if (serverLevelModel[0].Get())
		{
			serverLevel = serverLevelModel[0].Get()->GetLevelObject();
		}
	}

	if (nullptr == serverLevel)
	{
		return;
	}

	world->SetCurrentLevel(serverLevel);

	//Get spawn data at current server level from table
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();

	if (UGsTable * table = tableMgr.GetTable(FGsSchemaSpawnData::StaticStruct()))
	{
		FName rowName = FName(*FString::FromInt(levelIndex));

		const FGsSchemaSpawnData* spawnData = nullptr;

		if (table->FindRow<FGsSchemaSpawnData>(rowName, spawnData))
		{
			UGsTerritoryLevelUtil::CreateTerritoryActorFromDataTable(spawnData);
		}
	}

	FTerritoryEditor::_isCanSaveTerritoryData = true;
#endif
}

void UGsTerritoryEditorFileUtil::SaveMapMarkers(UWorld* inWorld)
{
#if TERRITORY_EDITOR_FILE_UTIL
	if (nullptr == inWorld)
	{
		return;
	}

	UGsTerritoryLevelUtil::LoadWorkLevels(inWorld, EGsTerritoryLevelType::Server);

	//Get persistant level name 	
	FString levelName = inWorld->GetLevel(0)->GetOuter()->GetFName().ToString();

	//Find level index
	int32 index = GetLevelIndex(levelName);

	if (INDEX_NONE == index)
	{
		return;
	}

	//Open map data info table
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();

	if (UGsTable * table = tableMgr.GetTable(FGsSchemaMapData::StaticStruct()))
	{
		FString indexName = FString::FromInt(index);
		FName rowName = FName(*indexName);

		table->RemoveRow(FName(*indexName));

		FGsSchemaMapData mapData;

		mapData.nameText = levelName;
		mapData.RowName = rowName;

		if (TrySaveMapData(inWorld, mapData))
		{
			table->AddRow(mapData);
			table->SaveDataTable();
		}
	}
#endif
}
#if TERRITORY_EDITOR_FILE_UTIL
bool UGsTerritoryEditorFileUtil::TrySaveMapData(UWorld* inWorld, FGsSchemaMapData& mapInfo)
{

	TArray<AActor*> spawnList;

	UGameplayStatics::GetAllActorsOfClass(inWorld, AGsSpawnMarker::StaticClass(), spawnList);

	for (AActor* marker : spawnList)
	{
		if (marker)
		{
			FGsSchemaMapMarkerInfo info;

			if (AGsSpawnMarker* spawnMarker = Cast<AGsSpawnMarker>(marker))
			{
				info.teamType = spawnMarker->_teamType;
				info.pos = spawnMarker->GetActorLocation();
				info.dir = FGsDir::RotatorToAngle(spawnMarker->GetActorRotation());
				info.posRandomRange = spawnMarker->_posRandomRange;

				mapInfo.spawnList.Add(info);
			}
		}
	}

	TArray<AActor*> resserectionList;

	UGameplayStatics::GetAllActorsOfClass(inWorld, AGsResserectionMarker::StaticClass(), resserectionList);

	for (AActor* marker : resserectionList)
	{
		if (marker)
		{
			FGsSchemaMapMarkerInfo info;

			if (AGsResserectionMarker* resserectionMarker = Cast<AGsResserectionMarker>(marker))
			{
				info.teamType = resserectionMarker->_teamType;
				info.pos = resserectionMarker->GetActorLocation();
				info.dir = FGsDir::RotatorToAngle(resserectionMarker->GetActorRotation());
				info.posRandomRange = resserectionMarker->_posRandomRange;

				mapInfo.resurrectList.Add(info);
			}
		}
	}

	return true;
}
#endif

void UGsTerritoryEditorFileUtil::ExportMapData()
{
#if TERRITORY_EDITOR_FILE_UTIL
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();

	if (UGsTable * table = tableMgr.GetTable(FGsSchemaMapData::StaticStruct()))
	{
		if (UGsTerritoryEditorSetting* setting = GetMutableDefault<UGsTerritoryEditorSetting>())
		{
			TArray<TSoftObjectPtr<UGsDataTable>> rsqlArray;

			rsqlArray.Add(table->GetCachedDataTable());

			FString root = FPaths::ProjectDir();
			FString absolutePath = FPaths::ConvertRelativePathToFull(root);
			FString path = absolutePath + setting->_raonJsonSavePath.Path;

			const FString OverrideGroupName(TEXT(""));
			UGsTableManagerEditor::ExportTerritoryDataTableJson(path, OverrideGroupName, &rsqlArray);
		}
	}
#endif
}

void UGsTerritoryEditorFileUtil::ImportMapData()
{
#if TERRITORY_EDITOR_FILE_UTIL
	FTerritoryEditor::_isCanSaveTerritoryData = false;

	UGsTerritoryLevelUtil::DestroyAllMapMarkers();

	UWorld* world = GEditor->GetEditorWorldContext().World();

	if (nullptr == world)
	{
		return;
	}

	FString levelName = world->GetLevel(0)->GetOuter()->GetFName().ToString();

	//Check current server level index	
	int32 levelIndex =	UGsTerritoryEditorFileUtil::GetLevelIndex(levelName);

	TWeakPtr<FLevelCollectionModel> serverLevelCollection;
	TArray<TSharedPtr<FLevelModel>> serverLevelModel;

	if (false == UGsTerritoryLevelUtil::TryGetWorkLevels(serverLevelCollection, serverLevelModel, world, EGsTerritoryLevelType::Server))
	{
		return;
	}
	
	ULevel* serverLevel = nullptr;
	
	if (serverLevelModel.Num() > 0)
	{
		if (serverLevelModel[0].Get())
		{
			serverLevel = serverLevelModel[0].Get()->GetLevelObject();
		}		
	}	

	if (nullptr == serverLevel)
	{
		return;
	}

	world->SetCurrentLevel(serverLevel);

	//Get map maker info at current server level from table
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();

	if (UGsTable * table = tableMgr.GetTable(FGsSchemaMapData::StaticStruct()))
	{
		FName rowName = FName(*FString::FromInt(levelIndex));

		const FGsSchemaMapData* mapData = nullptr;

		if (table->FindRowById<FGsSchemaMapData>(rowName, mapData))
		{
			UGsTerritoryLevelUtil::CreateMapMarkersFromDataTable(mapData);
		}	   
	}

	FTerritoryEditor::_isCanSaveTerritoryData = true;
#endif
}

//save ========================================================================================
void UGsTerritoryEditorFileUtil::RegisterServerLevelDirectorysToNeverCook()
{
	TArray<FString> serverMapPath;
	if (TryGetAllServerLevelDirectorys(serverMapPath))
	{
		serverMapPath = GetRegistedLevelInAreaData(serverMapPath);

		TArray<FString> unresisterServerMapPath;
		if (TryGetUnresisteredServerLevelDirectorysInNeverCook(unresisterServerMapPath, serverMapPath))
		{
			RegisterServerLevelsToNeverCook(unresisterServerMapPath);
		}
	}
}


void UGsTerritoryEditorFileUtil::RegisterLevelDirectorysToPakage()
{
	TArray<FString> levelPathList;
	if (TryGetAllLevelDirectorys(levelPathList))
	{
		TArray<FString> unresisterLevelPath;
		levelPathList = GetRegistedLevelInAreaData(levelPathList);

		if (TryGetUnregisteredLevelDirectorysInPakage(unresisterLevelPath, levelPathList))
		{
			RegisterLevelsToPakage(unresisterLevelPath);
		}
	}
}

bool UGsTerritoryEditorFileUtil::TryGetAllServerLevelDirectorys(TArray<FString>& OutDirecotrys)
{
	OutDirecotrys.Empty();

	//find all server map
	FString root = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*(FPaths::ProjectContentDir() + MAP_DIRECTORY));
	FString findRoot = root + "*";

	IFileManager& fileManager = FFileManagerGeneric::Get();

	TArray<FString> mapDirectory;

	fileManager.FindFiles(mapDirectory, *findRoot, false, true);

	for (FString& path : mapDirectory)
	{
		FString subDriectory = root + path + SEVER;
		TArray<FString> severDirectorys;

		if (FPaths::DirectoryExists(subDriectory))
		{
			FString temp = NEVER_COOK_SEVER_PRE + path + NEVER_COOK_SERVER_POST;
			//UE_LOG(LogTerritoryEditor, Log, TEXT("server map path : %s"), *temp);

			OutDirecotrys.Add(temp);
		}
	}

	return OutDirecotrys.Num() > 0;
}


bool UGsTerritoryEditorFileUtil::TryGetUnresisteredServerLevelDirectorysInNeverCook(TArray<FString>& OutPaths, TArray<FString>& InSeverMapPaths)
{
	//check unregistered servermap			
	OutPaths.Empty();

	if (FConfigFile* file = GConfig->Find(DEFAULT_GAME_PATH, false))
	{
		if (FConfigSection* section = file->Find(PACKAGE_SECTION_NAME))
		{
			TArray<FString> finds;
			section->MultiFind(DIRECTORY_TO_NEVER_COOK, finds);

			for (FString& path : InSeverMapPaths)
			{
				if (false == finds.Contains(path))
				{
					OutPaths.Add(path);
					//UE_LOG(LogTerritoryEditor, Log, TEXT("path : %s"), *path);
				}
			}
		}
	}

	return OutPaths.Num() > 0;
}


void UGsTerritoryEditorFileUtil::RegisterServerLevelsToNeverCook(TArray<FString>& InPaths)
{
	if (InPaths.Num() > 0)
	{
		if (FConfigFile* file = GConfig->Find(DEFAULT_GAME_PATH, false))
		{
			if (FConfigSection* section = file->Find(PACKAGE_SECTION_NAME))
			{
				for (FString& path : InPaths)
				{
					section->Add(DIRECTORY_TO_NEVER_COOK, path);
				}

				file->Dirty = true;

				GConfig->Flush(false, DEFAULT_GAME_PATH);
			}
		}
	}
}

bool UGsTerritoryEditorFileUtil::TryGetAllLevelDirectorys(TArray<FString>& OutDirecotrys)
{
	OutDirecotrys.Empty();

	//find all server map
	FString root = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*(FPaths::ProjectContentDir() + MAP_DIRECTORY));
	FString findRoot = root + "*";

	IFileManager& fileManager = FFileManagerGeneric::Get();

	TArray<FString> mapDirectory;

	fileManager.FindFiles(mapDirectory, *findRoot, false, true);

	for (FString& path : mapDirectory)
	{
		FString subDriectory = root + path;
		TArray<FString> severDirectorys;

		if (FPaths::DirectoryExists(subDriectory))
		{
			FString temp = MAPS_TO_COOK_PRE + path + TEXT("/") + path + TEXT("\")");
			//UE_LOG(LogTerritoryEditor, Log, TEXT("server map path : %s"), *temp);

			OutDirecotrys.Add(temp);
		}
	}

	return OutDirecotrys.Num() > 0;
}

bool UGsTerritoryEditorFileUtil::TryGetUnregisteredLevelDirectorysInPakage(TArray<FString>& OutDirecotrys, TArray<FString>& InMapPaths)
{
	//check unregistered servermap			
	OutDirecotrys.Empty();

	if (FConfigFile* file = GConfig->Find(DEFAULT_GAME_PATH, false))
	{
		if (FConfigSection* section = file->Find(PACKAGE_SECTION_NAME))
		{
			TArray<FString> finds;
			section->MultiFind(MAPS_TO_COOK, finds);

			for (FString& path : InMapPaths)
			{
				if (false == finds.Contains(path))
				{
					OutDirecotrys.Add(path);
					//UE_LOG(LogTerritoryEditor, Log, TEXT("path : %s"), *path);
				}
			}
		}
	}

	return OutDirecotrys.Num() > 0;
}

void UGsTerritoryEditorFileUtil::RegisterLevelsToPakage(TArray<FString>& InPaths)
{
	if (InPaths.Num() > 0)
	{
		if (FConfigFile* file = GConfig->Find(DEFAULT_GAME_PATH, false))
		{
			if (FConfigSection* section = file->Find(PACKAGE_SECTION_NAME))
			{
				for (FString& path : InPaths)
				{
					section->Add(MAPS_TO_COOK, path);
				}

				file->Dirty = true;

				GConfig->Flush(false, DEFAULT_GAME_PATH);
			}
		}
	}
}

TArray<FString> UGsTerritoryEditorFileUtil::GetRegistedLevelInAreaData(const TArray<FString>& inPathList)
{
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaAreaData::StaticStruct());

	TArray<const FGsSchemaAreaData*> areaList;
	table->GetAllRows<FGsSchemaAreaData>(areaList);
	TArray<FString> areaNameList;

	for (const FGsSchemaAreaData* area : areaList)
	{
		if (area)
		{
			areaNameList.Add(area->levelName);
		}
	}

	TArray<FString> pathList;
	for (const FString& path : inPathList)
	{
		for (const FString& areaName : areaNameList)
		{
			if (path.Contains(areaName))
			{
				pathList.Add(path);
				break;
			}
		}
	}

	return pathList;
}

UGsDataTable* UGsTerritoryEditorFileUtil::FindNpcDummyData()
{
	//get npc data
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTableNpcMapData* npcMapTable = Cast<UGsTableNpcMapData>(tableManager.GetTable(FGsSchemaNpcMapData::StaticStruct()));

	const UGsTableNpcData* npcDataTable = Cast<UGsTableNpcData>(FGsSchemaNpcData::GetStaticTable());
	//get map data
	const UGsTableMapData* mapDataTable = Cast<UGsTableMapData>(FGsSchemaMapData::GetStaticTable());
	const UGsTableGuildDungeon* guildDungeonTable = Cast<UGsTableGuildDungeon>(FGsSchemaGuildDungeon::GetStaticTable());
	const UGsTableFieldBossData* fieldBossTable = Cast<UGsTableFieldBossData>(FGsSchemaFieldBossData::GetStaticTable());
	const UGsTableSpaceCrackData* spaceCrackTable = Cast<UGsTableSpaceCrackData>(FGsSchemaSpaceCrackData::GetStaticTable());

	TArray<const FGsSchemaNpcData*> npcArray;
	npcDataTable->GetAllRows(npcArray);
	bool isDummy = true;

	//npc map table 비움
	npcMapTable->Empty();

	int32 index = 1;
	FGsSchemaNpcMapData npcMapData;
	for(const FGsSchemaNpcData* npc : npcArray)
	{
		isDummy = true;

		//get find npc data
		if (TryFindNpcInMap(mapDataTable, npc, guildDungeonTable, fieldBossTable, npcMapData))
		{
			isDummy = false;
		}

		/*if (TryFindNpcInSpaceCrack(spaceCrackTable, npc, npcMapData))
		{
			isDummy = false;
		}*/

		if (TryFindNpcInGuildDungeon(guildDungeonTable, npc, npcMapData))
		{
			isDummy = false;
		}

		if (TryFindNpcInFieldBoss(fieldBossTable, npc, mapDataTable, npcMapData))
		{
			isDummy = false;
		}

		const FGsSchemaNpcBalance* balance = npc->balanceId.GetRow();

		npcMapData.RowName = FName(*FString::FromInt(index));
		npcMapData.id = npc->id;	
		
		if (balance)
		{
			npcMapData.rank = npc->grade;
			npcMapData.balanceId = balance->id;
		}
		
		npcMapData.isDummy = isDummy;

		//npc map table 추가
		npcMapTable->AddRow(npcMapData);
		index++;
	}

	//npc map table 저장
	npcMapTable->SaveDataTable();

	return npcMapTable->GetCachedDataTable();
}

bool UGsTerritoryEditorFileUtil::TryFindNpcInMap(const class UGsTableMapData* inMapData
, const struct FGsSchemaNpcData* inNpcData
, const UGsTableGuildDungeon* inGuildDungeon
, const class UGsTableFieldBossData* inFieldBoss
, OUT FGsSchemaNpcMapData& outNpcMapData)
{
	TArray<const FGsSchemaMapData*> mapDataArray;
	inMapData->GetAllRows(mapDataArray);	
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();	
	FGsSchemaNpcMapInfo spawnDataInfo;
	FGsSchemaNpcMapInfo clientSpawnDataInfo;
	FGsSchemaNpcMapInfo eventSpawnDataInfo;
	int32 count;

	TArray<const FGsSchemaGuildDungeon*> guildDungeonArray;
	inGuildDungeon->GetAllRows(guildDungeonArray);

	TArray<const FGsSchemaFieldBossData*> fieldBossDataArray;
	inFieldBoss->GetAllRows(fieldBossDataArray);

	outNpcMapData.spawnDataArray.Empty();
	outNpcMapData.clientSpawnDataArray.Empty();
	outNpcMapData.eventSpawnDataArray.Empty();
	const FGsSchemaGuildDungeon** findGuild = nullptr;
	const FGsSchemaGuildDungeon* guildData = nullptr;
	const FGsSchemaFieldBossData** findFieldBoss = nullptr;
	const FGsSchemaFieldBossData* fieldBoss = nullptr;

	int32 mapId;
	for (const FGsSchemaMapData* mapData : mapDataArray)
	{
		//만약 테스트 레벨이거나, 시공의 틈새라면 제외
		mapId = mapData->id;
		if (mapId == 1 
		|| mapId == 0 
		|| mapData->isTestLevel
		|| mapData->ContentsType == MapContentsType::SPACE_CRACK)
		{
			continue;
		}	
		
		//해당 맵 길드 던전 데이터 찾기
		findGuild = guildDungeonArray.FindByPredicate([&](const FGsSchemaGuildDungeon* guildDungeon)->bool {
			return guildDungeon->mapId.GetRow()->id == mapId;
			});		
		guildData = findGuild ? (*findGuild) : nullptr;		

		//해당 맵 길드 던전 데이터 찾기
		findFieldBoss = fieldBossDataArray.FindByPredicate([&](const FGsSchemaFieldBossData* fieldBossData)->bool {
			return fieldBossData->mapId.GetRow()->id == mapId;
			});		
		fieldBoss = findFieldBoss ? (*findFieldBoss) : nullptr;		

		//get spawn data				
		const FString& spawnTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(FString::FromInt(mapId));		
		UGsTableSpawnData* spawnTable = Cast<UGsTableSpawnData>(tableMgr.GetTable(FGsSchemaSpawnData::StaticStruct(), spawnTableName));
		if (spawnTable)
		{
			if (TryFindNpcInSpawnData(spawnTable, inNpcData, guildData, count))
			{
				spawnDataInfo.MapId = mapData->id;
				spawnDataInfo.count = count;
				spawnDataInfo.contentType = mapData->ContentsType;
				spawnDataInfo.mapType = mapData->mapType;

				outNpcMapData.spawnDataArray.Add(spawnDataInfo);
			}			
		}
		
		//get client spawn data		
		const FString& clientSpawnTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableClientSpawnData>(FString::FromInt(mapId));
		if (UGsTableClientSpawnData* clientSpawnTable = Cast<UGsTableClientSpawnData>(tableMgr.GetTable(FGsSchemaClientSpawnData::StaticStruct(), clientSpawnTableName)))
		{
			if (TryFindNpcInClientSpawnData(clientSpawnTable, inNpcData, count))
			{
				spawnDataInfo.MapId = mapData->id;
				spawnDataInfo.count = count;
				spawnDataInfo.contentType = mapData->ContentsType;
				spawnDataInfo.mapType = mapData->mapType;

				outNpcMapData.clientSpawnDataArray.Add(spawnDataInfo);
			}			
		}

		const FString& eventSpawnTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableEventSpawnData>(FString::FromInt(mapId));
		if (UGsTableEventSpawnData* eventSpawnTable = Cast<UGsTableEventSpawnData>(tableMgr.GetTable(FGsSchemaEventSpawnData::StaticStruct(), eventSpawnTableName)))
		{
			if (TryFindNpcInEventSpawnData(eventSpawnTable, spawnTable, inNpcData, fieldBoss, count))
			{
				spawnDataInfo.MapId = mapData->id;
				spawnDataInfo.count = count;
				spawnDataInfo.contentType = mapData->ContentsType;
				spawnDataInfo.mapType = mapData->mapType;

				outNpcMapData.eventSpawnDataArray.Add(spawnDataInfo);
			}
		}
	}

	return ((outNpcMapData.spawnDataArray.Num() > 0)
		|| (outNpcMapData.clientSpawnDataArray.Num() > 0)
		|| (outNpcMapData.eventSpawnDataArray.Num() > 0));
}

bool UGsTerritoryEditorFileUtil::TryFindNpcInSpawnData(const class UGsTableSpawnData* inSpawnTable
, const FGsSchemaNpcData* inNpcData
, const FGsSchemaGuildDungeon* inGuildDungeon
, OUT int32& outCount)
{
	outCount = 0;
	TArray<const FGsSchemaSpawnData*> spawnDataArray;
	inSpawnTable->GetAllRows(spawnDataArray);	
	int32 guildDungeonBossId = (inGuildDungeon ? inGuildDungeon->bossId.GetRow()->id : -1);

	for (const FGsSchemaSpawnData* spawnData : spawnDataArray)
	{
		if (spawnData->byDefault
		&& false == spawnData->byClient)
		{
			for (const FGsSchemaSpawnElement& spawnEelement : spawnData->SpawnInfoList)
			{				
				if (spawnEelement.SpawnType != CreatureSpawnType::NPC)
				{
					continue;
				}
					
				if (inGuildDungeon)
				{
					if (guildDungeonBossId == spawnEelement.CreatureId)
					{
						continue;
					}
				}

				if (spawnEelement.CreatureId == inNpcData->id)
				{
					outCount++;
				}
			}
		}
	}

	return outCount > 0;
}

bool UGsTerritoryEditorFileUtil::TryFindNpcInClientSpawnData(const class UGsTableClientSpawnData* inSpawnTable
	, const FGsSchemaNpcData* inNpcData
	, OUT int32& outCount)
{
	outCount = 0;
	TArray<const FGsSchemaClientSpawnData*> spawnDataArray;
	inSpawnTable->GetAllRows(spawnDataArray);

	for (const FGsSchemaClientSpawnData* spawnData : spawnDataArray)
	{
		for (const FGsSchemaSpawnElement& spawnEelement : spawnData->SpawnInfoList)
		{
			if (spawnEelement.SpawnType == CreatureSpawnType::NPC)
			{
				if (spawnEelement.CreatureId == inNpcData->id)
				{
					outCount++;
				}
			}
		}
	}

	return outCount > 0;
}

bool UGsTerritoryEditorFileUtil::TryFindNpcInEventSpawnData(const class UGsTableEventSpawnData* inEventSpawnTable
, const class UGsTableSpawnData* inSpawnTable
, const struct FGsSchemaNpcData* inNpcData
, const struct FGsSchemaFieldBossData* inFieldBoss
, OUT int32& outCount)
{
	outCount = 0;
	TArray<const FGsSchemaEventSpawnData*> eventSpawnDataArray;
	inEventSpawnTable->GetAllRows(eventSpawnDataArray);
	TArray<int32> npcIdArray;
	int32 npcId = inNpcData->id;
	for (const FGsSchemaEventSpawnData* spawnData : eventSpawnDataArray)
	{
		if (inFieldBoss)
		{
			if (inFieldBoss->spawnGroupIdList.Contains(spawnData->SpawnGroupId))
			{
				continue;
			}
		}		

		npcIdArray = FindNpcIdArray(inSpawnTable, spawnData->SpawnGroupId);
		if (npcIdArray.Contains(npcId))
		{
			outCount++;
		}		
	}

	return outCount > 0;	
}

//bool UGsTerritoryEditorFileUtil::TryFindNpcInSpaceCrack(const class UGsTableSpaceCrackData* inSpaceCrack, const struct FGsSchemaNpcData* inNpcData, OUT FGsSchemaNpcMapData& outNpcMapData)
//{
//	int32 count = 0;
//	TArray<const FGsSchemaSpaceCrackData*> spaceCrackDataArray;
//	inSpaceCrack->GetAllRows(spaceCrackDataArray);
//	TArray<int32> npcIdArray;
//	int32 npcId = inNpcData->id;
//	FGsSchemaNpcMapInfo spawnDataInfo;	
//	const FGsSchemaMapData* destMapData = nullptr;
//
//	outNpcMapData.spaceCrackArray.Empty();
//
//	for (const FGsSchemaSpaceCrackData* spaceCrack : spaceCrackDataArray)
//	{
//		count = 0;
//		destMapData = spaceCrack->destMapId.GetRow();
//		if (destMapData->isTestLevel)
//		{
//			continue;
//		}
//
//		for (const FGsSchemaNpcDataRow& npcData : spaceCrack->monsterZakoList)
//		{
//			if (npcData.GetRow()->id == npcId)
//			{
//				count++;
//			}
//		}
//
//		for (const FGsSchemaNpcDataRow& npcData : spaceCrack->monsterEliteList)
//		{
//			if (npcData.GetRow()->id == npcId)
//			{
//				count++;
//			}
//		}	
//		
//		if (count > 0)
//		{
//			spawnDataInfo.MapId = destMapData->id;
//			spawnDataInfo.count = count;
//			spawnDataInfo.contentType = destMapData->ContentsType;
//			spawnDataInfo.mapType = destMapData->mapType;
//
//			outNpcMapData.spaceCrackArray.Add(spawnDataInfo);
//		}
//	}
//
//	return outNpcMapData.spaceCrackArray.Num() > 0;
//}

bool UGsTerritoryEditorFileUtil::TryFindNpcInGuildDungeon(const class UGsTableGuildDungeon* inGuildDungoen
, const struct FGsSchemaNpcData* inNpcData
, OUT FGsSchemaNpcMapData& outNpcMapData)
{	
	FGsSchemaNpcMapInfo spawnDataInfo;
	TArray<const FGsSchemaGuildDungeon*> guildDungeonDataArray;
	inGuildDungoen->GetAllRows(guildDungeonDataArray);	
	outNpcMapData.guildDungeonArray.Empty();
	const FGsSchemaMapData* mapData;
	for (const FGsSchemaGuildDungeon* guildDungeon : guildDungeonDataArray)
	{
		mapData = guildDungeon->mapId.GetRow();
		if (mapData->isTestLevel)
		{
			continue;
		}

		if (guildDungeon->bossId.GetRow()->id == inNpcData->id)
		{			
			spawnDataInfo.MapId = mapData->id;
			spawnDataInfo.contentType = mapData->ContentsType;
			spawnDataInfo.mapType = mapData->mapType;
			spawnDataInfo.count = 1;
			
			outNpcMapData.guildDungeonArray.Add(spawnDataInfo);
		}
	}

	return outNpcMapData.guildDungeonArray.Num() > 0;
}

bool UGsTerritoryEditorFileUtil::TryFindNpcInFieldBoss(const class UGsTableFieldBossData* inFieldBoss
, const struct FGsSchemaNpcData* inNpcData
, const class UGsTableMapData* inMapTable
, OUT FGsSchemaNpcMapData& outNpcMapData)
{
	FGsSchemaNpcMapInfo spawnDataInfo;
	TArray<const FGsSchemaFieldBossData*> fiedldBossDataArray;
	inFieldBoss->GetAllRows(fiedldBossDataArray);
	TArray<int32> npcIdArray;
	int32 npcId = inNpcData->id;
	int32 mapId;
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();
	const FGsSchemaMapData* mapData;

	outNpcMapData.fieldBossArray.Empty();
	for (const FGsSchemaFieldBossData* fieldBoss : fiedldBossDataArray)
	{
		mapData = fieldBoss->mapId.GetRow();
		if (mapData->isTestLevel)
		{
			continue;
		}

		for (int64 groupId : fieldBoss->spawnGroupIdList)
		{
			mapId = mapData->id;
			const FString& spawnTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(FString::FromInt(mapId));
			if (UGsTableSpawnData* spawnTable = Cast<UGsTableSpawnData>(tableMgr.GetTable(FGsSchemaSpawnData::StaticStruct(), spawnTableName)))
			{
				npcIdArray = FindNpcIdArray(spawnTable, groupId);
				if (npcIdArray.Contains(npcId))
				{
					spawnDataInfo.MapId = mapData->id;
					spawnDataInfo.contentType = mapData->ContentsType;
					spawnDataInfo.mapType = mapData->mapType;
					spawnDataInfo.count = 1;

					outNpcMapData.fieldBossArray.Add(spawnDataInfo);
				}
			}			
		}		
	}

	return outNpcMapData.fieldBossArray.Num() > 0;
}

const TArray<int32> UGsTerritoryEditorFileUtil::FindNpcIdArray(const class UGsTableSpawnData* inSpawnTable, int32 inGroupId)
{
	TArray<const FGsSchemaSpawnData*> spawnArray;
	TArray<int32> npcDataArray;
	inSpawnTable->GetAllRows(spawnArray);
	
	for(const FGsSchemaSpawnData* spawnData : spawnArray)
	{
		if (spawnData->SpawnGroupId == inGroupId)
		{
			for (const FGsSchemaSpawnElement& spawnElement : spawnData->SpawnInfoList)
			{
				npcDataArray.Add(spawnElement.CreatureId);
			}	
			
			return npcDataArray;
		}		
	}

	return npcDataArray;
}

void UGsTerritoryEditorFileUtil::ConvertMapTerritory()
{
	//get all map data
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();
	UGsTable* mapTable = tableMgr.GetTable(FGsSchemaMapData::StaticStruct());
	if (nullptr == mapTable)
	{
		return;
	}	

	//convert all map data
	TArray<const FGsSchemaMapData*> mapDataArray;
	mapTable->GetAllRows<FGsSchemaMapData>(mapDataArray);

	for (const FGsSchemaMapData* mapData : mapDataArray)
	{
		int32 mapId = mapData->id;
		FString levelKey = FString::FromInt(mapId);
		FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(FString::FromInt(mapId));
		UGsTable* mapTerritoryTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
		if (nullptr == mapTerritoryTable)
		{
			continue;
		}

		const FGsSchemaMapTerritoryData* row = nullptr;
		mapTerritoryTable->FindRow<FGsSchemaMapTerritoryData>(*levelKey, row);
		FGsSchemaMapTerritoryData newMapData;
		if (row)
		{
			newMapData = (*row);
			newMapData.RowName = *levelKey;

			for (const FGsSchemaMapTerritoryInfo& territoryInfo : row->territoryList)
			{
				switch (territoryInfo.territoryType)
				{
				case TerritoryType::SOUND:
				{
					FGsSchemaMapTerritorySound sound;

					sound.territoryId = territoryInfo.territoryId;
					sound.fenceId = territoryInfo.fenceId;
					sound.questSoundList = territoryInfo.questSoundArray;

					newMapData.soundTerritoryList.Add(sound);
				}
				break;
				case TerritoryType::Env:
				{
					FGsSchemaMapTerritoryEnv env;

					env.territoryId = territoryInfo.territoryId;
					env.fenceId = territoryInfo.fenceId;
					env.envConditionList = territoryInfo.envConditionArray;

					newMapData.envTerritoryList.Add(env);
				}
				break;
				default:
				{
					newMapData.territoryList.Add(territoryInfo);
				}
				break;
				}
			}

			mapTerritoryTable->RemoveRow(row);
			mapTerritoryTable->AddRow(newMapData);
			mapTerritoryTable->SaveDataTable();
		}		
	}


	//save all map data
}

void UGsTerritoryEditorFileUtil::ConvertRTTtoRTTList()
{
	//get map
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();	
	UGsTable* mapTable = tableManager.GetTable(FGsSchemaMapData::StaticStruct());

	TArray<const FGsSchemaMapData*> mapDataArray;
	mapTable->GetAllRows(mapDataArray);

	//
	for (const FGsSchemaMapData* mapData : mapDataArray)
	{
		FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(FString::FromInt(mapData->id));
		UGsTable* table = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
		if (nullptr == table)
		{
			continue;
		}

		const FGsSchemaMapTerritoryData* row = nullptr;
		table->FindRow<FGsSchemaMapTerritoryData>(*FString::FromInt(mapData->id), row);
		FGsSchemaMapTerritoryData newMapData;
		if (row)
		{
			newMapData = (*row);			
		}

		table->Empty();

		newMapData.RTTList.Add(newMapData.RTT);

		table->AddRow(newMapData);
		table->SaveDataTable();
	}
}
