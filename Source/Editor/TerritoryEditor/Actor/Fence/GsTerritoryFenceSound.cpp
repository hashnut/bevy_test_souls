// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceSound.h"
#include "Util/GsTerritoryZoneShapeUtil.h"

void AGsTerritoryFenceSound::InvalidDestription()
{
	SetTerritoryTitleText(TEXT("SoundFence"));

	_description = FString("fence data\n")
		+ FString::Printf(TEXT("unique id : %lld\n"), _uniqueID)
		+ FString::Printf(TEXT("instance id : %lld\n"), _instanceID)
		+ FString::Printf(TEXT("name : %s\n"), *_name.ToString())
		+ FString::Printf(TEXT("center : %s\n"), *_center.ToString())
		+ FString::Printf(TEXT("radius : %.1f\n"), _radius);
}

void AGsTerritoryFenceSound::InitFenceSound(const FText& inName, const TArray<FGsQuestSound>& inQuestSoundArray)
{
	_name = inName;
	_questSoundArray = inQuestSoundArray;

	InvalidWidgetText();
}

void AGsTerritoryFenceSound::SetQuestSound(const TArray<FGsQuestSound>& inQuestSoundArray)
{
	_questSoundArray = inQuestSoundArray;

	InvalidWidgetText();
}

void AGsTerritoryFenceSound::Export(OUT FGsSchemaMapTerritorySound& outMapTerritorySound, OUT FGsSchemaFenceElement& outFenceElement)
{
	outMapTerritorySound.territoryId = GetTerritoryUniqueID();
	outMapTerritorySound.fenceId = GetTerritoryUniqueID();
	outMapTerritorySound.name = _name;
	outMapTerritorySound.questSoundList = _questSoundArray;

	//fence
	outFenceElement.FenceId = GetTerritoryUniqueID();
	outFenceElement.Center = _center;
	outFenceElement.Radius = _radius;

	TArray<FVector> pointArray;
	UGsTerritoryZoneShapeUtil::GetPointList<FVector>(_splineComponent, pointArray, ESplineCoordinateSpace::World);
	outFenceElement.vertexList = pointArray;
}

void AGsTerritoryFenceSound::Import(const FGsSchemaMapTerritorySound& inTerritoryInfo, const FGsSchemaFenceElement& inFence)
{	
	_name = inTerritoryInfo.name;
	_questSoundArray = inTerritoryInfo.questSoundList;

	//fence
	SetActorLocation(inFence.Center);
	UGsTerritoryShapeOwnerInterface::SetSplinePoints(_splineComponent, inFence.vertexList);
	_radius = inFence.Radius;
	_points = inFence.vertexList;

	InvalidShape();
}

void AGsTerritoryFenceSound::Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo, const FGsSchemaFenceElement& inFence)
{
	_name = inTerritoryInfo.name;
	_questSoundArray = inTerritoryInfo.questSoundArray;

	//fence
	SetActorLocation(inFence.Center);
	UGsTerritoryShapeOwnerInterface::SetSplinePoints(_splineComponent, inFence.vertexList);
	_radius = inFence.Radius;
	_points = inFence.vertexList;

	InvalidShape();
}
