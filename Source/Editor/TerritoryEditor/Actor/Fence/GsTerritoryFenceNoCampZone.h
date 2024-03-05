// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "GsTerritoryFenceBase.h"
#include "DataSchema/Map/GsSchemaMapTerritoryInfo.h"
#include "DataSchema/Map/Fence/GsSchemaFenceElement.h"
#include "GsTerritoryFenceNoCampZone.generated.h"

/**
 *
 */

UCLASS()
class TERRITORYEDITOR_API AGsTerritoryFenceNoCampZone : public AGsTerritoryFenceBase
{
	GENERATED_BODY()

public:
	virtual void InvalidDestription() override;

public:
	virtual TerritoryType GetFenceType() override { return TerritoryType::NO_CAMP_ZONE; }	
	void Export(OUT FGsSchemaMapTerritoryInfo& outTerritoryInfo, OUT FGsSchemaFenceElement& outFence);
	void Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo, const FGsSchemaFenceElement& inFence);
};
