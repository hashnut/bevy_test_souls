#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedIffEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 진영
enum class IffFactionType : uint8
{

	// 없음
	NONE = 0,

	// 플레이어 사이드
	PLAYER = 1,

	// 침공 진행 중, 방어측 유저
	PLAYER_INVADE_DEFENSE = 2,

	// 침공 진행 중, 공격측 유저
	PLAYER_INVADE_ATTACK = 3,

	// 그림자 전장 RED 팀 유저
	PLAYER_ARENA_TEAM_RED = 4,

	// 그림자 전장 BLUE 팀 유저
	PLAYER_ARENA_TEAM_BLUE = 5,

	// PLAYER_ENEMY 아닌 유저
	PLAYER_ALLY = 6,

	// 경계, 적대 기사단, 카오 플레이어
	PLAYER_ENEMY = 7,

	// 시공의 틈새 제외 몬스터
	MONSTER = 8,

	// 시공의 틈새 몬스터
	MONSTER_SPACECRACK = 9,

	// 보물 몬스터
	MONSTER_TREASURE = 10,

	// 중립
	NEUTRAL = 11,

	// 호위 퀘스트 NPC
	NPC_ESCORT = 12,

	// 경비병 NPC
	NPC_GUARD = 13,

	// 침공 수호병 NPC
	NPC_INVASIONGUARDIAN = 14,

	// 침공 마차 NPC
	NPC_WAGON = 15,

	// 침공 마차 호위병 NPC
	NPC_WAGONGUARD = 16,

	// 침공 마차 행상인 NPC
	NPC_WAGONPEDDLER = 17,

	// 침공 마차 도적단 NPC
	NPC_WAGONTHIEF = 18,

	// 그림자 전장 RED 팀 NPC
	NPC_ARENA_TEAM_RED = 19,

	// 그림자 전장 BLUE 팀 NPC
	NPC_ARENA_TEAM_BLUE = 20,

	MAX = 21,
};

UENUM(BlueprintType)
// 팀
enum class IffTeamType : uint8
{

	// 중립 팀
	NEUTRAL = 0,

	// Free-For-All
	FFA = 1,

	// 레드 팀
	RED = 2,

	// 블루 팀
	BLUE = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 피아 구분을 위한 필터 타입
enum class IffApplyType : uint8
{

	// 해당 사항 없음 (발사체 생성 등의 중립적인 이벤트에 사용됨)
	NONE = 0,

	// 자신
	SELF = 1,

	// 적
	ENEMY = 2,

	// 매개체(medium) 제외 캐스터(caster)의 적 대상
	ENEMY_WITHOUT_TARGET = 3,

	// 같은 파티원 (자신도 포함)
	PARTY = 4,

	// 동맹 (자신도 포함)
	ALLY = 5,

	// 모든 대상 (자신 포함)
	ALL = 6,

	// 모든 대상 (자신 제외)
	ALL_WITHOUT_SELF = 7,

	// 길드 (자신 제외)
	GUILD = 8,

	// 자신을 소환한 소환사 대상
	SUMMONER = 9,

	// 자신이 소환한 소환수 대상
	SUMMONEE = 10,

	MAX = 11,
};

UENUM(BlueprintType)
// PVP 성향 등급
enum class PVPAlignmentGrade : uint8
{

	// NORMAL
	NETURAL = 0,

	// 위협 단계 - 점수대역에 따라, duration에 따라 풀림
	THREAT = 1,

	// 카오틱
	CHAOTIC = 2,
};

UENUM(BlueprintType)
// 대상과 IffMatch를 하는 용도
enum class IffCheckType : uint8
{

	// 스캔 시 사용
	SCAN = 0,

	// 대상 선정 시 사용
	TARGET = 1,

	// 스킬 발동 시 사용
	ATTACK = 2,

	// 스킬 효과의 추가 대상 선정 시 사용
	EXTENSIVE = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 침공 중인지, 공격인지 수비인지 확인하는 용도
enum class IffInvadeType : uint8
{

	// 침공전 미참여
	NONE = 0,

	// 침공전 공격
	INVADE = 1,

	// 침공전 수비
	DEFENCE = 2,

	MAX = 3,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline IffFactionType IffFactionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NONE;
	if (str.Compare(TEXT("PLAYER"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::PLAYER;
	if (str.Compare(TEXT("PLAYER_INVADE_DEFENSE"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::PLAYER_INVADE_DEFENSE;
	if (str.Compare(TEXT("PLAYER_INVADE_ATTACK"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::PLAYER_INVADE_ATTACK;
	if (str.Compare(TEXT("PLAYER_ARENA_TEAM_RED"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::PLAYER_ARENA_TEAM_RED;
	if (str.Compare(TEXT("PLAYER_ARENA_TEAM_BLUE"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::PLAYER_ARENA_TEAM_BLUE;
	if (str.Compare(TEXT("PLAYER_ALLY"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::PLAYER_ALLY;
	if (str.Compare(TEXT("PLAYER_ENEMY"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::PLAYER_ENEMY;
	if (str.Compare(TEXT("MONSTER"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::MONSTER;
	if (str.Compare(TEXT("MONSTER_SPACECRACK"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::MONSTER_SPACECRACK;
	if (str.Compare(TEXT("MONSTER_TREASURE"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::MONSTER_TREASURE;
	if (str.Compare(TEXT("NEUTRAL"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NEUTRAL;
	if (str.Compare(TEXT("NPC_ESCORT"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NPC_ESCORT;
	if (str.Compare(TEXT("NPC_GUARD"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NPC_GUARD;
	if (str.Compare(TEXT("NPC_INVASIONGUARDIAN"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NPC_INVASIONGUARDIAN;
	if (str.Compare(TEXT("NPC_WAGON"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NPC_WAGON;
	if (str.Compare(TEXT("NPC_WAGONGUARD"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NPC_WAGONGUARD;
	if (str.Compare(TEXT("NPC_WAGONPEDDLER"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NPC_WAGONPEDDLER;
	if (str.Compare(TEXT("NPC_WAGONTHIEF"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NPC_WAGONTHIEF;
	if (str.Compare(TEXT("NPC_ARENA_TEAM_RED"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NPC_ARENA_TEAM_RED;
	if (str.Compare(TEXT("NPC_ARENA_TEAM_BLUE"), ESearchCase::IgnoreCase) == 0)	return IffFactionType::NPC_ARENA_TEAM_BLUE;
	return IffFactionType::MAX;
}
inline IffTeamType IffTeamTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NEUTRAL"), ESearchCase::IgnoreCase) == 0)	return IffTeamType::NEUTRAL;
	if (str.Compare(TEXT("FFA"), ESearchCase::IgnoreCase) == 0)	return IffTeamType::FFA;
	if (str.Compare(TEXT("RED"), ESearchCase::IgnoreCase) == 0)	return IffTeamType::RED;
	if (str.Compare(TEXT("BLUE"), ESearchCase::IgnoreCase) == 0)	return IffTeamType::BLUE;
	return IffTeamType::MAX;
}
inline IffApplyType IffApplyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::NONE;
	if (str.Compare(TEXT("SELF"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::SELF;
	if (str.Compare(TEXT("ENEMY"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::ENEMY;
	if (str.Compare(TEXT("ENEMY_WITHOUT_TARGET"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::ENEMY_WITHOUT_TARGET;
	if (str.Compare(TEXT("PARTY"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::PARTY;
	if (str.Compare(TEXT("ALLY"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::ALLY;
	if (str.Compare(TEXT("ALL"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::ALL;
	if (str.Compare(TEXT("ALL_WITHOUT_SELF"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::ALL_WITHOUT_SELF;
	if (str.Compare(TEXT("GUILD"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::GUILD;
	if (str.Compare(TEXT("SUMMONER"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::SUMMONER;
	if (str.Compare(TEXT("SUMMONEE"), ESearchCase::IgnoreCase) == 0)	return IffApplyType::SUMMONEE;
	return IffApplyType::MAX;
}
inline PVPAlignmentGrade PVPAlignmentGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NETURAL"), ESearchCase::IgnoreCase) == 0)	return PVPAlignmentGrade::NETURAL;
	if (str.Compare(TEXT("THREAT"), ESearchCase::IgnoreCase) == 0)	return PVPAlignmentGrade::THREAT;
	if (str.Compare(TEXT("CHAOTIC"), ESearchCase::IgnoreCase) == 0)	return PVPAlignmentGrade::CHAOTIC;
	return PVPAlignmentGrade::CHAOTIC;
}
inline IffCheckType IffCheckTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("SCAN"), ESearchCase::IgnoreCase) == 0)	return IffCheckType::SCAN;
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return IffCheckType::TARGET;
	if (str.Compare(TEXT("ATTACK"), ESearchCase::IgnoreCase) == 0)	return IffCheckType::ATTACK;
	if (str.Compare(TEXT("EXTENSIVE"), ESearchCase::IgnoreCase) == 0)	return IffCheckType::EXTENSIVE;
	return IffCheckType::MAX;
}
inline IffInvadeType IffInvadeTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return IffInvadeType::NONE;
	if (str.Compare(TEXT("INVADE"), ESearchCase::IgnoreCase) == 0)	return IffInvadeType::INVADE;
	if (str.Compare(TEXT("DEFENCE"), ESearchCase::IgnoreCase) == 0)	return IffInvadeType::DEFENCE;
	return IffInvadeType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString IffFactionTypeEnumToString(IffFactionType value)
{
	switch (value)
	{
		case IffFactionType::NONE: return TEXT("NONE");
		case IffFactionType::PLAYER: return TEXT("PLAYER");
		case IffFactionType::PLAYER_INVADE_DEFENSE: return TEXT("PLAYER_INVADE_DEFENSE");
		case IffFactionType::PLAYER_INVADE_ATTACK: return TEXT("PLAYER_INVADE_ATTACK");
		case IffFactionType::PLAYER_ARENA_TEAM_RED: return TEXT("PLAYER_ARENA_TEAM_RED");
		case IffFactionType::PLAYER_ARENA_TEAM_BLUE: return TEXT("PLAYER_ARENA_TEAM_BLUE");
		case IffFactionType::PLAYER_ALLY: return TEXT("PLAYER_ALLY");
		case IffFactionType::PLAYER_ENEMY: return TEXT("PLAYER_ENEMY");
		case IffFactionType::MONSTER: return TEXT("MONSTER");
		case IffFactionType::MONSTER_SPACECRACK: return TEXT("MONSTER_SPACECRACK");
		case IffFactionType::MONSTER_TREASURE: return TEXT("MONSTER_TREASURE");
		case IffFactionType::NEUTRAL: return TEXT("NEUTRAL");
		case IffFactionType::NPC_ESCORT: return TEXT("NPC_ESCORT");
		case IffFactionType::NPC_GUARD: return TEXT("NPC_GUARD");
		case IffFactionType::NPC_INVASIONGUARDIAN: return TEXT("NPC_INVASIONGUARDIAN");
		case IffFactionType::NPC_WAGON: return TEXT("NPC_WAGON");
		case IffFactionType::NPC_WAGONGUARD: return TEXT("NPC_WAGONGUARD");
		case IffFactionType::NPC_WAGONPEDDLER: return TEXT("NPC_WAGONPEDDLER");
		case IffFactionType::NPC_WAGONTHIEF: return TEXT("NPC_WAGONTHIEF");
		case IffFactionType::NPC_ARENA_TEAM_RED: return TEXT("NPC_ARENA_TEAM_RED");
		case IffFactionType::NPC_ARENA_TEAM_BLUE: return TEXT("NPC_ARENA_TEAM_BLUE");
	}
	return FString();
}
inline FString IffTeamTypeEnumToString(IffTeamType value)
{
	switch (value)
	{
		case IffTeamType::NEUTRAL: return TEXT("NEUTRAL");
		case IffTeamType::FFA: return TEXT("FFA");
		case IffTeamType::RED: return TEXT("RED");
		case IffTeamType::BLUE: return TEXT("BLUE");
	}
	return FString();
}
inline FString IffApplyTypeEnumToString(IffApplyType value)
{
	switch (value)
	{
		case IffApplyType::NONE: return TEXT("NONE");
		case IffApplyType::SELF: return TEXT("SELF");
		case IffApplyType::ENEMY: return TEXT("ENEMY");
		case IffApplyType::ENEMY_WITHOUT_TARGET: return TEXT("ENEMY_WITHOUT_TARGET");
		case IffApplyType::PARTY: return TEXT("PARTY");
		case IffApplyType::ALLY: return TEXT("ALLY");
		case IffApplyType::ALL: return TEXT("ALL");
		case IffApplyType::ALL_WITHOUT_SELF: return TEXT("ALL_WITHOUT_SELF");
		case IffApplyType::GUILD: return TEXT("GUILD");
		case IffApplyType::SUMMONER: return TEXT("SUMMONER");
		case IffApplyType::SUMMONEE: return TEXT("SUMMONEE");
	}
	return FString();
}
inline FString PVPAlignmentGradeEnumToString(PVPAlignmentGrade value)
{
	switch (value)
	{
		case PVPAlignmentGrade::NETURAL: return TEXT("NETURAL");
		case PVPAlignmentGrade::THREAT: return TEXT("THREAT");
		case PVPAlignmentGrade::CHAOTIC: return TEXT("CHAOTIC");
	}
	return FString();
}
inline FString IffCheckTypeEnumToString(IffCheckType value)
{
	switch (value)
	{
		case IffCheckType::SCAN: return TEXT("SCAN");
		case IffCheckType::TARGET: return TEXT("TARGET");
		case IffCheckType::ATTACK: return TEXT("ATTACK");
		case IffCheckType::EXTENSIVE: return TEXT("EXTENSIVE");
	}
	return FString();
}
inline FString IffInvadeTypeEnumToString(IffInvadeType value)
{
	switch (value)
	{
		case IffInvadeType::NONE: return TEXT("NONE");
		case IffInvadeType::INVADE: return TEXT("INVADE");
		case IffInvadeType::DEFENCE: return TEXT("DEFENCE");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString IffFactionTypeEnumToDisplayKey(IffFactionType value)
{
	switch (value)
	{
		case IffFactionType::NONE: return TEXT("");
		case IffFactionType::PLAYER: return TEXT("");
		case IffFactionType::PLAYER_INVADE_DEFENSE: return TEXT("");
		case IffFactionType::PLAYER_INVADE_ATTACK: return TEXT("");
		case IffFactionType::PLAYER_ARENA_TEAM_RED: return TEXT("");
		case IffFactionType::PLAYER_ARENA_TEAM_BLUE: return TEXT("");
		case IffFactionType::PLAYER_ALLY: return TEXT("");
		case IffFactionType::PLAYER_ENEMY: return TEXT("");
		case IffFactionType::MONSTER: return TEXT("");
		case IffFactionType::MONSTER_SPACECRACK: return TEXT("");
		case IffFactionType::MONSTER_TREASURE: return TEXT("");
		case IffFactionType::NEUTRAL: return TEXT("");
		case IffFactionType::NPC_ESCORT: return TEXT("");
		case IffFactionType::NPC_GUARD: return TEXT("");
		case IffFactionType::NPC_INVASIONGUARDIAN: return TEXT("");
		case IffFactionType::NPC_WAGON: return TEXT("");
		case IffFactionType::NPC_WAGONGUARD: return TEXT("");
		case IffFactionType::NPC_WAGONPEDDLER: return TEXT("");
		case IffFactionType::NPC_WAGONTHIEF: return TEXT("");
		case IffFactionType::NPC_ARENA_TEAM_RED: return TEXT("");
		case IffFactionType::NPC_ARENA_TEAM_BLUE: return TEXT("");
	}
	return FString();
}
inline FString IffTeamTypeEnumToDisplayKey(IffTeamType value)
{
	switch (value)
	{
		case IffTeamType::NEUTRAL: return TEXT("");
		case IffTeamType::FFA: return TEXT("");
		case IffTeamType::RED: return TEXT("");
		case IffTeamType::BLUE: return TEXT("");
	}
	return FString();
}
inline FString IffApplyTypeEnumToDisplayKey(IffApplyType value)
{
	switch (value)
	{
		case IffApplyType::NONE: return TEXT("");
		case IffApplyType::SELF: return TEXT("");
		case IffApplyType::ENEMY: return TEXT("");
		case IffApplyType::ENEMY_WITHOUT_TARGET: return TEXT("");
		case IffApplyType::PARTY: return TEXT("");
		case IffApplyType::ALLY: return TEXT("");
		case IffApplyType::ALL: return TEXT("");
		case IffApplyType::ALL_WITHOUT_SELF: return TEXT("");
		case IffApplyType::GUILD: return TEXT("");
		case IffApplyType::SUMMONER: return TEXT("");
		case IffApplyType::SUMMONEE: return TEXT("");
	}
	return FString();
}
inline FString PVPAlignmentGradeEnumToDisplayKey(PVPAlignmentGrade value)
{
	switch (value)
	{
		case PVPAlignmentGrade::NETURAL: return TEXT("");
		case PVPAlignmentGrade::THREAT: return TEXT("");
		case PVPAlignmentGrade::CHAOTIC: return TEXT("");
	}
	return FString();
}
inline FString IffCheckTypeEnumToDisplayKey(IffCheckType value)
{
	switch (value)
	{
		case IffCheckType::SCAN: return TEXT("");
		case IffCheckType::TARGET: return TEXT("");
		case IffCheckType::ATTACK: return TEXT("");
		case IffCheckType::EXTENSIVE: return TEXT("");
	}
	return FString();
}
inline FString IffInvadeTypeEnumToDisplayKey(IffInvadeType value)
{
	switch (value)
	{
		case IffInvadeType::NONE: return TEXT("");
		case IffInvadeType::INVADE: return TEXT("");
		case IffInvadeType::DEFENCE: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidIffFactionType(IffFactionType value) noexcept
{
	return (value >= IffFactionType::NONE && value < IffFactionType::MAX);
}

inline bool IsValidIffTeamType(IffTeamType value) noexcept
{
	return (value >= IffTeamType::NEUTRAL && value < IffTeamType::MAX);
}

inline bool IsValidIffApplyType(IffApplyType value) noexcept
{
	return (value >= IffApplyType::NONE && value < IffApplyType::MAX);
}

inline bool IsValidPVPAlignmentGrade(PVPAlignmentGrade value) noexcept
{
	return (value >= PVPAlignmentGrade::NETURAL && value <= PVPAlignmentGrade::CHAOTIC);
}

inline bool IsValidIffCheckType(IffCheckType value) noexcept
{
	return (value >= IffCheckType::SCAN && value < IffCheckType::MAX);
}

inline bool IsValidIffInvadeType(IffInvadeType value) noexcept
{
	return (value >= IffInvadeType::NONE && value < IffInvadeType::MAX);
}

