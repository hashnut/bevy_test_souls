// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildDiplomacyData.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"

//----------------------------------------------------------------------------------------------------------------------
void FGsGuildDiplomacyDataBase::SetBaseData(FGsGuildDiplomacyDataBase* InData)
{
	_guildDBId = InData->_guildDBId;
	_emblemId = InData->_emblemId;
	_guildName = InData->_guildName;
	_masterName = InData->_masterName;
	_homePlanetWorldId.value = InData->_homePlanetWorldId.value;
}

WorldId FGsGuildDiplomacyDataBase::GetWorldId() const
{
	return _homePlanetWorldId.st.worldId;
}

//----------------------------------------------------------------------------------------------------------------------
void FGsGuildDiplomacyDataEnemy::SetListData(PD::GC::PKT_GC_ACK_GUILD_HOSTILE_LIST_READ::List& InData)
{
	_guildDBId = InData.GuildId();
	_emblemId = InData.GuildEmblemId();
	_guildName = InData.GuildName();
	_masterName = InData.GuildCaptianName();
	_homePlanetWorldId.value = InData.ConstRefCurrentHomePlanetWorldId().value;

	_enemyTime = InData.DateTime();

	_bIsValidData = true;
}

void FGsGuildDiplomacyDataEnemy::SetData(PD::GC::PKT_GC_ACK_GUILD_HOSTILE_ADD_READ& InData)
{
	_guildDBId = InData.GuildId();
	_emblemId = InData.GuildEmblemId();
	_guildName = InData.GuildName();
	_masterName = InData.GuildCaptianName();
	_homePlanetWorldId.value = InData.ConstRefCurrentHomePlanetWorldId().value;

	_enemyTime = InData.DateTime();

	_bIsValidData = true;
}

void FGsGuildDiplomacyDataEnemy::SetNotifyData(PD::GC::PKT_GC_INFORM_GUILD_HOSTILE_ADD_READ& InData)
{
	_guildDBId = InData.GuildDBId();
	//_emblemId
	_guildName = InData.GuildName();
	//_masterName
	_homePlanetWorldId.value = InData.ConstRefCurrentHomePlanetWorldId().value;	
	
	//_enemyTime

	_bIsValidData = false;
}

//----------------------------------------------------------------------------------------------------------------------
void FGsGuildDiplomacyDataAlly::SetListData(PD::GC::PKT_GC_ACK_GUILD_ALLY_LIST_READ::List& InData)
{
	_guildDBId = InData.GuildId();
	_emblemId = InData.GuildEmblemId();
	_guildName = InData.GuildName();
	_masterName = InData.GuildCaptianName();
	_homePlanetWorldId.value = InData.ConstRefCurrentHomePlanetWorldId().value;

	_allyTime = InData.ConfirmedDateTm();

	_bIsValidData = true;
}

void FGsGuildDiplomacyDataAlly::SetData(PD::GC::PKT_GC_INFORM_GUILD_ALLY_READ& InData)
{
	_guildDBId = InData.GuildId();
	//_emblemId
	_guildName = InData.GuildName();
	//_masterName
	_homePlanetWorldId.value = InData.ConstRefCurrentHomePlanetWorldId().value;
	
	_bIsValidData = false;
}

void FGsGuildDiplomacyDataAlly::SetAllyData(FGsGuildDiplomacyDataAlly* InData)
{
	SetBaseData(InData);

	_allyTime = InData->_allyTime;

	_bIsValidData = true;
}

//----------------------------------------------------------------------------------------------------------------------
void FGsGuildDiplomacyDataState::SetData(PD::GC::PKT_GC_ACK_GUILD_ALLY_REQUEST_LIST_READ::List& InData)
{
	_guildDBId = InData.GuildId();
	_emblemId = InData.GuildEmblemId();
	_guildName = InData.GuildName();
	_masterName = InData.GuildCaptianName();
	_homePlanetWorldId.value = InData.ConstRefCurrentHomePlanetWorldId().value;

	_allyType = InData.Type();	
	SetEndTime(InData.ExpirationDateTm());

	_bIsValidData = true;
}

void FGsGuildDiplomacyDataState::SetData(PD::GC::PKT_GC_INFORM_GUILD_ALLY_ACCEPT_READ& InData)
{
	_guildDBId = InData.GuildId();
	//_emblemId
	_guildName = InData.GuildName();
	//_masterName
	_homePlanetWorldId.value = InData.ConstRefCurrentHomePlanetWorldId().value;

	_bIsValidData = false;
}

void FGsGuildDiplomacyDataState::SetStateData(FGsGuildDiplomacyDataState* InData)
{
	SetBaseData(InData);

	_allyType = InData->_allyType;
	SetEndTime(InData->_endTime);

	_bIsValidData = true;
}

void FGsGuildDiplomacyDataState::SetEndTime(time_t InEndTime)
{
	_endTime = InEndTime;
}

FTimespan FGsGuildDiplomacyDataState::GetRemainTime() const
{
	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
	time_t remainSec = 0;
	if (serverNowSec < _endTime)
	{
		remainSec = _endTime - serverNowSec;
	}

	return FTimespan::FromSeconds(remainSec);
}

bool FGsGuildDiplomacyDataState::IsRequest() const
{
	return (GuildAllyType::GUILD_ALLY_WAITING_FOR_REQUESTS == _allyType) ? true : false;
}
