#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedMonsterknowledgeEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 지식의 석판 등급
enum class MonsterKnowledgeSlateGrade : uint8
{

	// 노멀
	NORMAL = 0,

	// 매직
	MAGIC = 1,

	// 레어
	RARE = 2,

	// 에픽
	EPIC = 3,

	// 유니크
	UNIQUE = 4,

	// 레전드
	LEGEND = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// 지식의 석판 슬롯 방향
enum class MonsterKnowledgeSlateSlotDir : uint8
{

	// 방향 없음(초기값)
	NONE = 0,

	// 왼쪽
	LEFT = 1,

	// 오른쪽
	RIGHT = 2,

	// 위
	TOP = 3,

	// 아래
	BOTTOM = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 몬스터 지식 등장 위치 타입
enum class MonsterKnowledgeAppearType : uint8
{

	// 필드, value : mapId
	FIELD = 0,

	// 레벨 던전, value : 레벨 던전 MapId 리스트 중 대표 mapId
	DUNGEON_LEVEL = 1,

	// 층으로 구성된 던전, value : 각 층의 mapId
	DUNGEON_FLOOR = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 몬스터 지식 등급
enum class MonsterKnowledgeBookGrade : uint8
{

	// 에러값
	NONE = 0,

	// 등급 - NORMAL
	NORMAL = 1,

	// 등급 - MAGIC
	MAGIC = 2,

	// 등급 - RARE
	RARE = 3,

	// 등급 - EPIC
	EPIC = 4,

	// 등급 - UNIQUE
	UNIQUE = 5,

	MAX = 6,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline MonsterKnowledgeSlateGrade MonsterKnowledgeSlateGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateGrade::NORMAL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateGrade::RARE;
	if (str.Compare(TEXT("EPIC"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateGrade::EPIC;
	if (str.Compare(TEXT("UNIQUE"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateGrade::UNIQUE;
	if (str.Compare(TEXT("LEGEND"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateGrade::LEGEND;
	return MonsterKnowledgeSlateGrade::MAX;
}
inline MonsterKnowledgeSlateSlotDir MonsterKnowledgeSlateSlotDirStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateSlotDir::NONE;
	if (str.Compare(TEXT("LEFT"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateSlotDir::LEFT;
	if (str.Compare(TEXT("RIGHT"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateSlotDir::RIGHT;
	if (str.Compare(TEXT("TOP"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateSlotDir::TOP;
	if (str.Compare(TEXT("BOTTOM"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeSlateSlotDir::BOTTOM;
	return MonsterKnowledgeSlateSlotDir::MAX;
}
inline MonsterKnowledgeAppearType MonsterKnowledgeAppearTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("FIELD"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeAppearType::FIELD;
	if (str.Compare(TEXT("DUNGEON_LEVEL"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeAppearType::DUNGEON_LEVEL;
	if (str.Compare(TEXT("DUNGEON_FLOOR"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeAppearType::DUNGEON_FLOOR;
	return MonsterKnowledgeAppearType::MAX;
}
inline MonsterKnowledgeBookGrade MonsterKnowledgeBookGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeBookGrade::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeBookGrade::NORMAL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeBookGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeBookGrade::RARE;
	if (str.Compare(TEXT("EPIC"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeBookGrade::EPIC;
	if (str.Compare(TEXT("UNIQUE"), ESearchCase::IgnoreCase) == 0)	return MonsterKnowledgeBookGrade::UNIQUE;
	return MonsterKnowledgeBookGrade::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString MonsterKnowledgeSlateGradeEnumToString(MonsterKnowledgeSlateGrade value)
{
	switch (value)
	{
		case MonsterKnowledgeSlateGrade::NORMAL: return TEXT("NORMAL");
		case MonsterKnowledgeSlateGrade::MAGIC: return TEXT("MAGIC");
		case MonsterKnowledgeSlateGrade::RARE: return TEXT("RARE");
		case MonsterKnowledgeSlateGrade::EPIC: return TEXT("EPIC");
		case MonsterKnowledgeSlateGrade::UNIQUE: return TEXT("UNIQUE");
		case MonsterKnowledgeSlateGrade::LEGEND: return TEXT("LEGEND");
	}
	return FString();
}
inline FString MonsterKnowledgeSlateSlotDirEnumToString(MonsterKnowledgeSlateSlotDir value)
{
	switch (value)
	{
		case MonsterKnowledgeSlateSlotDir::NONE: return TEXT("NONE");
		case MonsterKnowledgeSlateSlotDir::LEFT: return TEXT("LEFT");
		case MonsterKnowledgeSlateSlotDir::RIGHT: return TEXT("RIGHT");
		case MonsterKnowledgeSlateSlotDir::TOP: return TEXT("TOP");
		case MonsterKnowledgeSlateSlotDir::BOTTOM: return TEXT("BOTTOM");
	}
	return FString();
}
inline FString MonsterKnowledgeAppearTypeEnumToString(MonsterKnowledgeAppearType value)
{
	switch (value)
	{
		case MonsterKnowledgeAppearType::FIELD: return TEXT("FIELD");
		case MonsterKnowledgeAppearType::DUNGEON_LEVEL: return TEXT("DUNGEON_LEVEL");
		case MonsterKnowledgeAppearType::DUNGEON_FLOOR: return TEXT("DUNGEON_FLOOR");
	}
	return FString();
}
inline FString MonsterKnowledgeBookGradeEnumToString(MonsterKnowledgeBookGrade value)
{
	switch (value)
	{
		case MonsterKnowledgeBookGrade::NONE: return TEXT("NONE");
		case MonsterKnowledgeBookGrade::NORMAL: return TEXT("NORMAL");
		case MonsterKnowledgeBookGrade::MAGIC: return TEXT("MAGIC");
		case MonsterKnowledgeBookGrade::RARE: return TEXT("RARE");
		case MonsterKnowledgeBookGrade::EPIC: return TEXT("EPIC");
		case MonsterKnowledgeBookGrade::UNIQUE: return TEXT("UNIQUE");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString MonsterKnowledgeSlateGradeEnumToDisplayKey(MonsterKnowledgeSlateGrade value)
{
	switch (value)
	{
		case MonsterKnowledgeSlateGrade::NORMAL: return TEXT("");
		case MonsterKnowledgeSlateGrade::MAGIC: return TEXT("");
		case MonsterKnowledgeSlateGrade::RARE: return TEXT("");
		case MonsterKnowledgeSlateGrade::EPIC: return TEXT("");
		case MonsterKnowledgeSlateGrade::UNIQUE: return TEXT("");
		case MonsterKnowledgeSlateGrade::LEGEND: return TEXT("");
	}
	return FString();
}
inline FString MonsterKnowledgeSlateSlotDirEnumToDisplayKey(MonsterKnowledgeSlateSlotDir value)
{
	switch (value)
	{
		case MonsterKnowledgeSlateSlotDir::NONE: return TEXT("");
		case MonsterKnowledgeSlateSlotDir::LEFT: return TEXT("");
		case MonsterKnowledgeSlateSlotDir::RIGHT: return TEXT("");
		case MonsterKnowledgeSlateSlotDir::TOP: return TEXT("");
		case MonsterKnowledgeSlateSlotDir::BOTTOM: return TEXT("");
	}
	return FString();
}
inline FString MonsterKnowledgeAppearTypeEnumToDisplayKey(MonsterKnowledgeAppearType value)
{
	switch (value)
	{
		case MonsterKnowledgeAppearType::FIELD: return TEXT("");
		case MonsterKnowledgeAppearType::DUNGEON_LEVEL: return TEXT("");
		case MonsterKnowledgeAppearType::DUNGEON_FLOOR: return TEXT("");
	}
	return FString();
}
inline FString MonsterKnowledgeBookGradeEnumToDisplayKey(MonsterKnowledgeBookGrade value)
{
	switch (value)
	{
		case MonsterKnowledgeBookGrade::NONE: return TEXT("");
		case MonsterKnowledgeBookGrade::NORMAL: return TEXT("");
		case MonsterKnowledgeBookGrade::MAGIC: return TEXT("");
		case MonsterKnowledgeBookGrade::RARE: return TEXT("");
		case MonsterKnowledgeBookGrade::EPIC: return TEXT("");
		case MonsterKnowledgeBookGrade::UNIQUE: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidMonsterKnowledgeSlateGrade(MonsterKnowledgeSlateGrade value) noexcept
{
	return (value >= MonsterKnowledgeSlateGrade::NORMAL && value < MonsterKnowledgeSlateGrade::MAX);
}

inline bool IsValidMonsterKnowledgeSlateSlotDir(MonsterKnowledgeSlateSlotDir value) noexcept
{
	return (value >= MonsterKnowledgeSlateSlotDir::NONE && value < MonsterKnowledgeSlateSlotDir::MAX);
}

inline bool IsValidMonsterKnowledgeAppearType(MonsterKnowledgeAppearType value) noexcept
{
	return (value >= MonsterKnowledgeAppearType::FIELD && value < MonsterKnowledgeAppearType::MAX);
}

inline bool IsValidMonsterKnowledgeBookGrade(MonsterKnowledgeBookGrade value) noexcept
{
	return (value >= MonsterKnowledgeBookGrade::NONE && value < MonsterKnowledgeBookGrade::MAX);
}

