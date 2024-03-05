// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "GsTerritoryFenceBase.h"
#include "DataSchema/Map/GsSchemaMapTerritoryInfo.h"
#include "DataSchema/Map/Fence/GsSchemaFenceElement.h"
#include "GsTerritoryFenceTown.generated.h"

/**
 *
 */

UCLASS()
class TERRITORYEDITOR_API AGsTerritoryFenceTown : public AGsTerritoryFenceBase
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
	virtual TerritoryType GetFenceType() override { return TerritoryType::TOWN; }
	void SetSound(const FGsSchemaSoundResDataRow& inSound);
	void Export(OUT FGsSchemaMapTerritoryInfo& outTerritoryInfo, OUT FGsSchemaFenceElement& outFence);
	void Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo, const FGsSchemaFenceElement& inFence);
};
