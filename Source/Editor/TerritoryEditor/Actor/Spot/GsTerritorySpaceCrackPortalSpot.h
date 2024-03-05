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

#include "GsTerritorySpaceCrackPortalSpot.generated.h"

class UArrowComponent;
class USplineComopnent;
class UStaticMeshComponent;
class AGsTerritoryWarp;

UCLASS(hideCategories = (Display, ArrowComponent, Physics, Collision, Rendering, VirtualTexture, Tags, Activation, Cooking, Replication, Input, Actor, HLOD, Mobile, AsseteUserData))
class TERRITORYEDITOR_API AGsTerritorySpaceCrackPortalSpot : public AGsTerritorySpot
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, Category = "Warp")
		int32 _shapeID;

public:	
	AGsTerritorySpaceCrackPortalSpot();
	virtual void InvalidWidgetText() override;	
	virtual const EGsTerritorySpotType GetSpotType() const override { return EGsTerritorySpotType::SpaceCrackPortal; }
};