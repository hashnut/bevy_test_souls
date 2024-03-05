// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//ue4
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"

//t1 project
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"

//territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Map/Spot/GsSchemaSpotData.h"

#include "GsTerritorySpot.generated.h"

const float DEFAULT_SPOT_MESH_RADIUS = 50.0f;

class UArrowComponent;
class USplineComopnent;
class UStaticMeshComponent;
class AGsTerritoryWarp;
class USplineComponent;
class UMaterial;

UENUM()
enum class EGsTerritorySpotType : uint8
{
	Normal,
	Spawn,
	Resurrect,
	Warp,
	Teleport,
	SpaceCrackPortal,
	Max
};


UCLASS(hideCategories=(Display, ArrowComponent, Physics, Collision, Rendering, VirtualTexture, Tags, Activation, Cooking, Replication, Input, Actor, HLOD, Mobile, AsseteUserData))
class TERRITORYEDITOR_API AGsTerritorySpot : public AGsTerritoryActor
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spot")
		class UMaterial* _material;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* _arrowComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class USplineComponent* _splineComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* _AreaComponent;

public:	
	UPROPERTY(EditInstanceOnly, Category = "Spot")
		float _radius = 50;
	UPROPERTY(VisibleInstanceOnly, Category = "Spot")
		int64 _index;
	UPROPERTY(EditInstanceOnly, Category = "Spot")
		FText _name;
	UPROPERTY(EditInstanceOnly, Category = "Spot")
		bool _randomDir;
	UPROPERTY(EditInstanceOnly, Category = "Spot")
		FColor _spotColor;	
	UPROPERTY(EditInstanceOnly, Category = "Spot")
		bool _isExceptRandomTeleport;

private:
	UPROPERTY()
		class UMaterial* _transMaterial;

public:
	// Sets default values for this actor's properties
	AGsTerritorySpot();		
	virtual void OnConstruction(const FTransform& inTransform) override;	

	//shape
public:
	virtual void InvalidShape() override;
	void TransparentFloor();

public:
	virtual const EGsTerritoryObjType GetTerritoryObjType() const override { return EGsTerritoryObjType::Spot; }

	//widget
protected:
	virtual FString GetSpotDescription(){return FString();}	
	bool TryGetDefualtColor(OUT FColor& outColor, EGsTerritorySpotType inSpotType);

public:
	virtual void SetTerritoryUniqueID(int64& inUniqueID, int64& inInstanceID) override;
	virtual bool SetIndexForce(uint32 inInstanceId);
	virtual void Export(FGsSchemaSpotInfo& outSpot);
	void SetText(const FText& inText);
	virtual bool InvalidSpotIndex(){return false;}
	virtual const EGsTerritorySpotType GetSpotType() const { return EGsTerritorySpotType::Max; }	
};

UCLASS(hideCategories = (Display, ArrowComponent, Physics, Collision, Rendering, VirtualTexture, Tags, Activation, Cooking, Replication, Input, Actor, HLOD, Mobile, AsseteUserData))
class TERRITORYEDITOR_API AGsTerritoryNormalSpot : public AGsTerritorySpot
{
	GENERATED_BODY()

public:	
	AGsTerritoryNormalSpot();
	virtual void InvalidWidgetText() override;
	virtual const EGsTerritorySpotType GetSpotType() const override { return EGsTerritorySpotType::Normal; }
	
};