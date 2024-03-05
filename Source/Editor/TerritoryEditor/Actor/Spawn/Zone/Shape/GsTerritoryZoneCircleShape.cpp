// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZoneCircleShape.h"

//UE4
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#include "Editor/ComponentVisualizers/Public/SplineComponentVisualizer.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"

//territory editor
#include "Util/GsTerritoryZoneShapeUtil.h"

void AGsTerritoryZoneCircleShape::InitZoneShape()
{
	if (nullptr == _splineComponent)
	{
		return;
	}

	_splineComponent->SetClosedLoop(true);

	const FVector& origin = GetActorLocation();
	float distance = 0;
	int num = _splineComponent->GetNumberOfSplinePoints();
	FVector location;
	int32 out_selectIndex;

	if (UGsTerritoryZoneShapeUtil::TryGetSelectedIndexInSpline(out_selectIndex))
	{
		if (_splineComponent->GetNumberOfSplinePoints() >= out_selectIndex)
		{
			location = _splineComponent->GetWorldLocationAtSplinePoint(out_selectIndex);
			_radius = FVector::Distance(origin, location);
		}
	}

	if (_radius == 0)
	{
		_radius = DEFAULT_TERRITORY_FENCE_DISTANCE;
	}	

	num = (num < DEFAULT_TERRITORY_CIRCLE_POINT_NUM ? DEFAULT_TERRITORY_CIRCLE_POINT_NUM : num);

	_splineComponent->ClearSplinePoints();

	if (num > 0)
	{
		float gap = 360.0f / num;
		float degree = 0;
		FVector direction;
		for (int i = 0; i < num; ++i)
		{
			degree = gap * i;
			direction = FVector::ForwardVector.RotateAngleAxis(degree, FVector::UpVector) * _radius;
			_splineComponent->AddSplinePoint(origin + direction, ESplineCoordinateSpace::World);
		}
	}

	InitPointArray();
}

void AGsTerritoryZoneCircleShape::DrawZoneShape(const FTransform& in_transform)
{
	DrawPlillar();
	DrawPlanes();
	DrawFloor(in_transform);
}

void AGsTerritoryZoneCircleShape::DrawZoneShape(const FTransform& in_transform, const TArray<FVector>& pointArray, float in_radius)
{
	Super::DrawZoneShape(in_transform, pointArray, in_radius);

	_radius = in_radius;	
}

float AGsTerritoryZoneCircleShape::GetRadius() const
{
	return _radius;
}

bool AGsTerritoryZoneCircleShape::IsInvalidePointNum(int32 in_num) const
{
	return (in_num < DEFAULT_TERRITORY_CIRCLE_POINT_NUM);
}

void AGsTerritoryZoneCircleShape::GetDefaultPoints(TArray<FVector>& out_pointArray) const
{
	out_pointArray.Empty();

	int32 num = DEFAULT_TERRITORY_CIRCLE_POINT_NUM;
	float gap = 360.0f / num;
	float degree = 0;
	FVector location;

	for (int32 i = 0; i < num; ++i)
	{
		degree = gap * i;
		location = FVector::ForwardVector.RotateAngleAxis(degree, FVector::UpVector) * DEFAULT_TERRITORY_FENCE_DISTANCE;

		out_pointArray.Add(location);
	}	
}