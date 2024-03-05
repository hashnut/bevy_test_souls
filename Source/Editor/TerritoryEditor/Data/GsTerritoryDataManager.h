// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define DIVIDE_TERRITORY_TABLE false

#include "CoreMinimal.h"

//t1 project
#include "DataSchema/Map/Spawn/GsSchemaTerritoryNpc.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "DataSchema/Map/Spawn/GsSchemaTerritoryZoneShape.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "DataSchema/Map/Spot/GsSchemaSpotData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "DataSchema/Map/GsSchemaMapWarpInfo.h"
#include "DataSchema/Map/GsSchemaMapWarpSpot.h"
#include "GsTableManagerEditor.h"
#include "GsTable.h"
#include "Actor/Spawn/GsTerritoryActor.h"
#include "UTIL/GsTableUtil.h"
#include "GsSchemaBase.h"
#include "DataSchema/Map/GsSchemaMapTerritorySpaceCrack.h"
#include "DataSchema/Map/GsSchemaMapTerritorySound.h"
#include "DataSchema/Map/GsSchemaMapTerritoryEnv.h"
#include "DataSchema/Map/GsSchemaMapTerritoryInfo.h"

class AGsTerritoryZone;
class AGsTerritorySpawnActor;

/**
 * Managing class about territory data save to GsTable and export raon json
 */
enum EGsTerritoryMapType
{
	Server,
	Design,
	Art,
};

class TERRITORYEDITOR_API FGsTerritoryDataBase
{
public:
	virtual ~FGsTerritoryDataBase(){}

public:
	virtual bool Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inLevel, const FString& inLevelKey, bool inSaveForce = false);
	virtual void Clear() {}

protected:
	virtual void SaveTable(ULevel* inLevel, const FString& inLevelKey){}
	virtual bool IsNeedToSave(ULevel* inLevel, const FString& inLevelKey){return false; }	
	virtual void Export(const FString& inLevelKey){}

public:
	virtual EGsTerritoryMapType GetMapType() {
		return EGsTerritoryMapType::Server;
	}

protected:
	template<typename SchemaClass = FGsSchemaBase, typename ActorClass = AGsTerritoryActor>
	TArray<SchemaClass> GetAllActorForSchema(ULevel* inLevel , const FString& inLevelName)
	{
		TArray<SchemaClass> schemaDataList;
		if (nullptr == inLevel)
		{
			return schemaDataList;
		}

		for (AActor* actor : inLevel->Actors)
		{
			if (nullptr == actor
				|| false == actor->IsA(StaticClass<ActorClass>()))
			{
				continue;
			}
	
			if (ULevel* level = actor->GetLevel())
			{
				if (UObject* outer = level->GetOuter())
				{
					if (outer->GetName().Equals(inLevelName))
					{
						if (ActorClass* temp = Cast<ActorClass>(actor))
						{
#if TERRITORY_EDITOR_DEBUG
							UE_LOG(LogTemp, Log, TEXT("FGsTerritoryDataBase::GetAllActorForSchema - export - label : %s"), *actor->GetActorLabel());
#endif
							SchemaClass schemaData;
							temp->Export(schemaData);
							schemaDataList.Add(schemaData);
						}
					}
				}
			}
		}

		return schemaDataList;
	}

	template<typename SchemaClass1 = FGsSchemaBase, typename ActorClass = AGsTerritoryActor>
	bool TryGetAllActorForSchema(ULevel* inLevel, const FString& inLevelName, OUT TArray<SchemaClass1>& outSchemaClass1Array)
	{
		if (nullptr == inLevel)
		{
			return false;
		}

		outSchemaClass1Array.Empty();		

		for (AActor* actor : inLevel->Actors)
		{
			if (nullptr == actor
				|| false == actor->IsA(StaticClass<ActorClass>()))
			{
				continue;
			}

			ULevel* level = actor->GetLevel();
			if (nullptr == level)
			{
				continue;
			}

			UObject* outer = level->GetOuter();
			if (nullptr == outer)
			{
				continue;
			}

			if (false == outer->GetName().Equals(inLevelName))
			{
				continue;
			}

			ActorClass* temp = Cast<ActorClass>(actor);
			if (nullptr == temp)
			{
				continue;
			}

#if TERRITORY_EDITOR_DEBUG
			UE_LOG(LogTemp, Log, TEXT("FGsTerritoryDataBase::GetAllActorForSchema - export - label : %s"), *actor->GetActorLabel());
#endif
			SchemaClass1 schemaData1;			
			temp->Export(schemaData1);
			outSchemaClass1Array.Add(schemaData1);			
		}

		return (outSchemaClass1Array.Num() > 0);
	}

	template<typename ActorClass = AGsTerritoryActor, typename SchemaClass1 = FGsSchemaBase, typename SchemaClass2 = FGsSchemaBase>
	bool TryGetAllActorForSchema(ULevel* inLevel, const FString& inLevelName, OUT TArray<SchemaClass1>& outSchemaClass1Array, OUT TArray<SchemaClass2>& outSchemaClass2Array)
	{
		if (nullptr == inLevel)
		{
			return false;
		}

		outSchemaClass1Array.Empty();
		outSchemaClass2Array.Empty();

		for (AActor* actor : inLevel->Actors)
		{
			if (nullptr == actor
				|| false == actor->IsA(StaticClass<ActorClass>()))
			{
				continue;
			}

			ULevel* level = actor->GetLevel();
			if (nullptr == level)
			{
				continue;
			}

			UObject* outer = level->GetOuter();
			if (nullptr == outer)
			{
				continue;
			}
						
			if (false == outer->GetName().Equals(inLevelName))
			{
				continue;
			}			

			ActorClass* temp = Cast<ActorClass>(actor);
			if (nullptr == temp)
			{
				continue;
			}
			
#if TERRITORY_EDITOR_DEBUG
			UE_LOG(LogTemp, Log, TEXT("FGsTerritoryDataBase::GetAllActorForSchema - export - label : %s"), *actor->GetActorLabel());
#endif
			SchemaClass1 schemaData1;
			SchemaClass2 schemaData2;
			temp->Export(schemaData1, schemaData2);
			outSchemaClass1Array.Add(schemaData1);
			outSchemaClass2Array.Add(schemaData2);
		}

		return (outSchemaClass1Array.Num() > 0) && (outSchemaClass2Array.Num() > 0);
	}

	template<typename ActorClass = AGsTerritoryActor>
	bool TryGetAllActor(ULevel* inLevel, const FString& inLevelName, OUT TArray<ActorClass*>& outActorArray)
	{		
		outActorArray.Empty();
		if (nullptr == inLevel)
		{
			return false;
		}

		TArray<AActor*>& actorArray = inLevel->Actors;
		for (AActor* actor : actorArray)
		{
			if (nullptr == actor
				|| false == actor->IsA(StaticClass<ActorClass>()))
			{
				continue;
			}

			if (ULevel* level = actor->GetLevel())
			{
				if (UObject* outer = level->GetOuter())
				{
					if (outer->GetName().Equals(inLevelName))
					{
						if (ActorClass* temp = Cast<ActorClass>(actor))
						{
#if TERRITORY_EDITOR_DEBUG
							UE_LOG(LogTemp, Log, TEXT("FGsTerritoryDataBase::GetAllActorForSchema - export - label : %s"), *actor->GetActorLabel());
#endif
							outActorArray.Add(temp);
						}
					}
				}
			}
		}

		return true;
	}

	template<typename SchemaClass = FGsSchemaBase, typename ActorClass = AGsTerritoryActor>
	TArray<SchemaClass> GetAllSpotForSchema(ULevel* inLevel, const FString& inLevelName)
	{
		TArray<SchemaClass> schemaDataList;
		if (nullptr == inLevel)
		{
			return schemaDataList;
		}

		for (AActor* actor : inLevel->Actors)
		{
			if (nullptr == actor
				|| false == actor->IsA(StaticClass<ActorClass>()))
			{
				continue;
			}

			if (ULevel* level = actor->GetLevel())
			{
				if (UObject* outer = level->GetOuter())
				{
					if (outer->GetName().Equals(inLevelName))
					{
						if (ActorClass* temp = Cast<ActorClass>(actor))
						{
#if TERRITORY_EDITOR_DEBUG
							UE_LOG(LogTemp, Log, TEXT("FGsTerritoryDataBase::GetAllActorForSchema - export - label : %s"), *actor->GetActorLabel());
#endif
							SchemaClass schemaData;;
							temp->Export(schemaData);
							schemaDataList.Add(schemaData);
						}
					}
				}
			}
		}

		return schemaDataList;
	}
};


class TERRITORYEDITOR_API FGsTerritoryDataManager
{
private:
	bool _isSaveValid;

public:	
	TArray<TSharedPtr<FGsTerritoryDataBase>> _autoSaveDataManager;
	TArray<TSharedPtr<FGsTerritoryDataBase>> _manualSaveDataManagerList;
	TWeakPtr<FGsTerritoryDataBase> _worldMapMonsterData;
	TWeakPtr<FGsTerritoryDataBase> _monsterKnowledgeMapData;
	TWeakPtr<FGsTerritoryDataBase> _monsterToMapData;

public:
	FGsTerritoryDataManager();
	~FGsTerritoryDataManager();
		
	//Warp
public:
	bool Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, bool inSaveForce = false);
	void SaveManually();

public:
	void Initialize();
	void Clear();	
	void SetSaveValid(bool inSave){_isSaveValid = inSave;}
	void Export();

private:
	void InitAutoSaveData();
	void ClearAutoSaveData();
	void InitManualSaveData();
	void ClearManualSaveData();

public:	
	static void ExportTable(UGsTable* inTable, const FString& inGroupName);

private:
	void CheckTerritoryDataMissing();

public:	
	template<typename schemaData, typename table>
	static bool CheckCreateDataTable(OUT UGsTable*& outTable, FString& outTableName, const FString& inLevelKey)
	{
		FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<table>(inLevelKey);
		outTableName = tableName;

		if (IsExistDataTable<schemaData>(tableName, outTable))
		{
			return false;
		}

		return TryCreateCustomDataTable<schemaData>(outTable, tableName);
	}

	template<typename schemaData>
	static UGsTable* GetCustomDataTable(const FString& inTableName)
	{
		UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
		return tableManager.GetTable(schemaData::StaticStruct(), inTableName);
	}

	template<typename schemaData>
	static bool IsExistDataTable(const FString& inTableName, OUT UGsTable*& outTable)
	{
		UGsTable* table = GetCustomDataTable<schemaData>(inTableName);
		if (table)
		{
			outTable = table;
		}

		return (table != nullptr);
	}

	template<typename schemaData>
	static bool TryCreateCustomDataTable(UGsTable*& outCreateTable, const FString& inTableName)
	{
		UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
		UGsTable* table = tableManager.CreateCustomTable(schemaData::StaticStruct(), inTableName, true);

		if (nullptr == table)
		{
			return false;
		}

		outCreateTable = table;

		return table->SaveDataTable();
	}

	template<typename schemaData>
	static bool TryClearCustomDataTable(const FString& inTableName)
	{
		UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(schemaData::StaticStruct(), inTableName);
		if (table)
		{
			table->Empty();
			table->SaveDataTable();

			return true;
		}

		return false;		 
	}

public:
	static void ClearAllTerritoryData();
	TWeakPtr<FGsTerritoryDataBase> GetWorldMapMonsterData() { return _worldMapMonsterData; }
	TWeakPtr<FGsTerritoryDataBase> GetMonsterKnowledgeMapData() { return _monsterKnowledgeMapData; }
	TWeakPtr<FGsTerritoryDataBase> GetMonsterToMapData() { return _monsterToMapData; }
};

template<typename schemaData>
static bool IsDifferentArray(const TArray<schemaData>& inSourceArray, const TArray<schemaData>& inDestArray)
{
	if (inSourceArray.Num() != inDestArray.Num())
	{
		return true;
	}

	for (int32 i = 0; i < inSourceArray.Num(); ++i)
	{
		if (inSourceArray[i] != inDestArray[i])
		{
			return true;
		}
	}

	return false;
}

static bool operator != (const FGsSchemaMapTerritoryInfo& inSource, const FGsSchemaMapTerritoryInfo& inDest)
{
	if (inSource.territoryId != inDest.territoryId
		|| inSource.territoryType != inDest.territoryType
		|| inSource.fenceId != inDest.fenceId
		|| inSource.sound != inDest.sound
		|| false == inSource.name.EqualTo(inDest.name)
		|| inSource.byClient != inDest.byClient)
	{
		return true;
	}

	return false;
}

static bool operator != (const FGsQuestSound& inSource, const FGsQuestSound& inDest)
{
	if (inSource._endQuestId != inDest._endQuestId
		|| inSource._sound != inDest._sound
		|| inSource._startQuestId != inDest._startQuestId)
	{
		return true;
	}

	return false;
}

static bool operator != (const FGsEnvCondition& inSource, const FGsEnvCondition& inDest)
{
	if (inSource._startQuestId != inDest._startQuestId
		|| inSource._endQuestId != inDest._endQuestId
		|| inSource._envEvent != inDest._envEvent)
	{
		return true;
	}

	return false;
}

static bool operator != (const FGsSchemaMapTerritoryEnv& inSource, const FGsSchemaMapTerritoryEnv& inDest)
{
	if (inSource.territoryId != inDest.territoryId
		|| inSource.fenceId != inDest.fenceId
		|| false == inSource.name.EqualTo(inDest.name))
	{
		return true;
	}

	return IsDifferentArray<FGsEnvCondition>(inSource.envConditionList, inDest.envConditionList);
}

static bool operator != (const FGsSchemaMapTerritorySound& inSource, const FGsSchemaMapTerritorySound& inDest)
{
	if (inSource.territoryId != inDest.territoryId
		|| inSource.fenceId != inDest.fenceId
		|| false == inSource.name.EqualTo(inDest.name))
	{
		return true;
	}

	return IsDifferentArray<FGsQuestSound>(inSource.questSoundList, inDest.questSoundList);
}

static bool operator != (const FGsSchemaMapTerritorySpaceCrack& inSource, const FGsSchemaMapTerritorySpaceCrack& inDest)
{
	if (inSource.territoryId != inDest.territoryId
		|| inSource.center != inDest.center
		|| inSource.radius != inDest.radius
		|| inSource.degree != inDest.degree)
	{
		return true;
	}

	return IsDifferentArray<FVector>(inSource.vertexList, inDest.vertexList);
}