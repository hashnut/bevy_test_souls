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

#include "GsTerritoryWarpSpot.generated.h"

class UArrowComponent;
class USplineComopnent;
class UStaticMeshComponent;
class AGsTerritoryWarp;

UCLASS(hideCategories = (Display, ArrowComponent, Physics, Collision, Rendering, VirtualTexture, Tags, Activation, Cooking, Replication, Input, Actor, HLOD, Mobile, AsseteUserData))
class TERRITORYEDITOR_API AGsTerritoryWarpSpot : public AGsTerritorySpot
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AGsTerritoryWarp* _warp;
	UPROPERTY(EditInstanceOnly, Category = "Warp")
	float _warpHeight = 500.0f;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Territory spawn actor")
		UChildActorComponent* _shapeChildActor;

public:
	AGsTerritoryWarpSpot();
	void OnConstruction(const FTransform& inTransform) override;
	virtual void InvalidWidgetText() override;	
	virtual FString GetSpotDescription() override;
	void SetWarp(AGsTerritoryWarp* inWarp);
	void KeepTerritorWarpLocation(const FTransform& inTransform);
	virtual void DestroyManually() override;	
	virtual void Copy() override;
	virtual bool IsNeedToCheckOnNavmesh() override { return true; }
	virtual bool InvalidSpotIndex() override;
	virtual const EGsTerritorySpotType GetSpotType() const override { return EGsTerritorySpotType::Warp; }
	virtual void InvalidShape() override;
	void SetWarpIndexForce(int64 inSpotInstanceId, int64 inWarpUniqueId);

	//copy
public:
	virtual bool IsNeedToCopy() override { return true; }
};