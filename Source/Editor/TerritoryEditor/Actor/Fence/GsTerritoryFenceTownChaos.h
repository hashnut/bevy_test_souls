// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryFenceBase.h"
#include "DataSchema/Map/GsSchemaMapTerritoryEnv.h"
#include "DataSchema/Map/Fence/GsSchemaFenceElement.h"
#include "GsTerritoryFenceTownChaos.generated.h"

UCLASS()
class TERRITORYEDITOR_API AGsTerritoryFenceTownChaos : public AGsTerritoryFenceBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Territory", meta = (EditCondition = "(_territoryType == TOWN) || (_territoryType == SAFE_ZONE)"))
		FGsSchemaSoundResDataRow _sound;

public:
	void InitTownFence(FText inName, const FGsSchemaSoundResDataRow& inSound);

public:
	virtual void InvalidDestription() override;

public:
	void SetSound(const FGsSchemaSoundResDataRow& inSound);

public:
	void Export(OUT FGsSchemaMapTerritoryInfo& outTerritoryInfo, OUT FGsSchemaFenceElement& outFenceElement);
	void Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo, const FGsSchemaFenceElement& inFence);

public:
	virtual TerritoryType GetFenceType() override { return TerritoryType::TOWN_CHAOS; }
};
