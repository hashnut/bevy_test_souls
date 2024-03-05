// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/GsSelectionPrevantInterface.h"
#include "Define/GsTerritoryShapeDefine.h"
#include "Actor/GsTerritoryShapeInterface.h"
#include "Map/GsTerritoryShapeType.h"
#include "GsTerritoryShape.generated.h"

UCLASS(Blueprintable)
class TERRITORYEDITOR_API AGsTerritoryShape : public AActor
, public IGsSelectionPrevantInterface
, public IGsTerritoryShapeInterface
{
	GENERATED_BODY()

//View 
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsEditorBaseTerritory")
		float _height = DEFAULT_TERRITORY_FENCE_HEIGHT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsEditorBaseTerritory")
		FColor _pillarColor = FColor::Yellow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsEditorBaseTerritory")
		FColor _planeOuterColor = FColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsEditorBaseTerritory")
		FColor _planeInsideColor = FColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsEditorBaseTerritory")
		FColor _floorColor = FColor::Green;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsEditorBaseTerritory")
		class UMaterial* _material;

public:
	//Info	
	UPROPERTY(BlueprintReadOnly, Category = "GsEditorBaseTerritory")
		TArray<FVector> _pointArray;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsEditorBaseTerritory")
		class UGsTerritoryZonePolygonComponent* _floorComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsEditorBaseTerritory")
		class UGsTerritoryZoneVerticalStripeComponent* _fenceComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsEditorBaseTerritory")
		class UGsTerritoryZonePillarBunchComponent* _pillarBunchComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsEditorBaseTerritory")
		class USplineComponent* _splineComponent;

public:
	TWeakObjectPtr<class AGsTerritoryActor> _owner;
	EGsTerritoryShapeType _shapeType = EGsTerritoryShapeType::Circle;
	bool _isDrawFloor;

public:
	AGsTerritoryShape();
	virtual void OnConstruction(const FTransform& Transform) override;	
	virtual void BeginDestroy() override;

	//View
public:
	virtual void InitShape() {};	
	void InitPointArray(const TArray<FVector>& in_pointsArray);	
	virtual void DrawShape(const FTransform& in_transform, const TArray<FVector>& pointArray, float in_radius);

protected:
	virtual void DrawShape(const FTransform& in_transform);
	void InitPointArray();
	void DrawPlanes(bool in_close = true);
	void DrawPlillar(bool in_close = true);
	void DrawFloor(const FTransform& in_transform);
	void ClearFloor();
	
	//Util
protected:
	FVector GetRandomPointInTerritoryZone() const;

public:
	void TransparentFloor();

	//get set
public:
	//Get zone shape string
	UFUNCTION(BlueprintCallable, Category = "GsEditorBaseTerritory")
	virtual FString GetShapeTypeString() const { return ""; }
	UFUNCTION(BlueprintCallable, Category = "Territory editor")
	void SetDrawFloor(bool inDraw){_isDrawFloor = inDraw;}
	//Get Zone shape type
	UFUNCTION(BlueprintCallable, Category = "Territory editor")
	virtual EGsTerritoryShapeType GetShapeType() const { return _shapeType; }
	//Set pillar height
	void SetFenceHeight(float in_height) { _height = in_height;}
	//Active spline
	void ActiveSplineDebug(bool in_active = true);
	static void GetDefaultPoints(EGsTerritoryShapeType inShape, const FVector& inLocation, OUT TArray<FVector>& outPointArray, float inDefaultDistance = DEFAULT_TERRITORY_FENCE_DISTANCE, float inRot = 0);
	virtual bool IsInvalidePointNum(int32 in_num);
	FVector GetCenterBetweenPoints(int32 in_start, int32 in_end);
	void SetColors(const FColor& in_pillar, const FColor& in_fenceOuter, const FColor& in_fenceInner, const FColor& in_floor = FColor::Yellow);
	void SetShapeOwner(class AGsTerritoryActor* inZone);
	void SetColsedLoop(bool inClose);
	virtual bool IsClosedShape() const;
	virtual class UMaterial* GetMaterial() override;
};
