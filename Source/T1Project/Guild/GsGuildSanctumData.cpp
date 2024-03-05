// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildSanctumData.h"
#include "DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "UTIL/GsTableUtil.h"


void FGsGuildSanctumData::Clear()
{
	_sanctumId = INVALID_SANCTUM_AREA_ID;
	_mapId = INVALID_MAP_ID;
	_nexusId = INVALID_SANCTUM_NEXUS_ID;
	_nexusState = GuildCampStateType::NONE;
	_buildingLevel = INVALID_GUILD_BUILDING_LEVEL;
}

void FGsGuildSanctumData::SetData(const SanctumNexusInfo& InData)
{
	_sanctumId = InData.mSanctumAreaId;
	_mapId = InData.mMapId;
	_nexusId = InData.mSanctumNexusId;
	_nexusState = InData.mGuildCampStateType;
	_buildingLevel = InData.mLevel;
}
