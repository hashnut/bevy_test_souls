#pragma once

#include "Classes/GsMessageHandler.h"
#include "Classes/GsMessageHandlerOneParam.h"
// bak1210 
// 플로우 (스테이지가 변경)을 처리해야하는 메시지를 추가할때
UENUM()
enum class MessageStage : uint8
{
	//Lobby	
	ENTER_CHARACTERSELECT_STAGE,
	ENTER_CHARACTERCREATE_STAGE,
	ENTER_CHARACTER_CUSTOMIZING,
	ENTER_NETADDRESSSELECT_STAGE,	

	ENTER_OFFLINE_PLAY_BACKGROUND,		// 2021/08/25 PKT - 오프라인 플레이 백그라운드 모드		
	ENTER_PLAYABLE_EVENT,				// 플레이어블 이벤트 진입
	ENTER_CHARACTER_EMPTY_STATE,		// 캐릭터가 하나도 없을때 연출되는
	ENTER_PRE_PLAYABLE_MOVIE_STATE,		// 천마대전1 무비 플레이 스테이트
	ENTER_POST_PLAYABLE_MOVIE_STATE,	// 천마대전2 무비 플레이 스테이트

	ENTER_Z_PAY,						// Zpay 스테이트 진입
	LEAVE_Z_PAY,						// Zpay 스테이트 Out

	LOBBY_STATE_UPDATE_NOTIFY,			// 로비 상태 보고 
	COMPLETED_MODELAUNCHER,				// 런처 모드 로드가 끝났을 때
	COMPLETED_BACKTO_LAUNCHER,			// 런처 모드로 다시 돌아와 로드가 끝났을 때
	FNINSHED_INTRO_EFFECT,				// 인트로 연출이 끝났을 때
	SUCCEEDED_HIVE_DIVERGENCE,			// Divergence 로드가 완료되었을 때
	BACKTO_DIVERGENCE,					// Divergence로 다시 돌아왔을 때
	SUCCEEDED_HIVE_INIT,				// 하이브 init 됐을때
	RETRY_HIVE_INIT,					// 하이브 init 재시도
	RETRY_HIVE_AUTH_SINGNIN,			// 하이브 로그인 재시도
	SUCCEEDED_HIVE_AUTH,				// 하이브 로그인 완료 됐을때
	SUCCEEDED_CHECK_MAINTENANCE,		// 점검팝업 체크 완료 되었을때
	SUCCEEDED_CHECK_AUTH_MAINTENANCE,	// AUTH 로그인 전 점검팝업 체크 완료 되었을때
	SUCCEEDED_CHECK_GATEWAY_MAINTENANCE,// GATEWAY 로그인 전 점검팝업 체크 완료 되었을때
	FAILED_CHECK_MAINTENANCE,			// 점검팝업 체크 중 오류
	COMPLETED_NETCHECK_HANDSHAKE,		// 네트워크 점검 체크 완료
	FAILED_NETCHECK_HANDSHAKE,			// 네트워크 오류
	SELECTED_NETADDRESS,				// 서버를 선택 했을 때
	//SUCCEEDED_AUTH_SERVER_CONNECTION,	// 서버에 Connection 성공 시
	//TRY_LOGIN,							// 서버 로그인 요청
	SUCCEEDED_LOGIN,					// 서버 로그인 성공
	FAILED_LOGIN,						// 서버 로그인 실패
	COMPLETED_PATCH,					// 패치 성공
	FAILED_PATCH,						// 패치 실패
	FAILED_REPAIR_CHECK,				// 패치 복구 실패
	REQUEST_PATCH_START,				// 패치를 요청
	REQUEST_PATHC_REJECT,				// 패치를 거부 했을 때	
	REQUEST_PATCH_REPAIR,				// 패치 복구(검사) 요청
	CHECK_PATHC_COMPLETE,				// 패치 체크했는데 패치 받을게 없다.
	CHECK_PATHC_FAILED,					// 패치 체크했는데 여타의 이유로 실패가 났다.
	REQUEST_TITLE_CLICK_EVENT,			// 타이틀 화면에서 대기 하는 중에 클릭 요청이 왔을 때
	SUCCEEDED_HIVE_LOGOUT,				// 타이틀 화면에서 하이브 로그아웃을 하였을 때
	ERROR_OFFLINE_PLAY_BACKGROUND,		// ERROR - 오프라인 플레이 백그라운드 모드
	SUCCEEDED_HERCULES_AUTH,			// HERCULES 성공
	FAILED_HERCULES_AUTH,				// HERCULES 실패
	//SUCCEEDED_BANNEDWORD_PROCESS,		// 금칙어 파일 존재할 때 (다운로드 또는 업데이트 되지 않은 것)
	//FAILED_BANNEDWORD_PROCESS,			// 금칙어 파일 없음 (다운로드 실패)
	//CHANGE_ACCOUNT,						// 계정 변경	

	//Game
	ENTER_TOWN,			//필드내에 테리토리형식으로 체크해야함		

	ENTER_STAGE_CALLBACK, // 어떠한 스테이지이든 Enter 변경 시점
	LEAVE_STAGE_CALLBACK, // 어떠한 스테이지이든 LEAVE 변경 시점

	ENTER_TOWN_CALLBACK, //Enter town stage 호출 후 Callback
	LEAVE_TOWN_CALLBACK, //Leave town stage 호출 후 Callback	

	//chaos
	ENTER_CHAOS_CALLBACK, //Enter town stage 호출 후 Callback
	LEAVE_CHAOS_CALLBACK, //Leave town stage 호출 후 Callback	

	//non pk 
	ENTER_NON_PK_CALLBACK, //Enter non pk 호출 후 Callback
	LEAVE_NON_PK_CALLBACK, //Leave non pk 호출 후 Callback	

	//Safety
	ENTER_SAFETY,			//Territory 진입 후 Safety Stage로 변경 이벤트
	ENTER_SAFETY_CALLBACK, //Enter town stage 호출 후 Callback
	LEAVE_SAFETY_CALLBACK, //Leave town stage 호출 후 Callback	
	
	ENTER_FIELD,
	ENTER_DUNGEON,
	ENTER_GUILD_DUNGEON,
	ENTER_PVP_CONTENT,	

	ENTER_ARENA_READY_CALLBACK, // Enter Arena_Ready Stage 호출 후 Callback
	LEAVE_ARENA_READY_CALLBACK, // Leave Arena_Ready Stage 호출 후 Callback

	ENTER_ARENA_CALLBACK, // Enter Arena Stage 호출 후 Callback
	LEAVE_ARENA_CALLBACK, // Leave Arena Stage 호출 후 Callback

	ENTER_WORLD_BOSS,

	//Max
	STAGE_MAX,
	
};

using	MsgStageHandle		= TPair<MessageStage, FDelegateHandle>;
using	MsgStageHandleArray = TArray<MsgStageHandle>;

using	MStage				= TGsMessageHandler<MessageStage>;
using	MStageParam			= TGsMessageHandlerOneParam<MessageStage, const struct IGsMessageParam*>;