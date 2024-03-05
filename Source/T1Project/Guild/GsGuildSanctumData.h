// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"

//struct FGsSchemaSanctumData;
struct FGsSchemaSanctumNexusData;

/**
 * 길드 성소 정보
 */
struct T1PROJECT_API FGsGuildSanctumData final
{
public:
	SanctumAreaId _sanctumId = INVALID_SANCTUM_AREA_ID;
	MapId _mapId = INVALID_MAP_ID;
	//ChannelId // 필요한가?
	SanctumNexusId _nexusId = INVALID_SANCTUM_NEXUS_ID;
	//StatValue _currentHp; // 필요한가?
	GuildCampStateType _nexusState = GuildCampStateType::NONE;
	//time_t _startTime // 필요한가?
	//time_t _endTime // 필요한가?
	GuildBuildingLevel _buildingLevel = INVALID_GUILD_BUILDING_LEVEL;
	//PlanetWorldId // 필요한가?

public:
	FGsGuildSanctumData() = default;
	~FGsGuildSanctumData() = default;

public:
	void Clear();
	void SetData(const SanctumNexusInfo& InData);	
};
