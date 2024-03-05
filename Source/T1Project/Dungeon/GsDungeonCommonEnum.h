#pragma once


enum class EDungeonProgressState
{
	ENone = 0,		// 2022/11/01 PKT - 아무런 상태가 아님.
	EMatching,		// 2022/11/01 PKT - 매칭 중
	EJoinParty,		// 2022/11/01 PKT - 파티에 참여.
	EReady,			// 2022/12/13 PKT - 던전 입장 준비 완료.
	EInDungeon,		// 2022/11/01 PKT - 던전에 들어 감.
};


enum class EDungeonHelpseekingState
{
	ENone = 0,		// 2022/11/01 PKT - 아무런 상태가 아님.
	EActive,		// 2022/11/01 PKT - 활성화
	EWait,			// 2022/11/01 PKT - 대기
	EInActive,		// 2022/11/01 PKT - 비활성
};