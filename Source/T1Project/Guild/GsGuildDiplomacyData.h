// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"


//----------------------------------------------------------------------------------------------------------------------
/**
 * 기사단 외교 기본 데이터
 */
class T1PROJECT_API FGsGuildDiplomacyDataBase
{
public:
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;	
	GuildEmblemId _emblemId = INVALID_GUILD_EMBLEM_ID;
	FString _guildName;
	FString _masterName;
	PlanetWorldId _homePlanetWorldId;

protected:
	// 부족한 정보가 있으면 true
	bool _bIsValidData = false;

public:
	FGsGuildDiplomacyDataBase() = default;
	virtual ~FGsGuildDiplomacyDataBase() = default;

public:
	void SetBaseData(FGsGuildDiplomacyDataBase* InData);
	bool IsValidData() const { return _bIsValidData; }
	WorldId GetWorldId() const;
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 길드 기본정보 (가입 창에서 필요한 정보)
 */
class T1PROJECT_API FGsGuildDiplomacyDataEnemy : public FGsGuildDiplomacyDataBase
{
public:
	time_t _enemyTime; // 적대 시작한 시간

public:
	FGsGuildDiplomacyDataEnemy() = default;
	virtual ~FGsGuildDiplomacyDataEnemy() = default;

public:
	void SetListData(PD::GC::PKT_GC_ACK_GUILD_HOSTILE_LIST_READ::List& InData);
	void SetData(PD::GC::PKT_GC_ACK_GUILD_HOSTILE_ADD_READ& InData);
	void SetNotifyData(PD::GC::PKT_GC_INFORM_GUILD_HOSTILE_ADD_READ& InData);
};

//----------------------------------------------------------------------------------------------------------------------
class T1PROJECT_API FGsGuildDiplomacyDataAlly : public FGsGuildDiplomacyDataBase
{
public:
	time_t _allyTime;	

public:
	FGsGuildDiplomacyDataAlly() = default;
	virtual ~FGsGuildDiplomacyDataAlly() = default;

public:	
	void SetListData(PD::GC::PKT_GC_ACK_GUILD_ALLY_LIST_READ::List& InData);
	void SetData(PD::GC::PKT_GC_INFORM_GUILD_ALLY_READ& InData);
	void SetAllyData(FGsGuildDiplomacyDataAlly* InData);
};

//----------------------------------------------------------------------------------------------------------------------
class T1PROJECT_API FGsGuildDiplomacyDataState : public FGsGuildDiplomacyDataBase
{
public:
	time_t _endTime;
	GuildAllyType _allyType;

public:
	FGsGuildDiplomacyDataState() = default;
	virtual ~FGsGuildDiplomacyDataState() = default;

public:
	void SetData(PD::GC::PKT_GC_ACK_GUILD_ALLY_REQUEST_LIST_READ::List& InData);
	void SetData(PD::GC::PKT_GC_INFORM_GUILD_ALLY_ACCEPT_READ& InData);
	void SetStateData(FGsGuildDiplomacyDataState* InData);

	void SetEndTime(time_t InEndTime);
	FTimespan GetRemainTime() const;

	bool IsRequest() const;
};
