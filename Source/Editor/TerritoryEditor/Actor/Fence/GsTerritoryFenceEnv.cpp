// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceEnv.h"
#include "Actor/Shape/GsTerritoryShape.h"
#include "Components/SplineComponent.h"
#include "GsTerritoryFenceBase.h"
#include "Util/GsTerritoryZoneShapeUtil.h"

void AGsTerritoryFenceEnv::InvalidDestription()
{
	SetTerritoryTitleText(TEXT("EnvFence"));

	_description = FString("fence data\n")
		+ FString::Printf(TEXT("unique id : %lld\n"), _uniqueID)
		+ FString::Printf(TEXT("instance id : %lld\n"), _instanceID)
		+ FString::Printf(TEXT("name : %s\n"), *_name.ToString())
		+ FString::Printf(TEXT("center : %s\n"), *_center.ToString())
		+ FString::Printf(TEXT("radius : %.1f\n"), _radius);
}

void AGsTerritoryFenceEnv::InitFenceEnv(const FText& inName, const TArray<FGsEnvCondition>& inEnv)
{
	_name = inName;
	_envConditionArray = inEnv;

	InvalidWidgetText();
}

void AGsTerritoryFenceEnv::Export(OUT FGsSchemaMapTerritoryEnv& outMapEnv, OUT FGsSchemaFenceElement& outFenceElement)
{
	//env
	outMapEnv.territoryId = GetTerritoryUniqueID();
	outMapEnv.fenceId = GetTerritoryUniqueID();
	outMapEnv.name = _name;
	outMapEnv.envConditionList = _envConditionArray;

	//fence
	outFenceElement.FenceId = GetTerritoryUniqueID();
	outFenceElement.Center = _center;
	outFenceElement.Radius = _radius;

	TArray<FVector> pointArray;
	UGsTerritoryZoneShapeUtil::GetPointList<FVector>(_splineComponent, pointArray, ESplineCoordinateSpace::World);
	outFenceElement.vertexList = pointArray;
}

void AGsTerritoryFenceEnv::Import(const FGsSchemaMapTerritoryEnv& inTerritoryInfo, const FGsSchemaFenceElement& inFence)
{	
	_name = inTerritoryInfo.name;
	_envConditionArray = inTerritoryInfo.envConditionList;

	//fence
	SetActorLocation(inFence.Center);
	UGsTerritoryShapeOwnerInterface::SetSplinePoints(_splineComponent, inFence.vertexList);
	_radius = inFence.Radius;
	_points = inFence.vertexList;

	InvalidShape();
}

void AGsTerritoryFenceEnv::Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo, const FGsSchemaFenceElement& inFence)
{
	_name = inTerritoryInfo.name;
	_envConditionArray = inTerritoryInfo.envConditionArray;

	//fence
	SetActorLocation(inFence.Center);
	UGsTerritoryShapeOwnerInterface::SetSplinePoints(_splineComponent, inFence.vertexList);
	_radius = inFence.Radius;
	_points = inFence.vertexList;

	InvalidShape();
}
