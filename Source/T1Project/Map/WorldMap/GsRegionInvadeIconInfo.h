#pragma once
#include "CoreMinimal.h"
#include "Map/WorldMap/GsWorldMapDefine.h"

// region map: invade icon info
class FGsRegionInvadeIconInfo
{
public:
	int64 _gameId;
	FVector _pos;
	EGsRegionInvadeNPCType _NPCType;

public:
	virtual ~FGsRegionInvadeIconInfo() = default;
};