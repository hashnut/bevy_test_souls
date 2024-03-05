// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Actor/GsTerritoryShapeOwnerInterface.h"
#include "Shape/GsTerritoryZoneShape.h"
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "GsTerritoryZoneBase.generated.h"

/**
 *
 */

UCLASS(BlueprintType)
class TERRITORYEDITOR_API AGsTerritoryZoneBase : public AGsTerritoryActor, public IGsTerritoryShapeOwnerInterface
{
	GENERATED_BODY()

public:
	//View
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Territory zone")
		class USplineComponent* _splineComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Territory zone")
		class UChildActorComponent* _shapeActorChild;

public:
	//View setting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory zone view")
		float _height = DEFAULT_TERRITORY_FENCE_HEIGHT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory zone view")
		FColor _pillarColor = FColor::Yellow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory zone view")
		FColor _fenceOuterColor = FColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory zone view")
		FColor _fenceInsideColor = FColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory zone view")
		FColor _floorColor = FColor::Green;

public:
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		float _radius = DEFAULT_TERRITORY_FENCE_DISTANCE;
	UPROPERTY(VisibleInstanceOnly, Category = "Territory zone")
		TArray<FVector> _points;

	//Editor
protected:
	AGsTerritoryZoneBase();
	void PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent);
	
protected:
	void ActiveSelectOutline(bool in_activeOutline);
	virtual bool CheckDefaultShape();
	virtual void SetSplineLoopType();	

	//shape
public:	
	virtual void InvalidShape() override;
	virtual void CreateView(const FTransform& in_transform);

	//Reset shape
	virtual void InitShape(const FTransform& in_transform);
	void InitCircle(const FTransform& in_transform);

	//Initialize default shape
	virtual void SetDefaultShape(const FTransform& in_transform);
	void InitPointArray();

	void TransparentFloor();

	virtual void SelectInternal(UObject* in_select) override;
	virtual void CopyZoneInfo(class AGsTerritoryZoneBase* inZone);

public:
	virtual const EGsTerritoryObjType GetTerritoryObjType() const override { return EGsTerritoryObjType::Zone; }	
	EGsTerritoryZoneShapeType GetShapeType() const;
	TArray<FVector> GetPoints();

};
