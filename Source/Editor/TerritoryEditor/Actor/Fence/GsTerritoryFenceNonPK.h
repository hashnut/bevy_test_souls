// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "GsTerritoryFenceBase.h"
#include "DataSchema/Map/GsSchemaMapTerritoryInfo.h"
#include "DataSchema/Map/Fence/GsSchemaFenceElement.h"
#include "GsTerritoryFenceNonPK.generated.h"

/**
 *
 */

UCLASS()
class TERRITORYEDITOR_API AGsTerritoryFenceNonPK : public AGsTerritoryFenceBase
{
	GENERATED_BODY()

public:
	virtual void InvalidDestription() override;
	void Export(OUT FGsSchemaMapTerritoryInfo& outTerritoryInfo, OUT FGsSchemaFenceElement& outFence);
	void Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo, const FGsSchemaFenceElement& inFence);

public:
	virtual TerritoryType GetFenceType() override { return TerritoryType::NON_PK; }
};
