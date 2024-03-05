// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GsTerritoryDataManager.h"

//t1 project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"
#include "DataSchema/Map/GsSchemaMapTerritoryInfo.h"

/**
 * 
 */
class TERRITORYEDITOR_API FGsTerrEdTerritoryData : public FGsTerritoryDataBase
{
public:
	FGsTerrEdTerritoryData();
	virtual ~FGsTerrEdTerritoryData();

protected:
	virtual void SaveTable(ULevel* inLevel, const FString& inLevelKey) override;
	virtual bool IsNeedToSave(ULevel* inLevel, const FString& inLevelKey) override;	
};
