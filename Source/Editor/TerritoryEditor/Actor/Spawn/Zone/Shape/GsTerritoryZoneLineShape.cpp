// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZoneLineShape.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Define/GsTerritoryShapeDefine.h"

AGsTerritoryZoneLineShape::AGsTerritoryZoneLineShape()
{
	if (_splineComponent)
	{
		_splineComponent->SetClosedLoop(false);
	}
}

void AGsTerritoryZoneLineShape::InitZoneShape()
{
	if (_splineComponent)
	{
		int32 num = _splineComponent->GetNumberOfSplinePoints();

		if (num < DEFAULT_TERRITORY_LINE_POINT_NUM)
		{
			FVector origin = GetActorLocation();

			_splineComponent->ClearSplinePoints();

			_splineComponent->AddSplinePoint(origin, ESplineCoordinateSpace::World);
			_splineComponent->AddSplinePoint(origin + FVector(DEFAULT_TERRITORY_FENCE_DISTANCE, 0, 0), ESplineCoordinateSpace::World);
			_splineComponent->AddSplinePoint(origin + FVector(DEFAULT_TERRITORY_FENCE_DISTANCE * 2, 0, 0), ESplineCoordinateSpace::World);
		}

		InitPointArray();
	}
}

void AGsTerritoryZoneLineShape::DrawZoneShape(const FTransform& in_transform)
{
	DrawPlillar();
	DrawPlanes(false);
}

bool AGsTerritoryZoneLineShape::IsInvalidePointNum(int32 in_num) const
{
	return (in_num < DEFAULT_TERRITORY_LINE_POINT_NUM);
}

void AGsTerritoryZoneLineShape::GetDefaultPoints(TArray<FVector>& out_pointArray) const
{
	out_pointArray.Empty();

	FVector location;

	location = FVector::ZeroVector;
	out_pointArray.Add(location);

	location = FVector(DEFAULT_TERRITORY_FENCE_DISTANCE * 2, 0, 0);
	out_pointArray.Add(location);
}