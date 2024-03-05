

#pragma once

#include "CoreMinimal.h"
#include "ECoopPartyEnum.h
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"

/**
* file		GsCoopPartyData.h
* @brief	협동 던전 내 필요한 Data 정의
* @author	PKT
* @date		2022/05/20
**/


/**
 * 2022/05/20 PKT - Window 던전 파티원 리스트 멤버 정보
 */
struct FGsCoopPartyMemberData
{
	FName				_name;		// 2022/05/20 PKT - 이름 정보
	bool				_isLeader;	// 2022/05/20 PKT - 파티장 Flag
	Level				_level;		// 2022/05/20 PKT - Level 정보
	CreatureWeaponType	_weapon;	// 2022/05/20 PKT - 무기 정보
};

/**
 * 2022/05/20 PKT - Window 던전 파티 목록 Item
 */
struct FGsCoopPartyListItemData
{
	PartyId _partyId = INVALID_PARTY_ID;							// 2022/05/27 PKT - 파티 고유 아이디
	DungeonGroupId _groupId = INVALID_DUNGEON_GROUP_ID;				// 2022/05/27 PKT - 던전 그룹 아이디
	DungeonDifficulty _difficulty = DungeonDifficulty::NONE;		// 2022/05/27 PKT - 던전 난이도
	PartyDungeonStepStatus _status = PartyDungeonStepStatus::MAX;	// 2022/05/27 PKT - 던전 진행 상태	
	bool _canForcedEnter = false;									// 2022/05/24 PKT - 던전 내 진입하여 이미 플레이 중인가?
	TArray<FGsCoopPartyMemberData> _memberList;						// 2022/05/24 PKT - 파티원 정보
};