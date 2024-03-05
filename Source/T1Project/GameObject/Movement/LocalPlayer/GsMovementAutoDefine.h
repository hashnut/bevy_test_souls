#pragma once

/*
	오토 디파인
*/

// 자동 이동 컨텐츠 타입
enum class EGsMovementAutoContentsType : uint8
{
	None,							// 없음
	QuestMove,						// 퀘스트 이동
	NpcListMove,					// npc list 이동
	MoveToSkill,					// 전투 이동
	RegionMapPortalMove,			// 지역맵: 포털 이동
	RegionMapBookmarkMove,			// 지역맵: 즐겨찾기 이동
	AutoReturnMove,					// 제자리 돌아가기 이동
	TouchUIMapMove,					// ui map 터치 이동
	FieldBossMove,					// move to field boss
	TerrainTouchMove,				// terrain touch move
	LootItemMove,					// for loot item
	QuestKillCenterMove,
	AIAvoidCollision,
	TouchInteractionTargetMove,		// 인터랙션 타겟 터치로 이동(인터랙션 버튼, 오브젝트 몸통) https://jira.com2us.com/jira/browse/C2URWQ-3193
};

// 이동 실패 이유
enum class EGsMoveFailedReason : uint8
{
	None,							// 없음
	InteractionNPCNotExist,			// 인터랙션 npc 없음
	ControllerAutoMoveFailed,		// 이동 실패(controller move 실패)
};