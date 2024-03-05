#pragma once
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"


struct FGsPartyDungeonPartyStatusList
{
	// 2023/4/26 PKT - Dungeon Info
	PartyId _partyId;
	DungeonGroupId _dungeonGroupId;
	DungeonDifficulty _difficulty;
	PartyDungeonStepStatus _status;
	bool _canForcedEnter;

	// 2023/4/26 PKT - Leader Info
	FString _leaderName;
	Level _leaderLevel;
	CreatureWeaponType _leaderWeaponType;

	// 2023/4/26 PKT - Member Info
	TArray<TPair<CreatureWeaponType,Level>> _memberList;
};