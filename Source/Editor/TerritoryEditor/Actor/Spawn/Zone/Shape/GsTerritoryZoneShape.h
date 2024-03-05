// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//UE4
#include "Runtime/Core/Public/Templates/SharedPointer.h"

//T1 Project
#include "Editor/GsSelectionPrevantInterface.h"

//Territory editor
#include "Define/GsTerritorySpawnDefine.h"
#include "Define/GsTerritoryZoneShapeDefine.h"
#include "Define/GsTerritoryShapeDefine.h"
#include "Actor/GsTerritoryShapeOwnerInterface.h"
#include "Actor/GsTerritoryShapeInterface.h"
#include "GsTerritoryZoneShape.generated.h"

class USceneComponent;
class AGsTerritoryZone;

///----------------------------------------------------------------
// Scene 내 이벤트에 필요한 영역과 기능을 관리해주는 에디터용 액터
//----------------------------------------------------------------

const int32 INVALIDE_TERIITORY_SPLINE_INDEX = -1;
//const FName TERRITORY_ZONE_BOTTOM_COLLISION_PRESET = TEXT("NoCollision");
const FName TERRITORY_ZONE_BOTTOM_COLLISION_PRESET = TEXT("Territory");
const FName TERRITORY_ZONE_COLLISION_PRESET = TEXT("ALLIgnore");

UENUM(BlueprintType)
enum class EGsTerritoryZoneShapeType : uint8
{
	Polygon,
	Circle,
	Line,
	Max,
};

UCLASS(Blueprintable)
class TERRITORYEDITOR_API AGsTerritoryZoneShape : public AActor
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

public:
	AGsTerritoryZoneShape();
	virtual void OnConstruction(const FTransform& Transform) override;	
	virtual void BeginDestroy() override;

	//View
public:
	virtual void InitZoneShape() {};	
	void InitPointArray(const TArray<FVector>& in_pointsArray);	
	virtual void DrawZoneShape(const FTransform& in_transform, const TArray<FVector>& pointArray, float in_radius);

protected:
	virtual void DrawZoneShape(const FTransform& in_transform);
	void InitPointArray();
	void DrawPlanes(bool in_close = true);
	void DrawPlillar(bool in_close = true);
	void DrawFloor(const FTransform& in_transform);
	
	//Util
protected:
	FVector GetRandomPointInTerritoryZone() const;

public:
	void TransparentFloor();

	//get set
public:
	//Get zone shape string
	UFUNCTION(BlueprintCallable, Category = "GsEditorBaseTerritory")
	virtual FString GetZoneShapeTypeString() const { return ""; }
	//Get Zone shape type
	UFUNCTION(BlueprintCallable, Category = "Territory editor")
	virtual EGsTerritoryZoneShapeType GetZoneShapeType() const { return EGsTerritoryZoneShapeType::Max; }
	//Set pillar height
	void SetFenceHeight(float in_height) { _height = in_height;}
	//Active spline
	void ActiveSplineDebug(bool in_active = true);
	virtual void GetDefaultPoints(TArray<FVector>& outPointArray) const {}
	virtual bool IsInvalidePointNum(int32 in_num) const { return false; }
	virtual bool IsClosedShape() const { return true; }
	FVector GetCenterBetweenPoints(int32 in_start, int32 in_end);
	void SetColors(const FColor& in_pillar, const FColor& in_fenceOuter, const FColor& in_fenceInner, const FColor& in_floor = FColor::Yellow);
	void SetShapeOwner(class AGsTerritoryActor* inZone);
	virtual class UMaterial* GetMaterial() override;
};
