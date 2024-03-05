// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelect.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsRegionMapIconSelectMonster.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsRegionMapIconSelectMonster : public UGsRegionMapIconSelect
{
	GENERATED_BODY()

private:
	MapId _mapId;
	CreatureId _npcId;
	SpawnGroupId _spawnGroupId;

	// Getter
public:
	MapId GetMapId() { return _mapId; }
	CreatureId GetNpcId() { return _npcId; }
	SpawnGroupId GetSpawnGroupId() { return _spawnGroupId; }

	// Setter
public:
	void SetMapId(MapId InMapId) { _mapId = InMapId; }
	void SetNpcId(CreatureId InNpcId) { _npcId = InNpcId; }
	void SetSpawnGroupId(SpawnGroupId InSpawnGroupId) { _spawnGroupId = InSpawnGroupId; }
};
