#pragma once

#include "Classes/GsMessageHandlerOneParam.h"
//-------------------------------------------------------------------------------------------------
// MessageContentDungeon
//-------------------------------------------------------------------------------------------------
UENUM()
enum class MessageContentDungeon : uint8
{
	/**
	 * UI
	 */
	INVALIDATE_DATA,				// 2022/06/09 PKT - 던전 그룹 내 화면만 갱신(서버에 따로 요청하거나 그러지 않음. 내부 데이터 변경으로 인한 화면 갱신이 필요 할때 호출)
	WINDOW_MAP_ID,					// 지정던전 아이디 셋팅
	CLOSE_WINDOW,					// 2022/06/14 PKT - 던전 Window 닫기
	PARTY_DUNGEON_PARTYLIST,		// 2022/05/30 PKT - 파티 던전 목록 리스트
	SHOTCUT_PARTY_DUNGEON,			// 2022/06/14 PKT - 파티 던전 바로 가기
	RESET_TIME,				// 2022/10/25 PKT - 던전 윈도우가 활성화 되어 있는 상태인가를 묻는다.

	/**
	 * Dungeon Play
	 */
	ENTER,							// 2022/07/27 PKT - 던전 입장(입장 후 : In Dungeon)
	EXIT,							// 2022/07/27 PKT - 던전 퇴장(퇴장 : Out Dungeon)
	UPDATE_TIME,					// 2022/07/27 PKT - 던전 내 시간값 업데이트
	USE_CHARGE_TIME,				// 2022/08/19 PKT - 아이템을 사용하여 던전 시간을 충전
		
	TRY_EXIT_DUNGEON,				// 2022/07/27 PKT - 던전 퇴장을 시도하다(퇴장 전 : In Dungeon)
	OPEN_RESULT_WINDOW,				// 2022/06/09 PKT - 던전 플레이 결과를 띄운다.(파티 던전 Only)
	PARTY_ADD_MEMBER,				// 2022/07/08 PKT - 파티 멤버 추가

	INTERACTION_PROP_PORTAL,		// 2022/08/16 PKT - Prop Portal Interaction

	START_PARTY_DUNGEON_COUNT,		// 2023/3/3 PKT - 파티 던전 시작 대기 카운트
	START_PARTY_DUNGEON_PLAY,		// 2023/3/3 PKT - 파티 던전이 시작 되었다.( 입장 말고.. 플레이 시작 )
	CLOSE_IMMINENT_PARTY_DUNGEON_PLAY,		// 2023/3/21 PKT - 파티 던전 종료가 임박했다.

	PARTY_DUNGEON_REQUEST_HELPSEEKING,	// 2023/3/23 PKT - 파티 던전 내 도움 요청 시도
	END_PARTY_DUNGEON_PLAY,				// 2023/3/3 PKT - 파티 던전 종료

	CONTENTS_DUNGEON_STAGE_ENTER,
	CONTENTS_DUNGEON_STAGE_EXIT,
};
// 공유 선언
using	MsgDungeonHandle		= TPair<MessageContentDungeon, FDelegateHandle>;
using	MsgDungeonHandleArray	= TArray<MsgDungeonHandle>;
using	MDungeon				= TGsMessageHandlerOneParam<MessageContentDungeon, const struct IGsMessageParam*>;

/*
* SendMessage Issue 때문에 Dungeon HUD Timer 관련 메세지 분리
*/
UENUM()
enum class MessageContentDungeonHUDTime : uint8
{
	HUD_TIME,						// 2022/07/27 PKT - HUD에 Time 세팅
	ON_HUD_TIME,					// 2022/07/27 PKT - HUD에 Time 켜기
	PAUSE_HUD_TIME,					// 2022/07/27 PKT - HUD에 Time 일시 정지
	OFF_HUD_TIME,					// 2022/07/27 PKT - HUD에 Time 끄기
};
using	MsgDungeonHUDTimeHandle = TPair<MessageContentDungeonHUDTime, FDelegateHandle>;
using	MsgDungeonHUDTimeHandleArray = TArray<MsgDungeonHUDTimeHandle>;
using	MDungeonHUDTime = TGsMessageHandlerOneParam<MessageContentDungeonHUDTime, const struct IGsMessageParam*>;


//-------------------------------------------------------------------------------------------------
// MessageContentGuildDungeon
//-------------------------------------------------------------------------------------------------
UENUM()
enum class MessageContentGuildDungeon : uint8
{
	ENTER,			// 길드 던전 입장
	EXIT,				// 길드 던전 퇴장	
	CLEAR,			// 길드 던전 성공
	OBSERVING,		// 길드 던전 옵저빙
	UPDATE_GUILD_DUNGEON_RANKING, // 기사단 던전 랭킹 갱신
	UPDATE_AGIT_RAID_RANKING, // 기사단 영지 던전 랭킹 갱신

};

// 공유 선언
using	MsgGuildDungeonHandle		= TPair <MessageContentGuildDungeon, FDelegateHandle>;
using	MsgGuildDungeonHandleArray	= TArray<MsgGuildDungeonHandle>;
using	MGuildDungeon				= TGsMessageHandlerOneParam<MessageContentGuildDungeon, const struct IGsMessageParam*>;



//-------------------------------------------------------------------------------------------------
// MessageContentSpaceCrack
//-------------------------------------------------------------------------------------------------
UENUM()
enum class MessageContentSpaceCrack : uint8
{
	WINDOW_INVALIDTEALL,	// 시공의틈새 UI 갱신
	//HUD_TIME,				// 시공의틈새 던전 유지 시간
	HUD_KILL_COUNT,			// 시공의틈새 몬스터 처리 마리수
	HUD_KILL_MAX_COUNT,		// 시공의틈새 몬스터 Max 처리 마리수
	ENTER,					// 시공의틈새 입장
	EXIT,					// 시공의틈새 퇴장
	STATE,					// 시공의틈새 상태
	EXIT_BUTTON_CLICK,		// 시공의틈새 HUD 나가기 버튼 클릭
};

// 공유 선언
using	MsgSpceCrackHandle		= TPair<MessageContentSpaceCrack, FDelegateHandle>;
using	MsgSpceCrackHandleArray	= TArray<MsgSpceCrackHandle>;
using	MSpaceCrack				= TGsMessageHandlerOneParam<MessageContentSpaceCrack, const struct IGsMessageParam*>;