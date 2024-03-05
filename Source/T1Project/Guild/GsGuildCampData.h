// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"


/**
 * 길드 캠프 정보
 */
struct T1PROJECT_API FGsGuildCampData final
{
public:
	MapId _mapId;
	CampId _campId;
	GuildCampStateType _campState;
	time_t _endTime;
	GuildBuildingLevel _level;

public:
	FGsGuildCampData() = default;	
	~FGsGuildCampData() = default;

public:
	void SetData(const GuildCampInfo& InData);
};
