// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "TerritoryEditor.h"
#include "GsTerritoryFenceBase.generated.h"

/**
 *
 */
class UMaterial;
class UProceduralMeshComponent;
class USplineComponent;
class AGsTerritoryFenceZone;
class AGsTerrEdTerritory;

/*
* Fence 모양을 만들어주는 액터
*/

UCLASS()
class TERRITORYEDITOR_API AGsTerritoryFenceBase : public AGsTerritoryActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Territory fence")
		class USplineComponent* _splineComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Territory fence")
		class UChildActorComponent* _shapeActorChild;

public:	
	UPROPERTY(VisibleInstanceOnly)
		FVector _center;
	UPROPERTY(VisibleInstanceOnly)
		float _radius;	
	UPROPERTY(VisibleInstanceOnly, Category = "Territory zone")
		TArray<FVector> _points;

public:
	UPROPERTY(EditDefaultsOnly, Category = "TerritorySafetyZone")
		class UMaterial* _material;
	UPROPERTY(EditInstanceOnly, Category = "TerritorySafetyZone")
		FColor _pillarColor = FColor::Green;
	UPROPERTY(EditInstanceOnly, Category = "TerritorySafetyZone")
		FColor _verticalStripeColor = FColor::Red;
	UPROPERTY(EditInstanceOnly, Category = "TerritorySafetyZone")
		FColor _floorColor = FColor::Blue;
	UPROPERTY(EditInstanceOnly, Category = "TerritorySafetyZone")
		float _height = 100;

public:	
	UPROPERTY(EditInstanceOnly, Category = "Territory")
		FText _name;

public:	
	virtual void OnConstruction(const FTransform& inTransform) override;
	virtual void InvalidWidgetText() override;
	virtual void InvalidWidgetPosition(const FVector& in_location) override;
	virtual void DestroyManually() override;

public:
	AGsTerritoryFenceBase();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	//shape
public:
	virtual void InvalidShape() override;

protected:
	virtual void SetSplineLoopType();
	virtual void CreateView(const FTransform& in_transform);
	//Reset shape
	void InitShape(class USplineComponent* inSplineComp, class UChildActorComponent* inShapeActorChild, const FTransform& in_transform
		, OUT float& outRadius, OUT FVector& outCenter, OUT TArray<FVector>& outPointArray);
	void InitPolygon(class USplineComponent* inSplineComp, const FTransform& in_transform
		, OUT float& outRadius, OUT FVector& outCenter, OUT TArray<FVector>& outPointArray);
	void InitCircle(class USplineComponent* inSplineComp, const FTransform& in_transform
		, OUT float& outRadius, OUT FVector& outCenter, OUT TArray<FVector>& outPointArray);
	void InitSquare(class USplineComponent* inSplineComp, const FTransform& in_transform
		, OUT float& outRadius, OUT FVector& outCenter, OUT TArray<FVector>& outPointArray);
	virtual bool CheckDefaultShape(class UChildActorComponent* inShapeActorChild, OUT TArray<FVector>& outPointArray);
	virtual void SetDefaultShape(class USplineComponent* inSplineComp, class UChildActorComponent* inShapeActorChild, const FTransform& in_transform);
	void InitPointArray(class USplineComponent* inSplineComp, OUT TArray<FVector>& outPointArray);
	virtual void SelectInternal(UObject* in_select) override;

public:
	virtual const EGsTerritoryObjType GetTerritoryObjType() const override {return EGsTerritoryObjType::Fence;}	
	void KeepTerritorFenceZoneLocation(const FTransform& inTransform);
	virtual void Copy() override;
	//void Export(FGsSchemaFenceElement& outFence);
	void Import(const FGsSchemaFenceElement& inFence);
	virtual bool IsNeedToCheckOnNavmesh() override {return false;}	
	bool TryGetPointArray(OUT TArray<FVector>& outPointArray);
	bool TryGet2DPointArray(OUT TArray<FVector2D>& outPointArray);
	void SetPointArray(const TArray<FVector>& inPointArray);

	//copy
public:
	virtual bool IsNeedToCopy() override { return true; }

private:
	bool IsValidFenceNum();

public:
	virtual TerritoryType GetFenceType() { return TerritoryType::MAX; }

public:
	void SetName(FText inName);
};
