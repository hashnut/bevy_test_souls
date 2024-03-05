// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceNonPK.h"
#include "Util/GsTerritoryZoneShapeUtil.h"

void AGsTerritoryFenceNonPK::InvalidDestription()
{
	SetTerritoryTitleText(TEXT("NonPKFence"));
	
	_description = FString("fence data\n")
		+ FString::Printf(TEXT("unique id : %lld\n"), _uniqueID)
		+ FString::Printf(TEXT("instance id : %lld\n"), _instanceID)
		+ FString::Printf(TEXT("name : %s\n"), *_name.ToString())
		+ FString::Printf(TEXT("center : %s\n"), *_center.ToString())
		+ FString::Printf(TEXT("radius : %.1f\n"), _radius);
}

void AGsTerritoryFenceNonPK::Export(OUT FGsSchemaMapTerritoryInfo& outTerritoryInfo, OUT FGsSchemaFenceElement& outFenceElement)
{
	//town
	outTerritoryInfo.territoryId = GetTerritoryUniqueID();
	outTerritoryInfo.territoryType = GetFenceType();
	outTerritoryInfo.fenceId = GetTerritoryUniqueID();	
	outTerritoryInfo.byClient = false;
	outTerritoryInfo.name = _name;

	//fence
	outFenceElement.FenceId = GetTerritoryUniqueID();
	outFenceElement.Center = _center;
	outFenceElement.Radius = _radius;

	TArray<FVector> pointList;
	UGsTerritoryZoneShapeUtil::GetPointList<FVector>(_splineComponent, pointList, ESplineCoordinateSpace::World);
	outFenceElement.vertexList = pointList;
}

void AGsTerritoryFenceNonPK::Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo
	, const FGsSchemaFenceElement& inFence)
{	
	_name = inTerritoryInfo.name;	

	//fence
	SetActorLocation(inFence.Center);
	UGsTerritoryShapeOwnerInterface::SetSplinePoints(_splineComponent, inFence.vertexList);
	_radius = inFence.Radius;	
	_points = inFence.vertexList;

	InvalidShape();
}
