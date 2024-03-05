#pragma once

#include "Scan/GsScanMemberInfo.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"

/*
* 스캔 플레이어 정보
*/
class FGsGameObjectDataPlayer;

class FGsScanMemberInfoPlayer : public FGsScanMemberInfo
{
	// 생성자
public:
	FGsScanMemberInfoPlayer(int In_orderNumber,
		TWeakObjectPtr<UGsGameObjectBase> In_targetObject,
		TWeakObjectPtr<UGsGameObjectBase> In_localObject) :
		FGsScanMemberInfo(In_orderNumber, In_targetObject, In_localObject)
	{
		UpdateName();
	}

	// get
public:
	// 길드 엠블렘 id
	GuildEmblemId GetGuildEmblemId();
	// 길드에 속했는지
	bool GetIsInGuild();
	// pvp 등급
	PVPAlignmentGrade GetPVPGrade();
	// 적대 길드 인가
	bool GetEnemyGuild();
	// 보호막
	int GetNowShield();
	// 침공전 다른 서버 플레이어인지
	bool IsInvadeWorldOtherServerPlayer() const;

	FGsGameObjectDataPlayer* GetPlayerData();
};