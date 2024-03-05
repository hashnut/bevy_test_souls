// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZoneSpaceCrack.h"
#include "TerritoryEditor/Actor/Shape/GsTerritoryShape.h"
#include "Components/SplineComponent.h"
#include "Util/GsTerritoryZoneShapeUtil.h"

AGsTerritoryZoneSpaceCrack::AGsTerritoryZoneSpaceCrack()
{
	_splineComponent->SetClosedLoop(true);
}

void AGsTerritoryZoneSpaceCrack::CreateView(const FTransform& in_transform)
{
	if (nullptr == _shapeActorChild)
	{
		return;
	}

	_shapeActorChild->SetRelativeLocation(FVector::ZeroVector);
	if (AActor* actor = _shapeActorChild->GetChildActor())
	{
		if (AGsTerritoryShape* shape = Cast<AGsTerritoryShape>(actor))
		{
			if (CheckDefaultShape())
			{
				SetDefaultShape(in_transform);
			}

			InitShape(in_transform);

			FTransform trs;
			trs.SetLocation(in_transform.GetLocation());
			trs.SetRotation(in_transform.GetRotation());
			shape->SetShapeOwner(this);
			shape->SetColsedLoop(true);
			shape->SetDrawFloor(true);
			shape->SetColors(_pillarColor, _fenceOuterColor, _fenceInsideColor, _floorColor);
			shape->SetFenceHeight(_height);
			shape->DrawShape(trs, _points, _radius);
		}
	}
}

bool AGsTerritoryZoneSpaceCrack::CheckDefaultShape()
{
	AGsTerritoryShape* shape = Cast<AGsTerritoryShape>(_shapeActorChild->GetChildActor());
	if (nullptr == shape)
	{
		return false;
	}

	return _points.Num() < DEFAULT_TERRITORY_SQUARE_POINT_NUM;
}

void AGsTerritoryZoneSpaceCrack::SetDefaultShape(const FTransform& in_transform)
{
	if (nullptr == _shapeActorChild)
	{
		return;
	}

	AGsTerritoryShape* shape = Cast<AGsTerritoryShape>(_shapeActorChild->GetChildActor());
	if (nullptr == shape)
	{
		return;
	}

	TArray<FVector> defaultArray;
	const FVector& origin = in_transform.GetLocation();
	AGsTerritoryShape::GetDefaultPoints(EGsTerritoryShapeType::Sqaure, origin, defaultArray);
	_splineComponent->ClearSplinePoints();
	for (const FVector& point : defaultArray)
	{
		_splineComponent->AddSplinePoint(origin + point, ESplineCoordinateSpace::World);
	}
}

void AGsTerritoryZoneSpaceCrack::InitShape(const FTransform& in_transform)
{
	if (nullptr == _shapeActorChild)
	{
		return;
	}
	
	if (AActor* shape = _shapeActorChild->GetChildActor())
	{
		InitSquare(in_transform);
	}
}

void AGsTerritoryZoneSpaceCrack::InitSquare(const FTransform& in_transform)
{
	if (nullptr == _splineComponent)
	{
		return;
	}

	const FVector& origin = in_transform.GetLocation();
	float distance = 0;
	int32 num = _splineComponent->GetNumberOfSplinePoints();
	FVector location;
	int32 out_selectIndex;

#if WITH_EDITOR
	if (UGsTerritoryZoneShapeUtil::TryGetSelectedIndexInSpline(out_selectIndex))
	{
		if (_splineComponent->GetNumberOfSplinePoints() >= out_selectIndex)
		{
			location = _splineComponent->GetWorldLocationAtSplinePoint(out_selectIndex);
			_radius = FVector::Distance(origin, location);
		}
	}
#endif

	if (0 == _radius)
	{
		_radius = DEFAULT_TERRITORY_FENCE_DISTANCE;
	}

	num = (num < DEFAULT_TERRITORY_CIRCLE_POINT_NUM ? DEFAULT_TERRITORY_CIRCLE_POINT_NUM : num);

	_splineComponent->ClearSplinePoints();

	//v1
	float degree = 45;
	location = GetActorForwardVector().RotateAngleAxis(45, FVector::UpVector) * _radius;
	_splineComponent->AddSplinePoint(origin + location, ESplineCoordinateSpace::World);

	//v2 ~ v4
	for (int32 i = 1; i <= 3; ++i)
	{		
		degree += 90;
		location = GetActorForwardVector().RotateAngleAxis(degree, FVector::UpVector) * _radius;
		_splineComponent->AddSplinePoint(origin + location, ESplineCoordinateSpace::World);
	}	

	InitPointArray();
}

void AGsTerritoryZoneSpaceCrack::InvalidDestription()
{
	SetTerritoryTitleText(TEXT("SpaceCrackZone"));

	if (_isValid)
	{
		_description = FString::Printf(TEXT("Unique id : %lld\n"), _uniqueID)
				+ FString::Printf(TEXT("radius : %f\n"), _radius)
				+ FString::Printf(TEXT("Location : %s\n"), *GetActorLocation().ToString());
	}
	else
	{
		_description = INVALID_TERRITORY_ACTOR;
	}
}

void AGsTerritoryZoneSpaceCrack::Export(OUT FGsSchemaMapTerritorySpaceCrack& outMapTerritorySpaceCrack)
{
	outMapTerritorySpaceCrack.territoryId = GetTerritoryUniqueID();
	outMapTerritorySpaceCrack.fenceId = GetTerritoryUniqueID();
	outMapTerritorySpaceCrack.center = GetActorLocation();
	outMapTerritorySpaceCrack.vertexList = GetPoints();
	outMapTerritorySpaceCrack.degree = GetActorRotation().Yaw;
	outMapTerritorySpaceCrack.radius = _radius;
}

void AGsTerritoryZoneSpaceCrack::Import(const FGsSchemaMapTerritorySpaceCrack& inSpaceCrack)
{
	_radius = inSpaceCrack.radius;	

	CreateView(GetTransform());
}
