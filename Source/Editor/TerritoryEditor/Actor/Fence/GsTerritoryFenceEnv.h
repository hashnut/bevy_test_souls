// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryFenceBase.h"
#include "DataSchema/Map/GsSchemaMapTerritoryEnv.h"
#include "DataSchema/Map/Fence/GsSchemaFenceElement.h"
#include "GsTerritoryFenceEnv.generated.h"

UCLASS()
class TERRITORYEDITOR_API AGsTerritoryFenceEnv : public AGsTerritoryFenceBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Territory", meta = (EditCondition = "(_territoryType == Env)"))
		TArray<FGsEnvCondition> _envConditionArray;

public:
	virtual void InvalidDestription() override;

public:
	void InitFenceEnv(const FText& inName, const TArray<FGsEnvCondition>& inEnv);
	void SetEnvConditionArray(const TArray<FGsEnvCondition>& inEnvConditionArray) { _envConditionArray = inEnvConditionArray; }

public:
	void Export(OUT FGsSchemaMapTerritoryEnv& outMapEnv, OUT FGsSchemaFenceElement& outFenceElement);
	void Import(const FGsSchemaMapTerritoryEnv& inTerritoryInfo, const FGsSchemaFenceElement& inFence);
	void Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo, const FGsSchemaFenceElement& inFence);

public:
	virtual TerritoryType GetFenceType() override { return TerritoryType::Env; }
};
