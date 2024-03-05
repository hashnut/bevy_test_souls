#pragma once

// 스캔 정렬 대상 타입
// 우선순위2
enum class EGsScanSortTypeTarget
{
	Boss				= 0,		// 보스
	Camp				= 1,		// camp
	User				= 2,		// 유저
	TreasureMonster		= 3,		// treasure monster
	Monster				= 4,		// 몬스터	
};

// 스캔 정렬 보스 타입
// 우선순위4
enum class EGsScanSortTypeBoss
{
	Raid	= 0,		// 레이드
	Boss	= 1,		// 보스
	Named	= 2,		// 네임드
};

// 스캔 정렬 유저 타입
// 우선순위4
enum class EGsScanSortTypeUser
{
	Attacker	= 0,		// 공격자
	EnemyGuild	= 1,		// 적대 기사단
	Alert		= 2,		// 경계대상
	OtherServer = 3,		// other server remote
	Chaotic		= 4,		// 카오(pvp 성향 1th)
	Threat		= 5,		// 위협(pvp 성향 2th)
	Etc			= 6,		// 기타
};

// 스캔 정렬 몬스터 타입
// 우선순위4
enum class EGsScanSortTypeMonster
{
	Attacker		= 0,		// 공격자
	QuestMonster	= 1,		// 퀘스트 몬스터
	NormalMonster	= 2,		// 일반 몬스터
};

// 정보 타입(개별 갱신용)
enum class EGsScanMemberInfoType : uint32
{
	LocalTarget		= 0x1,			// 로컬이 타겟 잡음			(0)
	Attacker		= 0x2,			// 공격자					(1)
	Dimmed			= 0x4,			// 딤드(거리, 시야밖 디스폰)	(2)	
};
// 스캔 옵션 타겟 타입
enum class EGsScanOptionTargetType : uint32
{
	Boss					= 0x1,			// 보스						(0)
	Named					= 0x2,			// named boss				(1)

	AttackerRemote			= 0x4,			// attacker remote			(2)
	EnemyGuild				= 0x8,			// enemy guild remote		(3)
	AlertTarget				= 0x10,			// alert target remote		(4)
	Chaos					= 0x20,			// chaos remote				(5)
	NormalRemote			= 0x40,			// normal remote			(6)

	AttackerMonster			= 0x80,			// attacker monster			(7)
	QuestMonster			= 0x100,		// quest monster			(8)
	NormalMonster			= 0x200,		// 일반 몬스터				(9)

	TreasureMonster			= 0x400,		// treasure monster			(10)
	Camp					= 0x800,		// camp 					(11)

	OtherServerRemote		= 0x1000,		// other server remote		(12)
};

// scan option group target type
enum class EGsScanOptionGroupTargetType : uint32
{
	Boss					= 0x1,			// boss						(0)
	Remote					= 0x2,			// remote					(1)
	Monster					= 0x4,			// monster					(2)
	ETC						= 0x8,			// ETC						(3)
};

// 스캔 스태틱 함수 
// 늘어나면 별도 파일 분리 필요
class FGsScaneFunc
{
public:
	// 기본 스캔 탐지 대상 값(ini 최초 로드 값)
	static uint32 GetDefaultScanDetectType()
	{
		// 일반 몬스터만 넣자
		uint32 findVal = (uint32)EGsScanOptionTargetType::NormalMonster;		
		return findVal;
	}


	static uint32 GetDefaultScanDetectGroupType()
	{
		uint32 findVal = (uint32)EGsScanOptionGroupTargetType::Boss |
						 (uint32)EGsScanOptionGroupTargetType::Remote |
						 (uint32)EGsScanOptionGroupTargetType::Monster |
						 (uint32)EGsScanOptionGroupTargetType::ETC;
		return findVal;
	}
};
