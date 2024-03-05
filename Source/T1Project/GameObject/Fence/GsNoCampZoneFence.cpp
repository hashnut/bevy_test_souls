// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNoCampZoneFence.h"
#include "UTIL/GsText.h"
#include "GsFenceHandler.h"

void FGsNoCampZoneFence::Initialize(const struct FGsSchemaMapTerritoryInfo* inMapTerritoryInfo, const struct FGsSchemaFenceElement* inFence)
{
	_territory = inMapTerritoryInfo;
	_fence = inFence;
}