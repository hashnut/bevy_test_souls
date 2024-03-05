// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryZoneBase.h"
#include "DataSchema/Map/GsSchemaMapTerritorySpaceCrack.h"
#include "GsTerritoryZoneSpaceCrack.generated.h"

/**
 *
 */

UCLASS(BlueprintType)
class TERRITORYEDITOR_API AGsTerritoryZoneSpaceCrack : public AGsTerritoryZoneBase
{
	GENERATED_BODY()

	//Editor
protected:
	AGsTerritoryZoneSpaceCrack();		

	//shape
protected:
	virtual void CreateView(const FTransform& in_transform) override;
	virtual bool CheckDefaultShape() override;
	virtual void SetDefaultShape(const FTransform& in_transform) override;	
	virtual void InitShape(const FTransform& in_transform) override;
	void InitSquare(const FTransform& in_transform);

public:
	virtual void InvalidDestription() override;
	void Export(OUT FGsSchemaMapTerritorySpaceCrack& outMapTerritorySpaceCrack);
	void Import(const FGsSchemaMapTerritorySpaceCrack& inSpaceCrack);
};
