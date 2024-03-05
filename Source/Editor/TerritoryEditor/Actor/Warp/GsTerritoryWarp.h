// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Spawn/GsTerritoryActor.h"

//t1 project
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/Map/GsSchemaMapWarpInfo.h"
#include "Editor/GsSelectionPrevantInterface.h"

#include "GsTerritoryWarp.generated.h"

/**
 * 
 */
 class UArrowComponent;
 class USplineComponent;
 class UStaticMeshComponent;
 class AGsTerritoryWarpSpot;

 const float DEFAULT_WARP_MESH_RADIUS = 50.0f;

UCLASS()
class TERRITORYEDITOR_API AGsTerritoryWarp : public AGsTerritoryActor, public IGsSelectionPrevantInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* _shapeComponent;
	UPROPERTY(VisibleAnywhere, Category = "Warp")
		AGsTerritoryWarpSpot* _warpSpot;
public:
	UPROPERTY(VisibleInstanceOnly, Category = "Warp")
		int32 _shapeID;
	UPROPERTY(VisibleInstanceOnly, Category = "Warp")
		int32 _sourceMapID;
	UPROPERTY(VisibleInstanceOnly, Category = "Warp")
		int64 _sourceSpotIndex;
	UPROPERTY(VisibleInstanceOnly, Category = "Warp")
		int32 _destMapID;
	UPROPERTY(VisibleInstanceOnly, Category = "Warp")
		int64 _destSpotIndex;
	UPROPERTY(EditInstanceOnly, Category = "Warp")
		WarpType _warpType;
	UPROPERTY(EditInstanceOnly, Category = "Warp")
		int32 _groupId = 0;

public:
	AGsTerritoryWarp();	
	void OnConstruction(const FTransform& inTransform);
	virtual void DestroyManually() override;

private:
	void RegisterWarp();

	//widget
public:
	virtual void InvalidWidgetText() override;
	virtual void InvalidWidgetPosition(const FVector& in_location);

public:
	virtual const EGsTerritoryObjType GetTerritoryObjType() const override { return EGsTerritoryObjType::Warp; }	
	void Export(FGsSchemaMapWarpInfo& outWarp);
	void SetWarpSpot(AGsTerritoryWarpSpot* inSpot);
	void InvalidLocation();
	virtual bool IsNeedToCheckOnNavmesh() override{return false;}
};

