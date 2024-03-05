#pragma once

#include "CoreMinimal.h"

#include "GsEventProgressDefine.generated.h"

UENUM(BlueprintType)
enum class EGsEventProgressType : uint8
{
	NONE,					// 없음
	//QUEST_CINEMATIC,		// 퀘스트 시네마틱
	NPC_DIALOG,				// NPC 대화
	SUMMON_ROOM,			// 뽑기 방
	UI_POPUP_ITEM_ENCHANT,	// 인첸트 팝업 UI
	UI_POPUP_ITEM_COLLECTION_ENCHANT, // 아이템 컬렉션 강화
	FAIRY_ROOM,				// 페어리 룸
	COSTUME_ROOM,			// 코스튬 룸
	NPC_SHOP,				// NPC 샵
	COMMUNITY_ROOM,			// 커뮤니티 룸
	BOSS_ROOM,				// 보스 룸
	SAVE_BATTERY,			// 절전 모드
	USER_PROFILE,			// 유저 프로필
	//QUEST_BOARD,			// 퀘스트 게시판
	PK_BOOK,				// 이벤트 프로그레스
	PERSONAL_TRADING,		// 개인 거래
	AGIT_DUNGEON,			// 기사단 영지 던전

	SKILL_PREVIEW_ROOM,		// 스킬 프리뷰 룸
	BOSS_PREVIEW_ROOM,		// 보스 프리뷰 룸

	EXCHANGE_COSTUME_ROOM,	// 코스튬 교환 룸

	UI_POPUP_RECOVERY_PENALTY,				// 페널티 복구 팝업 UI
	UI_POPUP_TELEPORT,						// 텔레포트 팝업 UI(퀘스트)

	UI_POPUP_WARP_REGION_ITEM,				// 워프 팝업 UI - 지역맵 아이템
	UI_POPUP_WARP_CONTINENTAL_DUNGEON,		// 워프 팝업 UI - 대륙맵 던전
	UI_POPUP_WARP_BOOKMARK_SHARE,			// 워프 팝업 UI - 북마크 공유
	UI_POPUP_WARP_BOOKMARK_ITEM,			// 워프 팝업 UI - 북마크 아이템
	UI_POPUP_WARP_MOVE_TO_PARTY_LEADER,		// 워프 팝업 UI - 파티장으로 이동
	UI_POPUP_WARP_REGION_MONSTER,			// 워프 팝업 UI - 몬스터 드랍 위치로 이동

	UI_POPUP_COMMON_PAY_DUNGEON_WARP,		// 공통 지불 팝업 UI - 던전 워프

	UI_POPUP_YES_NO_CHNNEL_MOVE,			// 공통 YES,NO 팝업 UI - 채널 이동

	UI_TRAY_AREA_NAME,						// 지역명 출력	
	UI_TRAY_OFFLINEPLAY_RESULT,				// 2022/11/02 PKT - 오프라인 플레이 결과 창
	UI_TRAY_PKMOCKERY_TICKER,				// 2023/1/18 PKT - PK 조롱 시스템 티커
	UI_TRAY_NOTIFY_PK_GUILD_TICKER,			// 2023/1/18 PKT - 타길드원을 죽였거나/죽었을 경우의 길드원에게 보이는 티커
	UI_TRAY_LEVEL_UP,						// 레벨 업 트레이 연출 

	UI_POPUP_EVENT_PRAYER,

	QUEST_GOTO_FIELD,						// instance field or last public field

	QUEST_EVENT_ACTION,					// dimmed show/ hide, monster spawn, cinematic show

	EVENT_ACTION_CINEMATIC,				// event action cinematic
	EVENT_ACTION_MOVIE,	
	EVENT_ACTION_SOUND,				

	EVENT_ACTION_DIMMED_ON,				// event action dimmed on

	UI_POPUP_QUESTACCEPT,				// 메인퀘스트 수락 팝업
	UI_POPUP_QUESTCOMPLETE,				// 메인퀘스트 보상 팝업

	UI_POPUP_ITEMFUSION,				// 2023/10/11 PKT - 아이템 합성
	QUEST_AUTO_TELEPORT,				// 의뢰 퀘스트 자동 진행 중 텔레포트

};

UENUM(BlueprintType)
enum class EGsEventProgressWorkType : uint8
{
	NOT_DO,					// 아무것도 안함
	DO_NOW,					// 바로 진행
	WAIT_PRE_EVENT,			// 이전 이벤트 종료까지 대기후 진행
};