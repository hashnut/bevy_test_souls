/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Map/Spawn/GsSchemaSpawnElement.h"
FGsSchemaSpawnElement::FGsSchemaSpawnElement()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSpawnElement)
	
	SpawnId = 0;
	memberType = NpcSpawnGroupMemberType::NONE;
	SpawnType = CreatureSpawnType::NONE;
	CreatureId = 0;
	SpawnCount = 0;
	RandomPos = false;
	spawnLevel = 0;
	Pos = FVector(0.000000, 0.000000, 0.000000);
	Dir = 0;
	RespawnInterval = 0;
	spawnRate = 0;
	groupLimitRange = 0.000000f;
	ClientGameId = 0;
	InitVisible = false;
	VisibleType = EGsVisibleType::Show;
	offensiveType = OffensiveType::USE_NPC_DATA;
	sightRadius = 0.000000f;
	aggroLink = false;
	AiOptimizeOnOff = true;
	isFixedLocation = false;
	skipTerrainCheck = false;
}
