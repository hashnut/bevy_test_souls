// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//t1 project
#include "DataSchema/Map/Spawn/GsSchemaTerritoryNpc.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "DataSchema/Map/Spawn/GsSchemaTerritoryZoneShape.h"

#include "Data/GsTerritoryDataManager.h"
#include "Map/Spawn/GsSchemaClientSpawnData.h"
#include "Map/Spawn/GsSchemaSpawnElement.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"

/**
 * 
 */
 class AGsTerrEdRTTActor;

class TERRITORYEDITOR_API FGsTerritoryUniqueData : public FGsTerritoryDataBase
{
public:
	FGsTerritoryUniqueData();
	virtual ~FGsTerritoryUniqueData();

public:
	virtual bool Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inLevel, const FString& inLevelKey, bool inSaveForce) override;
	
private:
	bool IsNeedToSaveRTT(const FString& inLevelKey, const AGsTerrEdRTTActor* inRTT);
	void SaveRTT(const FString& inLevelKey, const AGsTerrEdRTTActor* inRTT);
	void ExportRTT(const FString& inLevelKey);

};