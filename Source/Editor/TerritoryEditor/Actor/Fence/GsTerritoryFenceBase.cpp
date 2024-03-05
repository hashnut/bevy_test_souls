// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceBase.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "Util/GsTerritoryZoneShapeUtil.h"
#include "Components/SplineComponent.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZonePolygonShape.h"
#include "Actor/Spawn/GsTerritoryActor.h"
#include "ActorComponentEx/WidgetComponentEx.h"
#include "Component/GsTerritorySplineComponent.h"
#include "GsTerritoryEditorSetting.h"
#include "Editor.h"
#include "UTIL/GsLevelUtil.h"
#include "NoExportTypes.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Editor/TerritoryEditor/Component/GsTerrEdWidgetComponent.h"
#include "Actor/Shape/GsTerritoryShape.h"
#include "ComponentVisualizers/Public/SplineComponentVisualizer.h"

AGsTerritoryFenceBase::AGsTerritoryFenceBase()
{
	_splineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (_splineComponent)
	{
		_splineComponent->SetRelativeLocation(FVector::ZeroVector);		
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
		_descWidgetBGColor = settings->_fenceColor;
	}

	bRunConstructionScriptOnDrag = false;
}

void AGsTerritoryFenceBase::OnConstruction(const FTransform& inTransform)
{
	Super::OnConstruction(inTransform);		

	SetSplineLoopType();
}

void AGsTerritoryFenceBase::PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent)
{
	Super::PostEditChangeProperty(inPropertyChangedEvent);

	if (FProperty* property = inPropertyChangedEvent.Property)
	{
		InvalidShape();
	}
}
//TerritoryZone와 TerritoryFence 모양 표시 통일화 
void AGsTerritoryFenceBase::InvalidShape()
{
	CreateView(GetActorTransform());
}

void AGsTerritoryFenceBase::SetSplineLoopType()
{
	if (nullptr == _splineComponent)
	{
		return;
	}

	_splineComponent->SetClosedLoop(true);
}

void AGsTerritoryFenceBase::CreateView(const FTransform& in_transform)
{
	if (nullptr == _shapeActorChild)
	{
		return;
	}

	_shapeActorChild->SetRelativeLocation(FVector::ZeroVector);
	AGsTerritoryShape* shape = Cast<AGsTerritoryShape>(_shapeActorChild->GetChildActor());
	if (nullptr == shape)
	{
		return;
	}

	if (CheckDefaultShape(_shapeActorChild, _points))
	{
		SetDefaultShape(_splineComponent, _shapeActorChild, in_transform);
	}

	InitShape(_splineComponent, _shapeActorChild, in_transform
	, _radius, _center, _points);

	FTransform trs;
	trs.SetLocation(in_transform.GetLocation());
	shape->SetShapeOwner(this);
	shape->SetColsedLoop(true);
	shape->SetDrawFloor(true);
	shape->SetColors(_pillarColor, _verticalStripeColor, _verticalStripeColor, _floorColor);
	shape->SetFenceHeight(_height);
	shape->DrawShape(trs, _points, _radius);
}

void AGsTerritoryFenceBase::InitShape(class USplineComponent* inSplineComp, class UChildActorComponent* inShapeActorChild, const FTransform& in_transform
	, OUT float& outRadius, OUT FVector& outCenter, OUT TArray<FVector>& outPointArray)
{
	AActor* childActor = inShapeActorChild->GetChildActor();
	if (nullptr == childActor)
	{
		return;
	}
	
	if (false == childActor->IsA(AGsTerritoryShape::StaticClass()))
	{
		return;
	}

	AGsTerritoryShape* shape =  Cast<AGsTerritoryShape>(childActor);
	if (nullptr == shape)
	{
		return;
	}

	switch (shape->GetShapeType())
	{
	case EGsTerritoryShapeType::Circle:
		InitCircle(inSplineComp, in_transform, outRadius, outCenter, outPointArray);
		break;
	case EGsTerritoryShapeType::Polygon:
		InitPolygon(inSplineComp, in_transform, outRadius, outCenter, outPointArray);
		break;
	case EGsTerritoryShapeType::Sqaure:
		InitSquare(inSplineComp, in_transform, outRadius, outCenter, outPointArray);
		break;	
	}	
}

void AGsTerritoryFenceBase::InitPolygon(class USplineComponent* inSplineComp, const FTransform& in_transform
	, OUT float& outRadius, OUT FVector& outCenter, OUT TArray<FVector>& outPointArray)
{
	if (nullptr == inSplineComp)
	{
		return;
	}

	const FVector& origin = in_transform.GetLocation();
	float distance = 0;
	int32 num = inSplineComp->GetNumberOfSplinePoints();
	FVector location;
	int32 out_selectIndex;

	if (UGsTerritoryZoneShapeUtil::TryGetSelectedIndexInSpline(out_selectIndex))
	{
		if (inSplineComp->GetNumberOfSplinePoints() >= out_selectIndex)
		{
			location = inSplineComp->GetWorldLocationAtSplinePoint(out_selectIndex);
			outRadius = FVector::Distance(origin, location);
		}
	}

	if (0 == outRadius)
	{
		outRadius = DEFAULT_TERRITORY_FENCE_DISTANCE;
	}

	num = (num < DEFAULT_TERRITORY_POLYGON_POINT_NUM ? DEFAULT_TERRITORY_POLYGON_POINT_NUM : num);	
	if (num < DEFAULT_TERRITORY_POLYGON_POINT_NUM)
	{
		inSplineComp->ClearSplinePoints();
		AGsTerritoryShape::GetDefaultPoints(EGsTerritoryShapeType::Polygon, in_transform.GetLocation(), outPointArray, _radius);
		for (int i = 0; i < outPointArray.Num(); ++i)
		{
			inSplineComp->AddSplinePoint(outPointArray[i], ESplineCoordinateSpace::World);
		}		
	}

	TArray<FVector> pointList;
	UGsTerritoryZoneShapeUtil::GetPointList(inSplineComp, pointList, ESplineCoordinateSpace::World);	
	outCenter = UGsTerritoryZoneShapeUtil::FindCentroid(pointList, outRadius);	

	InitPointArray(inSplineComp, outPointArray);
}

void AGsTerritoryFenceBase::InitCircle(class USplineComponent* inSplineComp, const FTransform& in_transform, OUT float& outRadius, OUT FVector& outCenter, OUT TArray<FVector>& outPointArray)
{
	if (nullptr == inSplineComp)
	{
		return;
	}

	const FVector& origin = in_transform.GetLocation();
	float distance = 0;
	int32 num = inSplineComp->GetNumberOfSplinePoints();
	FVector location;
	int32 out_selectIndex;

	if (UGsTerritoryZoneShapeUtil::TryGetSelectedIndexInSpline(out_selectIndex))
	{
		if (inSplineComp->GetNumberOfSplinePoints() >= out_selectIndex)
		{
			location = inSplineComp->GetWorldLocationAtSplinePoint(out_selectIndex);
			outRadius = FVector::Distance(origin, location);
		}
	}

	if (0 == outRadius)
	{
		outRadius = DEFAULT_TERRITORY_FENCE_DISTANCE;
	}

	num = (num < DEFAULT_TERRITORY_CIRCLE_POINT_NUM ? DEFAULT_TERRITORY_CIRCLE_POINT_NUM : num);

	inSplineComp->ClearSplinePoints();
	AGsTerritoryShape::GetDefaultPoints(EGsTerritoryShapeType::Circle, in_transform.GetLocation(), outPointArray, outRadius);
	for (int i = 0; i < outPointArray.Num(); ++i)
	{
		inSplineComp->AddSplinePoint(outPointArray[i], ESplineCoordinateSpace::World);
	}

	TArray<FVector> pointList;
	UGsTerritoryZoneShapeUtil::GetPointList(inSplineComp, pointList, ESplineCoordinateSpace::World);
	outCenter =  UGsTerritoryZoneShapeUtil::FindCentroid(pointList, outRadius);	

	InitPointArray(inSplineComp, outPointArray);
}

void AGsTerritoryFenceBase::InitSquare(class USplineComponent* inSplineComp, const FTransform& in_transform
	, OUT float& outRadius, OUT FVector& outCenter, OUT TArray<FVector>& outPointArray)
{
	if (nullptr == inSplineComp)
	{
		return;
	}

	const FVector& origin = in_transform.GetLocation();
	float distance = 0;
	int32 num = inSplineComp->GetNumberOfSplinePoints();
	FVector location;
	int32 out_selectIndex;

#if WITH_EDITOR
	if (UGsTerritoryZoneShapeUtil::TryGetSelectedIndexInSpline(out_selectIndex))
	{
		if (inSplineComp->GetNumberOfSplinePoints() >= out_selectIndex)
		{
			location = inSplineComp->GetWorldLocationAtSplinePoint(out_selectIndex);
			outRadius = FVector::Distance(origin, location);
		}
	}
#endif

	if (0 == outRadius)
	{
		outRadius = DEFAULT_TERRITORY_FENCE_DISTANCE;
	}

	num = (num < DEFAULT_TERRITORY_SQUARE_POINT_NUM ? DEFAULT_TERRITORY_SQUARE_POINT_NUM : num);

	inSplineComp->ClearSplinePoints();

	//v1
	float degree = 45;
	location = GetActorForwardVector().RotateAngleAxis(45, FVector::UpVector) * outRadius;
	inSplineComp->AddSplinePoint(origin + location, ESplineCoordinateSpace::World);

	//v2 ~ v4
	for (int32 i = 1; i <= 3; ++i)
	{
		degree += 90;
		location = GetActorForwardVector().RotateAngleAxis(degree, FVector::UpVector) * outRadius;
		inSplineComp->AddSplinePoint(origin + location, ESplineCoordinateSpace::World);
	}

	TArray<FVector> pointList;
	UGsTerritoryZoneShapeUtil::GetPointList(inSplineComp, pointList, ESplineCoordinateSpace::World);	
	outCenter = UGsTerritoryZoneShapeUtil::FindCentroid(pointList, outRadius);	

	InitPointArray(inSplineComp, outPointArray);
}

bool AGsTerritoryFenceBase::CheckDefaultShape(class UChildActorComponent* inShapeActorChild, OUT TArray<FVector>& outPointArray)
{		
	AActor* childActor = inShapeActorChild->GetChildActor();
	if (nullptr == childActor)
	{
		return false;
	}

	if (false == childActor->IsA(AGsTerritoryShape::StaticClass()))
	{
		return false;
	}

	AGsTerritoryShape* shape = Cast<AGsTerritoryShape>(childActor);
	if (nullptr == shape)
	{
		return false;
	}

	return shape->IsInvalidePointNum(outPointArray.Num());
}

void AGsTerritoryFenceBase::SetDefaultShape(class USplineComponent* inSplineComp, class UChildActorComponent* inShapeActorChild, const FTransform& in_transform)
{
	if (nullptr == inShapeActorChild)
	{
		return;
	}

	AGsTerritoryShape* shape = Cast<AGsTerritoryShape>(inShapeActorChild->GetChildActor());
	if (nullptr == shape)
	{
		return;
	}

	TArray<FVector> defaultArray;
	const FVector& origin = in_transform.GetLocation();
	EGsTerritoryShapeType shapeType = shape->GetShapeType();

	//환경 펜스는 Circle
	AGsTerritoryShape::GetDefaultPoints(shapeType, origin, defaultArray);

	inSplineComp->ClearSplinePoints();
	for (const FVector& point : defaultArray)
	{
		inSplineComp->AddSplinePoint(point, ESplineCoordinateSpace::World);
	}
}

void AGsTerritoryFenceBase::InitPointArray(class USplineComponent* inSplineComp, OUT TArray<FVector>& outPointArray)
{
	outPointArray.Empty();
	if (nullptr == inSplineComp)
	{
		return;
	}
	
	int32 num = inSplineComp->GetNumberOfSplinePoints();
	for (int32 i = 0; i < num; ++i)
	{
		outPointArray.Add(inSplineComp->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	}
}

void AGsTerritoryFenceBase::SelectInternal(UObject* in_select)
{
	Super::SelectInternal(in_select);
}

void AGsTerritoryFenceBase::KeepTerritorFenceZoneLocation(const FTransform& inTransform)
{
	const FVector& location = _center + FVector(0, 0,_height + (2 *_widgetHeight));	
}

void AGsTerritoryFenceBase::InvalidWidgetText()
{
	if (_splineComponent)
	{
		TArray<FVector> pointList;
		UGsTerritoryZoneShapeUtil::GetPointList<FVector>(_splineComponent, pointList);
		
		if (false == IsValidFenceNum())
		{
			_description = INVALID_FENCE_NUM + FString::Printf(TEXT(" - fence instance id : %lld"), _instanceID);

			FGsTerritoryEditorUtil::ShowWarningNotification(_description);
		}
		else
		{
			_description = FString("fence data\n")
				+ FString::Printf(TEXT("unique id : %lld\n"), _uniqueID)
				+ FString::Printf(TEXT("instance id : %lld\n"), _instanceID)
				+ FString::Printf(TEXT("point count : %d\n"), _splineComponent->GetNumberOfSplinePoints());
		}
	}

	Super::InvalidWidgetText();
}

void AGsTerritoryFenceBase::InvalidWidgetPosition(const FVector& in_location)
{
	const FVector& center = _center + FVector(0, 0, _height + _widgetHeight);
	if (_widgetComp)
	{
		_widgetComp->SetWorldLocation(center);
	}
}

void AGsTerritoryFenceBase::DestroyManually()
{
	Super::DestroyManually();
}

void AGsTerritoryFenceBase::Copy()
{	

}

void AGsTerritoryFenceBase::Import(const FGsSchemaFenceElement& inFence)
{
	if (nullptr == _splineComponent)
	{
		return;
	}

	SetPointArray(inFence.vertexList);
}

void AGsTerritoryFenceBase::SetPointArray(const TArray<FVector>& inPointArray)
{	
	_splineComponent->ClearSplinePoints();
	int32 num = inPointArray.Num();
	for (int32 i = 0; i < num; ++i)
	{
		_splineComponent->AddSplineWorldPoint(inPointArray[i]);
	}

	InvalidShape();
}

bool AGsTerritoryFenceBase::TryGetPointArray(OUT TArray<FVector>& outPointArray)
{
	TArray<FVector> pointArray;
	UGsTerritoryZoneShapeUtil::GetPointList(_splineComponent, pointArray, ESplineCoordinateSpace::World);
	outPointArray = pointArray;

	return true;
}

bool AGsTerritoryFenceBase::TryGet2DPointArray(OUT TArray<FVector2D>& outPointArray)
{
	outPointArray.Num();
	TArray<FVector> pointArray;
	TryGetPointArray(pointArray);
	for (int32 i = 0; i < pointArray.Num(); ++i)
	{
		outPointArray.Add(FVector2D(pointArray[i]));
	}

	return outPointArray.Num() > 0;
}

bool AGsTerritoryFenceBase::IsValidFenceNum()
{
	return true;
}

void AGsTerritoryFenceBase::SetName(FText inName)
{
	_name = inName;

	InvalidWidgetText();
}
