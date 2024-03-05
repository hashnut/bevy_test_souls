// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GsTerritoryDataManager.h"

struct FGsSchemaFenceElement;


/**
 * 
 */
class TERRITORYEDITOR_API FGsTerritoryBlockData: public FGsTerritoryDataBase
{
public:
	FGsTerritoryBlockData();
	virtual ~FGsTerritoryBlockData();

public:
	virtual EGsTerritoryMapType GetMapType() override {
		return EGsTerritoryMapType::Design;
	}

private:
	virtual bool Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce = false) override;
	bool IsNeedToSaveBlockData(const FString& inLevelKey, const TArray<class UBoxComponent*>& inBoxComponentArray);
	void SaveBlockData(const FString& inLevelKey, const TArray<class UBoxComponent*>& inBoxComponentArray);
	void Export(const FString& inLevelKey);
};
