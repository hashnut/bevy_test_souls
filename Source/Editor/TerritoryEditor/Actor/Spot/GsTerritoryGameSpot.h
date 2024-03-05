// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritorySpot.h"

//ue4
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"

//t1 project
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"

//territory editor
#include "Actor/Spawn/GsTerritoryActor.h"

#include "GsTerritoryGameSpot.generated.h"

class UArrowComponent;
class USplineComopnent;
class UStaticMeshComponent;
class AGsTerritoryWarp;

UCLASS(hideCategories = (Display, ArrowComponent, Physics, Collision, Rendering, VirtualTexture, Tags, Activation, Cooking, Replication, Input, Actor, HLOD, Mobile, AsseteUserData))
class TERRITORYEDITOR_API AGsTerritoryGameSpot : public AGsTerritorySpot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Marker")
		IffTeamType _teamType;

public:
	virtual FString GetSpotDescription() override;	
};

UCLASS(hideCategories = (Display, ArrowComponent, Physics, Collision, Rendering, VirtualTexture, Tags, Activation, Cooking, Replication, Input, Actor, HLOD, Mobile, AsseteUserData))
class TERRITORYEDITOR_API AGsTerritorySpawnSpot : public AGsTerritoryGameSpot
{
	GENERATED_BODY()

public:
	AGsTerritorySpawnSpot();
	virtual void InvalidWidgetText() override;
	virtual const EGsTerritorySpotType GetSpotType() const override { return EGsTerritorySpotType::Spawn; }
};

UCLASS(hideCategories = (Display, ArrowComponent, Physics, Collision, Rendering, VirtualTexture, Tags, Activation, Cooking, Replication, Input, Actor, HLOD, Mobile, AsseteUserData))
class TERRITORYEDITOR_API AGsTerritoryResurrectionSpot : public AGsTerritoryGameSpot
{
	GENERATED_BODY()

public:
	AGsTerritoryResurrectionSpot();
	virtual void InvalidWidgetText() override;
	virtual const EGsTerritorySpotType GetSpotType() const override { return EGsTerritorySpotType::Resurrect; }
};

UCLASS(hideCategories = (Display, ArrowComponent, Physics, Collision, Rendering, VirtualTexture, Tags, Activation, Cooking, Replication, Input, Actor, HLOD, Mobile, AsseteUserData))
class TERRITORYEDITOR_API AGsTerritoryTeleportSpot : public AGsTerritoryGameSpot
{
	GENERATED_BODY()

public:
	AGsTerritoryTeleportSpot();
	virtual void InvalidWidgetText() override;
	virtual const EGsTerritorySpotType GetSpotType() const override { return EGsTerritorySpotType::Teleport; }
};