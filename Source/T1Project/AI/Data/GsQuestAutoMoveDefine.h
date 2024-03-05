#pragma once
/*
	퀘스트 자동 이동 define
*/
// 조건
enum class EGsQuestAutoMoveConditionType
{
	IS_AUTO_PROGRESSING,					// 퀘스트 자동 진행 중인가(HUD UI ON인지)
	IS_RESERVE_QUEST_MOVE_DATA,				// 예약된 퀘스트 이동 데이터가 있는가
	IS_LOCAL_STATE_IDLE,					// 로컬 상태가 IDLE 인가
	IS_DIRTY_FORCE_START,					// 강제 이동 시작인가(액션에 의한)

	IS_NPC_INTERACTION_TARGET_NOT_EXIST,	// 인터랙션 대상 NPC 없는지
	IS_NPC_WAIT_START,						// 대기 시작 했는지
	IS_NPC_WAIT_TIME_END,					// 대기시간 끝났는지

	IS_RETRY_POSIBBLE_FAILED_RESULT,		// 재시도 가능한 실패인지
	IS_PATH_AND_NOT_ABORT_RESULT,			// PATH 에러고 취소가 아닌지(팝업 출력 체크용)

	IS_DIRTY_MOVE_FAILED,					// 최근 이동 실패한적이 있나

	IS_MANUAL_AUTO_TARGET_EXIST,			// 수동 공격 타겟(죽을때 까지 때릴)이 있는지

	IS_LOOT_POSSIBLE,						// check loot possible

	IS_QUEST_KILL_AUTO_START,				// quest kill auto start
	IS_ENEMY_EXIST,							// enemy exist check
	IS_QUEST_KILL_CENTER_INSIDE,			// quest kill center

	IS_SERVER_LOCK,							// server lock check
};

// 행동
enum class EGsQuestAutoMoveActionType
{
	QUEST_MOVE_START,				// 퀘스트 이동 시작
	QUEST_MOVE_STOP,				// 퀘스트 이동 정지
	NPC_SPAWN_WAIT_START,			// NPC 스폰 대기 시작
	SHOW_MOVE_FAILED_ERROR_POPUP,	// 이동 실패 에러 팝업 출력
	QUEST_MONSTER_KILL_CENTER_MOVE,	// center move
};