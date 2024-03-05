#pragma once
#include "CoreMinimal.h"

/*
	맵전환 월드맵
*/
enum class EGsWorldMapChangeType : uint8
{
	NONE,							// 없음
	TO_REGIONMAP_AUTOMOVEPOS,		// 지역맵: 목적지
	TO_REGIONMAP_MYPOS,				// 지역맵: 내위치
	TO_REGIONMAP_CLICK_ITEM,		// 지역맵: 아이템 클릭
	TO_REGIONMAP_CLICK_MINIMAP,		// 지역맵: 미니맵 클릭
	TO_REGIONMAP_OLD_REGIONMAP,		// 지역맵: 이전맵
	TO_REGIONMAP_DUNGEON_MONSTERINF0, // 지역맵: 던전에서 몬스터 정보
	TO_REGIONMAP_BOOKMARK,			// 지역맵: 위치 즐겨 찾기에서 클릭
	TO_REGIONMAP_INVASION,			// 침공전 정보에서 포털 활성화된 맵 클릭
	TO_REGIONMAP_DROP,				// 아이템 상세창에서 획득처 -> 맵 클릭
	TO_REGIONMAP_MonsterInfoPopup,	// 지역맵: 몬스터 정보 팝업과 같이 열기(도감에서 호출)
};

/*
	지역 몬스터 드롭 아이템 타입
*/
enum class EGsRegionMonsterDropItemType : uint8
{
	Equip,			// 장비
	Etc,			// 기타
};

/*
	지역맵 타입
*/

enum class EGsRegionMapType : uint8
{
	Spoils = 0,			// 전리품		
	Monster = 1,		// 몬스터	
	Sanctum = 2,			// 성소
};

// 지역맵 워프 리스트 아이템
enum class EGsRegionWarpListItemType : uint8
{
	PortalTypeDesc = 0,		// 포털 설명
	BookmarkTypeDesc = 1,	// 즐겨찾기 설명
	Portal = 2,				// 포털
	Bookmark = 3,			// 즐겨 찾기
	MonsterDrop = 4,		// 몬스터 스폰
	EmptyDesc = 5,			// 비어있다는 설명
};

enum class EGsContinentalSpaceCrackListItemType : uint8
{
	ActiveInfoItem		= 0,			// active information item
	DeactiveInfoItem	= 1,			// deactive information item
	LineDeco			= 2,			// line decoration
};

// 지역맵 아이콘 중 선택 처리 되는 타입
enum class EGsRegionIconSelectType : uint8 
{
	PortalIcon  = 0,				// 포털 아이콘
	BookmarkIcon = 1,				// 즐겨찾기 아이콘
	AutoMoveTargetPosIcon = 2,		// 자동 이동 목표 위치 아이콘 
	MonsterDropIcon = 3,			// 몬스터 드랍 스폰 위치
};
// 지역맵: 필드 보스 상태
enum class EGsRegionFieldBossState : uint8 
{
	SpawnReady = 0,					// 스폰대기(시간표시)
	NotSpawn = 1,					// 스폰 x
	Spawn = 2,						// 스폰 o
};

// 월드맵: 팝업 워프 타입
enum class EGsWorldmapPopupWarpType : uint8
{
	Bookmark = 0,				// 즐겨찾기
	Portal = 1,					// 포탈
	MonsterDrop = 2,			// 몬스터 드랍
	InvasionPortal = 3,			// 침공 포탈
};

// region map: monster switch type
enum class EGsReionMapMonsterSwitchType : uint8
{
	Normal = 0,				
	Boss = 1, 
	FieldBossCompetition = 2,
};

enum class EGsRegionMapMonsterSpawnTimeSwitchType : uint8
{
	Cooperation = 0,
	Competition = 1,
};

enum class EGsRegionEtcItemSecondSortType : uint8
{
	SKILLBOOK			= 0,
	CRAFT				= 1,
	SCROLL				= 2,
	MONSTER_KNOWLEDGE	= 3,
	ETC					= 4,
};
// 월드맵 개선(드랍템 장비, etc 통합)
// 1. 등급
// 2. 아이템 메인 분류(Weapon-Armor-Accessary-Skillbook-Else)
// 3. 아이템 id 순(오름 차순)
// https://jira.com2us.com/jira/browse/CHR-23756
enum class EGsRegionItemSecondSortType : uint8
{
	WEAPON		= 0,
	ARMOR		= 1,
	ACCESSARY	= 2,
	SKILLBOOK	= 3,
	ETC			= 4,
};

enum class EGsRegionItemInfoSetType : uint8
{
	Normal			= 0,
	Invade_Attack	= 1,
	Invade_Defense	= 2,
};

enum class EGsRegionInvadeNPCType : uint8
{
	Guardian		= 0,
	Monster			= 1,
	Wagon			= 2,
};

enum class EGsRegionInvadeNPCNewInfoResponse :uint8 
{
	Add				= 0,
	Del				= 1,
	Update			= 2,
	None			= 3,
};
// 월드맵 개선
// 드랍 위치(몬스터 위치)도 아이콘 메뉴 지원
// https://jira.com2us.com/jira/browse/CHR-23756
enum class EGsIconMenuType : uint8
{
	NormalPortal	= 0,		// 일반 포탈
	InvasionPortal	= 1,		// 침공넘어가서 포탈
	MonsterPos		= 2,		// 몬스터 위치
	Sanctum			= 3,		// 성소
};
// 기존엔 일반 필드 or 던전만 처리 했는데(bool)
// 기사단 영지 콤보 박스 처리때문에... 이넘으로 추가
enum class EGsWorldMapGroupType : uint8
{
	Field			= 0,		// 필드 타입	MapType::PUBLIC_FIELD, MapType::INTER_FIELD
	Dungeon			= 1,		// 던전 타입 MapType::PUBLIC_DUNGEON, MapType::ELITE_DUNGEON_1, MapType::WORLD_BOSS, MapType::GUILD_WAR
	Agit			= 2,		// 아지트 타입 MapType::AGIT
};

// 전리품 탭
// https://jira.com2us.com/jira/browse/CHR-25993
enum class EGsWorldMapSpoilTabType : uint8
{
	Equip			= 0,		// 장비
	ETC				= 1,		// 기타
};