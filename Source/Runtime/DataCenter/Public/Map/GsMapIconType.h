#pragma once

#include "GsMapIconType.generated.h"

/*
	맵 아이콘 타입 이넘
*/
UENUM(BlueprintType)
enum class EGsMapIconType : uint8 
{
	None,					// 없음
	LocalPlayer,			// 나
	RemoteTeamRed,			// 타유저 레드팀
	RemoteTeamBlue,			// 타유저 블루팀
	Monster,				// 몬스터
	BossMonster,			// 보스 몬스터
	MainQuest,				// 메인 퀘스트 
	Purchase,				// 구매(임시)
	Sale,					// 판매(임시)
	Revival,				// 회복(임시)
	Portal,					// 포털	
	Bookmark,				// 위치 즐겨찾기
	Party1,					// 파티원 1
	Party2,					// 파티원 2
	Party3,					// 파티원 3
	Party4,					// 파티원 4
	Party1_OtherChannel,	// 파티원 1(다른채널)
	Party2_OtherChannel,	// 파티원 2(다른채널)
	Party3_OtherChannel,	// 파티원 3(다른채널)
	Party4_OtherChannel,	// 파티원 4(다른채널)
	SubQuest,				// 서브 퀘스트
	Depot,					// 창고
	SpaceCrackPortal,		// 시공의 틈새 포탈
	RepeatQuest,			// 반복 퀘스트
	MyTreasureMonster,		// 보물 몬스터
	TreasureMonster,		// 보물 몬스터
	DetectionTarget,		// 2022/09/29 PKT - 경계 대상
	InvasionGuardian,		// invasion guardian
	InvasionMonster,		// invasion monster
	InvasionWagon,			// invasion wagon
	InvasionTreasureBox,	// invasion treasure box
	MonsterDrop1,			// 몬스터 드랍 스폰 위치 1순위
	MonsterDrop2,			// 몬스터 드랍 스폰 위치 2순위
	MonsterDrop3,			// 몬스터 드랍 스폰 위치 3순위
	InvasionPortal,			// invasion portal
	AutoMovePath,			// 자동 이동 경로
	Agit_Dungeon,			// 원정대 관리자 npc(기사단 영지) https://jira.com2us.com/jira/browse/C2URWQ-8207
};
/* 대륙맵 아이콘 타입*/
UENUM(BlueprintType)
enum class EGsContinentalMapIconType : uint8
{
	None,					// 없음
	LocalPlayer,			// 나
	Party1,					// 파티원 1
	Party2,					// 파티원 2
	Party3,					// 파티원 3
	Party4,					// 파티원 4
	Party1_OtherChannel,	// 파티원 1(다른채널)
	Party2_OtherChannel,	// 파티원 2(다른채널)
	Party3_OtherChannel,	// 파티원 3(다른채널)
	Party4_OtherChannel,	// 파티원 4(다른채널)
};

/* 지역맵 아이콘 타입*/
UENUM(BlueprintType)
enum class EGsRegionMapIconType : uint8
{
	None,					// 없음
	LocalPlayer,			// 나
	Party1,					// 파티원 1
	Party2,					// 파티원 2
	Party3,					// 파티원 3
	Party4,					// 파티원 4
	Party1_OtherChannel,	// 파티원 1(다른채널)
	Party2_OtherChannel,	// 파티원 2(다른채널)
	Party3_OtherChannel,	// 파티원 3(다른채널)
	Party4_OtherChannel,	// 파티원 4(다른채널)
	Portal,					// 포털	
	TeleportSpot,			// 텔레포트 거점
	AutoMoveTargetPos,		// 자동 이동 목적지
	Bookmark,				// 즐겨 찾기
	SpaceCrackPortal,		// 시공의 틈새 포탈
	FieldBoss,				// 필드 보스
	MyTreasureMonster,		// 보물 몬스터
	TreasureMonster,		// 보물 몬스터
	InvasionPortal,			// 침공 포탈
	Portal_In_Town,			// portal(in town)
	InvasionGuardian,		// invasion guardian
	InvasionMonster,		// invasion monster
	InvasionWagon,			// invasion wagon
	MonsterDrop1,			// 몬스터 드랍 스폰 위치 1순위
	MonsterDrop2,			// 몬스터 드랍 스폰 위치 2순위
	MonsterDrop3,			// 몬스터 드랍 스폰 위치 3순위
	Sanctum,				// 성소
};
/*region map icon filtering*/
UENUM(BlueprintType)
enum class EGsRegionMapIconFiltering : uint8
{
	Party,
	TeleportSpot,			//  warp(map - map)
	Bookmark,
	SpaceCrackPortal,
	FieldBoss,
	Sanctum,				// 성소
	MAX_COUNT,
};

