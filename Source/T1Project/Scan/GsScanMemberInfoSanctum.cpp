// Fill out your copyright notice in the Description page of Project Settings.


#include "GsScanMemberInfoSanctum.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumBase.h"


void FGsScanMemberInfoSanctum::UpdateName()
{
	if (_targetObject.IsValid())
	{
		if (UGsGameObjectSanctumBase* sanctumObj = _targetObject->CastGameObject<UGsGameObjectSanctumBase>())
		{
			_name = sanctumObj->GetFormattedSanctumName();
			return;
		}
	}

	_name = TEXT("");
}

GuildEmblemId FGsScanMemberInfoSanctum::GetGuildEmblemId()
{
	if (_targetObject.IsValid())
	{
		if (UGsGameObjectSanctumBase* sanctumObj = _targetObject->CastGameObject<UGsGameObjectSanctumBase>())
		{
			return sanctumObj->GetGuildEmblemId();
		}
	}

	return INVALID_GUILD_EMBLEM_ID;
}

bool FGsScanMemberInfoSanctum::GetIsInGuild()
{
	if (_targetObject.IsValid())
	{
		if (UGsGameObjectSanctumBase* sanctumObj = _targetObject->CastGameObject<UGsGameObjectSanctumBase>())
		{
			if (INVALID_GUILD_DB_ID != sanctumObj->GetGuildDBId())
			{
				return true;
			}
		}
	}

	return false;
}

uint64 FGsScanMemberInfoSanctum::GetGuildDbId()
{
	if (_targetObject.IsValid())
	{
		if (UGsGameObjectSanctumBase* sanctumObj = _targetObject->CastGameObject<UGsGameObjectSanctumBase>())
		{
			return sanctumObj->GetGuildDBId();
		}
	}

	return INVALID_GUILD_DB_ID;
}
