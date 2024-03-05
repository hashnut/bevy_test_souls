// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZonePolygonShape.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"

void AGsTerritoryZonePolygonShape::InitZoneShape()
{
	if (_splineComponent)
	{
		_splineComponent->SetClosedLoop(true);

		if (_splineComponent->GetNumberOfSplinePoints() < DEFAULT_TERRITORY_POLYGON_POINT_NUM)
		{
			const FVector& origin = GetActorLocation();
			_splineComponent->ClearSplinePoints();
			_splineComponent->AddSplinePoint(origin, ESplineCoordinateSpace::World);
			_splineComponent->AddSplinePoint(origin + FVector(DEFAULT_TERRITORY_FENCE_DISTANCE, 0, 0), ESplineCoordinateSpace::World);
			_splineComponent->AddSplinePoint(origin + FVector(0, DEFAULT_TERRITORY_FENCE_DISTANCE, 0), ESplineCoordinateSpace::World);
		}

		InitPointArray();
	}
}

void AGsTerritoryZonePolygonShape::DrawZoneShape(const FTransform& in_transform)
{
	DrawPlillar();
	DrawPlanes();
	DrawFloor(in_transform);
}

bool AGsTerritoryZonePolygonShape::IsInvalidePointNum(int32 in_num) const
{
	return (in_num < DEFAULT_TERRITORY_POLYGON_POINT_NUM);
}

void AGsTerritoryZonePolygonShape::GetDefaultPoints(TArray<FVector>& out_pointArray) const
{	
	out_pointArray.Empty();
	FVector location;

	//v1
	location = FVector::ZeroVector;
	out_pointArray.Add(location);

	//v2
	location = FVector(DEFAULT_TERRITORY_FENCE_DISTANCE, 0, 0);
	out_pointArray.Add(location);

	//v3
	location = FVector(0, DEFAULT_TERRITORY_FENCE_DISTANCE, 0);
	out_pointArray.Add(location);
}

bool AGsTerritoryZonePolygonShape::TryGetTriangleArray(TArray<FPoly>& out_polyArray) const
{
	out_polyArray.Empty();
	FPoly poly;
	FVector temp;

	for (int i = 0; i < _pointArray.Num(); i++)
	{
		temp = _pointArray[i];
		poly.InsertVertex(i, temp);
	}

	if (poly.Triangulate(GetWorld()->GetDefaultBrush(), out_polyArray) > 0)
	{
		return true;
	}

	return false;
}