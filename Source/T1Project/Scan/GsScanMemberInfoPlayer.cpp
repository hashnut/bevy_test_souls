#include "GsScanMemberInfoPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"

#include "Guild/GsGuildPlayerData.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsInterServerManager.h"

// 길드 엠블렘 id
GuildEmblemId FGsScanMemberInfoPlayer::GetGuildEmblemId()
{
	if (_targetObject == nullptr)
	{
		return 0;
	}

	FGsGameObjectDataPlayer* dataPlayer = _targetObject->GetCastData<FGsGameObjectDataPlayer>();
	if (dataPlayer == nullptr)
	{
		return 0;
	}

	const FGsGuildPlayerData* guildData = dataPlayer->GetGuildData();
	if (guildData == nullptr ||
		INVALID_GUILD_DB_ID == guildData->_guildDBId)
	{
		return 0;
	}

	return guildData->_emblemId;
}
// 길드에 속했는지
bool FGsScanMemberInfoPlayer::GetIsInGuild()
{
	if (_targetObject == nullptr)
	{
		return false;
	}

	FGsGameObjectDataPlayer* dataPlayer = _targetObject->GetCastData<FGsGameObjectDataPlayer>();
	if (dataPlayer == nullptr)
	{
		return false;
	}

	const FGsGuildPlayerData* guildData = dataPlayer->GetGuildData();
	if (guildData == nullptr ||
		INVALID_GUILD_DB_ID == guildData->_guildDBId)
	{
		return false;
	}

	return true;
}
// pvp 등급
PVPAlignmentGrade FGsScanMemberInfoPlayer::GetPVPGrade()
{
	if (_targetObject == nullptr)
	{
		return PVPAlignmentGrade::NETURAL;
	}

	FGsGameObjectDataPlayer* dataPlayer = _targetObject->GetCastData<FGsGameObjectDataPlayer>();
	if (dataPlayer == nullptr)
	{
		return PVPAlignmentGrade::NETURAL;
	}

	return dataPlayer->GetUserLookInfo()->mPVPAlignmentGrade;
}
// 적대 길드 인가
bool FGsScanMemberInfoPlayer::GetEnemyGuild()
{
	if (_targetObject == nullptr)
	{
		return false;
	}

	FGsGameObjectDataPlayer* dataPlayer = _targetObject->GetCastData<FGsGameObjectDataPlayer>();
	if (dataPlayer == nullptr)
	{
		return false;
	}

	return dataPlayer->IsEnemyGuild();
}

// 보호막
int FGsScanMemberInfoPlayer::GetNowShield()
{
	if (_targetObject == nullptr)
	{
		return 0;
	}

	if (UGsGameObjectCreature* creature = _targetObject->CastGameObject<UGsGameObjectCreature>())
	{
		if (const FGsStatInfo* hpStat = creature->GetCreatureStatInfoByType(StatType::HP_SHIELD))
		{
			return hpStat->GetStatValue();
		}
	}
	return 0;
}

bool FGsScanMemberInfoPlayer::IsInvadeWorldOtherServerPlayer() const
{
	if (_targetObject.IsValid())
	{
		// 1.다른서버인지 체크
		bool isOhterServer = false;
		if (FGsGameObjectDataPlayer* playerData = _targetObject->GetCastData<FGsGameObjectDataPlayer>())
		{			
			WorldId myHomeWorldId = GGameData()->GetUserData()->mHomeWorldId;
			WorldId nowMapWorldId = GGameData()->GetLoadWorldId();

			isOhterServer = SharedFunc::HasOtherUserNamePrefix(GGameData()->GetLoadWorldType(), nowMapWorldId, myHomeWorldId, playerData->GetHomeWorldId());
		}

		// 2.현재 침공맵인지 체크(넘어갔거나 침공 가능한곳이거나)
		bool isInvadeMap = (GGameData()->IsInvadeWorld() == true || GSInterServer()->IsValidAirRaidState() == true)? true :false;

		if (isOhterServer == true && isInvadeMap == true)
		{
			return true;
		}
	}

	return false;
}

FGsGameObjectDataPlayer* FGsScanMemberInfoPlayer::GetPlayerData()
{
	if (_targetObject == nullptr)
	{
		return nullptr;
	}

	FGsGameObjectDataPlayer* dataPlayer = _targetObject->GetCastData<FGsGameObjectDataPlayer>();
	return dataPlayer;
}