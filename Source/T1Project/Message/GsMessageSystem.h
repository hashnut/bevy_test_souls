#pragma once

#include "Classes/GsMessageHandler.h"
#include "Classes/GsMessageHandlerOneParam.h"
// bak1210 
// 각 매니져, 플로우, 컨텐츠 등에서 공용으로 처리해야 할 이벤트가 정의되어야 할때 
UENUM()
enum class MessageSystem : uint16
{
	SYSTEM_INIT,					//인스턴스 생성후 Holder 클래스 생성완료
	SYSTEM_SHUTDOWN,				//인스턴스 삭제
	//Net
	AUTH_SERVER_CONNECTION_START,	//인증서버 접속 시작
	AUTH_SERVER_CONNECTION_SUCCESS, //인증서버 접속
	AUTH_SERVER_CONNECTION_FAIL,	//인증서버 접속 실패
	AUTH_SERVER_DISCONNECTION,		//인증서버 접속 종료

	AUTH_SERVER_QUERY_IP_FAIL,		//인증서버 IP 쿼리 실패
	AUTH_SERVER_QUERY_IP_SUCCESS,	//인증서버 IP 쿼리 성공
	GAME_SERVER_QUERY_IP_FAIL,		//게임서버 IP 쿼리 실패
	GAME_SERVER_QUERY_IP_SUCCESS,	//게임서버 IP 쿼리 성공

	GAME_SERVER_CONNECTION_SUCCESS,	//게임서버 접속
	GAME_SERVER_CONNECTION_FAIL,	//게임서버 접속 실패
	GAME_SERVER_DISCONNECTION,		//게임서버 접속 종료
	GAME_SERVER_RECONNECTION_SUCCESS,//게임서버 재접속 성공

	GATEWAY_SERVER_LOGIN_RETRY,		//게이트웨이 서버 재접속 시도

	NET_DISCONECTED,				// 서버와의 접속 종료
	//Reconnect
	RECONNECT_START,				// 재연결 시작
	RECONNECT_END,					// 재연결 완료

	//Flow
	ENTER_INGAME,					// 인게임 진입
	AHUD_BEGIN_PLAY,				// F
	BEFORE_CHANGE_LEVEL,			// 레벨 로딩직전 
	GAMEMODE_WORLD_START_PLAY,		// 월드게임모드 START(게임내 맵전환시 호출)
	GAMEMODE_WORLD_EXIT,			// 월드게임모드 EXIT(게임월드에서 나갈때 호출)
	BEFORE_CLOSE_LOADING_TRAY,		// 로딩Tray 지워지기직전(이미 게임화면은 보임)
	BEFORE_GAME_TO_LOBBY,			// 인게임에서 캐릭터 선택창으로 넘어가기 전

	//로비로 이전 (Stage 매니져에서도 핸들링하는 메지지임으로 시스템레벨에 존재)
	BACKTO_LOGIN,					// 다시시작(로그인으로 - 로그인이 끊기고 돌아갈 경우)
	BACKTO_LOBBY,					// 다시시작(캐릭터 선택창으로 - 로그인이 유지된 상태에서 돌아갈 경우)
	BACKTO_TITLE,					// 타이틀 화면으로 이동
	RESTART_GAME,					// 인트로 화면으로 이동
	EXIT_GAME,						// 게임 종료(현재 인게임에선 로비로가기, 로비에선 타이틀로 가기로 활용되고 있다)
	QUIT_GAME,						// 게임 끄기(종료팝업, 안드로이드는 하이브 종료팝업, IOS는 정책상 종료기능 없음)

	//Message
	SHOW_DEVELOP_MESSAGE,			// 개발중 메시지 출력
	INIT_SMOKE_SIGNAL,				// 특정 개발 메세지 송신 초기화
	SEND_SMOKE_SIGNAL,				// 특정 개발 메세지 송신

	//Gvs
	GVS_NO_CLIENT_VERSION_INFO,		// 해당하는 버전 정보가 없는 상황
	GVS_SAME_VERSION,				// 같은 버전
	GVS_NEED_UPDATE,				// 업데이트 필요

	//
	AFTER_LOAD_TOPO_FIN,			// 서버로 LOAD_TOPO_FIN 패킷 보낸 후

	GAMEOBJECT_REMOVE_ALL,			// 게임오브젝트객체 전체삭제

	// World Time
	SYNC_WORLD_TIME,				// 월드 타임 사용 부분 시간 동기화 처리

	// Process LoadTopo
	LOAD_TOPO_START,				// LoadTopo 시작
	LOAD_TOPO_END_WARP_EFFECT,		// 워프 연출 종료
	LOAD_TOPO_START_SPAWN,			// 스폰 시작
	LOAD_TOPO_REQUEST_WARP,			// 워프 요청을 했는지 여부

	LEAVE_INSTANCE_SINGLE_MAP,		// 인던에서 떠날 때

	SEQUENCE_PLAYER_START,					// 시네마틱 연출 시작
	SEQUENCE_PLAYER_STOP,					// 시네마틱 연출 중지
	SEQUENCE_PLAYER_END,					// 시네마틱 연출 끝
	SEQUENCE_PLAYER_FORCE_STOP,				// 시네마틱 즉시 종료 테스트
	SEQUENCE_PLAYER_PAUSE,					// 시네마틱 일시 멈춤
	SEQUENCE_PLAYER_PLAY,					// 시네마틱 일시 멈춤 후 다시 플레이

	SEQUENCE_PLAYER_STOP_ONLY_PLAYER,		// 시네마틱 연출 중지(플레이어만: 메시지 send, 나머지 정리 빼고)
	SEQUENCE_PLAYER_ERROR_STOP,				// 플레이 에러상황 즉시 멈춤

	SUMMON_COMPOSE_SKIP,					// compose skip

	RESET_TIME_CONTENTS, // 기간제 정보 리셋 (일일, 주별, 월별, ...) FGsPrimitiveInt32 사용

	SERVER_TIME_START,				// 서버에서 TIME 시작 알림

	WARP_WITH_DESPAWN_READY,		// 맵 이동 및 채널 이동
	WARP_FAILUER,					// 이동 실패

	HIVE_CONNECT_SUCCESS,			// 계정 연동 성공
	HIVE_CONNECT_FAIL,				// 계정 연동 실패

	HIVE_DISCONNECT_SUCCESS,		// 계정 해제 성공
	HIVE_DISCONNECT_FAIL,			// 계정 해제 실패

	HIVE_CHECK_MAINTENANCE_TRUE,	// 하이브 서버점검 팝업 성공
	HIVE_CHECK_MAINTENANCE_FALSE,	// 하이브 서버점검 팝업 실패, 앱종료
	HIVE_CHECK_MAINTENANCE_RETRY,   // 하이브 서버점검 요청이 AuthV4NetworkError, AuthV4InvalidResponseData, AuthV4ServerResponseError 의 이유로 실패하면 한번 제시도(그다음은 종료팝업)
	HIVE_CHECK_MAINTENANCE_RETRY_FAILED, // 하이브 서버점검 요청 제시도도 실패시 종료팝업 출력 후 종료

	HIVE_DIVERGENCE_TRUE,			// Divergence 성공
	HIVE_DIVERGENCE_FALSE,			// Divergence 실패

	HIVE_INIT_TRUE,					// 하이브 INIT 성공
	HIVE_INIT_FALSE,				// 하이브 INIT 실패
	HIVE_AUTH_TRUE,					// 하이브 AUTH 성공
	HIVE_AUTH_FALSE,				// 하이브 AUTH 실패
	HIVE_AUTH_LOGOUT_TRUE,			// 하이브 로그아웃 성공
	HIVE_AUTH_LOGOUT_FALSE,			// 하이브 로그아웃 실패
	HIVE_Conflict_Helper,
	HIVE_Show_SignIn,
	HIVE_SignOut,					// 하이브 로그아웃 요청
	HIVE_IdpConnectCancle,			// 옵션창에서 idp 연동 취소
	PATCH_TRUE,						// 다운로드 성공
	PATCH_FALSE,					// 다운로드 실패	
	PATCH_Click,					// 다운로드 페이지 클릭
	REPAIR_TRUE,					// 패치 복구 성공
	REPAIR_FALSE,					// 패치 복구 실패
	HERCULES_AUTH_SUCCEEDED,		// HERCULES 성공
	HERCULES_AUTH_FAILED,			// HERCULES 실패
	REGISTER_DEVIECE_POPUP_SUCCEEDED,		// 기기등록 HIVE 팝업 성공
	REGISTER_DEVIECE_POPUP_FAILED,			// 기기등록  HIVE 팝업 실패
	REGISTER_DEVIECE_RESULT_RECEVIED_SERVER, // 기기등록 여부를 서버에서 받음
	HIVE_USER_PROFILE_UPDATE,				// 유저 프로필 업데이트
	HIVE_IDP_CONFLICT_PLAYER,				// 유저 IDP 연동 충돌 발생


	NET_AUTH_SERVER_CONNECTION_SUCCEEDED,	// 서버 선택 완료
	NET_LOGIN_SUCCEEDED,					// 서버 로그인 성공
	NET_LOGIN_FAILED,						// 서버 로그인 실패
	NET_PRE_LOGIN_SUCCEEDED,				// 서버 프리 로그인 성공
	NET_PRE_LOGIN_FAILED,					// 서버 프리 로그인 실패
	NET_CHECKVERSION_FAILED,				// 서버 버젼체크 실패

	NET_SERVER_SELECT_SUCCEEDED,			// 서버 선택 성공
	NET_SERVER_SELECT_FAILED,				// 서버 선택 실패

	NET_GATEWAY_LOGIN_SUCCEEDED,			// 서버 게이트웨이 로그인 성공
	NET_GATEWAY_LOGIN_FAILED,				// 서버 게이트웨이 로그인 실패

	HIVE_PUSH,						// 하이브 푸쉬 상태

	BANNEDWORD_PROCESS_SUCCESS,		// 금칙어 프로세스 성공적 완료
	BANNEDWORD_PROCESS_FAILURE,		// 금칙어 프로세스 에러

	WINDOW_RESOLUTION_UPDATE,		// 윈도우 해상도 변경
	WINDOW_SCREENMODE_UPDATE,		// 윈도우 스크린 모드 변경

	VIEWPORT_LOST_FOCUS,			// 포커스를 잃었을때 (서스펜스됐을때)
	VIEWPORT_RECEIVE_FOCUS,			// 포커스 됐을 때

	TOUCH_SCREEN,					// 화면 터치 알림
};
	
using	MsgSystemHandle			= TPair<MessageSystem, FDelegateHandle>;
using	MsgSystemHandleArray	= TArray<MsgSystemHandle>;

// 시스템 레벨
using	MSystem					= TGsMessageHandler<MessageSystem>;
using	MSystemParam			= TGsMessageHandlerOneParam<MessageSystem, const struct IGsMessageParam*>;
using	MSystemInt				= TGsMessageHandlerOneParam<MessageSystem, int32>;
