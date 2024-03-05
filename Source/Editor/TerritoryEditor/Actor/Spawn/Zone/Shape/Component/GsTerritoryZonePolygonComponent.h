// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/Polys.h"
#include "GsTerritoryZonePolygonComponent.generated.h"

class AGsTerritoryZoneShape;
class UMaterialInstanceDynamic;
/**
 * 
 */
UCLASS()
class TERRITORYEDITOR_API UGsTerritoryZonePolygonComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:
	TWeakObjectPtr<class AActor> _shape;
	UPROPERTY()
		bool _isCanCollision = false;	
	UPROPERTY()
		FColor _color;

public:
	UGsTerritoryZonePolygonComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginDestroy() override;

public:
	void Draw(const FTransform& in_transform, const FColor& in_color, const TArray<FVector>& in_pointArray);
	bool TryGetTrianglesFromVertices(
		const FTransform& in_transform, const TArray<FVector>& in_pointArray, TArray<FPoly>& out_polyArray
	) const;
	void TransparentFloor();

private:	
	void SetNoCollision();
	void SetCollision();

public:
	void Clear();

public:
	void GetVertexArray(const TArray<FPoly>& in_polyArray, TArray<FVector>& out_vertexArray) const;
	bool TryGetIndexArray(
		const TArray<FVector>& in_vertexArray, const TArray<FPoly>& in_polyArray, TArray<int32>& out_indexArray
	) const;
	float GetMaxRadius(const TArray<FVector>& in_vertexArray, FVector& out_center) const;	
};
