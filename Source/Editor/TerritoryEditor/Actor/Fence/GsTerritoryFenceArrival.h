// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryFenceBase.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "DataSchema/Map/GsSchemaMapTerritoryInfo.h"
#include "DataSchema/Map/Fence/GsSchemaFenceElement.h"
#include "GsTerritoryFenceArrival.generated.h"



UCLASS()
class TERRITORYEDITOR_API AGsTerritoryFenceArrival : public AGsTerritoryFenceBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Territory", meta = (EditCondition = "(_territoryType == TOWN) || (_territoryType == SAFE_ZONE)"))
		FGsSchemaSoundResDataRow _sound;

public:
	void InitArrivalFence(const FText& inName, FGsSchemaSoundResDataRow inSound);

public:
	virtual void InvalidDestription() override;
	void SetSound(const FGsSchemaSoundResDataRow& inSound);
	virtual TerritoryType GetFenceType() override { return TerritoryType::ARRIVAL; }
	void Export(OUT FGsSchemaMapTerritoryInfo& outTerritoryInfo, OUT FGsSchemaFenceElement& outFence);
	void Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo
		, const FGsSchemaFenceElement& inFence);
};
