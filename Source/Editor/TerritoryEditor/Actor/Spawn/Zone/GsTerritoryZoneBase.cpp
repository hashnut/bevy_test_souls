// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZoneBase.h"
#include "Shape/GsTerritoryZoneShape.h"
#include "Components/SplineComponent.h"
#include "GsTerritoryEditorSetting.h"
#include "ComponentVisualizers/Public/SplineComponentVisualizer.h"
#include "Shape/GsTerritoryZoneCircleShape.h"
#include "Util/GsTerritoryZoneShapeUtil.h"

AGsTerritoryZoneBase::AGsTerritoryZoneBase()
{
	_splineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (_splineComponent)
	{
		_splineComponent->SetRelativeLocation(FVector::ZeroVector);
		_splineComponent->EditorUnselectedSplineSegmentColor = FColor::Blue;
		_splineComponent->EditorSelectedSplineSegmentColor = FColor::Yellow;

		SetRootComponent(_splineComponent);
	}

	_shapeActorChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("ActorChild"));
	if (_shapeActorChild)
	{
		_shapeActorChild->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_shapeActorChild->SetRelativeLocation(FVector::ZeroVector);
	}

	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_zoneColor;
	}

	bRunConstructionScriptOnDrag = false;
}

void AGsTerritoryZoneBase::PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent)
{
	Super::PostEditChangeProperty(inPropertyChangedEvent);

	if (FProperty* property = inPropertyChangedEvent.Property)
	{
		InvalidShape();
	}
}


void AGsTerritoryZoneBase::SelectInternal(UObject* in_select)
{
	Super::SelectInternal(in_select);

	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::SelectInternal - label : %s"), *GetActorLabel());

	if (this == in_select)
	{
		if (AGsTerritorySpawnActor* terrainActor = Cast<AGsTerritorySpawnActor>(in_select))
		{
			ActiveSelectOutline(terrainActor->_zone == this);
		}
	}
}

void AGsTerritoryZoneBase::TransparentFloor()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::TransparentFloor - label : %s"), *GetActorLabel());

	if (_shapeActorChild)
	{
		AGsTerritoryZoneShape* zoneShape = Cast<AGsTerritoryZoneShape>(_shapeActorChild->GetChildActor());
		if (zoneShape)
		{
			zoneShape->TransparentFloor();
		}
	}
}

void AGsTerritoryZoneBase::CopyZoneInfo(class AGsTerritoryZoneBase* inZone)
{

}

EGsTerritoryZoneShapeType AGsTerritoryZoneBase::GetShapeType() const
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::GetShapeType - label : %s"), *GetActorLabel());

	if (_shapeActorChild)
	{
		if (AActor* shape = _shapeActorChild->GetChildActor())
		{
			if (AGsTerritoryZoneShape* zoneShape = Cast<AGsTerritoryZoneShape>(shape))
			{
				return zoneShape->GetZoneShapeType();
			}
		}
	}

	return EGsTerritoryZoneShapeType::Max;
}

TArray<FVector> AGsTerritoryZoneBase::GetPoints()
{
	TArray<FVector> pointArray;
	UGsTerritoryZoneShapeUtil::GetPointList(_splineComponent, pointArray, ESplineCoordinateSpace::World);

	return pointArray;
}

void AGsTerritoryZoneBase::CreateView(const FTransform& in_transform)
{
	SetSplineLoopType();

	if (nullptr == _shapeActorChild)
	{
		return;
	}

	_shapeActorChild->SetRelativeLocation(FVector::ZeroVector);
	if (AActor* shape = _shapeActorChild->GetChildActor())
	{
		if (AGsTerritoryZoneShape* zoneShape = Cast<AGsTerritoryZoneShape>(shape))
		{
			if (CheckDefaultShape())
			{
				SetDefaultShape(in_transform);
			}

			InitShape(in_transform);

			FTransform trs;
			trs.SetLocation(in_transform.GetLocation());
			zoneShape->SetShapeOwner(this);
			zoneShape->SetColors(_pillarColor, _fenceOuterColor, _fenceInsideColor, _floorColor);
			zoneShape->SetFenceHeight(_height);
			zoneShape->DrawZoneShape(trs, _points, _radius);
		}
	}
}

void AGsTerritoryZoneBase::InvalidShape()
{
	CreateView(GetActorTransform());
}

void AGsTerritoryZoneBase::SetSplineLoopType()
{
	_splineComponent->SetClosedLoop(true);
}

bool AGsTerritoryZoneBase::CheckDefaultShape()
{
	if (_shapeActorChild)
	{
		if (AActor* child = _shapeActorChild->GetChildActor())
		{
			if (AGsTerritoryZoneShape* shape = Cast<AGsTerritoryZoneShape>(child))
			{
				return shape->IsInvalidePointNum(_points.Num());
			}
		}
	}

	return false;
}

void AGsTerritoryZoneBase::SetDefaultShape(const FTransform& in_transform)
{
	if (nullptr == _shapeActorChild)
	{
		return;
	}

	AGsTerritoryZoneShape* shape = Cast<AGsTerritoryZoneShape>(_shapeActorChild->GetChildActor());	
	if (nullptr == shape)
	{
		return;
	}
					
	TArray<FVector> defaultArray;
	const FVector& origin = in_transform.GetLocation();
	shape->GetDefaultPoints(defaultArray);
	_splineComponent->ClearSplinePoints();
	for (const FVector& point : defaultArray)
	{
		_splineComponent->AddSplinePoint(origin + point, ESplineCoordinateSpace::World);
	}
}

void AGsTerritoryZoneBase::InitShape(const FTransform& in_transform)
{
	if (_shapeActorChild)
	{
		if (AActor* shape = _shapeActorChild->GetChildActor())
		{
			if (shape->IsA(AGsTerritoryZoneCircleShape::StaticClass()))
			{
				InitCircle(in_transform);
			}
			else
			{
				if (_splineComponent)
				{
					InitPointArray();
				}
			}
		}
	}
}


void AGsTerritoryZoneBase::InitCircle(const FTransform& in_transform)
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

	if (UGsTerritoryZoneShapeUtil::TryGetSelectedIndexInSpline(out_selectIndex))
	{
		if (_splineComponent->GetNumberOfSplinePoints() >= out_selectIndex)
		{
			location = _splineComponent->GetWorldLocationAtSplinePoint(out_selectIndex);
			_radius = FVector::Distance(origin, location);
		}
	}

	if (0 == _radius)
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

void AGsTerritoryZoneBase::ActiveSelectOutline(bool in_activeOutline)
{

}


void AGsTerritoryZoneBase::InitPointArray()
{
	_points.Empty();

	if (_splineComponent)
	{
		int32 num = _splineComponent->GetNumberOfSplinePoints();

		for (int32 i = 0; i < num; ++i)
		{
			_points.Add(_splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		}
	}
}