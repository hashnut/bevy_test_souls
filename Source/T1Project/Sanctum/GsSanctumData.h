// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"

// 필요시 FGsSchemaSanctumData 를 그냥 헤더에 포함하자

struct FGsSchemaSanctumData;

/**
 * 성소 데이터 
 */
struct T1PROJECT_API FGsSanctumData final
{
public:
	// 점령 길드 정보
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;
	FString _guildName;
	GuildEmblemId _guildEmblemId = INVALID_GUILD_EMBLEM_ID;

private:
	SanctumAreaId _sanctumId = INVALID_SANCTUM_AREA_ID;
	MapId _mapId = INVALID_MAP_ID;
	const FGsSchemaSanctumData* _tableData = nullptr;

	int32 _guildNexusLevel = 0;

public:
	FGsSanctumData() = default;
	~FGsSanctumData();

public:
	static PassivityId GetNexusPassivityIdStatic(SanctumAreaId InSanctumId, int32 InCampLevel);

public:
	void SetTableData(const FGsSchemaSanctumData* InData);	

public:
	void SetGuildData(PD::GC::PKT_GC_ACK_MAP_SANCTUM_INFO_READ::OccupancyList& InData);
	void Clear();
	void PrepareBattle();

public:
	SanctumAreaId GetSanctumId() const { return _sanctumId;	}
	MapId GetMapId() const { return _mapId; }
	FText GetMapName() const;

	// 성소이름
	FText GetSanctumName() const;
	// 요구 레벨
	int32 GetRequiredNexusLevel() const;
	// 넥서스 레벨
	int32 GetGuildNexusLevel() const { return _guildNexusLevel;	}

	// 성소 패시비티
	PassivityId GetSanctumPassivityId() const;
	// 기사단 수호탑 건설 버프 패시비티
	PassivityId GetNexusPassivityId(int32 InCampLevel) const;

public:
	bool IsOccupied() const;
};
