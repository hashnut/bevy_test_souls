#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedBossEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 필드(월드) 보스 타입
enum class BossType : uint8
{
	// 협동형 보스(월드보스)
	COOPERATION = 0,

	// 경쟁형 보스(필드보스)
	COMPETITION = 1,

	// 협동형 변종 보스(필드)
	COOPERATION_MUTANT = 2,

	// 경쟁형 변종 보스(필드)
	COMPETITION_MUTANT = 3,

	MAX = 4,
};

UENUM()
// 게임 디자인 실에서 사용할 보스 종류
enum class GameDesign_BossType : uint8
{
	GUILD_DUNGEON_BOSS = 0,
	FIELD_BOSS = 1,
	WORLD_BOSS = 2,
	INVADE_BOSS = 3,
	PARTY_DUNGEON_BOSS = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 필드 보스  : 스폰이 진행될 채널 지정
enum class BossSpawnChannel : uint8
{
	// 필드 보스 :스폰 확률에 따라 전체 채널에 동일하게 적용
	ALL = 0,

	// 필드 보스 :1채널
	FIRST = 1,

	// 필드 보스 : 스폰 확률을 각 채널에 독립 시행하여 적용
	EACH = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 필드 보스 : 스폰포인트 중 랜덤 1곳에 발생시킬지, 혹은 각 스폰 좌표에 모두 발생시킬지
enum class BossSpawnType : uint8
{
	// 필드 보스 : 설정된 모든 좌표에 동시 스폰. 특정한 1곳에만 발생시킬 경우 1개의 좌표 입력 & ALL 설정
	ALL = 0,

	// 필드 보스 : 설정된 좌표 중 임의의 1곳에 스폰
	RANDOM = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 필드 보스 보상 테이블 지급 방식
enum class BossRewardGiveType : uint8
{

	// 없다
	NONE = 0,

	// 필드 드랍
	FILED_DROP = 1,

	// 인벤토리로 지급
	INVENTORY = 2,

	// 우편으로 바로지급
	MAIL = 3,

	MAX = 4,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline BossType BossTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("COOPERATION"), ESearchCase::IgnoreCase) == 0)	return BossType::COOPERATION;
	if (str.Compare(TEXT("COMPETITION"), ESearchCase::IgnoreCase) == 0)	return BossType::COMPETITION;
	if (str.Compare(TEXT("COOPERATION_MUTANT"), ESearchCase::IgnoreCase) == 0)	return BossType::COOPERATION_MUTANT;
	if (str.Compare(TEXT("COMPETITION_MUTANT"), ESearchCase::IgnoreCase) == 0)	return BossType::COMPETITION_MUTANT;
	return BossType::MAX;
}
inline GameDesign_BossType GameDesign_BossTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("GUILD_DUNGEON_BOSS"), ESearchCase::IgnoreCase) == 0)	return GameDesign_BossType::GUILD_DUNGEON_BOSS;
	if (str.Compare(TEXT("FIELD_BOSS"), ESearchCase::IgnoreCase) == 0)	return GameDesign_BossType::FIELD_BOSS;
	if (str.Compare(TEXT("WORLD_BOSS"), ESearchCase::IgnoreCase) == 0)	return GameDesign_BossType::WORLD_BOSS;
	if (str.Compare(TEXT("INVADE_BOSS"), ESearchCase::IgnoreCase) == 0)	return GameDesign_BossType::INVADE_BOSS;
	if (str.Compare(TEXT("PARTY_DUNGEON_BOSS"), ESearchCase::IgnoreCase) == 0)	return GameDesign_BossType::PARTY_DUNGEON_BOSS;
	return GameDesign_BossType::MAX;
}
inline BossSpawnChannel BossSpawnChannelStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ALL"), ESearchCase::IgnoreCase) == 0)	return BossSpawnChannel::ALL;
	if (str.Compare(TEXT("FIRST"), ESearchCase::IgnoreCase) == 0)	return BossSpawnChannel::FIRST;
	if (str.Compare(TEXT("EACH"), ESearchCase::IgnoreCase) == 0)	return BossSpawnChannel::EACH;
	return BossSpawnChannel::MAX;
}
inline BossSpawnType BossSpawnTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ALL"), ESearchCase::IgnoreCase) == 0)	return BossSpawnType::ALL;
	if (str.Compare(TEXT("RANDOM"), ESearchCase::IgnoreCase) == 0)	return BossSpawnType::RANDOM;
	return BossSpawnType::MAX;
}
inline BossRewardGiveType BossRewardGiveTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return BossRewardGiveType::NONE;
	if (str.Compare(TEXT("FILED_DROP"), ESearchCase::IgnoreCase) == 0)	return BossRewardGiveType::FILED_DROP;
	if (str.Compare(TEXT("INVENTORY"), ESearchCase::IgnoreCase) == 0)	return BossRewardGiveType::INVENTORY;
	if (str.Compare(TEXT("MAIL"), ESearchCase::IgnoreCase) == 0)	return BossRewardGiveType::MAIL;
	return BossRewardGiveType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString BossTypeEnumToString(BossType value)
{
	switch (value)
	{
		case BossType::COOPERATION: return TEXT("COOPERATION");
		case BossType::COMPETITION: return TEXT("COMPETITION");
		case BossType::COOPERATION_MUTANT: return TEXT("COOPERATION_MUTANT");
		case BossType::COMPETITION_MUTANT: return TEXT("COMPETITION_MUTANT");
	}
	return FString();
}
inline FString GameDesign_BossTypeEnumToString(GameDesign_BossType value)
{
	switch (value)
	{
		case GameDesign_BossType::GUILD_DUNGEON_BOSS: return TEXT("GUILD_DUNGEON_BOSS");
		case GameDesign_BossType::FIELD_BOSS: return TEXT("FIELD_BOSS");
		case GameDesign_BossType::WORLD_BOSS: return TEXT("WORLD_BOSS");
		case GameDesign_BossType::INVADE_BOSS: return TEXT("INVADE_BOSS");
		case GameDesign_BossType::PARTY_DUNGEON_BOSS: return TEXT("PARTY_DUNGEON_BOSS");
	}
	return FString();
}
inline FString BossSpawnChannelEnumToString(BossSpawnChannel value)
{
	switch (value)
	{
		case BossSpawnChannel::ALL: return TEXT("ALL");
		case BossSpawnChannel::FIRST: return TEXT("FIRST");
		case BossSpawnChannel::EACH: return TEXT("EACH");
	}
	return FString();
}
inline FString BossSpawnTypeEnumToString(BossSpawnType value)
{
	switch (value)
	{
		case BossSpawnType::ALL: return TEXT("ALL");
		case BossSpawnType::RANDOM: return TEXT("RANDOM");
	}
	return FString();
}
inline FString BossRewardGiveTypeEnumToString(BossRewardGiveType value)
{
	switch (value)
	{
		case BossRewardGiveType::NONE: return TEXT("NONE");
		case BossRewardGiveType::FILED_DROP: return TEXT("FILED_DROP");
		case BossRewardGiveType::INVENTORY: return TEXT("INVENTORY");
		case BossRewardGiveType::MAIL: return TEXT("MAIL");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString BossTypeEnumToDisplayKey(BossType value)
{
	switch (value)
	{
		case BossType::COOPERATION: return TEXT("");
		case BossType::COMPETITION: return TEXT("");
		case BossType::COOPERATION_MUTANT: return TEXT("");
		case BossType::COMPETITION_MUTANT: return TEXT("");
	}
	return FString();
}
inline FString GameDesign_BossTypeEnumToDisplayKey(GameDesign_BossType value)
{
	switch (value)
	{
		case GameDesign_BossType::GUILD_DUNGEON_BOSS: return TEXT("");
		case GameDesign_BossType::FIELD_BOSS: return TEXT("");
		case GameDesign_BossType::WORLD_BOSS: return TEXT("");
		case GameDesign_BossType::INVADE_BOSS: return TEXT("");
		case GameDesign_BossType::PARTY_DUNGEON_BOSS: return TEXT("");
	}
	return FString();
}
inline FString BossSpawnChannelEnumToDisplayKey(BossSpawnChannel value)
{
	switch (value)
	{
		case BossSpawnChannel::ALL: return TEXT("");
		case BossSpawnChannel::FIRST: return TEXT("");
		case BossSpawnChannel::EACH: return TEXT("");
	}
	return FString();
}
inline FString BossSpawnTypeEnumToDisplayKey(BossSpawnType value)
{
	switch (value)
	{
		case BossSpawnType::ALL: return TEXT("");
		case BossSpawnType::RANDOM: return TEXT("");
	}
	return FString();
}
inline FString BossRewardGiveTypeEnumToDisplayKey(BossRewardGiveType value)
{
	switch (value)
	{
		case BossRewardGiveType::NONE: return TEXT("");
		case BossRewardGiveType::FILED_DROP: return TEXT("");
		case BossRewardGiveType::INVENTORY: return TEXT("");
		case BossRewardGiveType::MAIL: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidBossType(BossType value) noexcept
{
	return (value >= BossType::COOPERATION && value < BossType::MAX);
}

inline bool IsValidGameDesign_BossType(GameDesign_BossType value) noexcept
{
	return (value >= GameDesign_BossType::GUILD_DUNGEON_BOSS && value < GameDesign_BossType::MAX);
}

inline bool IsValidBossSpawnChannel(BossSpawnChannel value) noexcept
{
	return (value >= BossSpawnChannel::ALL && value < BossSpawnChannel::MAX);
}

inline bool IsValidBossSpawnType(BossSpawnType value) noexcept
{
	return (value >= BossSpawnType::ALL && value < BossSpawnType::MAX);
}

inline bool IsValidBossRewardGiveType(BossRewardGiveType value) noexcept
{
	return (value >= BossRewardGiveType::NONE && value < BossRewardGiveType::MAX);
}

