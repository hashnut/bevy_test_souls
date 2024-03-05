#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"


/**
 * Battle Arena 진행 상태
 */
enum class EBattleArenaGuildWarState
{
	ENone = 0,
	ERegisterPeriod,			// 2023/11/22 PKT - Arena 신청 가능
	ERegisterCompleted,			// 2023/11/22 PKT - Arena 신청 완료
	EMatchingWait,				// 2023/12/5 PKT -  Arena 매칭 대기 상태
	EAdmissionGameAvailable,	// 2023/11/22 PKT - Arena 입장 가능
	EStandByStartGame,			// 2023/11/22 PKT - Arena 입장 후 경기 시작 대기 상태
	EGameBegin,					// 2023/12/5 PKT - 경기 시작 전 UI 세팅등을 위해 만든 타입
	EGamePlaying,				// 2023/11/22 PKT - Arena 경기 시작 및 경기 진행 중
	EGameDone,				// 2023/11/22 PKT - Arena 경기 종료
};

/**
 * 참가 대기 인원 정보, Battle Arena 내에서는 사용하지 않음. 일반 필드에서 인원 체크 용도로 사용
 */
struct FGsBattleArenaEntryInfo
{
	UserDBId _userdbId = INVALID_USER_DB_ID;
	FText _name = FText::GetEmpty();
};

/**
 * Battle Arena 에서 점수 현황에서 사용
 */
struct FGsBattleArenaGuildWarMemberInfo
{
	UserDBId _userDBId = INVALID_USER_DB_ID;
	FText _name;
	int32 _point = 0;
	int32 _killCount = 0;
	int32 _assistCount = 0;
	int32 _deathCount = 0;
};

/**
 * HUD 표기 용도 및 점수 현황판 사용 용도.
 */
struct FGsBattleArenaGuildWarTeamInfo
{
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;
	FText _guildName = FText::GetEmpty();
	GuildEmblemId _guildEmblemId = INVALID_GUILD_EMBLEM_ID;
	WorldId _worldId = INVALID_WORLD_ID;
	int32 _score = 0;
	TArray<FGsBattleArenaGuildWarMemberInfo> _members;

	void Reset()
	{
		_guildDBId = INVALID_GUILD_DB_ID;
		_guildName = FText::GetEmpty();
		_guildEmblemId = INVALID_GUILD_EMBLEM_ID;
		_worldId = INVALID_WORLD_ID;
		_score = 0;
		_members.Empty(0);
	}
};

/**
 * Battle Arena 결과( 승패, 승패 사유, 나의 포인트 )
 */
struct FGSBattleArenaGuilWarResultData
{
	bool _isSaved = false;
	GuildDBId _winGuildDBId = INVALID_GUILD_DB_ID;
	GuildWarEndReason _endReason = GuildWarEndReason::MAX;
	int32 _myPointScore = 0;
	
	void Reset()
	{
		_isSaved = false;
		_winGuildDBId = INVALID_GUILD_DB_ID;
		_endReason = GuildWarEndReason::MAX;
		_myPointScore = 0;		
	}
};

struct FGsBattleArenaMemberPositionInfos
{
	TArray<TPair<UserDBId, FVector>> _memberSet;

	void Reset()
	{
		_memberSet.Empty();
	}
};