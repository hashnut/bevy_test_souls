// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceTownChaos.h"
#include "Actor/Shape/GsTerritoryShape.h"
#include "Components/SplineComponent.h"
#include "GsTerritoryFenceBase.h"
#include "Util/GsTerritoryZoneShapeUtil.h"

void AGsTerritoryFenceTownChaos::InitTownFence(FText inName, const FGsSchemaSoundResDataRow& inSound)
{
	_name = inName;
	_sound = inSound;

	InvalidWidgetText();
}

void AGsTerritoryFenceTownChaos::InvalidDestription()
{
	SetTerritoryTitleText(TEXT("TownChaos"));

	_description = FString("fence data\n")
		+ FString::Printf(TEXT("unique id : %lld\n"), _uniqueID)
		+ FString::Printf(TEXT("instance id : %lld\n"), _instanceID)
		+ FString::Printf(TEXT("name : %s\n"), *_name.ToString())
		+ FString::Printf(TEXT("center : %s\n"), *_center.ToString())
		+ FString::Printf(TEXT("radius : %.1f\n"), _radius);

	if (_sound.GetRow())
	{
		_description += FString::Printf(TEXT("sound id : %lld\n"), _sound.GetRow()->id);
	}
}

void AGsTerritoryFenceTownChaos::SetSound(const FGsSchemaSoundResDataRow& inSound)
{
	_sound = inSound;

	InvalidWidgetText();
}

void AGsTerritoryFenceTownChaos::Export(OUT FGsSchemaMapTerritoryInfo& outTerritoryInfo, OUT FGsSchemaFenceElement& outFenceElement)
{
	//town
	outTerritoryInfo.territoryId = GetTerritoryUniqueID();
	outTerritoryInfo.territoryType = GetFenceType();
	outTerritoryInfo.fenceId = GetTerritoryUniqueID();
	outTerritoryInfo.byClient = false;
	outTerritoryInfo.name = _name;
	outTerritoryInfo.sound = _sound;

	//fence
	outFenceElement.FenceId = GetTerritoryUniqueID();
	outFenceElement.Center = _center;
	outFenceElement.Radius = _radius;

	TArray<FVector> pointList;
	UGsTerritoryZoneShapeUtil::GetPointList<FVector>(_splineComponent, pointList, ESplineCoordinateSpace::World);
	outFenceElement.vertexList = pointList;
}

void AGsTerritoryFenceTownChaos::Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo
	, const FGsSchemaFenceElement& inFence)
{
	_name = inTerritoryInfo.name;
	_sound = inTerritoryInfo.sound;

	//fence
	SetActorLocation(inFence.Center);
	UGsTerritoryShapeOwnerInterface::SetSplinePoints(_splineComponent, inFence.vertexList);
	_radius = inFence.Radius;
	_points = inFence.vertexList;

	InvalidShape();
}
