
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

//ue4

//territory eidtor
#include "Define/GsTerritorySpawnDefine.h"
#include "Define/GsTerritoryZoneShapeDefine.h"
#include "GsTerritoryUniqueIDController.h"

//RSQL 저장 기능이 없어서 예전 방식으로 저장
#include "Define/GsTerritoryShapeDefine.h"
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "Define/GsTerritoryUniqueTable.h"

const FString UNIQUE_ID_GENERATE_INVALID = TEXT("Territory Actor를 위한 Unique ID를 발급할 수 없습니다.");
const FString INSTANCE_ID_TOO_LARGE = TEXT("Instance id가 1048575를 넘었습니다.");

class TERRITORYEDITOR_API FGsTerritoryUniqueIDManager
{
public:	
	TMap<FString, TSharedPtr<FGsTerritoryServerLevelInstanceIDController>> _serverLevelInstanceIDController;

	//Unique
private:
	bool _isGenerateUniqueIDValid;
	FString _mapName;
	FString _mapPath;	

private:	
	//Init unique id controller when map opened
	FDelegateHandle _serverLevelLoadedHandle;
	//export data when save
	FDelegateHandle _exportDataHandle;

public:	
	virtual ~FGsTerritoryUniqueIDManager();

public: 
	void Initialize();
	void Deinitialize();
	void Clear();

	//initialize unique id when server level load
public:
	void OnCheckServerLevelLoaded();
	bool InitServerLevelUniqueIDControllers(ULevel* inLevel);

	//check unique id when server level saved
public:	
	void OnPostSaveUniqueTable(uint32 in_saveFlag, class UWorld* in_world, bool in_success);
	void RefreshWorldBrowser();	

public:
	bool SetUniqueID(class AActor* inActor);	

	//Get Set
public:
	bool TryGenerateUniqueID(class ULevel* inLevel, EGsTerritoryObjType inObjType, int64& outUniqueID, int64& outInstanceID);
	bool TryGenerateUniqueID(class ULevel* inLevel, EGsTerritoryObjType inObjType, uint32& inInstanceID, int64& outUniqueID, int64& outInstanceID);
	void AddUniqueIdForce(class ULevel* inLevel, EGsTerritoryObjType inObjType, uint32& inInstanceID);
	bool IsCanGenerateUniqueID(class ULevel* inLevel, EGsTerritoryObjType inObjType);
	bool IsCanGenerateUniqueID(class ULevel* inLevel, EGsTerritoryObjType inObjType, uint32& inInstanceID, bool inIsForceCheck = false);
	bool IsGenerateUniqueIDValide(){return _isGenerateUniqueIDValid;}
	bool Remove(class ULevel* inLevelName, EGsTerritoryObjType inObjType, int64& inInstanceID);
	static FString CombineTerritoryActorName(const FString& inLevelKey, const EGsTerritoryObjType inObjType, const int64& inInstanceID);
	static FString CombineInvalidTerrritoryActorName(const EGsTerritoryObjType inObjType);
	static void InvalidTerritoryActorName(FString inLevelKey, class AGsTerritoryActor* inActor);
	void SetGenerateUniqueIDValid(bool inValid){ _isGenerateUniqueIDValid = inValid;}

private:
	FGsTerritoryServerLevelInstanceIDController* GetServerController(class ULevel* inLevel);
};