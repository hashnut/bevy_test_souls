// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/TerritoryEditor/Actor/Spawn\GsTerritoryActor.h"
#include "Map/GsLineShapeType.h"
#include "Define/GsTerritorySpawnDefine.h"
#include "Map/Line/GsSchemaLineData.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Actor/GsTerritoryShapeOwnerInterface.h"
#include "GsTerritoryLine.generated.h"

/**
 *
 */

UENUM()
enum class EGsTerritoryLineType : uint8
{
	Patrol,
	Max
};


UCLASS(BlueprintType)
class TERRITORYEDITOR_API AGsTerritoryLine : public AGsTerritoryActor, public IGsTerritoryShapeOwnerInterface
{
	GENERATED_BODY()

	//View
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsTerritoryLine")
		class USplineComponent* _splineComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsTerritoryLine")
		class UChildActorComponent* _shapeActorChild;

	//Data
public:			
	UPROPERTY(VisibleInstanceOnly, Category = "GsTerritoryLine")
		EGsTerritoryShapeType _lineShape;
	UPROPERTY(VisibleInstanceOnly, Category = "GsTerritoryLine")
		float _distance;
	UPROPERTY(EditInstanceOnly, Category = "GsTerritoryLine", meta=(EditCondition =" _lineShape == EGsTerritoryShapeType::Circle"))
		float _radius;
	UPROPERTY(VisibleInstanceOnly, Category = "GsTerritoryLine")
		TArray<FVector> _points;	

public:
	//View setting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsTerritoryLine")
		float _height = DEFAULT_TERRITORY_FENCE_HEIGHT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsTerritoryLine")
		FColor _pillarColor = FColor::Yellow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsTerritoryLine")
		FColor _fenceOuterColor = FColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsTerritoryLine")
		FColor _fenceInsideColor = FColor::Red;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsTerritoryLine")
		float _indexTextHeight = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsTerritoryLine")
		FColor _indexTextColor = FColor::Black;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsTerritoryLine")
		float _indexTextSize = 5;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "GsTerritoryLine")
		TArray<class UTextRenderComponent*> _textRenderComponentArray;

public:
	AGsTerritoryLine();
	virtual void OnConstruction(const FTransform& Transform) override;		
	virtual void PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent) override;
	virtual void Destroyed() override;

public:	
	virtual void DestroyManually() override;
	
	//Editor
protected:	
	void ActiveSelectOutline(bool in_activeOutline);

	//View
public:
	void CreateView(const FTransform& in_transform);
	virtual void InvalidShape() override;

protected:	
	void SetSplineLoopType();
	bool CheckDefaultShape();

	//Reset shape
	void InitShape(const FTransform& in_transform);
	void InitCircle(const FTransform& in_transform);

	//Initialize default shape
	void SetDefaultShape(const FTransform& in_transform);
	void InitPointArray();

	//widget
public:	
	virtual void InvalidWidgetPosition(const FVector& in_location) override;
	virtual void InvalidDestription() override;

private:
	class AActor* CreateTerritoryTemplate(const FGsTerritorySpawnInfo& in_data, const FVector& in_worldLocation);
	bool IsValideKey(const FGsTerritorySpawnInfo& inData) const;

	//UniqueID
public:
	//Temp function for revcovery uniuqeID
	virtual bool RecoveryTerritoryUniqueID() override;

public:
	//Copy child actor, when actor created
	virtual void Copy() override;

public:
	virtual void SelectInternal(UObject* in_select) override;

public:
	void ChangeLineShape(float inRot, float inDist);
	void ChangeCircleShape(float inRadius);
	void ChangePolygonShape();

public:
	virtual void OptimizePath();

	// dat export
public:
	virtual void Export(OUT FGsSchemaLineElement& outSpawnData);
	void Import(const FGsSchemaLineElement* inSpawnData);

public://get set
	virtual const EGsTerritoryObjType GetTerritoryObjType() const override { return EGsTerritoryObjType::Line; }		
	EGsLineShapeType GetShapeType() const;
	virtual void CopyLine(AGsTerritoryLine* zone);
	TArray<FVector> GetPoints();	

	//copy
public:
	virtual bool IsNeedToCopy() override {return true;}
	virtual EGsTerritoryLineType GetLineType(){return EGsTerritoryLineType::Max;}
	EGsTerritoryShapeType GetShapeType(){return _lineShape;}
};
