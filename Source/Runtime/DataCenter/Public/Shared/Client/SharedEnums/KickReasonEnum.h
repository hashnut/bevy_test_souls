#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// KickReasonEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
enum class KickReason : uint8
{
	UNKNOWN = 0,

	// 계정 정보가 셋업되지 않거나 유효하지 않음.
	INVALID_ACCOUNT = 1,

	// World 서버와의 접속이 끊어짐
	WORLD_SERVER_DISCONNECTED = 2,

	// 인증이 완료된 이후의 재접속 실패
	RECONNECT_FAILED = 3,

	// 중복 접속 시도
	REDUNDANT_LOGIN = 4,

	// 중복 인증 요청
	REDUNDANT_REQ_AUTH = 5,

	// 인증 타임 아웃
	AUTH_SLOWLORIS_CHECKED = 6,

	// 없는 캐릭터를 선택
	SELECT_NOT_EXIST_USER = 7,

	// 접속 대기열 중인데, 대기열 이후 보낼 수 있는 패킷을 보내옴
	INVALID_PACKET_STATE_FOR_LOBBY = 8,

	// World 서버에서 플레이중이지 않은 상태에서, 플레이중일 때만 보낼 수 있는 패킷을 보내옴
	INVALID_PACKET_STATE_FOR_WORLD_PLAYING = 9,

	// Shipping 빌드에서는 AdminCommand-QA 패킷을 보내면 아니댐
	ADMIN_COMMAND_QA_NOT_ALLOWED = 10,

	// 오프라인 모드 시작
	OFF_LINE_START = 11,

	// 명령어 때문에 다시 접속
	ADMIN_COMMAND_RESTART = 12,

	// 오프라인 모드 시간 종료
	OFF_LINE_TIME_FINISH = 13,

	// 서버 점검을 위한 유저 킥
	KICK_USER_FOR_SERVER_MAINTENANCE = 14,

	// GM툴 유저 킥
	GM_TOOL_KICK_USER = 15,

	// 대기열에 존재 하지 않는 유저
	NOT_EXIST_WAITING_QUEUE = 16,

	// GM툴 유저 킥(유저 차단)
	GM_TOOL_KICK_USER_TO_BLOCK = 17,

	// Bypass Ticket 이 안맞아서 발생함 
	BYPASS_TICKET_ERROR = 18,

	// 계정 삭제
	DELETE_ACCOUNT = 19,

	// PC 연결 해제 되어 접속 종료
	ZPAY_SESSION_KICK_WHEN_USER_DISCONNECTED = 20,

	// PC 연결 유저 Offline 시작
	ZPAY_SESSION_KICK_WHEN_OFFLINE_START = 21,

	// PC연결 유저 로비로 이동하여 접속종료 시작
	ZPAY_SESSION_KICK_WHEN_USER_LOBBY = 22,

	// 서버 이전으로 인한 데이터 이전 작업 시작
	SERVER_MIGRATE_START = 23,

	MAX = 24,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline KickReason KickReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return KickReason::UNKNOWN;
	if (str.Compare(TEXT("INVALID_ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return KickReason::INVALID_ACCOUNT;
	if (str.Compare(TEXT("WORLD_SERVER_DISCONNECTED"), ESearchCase::IgnoreCase) == 0)	return KickReason::WORLD_SERVER_DISCONNECTED;
	if (str.Compare(TEXT("RECONNECT_FAILED"), ESearchCase::IgnoreCase) == 0)	return KickReason::RECONNECT_FAILED;
	if (str.Compare(TEXT("REDUNDANT_LOGIN"), ESearchCase::IgnoreCase) == 0)	return KickReason::REDUNDANT_LOGIN;
	if (str.Compare(TEXT("REDUNDANT_REQ_AUTH"), ESearchCase::IgnoreCase) == 0)	return KickReason::REDUNDANT_REQ_AUTH;
	if (str.Compare(TEXT("AUTH_SLOWLORIS_CHECKED"), ESearchCase::IgnoreCase) == 0)	return KickReason::AUTH_SLOWLORIS_CHECKED;
	if (str.Compare(TEXT("SELECT_NOT_EXIST_USER"), ESearchCase::IgnoreCase) == 0)	return KickReason::SELECT_NOT_EXIST_USER;
	if (str.Compare(TEXT("INVALID_PACKET_STATE_FOR_LOBBY"), ESearchCase::IgnoreCase) == 0)	return KickReason::INVALID_PACKET_STATE_FOR_LOBBY;
	if (str.Compare(TEXT("INVALID_PACKET_STATE_FOR_WORLD_PLAYING"), ESearchCase::IgnoreCase) == 0)	return KickReason::INVALID_PACKET_STATE_FOR_WORLD_PLAYING;
	if (str.Compare(TEXT("ADMIN_COMMAND_QA_NOT_ALLOWED"), ESearchCase::IgnoreCase) == 0)	return KickReason::ADMIN_COMMAND_QA_NOT_ALLOWED;
	if (str.Compare(TEXT("OFF_LINE_START"), ESearchCase::IgnoreCase) == 0)	return KickReason::OFF_LINE_START;
	if (str.Compare(TEXT("ADMIN_COMMAND_RESTART"), ESearchCase::IgnoreCase) == 0)	return KickReason::ADMIN_COMMAND_RESTART;
	if (str.Compare(TEXT("OFF_LINE_TIME_FINISH"), ESearchCase::IgnoreCase) == 0)	return KickReason::OFF_LINE_TIME_FINISH;
	if (str.Compare(TEXT("KICK_USER_FOR_SERVER_MAINTENANCE"), ESearchCase::IgnoreCase) == 0)	return KickReason::KICK_USER_FOR_SERVER_MAINTENANCE;
	if (str.Compare(TEXT("GM_TOOL_KICK_USER"), ESearchCase::IgnoreCase) == 0)	return KickReason::GM_TOOL_KICK_USER;
	if (str.Compare(TEXT("NOT_EXIST_WAITING_QUEUE"), ESearchCase::IgnoreCase) == 0)	return KickReason::NOT_EXIST_WAITING_QUEUE;
	if (str.Compare(TEXT("GM_TOOL_KICK_USER_TO_BLOCK"), ESearchCase::IgnoreCase) == 0)	return KickReason::GM_TOOL_KICK_USER_TO_BLOCK;
	if (str.Compare(TEXT("BYPASS_TICKET_ERROR"), ESearchCase::IgnoreCase) == 0)	return KickReason::BYPASS_TICKET_ERROR;
	if (str.Compare(TEXT("DELETE_ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return KickReason::DELETE_ACCOUNT;
	if (str.Compare(TEXT("ZPAY_SESSION_KICK_WHEN_USER_DISCONNECTED"), ESearchCase::IgnoreCase) == 0)	return KickReason::ZPAY_SESSION_KICK_WHEN_USER_DISCONNECTED;
	if (str.Compare(TEXT("ZPAY_SESSION_KICK_WHEN_OFFLINE_START"), ESearchCase::IgnoreCase) == 0)	return KickReason::ZPAY_SESSION_KICK_WHEN_OFFLINE_START;
	if (str.Compare(TEXT("ZPAY_SESSION_KICK_WHEN_USER_LOBBY"), ESearchCase::IgnoreCase) == 0)	return KickReason::ZPAY_SESSION_KICK_WHEN_USER_LOBBY;
	if (str.Compare(TEXT("SERVER_MIGRATE_START"), ESearchCase::IgnoreCase) == 0)	return KickReason::SERVER_MIGRATE_START;
	return KickReason::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString KickReasonEnumToString(KickReason value)
{
	switch (value)
	{
		case KickReason::UNKNOWN: return TEXT("UNKNOWN");
		case KickReason::INVALID_ACCOUNT: return TEXT("INVALID_ACCOUNT");
		case KickReason::WORLD_SERVER_DISCONNECTED: return TEXT("WORLD_SERVER_DISCONNECTED");
		case KickReason::RECONNECT_FAILED: return TEXT("RECONNECT_FAILED");
		case KickReason::REDUNDANT_LOGIN: return TEXT("REDUNDANT_LOGIN");
		case KickReason::REDUNDANT_REQ_AUTH: return TEXT("REDUNDANT_REQ_AUTH");
		case KickReason::AUTH_SLOWLORIS_CHECKED: return TEXT("AUTH_SLOWLORIS_CHECKED");
		case KickReason::SELECT_NOT_EXIST_USER: return TEXT("SELECT_NOT_EXIST_USER");
		case KickReason::INVALID_PACKET_STATE_FOR_LOBBY: return TEXT("INVALID_PACKET_STATE_FOR_LOBBY");
		case KickReason::INVALID_PACKET_STATE_FOR_WORLD_PLAYING: return TEXT("INVALID_PACKET_STATE_FOR_WORLD_PLAYING");
		case KickReason::ADMIN_COMMAND_QA_NOT_ALLOWED: return TEXT("ADMIN_COMMAND_QA_NOT_ALLOWED");
		case KickReason::OFF_LINE_START: return TEXT("OFF_LINE_START");
		case KickReason::ADMIN_COMMAND_RESTART: return TEXT("ADMIN_COMMAND_RESTART");
		case KickReason::OFF_LINE_TIME_FINISH: return TEXT("OFF_LINE_TIME_FINISH");
		case KickReason::KICK_USER_FOR_SERVER_MAINTENANCE: return TEXT("KICK_USER_FOR_SERVER_MAINTENANCE");
		case KickReason::GM_TOOL_KICK_USER: return TEXT("GM_TOOL_KICK_USER");
		case KickReason::NOT_EXIST_WAITING_QUEUE: return TEXT("NOT_EXIST_WAITING_QUEUE");
		case KickReason::GM_TOOL_KICK_USER_TO_BLOCK: return TEXT("GM_TOOL_KICK_USER_TO_BLOCK");
		case KickReason::BYPASS_TICKET_ERROR: return TEXT("BYPASS_TICKET_ERROR");
		case KickReason::DELETE_ACCOUNT: return TEXT("DELETE_ACCOUNT");
		case KickReason::ZPAY_SESSION_KICK_WHEN_USER_DISCONNECTED: return TEXT("ZPAY_SESSION_KICK_WHEN_USER_DISCONNECTED");
		case KickReason::ZPAY_SESSION_KICK_WHEN_OFFLINE_START: return TEXT("ZPAY_SESSION_KICK_WHEN_OFFLINE_START");
		case KickReason::ZPAY_SESSION_KICK_WHEN_USER_LOBBY: return TEXT("ZPAY_SESSION_KICK_WHEN_USER_LOBBY");
		case KickReason::SERVER_MIGRATE_START: return TEXT("SERVER_MIGRATE_START");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString KickReasonEnumToDisplayKey(KickReason value)
{
	switch (value)
	{
		case KickReason::UNKNOWN: return TEXT("UNKNOWN");
		case KickReason::INVALID_ACCOUNT: return TEXT("INVALID_ACCOUNT");
		case KickReason::WORLD_SERVER_DISCONNECTED: return TEXT("WORLD_SERVER_DISCONNECTED");
		case KickReason::RECONNECT_FAILED: return TEXT("RECONNECT_FAILED");
		case KickReason::REDUNDANT_LOGIN: return TEXT("REDUNDANT_LOGIN");
		case KickReason::REDUNDANT_REQ_AUTH: return TEXT("REDUNDANT_REQ_AUTH");
		case KickReason::AUTH_SLOWLORIS_CHECKED: return TEXT("AUTH_SLOWLORIS_CHECKED");
		case KickReason::SELECT_NOT_EXIST_USER: return TEXT("SELECT_NOT_EXIST_USER");
		case KickReason::INVALID_PACKET_STATE_FOR_LOBBY: return TEXT("INVALID_PACKET_STATE_FOR_LOBBY");
		case KickReason::INVALID_PACKET_STATE_FOR_WORLD_PLAYING: return TEXT("INVALID_PACKET_STATE_FOR_WORLD_PLAYING");
		case KickReason::ADMIN_COMMAND_QA_NOT_ALLOWED: return TEXT("ADMIN_COMMAND_QA_NOT_ALLOWED");
		case KickReason::OFF_LINE_START: return TEXT("OFF_LINE_START");
		case KickReason::ADMIN_COMMAND_RESTART: return TEXT("ADMIN_COMMAND_RESTART");
		case KickReason::OFF_LINE_TIME_FINISH: return TEXT("OFF_LINE_TIME_FINISH");
		case KickReason::KICK_USER_FOR_SERVER_MAINTENANCE: return TEXT("KICK_USER_FOR_SERVER_MAINTENANCE");
		case KickReason::GM_TOOL_KICK_USER: return TEXT("GM_TOOL_KICK_USER");
		case KickReason::NOT_EXIST_WAITING_QUEUE: return TEXT("NOT_EXIST_WAITING_QUEUE");
		case KickReason::GM_TOOL_KICK_USER_TO_BLOCK: return TEXT("GM_TOOL_KICK_USER_TO_BLOCK");
		case KickReason::BYPASS_TICKET_ERROR: return TEXT("BYPASS_TICKET_ERROR");
		case KickReason::DELETE_ACCOUNT: return TEXT("DELETE_ACCOUNT");
		case KickReason::ZPAY_SESSION_KICK_WHEN_USER_DISCONNECTED: return TEXT("ZPAY_SESSION_KICK_WHEN_USER_DISCONNECTED");
		case KickReason::ZPAY_SESSION_KICK_WHEN_OFFLINE_START: return TEXT("ZPAY_SESSION_KICK_WHEN_OFFLINE_START");
		case KickReason::ZPAY_SESSION_KICK_WHEN_USER_LOBBY: return TEXT("ZPAY_SESSION_KICK_WHEN_USER_LOBBY");
		case KickReason::SERVER_MIGRATE_START: return TEXT("SERVER_MIGRATE_START");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidKickReason(KickReason value) noexcept
{
	return (value >= KickReason::UNKNOWN && value < KickReason::MAX);
}

