// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

/**
 * FGsGameObjectDataPlayer 에 저장하는 플레이어의 길드 정보 (SC_NOTIFY_USER_GUILD_INFO) 
 */
struct T1PROJECT_API FGsGuildPlayerData final
{
public:
	enum EGsGuildIffType
	{
		NONE,
		ALLY,
		ENEMY,
	};

public:
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;
	GuildEmblemId _emblemId = 0;	
	EGsGuildIffType _guildIff = EGsGuildIffType::NONE;

private:
	FString _guildName;

public:
	FGsGuildPlayerData() = default;
	~FGsGuildPlayerData() = default;

public:
	void Empty();
	// 내 캐릭터 접속 시
	void Set(PD::SC::PKT_SC_USER_BASIC_DATA_READ& InData);
	// 타유저 스폰시
	void Set(PD::SC::PKT_SC_SPAWN_USER_READ& InData);
	// 내 캐릭터 가입시
	void Set(PD::SC::PKT_SC_USER_GUILD_INFO_READ& InData);
	// 타 캐릭터 가입시
	void Set(PD::SC::PKT_SC_NOTIFY_USER_GUILD_INFO_READ& InData);
	void Set(const FGsGuildPlayerData& InData);

	FString GetGuildName();

	void CheckAndSetGuildDiplomacy();
	bool IsEnemy() const;
	bool IsAlly() const;
	bool IsMyGuild() const;		

	static FGsGuildPlayerData::EGsGuildIffType GetGuildIff(uint64 In_guildDbId);
};
