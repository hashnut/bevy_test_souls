#include "GsScanMemberInfoCamp.h"

#include "Management/GsMessageHolder.h"

#include "GameObject/ObjectClass/GsGameObjectCamp.h"

// update name
void FGsScanMemberInfoCamp::UpdateName()
{
	if (_targetObject == nullptr)
	{
		_name = "";
		return;
	}

	UGsGameObjectCamp* camp = _targetObject->CastGameObject<UGsGameObjectCamp>();

	if (camp == nullptr)
	{
		_name = "";
		return;
	}

	_name = camp->GetFormattedCampName();
}

// 길드 엠블렘 id
GuildEmblemId FGsScanMemberInfoCamp::GetGuildEmblemId()
{
	if (_targetObject == nullptr)
	{
		return 0;
	}

	UGsGameObjectCamp* camp = _targetObject->CastGameObject<UGsGameObjectCamp>();
	if (camp == nullptr)
	{
		return 0;
	}
	
	GuildEmblemId emblemId =  camp->GetGuildEmblemID();
	return emblemId;
}
// 길드에 속했는지
bool FGsScanMemberInfoCamp::GetIsInGuild()
{
	if (_targetObject == nullptr)
	{
		return false;
	}

	UGsGameObjectCamp* camp = _targetObject->CastGameObject<UGsGameObjectCamp>();
	if (camp == nullptr)
	{
		return false;
	}

	if (INVALID_GUILD_DB_ID == camp->GetOwnerGuildDbid())
	{
		return false;
	}

	return true;
}
uint64 FGsScanMemberInfoCamp::GetGuildDbId()
{
	if (_targetObject == nullptr)
	{
		return 0;
	}

	UGsGameObjectCamp* camp = _targetObject->CastGameObject<UGsGameObjectCamp>();
	if (camp == nullptr)
	{
		return 0;
	}

	return camp->GetOwnerGuildDbid();	
}