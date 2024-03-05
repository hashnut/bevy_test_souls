

#pragma once
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"


/*
* 2022/09/19 PKT - 경계 대상 정보
* 
*/
struct FGsPKDetectionTargetData
{
	// 2022/09/19 PKT - 유저 Id	
	UserDBId _userDBId = INVALID_USER_DB_ID;

	// 2022/09/19 PKT - 유저 이름
	FText _targetName = FText::GetEmpty();

	// 2022/09/19 PKT - 최종 접속 시간
	time_t _lastAccess = 0;

	// 2023/9/5 PKT - 유저의 WorldId
	WorldId _worldId = INVALID_WORLD_ID;
};


// 2022/09/19 PKT - PK Recording Contributor Data
struct FGsPKRecordingContributorData
{
	// 2022/09/26 PKT - 기여 사유
	PKBookResult _reason;
	// 2022/09/26 PKT - 기여자 타입(NPC, PC)
	PKContributorType _contributorType;
	// 2022/09/19 PKT - 기여자 고유 식별 ID(User or NPC)
	PKContributorId _contributorId;
	// 2022/09/19 PKT - 기여자 이름.
	FText _name = FText::GetEmpty();
	// 2022/09/19 PKT - 길드 아이콘 정보(몬스터일 경우 길드 정보 없음.)
	GuildEmblemId _guildEmblemId = INVALID_GUILD_EMBLEM_ID;
	// 2022/09/19 PKT - 길드 이름(몬스터일 경우 길드 정보 없음.)
	FText _guildName = FText::GetEmpty();
	// 2022/09/21 PKT - 무기 정보(몬스터 일 경우 무기 정보 없음.)
	CreatureWeaponType _weaponType = CreatureWeaponType::NONE;
	// 2022/09/19 PKT - 피해량
	int32 _damegeAmount = 0;
	// 2023/8/1 PKT - 기여자 WorldId
	WorldId _homeWorldId = INVALID_WORLD_ID;
};


// 2022/09/19 PKT - PK Detail Record Data
struct FGsPKDetailRecordData
{
	// 2022/09/19 PKT - 기록에 대한 도움 정보(어시스트 정보)
	TArray<FGsPKRecordingContributorData> _contributorSet;
	// 2022/09/21 PKT - 잃은 경험치
	Exp _lostExp = 0;
	// 2022/09/21 PKT - 잃은 경험치 백분율
	float _lostExpPercent = 0.f;
	// 2022/09/21 PKT - 복수 / 조롱 남은 카운트( 기록 상태에 따라 복수 카운트가 될수도.. 조롱 카운트가 될수도.. 있음.)
	int32 _actionRemaindCount = 0;
};