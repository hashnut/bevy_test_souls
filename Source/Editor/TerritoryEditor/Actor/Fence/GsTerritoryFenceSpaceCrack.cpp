// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceSpaceCrack.h"
#include "Actor/Shape/GsTerritoryShape.h"
#include "Map/GsTerritoryShapeType.h"
#include "Components/SplineComponent.h"
#include "GsTerritoryEditorSetting.h"
#include "Util/GsTerritoryZoneShapeUtil.h"
#include "UTIL/GsText.h"

AGsTerritoryFenceSpaceCrack::AGsTerritoryFenceSpaceCrack()
{
	_portalSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("PortalSpline"));
	if (_portalSplineComponent)
	{
		_portalSplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_portalSplineComponent->SetRelativeLocation(FVector::ZeroVector);
	}

	_portalShapeActorChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("PortalActorChild"));
	if (_portalShapeActorChild)
	{
		_portalShapeActorChild->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_portalShapeActorChild->SetRelativeLocation(FVector::ZeroVector);
	}

	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_fenceColor;
	}

	bRunConstructionScriptOnDrag = false;
}

void AGsTerritoryFenceSpaceCrack::InitSpaceCrack(const FText& inName, EGsEnvEvent inEnvEvent)
{
	_name = inName;
	_envEvent = inEnvEvent;

	InvalidWidgetText();
}

void AGsTerritoryFenceSpaceCrack::InvalidDestription()
{
	SetTerritoryTitleText(TEXT("SpaceCrackFence"));

	FString envString = FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), _envEvent);

	_description = FString("fence data\n")
		+ FString::Printf(TEXT("unique id : %lld\n"), _uniqueID)
		+ FString::Printf(TEXT("instance id : %lld\n"), _instanceID)
		+ FString::Printf(TEXT("name : %s\n"), *_name.ToString())
		+ FString::Printf(TEXT("envEvent : %s\n"), *envString)
		+ FString::Printf(TEXT("center : %s\n"), *_center.ToString())
		+ FString::Printf(TEXT("portal radius : %.1f\n"), _portalRadius)
		+ FString::Printf(TEXT("radius : %.1f\n"), _radius);
}

void AGsTerritoryFenceSpaceCrack::CreateView(const FTransform& in_transform)
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

	_shapeActorChild->SetRelativeLocation(FVector::ZeroVector);
	AGsTerritoryShape* portalShape = Cast<AGsTerritoryShape>(_portalShapeActorChild->GetChildActor());
	if (nullptr == portalShape)
	{
		return;
	}

	if (CheckDefaultShape(_shapeActorChild, _points))
	{
		SetDefaultShape(_splineComponent, _shapeActorChild, in_transform);		
	}

	if (CheckDefaultShape(_portalShapeActorChild, _portalPoints))
	{
		SetDefaultShape(_portalSplineComponent, _portalShapeActorChild, in_transform);
	}

	FVector squareCenter;
	InitShape(_splineComponent, _shapeActorChild, in_transform
		, _radius, _center, _points);
	InitShape(_portalSplineComponent, _portalShapeActorChild, in_transform
		, _portalRadius, squareCenter, _portalPoints);

	FTransform trs;
	trs.SetLocation(in_transform.GetLocation());	

	shape->SetShapeOwner(this);
	shape->SetColsedLoop(true);
	shape->SetDrawFloor(false);
	shape->SetColors(_pillarColor, _verticalStripeColor, _verticalStripeColor, _floorColor);
	shape->SetFenceHeight(_height);
	shape->DrawShape(trs, _points, _radius);

	FTransform portalTrs;
	portalTrs.SetLocation(in_transform.GetLocation());
	portalTrs.SetRotation(in_transform.GetRotation());

	portalShape->SetShapeOwner(this);
	portalShape->SetColsedLoop(true);
	portalShape->SetDrawFloor(true);
	portalShape->SetColors(_pillarColor, _verticalStripeColor, _verticalStripeColor, _floorColor);
	portalShape->SetFenceHeight(_height);
	portalShape->DrawShape(portalTrs, _portalPoints, _portalRadius);
}

void AGsTerritoryFenceSpaceCrack::SetEnvEvent(EGsEnvEvent inEnvEvent)
{
	_envEvent = inEnvEvent;

	InvalidWidgetText();
}

void AGsTerritoryFenceSpaceCrack::Export(OUT FGsSchemaMapTerritorySpaceCrack& outMapSpaceCrack, OUT FGsSchemaFenceElement& outFenceElement)
{
	//space crack	
	outMapSpaceCrack.territoryId = GetTerritoryUniqueID();
	outMapSpaceCrack.fenceId = GetTerritoryUniqueID();
	outMapSpaceCrack.center = GetActorLocation();
	outMapSpaceCrack.name = _name;

	TArray<FVector> pointList;
	UGsTerritoryZoneShapeUtil::GetPointList<FVector>(_portalSplineComponent, pointList, ESplineCoordinateSpace::World);
	outMapSpaceCrack.vertexList = pointList;

	outMapSpaceCrack.degree = GetActorRotation().Yaw;
	outMapSpaceCrack.radius = _portalRadius;
	outMapSpaceCrack.envEvent = _envEvent;

	//fence
	outFenceElement.FenceId = GetTerritoryUniqueID();
	outFenceElement.Center = _center;
	outFenceElement.Radius = _radius;
	
	UGsTerritoryZoneShapeUtil::GetPointList<FVector>(_splineComponent, pointList, ESplineCoordinateSpace::World);
	outFenceElement.vertexList = pointList;
}

void AGsTerritoryFenceSpaceCrack::Import(const FGsSchemaMapTerritorySpaceCrack& inMapSpaceCrack, const FGsSchemaFenceElement& inFenceElement)
{
	//space crack	
	_name = inMapSpaceCrack.name;
	SetActorLocation(inMapSpaceCrack.center);
	_portalRadius = inMapSpaceCrack.radius;
	SetActorRotation(FRotator(0, 0, inMapSpaceCrack.degree));
	UGsTerritoryShapeOwnerInterface::SetSplinePoints(_portalSplineComponent, inMapSpaceCrack.vertexList);
	_envEvent = inMapSpaceCrack.envEvent;

	//fence
	UGsTerritoryShapeOwnerInterface::SetSplinePoints(_splineComponent, inFenceElement.vertexList);
	_radius = inFenceElement.Radius;
	_points = inFenceElement.vertexList;

	InvalidShape();
}