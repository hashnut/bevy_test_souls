// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildPlayerData.h"
#include "Management/ScopeGlobal/GsGuildManager.h"


void FGsGuildPlayerData::Empty()
{
	_guildDBId = INVALID_GUILD_DB_ID;
	_emblemId = 0;
	_guildName.Empty();
	_guildIff = EGsGuildIffType::NONE;
}

void FGsGuildPlayerData::Set(PD::SC::PKT_SC_USER_BASIC_DATA_READ& InData)
{
	_guildDBId = InData.GuildDBId();
	_emblemId = InData.GuildEmblemId();
	// 내 캐릭터의 길드명이라 받지 않음. 
	// 비워주지 않으면 정보가 유지되어 GetGuildName() 에서 갱신이 안됨에 유의
	_guildName.Empty(); 
	_guildIff = EGsGuildIffType::NONE;
}

void FGsGuildPlayerData::Set(PD::SC::PKT_SC_SPAWN_USER_READ& InData)
{
	_guildDBId = InData.GuildDBId();
	_emblemId = InData.GuildEmblemId();
	_guildName = InData.GuildName();
	CheckAndSetGuildDiplomacy();
}

void FGsGuildPlayerData::Set(PD::SC::PKT_SC_USER_GUILD_INFO_READ& InData)
{
	_guildDBId = InData.GuildDBId();
	_emblemId = InData.GuildEmblemId();
	_guildName = InData.GuildName();
	CheckAndSetGuildDiplomacy();
}

void FGsGuildPlayerData::Set(PD::SC::PKT_SC_NOTIFY_USER_GUILD_INFO_READ& InData)
{
	_guildDBId = InData.GuildDBId();
	_emblemId = InData.GuildEmblemId();
	_guildName = InData.GuildName();
	CheckAndSetGuildDiplomacy();
}

void FGsGuildPlayerData::Set(const FGsGuildPlayerData& InData)
{
	_guildDBId = InData._guildDBId;
	_emblemId = InData._emblemId;
	_guildName = InData._guildName;
	CheckAndSetGuildDiplomacy();
}

FString FGsGuildPlayerData::GetGuildName()
{
	// 길드 없으면 이름 없음
	if (INVALID_GUILD_DB_ID == _guildDBId)
	{
		_guildName.Empty();
	}
	else
	{
		// 내 정보일 경우 길드명이 없을 수 있어서 한 번 찾아서 저장해둔다
		if (_guildName.IsEmpty())
		{
			if (FGsGuildManager* guildMgr = GGuild())
			{
				if (const FGsGuildDataMine* myGuildData = guildMgr->GetMyGuildData())
				{	
					if (myGuildData->_guildDBId == _guildDBId)
					{
						_guildName = myGuildData->_guildName;
					}
				}
			}
		}
	}
	
	return _guildName;
}

void FGsGuildPlayerData::CheckAndSetGuildDiplomacy()
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (guildMgr->IsAllyGuild(_guildDBId))
		{
			_guildIff = EGsGuildIffType::ALLY;
			return;
		}

		if (guildMgr->IsEnemyGuild(_guildDBId))
		{
			_guildIff = EGsGuildIffType::ENEMY;
			return;
		}
	}

	_guildIff = EGsGuildIffType::NONE;
}

bool FGsGuildPlayerData::IsEnemy() const
{
	return (EGsGuildIffType::ENEMY == _guildIff) ? true : false;
}

bool FGsGuildPlayerData::IsAlly() const
{
	return (EGsGuildIffType::ALLY == _guildIff) ? true : false;
}

bool FGsGuildPlayerData::IsMyGuild() const
{
	if (INVALID_GUILD_DB_ID != _guildDBId)
	{
		GuildDBId myGuildDBId = GGuild()->GetGuildDBId();
		if (_guildDBId == myGuildDBId)
		{
			return true;
		}
	}

	return false;
}

FGsGuildPlayerData::EGsGuildIffType FGsGuildPlayerData::GetGuildIff(uint64 In_guildDbId)
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (guildMgr->IsAllyGuild(In_guildDbId))
		{
			return FGsGuildPlayerData::EGsGuildIffType::ALLY;
		}

		if (guildMgr->IsEnemyGuild(In_guildDbId))
		{	
			return FGsGuildPlayerData::EGsGuildIffType::ENEMY;
		}
	}

	return EGsGuildIffType::NONE;
}
