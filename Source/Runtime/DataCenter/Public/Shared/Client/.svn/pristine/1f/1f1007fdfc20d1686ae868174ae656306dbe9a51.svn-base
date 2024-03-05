#pragma once

namespace PD {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// PD_ResultEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
enum Result
{

	// 성공
	RESULT_SUCCESS = 0,

	// 재시도 팝업
	LOGIN_ERROR_RETRY = 100,

	// 연결 끊고 로그인 화면으로 이동 팝업
	LOGIN_ERROR_RESTART = 101,

	// 로그인 실패 - 알 수 없는 에러
	LOGIN_ERROR_UNKNOWN = 102,

	// 로그인 실패 - 패킷 버전 불일치
	LOGIN_ERROR_WRONG_WZ_PACKET_VERSION = 103,

	// 로그인 실패 - 서버 에러
	LOGIN_ERROR_INTERNAL_SERVER_ERROR = 104,

	// 로그인 실패 - DB 에러
	LOGIN_ERROR_INTERNAL_DB_ERROR = 105,

	// 로그인 실패 - 이름 설정이 필요함
	LOGIN_ERROR_NEED_UPDATE_ACCOUNT_NAME = 106,

	// 로그인 실패 - 아이디, 패스워드 인증 실패
	LOGIN_ERROR_ID_PW_AUTH_FAILED = 107,

	// 로그인 실패 - 유효하지 않은 AUTHORIZATION
	LOGIN_ERROR_INVALID_AUTHORIZATION = 108,

	// 로그인 실패 - 게이트웨이 서버 시작 중 입니다.(계속된 메시지 발생시 관리자에게 문의바랍니다.)
	LOGIN_ERROR_GETTING_READY_FOR_GATEWAYSERVER = 109,

	// 로그인 실패 - 유효하지 않은 계정 ID
	LOGIN_ERROR_INVALID_ACCOUNT_ID = 110,

	// 로그인 실패 - 유효하지 않은 VID
	LOGIN_ERROR_INVALID_VID = 111,

	// 로그인 실패 - 유효하지 않은 토큰
	LOGIN_ERROR_INCORRECT_TOKEN = 112,

	// 로그인 실패 - 계정DB 읽어오기 실패
	LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB = 113,

	// 로그인 실패 - 제한 시간을 초과함
	LOGIN_ERROR_TIME_OUT = 114,

	// 로그인 실패 - 유효하지 않은 상태
	LOGIN_ERROR_INVALID_AUTH_STATUS = 115,

	// 로그인 실패 - 계정 만료 중. 잠시 후 다시 시도 하시오
	LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN = 116,

	// 로그인 실패 - 서버 점검 중
	LOGIN_ERROR_SERVER_MAINTENANCE = 117,

	// 로그인 실패 - 지정된 서버가 아닙니다.
	LOGIN_ERROR_NOT_MATCHED_VID_PLANETWORLD_ID = 118,

	// 로그인 실패 - Hive V4 인증 셋팅 필요
	LOGIN_ERROR_HIVE_VERIFY_TOKEN_SETTING = 119,

	// 로그인 실패 - 삭제된 계정입니다.
	LOGIN_ERROR_DELETED_ACCOUNT = 120,

	// 현재 입장이 불가능한 상태입니다. 잠시 후 다시 시도해주세요.
	SELECT_SERVER_ERROR_INACCESSIBLE = 121,

	// 서버가 매우 혼잡합니다. 잠시 후 다시 시도해주세요.
	SELECT_SERVER_ERROR_OVERLOAD = 122,

	// 서버를 찾을 수없다. 다른서버를 선택해 주시기 바랍니다.
	SELECT_SERVER_ERROR_NOT_FOUND = 123,

	// 로그인 실패 - Hive 기기 등록 조회 셋팅 필요
	LOGIN_ERROR_HIVE_REGISTER_DEVICE_SETTING = 124,

	// 로그인 실패 - 캐릭터 생성 제한 서버 생성된 캐릭터가 없어서 접속불가
	LOGIN_ERROR_NO_CHARACTER_CREATED = 125,

	// 하이브 - 유효하지 않은 파라미터(4000)
	LOGIN_ERROR_HIVE_INVALID_PARAMETER = 200,

	// 하이브 - request json 에러(4001)
	LOGIN_ERROR_HIVE_REQUEST_JSON = 201,

	// 하이브 - DB 에러(5000)
	LOGIN_ERROR_HIVE_DB_ERROR = 202,

	// 하이브 - 분산환경(AWS)에서 유효하지 않은 토큰(5001)
	LOGIN_ERROR_HIVE_INVALID_TOKEN_AWS = 203,

	// 하이브 - 유효하지 않는 appid(6000)
	LOGIN_ERROR_HIVE_INVALID_APPID = 204,

	// 하이브 - IDP 토큰 에러(6001)
	LOGIN_ERROR_HIVE_IDP_TOKEN_ERROR = 205,

	// 하이브 - 유효하지 않은 토큰(7000)
	LOGIN_ERROR_HIVE_INVALID_TOKEN = 206,

	// 하이브 - header에 토큰값 없음(7001)
	LOGIN_ERROR_HIVE_NO_TOKEN_HEADER = 207,

	// 하이브 - 알수없는 오류(9999)
	LOGIN_ERROR_HIVE_UNKNOWN_ERROR = 208,

	// 하이브 - 연결 불가(여러번 연결시도 실패 시)
	LOGIN_ERROR_HIVE_UNABLE_TO_CONNECT = 209,

	// 하이브 - 연결 오류
	LOGIN_ERROR_HIVE_CONNECT = 210,

	// 하이브 - 결과 파싱 오류
	LOGIN_ERROR_HIVE_PARSE = 211,

	// 하이브 - 결과 값을 파싱했지만 결과 코드가 없음
	LOGIN_ERROR_HIVE_NO_RESULT_CODE = 212,

	// 하이브 - 지정되지 않은 결과코드
	LOGIN_ERROR_HIVE_UNKNOWN_RESULT_CODE = 213,

	// 하이브 - 기기 등록 조회 파라미터가 없음
	LOGIN_ERROR_HIVE_NO_REGISTER_DEVICE_PARAMETER = 214,

	// 하이브 - 연결 오류(기기 등록 조회)
	LOGIN_ERROR_HIVE_CONNECT_REGISTER_DEVICE = 215,

	// 하이브 - 결과 파싱 오류(기기 등록 조회)
	LOGIN_ERROR_HIVE_PARSE_REGISTER_DEVICE = 216,

	// 채널 선택 - 알 수 없는 에러
	SELECT_CHANNEL_ERROR_UNKNOWN = 300,

	// 채널 선택 - 유효하지 않은 상태 (전투 중 or 탈 것)
	SELECT_CHANNEL_ERROR_INVALID_STATUS = 301,

	// 채널 선택 - 존재하지 않는 대륙
	SELECT_CHANNEL_ERROR_INVALID_AREA = 302,

	// 채널 선택 - 현재와 동일한 대륙/채널
	SELECT_CHANNEL_ERROR_SAME_AREA_CHANNEL = 303,

	// 채널 선택 - 채널이동이 불가능한 맵 타입
	SELECT_CHANNEL_ERROR_MAP_TYPE = 304,

	// 채널 선택 - 전투 중인 상태
	SELECT_CHANNEL_ERROR_IN_COMBAT = 305,

	// 채널 선택 - 이동용 탈것을 타고 있기 때문에 불가
	SELECT_CHANNEL_ERROR_IN_SHUTTLE = 306,

	// 채널 선택 - 해당 채널이 존재하지 않음
	SELECT_CHANNEL_ERROR_INVALID_CHANNEL = 307,

	// 채널 선택 - 해당 채널이 포화상태
	SELECT_CHANNEL_ERROR_CONGESTION_FULL = 308,

	// ZPAY 로그인 실패 - 인증 되지 않은 상태
	ZPAY_LOGIN_ERROR_INVALID_AUTH = 309,

	// ZPAY 로그인 실패 - 유효 하지 않은 상태의 서버로 입장 시도 했습니다
	ZPAY_LOGIN_ERROR_ENTER_SERVER = 310,

	// ZPAY 로그인 실패 - 플레이중인 유저가 없다
	ZPAY_LOGIN_ERROR_PLAY_USER = 311,

	// ZPAY 로그인 실패 - 아직 열리지 않은 컨텐츠입니다
	ZPAY_LOGIN_ERROR_UNLOCK_CONTENTS = 312,

	// 아이템 - 알 수 없는 에러
	ITEM_ERROR_UNKNOWN = 400,

	// 아이템 - Gateway 서버와 연결이 없습니다
	ITEM_ERROR_NO_GATEWAY_SERVER_CONNECTION = 401,

	// 아이템 - Gateway 서버 작업이 실패했습니다
	ITEM_ERROR_GATEWAY_OPERATION_FAILED = 402,

	// 아이템 - 유효하지 않은 캐릭터
	ITEM_ERROR_INVALID_USER = 403,

	// 아이템 - 유효하지 않은 캐릭터 상태
	ITEM_ERROR_INVALID_USER_STATUS = 404,

	// 아이템 - 유효하지 않은 레벨
	ITEM_ERROR_NOT_ALLOWED_LEVEL = 405,

	// 존재하지 않는 아이템
	ITEM_ERROR_ITEM_NOT_FOUND = 406,

	// 유효하지 않은 ItemId
	ITEM_ERROR_INVALID_ITEM_ID = 407,

	// 유효하지 않은 ItemDBId
	ITEM_ERROR_INVALID_ITEM_DB_ID = 408,

	// 아이템 추가 실패 - 인벤토리가 가득 참
	ITEM_ERROR_ADD_INVENTORY_FULL = 409,

	// 아이템 추가 실패 - 허용되지 않은 성별
	ITEM_ERROR_ADD_NOT_ALLOWED_GENDER = 410,

	// 아이템 추가 실패 - 유효하지 않은 스택카운트
	ITEM_ERROR_ADD_INVALID_STACK_COUNT = 411,

	// 아이템 추가 실패 - 무게 초과
	ITEM_ERROR_ADD_WEIGHT_FULL = 412,

	// 아이템 에러 - 재화 부족
	ITEM_ERROR_NOT_ENOUGH_MONEY = 413,

	// 아이템 에러 - 재화 가득 참
	ITEM_ERROR_OVERFLOW_MAX_MONEY = 414,

	// 잠겨있는 아이템은 삭제될 수 없음
	ITEM_ERROR_DELETE_LOCKED = 415,

	// 이미 잠겨있는 아이템
	ITEM_ERROR_LOCK_ALREADY_LOCKED = 416,

	// 이미 잠금 해제된 아이템
	ITEM_ERROR_LOCK_ALREADY_UNLOCKED = 417,

	// 잠글 수 없는 아이템
	ITEM_ERROR_LOCK_NOT_LOCKABLE = 418,

	// 아이템 개수 부족
	ITEM_ERROR_DECREASE_INSUFFICIENT = 419,

	// 착용 용도의 아이템이 아님
	ITEM_ERROR_EQUIP_NOT_EQUIP_ITEM = 420,

	// 이미 착용하고 있음
	ITEM_ERROR_EQUIP_ALREADY_EQUIPPED = 421,

	// 착용상태의 아이템이 아님
	ITEM_ERROR_EQUIP_ALREADY_UNEQUIPPED = 422,

	// 유효하지 않은 PresetId
	ITEM_ERROR_EQUIP_INVALID_PRESET_ID = 423,

	// 유효하지 않은 PresetId
	ITEM_ERROR_EQUIP_INVALID_PRESET_NAME = 424,

	// 무기 타입 변경 발생
	ITEM_ERROR_EQUIP_CHANGE_WEAPON_TPYE = 425,

	// 무기 아이템은 해제 불가
	ITEM_ERROR_EQUIP_CAN_NOT_UNEQUIP_WEAPON = 426,

	// 소켓팅 실패 - 유효하지 않은 아이템에 소켓팅 시도
	ITEM_ERROR_SOCKET_INVALID_TARGET = 427,

	// 소켓팅 하려는 대상에 이미 소켓팅이 되어 있음
	ITEM_ERROR_SOCKET_ALREADY_SOCKETED = 428,

	// 소켓팅 실패 - 소켓이 비어있지 않음
	ITEM_ERROR_SOCKET_IS_NOT_EMPTY = 429,

	// 아이템 사용 실패 - 착용 용도의 아이템 사용은 불가
	ITEM_ERROR_USE_NOT_USABLE_ITEM = 430,

	// 아이템 사용 실패 - 유효하지 않은 ItemEffectType
	ITEM_ERROR_USE_INVALID_EFFECT_TYPE = 431,

	// 아이템 사용 실패 - 유효하지 않은 ItemEffectParam
	ITEM_ERROR_USE_INVALID_EFFECT_PARAM = 432,

	// 아이템 사용 실패 - 아이템 개수가 충분하지 않음
	ITEM_ERROR_USE_NOT_ENOUGH_ITEMS = 433,

	// 아이템 사용 실패 - 재사용 대기시간이 지나지 않았음
	ITEM_ERROR_USE_IN_COOLDOWN = 434,

	// 포션 아이템 사용 실패 - HP나 MP가 가득참
	ITEM_ERROR_USE_POTION_HP_MP_FULL = 435,

	// 아이템 사용 실패 - 잠겨있는 아이템 사용 불가
	ITEM_ERROR_USE_LOCKED = 436,

	// 아이템 사용 실패 - 이미 습득한 스킬
	ITEM_ERROR_USE_ALREADY_LEARN_SKILL = 437,

	// 아이템 사용 실패 - 보너스 스탯 아이템 사용 최대 횟수
	ITEM_ERROR_USE_MAX_USED_BONUS_STAT_POINT_ITEM = 438,

	// 아이템 사용 실패 - PvpAlignmentPoint(카오틱) 수치가 이미 0이여서 사용 할 수 없습니다.
	ITEM_ERROR_USE_PVP_ALIGNMENT = 439,

	// 아이템 강화 실패 - 유효하지 않은 주문서
	ITEM_ERROR_ENCHANT_INVALID_SCROLL = 440,

	// 아이템 강화 실패 - 유효하지 않은 개수
	ITEM_ERROR_ENCHANT_INVALID_AMOUNT = 441,

	// 아이템 강화 실패 - 강화 확률에 의한 실패
	ITEM_ERROR_ENCHANT_RATE_FAILED = 442,

	// 아이템 강화 실패 - 안전강화 레벨 초과한 장착된 장비는 강화할 수 없음, M2 한정 기획
	ITEM_ERROR_ENCHANT_EQUIPED_ITEM_CANNOT_OVER_SAFE_LEVEL = 443,

	// 아이템 강화 실패 - 잠겨있는 아이템 강화 불가
	ITEM_ERROR_ENCHANT_LOCKED = 444,

	// 아이템 강화 실패 - 유효하지 않은 타겟
	ITEM_ERROR_ENCHANT_INVALID_TARGET = 445,

	// 아이템 강화 실패 - 유효하지 않은 레벨
	ITEM_ERROR_ENCHANT_INVALID_LEVEL = 446,

	// 아이템 분해 실패 - 유효하지 CATEGORY(DECOMPOSE_KIT)
	ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY = 447,

	// 아이템 분해 실패 - 장착된 장비는 분해 할 수 없습니다.
	ITEM_ERROR_DECOMPOSE_INVALID_EQUIPED = 448,

	// 아이템 분해 실패 - 유효하지 CATEGORY(일반 장비)
	ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY_EQUIP = 449,

	// 아이템 분해 실패 - 캐시구매 아이템은 분해 불가
	ITEM_ERROR_DECOMPOSE_PAID_ITEM = 450,

	// 아이템 분해 실패 - 분해결과 RewardId가 셋팅되어 있지 않음
	ITEM_ERROR_DECOMPOSE_INVALID_REWARD_ID = 451,

	// 아이템 판매 실패 - 판매할 수 없는 아이템
	ITEM_ERROR_NPCSHOP_UNABLE_TO_SELL = 452,

	// 아이템 추가 실패 - 창고가 가득 참
	ITEM_ERROR_ADD_STORE_FULL = 453,

	// 아이템 추가 실패 - 옮기려는 곳이 이가득 참
	ITEM_ERROR_ADD_STORAGE_FULL = 454,

	// 아이템 사용 실패 - 유효기간 만료
	ITEM_ERROR_EXPIRED_DATE_TIME = 455,

	// 아이템 사용 실패 - 운영툴에서 언락아이템으로 등록되어 임시 사용 불가
	ITEM_ERROR_UNLOCK = 456,

	// 아이템 다중 강화 실패 - 강화 대상 서로 다른 카테고리 등록
	ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_TARGET_CATEGORY = 457,

	// 아이템 다중 강화 실패 - 서로 다른 레벨 강화 시도
	ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_LEVEL = 458,

	// 아이템 다중 강화 실패 - 서로 다른 강화 주문서 등록
	ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_SCROLL = 459,

	// 아이템 - 안전 지대에서만 사용 가능
	ITEM_ERROR_USABLE_IN_SAFE_ZONE = 460,

	// 아이템 - 전투 상태에선 사용 불가
	ITEM_ERROR_COMBAT_STATUS = 461,

	// 아이템 - 삭제목록이 비어있습니다.
	ITEM_ERROR_DELETE_LIST_EMPTY = 462,

	// 아이템 - 생성목록이 비어있습니다.
	ITEM_ERROR_CREATE_LIST_EMPTY = 463,

	// 아이템 - 이미 적용중인 이상 상태입니다.
	ITEM_ERROR_ABNORMALITY_STAT_ALREADY_IN_EFFECT = 464,

	// 아이템 - 서로 다른 아이템입니다.
	ITEM_ERROR_FUSE_DIFFERENT_ITEMS = 465,

	// 아이템 - 서로 같은 참끼리는 합성이 불가능 합니다.
	ITEM_ERROR_FUSE_CHARM_SAME_TEMPLATE_ID = 466,

	// 아이템 - 서로 다른 그룹의 아이템입니다.
	ITEM_ERROR_FUSE_DIFFERENT_EQUIP_GROUP_ID = 467,

	// 아이템 - 장착 아이템이 아닙니다.
	ITEM_ERROR_FUSE_NOT_EQUIP_ITEM = 468,

	// 아이템 - 올바른 합성 아이템이 아닙니다.
	ITEM_ERROR_FUSE_INVALID_FUSE_ITEM = 469,

	// 아이템 - 최소 합성 레벨보다 낮습니다
	ITEM_ERROR_FUSE_LESS_THAN_MIN_LEVEL = 470,

	// 아이템 - 서로 다른 강화도입니다.
	ITEM_ERROR_FUSE_NOT_SAME_ENCHANT_LEVEL = 471,

	// 아이템 - 재련 정보가 존재하지 않습니다.
	ITEM_ERROR_FUSE_REFINE_DATA_NOT_EXISTS = 472,

	// 아이템 - 재련 정보를 선택해주세요.
	ITEM_ERROR_FUSE_SELECT_REFINE_DATA = 473,

	// 아이템 - 유저가 사망 상태 입니다.
	ITEM_ERROR_INVALID_USER_DEATH = 474,

	// 아이템 마법부여 - 마법부여를 진행 할 수 없습니다.(공통)
	ITEM_ERROR_INVALID_MAGICAL_FORGE = 475,

	// 아이템 마법부여 - 마법부여 효과의 재료아이템이 아닙니다.
	ITEM_ERROR_INVALID_MAGICAL_FORGE_MATIRAL_ITEM_EFFECT_TYPE = 476,

	// 아이템 마법부여 - 마법부여 효과의 타겟 아이템이 아닙니다.
	ITEM_ERROR_MAGICAL_FORGE_INVALID_TARGET = 477,

	// 아이템 마법부여 - 마법부여 효과의 재료아이템이 아닙니다.
	ITEM_ERROR_MAGICAL_FORGE_INVALID_MATARIAL = 478,

	// 아이템 마법부여 - 마법부여 가 최대 레벨이라 더이상 진행이 불가능합니다.
	ITEM_ERROR_MAGICAL_FORGE_MAX_LEVEL = 479,

	// 아이템 마법부여 - 잠겨있는 아이템 마법부여 불가
	ITEM_ERROR_MAGICAL_FORGE_LOCKED = 480,

	// 아이템 마법부여 - 낮은 ItemGrade 아이템 마법부여 불가
	ITEM_ERROR_MAGICAL_FORGE_INVALID_GRADE = 481,

	// 아이템 - 사용할 수 없는 맵
	ITEM_ERROR_NOT_ALLOWED_MAP = 482,

	// 보상 - 알 수 없는 에러
	REWARD_ERROR_UNKNOWN = 500,

	// 보상 - Gateway 서버와 연결이 없습니다
	REWARD_ERROR_NO_GATEWAY_SERVER_CONNECTION = 501,

	// 보상 - Gateway 서버 작업이 실패했습니다
	REWARD_ERROR_GATEWAY_OPERATION_FAILED = 502,

	// 보상 - 유효하지 않은 보상
	REWARD_ERROR_INVALID_REWARD = 503,

	// 보상 - 유효하지 않은 보상타입
	REWARD_ERROR_INVALID_REWARD_TYPE = 504,

	// 보상 - 재화 보유량 초과
	REWARD_ERROR_CURRENCY_EXCEEDED = 505,

	// 탈 것 - 알 수 없는 에러
	VEHICLE_ERROR_UNKNOWN = 600,

	// 탈 것 - 유효하지 않은 ID
	VEHICLE_ERROR_INVALID_ID = 601,

	// 탑승 실패 - 이미 탑승 중
	VEHICLE_ERROR_ALREADY_MOUNTED = 602,

	// 탑승 실패 - 변신 중이면 탑승 불가
	VEHICLE_ERROR_IN_TRANSFORM = 603,

	// 탑승 실패 - 금지된 대륙
	VEHICLE_ERROR_FORBIDDEN_AREA = 604,

	// 탑승 실패 - 탑승 행동 불가
	VEHICLE_ERROR_DISABLE_BEHAVIOR = 605,

	// 탑승 실패 - 전투 상태에선 탑승 불가
	VEHICLE_ERROR_COMBAT_STATUS = 606,

	// 탑승 실패 - 이동 시간이 초과되지 않음
	VEHICLE_ERROR_NOT_YET_MOVE_TIME = 607,

	// 우편 - 알수 없는 에러
	MAIL_ERROR_UNKNOWN = 700,

	// 우편 - Gateway 서버와 연결이 없습니다
	MAIL_ERROR_NO_GATEWAY_SERVER_CONNECTION = 701,

	// 우편 - Gateway 서버 작업이 실패했습니다
	MAIL_ERROR_GATEWAY_OPERATION_FAILED = 702,

	// 우편 - 재화 보유량 초과
	MAIL_ERROR_CURRENCY_EXCEEDED = 703,

	// 우편 - 유효하지 않은 우편함
	MAIL_ERROR_INVALID_MAIL_BOX_TYPE = 704,

	// 우편 - 유효하지 않은 우편 카테고리
	MAIL_ERROR_INVALID_CATEGORY_TYPE = 705,

	// 우편 - 유효하지 않은 MailDBId
	MAIL_ERROR_INVALID_MAIL_DB_ID = 706,

	// 우편 받기 실패 - 만료된 우편
	MAIL_ERROR_RECV_ALREADY_EXPIRED = 707,

	// 우편 받기 실패 - 인벤토리가 가득 참
	MAIL_ERROR_RECV_INVENTORY_FULL = 708,

	// 우편 읽기 실패 - 이미 읽은 우편
	MAIL_ERROR_ALREADY_READ_MAIL = 709,

	// 우편 - 유효하지 않은 재화 타입
	MAIL_ERROR_INVALID_CURRENCY_TYPE = 710,

	// 우편 - 유효하지 않은 우편 문구 아이디
	MAIL_ERROR_INVALID_MAIL_STRING_ID = 711,

	// 우편 삭제 실패 - 삭제 요청 우편 리스트가 비어있음
	MAIL_ERROR_EMPTY_DELETED_MAIL_DB_ID_LIST = 712,

	// 우편 - 우편 리스트 삭제 디비 요청 실패
	MAIL_ERROR_DB_OPERATION_MAIL_LIST_DELETE = 713,

	// 우편 읽기 실패 - 이미 다른 서버에서 우편 지급 됨
	MAIL_ERROR_ALREADY_OHTER_SERVER_RECV = 714,

	// 우편 - 우편 타입 아이디 오류
	MAIL_ERROR_INVALID_MAIL_TYPE_ID = 715,

	// 스킬 - 알 수 없는 에러
	SKILL_ERROR_UNKNOWN = 800,

	// 스킬 - 유효하지 않은 위치
	SKILL_ERROR_INVALID_POS = 801,

	// 스킬 - 유효하지 않은 정보
	SKILL_ERROR_INVALID_INFO = 802,

	// 스킬 실패 - 재사용 대기시간이 지나지 않았음
	SKILL_ERROR_IN_COOL_DOWN = 803,

	// 스킬 실패 - 유효하지 않은 상태
	SKILL_ERROR_INVALID_STATE = 804,

	// 스킬 실패 - 스킬이 활성화되지 않았음
	SKILL_ERROR_DISABLED_SKILL = 805,

	// 스킬 실패 - 다른 스킬이 진행 중
	SKILL_ERROR_OTHER_SKILL_IN_PROGRESS = 806,

	// 스킬 실패 - 패시브 스킬은 사용할 수 없음
	SKILL_ERROR_CANNOT_USE_PASSIVE = 807,

	// 스킬 실패 - 이전 스킬 정보가 잘못 됨
	SKILL_ERROR_INVALID_PREV_SKILL_INFO = 808,

	// 스킬 실패 - 유효하지 않은 콤보 조건
	SKILL_ERROR_INVALID_CHAIN_CONDITION = 809,

	// 스킬 실패 - 요구비율 보다 작은 HP
	SKILL_ERROR_HP_LESS = 810,

	// 스킬 실패 - 요구비율 보다 큰 HP
	SKILL_ERROR_HP_GREATER = 811,

	// 스킬 실패 - 요구비율 보다 작은 MP
	SKILL_ERROR_MP_LESS = 812,

	// 스킬 실패 - 요구비율 보다 큰 MP
	SKILL_ERROR_MP_GREATER = 813,

	// 스킬 실패 - 유저가 아님
	SKILL_ERROR_ONLY_USER = 814,

	// 스킬 실패 - SP가 부족함
	SKILL_ERROR_SP_LESS = 815,

	// 스킬 실패 - 허용되지 않는 클래스
	SKILL_ERROR_NOT_ALLOWED_CLASS = 816,

	// 스킬 실패 - 허용되지 않는 대상
	SKILL_ERROR_NOT_ALLOWED_TARGET = 817,

	// 스킬 실패 - 아이템 불충분
	SKILL_ERROR_NO_REQUIRED_ITEM = 818,

	// 스킬 실패 - 대상이 유효거리에 있지 않음
	SKILL_ERROR_TARGET_NOT_IN_RANGE = 819,

	// 스킬 실패 - 목표 지점이 유효 범위를 벗어났음
	SKILL_ERROR_CURSOR_OUT_OF_RANGE = 820,

	// 스킬 실패 - 유효하지 않은 Skill Id
	SKILL_ERROR_INVALID_SKILL_ID = 821,

	// 스킬 실패 - 최대 스킬 레벨 달성
	SKILL_ERROR_ENCHANT_MAX_SKILL_LEVEL = 822,

	// 스킬 실패 - 유효하지 않은 스킬 비용 데이터
	SKILL_ERROR_INVALID_COST_DATA = 823,

	// 스킬 실패 - 체인스킬 조건에 맞지 않습니다(이전스킬 적중)
	SKILL_ERROR_INVALID_CHAIN_SKILL_HIT_CONDITION = 824,

	// 스킬 실패 - 스킬 배울 때, ROOT SKILL이 없다.
	SKILL_ERROR_LEARN_NOT_EXIST_ROOT_SKILL = 825,

	// 스킬 실패 - 스킬 배울 때, 요구 Passivity가 없다.
	SKILL_ERROR_LEARN_INVALID_ESSENTIAL_PASSIVITY = 826,

	// 스킬 실패 - 이미 배운 스킬
	SKILL_ERROR_LEARN_ALREADY_LEARN_SKILL = 827,

	// 스킬 실패 - 스킬 발동 조건 미달
	SKILL_ERROR_NOT_MATCH_TRIGGER_CONDITION = 828,

	// 스킬 실패 - 안전 지역에서는 공격스킬을 사용할 수 없습니다.
	SKILL_ERROR_NOT_USABLE_IN_SAFE_ZONE = 829,

	// 스킬 실패 - 직선 거리로 도달할 수 없는 곳에 사용하려고 했음
	SKILL_ERROR_UNREACHABLE_STRAIGHT = 830,

	// 스킬 - 컬리젼(셀블록) 체크 오류, 다른 유저와 겹쳐질 경우 등
	SKILL_ERROR_CHECK_COLLISION_FOR_SKILL = 831,

	// 이상상태 - 알 수 없는 에러
	ABNORMALITY_ERROR_UNKNOWN = 900,

	// 이상상태 - 우선순위에 의해 무시됨
	ABNORMALITY_ERROR_IGNORE_BY_PRIORITY = 901,

	// 이상상태 - 최대 이상상태 스택 카운트를 초과함
	ABNORMALITY_ERROR_EXCEED_STACK_COUNT = 902,

	// 부활 실패 - 스폰된 상태가 아님
	RESURRECT_RESULT_FAILED_NOT_SPAWNED = 1000,

	// 부활 실패 - 죽은 상태가 아님
	RESURRECT_RESULT_FAILED_ALIVE = 1001,

	// 부활 실패 - 부활에 필요한 최소 시간이 지나지 않았음
	RESURRECT_RESULT_FAILED_NOT_PAST_MIN_TICK = 1002,

	// QUEST - Gateway 서버 작업이 실패했습니다
	QUEST_ERROR_GATEWAY_OPERATION_FAILED = 1100,

	// QUEST - 유효하지 않은 상태
	QUEST_ERROR_INVALID_STATUS = 1101,

	// QUEST - 스폰되지 않았거나, 죽어있는 상태
	QUEST_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE = 1102,

	// QUEST - DataSheet 의 정보를 못찾았을 때
	QUEST_ERROR_INVALID_DATA_SHEET = 1103,

	// QUEST - 수락하려는 ChapterID가 유효하지 않습니다.
	QUEST_ERROR_INVALID_ACCEPT_CHPATER_ID = 1104,

	// QUEST - 수락하려는 StoryID가 유효하지 않습니다.
	QUEST_ERROR_INVALID_ACCEPT_STORY_ID = 1105,

	// QUEST - 수락하려는 QuestID가 유효하지 않습니다.
	QUEST_ERROR_INVALID_ACCEPT_QUEST_ID = 1106,

	// QUEST - 수락하려는 QuestType이 유효하지 않습니다.
	QUEST_ERROR_INVALID_ACCEPT_QUEST_TYPE = 1107,

	// QUEST - 첫번째 퀘스트가 아닙니다
	QUEST_ERROR_INVALID_ACCEPT_NOT_FIRST_QUEST = 1108,

	// QUEST - 현재 퀘스트가 스토리의 마지막퀘스트가 아닌데, 다음 스토리를 진행하려고 한다.
	QUEST_ERROR_INVALID_ACCEPT_NOT_LAST_QUEST = 1109,

	// QUEST - 수락하려는 QuestID가 이미 수락되어있습니다.
	QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID = 1110,

	// QUEST - 현재 QuestID가 스토리의 마지막퀘스트인데, 보상 받지 않은 상태입니다.
	QUEST_ERROR_INVALID_ACCEPT_NOT_REWARED = 1111,

	// QUEST - 수락하려는 QuestID가 다음퀘스트의 Story가 아닙니다.
	QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_STORY = 1112,

	// QUEST - 현재 퀘스트가 QuestID가 완료되어있지 않습니다
	QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED = 1113,

	// QUEST - 수락하려는 QuestID가 현재의 다음Quest가 아닙니다.
	QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_QUEST = 1114,

	// QUEST - 수락하기전에 수행되어야 할 PRE EVENT ACTION 이 모두 수행되지 않았습니다.
	QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_PRE_EVENT_ACTION = 1115,

	// QUEST - 수락하기전에 수행되어야 할 POST EVENT ACTION 이 모두 수행되지 않았습니다.
	QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_POST_EVENT_ACTION = 1116,

	// QUEST - 수락하려는 QUEST 최대갯수 초과
	QUEST_ERROR_INVALID_ACCEPT_QUEST_ACTIVE_COUNT_MAX = 1117,

	// QUEST - 수락하려는 QUEST REPEAT의 랜덤 PICK 실패
	QUEST_ERROR_INVALID_ACCEPT_QUEST_REPEAT_RANDOM_PICK = 1118,

	// QUEST - Quest를 통해 Warp 할수 없는 상태입니다.
	QUEST_ERROR_INVALID_UPDATE_WARP_STATUS = 1119,

	// QUEST - Quest를 진행 할수 없는 상태입니다.
	QUEST_ERROR_INVALID_UPDATE_STATUS = 1120,

	// QUEST - 진행 중인 Quest가 아닙니다.
	QUEST_ERROR_INVALID_UPDATE_QUEST_ID = 1121,

	// QUEST - 진행하려는 Quest의 유효한 Objective가 아닙니다.
	QUEST_ERROR_INVALID_UPDATE_OBJECTIVE = 1122,

	// QUEST - 진행하려는 Quest가 수락된 상태가 아닙니다.
	QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED = 1123,

	// QUEST - 진행하려는 Teleport Quest가 수락된 상태가 아닙니다.
	QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_TELEPORT = 1124,

	// QUEST - 진행하려는 WARP_TO_INSTANCE_FIELD Quest가 수락된 상태가 아닙니다.
	QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_INSTANCE_FIELD = 1125,

	// QUEST - 진행하려는 WARP_TO_LAST_PUBLIC_FIELD Quest가 수락된 상태가 아닙니다.
	QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_LAST_PUBLIC_FIELD = 1126,

	// QUEST - 진행하려는 Quest의 유효한 거리가 아닙니다.(GOTO_POS)
	QUEST_ERROR_INVALID_UPDATE_RANGE = 1127,

	// QUEST - 진행하려는 인스턴스던전 정보가 없습니다.
	QUEST_ERROR_INVALID_UPDATE_WARP_TO_INSTANCE_FIELD = 1128,

	// QUEST - 진행하려는 공용맵 정보가 없습니다.
	QUEST_ERROR_INVALID_UPDATE_WARP_TO_PUBLIC_FIELD = 1129,

	// QUEST - 보상받으려는 QuestID가 유효하지 않습니다.
	QUEST_ERROR_INVALID_REWARD_QUEST_ID = 1130,

	// QUEST - 보상받기전에, PostEventAction을 수행해야 합니다.
	QUEST_ERROR_INVALID_REWARD_NOT_PROCESS_POST_EVENT_ACTION = 1131,

	// QUEST - 보상받으려는 QuestID가 스토리내의 마지막퀘스트가 아닙니다
	QUEST_ERROR_INVALID_REWARD_NOT_LAST_QUEST_ID = 1132,

	// QUEST - 보상받으려는 QuestID가 이미 보상을 받았습니다
	QUEST_ERROR_ALREADY_REWARD_QUEST_ID = 1133,

	// QUEST - 현재 메인Quest 완료 되지 않았습니다.
	QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED = 1134,

	// QUEST - 보상받기 전에 수행되어야할 PRE EVENT ACTION 이 모두 수행되지 않았습니다.
	QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_PRE_EVENT_ACTION = 1135,

	// QUEST - 보상받기 전에 수행되어야할 POST EVENT ACTION 이 모두 수행되지 않았습니다.
	QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_POST_EVENT_ACTION = 1136,

	// QUEST - 현재 Quest의 보상을 받지 않았습니다.
	QUEST_ERROR_NOT_REWARDED = 1137,

	// QUEST - 수락하려는 퀘스트의 요구레벨에 도달하지 못했습니다.
	QUEST_ERROR_NOT_ENOUGH_LEVEL = 1138,

	// QUEST - 수락하려는 퀘스트의 요구 메인스토리까지 도달하지 못했습니다.
	QUEST_ERROR_NOT_ENOUGH_MAIN_STORY_ID = 1139,

	// QUEST - 워프 하려는 QuestID가 유효하지 않습니다.
	QUEST_ERROR_INVALID_WARP_QUEST_ID = 1140,

	// QUEST - 워프 하려는 Quest가 텔레포트가 불가능합니다
	QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE = 1141,

	// QUEST - 워프 하려는 Quest가 수락중이 아니여서 텔레포트가 불가능합니다
	QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE_NOT_ACCEPTED = 1142,

	// QUEST - 워프비용이 부족합니다.
	QUEST_ERROR_NOT_ENOUGH_MONEY = 1143,

	// QUEST - 포기할수 있는 상태가 아니다(진행중인 상태가 아니다)
	QUEST_ERROR_GIVE_UP_STATE = 1144,

	// QUEST - 보상받으려는 QuestType이 유효하지 않습니다.
	QUEST_ERROR_INVALID_REWARD_QUEST_TYPE = 1145,

	// QUEST - Gateway 서버와 연결이 없습니다
	QUEST_ERROR_GATEWAY_SERVER_CONNECTION = 1146,

	// QUEST - 이미 완료 된 퀘스트 입니다.
	QUEST_ERROR_ALREADY_COMPLETE_QUEST_ID = 1147,

	// QUEST - 진행중인 퀘스트가 아닙니다.
	QUEST_ERROR_NOT_RPOGRESS_QUEST_ID = 1148,

	// QUEST - 퀘스트 아이디에 문제가 있습니다.
	QUEST_ERROR_QUEST_ID = 1149,

	// QUEST - 유저 정보가 유효하지 않습니다.
	QUEST_ERROR_INVALID_USER_DATA = 1150,

	// QUEST - 하나의 기사단 퀘스트에서 개별 퀘스를 중복으로 수행 할 수 없습니다.
	QUEST_ERROR_NOT_ACCEPTABLE_GUILD_QUEST = 1151,

	// QUEST - 퀘스트 게시판 새로고침 재화가 부족합니다.
	QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_MONEY = 1152,

	// QUEST - 퀘스트 게시판 새로고침 가능 회수가 없습니다.
	QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_REFRESH_COUNT = 1153,

	// QUEST - 퀘스트 게시판 NPC가 아닙니다.
	QUEST_ERROR_QUEST_BOARD_NOT_QUEST_BOARD_NPC = 1154,

	// QUEST - 퀘스트 게시판 수락가능 회수가 없습니다.
	QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_ACCEPT_COUNT = 1155,

	// QUEST - 존재하지 않는 NPC입니다.
	QUEST_ERROR_QUEST_BOARD_INVALID_NPC = 1156,

	// QUEST - 존재하지 않는 퀘스트 게시판 ID입니다.
	QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_ID = 1157,

	// QUEST - 퀘스트 게시판 데이터가 존재하지 않습니다.
	QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_DATA = 1158,

	// QUEST - 잘못된 퀘스트 게시판 타입입니다.
	QUEST_ERROR_QUEST_BOARD_INVALID_TYPE = 1159,

	// QUEST - 잘못된 퀘스트 인덱스입니다.
	QUEST_ERROR_INVALID_QUEST_INDEX = 1160,

	// QUEST - 스크롤 퀘스트가 아닙니다.
	QUEST_ERROR_NOT_SCROLL_QUEST = 1161,

	// QUEST - 새로고침 가능 회수가 없습니다.
	QUEST_ERROR_NOT_REMAIN_REFRESH = 1162,

	// QUEST - 월드서버 내의 있는 맵이 아닙니다. Map이 아닙니다.
	QUEST_ERROR_INVALID_WARP_NOT_MAP_WORLD = 1163,

	// QUEST - 수락하려는 QuestID가 이미 수락되어있습니다. - 팝업이 아닌 티커처리.
	QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID_NOT_POP_UP = 1164,

	// Warp - 유효하지 않은 상태
	WARP_ERROR_INVALID_STATUS = 1200,

	// Warp - 유효하지 않은 볼륨
	WARP_ERROR_INVALID_VOLUME = 1201,

	// Warp - 볼륨 안에 있지 않습니다.
	WARP_ERROR_NOT_IN_VOLUME = 1202,

	// Warp - 인스턴스 싱글에서 나가기 버튼이 사용 가능합니다.
	WARP_ERROR_LEAVE_INSTANCE_SINGLE = 1203,

	// Warp - 전투중에는 이동할 수 없습니다.
	WARP_ERROR_IN_COMBAT = 1204,

	// Warp - SPOT데이타가 잘못었습니다.
	WARP_ERROR_INVALID_SPOT_INFO = 1205,

	// Warp - MAP데이타가 잘못 되었습니다.
	WARP_ERROR_INVALID_MAP = 1206,

	// Warp - 워프 할수 있는 상태가 아닌다(스폰X)
	WARP_ERROR_NOT_SPAWNED = 1207,

	// Warp - 워프할수 있는 상태가아니다(죽음?).
	WARP_ERROR_NOT_ALIVE = 1208,

	// Warp - 워프 할수 있는 상태가 아니다
	WARP_ERROR_CAN_NOT_DO_ACTION_WARP = 1209,

	// Warp - 던전 시간이 부족하다.
	WARP_ERROR_NOT_ENOUGH_DUNGEON_TIME = 1210,

	// Warp - 던전 오픈시간이 아니다.
	WARP_ERROR_NOT_OPEN_DUNGEON_TIME = 1211,

	// Warp - 던전 이용 금액이 부족하다.
	WARP_ERROR_NOT_ENOUGH_DUNGEON_COST = 1212,

	// Warp - MAP데이타가 잘못 되었습니다.(NEED_INSTANCE_SINGLE_MAP)
	WARP_ERROR_NEED_INSTANCE_SINGLE_MAP = 1213,

	// Warp - MAP데이타가 잘못 되었습니다.(TOPO)
	WARP_ERROR_TOPO_MAP_NOT_EXIST = 1214,

	// Warp - MAP데이타가 잘못 되었습니다.
	WARP_ERROR_RANDOM_POS_FAILED = 1215,

	// Warp - MAP데이타가 잘못 되었습니다.
	WARP_ERROR_INVALID_MAP_ID = 1216,

	// Warp - 잘못 된 채널 입니다.
	WARP_ERROR_INVALID_CHANNEL_ID = 1217,

	// Warp - 채널이 꽉찼다.
	WARP_ERROR_CHANNEL_CONGESTION_FULL = 1218,

	// Warp - 퀘스트 워프 불가능함.
	WARP_ERROR_INVALID_MAIN_QUEST = 1219,

	// Warp - 워프 가능 레벨이 아닙니다..
	WARP_ERROR_INVALID_LEVEL = 1220,

	// Warp - 클라이언트 준비가 덜되었다(WAIT_ACK_DESPAWN_READYING)
	WARP_ERROR_WAIT_ACK_DESPAWN_READYING = 1221,

	// Warp - 침공 오픈시간이 아닙니다. 
	WARP_ERROR_NOT_OPEN_INVADE_TIME = 1222,

	// Warp - 침공 플레이타임이 부족합니다..
	WARP_ERROR_NOT_ENOUGH_INVADE_TIME = 1223,

	// Warp - 침공 워프에 필요한 재화가 부족합니다..
	WARP_ERROR_NOT_ENOUGH_INVADE_COST = 1224,

	// Warp - 월드 보스 오픈 시간이 아닙니다
	WARP_ERROR_WORLD_BOSS_OPEN = 1225,

	// Warp - 월드 보스 맵 입장 비용이 모자랍니다
	WARP_ERROR_WORLD_BOSS_ENOUGH_COST = 1226,

	// Warp - 월드 보스 유효하지 않은 상태
	WARP_ERROR_WORLD_BOSS_INVALID_STATUS = 1227,

	// Warp - 월드 보스 이미 입장한 상태
	WARP_ERROR_WORLD_BOSS_ALREADY_ENTER = 1228,

	// Warp - Spawn Group가 없습니다.
	WARP_ERROR_INVALID_SPAWN_GROUP_ID = 1229,

	// Warp - 인스턴스 필드 나가기 대기시간입니다.
	WARP_ERROR_LEAVE_INSTANCE_SINGLE_DELAY_TIME = 1230,

	// Warp - 해당 NPC가 없습니다.
	WARP_ERROR_INVALID_NPC = 1231,

	// Warp - Spawn Group내에 해당 NPC가 없습니다.
	WARP_ERROR_INVALID_NPC_IN_SPAWN_GROUP = 1232,

	// Warp - 현재 이용할 수 없습니다.
	WARP_ERROR_UNLOCK = 1233,

	// Warp - 알수 없는 에러
	WARP_ERROR_UNKNOWN = 1234,

	// 파티 - 알 수 없는 에러
	PARTY_ERROR_UNKNOWN = 1300,

	// 파티 - 유효하지 않은 파티
	PARTY_ERROR_INVALID_PARTY = 1301,

	// 파티 - 이미 파티에 참여중인 상태
	PARTY_ERROR_ALREADY_PARTY_MEMBER = 1302,

	// 파티 - 이미 파티에 초대받은 상태
	PARTY_ERROR_ALREADY_PARTY_INVITED = 1303,

	// 파티 - 내가 파티 참여중이지 않음
	PARTY_ERROR_NOT_JOINED = 1304,

	// 파티 - 파티장이 아님
	PARTY_ERROR_NOT_LEADER = 1305,

	// 파티 - 대상이 파티멤버가 아님
	PARTY_ERROR_NOT_MEMBER = 1306,

	// 파티 - 존재하지 않는 유저입니다.
	PARTY_ERROR_NOT_FOUND_USER = 1307,

	// 파티 - 접속중이지 않은 유저입니다.
	PARTY_ERROR_NOT_LOGIN_USER = 1308,

	// 파티 - 파티멤버가 꽉 참
	PARTY_ERROR_MEMBER_FULL = 1309,

	// 파티 - 초대받은 상태가 아님
	PARTY_ERROR_NOT_INVITED = 1310,

	// 파티 - 스스로를 초대할 순 없음
	PARTY_ERROR_CANNOT_INVITE_SELF = 1311,

	// 파티 - 스스로를 추방할 순 없음
	PARTY_ERROR_CANNOT_KICK_SELF = 1312,

	// 파티 - 이미 파티장임
	PARTY_ERROR_ALREADY_LEADER = 1313,

	// 파티 - 유효하지 않은 드랍 소유권 유형
	PARTY_ERROR_INVALID_DROP_OWNERSHIP_TYPE = 1314,

	// 파티 - 파티장은 파티장에게 바로가기 불가
	PARTY_ERROR_WARP_TO_LEADER_MYSELF = 1315,

	// 파티 - 파티장의 맵이 존재하지 않음
	PARTY_ERROR_INVALID_LEADER_MAP = 1316,

	// 파티 - 파티장의 맵에 스폰 스팟이 존재하지 않음
	PARTY_ERROR_INVALID_LEADER_MAP_SPWAN_SPOT = 1317,

	// 파티 - 파티장이 싱글 인스턴스 던전에 있음
	PARTY_ERROR_LEADER_IN_INSTANCE_MAP = 1318,

	// 파티 - 파티원들이 모두 준비하지않음
	PARTY_ERROR_NOT_ALL_READY = 1319,

	// 파티 - 인스턴스 맵에 있음.
	PARTY_ERROR_USER_IN_INSTANCE_MAP = 1320,

	// 파티 - 동일한 파티입니다.
	PARTY_ERROR_SAME_PARTY = 1321,

	// 파티 - 이미 파티매칭 대기중인 상태
	PARTY_DUNGEON_ERROR_ALREADY_PARTY_MATCHING_WAIT = 1322,

	// 파티 - 파티매칭 대기중인 상태가 아닙니다
	PARTY_DUNGEON_ERROR_NOT_PARTY_MATCHING_WAIT = 1323,

	// 파티 - 파티매칭 시간초과로 실패했습니다.
	PARTY_DUNGEON_ERROR_MATCHING_TIMEOUT = 1324,

	// 파티 던전 - 오픈되지 않은 난이도
	PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY = 1325,

	// 파티 던전 - 진행중이 아닌 상태입니다.
	PARTY_DUNGEON_ERROR_NOT_PLAYING = 1326,

	// 파티 던전 - 도움 요청 상태가 아닙니다.
	PARTY_DUNGEON_ERROR_NOT_HELPCALL = 1327,

	// 파티 던전 - 입장 가능한 맵이 아닙니다.
	PARTY_DUNGEON_ERROR_INVALID_MAP = 1328,

	// 파티 던전 - 도움 가능 회수가 없습니다.
	PARTY_DUNGEON_ERROR_NO_REMAIN_HELP_COUNT = 1329,

	// 파티 던전 - 이미 도움 요청상태입니다.
	PARTY_DUNGEON_ERROR_ALREADY_HELP_STATUS = 1330,

	// 파티 던전 - 이미 진행중인 상태입니다.
	PARTY_DUNGEON_ERROR_PLAYING = 1331,

	// 파티 던전 - 던전 진행 시간이 얼마 남지 않았습니다.
	PARTY_DUNGEON_ERROR_JUST_BEFORE_END = 1332,

	// 파티 던전 - 파티원이 오픈되지 않은 난이도
	PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY_MEMBER = 1333,

	// 파티 - 파티 초대가 불가능한 유저입니다.
	PARTY_ERROR_NOT_INVITABLE_USER = 1334,

	// 파티 - 다른서버 유저는 초대가 불가능합니다.
	PARTY_ERROR_NOT_INVITABLE_OTHER_SERVER_USER = 1335,

	// 파티 - 파티던전 진행중에는 파티 초대를 할 수 없습니다.
	PARTY_DUNGEON_ERROR_CANNOT_INVITE = 1336,

	// 파티 던전 - 인터서버에서 사용 불가능 합니다.
	PARTY_DUNGEON_ERROR_CANNOT_INTER_SERVER = 1337,

	// 파티 던전 - 파티 던전에 입장할 수 없는 지역입니다.
	PARTY_DUNGEON_ERROR_INVALID_AREA = 1338,

	// 인터 파티 - 맞지 않는 서버Id입니다.
	PARTY_ERROR_INVALID_HOME_WORLD_ID = 1339,

	// 인터 파티 - 다른 월드에 접속중인 유저입니다.
	PARTY_ERROR_CANNOT_INVITE_OTHER_ENTER_USER = 1340,

	// 무기 타입 변경 - 알 수 없는 에러
	CHANGE_WEAPON_TYPE_ERROR_UNKNOWN = 1400,

	// 무기 타입 변경 - 유효하지 않은 유저 상태
	CHANGE_WEAPON_TYPE_ERROR_INVALID_USER_STATE = 1401,

	// 무기 타입 변경 - 유효하지 않은 지속 시간
	CHANGE_WEAPON_TYPE_ERROR_INVALID_DURATION = 1402,

	// 무기 타입 변경 - 클래스 변경 중
	CHANGE_WEAPON_TYPE_ERROR_IN_CHANGE_CLASS = 1403,

	// 무기 타입 변경 - 쿨타임 적용 중 다른 타입의 무기 변경 시도
	CHANGE_WEAPON_TYPE_ERROR_COOLDOWN_TIME = 1404,

	// 프리셋 쿨다운 에러
	PRESET_ERROR_COOLDOWN_TIME = 1450,

	// EVENT_ACTION - 유효하지 않은 상태
	EVENT_ACTION_ERROR_INVALID_STATUS = 1500,

	// EVENT_ACTION - 유효하지 않은 맵
	EVENT_ACTION_ERROR_INVALID_MAP = 1501,

	// EVENT_ACTION - 유효하지 않은 SPOT
	EVENT_ACTION_ERROR_INVALID_SPOT = 1502,

	// EVENT_ACTION - 잘못된 요청입니다.
	EVENT_ACTION_ERROR_INVALID_REQUEST = 1503,

	// EVENT_ACTION - 메인Quest만 가능합니다.
	EVENT_ACTION_ERROR_INVALID_MAIN_QUEST = 1504,

	// EVENT_ACTION - 진행하려는 QuestID가 다릅니다.
	EVENT_ACTION_ERROR_INVALID_QUESTID = 1505,

	// EVENT_ACTION - POST EVENT ACTION을 진행하려고하는데, Quest가 완료상태가 아닙니다.
	EVENT_ACTION_ERROR_INVALID_POST_EVENT_ACTION_QUEST_NOT_COMPLETED = 1506,

	// EVENT_ACTION - 유효하지 않은 INDEX 요청 입니다.
	EVENT_ACTION_ERROR_INVALID_INDEX = 1507,

	// EVENT_ACTION - 유효하지 않은 EventActionGroup Id 입니다.
	EVENT_ACTION_ERROR_INVALID_EVENT_ACTION_GROUP_ID = 1508,

	// EVENT_ACTION - 유효하지 않은 EventAction Parameter
	EVENT_ACTION_ERROR_INVALID_PARAM = 1509,

	// EVENT_ACTION - EVENT ACTION 이 모두 완료되었습니다.
	EVENT_ACTION_ERROR_ALL_COMPLETED = 1510,

	// EVENT_ACTION - 죽은 상태에서는 불가능 합니다.
	EVENT_ACTION_ERROR_NOT_ALIVE = 1511,

	// EVENT_ACTION - Warp 중일 때, EventAction 을 못하는 경우(CINEMATIC 못하도록 에러코드 분리)
	EVENT_ACTION_ERROR_USER_WARP = 1512,

	// 속성영역 - 유효하지 않은 상태
	TERRITORY_ERROR_INVALID_STATUS = 1600,

	// 속성영역 - 유효하지 않은 테리토리 아이디
	TERRITORY_ERROR_INVALID_ID = 1601,

	// 속성영역 - 유효하자 않은 위치
	TERRITORY_ERROR_INVALID_POSITION = 1602,

	// 아이템 습득 - 정의되지 않은 에러
	DROP_OBJECT_LOOT_ERROR_UNKNOWN = 1700,

	// 아이템 습득 - 유효하지 않은 유저 상태
	DROP_OBJECT_LOOT_ERROR_INVALID_USER_STATE = 1701,

	// 아이템 습득 - 유효하지 않은 드랍 개체
	DROP_OBJECT_LOOT_ERROR_INVALID_DROP_OBJECT = 1702,

	// 아이템 습득 - 유효하지 않은 루팅 거리
	DROP_OBJECT_LOOT_ERROR_INVALID_DISTANCE = 1703,

	// 아이템 습득 - 이미 습득 처리 중
	DROP_OBJECT_LOOT_ERROR_ALREADY_IN_LOOT = 1704,

	// 아이템 습득 - 소유권 없음
	DROP_OBJECT_LOOT_ERROR_NOT_OWNERSHIP = 1705,

	// 아이템 습득 - 설정된 게임 옵션에 부합되지 않는 아이템을 획득하려함
	DROP_OBJECT_LOOT_ERROR_NOT_MATCH_GAME_OPTION = 1706,

	// 아이템 습득 - 파티 분배 방식에 따라 내가 루팅을 했지만 파티원이 먹을 경우
	DROP_OBJECT_LOOT_ERROR_NOT_MY_ITEM_PARTY_LOOT = 1707,

	// 아이템 습득 - 기사단 소유
	DROP_OBJECT_LOOT_GUILD_STORE_OWNERSHIP = 1708,

	// 아이템 습득 - 잠시 후 다시 시도해주세요
	DROP_OBJECT_LOOT_ERROR_TRY_AGAIN = 1709,

	// 위치 즐겨찾기 - 정의되지 않은 에러
	BOOKMARK_ERROR_UNKNOWN = 1800,

	// 위치 즐겨찾기 - 유효하지 않은 상태
	BOOKMARK_ERROR_INVALID_STATUS = 1801,

	// 위치 즐겨찾기 - 최대 개수를 초과
	BOOKMARK_ERROR_LIST_MAX = 1802,

	// 위치 즐겨찾기 - 생성 불가능한 지역
	BOOKMARK_ERROR_INVALID_MAP = 1803,

	// 위치 즐겨찾기 - 유효하지 않은 즐겨찾기 이름 길이
	BOOKMARK_ERROR_INVALID_TITLE_LEN = 1804,

	// 위치 즐겨찾기 - Gateway 서버 작업이 실패했습니다
	BOOKMARK_ERROR_GATEWAY_OPERATION_FAILED = 1805,

	// 위치 즐겨찾기 - 텔레포트 유효하지 않은 소모 아이템
	BOOKMARK_ERROR_TELEPORT_INVALID_ITEM = 1806,

	// 위치 즐겨찾기 - 유효하지 않은 공유 타입
	BOOKMARK_ERROR_INVALID_SHARE_TYPE = 1807,

	// 위치 즐겨찾기 - 기사단에 가입되지 않음
	BOOKMARK_ERROR_NOT_GUILD_MEMBER = 1808,

	// 위치 즐겨찾기 - 즐겨찾기 공유 권한 없음(기사단 공유 시)
	BOOKMARK_ERROR_NO_SHARE_RIGHT = 1809,

	// 위치 즐겨찾기 - 인터서버로 등록 불가능합니다.
	BOOKMARK_ERROR_CANNOT_INTER_SERVER = 1810,

	// 위치 즐겨찾기 - 즐겨찾기 공유 쿨타임이 아직 남음
	BOOKMARK_ERROR_NOT_SHARE_TIME = 1811,

	// 위치 즐겨찾기 - 공유 할 파티원이 없음
	BOOKMARK_ERROR_NO_PARTY_MEMBERS = 1812,

	// 위치 즐겨찾기 - 공유 할 기사단원이 없음
	BOOKMARK_ERROR_NO_GUILD_MEMBERS = 1813,

	// 즐겨찾기 - 잘못된 타입값
	BOOKMARK_ERROR_INVALID_TYPE = 1814,

	// 즐겨찾기 - 이미 등록된 북마크
	BOOKMARK_ERROR_ALREADY_REGISTERED = 1815,

	// 즐겨찾기 - 인터서버로 워프 불가능합니다.
	BOOKMARK_ERROR_CANNOT_WARP_INTER_SERVER = 1816,

	// INTERACT - 유효한 상태가 아닙니다
	INTERACT_ERROR_INVALID_STATE = 1900,

	// INTERACT - 유효한 NPC가 아닙니다.
	INTERACT_ERROR_INVALID_NPC = 1901,

	// INTERACT - NPC와 유효한 거리가 아닙니다.
	INTERACT_ERROR_INVALID_NPC_RANGE = 1902,

	// INTERACT - START - PROP에 동시에 INTERACTION 유저수를 초과했습니다
	INTERACT_ERROR_INVALID_PROP_START_USERCOUNT_OVER = 1903,

	// INTERACT - START - PROP에 최대 가능한 INTERACTION 유저수를 초과했습니다
	INTERACT_ERROR_INVALID_PROP_MAX_USERCOUNT_LIMIT_OVER = 1904,

	// INTERACT - START - PROP에 이미 인터렉션 진행중이라 시작할 수 없습니다
	INTERACT_ERROR_INVALID_PROP_START_ALREADY = 1905,

	// INTERACT - END - PROP이 INTERACTION 을 할수 없는 상태입니다.
	INTERACT_ERROR_INVALID_PROP_NOT_ENDABLE = 1906,

	// INTERACT - END - PROP이 INTERACTION 을 할수 없는 상태입니다.
	INTERACT_ERROR_INVALID_PROP_STATUS_QUEST_NOT_ENDABLE = 1907,

	// INTERACT - END - PROP이 INTERACTION 을 할수 없는 상태입니다.
	INTERACT_ERROR_INVALID_PROP_STATUS_NOT_CANCEL_ABLE = 1908,

	// INTERACT - 유효한 PROP가 아닙니다.
	INTERACT_ERROR_INVALID_PROP = 1909,

	// INTERACT - QuestDataSheet의 PROP가 아닙니다.
	INTERACT_ERROR_INVALID_QUEST_PROP = 1910,

	// INTERACT - PROP이 SPAWNED 상태가 아닙니다.
	INTERACT_ERROR_NOT_SPAWNED_PROP = 1911,

	// INTERACT - PROP과 유효한 거리가 아닙니다.
	INTERACT_ERROR_INVALID_PROP_RANGE = 1912,

	// INTERACT - 기도사 이벤트 기간이 아닙니다.
	INTERACT_ERROR_DEACTIVATE_EVENT_PRAYER = 1913,

	// INTERACT - 침공포털에 이미 등록되어진 유저
	INTERACT_ERROR_INVADE_PORTAL_PROP_REGISTRATION_ALREADY = 1914,

	// INTERACT - 알맞은 PROPTYPE이 아닙니다.
	INTERACT_ERROR_INVALID_PROP_TYPE = 1915,

	// INTERACT - 죽은 상태에서는 불가능합니다.
	INTERACT_ERROR_NOT_ALIVE = 1916,

	// 스탯 분배 - 확정되지 않은 오류
	BONUS_STAT_ERROR_UNKNOWN = 2000,

	// 스탯 분배 - 유효하지 않는 유저 상태
	BONUS_STAT_ERROR_INVALID_USER_STATUS = 2001,

	// 스탯 분배 - 요청 데이터 오류
	BONUS_STAT_ERROR_INVALID_PKT_DATA = 2002,

	// 스탯 분배 - 유효하지 않은 스탯 타입
	BONUS_STAT_ERROR_INVALID_STAT_TYPE = 2003,

	// 스탯 분배 - DB 작업 실패
	BONUS_STAT_ERROR_DB_FAILED = 2004,

	// 스탯 분배 - 동일 스탯 타입 변경 요청
	BONUS_STAT_ERROR_DUPLICATE_STAT_TYPE = 2005,

	// 스탯 분배 - 스탯 포인트 부족
	BONUS_STAT_ERROR_NOT_ENOUGH_STAT_POINT = 2006,

	// 스탯 분배 - 스탯 최대 분배량 초과
	BONUS_STAT_ERROR_MAX_STAT = 2007,

	// 스킬 슬롯 - 확정되지 않은 오류
	SKILL_SLOT_ERROR_UNKNOWN = 2100,

	// 스킬 슬롯 - 유효하지 않는 유저 상태
	SKILL_SLOT_ERROR_INVALID_USER_STATUS = 2101,

	// 스킬 슬롯 - 요청 데이터 오류
	SKILL_SLOT_ERROR_INVALID_PKT_DATA = 2102,

	// 스킬 슬롯 - 유효하지 않은 스킬 ID
	SKILL_SLOT_ERROR_INVALID_SKILL_ID = 2103,

	// 스킬 슬롯 - 동일한 슬롯에 등록 요청
	SKILL_SLOT_ERROR_SAME_SLOT = 2104,

	// 스킬 슬롯 - 유효하지 않은 스킬 슬롯 정보
	SKILL_SLOT_ERROR_INVALID_SKILL_SLOT_ID = 2105,

	// 스킬 슬롯 - 슬롯에 등록하려는 스킬의 무기타입이 일치하지 않음
	SKILL_SLOT_ERROR_INVALID_WEAPON_TYPE = 2106,

	// 스킬 슬롯 - 동일한 사용 설정 요청
	SKILL_SLOT_ERROR_SAME_AUTO_FLAG = 2107,

	// 스킬 슬롯 - 자동 사용 요청한 스킬이 슬롯에 없음
	SKILL_SLOT_ERROR_NOT_RESIST_SLOT = 2108,

	// 스킬 슬롯 - DB 작업 실패
	SKILL_SLOT_ERROR_DB_FAILED = 2109,

	// 스킬 슬롯 - 동일 스킬 그룹으로 슬롯 등록 실패
	SKILL_SLOT_ERROR_DUPLICATE_SKILL_GROUP = 2110,

	// 스킬 슬롯 - 동일한 자동 사용 주기 설정 데이터 요청
	SKILL_SLOT_ERROR_SAME_INTERVAL_VALUE = 2111,

	// 스킬 슬롯 - 최대 사용 주기 시간 초과
	SKILL_SLOT_ERROR_OVER_INTERVAL_SECOND = 2112,

	// 유저 옵션 - DB 작업 실패
	USER_OPTIPON_ERROR_DB_FAILED = 2200,

	// 포탈 워프 - 유효하지 않은 상태
	PORTAL_WARP_ERROR_INVALID_STATUS = 2300,

	// 포탈 워프 - 알 수 없는 에러
	PORTAL_WARP_ERROR_UNKNOWN = 2301,

	// 포탈 워프 - 유효하지 않은 스팟
	PORTAL_WARP_ERROR_INVALID_SPOT_INFO = 2302,

	// 퀵슬롯 - 확정되지 않은 오류
	QUICK_SLOT_ERROR_UNKNOWN = 2400,

	// 퀵슬롯 - 유효하지 않는 유저 상태
	QUICK_SLOT_ERROR_INVALID_USER_STATUS = 2401,

	// 퀵슬롯 - 한 페이지당 편집 할 수 있는 퀵슬롯 갯수 초과
	QUICK_SLOT_ERROR_OVERFLOW_MAX_PAGE_SIZE = 2402,

	// 퀵슬롯 - 해당 퀵슬롯에 아이템이 존재하지 않음
	QUICK_SLOT_ERROR_ITEM_NOT_EXIST_TARGET = 2403,

	// 퀵슬롯 - 올바르지 않은 퀵슬롯 아이디입니다
	QUICK_SLOT_ERROR_INVALID_QUICK_SLOT_ID = 2404,

	// 퀵슬롯 - 해당하는 퀵슬롯 아이템과 인자 값이 매칭하지 않습니다.
	QUICK_SLOT_ERROR_QUICK_SLOT_ID_DIFF_ITEM_DB_ID = 2405,

	// 퀵슬롯 - Gateway 서버와 연결이 없습니다
	QUICK_SLOT_ERROR_NO_GATEWAY_SERVER_CONNECTION = 2406,

	// 퀵슬롯 - DB 등록에 실패하였습니다(퀵슬롯 편집)
	QUICK_SLOT_ERROR_SET_DB_ERROR = 2407,

	// 퀵슬롯 - DB 갱신에 실패하였습니다(퀵슬롯 자동사용 ON/OFF)
	QUICK_SLOT_ERROR_AUTO_DB_ERROR = 2408,

	// 퀵슬롯 - 자동 사용이 허용되지 않은 아이템 타입입니다
	QUICK_SLOT_ERROR_INVALID_ITEM_TYPE_AUTO = 2409,

	// 페어리 - 유효하지 않은 상태
	FAIRY_ERROR_INVALID_STATUS = 2500,

	// 페어리 - 알 수 없는 에러
	FAIRY_ERROR_UNKNOWN = 2501,

	// 페어리 - 유효하지 않은 페어리 정보
	FAIRY_ERROR_INVALID_FAIRY_ID = 2502,

	// 페어리 - 보유하지 않은 페어리
	FAIRY_ERROR_NOT_HAVE_FAIRY = 2503,

	// 페어리 - 보유 개수 부족
	FAIRY_ERROR_NOT_ENOUGH_FAIRY = 2504,

	// 페어리 - 보유 개수 초과
	FAIRY_ERROR_MAX_AMOUNT_OVER = 2505,

	// 페어리 - 이미 착용 중인 페어리
	FAIRY_ERROR_ALREADY_EQUIP_FAIRY = 2506,

	// 페어리 - 데이터 오류
	FAIRY_ERROR_INVALID_DATA = 2507,

	// 페어리 - 유효하지 않은 패킷 데이터
	FAIRY_ERROR_INVALID_PACKET_DATA = 2508,

	// 페어리 소환 - 유효하지 않은 페어리 등급
	FAIRY_SUMMON_ERROR_INVALID_FAIRY_GRADE = 2509,

	// 페어리 소환 - 확정 대기 페어리 수 초과
	FAIRY_SUMMON_ERROR_MAX_CONFIRM_COUNT = 2510,

	// 페어리 소환 - 소환 아이템 사용 효과 데이터 오류
	FAIRY_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM = 2511,

	// 페어리 확정 - 유효하지 않은 페어리 확정 데이터
	FAIRY_CONFRIM_ERROR_INVALID_FAIRY_CONFIRM_ID = 2512,

	// 페어리 확정 - 잔여 교체 횟수 부족
	FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT = 2513,

	// 페어리 확정 - 만료 기간 초과
	FAIRY_CONFRIM_ERROR_EXPIRED = 2514,

	// 페어리 확정 - 비용 부족
	FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY = 2515,

	// 페어리 성장 - 최대 레벨 초과
	FAIRY_ENCHANT_ERROR_MAX_LEVEL_OVER = 2516,

	// 페어리 성장 - 재료 부족
	FAIRY_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL = 2517,

	// 페어리 성장 - 비용 부족
	FAIRY_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY = 2518,

	// 페어리 합성 - 동일 등급이 아닌 페어리 포함
	FAIRY_COMPOSE_ERROR_NOT_SAME_GRADE = 2519,

	// 페어리 합성 - 최대 합성 카운트 초과
	FAIRY_COMPOSE_ERROR_MAX_COMPOSE_COUNT = 2520,

	// 페어리 합성 - 최대 합성 카운트 초과
	FAIRY_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT = 2521,

	// 페어리 합성 - 비용 부족
	FAIRY_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY = 2522,

	// 페어리 합성 - 레전드 등급은 합성 불가
	FAIRY_COMPOSE_ERROR_LEGEND_GRADE = 2523,

	// 채팅 - 채팅 타입이 유효하지 않는 채팅 타입
	CHAT_ERROR_INVALID_CHAT_TYPE = 2600,

	// 채팅 - 메세지 내용 없음
	CHAT_ERROR_MESSAGE_LEN_ZERO = 2601,

	// 채팅 - 메세지 길이 초과
	CHAT_ERROR_MESSAGE_LEN_OVER = 2602,

	// 채팅 - 찾을 수 없는 유저
	CHAT_ERROR_NOT_FOUND_USER = 2603,

	// 채팅 - 자신에게 보낼 수 없음
	CHAT_ERROR_CAN_NOT_SEND_TO_ME = 2604,

	// 채팅 - 채팅이 도배로 감지 됨
	CHAT_ERROR_DETECTED_AS_SPAM = 2605,

	// 채팅 - 침공 채팅 무료 횟수를 초과 했습니다.
	CHAT_ERROR_EXCEED_FREE_INVADE_CHAT = 2606,

	// 채팅 - 침공 채팅 무료 횟수가 초기화 되었습니다.
	CHAT_ERROR_RESET_FREE_INVADE_CHAT = 2607,

	// 채팅 - 채팅이 금지 되었습니다.
	CHAT_ERROR_USER_BANNED = 2608,

	// 채팅 - 연결 상태가 좋지 않습니다.
	CHAT_ERROR_SERVER_CONNECTION = 2609,

	// 채팅 - 침공 유료 채팅 데이터 문제가 있습니다.
	CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_TYPE = 2610,

	// 채팅 - 침공 유료 채팅 데이터 문제가 있습니다.
	CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_AMOUNT = 2611,

	// 채팅 - 재화가 부족하여 침공 채팅을 할 수 없습니다.
	CHAT_ERROR_PAID_INVADE_CHAT_NOT_ENOUGH_CURRENCY = 2612,

	// 채팅 - 월드에 유저가 존재하지 않습니다.
	CHAT_ERROR_PAID_INVADE_CHAT_NO_USER_IN_WORLD = 2613,

	// 채팅 - 재화 사용 요청에 실패 했습니다.
	CHAT_ERROR_PAID_INVADE_CHAT_FAIL_REQ_PAY = 2614,

	// 채팅 - 침공 채팅 레벨 조건을 충족하지 않습니다.
	CHAT_ERROR_INVADE_CHAT_LEVEL = 2615,

	// 채팅 - 침공 채팅 DB UPDATE 요청 실패
	CHAT_ERROR_INVADE_CHAT_DB_UPDATE_ERROR = 2616,

	// 채팅 - 유저가 현재 인터 서버에 있지 않음.
	CHAT_ERROR_INTER_CHAT_NOT_INTER_SERVER_USER = 2617,

	// 채팅 - 인터 서버 세션이 존재하지 않음.
	CHAT_ERROR_INTER_CHAT_NO_INTER_SERVER_SESSION = 2618,

	// 부활 테이블 ID 가 존재 하지 않음
	RESURRECT_ERROR_INVALID_RESURRECT_ID = 2700,

	// 부활 비용이 부족
	RESURRECT_ERROR_COST_VALUE = 2701,

	// 침공 접속한 경우가 아니인데 침공 종료 버튼을 클릭
	RESURRECT_ERROR_INVASIONOUT = 2702,

	// 사망 패널티 오류 
	DEATH_ERROR_INVALID_ID = 2800,

	// 사망 패널티 복구 골드 재화 부족 
	DEATH_ERROR_RECOVERY_GOLD_COST = 2801,

	// 사망 패널티 복구 다이아 재화 부족 
	DEATH_ERROR_RECOVERY_DIA_COST = 2802,

	// 사망 패널티 복구 복구 아이템 수치 재화 부족 
	DEATH_ERRORR_RECOVERY_ITEM_COST = 2803,

	// 사망 패널티 복구 시간 만료 
	DEATH_ERROR_RECOVERY_TIME_OUT = 2804,

	// 사망 패널티 복구 아이템 횟수 부족 
	DEATH_ERROR_RECOVERY_COUNT = 2805,

	// 기사단 생성 실패 - 유효하지 않은 기사단 이름 길이
	GUILD_CREATE_ERROR_INVALID_NAME_LENGTH = 2850,

	// 기사단 생성 실패 - 유효하지 않은 기사단 소개글 길이
	GUILD_CREATE_ERROR_INVALID_DESC_LENGTH = 2851,

	// 기사단 생성 실패 - 잘못된 이름
	GUILD_CREATE_ERROR_NOT_ALLOWED_NAME = 2852,

	// 기사단 생성 실패 - 중복된 이름
	GUILD_CREATE_ERROR_DUPLICATION_NAME = 2853,

	// 기사단 생성 실패 - 기사단이 이미 있다
	GUILD_CREATE_ERROR_HAD_GUILD = 2854,

	// 기사단 생성 실패 - 골드가 부족하다
	GUILD_CREATE_ERROR_NO_GOLD = 2855,

	// 기사단  - 기사단이 이미 있다
	GUILD_ERROR_HAD_GUILD = 2856,

	// 기사단 - DB 에러
	GUILD_ERROR_DB = 2857,

	// 기사단 - 알수없는 에러
	GUILD_ERROR_UNKNOWN = 2858,

	// 길드 - Gateway 서버와 연결이 없습니다
	GUILD_ERROR_NO_GATEWAY_SERVER_CONNECTION = 2859,

	// 길드 - Gateway 서버 작업이 실패했습니다
	GUILD_ERROR_GATEWAY_OPERATION_FAILED = 2860,

	// 기사단 해제 실패 - 맴버 수가 한명이 아닙니다
	GUILD_DELETE_ERROR_MEMBER_CNT = 2861,

	// 기사단 - 없는 기사단원 입니다
	GUILD_ERROR_NOT_EXSIT_MEMEBER = 2862,

	// 기사단 - 길드 가입 요청 상태가 아니다
	GUILD_ERROR_NOT_REQUEST_JOIN = 2863,

	// 기사단 가입 오류 - 인원이 다 찬 상태입니다
	GUILD_JOIN_ERROR_FULL = 2864,

	// 기사단 가입 오류 - 가입 제한 시간
	GUILD_JOIN_ERROR_COOLDOWN_TIME = 2865,

	// 기사단 가입 오류 - 삭제 되거나 없는 기사단입니다
	GUILD_JOIN_ERROR_INVALID_GUILD = 2866,

	// 기사단 가입 오류 - 가입제한 레벨입니다
	GUILD_JOIN_ERROR_USER_LEVEL = 2867,

	// 기사단 탈퇴 오류 -  기사단장은 기사단 탈퇴가 불가능합니다
	GUILD_LEAVE_ERROR_CAPTAIN = 2868,

	// 기사단 오류 -  등급 인원이 다 찬 상태입니다
	GUILD_ERROR_FULL_GRADE_NUM = 2869,

	// 기사단 삭제 - 해당 권한이 없습니다
	GUILD_DELETE_ERROR_GRADE = 2870,

	// 기사단  - 권한이 없습니다
	GUILD_ERROR_GRADE = 2871,

	// 가입하려는 기사단이 존재하지 않습니다.
	GUILD_ERROR_NOT_FIND = 2872,

	// 기사단  - 기부 횟수 초과
	GUILD_ERROR_DAILY_DONATE_OVER = 2873,

	// 기사단  - 재화 부족 함
	GUILD_ERROR_ENOUGH_CURRENCY = 2874,

	// 기사단  - 기사단 코인 초과
	GUILD_ERROR_GUILD_COIN_FULLY = 2875,

	// 기사단  - 기사단 운영비 부족
	GUILD_ERROR_NOT_ENOUGH_GUILD_ASSET = 2876,

	// 기사단  - 이미 출석 체크 함
	GUILD_ERROR_ATTENDANCE_ALREADY = 2877,

	// 기사단 등급 오류 - 없는 기사단원입니다
	GUILD_GRADE_ERROR_INAVLID_MEMBER = 2878,

	// 기사단 등급 오류 - 변경할 수 있는 등급이 아닙니다
	GUILD_GRADE_ERROR_DO_NOT_CHANGE_GRADE_LEVEL = 2879,

	// 기사단 건물 번호가 유효 하지 않음
	GUILD_ERROR_BUILDING_INAVLID_ID = 2880,

	// 기사단 건물 디비 오류
	GUILD_ERROR_BUILDING_DB = 2881,

	// 기사단 건물 설정 권한이 없음
	GUILD_ERROR_BUILDING_AUTH = 2882,

	// 기사단 건물 번호가 유효 하지 않음
	GUILD_ERROR_BUILDING_OPEN = 2883,

	// 기사단 건물 업그레이드 할수 없음
	GUILD_ERROR_BUILDING_UPGRADE = 2884,

	// 기사단 건물 서버에 데이터가 없음
	GUILD_ERROR_BUILDING_DATE = 2885,

	// 기사단 건물 레벨 부족 또는 레벨 오류
	GUILD_ERROR_BUILDING_LEVEL = 2886,

	// 기사단 캠프 레벨 부족 또는 레벨 오류
	GUILD_ERROR_GUILD_CAMP_LEVEL = 2887,

	// 기사단 건물 최고 레벨
	GUILD_ERROR_BUILDING_MAX_LEVEL = 2888,

	// 기사단 건물 운영자금이 부족한 경우 
	GUILD_ERROR_BUILDING_ASSET = 2889,

	// 기사단 건물 이미 사용중인 패시브티
	GUILD_ERROR_BUILDING_PASSIVITY = 2890,

	// 운영 권한이 없어 레벨업를 할 수 없습니다.
	GUILD_ERROR_BUILDING_UNAUTHORIZED_LEVELUP = 2891,

	// 기사단 - 기사단 레벨이 부족합니다
	GUILD_ERROR_UNABLE_GUILD_LEVEL = 2892,

	// 기사단 상점 - 기사단 레벨이 부족하여 구매할 수 없는 상품입니다
	GUILD_BUY_SHOP_ERROR_UNABLE_GUILD_LEVEL = 2893,

	// 기사단 상점 - 잘못된 상품정보
	GUILD_BUY_SHOP_INVALID_PRODUCT_ID = 2894,

	// 기사단 상점 - 구매 개수 초과
	GUILD_BUY_SHOP_ERROR_OVER_CNT = 2895,

	// 기사단 상점- 재화가 부족합니다
	GUILD_SHOP_ERROR_ENOUGH_CURRENCY = 2896,

	// 기사단  - 등급을 변경할 수 없다
	GUILD_ERROR_CHANGE_MEMBER_GRADE = 2897,

	// 기사단 가입 요청 - 가입 요청 대기 인원이 꽉찼습니다
	GUILD_JOIN_ERROR_FULL_OF_APPLICANTS = 2898,

	// 기사단 가입 요청 - 가입 요청할 수 있는 기사단 수 초과
	GUILD_JOIN_ERROR_OVER_REQUEST_CNT = 2899,

	// 기사단  - 가입 당일에는 출첵 기부등을 할수 없다
	GUILD_ACTIVITY_ERROR_GUILD_JOIN = 2900,

	// 기사단 - 요청한 기사단은 적대 리스트에 없습니다
	GUILD_ERROR_EXIST_HOSTILE_GUILD = 2901,

	// 기사단 - 유효하지 않은 캐릭터
	GUILD_ERROR_INVALID_USER = 2902,

	// 기사단 - 해당 기사단이 존재하지 않습니다
	GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID = 2903,

	// 기사단 외교- 이미 동맹 기사단 리스트
	GUILD_ERROR_ALLY_GUILD_ID = 2904,

	// 기사단 외교- 이미 동맹 요청중인 기사단 리스트
	GUILD_ERROR_ALLY_REQUEST_GUILD_ID = 2905,

	// 기사단 - 기사단이 존재하지 않습니다
	GUILD_ERROR_UNKNOWN_GUILD_ID = 2906,

	// 기사단 - 가입 신청 개수 초과
	GUILD_ERROR_MAX_REQUESTED = 2907,

	// 기사단 가입 오류 - 해당 기사단이 가입 불가 상태입니다
	GUILD_JOIN_ERROR_UNABLE = 2908,

	// 기사단 가입 오류 - 해당 기사단에 가입 요청중인 상태입니다
	GUILD_JOIN_ERROR_EXIST_LIST = 2909,

	// 기사단 - 기사단원이 아닙니다
	GUILD_ERROR_NOT_JOIN = 2910,

	// 기사단 - 유효하지 않는 채팅 아이디 입니다.
	GUILD_ERROR_INVALID_GUILD_CHAT_ID = 2911,

	// 기사단 검색 - 재 검색 대기시간이 지나지 않음 .
	GUILD_SEARCH_ERROR_COOLDOWN = 2912,

	// 기사단  - 기사단 운영비 초과
	GUILD_ERROR_GUILD_ASSET_FULLY = 2913,

	// 기사단 검색(외교) - 적대 기사단으로 등록되어있습니다.
	GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_HOSTILE_GUILD = 2914,

	// 기사단 검색(외교) -이미  동맹 기사단 요청중입니다 .
	GUILD_SEARCH_DILPLOMACY_ERROR_REQUEST_ALLY_GUILD = 2915,

	// 기사단 검색(외교) - 동맹 기사단으로 등록되어있습니다.
	GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_ALLY_GUILD = 2916,

	// 기사단 검색 - 찾을수 없는 기사단 이름 입니다
	GUILD_SEARCH_ERROR_MATCHING = 2917,

	// 기사단 - 요청한 기사단은 동맹 리스트에 없습니다
	GUILD_ERROR_EXIST_ALLY_GUILD = 2918,

	// 기사단 - 요청한 기사단은 동맹 요청 리스트에 없습니다
	GUILD_ERROR_EXIST_ALLY_REQUEST_GUILD = 2919,

	// 기사단 외교 - 현재 기사단과 같은 이름입니다.
	GUILD_ERROR_SAME_GUILD_ID = 2920,

	// 운영 권한이 없어 이용할 수 없습니다
	GUILD_ERROR_UNAUTHORIZED_DIPLOMACY = 2921,

	// 기사단 외교- 이미 적대 기사단 리스트이다
	GUILD_ERROR_HOSTILE_GUILD_ID = 2922,

	// 기사단 외교- 더 이상 추가할수 없습니다(외교 수 최대치)
	GUILD_ERROR_ERROR_FULL = 2923,

	// 기사단 외교- 요청한 기사단의 외교 수 가 최대치 입니다.
	GUILD_ERROR_OPPONENT_FULL = 2924,

	// 기사단 건물 이미 사용중인 패시브티
	GUILD_ERROR_BUILDING_REACTIVATE_TIME = 2925,

	// 기사단 선물 - 아이템 사용 효과 데이터 오류
	GUILD_GIFT_ERROR_INVALID_ITEM_EFFECT_PARAM = 2926,

	// 기사단 생성 실패 -  DB Error Insert Guild
	GUILD_CREATE_ERROR_INSERT_GUILD = 2927,

	// 기사단 생성 실패 -  DB Error Insert Guild Row Count
	GUILD_CREATE_ERROR_INSERT_GUILD_ROW_COUNT = 2928,

	// 기사단 생성 실패 -  DB Error Insert Guild Member
	GUILD_CREATE_ERROR_INSERT_GUILD_MEMBER = 2929,

	// 기사단 생성 실패 -  DB Error Delete Guild Request Join
	GUILD_CREATE_ERROR_DELETE_GUILD_REQUEST_JOIN = 2930,

	// 기사단 던전 입장 - 입장 티켓이 충분하지 않다
	GUILD_DUNGEON_WARP_ERROR_NOT_ENOUGH_TICKET = 2931,

	// 기사단 던전  입장 - 현재 기사단 던전맵 입니다.
	GUILD_DUNGEON_WARP_ERROR_IN_GUILD_DUNGEON_MAP = 2932,

	// 기사단 던전  입장 - 기사단 창고가 가득차서 던전으로 이동을 할수가 없습니다
	GUILD_DUNGEON_WARP_ERROR_STORE_FULL = 2933,

	// 기사단 던전  입장 - 이전 레벨 기사단 던전이 파괴 되지 않았습니다
	GUILD_DUNGEON_WARP_ERROR_DONT_DESTROY = 2934,

	// 기사단 던전  입장 - 더이상 던전 난이도가 없다
	GUILD_DUNGEON_WARP_ERROR_LEVEL = 2935,

	// 기사단 던전  입장 - 행동 불가 상태이다
	GUILD_DUNGEON_WARP_ERROR_ABNORMALITY_EFFECT = 2936,

	// 기사단 던전  관람 - 현재 기사단맵이 아니다
	GUILD_DUNGEON_WATCHING_ERROR_MAP = 2937,

	// 기사단 던전  관람 - 관람할 사람이 없다
	GUILD_DUNGEON_WATCHING_NO_ONE_HERE = 2938,

	// 기사단 던전 입장 - 결산 시간입니다
	GUILD_DUNGEON_RESET_TIME = 2939,

	// 기사단 던전  입장 - 기사단 레벨이 부족하다
	GUILD_DUNGEON_WARP_ERROR_ENOUGH_GUILD_LEVEL = 2940,

	// 기사단 던전  입장 - 가입후 일정 기간 지나지 않았다
	GUILD_DUNGEON_WARP_ERROR_THE_DAY_JOINED_GUILD = 2941,

	// 기사단 탈퇴 불가 - 기사단 던전 플레이중인 유저는 탈퇴가 불가능하다
	GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER = 2942,

	// 기사단 던전 입장 - 입장하려는 기사단 던전의 레벨이  아닙니다(연결이 끊어져 재 로그인/재 연결 시 다른 레벨의 기사단이 생성된경우)
	GUILD_DUNGEON_WARP_ERROR_ANOTHER_GUILD_LEVEL = 2943,

	// 인터서버 동맹 기능은 준비중입니다.
	GUILD_ERROR_INTER_ALLY_PREPARING = 2944,

	// 기사단 동맹 신청  - 기사단이 존재하지 않습니다
	GUILD_ERROR_INTER_ALLY_REQUEST_UNKNOWN_GUILD_ID = 2945,

	// 기사단 동맹 신청  - 서버 내부 오류
	GUILD_ERROR_INTER_ALLY_REQUEST_ERROR_DB = 2946,

	// 기사단 동맹 수락  - 기사단이 존재하지 않습니다
	GUILD_ERROR_INTER_ALLY_ACCEPT_UNKNOWN_GUILD_ID = 2947,

	// 기사단 동맹 수락  - 서버 내부 오류
	GUILD_ERROR_INTER_ALLY_ACCEPT_ERROR_DB = 2948,

	// 기사단 탈퇴 할수 없음 탈퇴를 진행할수 없는 상태입니다.
	GUILD_LEAVE_ERROR_CANNOT_STATE = 2949,

	// 영지 - 현재 영지 맵 입니다.
	AGIT_WARP_ERROR_IN_AGIT_MAP = 2950,

	// 영지 - 영지 임대 시간종료
	AGIT_WARP_ERROR_EXPIRE = 2951,

	// 영지 - 현재 영지 맵 이 아닙니다.
	AGIT_WARP_ERROR_NOT_AGIT_MAP = 2952,

	// 영지 -  유효하지 않은 캐릭터
	AGIT_INVALID_USER = 2953,

	// 영지 - 더이상 임대를 확장 할수 없다 
	AGIT_ERROR_NO_MORE_RENTAL_EXPANSION = 2954,

	// 영지 - 임대를 할수 없다 
	AGIT_INVALID_RENTAL = 2955,

	// 영지 - 영지 임대 중이 아닙니다
	AGIT_ERROR_NOT_RENTAING = 2956,

	// 재련 실패 - 유효하지 않은 제련 타겟
	ITEM_ERROR_REFINE_INVALID_TARGET = 3000,

	// 재련 실패 - 올바르지 않은 스탯 데이터
	ITEM_ERROR_REFINE_INVALID_STAT_DATA = 3001,

	// 재련 실패 - 타겟 아이템과 맞지 않은 재련석
	ITEM_ERROR_REFINE_OPTION_STONE_NOT_MEET_THE_CONDITION = 3002,

	// 재련 실패 - 잠겨있는 아이템은 제련 불가
	ITEM_ERROR_REFINE_LOCKED = 3003,

	// 아르카스 실패 - 유효하지 않은 상태 입니다
	ARCAS_ERROR_INVALID_STATUS = 3100,

	// 아르카스 실패 - Gateway 서버 작업이 실패했습니다
	ARCAS_ERROR_GATEWAY_OPERATION_FAILED = 3101,

	// 아르카스 실패 - 명상 상태에는 불가능 합니다
	ARCAS_ERROR_MEDITATION = 3102,

	// 아르카스 실패 - 명상 상태가 아닙니다
	ARCAS_ERROR_NOT_MEDITATION = 3103,

	// 아르카스 실패 - 아르카스 버프는 해제 할 수 없습니다
	ARCAS_ERROR_NOT_REMOVE_ARCAS_BUFF = 3104,

	// 아르카스 실패 - 아르카스 버프가 켜져 있습니다
	ARCAS_ERROR_ARCAS_BUFF_ON = 3105,

	// 아르카스 실패 - 아르카스 버프가 켜져있어서, 포인트 획득 아이템을 사용 할 수 없습니다.
	ARCAS_ERROR_NOT_USE_ARCAS_POINT_GAIN = 3106,

	// 아르카스 실패 - 아르카스 축복수치를 얻어 올수 없습니다.
	ARCAS_ERROR_INVALID_ARCAS_POINT_DATA = 3107,

	// 아르카스 실패 - 아르카스 축복수치가 부족합니다
	ARCAS_ERROR_NOT_ENOUGH_ARCAS_POINT = 3108,

	// 아르카스 실패 - 
	ARCAS_ERROR_INVALID_ARCAS_POINT_AND_NOT_EXIST_FAIRY_BUFF_ABNORMALITY = 3109,

	// 아르카스 실패 - 유료하지 않은 버프 아이디 입니다
	ARCAS_ERROR_INVALID_PASSIVITY_ID = 3110,

	// 아르카스 실패 - 최대 수치여서, 사용할 수 없습니다.
	ARCAS_ERROR_NOT_ITEM_USE_MAX_ARCAS_POINT = 3111,

	// 유저 일반 처리 실패 - 레벨 상승 시 보너스 스탯 포인트 정보 오류
	USER_COMMON_ERROR_INVALID_STAT_POINT_DATA = 3200,

	// 유저 일반 처리 실패 - 유효 하지 않은 캐릭터 상태입니다
	USER_COMMON_ERROR_INVALID_USER = 3201,

	// 코스튬 - 유효하지 않은 코스튬ID 입니다
	COSTUME_ERROR_UNKNOWN = 3300,

	// 코스튬 - 유효하지 않은 상태
	COSTUME_ERROR_INVALID_STATUS = 3301,

	// 코스튬 - 유효하지 않은 코스튬ID 입니다
	COSTUME_ERROR_INVALID_COSTUME_ID = 3302,

	// 코스튬 - 보유중이지 않은 코스튬입니다.
	COSTUME_ERROR_NOT_HAVE_COSTUME = 3303,

	// 코스튬 - 이미 장착된 코스튬과 동일합니다.
	COSTUME_ERROR_ALREADY_EQUIP_COSTUME = 3304,

	// 코스튬 - 보유중인 코스튬 갯수가 부족합니다
	COSTUME_ERROR_NOT_ENOUGH_COSTUME = 3305,

	// 코스튬 - 최대 보유 갯수를 초과 합니다.
	COSTUME_ERROR_MAX_AMOUNT_OVER = 3306,

	// 코스튬 - 데이터 오류
	COSTUME_ERROR_INVALID_DATA = 3307,

	// 코스튬 - 유효하지 않은 패킷 데이터
	COSTUME_ERROR_INVALID_PACKET_DATA = 3308,

	// 코스튬 소환 - 유효하지 않은 코스튬 등급
	COSTUME_SUMMON_ERROR_INVALID_COSTUME_GRADE = 3309,

	// 코스튬 소환 - 확정 대기 코스튬 수 초과
	COSTUME_SUMMON_ERROR_MAX_CONFIRM_COUNT = 3310,

	// 코스튬 소환 - 소환 아이템 사용 효과 데이터 오류
	COSTUME_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM = 3311,

	// 코스튬 확정 - 유효하지 않은 코스튬 확정 데이터
	COSTUME_CONFRIM_ERROR_INVALID_COSTUME_CONFIRM_ID = 3312,

	// 코스튬 확정 - 잔여 교체 횟수 부족
	COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT = 3313,

	// 코스튬 확정 - 만료 기간 초과
	COSTUME_CONFRIM_ERROR_EXPIRED = 3314,

	// 코스튬 확정 - 비용 부족
	COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY = 3315,

	// 코스튬 성장 - 최대 레벨 초과
	COSTUME_ENCHANT_ERROR_MAX_LEVEL_OVER = 3316,

	// 코스튬 성장 - 재료 부족
	COSTUME_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL = 3317,

	// 코스튬 성장 - 비용 부족
	COSTUME_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY = 3318,

	// 코스튬 합성 - 동일 등급이 아닌 코스튬 포함
	COSTUME_COMPOSE_ERROR_NOT_SAME_GRADE = 3319,

	// 코스튬 합성 - 최대 합성 카운트 초과
	COSTUME_COMPOSE_ERROR_MAX_COMPOSE_COUNT = 3320,

	// 코스튬 합성 - 최대 합성 카운트 초과
	COSTUME_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT = 3321,

	// 코스튬 합성 - 비용 부족
	COSTUME_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY = 3322,

	// 코스튬 합성 - 레전드 등급은 합성 불가
	COSTUME_COMPOSE_ERROR_LEGEND_GRADE = 3323,

	// 코스튬 스킬 - 코스튬 스킬이 아님
	COSTUME_SKILL_ERROR_NOT_COSTUME_SKILL = 3324,

	// 코스튬 스킬 - 코스튬 스킬이 아닌 스킬을 자동 사용 등록함
	COSTUME_SKILL_ERROR_NOT_COSTUME_SKILl_ID = 3325,

	// 코스튬 스킬 - 이미 코스튬 스킬을 습득 함.
	COSTUME_SKILL_ERROR_ALREADY_LEARNED_COSTUME_SKILL = 3326,

	// 코스튬 스킬 - 아직 습득한 코스튬 스킬이 아님.
	COSTUME_SKILL_ERROR_NOT_LEARNED_COSTUME_SKILL = 3327,

	// 코스튬 스킬 - 유효하지 않은 코스튬 스킬 아이디
	COSTUME_SKILL_ERROR_INVALID_SKILL_ID = 3328,

	// 코스튬 스킬 - 슬롯에 설정할 코스튬 스킬 정보가 없음
	COSTUME_SKILL_ERROR_FAIL_TO_SET_SKILL_SLOT = 3329,

	// 코스튬 스킬 - 코스튬에 스킬이 없음
	COSTUME_SKILL_ERROR_NO_COSTUME_SKILL = 3330,

	// 코스튬 스킬 - 장착된 코스튬 스킬이 없음
	COSTUME_SKILL_ERROR_NO_EQUIPED_COSTUME_SKILL = 3331,

	// 코스튬 교환 - 교환이 불가한 코스튬 등급
	COSTUME_CHANGE_ERROR_INVALID_GRADE = 3332,

	// 코스튬 교환 - 교환이 불가한 코스튬 ID
	COSTUME_CHANGE_ERROR_EXCLUDE_ID = 3333,

	// 코스튬 교환 - 재료와 선택이 동일함
	COSTUME_CHANGE_ERROR_SAME_COSTUME_ID = 3334,

	// 코스튬 교환 - 선택 대상이 아닌 코스튬을 선택함
	COSTUME_CHANGE_ERROR_NOT_IN_PROBABILITY_POOL = 3335,

	// 코스튬 교환 - 선택권 아이템이 아닌데 선택했음
	COSTUME_CHANGE_ERROR_SELECT_COSTUME = 3336,

	// 코스튬 교환 - 선택권 아이템인데 선택을 안함
	COSTUME_CHANGE_ERROR_NOT_SELECT_COSTUME = 3337,

	// 던전 - 알수 없는 에러
	DUNGEON_ERROR_UNKNOWN = 3400,

	// 던전 - 유효하지 않은 던전 그룹ID
	DUNGEON_ERROR_INVALID_DUNGEON_GROUP_ID = 3401,

	// 던전 - 유효하지 않은 던전 목록 인덱스
	DUNGEON_ERROR_INVALID_DUNGEON_LIST_IDX = 3402,

	// 던전 - 비용이 부족합니다
	DUNGEON_ERROR_NOT_ENOUGH_COST = 3403,

	// 던전 - 입장할 수 있는 상태가 아닙니다
	DUNGEON_ERROR_INVALID_STATUS = 3404,

	// 던전 - 충전 시간이 부족합니다
	DUNGEON_ERROR_NOT_ENOUGH_TIME = 3405,

	// 던전 - 충전 시간이 초과됩니다
	DUNGEON_ERROR_TIME_OVERFLOW = 3406,

	// 던전 - 오픈시간이 아닙니다.
	DUNGEON_ERROR_NOT_OPEN_TIME = 3407,

	// 던전 - 충전을 할수 없는 던전입니다.
	DUNGEON_ERROR_DISABLE_TIME_CHARGE = 3408,

	// 창고 - 알 수 없는 에러
	DEPOT_ERROR_UNKNOWN = 3500,

	// 창고 에러 - 재화 부족
	DEPOT_ERROR_NOT_ENOUGH_MONEY = 3501,

	// 창고 확장 에러 - 알 수 없는 에러
	DEPOT_EXTEND_ERROR_UNKNOWN = 3502,

	// 창고 확장 에러 - 더 이상 확장 할수 없습니다
	DEPOT_EXTEND_ERROR_MAX_SLOT = 3503,

	// 창고 확장 에러 - 재화가 모자랍니다
	DEPOT_EXTEND_ERROR_NOT_ENOUGH_MONEY = 3504,

	// 창고 - Gateway 서버와 연결이 없습니다
	DEPOT_ERROR_NO_GATEWAY_SERVER_CONNECTION = 3505,

	// 창고 - Gateway 서버 작업이 실패했습니다
	DEPOT_ERROR_GATEWAY_OPERATION_FAILED = 3506,

	// 창고 - 유효하지 않은 유저 상태
	DEPOT_ERROR_INVALID_USER_STATE = 3507,

	// 창고 - 유효하지 않은 유저
	DEPOT_ERROR_INVALID_USER = 3508,

	// 창고 에러 - 이동할수 없는 아이템
	DEPOT_ERROR_DONT_MOVE_ITEM = 3509,

	// 아이템 컬렉션 - 올바르지 않은 컬렉션 아이디입니다
	ITEM_COLLECTION_ERROR_INVALID_COLLECTION_ID = 3600,

	// 아이템 컬렉션 - 올바르지 않은 컬렉션 조건 아이디입니다
	ITEM_COLLECTION_ERROR_INVALID_COLLECTION_CONDITION_ID = 3601,

	// 아이템 컬렉션 - 조건에 맞는 아이템이 아닙니다
	ITEM_COLLECTION_ERROR_NOT_SUITABLE_ITEM = 3602,

	// 아이템 컬렉션 - 이미 등록된 아이템입니다
	ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_ITEM = 3603,

	// 아이템 컬렉션 - 올바르지 않은 카테고리 아이디입니다
	ITEM_COLLECTION_ERROR_INVALID_CATEGORY_ID = 3604,

	// 아이템 컬렉션 - 올바르지 않은 언락 타입입니다
	ITEM_COLLECTION_ERROR_INVALID_UNLOCK_TYPE = 3605,

	// 아이템 컬렉션 - 언락 조건에 부합되지 않습니다
	ITEM_COLLECTION_ERROR_DOES_NOT_MEET_THE_UNLOCK_CONDITION = 3606,

	// 아이템 컬렉션 - 올바르지 않은 퀘스트 타입입니다
	ITEM_COLLECTION_ERROR_INVALID_QUEST_TYPE = 3607,

	// 아이템 컬렉션 - 기간이 이미 지난 컬렉션입니다
	ITEM_COLLECTION_ERROR_EXPIRE_DATE_TIME = 3608,

	// 아이템 컬렉션 - 올바르지 않은 즐겨찾기입니다.
	ITEM_COLLECTION_ERROR_INVALID_BOOKMARK = 3609,

	// 천마석 - 게이트 웨이 작업에 실패하였습니다
	SPIRIT_SHOT_ERROR_GATEWAY_OPERATION_FAILED = 3700,

	// 천마석 - 스폰이 안되었거나, 죽어있는 상태
	SPIRIT_SHOT_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE = 3701,

	// 천마석 - 유효한 CategoryId가 아닙니다.
	SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_CATEGORY_ID = 3702,

	// 천마석 - 유효한 LevelId가 아닙니다.
	SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_LEVEL_ID = 3703,

	// 천마석 - 다음 레벨이 존재하지 않습니다.
	SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_NEXT_LEVEL_ID = 3704,

	// 천마석 - 추가 확률을 위해, 클라가 보내온 값 검증 실패
	SPIRIT_SHOT_ERROR_INVALID_MODIFY_ENCHANT_RATE_COUNT = 3705,

	// 천마석 - 재료아이템, 클라가 보내온 값 검증 실패
	SPIRIT_SHOT_ERROR_INVALID_ITEM_COUNT = 3706,

	// 천마석 - 이미 전과 동일한 상태입니다
	SPIRIT_SHOT_ERROR_ALREADY_SAME_STATE_AS_BEFORE = 3707,

	// 천마석 강화 - 게이트 웨이 작업에 실패하였습니다
	SPIRIT_SHOT_ENCHANT_ERROR_INVALID_STATUS = 3708,

	// 천마석 강화 - 게이트 웨이 작업에 실패하였습니다
	SPIRIT_SHOT_ENCHANT_ERROR_GATEWAY_OPERATION_FAILED = 3709,

	// 천마석 강화 - 이전과 동일한 강화 레벨 입니다
	SPIRIT_SHOT_ENCHANT_ERROR_ALREADY_SAME_STATE_AS_BEFORE = 3710,

	// 천마석 강화 - 잘못 요청된 강화 레벨 입니다
	SPIRIT_SHOT_ENCHANT_ERROR_LEVEL = 3711,

	// 천마석 강화 - 잘못 요청된 강화 레벨 입니다
	SPIRIT_SHOT_ENCHANT_ERROR_ITEM = 3712,

	// 천마석 강화 - UnLockContents 제약
	SPIRIT_SHOT_ENCHANT_ERROR_UNLOCK_CONTENTS = 3713,

	// 천마석 강화 - 골드 부족
	SPIRIT_SHOT_ENCHANT_ERROR_NOT_ENOUGH_MONEY = 3714,

	// 자동 사용 물약 - 게이트 웨이 작업에 실패하였습니다
	POTION_AUTO_USE_ERROR_GATEWAY_OPERATION_FAILED = 3800,

	// 자동 사용 물약 - 이미 전과 동일한 상태입니다
	POTION_AUTO_USE_ERROR_ALREADY_SAME_STATE_AS_BEFORE = 3801,

	// BM 상점 - 게이트 웨이 작업에 실패하였습니다
	BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED = 3900,

	// BM 상점 - 알수 없는 오류
	BM_SHOP_ERROR_UNKNOWN = 3901,

	// BM 상점 - Gateway 서버와 연결이 없습니다
	BM_SHOP_NO_GATEWAY_SERVER_CONNECTION = 3902,

	// BM 상점 - 유효하지 않은 캐릭터
	BM_SHOP_ERROR_INVALID_USER = 3903,

	// BM 상점 - 검증 되지 않은 Identifier 값입니다
	BM_SHOP_ERROR_IDENDTIFIER = 3904,

	// BM 상점 - Hive Sync error Market PId
	BM_SHOP_ERROR_NOT_HIVE_SYNC_MARKET_PID = 3905,

	// BM 상점 - 유효하지 않은 캐릭터 상태
	BM_SHOP_ERROR_INVALID_USER_STATUS = 3906,

	// BM 상점 - 유효하지 않은 레벨
	BM_SHOP_ERROR_NOT_ALLOWED_LEVEL = 3907,

	// BM 상점 - 유효하지 않은 길드 레벨
	BM_SHOP_ERROR_NOT_ALLOWED_GUILD_LEVEL = 3908,

	// BM 상점 - 존재하지 않는 아이템
	BM_SHOP_ERROR_ITEM_NOT_FOUND = 3909,

	// BM 상점 - 유효하지 않은 ItemId
	BM_SHOP_ERROR_INVALID_ITEM_ID = 3910,

	// BM 상점 - 존재하지 않는 상품
	BM_SHOP_ERROR_PRODCUT_NOT_FOUND = 3911,

	// BM 상점 - 유효하지 않은 ProductId
	BM_SHOP_ERROR_INVALID_PRODCUT_ID = 3912,

	// BM 상점 - 유효하지 않은 상품 TabId
	BM_SHOP_ERROR_INVALID_TAB_ID = 3913,

	// BM 상점 - 재화 부족
	BM_SHOP_ERROR_NOT_ENOUGH_MONEY = 3914,

	// BM 상점 - 재화 가득참
	BM_SHOP_ERROR_OVERFLOW_MAX_MONEY = 3915,

	// BM 상점 - 구매 횟수 초과
	BM_SHOP_ERROR_OVER_QUANTIY = 3916,

	// BM 상점 - 판매 날짜 아님
	BM_SHOP_ERROR_SALE_DATETIME = 3917,

	// BM 상점 - 캐쉬 구매 실패
	BM_SHOP_ERROR_CASH_PURCHASE = 3918,

	// BM 상점 - 구매  실패
	BM_SHOP_ERROR_PURCHASE = 3919,

	// BM 상점 - 구매  중복된 HIVE_IAP_TRANSACTION_ID
	BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID = 3920,

	// BM 상점 - 게이트 웨이 DB 작업에 실패하였습니다
	BM_SHOP_ERROR_DB_OPERATION_FAILED = 3921,

	// BM 상점 - 하이브 연동 작업에 실패하였습니다
	BM_SHOP_ERROR_HIVE_OPERATION_FAILED = 3922,

	// BM 상점 - 잘못된 정보의 maketPId 값입니다
	BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT = 3923,

	// BM 상점 - 구매 실패(살 수 없는 아이템 구매 시도)
	BM_SHOP_ERROR_IMPOSSIBLE_ITEM = 3924,

	// BM 상점 - 하이브 영수증 검증 안함
	BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY = 3925,

	// BM 상점 - 하이브 아이템 지급 결과 전송 안함
	BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT = 3926,

	// BM 상점 - 하이브 아이템 지급 DB 저장 안함
	BM_SHOP_DONT_TRY_SAVE_DB = 3927,

	// BM 상점 - 재화 부족하거나 더이상 획득 할수가 없습니다
	BM_SHOP_ERROR_MONEY = 3928,

	// BM 상점 - 구매목록 저장 실패
	BM_SHOP_ERROR_SAVE_BULK_SALE = 3929,

	// BM 상점 - 캐쉬 아이템은 하나만 살 수 있다
	BM_SHOP_ERROR_CASH_ITEM_ONE_AT_TIME = 3930,

	// BM 상점 - 일괄 구매상품이 아니다
	BM_SHOP_ERROR_BULK_SALE_ITEM = 3931,

	// BM 상점 - 구매할 수 있는 상품이 아니다.
	BM_SHOP_ERROR_POPUP_FAILED = 3932,

	// BM 상점 - 구매할 수 있는 상품이 아니다.
	BM_SHOP_ERROR_STEPUP_FAILED = 3933,

	// BM 상점 - 재화 아이템과 판매 아이템이 중복됨 관리자문의 
	BM_SHOP_DUPLICATION_COST_ITEM = 3934,

	// BM 상점 - 해당 쿠폰의 정보가 유효하지 않습니다. 
	BM_SHOP_COUPON_ITEM_INVALID = 3935,

	// BM 상점 - 캐쉬상품은 사용할수 없습니다 
	BM_SHOP_CAN_NOT_COUPON_ITEM_FOR_CASH = 3936,

	// BM 상점 - 캐쉬상품을 사용할수 없습니다 
	BM_SHOP_CAN_NOT_COUPON_ITEM = 3937,

	// BM 상점 - 별도 계정에서 결재를 진행한 이력이 있어 구매 요청을 하였으나, 계정이 달라 본 계정으로는 상품 결재가 이루어 지지 않았습니다. 결재를 진행한 계정을 접속하여 구매 상품을 확인하세요.
	BM_SHOP_ERROR_PURCHASE_OHTER_ACCOUNT = 3938,

	// BM 상점 - 구글 플레이 포인트 산 구매 상품에 없습니다.
	BM_SHOP_ERROR_PRODCUT_OOAP_NOT_FOUND = 3939,

	// BM 상점 리스토어- 존재하지 않는 아이템
	BM_SHOP_RESTORE_ERROR_ITEM_NOT_FOUND = 3940,

	// BM 상점 리스토어 - 존재하지 않는 상품
	BM_SHOP_RESTORE_ERROR_PRODCUT_NOT_FOUND = 3941,

	// BM 상점 리스토어  - 마켓 상품 오류
	BM_SHOP_RESTORE_ERROR_NOT_HIVE_SYNC_MARKET_PID = 3942,

	// BM 상점 - 게이트웨이 서버에서 유저를 찾을 수 없음
	BM_SHOP_ERROR_GATEWAY_FIND_USER = 3943,

	// BM 상점 - 유저가 보낸 APPID가 유효하지 않다.
	BM_SHOP_ERROR_GATEWAY_INVALID_APPID = 3944,

	// BM 상점 - 현재 PC결제 이용이 불가능합니다.
	BM_SHOP_ERROR_PG_UNLOCK = 3945,

	// BM 상점 - 잠시 후 이용부탁드립니다.(PG 결제 내역 조회)
	BM_SHOP_ERROR_PG_UNCONSUMED_COOLTIME = 3946,

	// 시공의 틈새  - 완료 횟수 부족
	SPACE_CRACK_ERROR_PLAY_LIMIT_COUNT = 4000,

	// 시공의 틈새  - 이미 보상 받은 상태 실패
	SPACE_CRACK_ERROR_REWARD_COMPLETED = 4001,

	// 시공의 틈새   주간 보상 테이블 오류 실패
	SPACE_CRACK_ERROR_REWARD_DATA_ID = 4002,

	// 시공의 틈새   주간 보상 테이블 오류 실패
	SPACE_CRACK_ERROR_REWARD_BOX_DATA_ID = 4003,

	// 시공의 틈새 재입장 시간 불가
	SPACE_CRACK_ERROR_ENTRY_TIME = 4004,

	// MULTI_LEVEL_RANK - 게이트 웨이 작업에 실패하였습니다
	MULTI_LEVEL_RANK_ERROR_GATEWAY_OPERATION_FAILED = 4100,

	// MULTI_LEVEL_RANK - 유효하지 않은 상태
	MULTI_LEVEL_RANK_ERROR_INVALID_STATUS = 4101,

	// MULTI_LEVEL_RANK - 승급 조건이 충분하지 않습니다
	MULTI_LEVEL_RANK_ERROR_PROMOTION_NOT_MEET_THE_CONDITION = 4102,

	// MULTI_LEVEL_RANK - 승급에 실패하였습니다
	MULTI_LEVEL_RANK_ERROR_PROMOTION_PROCESS_FAILED = 4103,

	// MULTI_LEVEL_RANK - 랭크 변경 조건이 충분하지 않습니다
	MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_MEET_THE_CONDITION = 4104,

	// MULTI_LEVEL_RANK - 랭크 변경에 필요한 재화가 부족합니다
	MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_ENOUGH_MONEY = 4105,

	// MULTI_LEVEL_RANK - 임무 수락에 대한 요구 경험치가 부족합니다
	MULTI_LEVEL_RANK_ERROR_ACCEPT_QUEST_NOT_ENOUGH_REQUIRE_EXP = 4106,

	// MULTI_LEVEL_RANK - 이전 랭크를 클리어하지 못했습니다
	MULTI_LEVEL_RANK_ERROR_ACCEPT_BEFORE_RANK_NOT_CLEAR_STATE = 4107,

	// MULTI_LEVEL_RANK - 이미 수행 중인 퀘스트입니다
	MULTI_LEVEL_RANK_ERROR_ACCEPT_ALREADY_ACCEPTED_QUEST = 4108,

	// MULTI_LEVEL_RANK - 보상 조건에 해당하지 않습니다
	MULTI_LEVEL_RANK_ERROR_REWARD_RANK_NOT_MEET_THE_CONDITION = 4109,

	// MULTI_LEVEL_RANK - 이미 보상 받은 다단계 랭크입니다
	MULTI_LEVEL_RANK_ERROR_ALREADY_RECEIVED_REWARD_RANK = 4110,

	// 거래소 - 알 수 없는 에러
	MARKET_ERROR_UNKNOWN = 4200,

	// 거래소 - 게이트 웨이 작업에 실패하였습니다
	MARKET_ERROR_GATEWAY_OPERATION_FAILED = 4201,

	// 거래소 - Gateway 서버와 연결이 없음
	MARKET_ERROR_NO_GATEWAY_SERVER_CONNECTION = 4202,

	// 거래소 - 거래 가능한 아이템이 아닙니다.
	MARKET_ERROR_NOT_TRADABLE_ITEM = 4203,

	// 거래소 - 아이템 등록 개수 초과
	MARKET_ERROR_ITEM_REGISTER_COUNT_OVER = 4204,

	// 거래소 - 아이템 보유 개수 부족
	MARKET_ERROR_NOT_ENOUGH_ITEM_COUNT = 4205,

	// 거래소 - 장착 중 아이템 등록 실패
	MARKET_ERROR_CANT_REGISTER_EQUIPED_ITEM = 4206,

	// 거래소 - 등록 수수료 부족
	MARKET_ERROR_NOT_ENOUGH_REGISTER_FEE = 4207,

	// 거래소 - 유효하지 않은 등록 가격
	MARKET_ERROR_INVALID_REGISTER_PRICE = 4208,

	// 거래소 - 유효하지 않은 거래 항목
	MARKET_ERROR_INVALID_MARKET_TRANSACTION = 4209,

	// 거래소 - 최대 다이아 보유량 초과
	MARKET_ERROR_EXCESS_DIA = 4210,

	// 거래소 - 찾을 수 없는 거래 항목
	MARKET_ERROR_NOT_FOUND_TRANSACTION = 4211,

	// 거래소 - 자신이 판매중인 아이템 구매 오류
	MARKET_ERROR_CANT_PURCHASE_MY_TRANSACTION = 4212,

	// 거래소 - 다이아 부족
	MARKET_ERROR_NOT_ENOUGH_DIA = 4213,

	// 거래소 - 타인이 등록한 아이템 취소 오류
	MARKET_ERROR_CANT_CANCEL_OTHER_TRANSACTION = 4214,

	// 거래소 - 유효하지 않는 거래소 카테고리
	MARKET_ERROR_INVALIDE_MARKET_CATEGORY = 4215,

	// 거래소 - 유효하지 않는 아이템 등급
	MARKET_ERROR_INVALIDE_ITEM_GRADE = 4216,

	// 거래소 - 유효하지 않는 유저
	MARKET_ERROR_INVALIDE_PLAYING_USER = 4217,

	// 거래소 - 유효하지 않는 아이템 아이디
	MARKET_ERROR_INVALIDE_ITEM_ID = 4218,

	// 거래소 - 거래소에 존재하지 않는 아이템
	MARKET_ERROR_NOT_EXIST_ITEM = 4219,

	// 거래소 - 유효하지 않는 아이템 레벨
	MARKET_ERROR_INVALID_ITEM_LEVEL = 4220,

	// 거래소 - 장비 아이템이 아님
	MARKET_ERROR_IS_NOT_EQUIP_ITEM = 4221,

	// 거래소 - 업데이트된 거래 항목이 없음.
	MARKET_ERROR_NO_UPDATED_TRANSACTION = 4222,

	// 거래소 - 즐겨찾기 개수 초과.
	MARKET_ERROR_BOOKMARK_LIMIT_EXCEEDED = 4223,

	// 거래소 - 정산 디비 요청 에러
	MARKET_ERROR_DB_REQUEST_CALCULATE = 4224,

	// 거래소 - 정산 에러
	MARKET_ERROR_CALCULATE = 4225,

	// 거래소 - 정산 디비 요청 에러, 판매완료 되지 않은 거래 항목
	MARKET_ERROR_CALCULATE_NOT_PERCHASED_TRANSACTION = 4226,

	// 거래소 - 정산 디비 요청 에러, 정산 시간 업데이트 에러
	MARKET_ERROR_CALCULATE_TRANSACTION_CACULATE = 4227,

	// 거래소 - 정산 디비 요청 에러, 재화 업데이트 에러
	MARKET_ERROR_CALCULATE_CURRENCY_UPDATE = 4228,

	// 거래소 - 정산 디비 요청 에러, 트랜젝션 커밋 에러
	MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_COMMIT = 4229,

	// 거래소 - 정산 디비 요청 에러, 트랜젝션 롤백 에러
	MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_ROLLBACK = 4230,

	// 거래소 - 아이템 구매 프로세스 에러
	MARKET_ERROR_PURCHASE = 4231,

	// 거래소 - 디비 요청 에러, 아이템 이동 실패 
	MARKET_ERROR_DB_REQUEST_MOVE_ITEM = 4232,

	// 거래소 - 디비 요청 에러, 아이템 인벤토리로 이동 실패 
	MARKET_ERROR_DB_ITEM_MOVE_ON_INVENTORY = 4233,

	// 거래소 - 디비 요청 에러, 아이템 이동 실패(아이템 생성) 
	MARKET_ERROR_DB_MOVE_ITEM_BY_CREATE = 4234,

	// 거래소 - 디비 요청 에러, 아이템 이동 실패(아이템 수량 증가) 
	MARKET_ERROR_DB_MOVE_ITEM_BY_INCREASE = 4235,

	// 거래소 - 디비 요청 에러, 아이템 등록 취소 실패(거래 삭제 실패) 
	MARKET_ERROR_DB_CANCEL_DELETE_TRANSACTION = 4236,

	// 거래소 - 디비 요청 에러, 아이템 구매 실패(찾을 수 없는 거래항목) 
	MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION = 4237,

	// 거래소 - 디비 요청 에러, 아이템 구매 실패(거래 이력 추가) 
	MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_DELETED = 4238,

	// 거래소 - 디비 요청 에러, 아이템 구매 실패(구매 완료 리스트 추가) 
	MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_PURCHASED = 4239,

	// 거래소 - 디비 요청 에러, 아이템 구매 실패(거래소에서 제거) 
	MARKET_ERROR_DB_PURCHASE_DELETE_MARKET = 4240,

	// 거래소 - 디비 요청 에러, 아이템 구매 실패 
	MARKET_ERROR_DB_PURCHASE_DB_OPERATION = 4241,

	// 거래소 - 아이템 등록 취소 프로세스 에러
	MARKET_ERROR_CANCEL = 4242,

	// 거래소 - 디비 요청 에러, 아이템 이동 트랜젝션 커밋 실패
	MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_COMMIT = 4243,

	// 거래소 - 디비 요청 에러, 아이템 이동 트랜젝션 롤백 실패
	MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_ROLLBACK = 4244,

	// 거래소 - GM은 이용 불가합니다.
	MARKET_ERROR_CANNOT_USE_GM = 4245,

	// 출석보상 - Gateway 서버 작업이 실패했습니다
	ATTENDANCE_ERROR_GATEWAY_OPERATION_FAILED = 4300,

	// 출석보상 - Gateway 서버와 연결이 없음
	ATTENDANCE_ERROR_NO_GATEWAY_SERVER_CONNECTION = 4301,

	// 출석보상 - 유효하지 않은 출석부
	ATTENDANCE_ERROR_INVALID_ID = 4302,

	// 출석보상 - 기간 만료
	ATTENDANCE_ERROR_EXPIRED = 4303,

	// 출석보상 - 이미 가지고 있는 출석부
	ATTENDANCE_ERROR_ALREADY_HAVE = 4304,

	// 출석보상 - 소유하지 않은 출석부
	ATTENDANCE_ERROR_NO_HAVE = 4305,

	// 출석보상 - 이미 출석 체크함
	ATTENDANCE_ERROR_ALREADY_CHECK_IN = 4306,

	// 출석보상 - 재화 부족
	ATTENDANCE_ERROR_NOT_ENOUGH_MONEY = 4307,

	// 출석보상 - 아이템 재화 부족
	ATTENDANCE_ERROR_NOT_ENOUGH_ITEM = 4308,

	// 출석보상 - 재화 가득참
	ATTENDANCE_ERROR_OVERFLOW_MAX_MONEY = 4309,

	// 출석보상 - 아직 출석부 오픈하지 않음
	ATTENDANCE_ERROR_NOT_OPEN = 4310,

	// 출석보상 - 출석부 시작 후 유효기간이 지남
	ATTENDANCE_ERROR_RECEIVE_DURATION = 4311,

	// 이벤트 상점 - 게이트 웨이 작업에 실패하였습니다
	NPC_EVENT_SHOP_ERROR_GATEWAY_OPERATION_FAILED = 4400,

	// 이벤트 상점 - 알수 없는 오류
	NPC_EVENT_SHOP_ERROR_UNKNOWN = 4401,

	// 이벤트 상점 - Gateway 서버와 연결이 없습니다
	NPC_EVENT_SHOP_NO_GATEWAY_SERVER_CONNECTION = 4402,

	// 이벤트 상점 - 유효하지 않은 캐릭터
	NPC_EVENT_SHOP_ERROR_INVALID_USER = 4403,

	// 이벤트 상점 - 유효하지 않은 캐릭터 상태
	NPC_EVENT_SHOP_ERROR_INVALID_USER_STATUS = 4404,

	// 이벤트 상점 - 유효하지 않은 레벨
	NPC_EVENT_SHOP_ERROR_NOT_ALLOWED_LEVEL = 4405,

	// 이벤트 상점 - 존재하지 않는 아이템
	NPC_EVENT_SHOP_ERROR_ITEM_NOT_FOUND = 4406,

	// 이벤트 상점 - 유효하지 않은 ItemId
	NPC_EVENT_SHOP_ERROR_INVALID_ITEM_ID = 4407,

	// 이벤트 상점 - 유효하지 않은 NPC
	NPC_EVENT_SHOP_ERROR_INVALID_NPC = 4408,

	// 이벤트 상점 - 다른 채널에 있다
	NPC_EVENT_SHOP_ERROR_DIFFERENT_CHANNEL = 4409,

	// 이벤트 상점 - 재화 부족
	NPC_EVENT_SHOP_ERROR_NOT_ENOUGH_MONEY = 4410,

	// 이벤트 상점 - 재화 가득참
	NPC_EVENT_SHOP_ERROR_OVERFLOW_MAX_MONEY = 4411,

	// 이벤트 상점 - 구입하는 상품이 총 재화 단위가 큽니다 - 오버플로우(uint범위)
	NPC_EVENT_SHOP_ERROR_OVERFLOW_CURRENCY_DELTA = 4412,

	// 이벤트 상점 - 구매 횟수 초과
	NPC_EVENT_SHOP_ERROR_OVER_QUANTIY = 4413,

	// 이벤트 상점 - 길드 등급 제한
	NPC_EVENT_SHOP_ERROR_GUILD_MEMBER_GRADE = 4414,

	// 이벤트 상점 - 가입한 길드가 없다
	NPC_EVENT_SHOP_ERROR_GUILD_ID = 4415,

	// 이벤트 상점 - 길드 레벨 제한
	NPC_EVENT_SHOP_ERROR_GUILD_LEVEL = 4416,

	// 이벤트 상점 - 판매 날짜 아님
	NPC_EVENT_SHOP_ERROR_SALE_DATETIME = 4417,

	// 이벤트 상점 - 구매  실패
	NPC_EVENT_SHOP_ERROR_PURCHASE = 4418,

	// 이벤트 상점 - 재화 아이템과 판매 아이템이 중복됨 관리자문의 
	NPC_EVENT_SHOP_DUPLICATION_COST_ITEM = 4419,

	// 자동 공격을 AUTO 설정후 사용 해주세요
	OFF_LINE_PLAY_AUTO_ATTACK = 4500,

	// 사망 상태에서는 사용할수 없습니다.
	OFF_LINE_PLAY_USER_DEATH = 4501,

	// 오프라인 플레이 만료 시간
	OFF_LINE_PLAY_EXPIRY_TIME = 4502,

	// 해당하는 지역에서는 사용할수 없습니다.
	OFF_LINE_PLAY_AREA = 4503,

	// 오프라인 데이터 로드 실패
	OFF_LINE_LOAD_ERROR = 4504,

	// 유효하지 않은 상태 입니다
	CLIENT_CINEMATIC_ERROR_INVALID_STATUS = 4600,

	// 다른 시네마틱 을 보고있다
	CLIENT_CINEMATIC_ALREADY = 4601,

	// 시네마틱 완료 정보가 유효하지 않습니다
	CLIENT_CINEMATIC_END_ERROR_SEQUENCE_RES_ID = 4602,

	// 시네마틱을 보고있지 않다
	CLIENT_CINEMATIC_ERROR_NOT_PLAYING = 4603,

	// 워프 중에는 시네마틱 진행이 불가능합니다.
	CLIENT_CINEMATIC_ERROR_WARP = 4604,

	// 아이템 제작 - 올바르지 않은 상태입니다.
	CRAFT_ERROR_INVALID_STATUS = 4700,

	// 아이템 제작 - 계정당 제작 제한 갯수를 초과했습니다.
	CRAFT_ERROR_EXCEED_LIMIT_COUNT_ACCOUNT = 4701,

	// 아이템 제작 - 재료가 부족합니다.
	CRAFT_ERROR_LACK_OF_MATERIAL = 4702,

	// 아이템 제작 - 기간이 지난 아이템 제작 요청입니다.
	CRAFT_ERROR_EXPIRE_DATE_ITEM_CRAFT = 4703,

	// 아이템 제작 - 서버당 제작 제한 갯수를 초과했습니다.
	CRAFT_ERROR_EXCEED_LIMIT_COUNT_SERVER = 4704,

	// 아이템 제작 - 언락 조건을 만족하지 못했습니다.
	CRAFT_ERROR_NOT_MEET_THE_UNLOCK_CONDITION = 4705,

	// 아이템 제작 - 최대 제작 수량을 초과했습니다.
	CRAFT_ERROR_EXCEED_MAX_CRAFT_COUNT = 4706,

	// 아이템 제작 - 일부 아이템만 제작되었음.
	CRAFT_ERROR_ONLY_SOME_ITEMS_WERE_CRAFTED = 4707,

	// 길드창고 - 알수없는 에러
	GUILD_STORE_UNKNOWN_ERROR = 4800,

	// 길드창고 - 길드원이 아닙니다.
	GUILD_STORE_ERROR_UNKNOWN_GUILD_ID = 4801,

	// 길드창고 - 분배할 권한이 없습니다.
	GUILD_STORE_ERROR_NOT_HAVE_GRADE_TO_DISTRIBUTE = 4802,

	// 길드창고 - 분배할 멤버가 길드원이 아닙니다.
	GUILD_STORE_ERROR_NOT_GUILD_MEMBER_TO_DIRTIBUTE = 4803,

	// 길드창고 - 분배할 권한이 없습니다.
	GUILD_STORE_ERROR_GRADE = 4804,

	// 길드창고 - 분배할 아이템이 없습니다.
	GUILD_STORE_ERROR_NO_ITEM_TO_DISTRIBUTE = 4805,

	// 길드창고 - 아이템을 분배받을 멤버가 길드에 없습니다.
	GUILD_STORE_ERROR_NO_GUILD_MEMBER_TO_RECEIVE_ITEM = 4806,

	// 길드창고 - 아이템 수량 오류
	GUILD_STORE_ERROR_ITEM_AMOUNT = 4807,

	// 길드창고 - 기간 만료 아이템
	GUILD_STORE_ERROR_EXPIRED_ITEM = 4808,

	// 길드창고 - 잠시 후 다시 시도(두 서버에서 요청이 들어온 경우 하나의 서버에서 온 요청만 처리)
	GUILD_STORE_ERROR_TRY_AGAIN = 4809,

	// 길드창고 - 확장에 필요한 재화 부족
	GUILD_EXTEND_ERROR_NOT_ENOUGH_MONEY = 4810,

	// 길드창고 - 더 이상 슬롯을 확장 할수 없습니다
	GUILD_EXTEND_ERROR_MAX_SLOT = 4811,

	// 길드창고 - 창고가 가득차 더이상 아이템을 획득할 수 없습니다.
	GUILD_EXTEND_ERROR_FULL = 4812,

	// 길드 아이템 컬렉션 - 알 수 없는 에러
	GUILD_ITEM_COLLECTION_ERROR_UNKNOWN = 4900,

	// 길드 아이템 컬렉션 - 이미 등록된 아이템 컬렉션 슬롯입니다.
	GUILD_ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_CONDITION = 4901,

	// 길드 아이템 컬렉션 - 올바르지 않은 조건 아이디입니다.
	GUILD_ITEM_COLLECTION_INVALID_CONDITION_ID = 4902,

	// 길드 아이템 컬렉션 - 최대 등록 횟수를 초과했습니다.
	GUILD_ITEM_COLLECTION_USER_REGISTER_COUNT_LIMIT_OVER = 4903,

	// 길드 아이템 컬렉션 - 이벤트 컬렉션 기간이 아닙니다.
	GUILD_ITEM_COLLECTION_INACTIVE_EVENT_COLLECTION = 4904,

	// 길드 아이템 컬렉션 - 이미 완료한 컬렉션 조건입니다.
	GUILD_ITEM_COLLECTION_ALREADY_CLEAR_CONDITION = 4905,

	// 길드 아이템 컬렉션 - 이미 완료한 컬렉션입니다.
	GUILD_ITEM_COLLECTION_ALREADY_CLEAR_COLLECTION = 4906,

	// 길드 아이템 컬렉션 - 올바르지 않은 아이템 조건입니다.
	GUILD_ITEM_COLLECTION_NOT_SUITABLE_ITEM = 4907,

	// 보물 몬스터 - 안전 지대에서 사용 할수 없습니다.
	TREASURE_MONSTER_ERROR_SAFE_TYTERRITORY = 5000,

	// 보물 몬스터 - 이미 생성된 보물 몬스터 가 존재 합니다.
	TREASURE_MONSTER_ERROR_ALREADY_CREATED = 5001,

	// 보물 몬스터 - 보물 몬스터의 알을 사용할 수 없는 지역입니다
	TREASURE_MONSTER_ERROR_INVALID_AREA = 5002,

	// 업적 - 알 수 없는 에러입니다.
	ACHIEVEMENT_ERROR_UNKNOWN = 5100,

	// 업적 - 업적 정보를 찾을 수 없습니다.
	ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_ID = 5101,

	// 업적 - 업적 테마 정보를 찾을 수 없습니다.
	ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_THEME_ID = 5102,

	// 업적 - 업적 보상을 받을 수 없는 조건입니다.
	ACHIEVEMENT_ERROR_NOT_MEET_THE_REWARD_CONDITION = 5103,

	// 몬스터 도감 - 유효하지 않은 캐릭터.(IsSpawned false)
	MONSTER_KNOWLEDGE_ERROR_INVALID_USER = 5200,

	// 몬스터 도감 - 레벨업을 할 수 없는 몬스터 지식 입니다.
	MONSTER_KNOWLEDGE_ERROR_CANT_LEVEL_UP_KNOWLEDGE = 5201,

	// 몬스터 도감 - 유효하지 않은 아이템 사용 개수 입니다.
	MONSTER_KNOWLEDGE_ERROR_INVALID_ITEM_AMOUNT = 5202,

	// 몬스터 도감 - 몬스터 지식 획득 아이템이 아닙니다.
	MONSTER_KNOWLEDGE_ERROR_NOT_GAIN_EXP_ITEM = 5203,

	// 몬스터 도감 - 레벨업 가능한 몬스터 지식이 없습니다.
	MONSTER_KNOWLEDGE_ERROR_NO_KNOWLEDGE_LEVEL_UP_AVAILABLE = 5204,

	// 몬스터 도감 - 지식의 석판 슬롯을 활성화 할 수 없습니다..
	MONSTER_KNOWLEDGE_ERROR_CANT_SLATE_SLOT_ACTIVATE = 5205,

	// 몬스터 도감 - 지식의 석판 슬롯 초기화 아이템이 아닙니다.
	MONSTER_KNOWLEDGE_ERROR_NOT_RESET_SLATE_SLOT_ITEM = 5206,

	// 몬스터 도감 - 디비 요청에 실패 했습니다.
	MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE = 5207,

	// 몬스터 도감 - 도감 포인트 초과로 인한 디비 요청에 실패 했습니다.
	MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE_EXCEEDED_CURRENCY = 5208,

	// 몬스터 도감 - 지식 경험치 획득 아이템 효과 데이터 오류
	MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_EFFECT_PARAM = 5209,

	// 몬스터 도감 - 지식 경험치 획득 아이템 템플릿 데이터 오류
	MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_TEMPLATE = 5210,

	// 몬스터 도감 - 지식의 석판 슬롯 활성화 비용 데이터가 유효하지 않습니다.
	MONSTER_KNOWLEDGE_ERROR_SLATE_SLOT_OPEN_COST = 5211,

	// 몬스터 도감 - 도감 포인트가 부족 합니다.
	MONSTER_KNOWLEDGE_ERROR_NOT_ENOUGHT_MONSTER_BOOK_POINT = 5212,

	// 몬스터 도감 - 지식의 석판 해금 아이템 효과 데이터 오류가 발생 했습니다.
	MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_EFFECT_PARAM = 5213,

	// 몬스터 도감 - 지식의 석판 해금 아이템 템플릿 데이터 오류가 발생 했습니다.
	MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_TEMPLATE = 5214,

	// 몬스터 도감 - 이미 오픈된 지식의 석판 입니다.
	MONSTER_KNOWLEDGE_ERROR_ALREADY_UNLOCK_SLATE = 5215,

	// 몬스터 도감 - 초기화 할 석판이 없습니다.
	MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_RESET_SLATE = 5216,

	// 몬스터 도감 - 몬스터 지식이 이미 최대 레벨입니다.
	MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_LEVEL = 5217,

	// 몬스터 도감 - 몬스터 지식 경험치가 최대 입니다.
	MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_EXP = 5218,

	// 몬스터 도감 - 존재하지 않는 몬스터 지식 입니다.
	MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_KNOWLEDGE = 5219,

	// 몬스터 도감 - 해금되지 않은 지식의 석판 입니다.
	MONSTER_KNOWLEDGE_ERROR_LOCKED_SLATE = 5220,

	// 몬스터 도감 - 유효하지 않는 몬스터 최대 경험치.
	MONSTER_KNOWLEDGE_ERROR_INVALID_KNOWLEDGE_MAX_EXP = 5221,

	// 몬스터 도감 - 지식의 석판 특수 노드 개방 실패.
	SLATE_PROBABILITY_FAIL_TO_ACTIVATE_SPECIAL_NODE = 5222,

	// 몬스터 도감 - 유효하지 않는 석판 아이디
	SLATE_ERROR_INVALID_SLATE_ID = 5223,

	// 몬스터 도감 - 유효하지 않는 특수 노드 아이디
	SLATE_ERROR_INVALID_SPECIAL_NODE_ID = 5224,

	// 몬스터 도감 - 유효하지 않는 유저
	SLATE_ERROR_INVALID_USER = 5225,

	// 몬스터 도감 - 석판이 잠겨있기 때문에 특수 노드를 활성화 할 수 없습니다.
	SLATE_ERROR_LOCKED_SLATE = 5226,

	// 몬스터 도감 - 더이상 특수 노드를 활성화 할 수 없습니다.
	SLATE_ERROR_CANT_ACTIVATE_SPECIAL_NODE_ANYMORE = 5227,

	// 몬스터 도감 - 석판 DB 요청 실패
	SLATE_ERROR_DB_REQUEST_FAILURE = 5228,

	// 몬스터 도감 - 유효하지 않은 유저.
	CODEX_ERROR_INVALID_USER = 5229,

	// 몬스터 도감 - 유효하지 않은 대륙 아이디.
	CODEX_ERROR_INVALID_CATEGORY_ID = 5230,

	// 몬스터 도감 - 몬스터 지식 부족
	CODEX_ERROR_NOT_ENOUGTH_MONSTER_KNOWLEDGE = 5231,

	// 몬스터 도감 - 유효하지 않은 도감 정보
	CODEX_ERROR_INVALID_CATEGORY_BALANCE_ID = 5232,

	// 몬스터 도감 - 맵 그룹 레벨 횟수 부족
	CODEX_ERROR_NOT_ENOUGH_MAP_GROUP_LEVEL_UP_COUNT = 5233,

	// 몬스터 도감 - 도감 포인트 변환 실패(도감 포인트 최대)
	CODEX_CANT_EXCHANGE_ANYMORE_MAX_POINT = 5234,

	// 몬스터 도감 - 대륙 레벨업 실패(데이터 오류)
	CODEX_ERROR_CATEGORY_LEVEL_UP_ZERO = 5235,

	// 몬스터 도감 - 대륙 레벨업 실패(레벨업 조건 불충족)
	CODEX_ERROR_CATEGORY_LEVEL_UP_SAME_LEVEL = 5236,

	// 몬스터 도감 - 요청 데이터 오류(아이템 수량 부족)
	CODEX_ERROR_REQUEST_ITEM_AMOUNT_NOT_ENOUGH = 5237,

	// 몬스터 도감 - 잘목된 아이템 사용
	CODEX_ERROR_NOT_GAIN_EXP_ITEM = 5238,

	// 몬스터 도감 - 잘못된 맵 그룹 아이디
	CODEX_ERROR_INVALID_MAP_GROUP_ID = 5239,

	// 몬스터 도감 - 재료 아이템이 아님
	CODEX_ERROR_NOT_INGREDIENT_ITEM = 5240,

	// 몬스터 도감 - 맵 그룹 최대 경험치 도달
	CODEX_ERROR_MAP_GROUP_MAX_EXP = 5241,

	// 몬스터 도감 - 도감 포인트 최대 도달
	CODEX_ERROR_MAX_MONSTER_BOOK_POINT = 5242,

	// 몬스터 도감 - 도감 DB요청 실패
	CODEX_ERROR_DB_REQUEST_FAILURE = 5243,

	// 해당하는 레벨 데이터가 존재 하지 않음
	DEATH_PENALTY_ERROR_USER_LEVEL_DATA_TABLE = 5300,

	// 부활 정보가 존재 하지 않음
	DEATH_ENALTY_ERROR_RESURREC_DATA_TABLE = 5301,

	// 재연결 실패 - 알 수 없는 에러
	RECONNECT_ERROR_UNKNOWN = 5400,

	// 재연결 실패 - 유효하지 않은 ID
	RECONNECT_ERROR_INVALID_ACCOUNT_ID = 5401,

	// 재연결 실패 - 유효하지 않은 상태
	RECONNECT_ERROR_INVALID_STATE = 5402,

	// 재연결 실패 - 유효하지 않은 토큰
	RECONNECT_ERROR_INCORRECT_TOKEN = 5403,

	// 재연결 실패 - 기존 접속 끊기 중. 잠시 후 다시 시도 하시오
	RECONNECT_ERROR_NOT_DISCONNECTED_YET = 5404,

	// 캐릭터 생성 실패 - 알 수 없는 에러
	CREATE_USER_ERROR_UNKNOWN = 5500,

	// 캐릭터 생성 실패 - 서버 에러
	CREATE_USER_ERROR_INTERNAL_ERROR = 5501,

	// 캐릭터 생성 실패 - DB 에러
	CREATE_USER_ERROR_DB_OPERATION_ERROR = 5502,

	// 캐릭터 생성 실패 - 이미 캐릭터 생성 또는 삭제 중
	CREATE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER = 5503,

	// 캐릭터 생성 실패 - 이미 존재하는 이름
	CREATE_USER_ERROR_ALREADY_EXIST_NAME = 5504,

	// 캐릭터 생성 실패 - 유효하지 않은 이름 길이
	CREATE_USER_ERROR_INVALID_LENGTH_NAME = 5505,

	// 캐릭터 생성 실패 - 허가되지 않은 이름
	CREATE_USER_ERROR_NOT_ALLOWED_NAME = 5506,

	// 캐릭터 생성 실패 - 허가되지 않은 성별
	CREATE_USER_ERROR_INVALID_GENDER_TYPE = 5507,

	// 캐릭터 생성 실패 - 허가되지 않은 무기 타입
	CREATE_USER_ERROR_INVALID_WEAPON_TYPE = 5508,

	// 캐릭터 생성 실패 - 초기 아이템 지급 오류
	CREATE_USER_ERROR_PUSH_INITIAL_ITEM = 5509,

	// 캐릭터 생성 실패 - 스킬 셋팅 오류
	CREATE_USER_ERROR_SET_INITIAL_SKILL = 5510,

	// 캐릭터 생성 실패 - 페어리 셋팅 오류
	CREATE_USER_ERROR_SET_INITIAL_FAIRY = 5511,

	// 캐릭터 생성 실패 - 생성 개수를 초과함
	CREATE_USER_ERROR_NO_MORE_USER_SOCKET = 5512,

	// 캐릭터 생성 실패 - 초기 재화 지급 오류
	CREATE_USER_ERROR_PUSH_INITIAL_CURRENCY = 5513,

	// 캐릭터 생성 실패 - 코스튬 셋팅 오류
	CREATE_USER_ERROR_SET_INITIAL_COSTUME = 5514,

	// 캐릭터 생성 실패 - 초기 우편 지급 에러
	CREATE_USER_ERROR_ADD_INITIAL_MAIL = 5515,

	// 캐릭터 생성 실패 - 신규 유저 출석부 지급 에러
	CREATE_USER_ERROR_SET_NEW_USER_ATTENDANCE = 5516,

	// 캐릭터 생성 실패 - 신규 유저 커뮤니티이벤트미션 지급 에러
	CREATE_USER_ERROR_SET_NEW_USER_COMMUNITY_EVENT_MISSION = 5517,

	// 캐릭터 생성 실패 - 무료 사망 패널티 갯수
	CREATE_USER_ERROR_PENALTY_FREE = 5518,

	// 캐릭터 생성 실패 - ARCAS
	CREATE_USER_ERROR_ARCAS = 5519,

	// 캐릭터 생성 실패 - Quest_main
	CREATE_USER_ERROR_QUEST_MAIN = 5520,

	// 캐릭터 생성 실패 - 전투 타겟 옵션 실패
	CREATE_USER_ERROR_COMBAT_OPTION = 5521,

	// 오프라인 데이터 생성 실패.
	CREATE_USER_ERROR_OFF_LINE = 5522,

	// 캐릭터 생성 실패 - DB error set issued user id
	CREATE_USER_ERROR_SET_ISSUED_USER_ID = 5523,

	// 캐릭터 생성 실패 - DB error insert user
	CREATE_USER_ERROR_INSERT_USER = 5524,

	// 캐릭터 생성 실패 - DB error insert user row count 0
	CREATE_USER_ERROR_INSERT_USER_ROW_COUNT = 5525,

	// 캐릭터 생성 실패 - error call EndTransaction()
	CREATE_USER_ERROR_CALL_END_TRANSACTION_COMMIT = 5526,

	// 캐릭터 생성 실패 - error call EndTransaction()
	CREATE_USER_ERROR_CALL_END_TRANSACTION_ROLLBACK = 5527,

	// 캐릭터 생성 실패 - 소셜 액션 에러
	CREATE_USER_ERROR_SET_NEW_USER_SOCIAL_ACTON = 5528,

	// 캐릭터 생성 실패 - 서버 캐릭터 생성수 제한
	CREATE_USER_ERROR_CREATION_LIMIT = 5529,

	// 캐릭터 삭제 실패 - 알 수 없는 에러
	DELETE_USER_ERROR_UNKNOWN = 5600,

	// 캐릭터 삭제 실패 - 이미 캐릭터 생성 또는 삭제 중
	DELETE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER = 5601,

	// 캐릭터 삭제 실패 - 유효하지 않은 UserDBId
	DELETE_USER_ERROR_INVALID_USER_DB_ID = 5602,

	// 캐릭터 삭제 실패 - DB 에러
	DELETE_USER_ERROR_DB_OPERATION_ERROR = 5603,

	// 캐릭터 삭제 실패 - 기사단 멤버는 유저 삭제 불가
	DELETE_USER_ERROR_GUILD_MEMBER_CANNOT = 5604,

	// 캐릭터 삭제 실패 - 레벨이 부족하여 유저 삭제 불가
	DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL = 5605,

	// 캐릭터 삭제 실패 - 거래소 판매 및 정산 목록 확인
	DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION = 5606,

	// 캐릭터 삭제 실패 - 월드 거래소 판매 및 정산 목록 확인
	DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION = 5607,

	// 캐릭터 선택 실패 - 알 수 없는 에러
	SELECT_USER_ERROR_UNKNOWN = 5700,

	// 캐릭터 선택 실패 - 월드 서버 다운
	SELECT_USER_ERROR_WORLD_SERVER_DOWN = 5701,

	// 캐릭터 선택 실패 - 이름을 변경해야 함
	SELECT_USER_ERROR_MUST_CHANGE_USER_NAME = 5702,

	// 캐릭터 선택 실패 - 월드 퇴장 중
	SELECT_USER_ERROR_LEAVING_WORLD = 5703,

	// 캐릭터 선택 실패 - 유효하지 않은 로그인 상태
	SELECT_USER_ERROR_INVALID_LOGIN_STATE = 5704,

	// 캐릭터 선택 실패 - 차단 유저
	SELECT_USER_ERROR_BLOCK_USER = 5705,

	// 로그아웃 실패 - 알 수 없는 에러
	LOGOUT_ERROR_UNKNOWN = 5800,

	// 로그아웃 실패 - 전투 상태에서는 로그아웃 할 수 없음
	LOGOUT_ERROR_COMBAT_STATE = 5801,

	// 친구 - 알 수 없는 에러
	BUDDY_ERROR_UNKNOWN = 5900,

	// 친구 - 유효하지 않는 이름 길이
	BUDDY_ERROR_INVALID_NAME_LENGTH = 5901,

	// 친구 - 자신을 요청
	BUDDY_ERROR_REQUEST_MY_SELF = 5902,

	// 친구 - 찾을 수 없는 유저
	BUDDY_ERROR_NOT_FOUND_USER = 5903,

	// 친구 - 유효하지 않은 User ID
	BUDDY_ERROR_INVALID_USER_ID = 5904,

	// 친구 - DB 에러
	BUDDY_ERROR_DB_OPERATION_ERROR = 5905,

	// 친구 - 유효하지 않은 초대
	BUDDY_ERROR_INVALID_INVITATION = 5906,

	// 친구 초대/수락 실패 - 이미 등록된 친구
	BUDDY_ERROR_ALREADY_BUDDY = 5907,

	// 친구 초대/수락 실패 - 최대 친구 수를 초과함
	BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY = 5908,

	// 친구 초대 실패 - 송신자가 수진자를 이미 초대함
	BUDDY_ERROR_ALREADY_INVITATION_SENT = 5909,

	// 친구 초대 실패 - 수신자가 송신자를 이미 초대함
	BUDDY_ERROR_ALREADY_INVITATION_RECV = 5910,

	// 친구 초대 실패 - 송신 자의 최대 친구 수를 초과함
	BUDDY_ERROR_SEND_INVITATION_FULL = 5911,

	// 친구 수락 실패 - 최대 친구 수를 초과함
	BUDDY_ERROR_REACHED_MAX_BUDDY = 5912,

	// 친구 삭제 실패 - 친구가 아님
	BUDDY_ERROR_NOT_BUDDY = 5913,

	// 초대 삭제 실패 - 
	BUDDY_ERROR_DELETE_SEND_INVITATION = 5914,

	// 초대 거절 실패
	BUDDY_ERROR_DELETE_RECV_INVITATION = 5915,

	// 친구 초대 실패 - 수신자의 최대 초대 받기 수를 초과함
	BUDDY_ERROR_TARGET_RECV_INVITATION_FULL = 5916,

	// 친구 탭 레드닷 제거 실패 - 유효하지 않은 탭 타입
	BUDDY_ERROR_INVALID_BUDDY_TAB_TYPE = 5917,

	// 친구 디비 추가 실패 - DB Insert Error
	BUDDY_ERROR_INSERT_BUDDY_TO_ME = 5918,

	// 친구 디비 추가 실패 - DB Insert Error
	BUDDY_ERROR_INSERT_ME_TO_BUDDY = 5919,

	// 친구 요청 디비 삭제 실패 - DB Delete Error
	BUDDY_ERROR_DELETE_REQUEST_INVITATION = 5920,

	// 친구 초대 요청 디비 추가 실패 - DB Insert Error
	BUDDY_ERROR_INSERT_REQUEST_INVITATION = 5921,

	// 동일 계정 친구 초대 실패
	BUDDY_ERROR_INVALID_INVITATION_SAME_ACCOUNT = 5922,

	// 캐싱 실패
	BUDDY_ERROR_CACHE_MISSING = 5923,

	// 월드 서버 정보 에러
	BUDDY_ERROR_WORLD_SERVER_ITEM = 5924,

	// 채팅 차단 추가 - DB 추가 실패
	CHAT_BLACKLIST_ADD_ERROR_DB = 6000,

	// 채팅 차단 - 더 이상 추가 할수가 없다
	CHAT_BLACKLIST_ADD_ERROR_OVER = 6001,

	// 채팅 차단 - 잘못 된 유저정보
	CHAT_BLACKLIST_ADD_ERROR_UNKNOWN_USER = 6002,

	// 채팅 차단 - 이미 등록된 유저다
	CHAT_BLACKLIST_ADD_ERROR_DUPLICATION = 6003,

	// 채팅 해제 - DB 추가 실패
	CHAT_BLACKLIST_REMOVE_ERROR_DB = 6004,

	// 채팅 신고 실패 - DB 추가 실패
	REPORT_BAD_CHAT_ERROR_DB = 6100,

	// 채팅 신고 실패 - 신고 횟수 초과
	REPORT_BAD_CHAT_ERROR_OVER = 6101,

	// 채팅 신고 실패 - 잘못 된 유저정보
	REPORT_BAD_CHAT_ERROR_UNKNOWN_USER = 6102,

	// QUEST - 기사단 퀘스트가 존재하지 않습니다.
	QUEST_ERROR_NOT_EXIST_GUILD_QUEST = 6200,

	// QUEST - 기사단 퀘스트 고유 번호가 유효하지 않습니다.
	QUEST_ERROR_INVALID_GUILD_QUEST_DB_ID = 6201,

	// QUEST - 기사단 퀘스트 DB 요청에 문제가 발생 했습니다
	QUEST_ERROR_GUILD_QUEST_DB_OPERATION = 6202,
	REGISTER_ERROR_UNKNOWN = 6300,
	REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID = 6301,

	// 하이브 영수증 검증 - 설정값 오류
	HIVE_RECEIPT_VERIFY_ERROR_SETTING = 6400,

	// 하이브 영수증 검증 - 호스트 설정값 오류
	HIVE_RECEIPT_VERIFY_ERROR_HOST = 6401,

	// 하이브 영수증 검증 - 검증 실패
	HIVE_RECEIPT_VERIFY_ERROR = 6402,

	// 하이브 영수증 검증 - 전송된 파라미터가 없을 때(1000001)
	HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER = 6403,

	// 하이브 영수증 검증 - DB Connection이 안 될 때(1000003)
	HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION = 6404,

	// 하이브 영수증 검증 - 내부 서버 에러(1000005)
	HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER = 6405,

	// 하이브 영수증 검증 - 필수 파라미터 값이 없을 때(1000006)
	HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER = 6406,

	// 하이브 영수증 검증 - 영수증 검증 실패 또는 해킹된 영수증일 때(1000503)
	HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE = 6407,

	// 하이브 영수증 검증 - 구글 영수증 검증을 위한 licence key가 등록되지 않았을 때(1000504)
	HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED = 6408,

	// 하이브 영수증 검증 - 원스토어 영수증 검증을 위한 market appid가 등록되지 않았을 때(1000505)
	HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED = 6409,

	// 하이브 영수증 검증 - 마켓 영수증 검증 서버에 이상이 있을 경우(1000516)
	HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL = 6410,

	// 하이브 영수증 검증 - 느린 결제(예. 편의점 결제) 시도 시 결제 대기(Pending) 상태로 전송되는 영수증. 결제 전 영수증이기 때문에 어떠한 처리도 하지 않음(1000519)
	HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE = 6411,

	// 하이브 영수증 검증 - 검증은 성공했으나 구매취소상태. 마켓이 애플 앱스토어나 원스토어일 경우에만 취소 상태 판별 가능
	HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE = 6412,

	// 하이브 영수증 검증 - 서버 연결 에러
	HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT = 6413,

	// 하이브 영수증 검증 - 파싱 에러
	HIVE_ERROR_RECEIPT_VERIFY_PARSE = 6414,

	// 하이브 영수증 검증 - result 없음
	HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT = 6415,

	// 하이브 영수증 검증 - 검증은 성공했으나 hiveiap_transaction_id 없음
	HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID = 6416,

	// 하이브 영수증 검증 - 알 수 없는 오류(지정된 결과 코드가 아님)
	HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN = 6417,

	// 하이브 영수증 검증 - PayLoad 값이 UserDBId 값과 다르다
	HIVE_ERROR_RECEIPT_DIFF_PAYLOAD = 6418,

	// 하이브 영수증 검증 - PayLoad 값이 없다
	HIVE_ERROR_RECEIPT_PAYLOAD_NONE = 6419,

	// 하이브 아이템 지급 결과 전송 - 설정값 오류
	HIVE_ERROR_IAP_ITEM_RESULT_SETTING = 6500,

	// 하이브 아이템 지급 결과 전송 - 호스트 설정값 오류
	HIVE_ERROR_IAP_ITEM_RESULT_HOST = 6501,

	// 하이브 아이템 지급 결과 전송 - 실패
	HIVE_ERROR_IAP_ITEM_RESULT = 6502,

	// 하이브 아이템 지급 결과 전송 - 서버 연결 에러
	HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT = 6503,

	// 하이브 아이템 지급 결과 전송 - 파싱 에러
	HIVE_ERROR_IAP_ITEM_RESULT_PARSE = 6504,

	// 하이브 아이템 지급 결과 전송 - RESULT 없음
	HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT = 6505,

	// 하이브 아이템 지급 결과 전송 - 알 수 없는 오류(지정된 결과 코드가 아님)
	HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN = 6506,

	// 하이브 타임존 결과 전송 - 실패
	HIVE_ERROR_TIME_ZONE_RESULT_FAIL = 6600,

	// 하이브 타임존 결과 전송 - 알 수 없는 오류(지정된 결과 코드가 아님)
	HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN = 6601,

	// 하이브 타임존 결과 전송 - 타임존 결과 없음
	HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO = 6602,

	// 가방 확장 에러 - 알 수 없는 에러
	INVENTORY_EXTEND_ERROR_UNKNOWN = 6700,

	// 가방 확장 에러 - 더 이상 확장 할수 없습니다
	INVENTORY_EXTEND_ERROR_MAX_SLOT = 6701,

	// 가방 확장 에러 - 재화가 모자랍니다
	INVENTORY_EXTEND_ERROR_NOT_ENOUGH_MONEY = 6702,

	// 유저 존재 확인 에러 - 유저의 이름이 비었습니다
	CHECK_USER_EXIST_ERROR_USER_NAME_EMPTY = 6800,

	// 유저 존재 확인 에러 - 유저의 이름이 길이가 유효하지 않습니다
	CHECK_USER_EXIST_ERROR_USER_NAME_LENGTH = 6801,

	// 유저 존재 확인 에러 - 유저가 존재하지 않습니다
	CHECK_USER_EXIST_ERROR_USER_NOT_EXIST = 6802,

	// 유저 존재 확인 에러 - 유저가 로그인 상태가 아닙니다
	CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD = 6803,

	// 칭호 장착 에러 - 잘못된 슬롯입니다.
	TITLE_ERROR_INVALID_EQUIP_SLOT = 7000,

	// 칭호 장착 에러 - 잘못된 칭호입니다.
	TITLE_ERROR_INVALID_TITLE_ID = 7001,

	// 칭호 장착 에러 - 활성화되지 않은 칭호입니다.
	TITLE_ERROR_NOT_ACTIVED_TITLE = 7002,

	// 칭호 장착 에러 - 알 수 없는 에러
	TITLE_ERROR_UNKNOWN = 7003,

	// 언락 정보 등록 에러 - DB저장에러
	REGISTER_USER_UNLOCKINFO_ERROR_UNKNOWN = 7004,

	// 통계 에러 - 알 수 없는 에러입니다.
	STATISTICS_ERROR_UNKNOWN = 7100,

	// 통계 에러 - 유효하지 않은 상태입니다.
	STATISTICS_ERROR_INVALID_STATUS = 7101,

	// 사용하지 않음 나중에 봐서 제거
	CAMP_ERROR_INVALID = 7200,

	// 권한이 없습니다.
	CAMP_ERROR_GUILD_GRADE = 7201,

	// 기사단 건물에 캠프가 없습니다.
	CAMP_ERROR_GUILD_BUILDING = 7202,

	// 안전 지역에서 사용할수 없습니다.
	CAMP_ERROR_SAFE_TYPE_TERRITORY = 7203,

	// 해당하는 지역에서는 맵에서는  할수 없습니다.
	CAMP_ERROR_INVALID_MAP = 7204,

	// 해당하는 지역에서는 설치 할수 없습니다.
	CAMP_ERROR_INVALID_AREA = 7205,

	// 아이템 데이터 관련 오류
	CAMP_ERROR_INVALID_ITEM = 7206,

	// 캠프 NPC 가 유효 하지 않습니다.
	CAMP_ERROR_INVALID_NPC = 7207,

	// 캠프 설치 되어 있습니다.
	CAMP_ERROR_SPAWN_CAMP = 7208,

	// 캠프 건설 참여할수 없습니다.
	CAMP_ERROR_UNABLE_INTERACTION = 7209,

	// 캠프 건설 참여할수 없습니다.
	CAMP_ERROR_NEAR_CAMPS_EXIST = 7210,

	// 캠프 파괴중입니다. (연출시간때문에 필요)
	CAMP_ERROR_CAMP_DESTROYING = 7211,

	// 주변에 다른 캠프가 설치가 되어 있어서 오류 발생
	CAMP_ERROR_INSTALLATION_LIMIT_DISTANCE = 7212,

	// 이미 캠프가 설치 되어 있는 경우
	CAMP_ERROR_ALREADY_INSTALLED = 7213,

	// 캠프 침공에서는 사용할수 없습니다.
	CAMP_ERROR_INVADE_WORLD = 7214,

	// 운영 권한이 없어 해체할 수 없습니다
	CAMP_ERROR_UNAUTHORIZED = 7215,

	// 권한이 없습니다.
	SANCTUM_ERROR_GUILD_GRADE = 7216,

	// 기사단 건물에 성소 없습니다.
	SANCTUM_ERROR_GUILD_BUILDING = 7217,

	// 해당하는 지역에서는 맵에서는  할수 없습니다.
	SANCTUM_ERROR_INVALID_MAP = 7218,

	// 해당하는 지역에서는 설치 할수 없습니다.
	SANCTUM_ERROR_INVALID_AREA = 7219,

	// 성소 NPC 가 유효 하지 않습니다.
	SANCTUM_ERROR_INVALID_NPC = 7220,

	// 성소 설치 되어 있습니다.
	SANCTUM_ERROR_SPAWN_SANCTUM = 7221,

	// 성소 건설 참여할수 없습니다.
	SANCTUM_ERROR_UNABLE_INTERACTION = 7222,

	// 성소 건설 참여할수 없습니다.
	SANCTUM_ERROR_NEAR_SANCTUMS_EXIST = 7223,

	// 성소 파괴중입니다. (연출시간때문에 필요)
	SANCTUM_ERROR_SANCTUM_DESTROYING = 7224,

	// 성소 침공에서는 사용할수 없습니다.
	SANCTUM_ERROR_INVADE_WORLD = 7225,

	// 운영 권한이 없어 해체할 수 없습니다
	SANCTUM_ERROR_UNAUTHORIZED = 7226,

	// 이미 다른 기사단의 성소가 설치됨
	SANCTUM_ERROR_ALREADY_INSTALLED = 7227,

	// 성소 쟁탈 시간이  아직 아니다
	SANCTUM_ERROR_BATTLE_BEGIN = 7228,

	// 다른 장소에 성소 넥서스가 설치되어있다
	SANCTUM_ERROR_EXIST_NEXUS = 7229,

	// 프로필 에러 - 알 수 없는 에러입니다.
	PROFILE_ERROR_UNKNOWN = 7300,

	// 프로필 에러 - 유효하지 않은 길이
	PROFILE_ERROR_INVALID_LENGTH_TEXT = 7301,

	// 프로필 에러 - 프로필 소개 DB 작업 실패
	PROFILE_ERROR_INTRO_DB_FAIL = 7302,

	// 프로필 에러 - 허용되지 않은 텍스트
	PROFILE_ERROR_NOT_ALLOWED_TEXT = 7303,

	// 프로필 에러 - 존재하지 않는 유저입니다.
	PROFILE_ERROR_INVALID_USER = 7304,

	// 프로필 에러 - 데이터 캐시 실패
	PROFILE_ERROR_DATA_CACHE_FAIL = 7305,

	// 프로필 에러 - 한마디 DB 작업 실패
	PROFILE_ERROR_COMMENT_DB_FAIL = 7306,

	// 프로필 에러 - 삭제된 한마디입니다.
	PROFILE_ERROR_COMMENT_DELETED = 7307,

	// 프로필 에러 - 작성 가능한 개수를 초과했습니다.
	PROFILE_ERROR_COMMENT_FULL = 7308,

	// 프로필 에러 - 자신의 프로필 한마디 작성이 불가능합니다.
	PROFILE_ERROR_COMMENT_INPUT_SELF = 7309,

	// 프로필 에러 - 다른 작성자의 한마디입니다.
	PROFILE_ERROR_COMMENT_DELETE_INVALID = 7310,

	// 프로필 에러 - 존재하지 않는 키워드입니다.
	PROFILE_ERROR_INVALID_KEYWORD = 7311,

	// 침공 에러 - 알 수 없는 에러입니다.
	INVADE_ERROR_UNKNOWN = 7400,

	// 침공 에러 - 침공 가능한 시간이 아닙니다.
	INVADE_ERROR_INVALID_TIME = 7401,

	// 침공 에러 - 더 이상 침공 포탈을 생성할 수 없습니다.
	INVADE_ERROR_PORTAL_MAX_COUNT = 7402,

	// 침공 에러 - 침공 가능 레벨 조건에 맞지 않아 진입이 불가능 합니다.
	INVADE_ERROR_INVALID_LEVEL = 7403,

	// 침공 에러 - 침공 가능 인원을 초과하여 더 이상 진입할 수 없습니다.
	INVADE_ERROR_ENTER_MAX_COUNT = 7404,

	// 침공 에러 - 침공 체류 시간이 부족합니다.
	INVADE_ERROR_NOT_ENOUGH_TIME = 7405,

	// 침공 에러 - 침공 유저 데이터 로드
	INVADE_ERROR_INVALID_LOAD_USER_DATA = 7406,

	// 침공 에러 - 침공 유저 데이터 저장
	INVADE_ERROR_INVALID_SAVE_USER_DATA = 7407,

	// 침공 에러 - 침공 맵 데이터 에러
	INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA = 7408,

	// 침공 에러 - 침공중에는 사용할 수 없는 기능 사용
	INVADE_ERROR_UNLOCK = 7409,

	// 침공 에러 - 침공을 할 수 없는 맵
	INVADE_ERROR_INVALID_MAP = 7410,

	// 유저 컨텍스트 UI 에러 - 존재하지 않는 유저입니다.
	USER_CONTEXT_UI_ERROR_INVALID_PROFILE_USER = 7500,

	// 유저 컨텍스트 UI 에러 - GM 유저입니다.
	USER_CONTEXT_UI_ERROR_IS_GM_USER = 7501,

	// Hive IAP v4 PG 결제 - 설정값 오류
	HIVE_ERROR_IAP_UNCONSUMED_SETTING = 7550,

	// Hive IAP v4 PG 결제 - HOST 설정 오류
	HIVE_ERROR_IAP_UNCONSUMED_HOST_SETTING = 7551,

	// Hive IAP v4 PG 결제 - APP ID 값이 없는 경우
	HIVE_ERROR_IAP_UNCONSUMED_NO_APP_ID = 7552,

	// Hive IAP v4 PG 결제 - 연결 불가
	HIVE_ERROR_IAP_UNCONSUMED_UNABLE_TO_CONNECT = 7553,

	// 하이브 IAP v4 PG 결제 - 파싱 에러
	HIVE_ERROR_IAP_UNCONSUMED_PARSE = 7554,

	// 하이브 IAP v4 PG 결제 - result 없음
	HIVE_ERROR_IAP_UNCONSUMED_NO_RESULT = 7555,

	// Hive IAP v4 PG 결제 - 응답 코드(연결은 되었으나 Response-result값이 0이 아닌 경우)
	HIVE_ERROR_IAP_UNCONSUMED_RESULT_ERROR = 7556,

	// Hive IAP v4 PG 결제 - 필수파라미터 값이 없는 경우
	HIVE_ERROR_IAP_UNCONSUMED_NO_REQUIRED_PARAMETER = 7557,

	// 튜토리얼 DB 작업 실패
	TUTORIAL_ERROR_INTERNAL_DB_ERROR = 7600,

	// 잘못된 튜토리얼 ID
	TUTORIAL_ERROR_INVALID_TUTORIAL_ID = 7601,

	// 파티던전 에러 - 알 수 없는 에러입니다.
	PARTY_DUNGEON_ERROR_UNKNOWN = 7700,

	// 파티던전 에러 - 파티던전 가능 인원을 초과하여 더 이상 진입할 수 없습니다.
	PARTY_DUNGEON_ERROR_ENTER_MAX_COUNT = 7701,

	// 파티던전 에러 - 파티던전 유저 데이터 로드
	PARTY_DUNGEON_ERROR_INVALID_LOAD_USER_DATA = 7702,

	// 파티던전 에러 - 파티던전 유저 데이터 저장
	PARTY_DUNGEON_ERROR_INVALID_SAVE_USER_DATA = 7703,

	// 파티던전 에러 - 파티던전 맵 데이터 에러
	PARTY_DUNGEON_ERROR_NOT_FOUND_PARTY_DUNGEON_MAP_DATA = 7704,

	// 파티던전 에러 - 파티던전중에는 사용할 수 없는 기능 사용
	PARTY_DUNGEON_ERROR_UNLOCK = 7705,

	// 대결 에러 - 알 수 없는 에러입니다.
	DUEL_ERROR_UNKNOWN = 7800,

	// 대결 에러 - 다른 유저를 초대한상태 입니다.
	DUEL_ERROR_ALEREADY_INVITE_OTHER_USER = 7801,

	// 대결 에러 - 이미 대결중입니다.
	DUEL_ERROR_ALEREADY_PLAYING = 7802,

	// 대결 에러 - 결투장에 참여 가능한 지역이 아닙니다.
	DUEL_ERROR_CANNOT_DUEL_TERRITORY = 7803,

	// 대결 에러 - 존재하지 않는 유저입니다.
	DUEL_ERROR_INVALID_USER = 7804,

	// 대결 에러 - 상대방이 결투장에 참여 가능한 지역이 아닙니다.
	DUEL_ERROR_CANNOT_DUEL_TERRITORY_OTHER_USER = 7805,

	// 대결 에러 - 다른 유저의 대결수락 대기상태 입니다.
	DUEL_ERROR_ALEREADY_INVITED_BY_OTHER_USER = 7806,

	// 대결 에러 - 대결 신청한 상태가 아닙니다.
	DUEL_ERROR_NOT_INVITE_USER = 7807,

	// 대결 에러 - 대결 신청받은 상태가 아닙니다.
	DUEL_ERROR_NOT_INVITED = 7808,

	// 대결 에러 - 다른 지역에서 대결을 할 수 없습니다.
	DUEL_ERROR_NOT_SAME_MAP = 7809,

	// 대결 에러 - 현재 진행중인 대결이 있습니다. 대결 종료 후 다시 시도해주세요.
	DUEL_ERROR_ARENA_ALREADY_PLAYING = 7810,

	// 대결 에러 - 대결 수락 대기 시간이 초과되었습니다.
	DUEL_ERROR_EXPIRED_INVITE_TIME = 7811,

	// 캐릭터 슬롯 확장 에러 - 알 수 없는 에러.
	CHARACTER_SLOT_UNLOCK_ERROR_UNKNOWN = 7900,

	// 캐릭터 슬롯 확장 에러 - 확장 한계 초과.
	CHARACTER_SLOT_UNLOCK_ERROR_EXCEEDED = 7901,

	// 배틀 패스 - 알 수 없는 에러.
	BATTLE_PASS_ERROR_UNKNOWN = 8000,

	// 배틀 패스 - 존재하지 않는 배틀 패스 입니다.
	BATTLE_PASS_ERROR_BATTLE_PASS_NOT_EXISTS = 8001,

	// 배틀 패스 - 보상을 받을 수 없는 조건입니다.
	BATTLE_PASS_ERROR_NOT_MEET_THE_REWARD_CONDITION = 8002,

	// 배틀 패스 - 이미 보상을 받았습니다.
	BATTLE_PASS_ERROR_ALREADY_REWARDED = 8003,

	// 배틀 패스 - 구매 레벨이 현재 레벨보다 낮습니다.
	BATTLE_PASS_ERROR_TARGET_LEVEL_LESS_THAN_CURRENT_LEVEL = 8004,

	// 배틀 패스 - 레벨 한계치를 초과하였습니다.
	BATTLE_PASS_ERROR_EXCEED_MAX_LEVEL = 8005,

	// 배틀 패스 - 레벨을 구매하기 위한 재화가 부족합니다.
	BATTLE_PASS_ERROR_BUY_LEVEL_CURRENCY_NOT_ENOUGH = 8006,

	// 배틀 패스 - 추가 보상이 활성화되지 않았습니다.
	BATTLE_PASS_ERROR_ADDITONAL_REWARD_NOT_ACTIVATED = 8007,

	// 배틀 패스 - 추가 보상이 이미 활성화되어있습니다.
	BATTLE_PASS_ERROR_ADDITONAL_REWARD_IS_ALREADY_ACTIVATED = 8008,

	// 배틀 패스 - 추가 보상 활성화 아이템 조건이 맞지 않습니다.
	BATTLE_PASS_ERROR_ADDITONAL_ITEM_CONDITION_NOT_MEETED = 8009,

	// 배틀 패스 - 추가 보상을 활성화 위한 재화가 부족합니다.
	BATTLE_PASS_ERROR_ACTIVATE_REWARD_CURRENCY_NOT_ENOUGH = 8010,

	// 이벤트 복구 상자 - 알 수 없는 에러.
	DELIVERY_BOX_ERROR_UNKNOWN = 8100,

	// 이벤트 복구 상자 - 이벤트 시간이 아닙니다.
	DELIVERY_BOX_ERROR_NOT_EVENT_TIME = 8101,

	// 이벤트 복구 상자 - 이벤트 타입이 맞지 않습니다.
	DELIVERY_BOX_ERROR_NOT_MEET_THE_EVENT_TYPE = 8102,

	// 이벤트 복구 상자 - 이미 복구한 내용입니다.
	DELIVERY_BOX_ERROR_ALREADY_RECOVERED = 8103,

	// 이벤트 복구 상자 - 이벤트 조건이 맞지 않습니다.
	DELIVERY_BOX_ERROR_NOT_MEET_THE_CONDITION = 8104,

	// 발동효과 - 알 수 없는 에러.
	COMMON_EFFECT_ERROR_UNKNOWN = 8200,

	// Task - Gateway 서버와 연결이 없습니다
	COMMON_ERROR_NO_GATEWAY_SERVER_CONNECTION = 8201,

	// Task - Gateway 서버 작업이 실패했습니다
	COMMON_ERROR_GATEWAY_OPERATION_FAILED = 8202,

	// 키보드 맵핑 저장 실패
	KEYBOARD_MAPPING_ERROR_SAVE = 8300,

	// 키보드 맵핑 로드 실패
	KEYBOARD_MAPPING_ERROR_LOAD = 8301,

	// PK북 - 알 수 없는 에러.
	PK_BOOK_ERROR_UNKNOWN = 8400,

	// PK북 - 존재하지 않는 PK데이터입니다.
	PK_BOOK_ERROR_INVALID_DATA = 8401,

	// PK북 - PK북 처리 실패.
	PK_BOOK_ERROR_GATEWAY_FAIL = 8402,

	// PK북 - 사망한 PK데이터가 아닙니다.
	PK_BOOK_ERROR_NOT_DIE_DATA = 8403,

	// PK북 - 처치한 PK데이터가 아닙니다.
	PK_BOOK_ERROR_NOT_KILL_DATA = 8404,

	// PK북 - 대응 가능 회수가 없습니다.
	PK_BOOK_ERROR_NOT_REMAIN_ACTION_COUNT = 8405,

	// PK북 - 대응 가능 시간이 지났습니다.
	PK_BOOK_ERROR_EXPIRED_ACTION_TIME = 8406,

	// PK북 - NPC에게 대응할 수 없습니다.
	PK_BOOK_ERROR_DISABLE_ACTION_TO_NPC = 8407,

	// PK북 - 워프 가능한 지역이 아닙니다.
	PK_BOOK_ERROR_NOT_WARPABLE_MAP = 8408,

	// PK북 - 접속중인 유저가 아닙니다.
	PK_BOOK_ERROR_NOT_LOGINED_USER = 8409,

	// PK북 - 대응에 필요한 재화가 부족합니다.
	PK_BOOK_ERROR_NOT_ENOUGH_ACTION_CURRENCY = 8410,

	// PK북 - 부적절한 PK메세지입니다.
	PK_BOOK_ERROR_PK_MESSAGE_INVALID_TEXT = 8411,

	// PK북 - 경계대상목록이 모두 찼습니다.
	PK_BOOK_ERROR_MONITORRING_USER_LIST_FULL = 8412,

	// PK북 - 다른서버 유저입니다.
	PK_BOOK_ERROR_ANOTER_SERVER_USER = 8413,

	// PK북 - 대상에게 이동할 수 없습니다.
	PK_BOOK_ERROR_NO_WAPABLE_TO_USER = 8414,

	// PK북 - 존재하지 않는 유저입니다.
	PK_BOOK_ERROR_INVALID_USER = 8415,

	// PK북 - 이미 등록된 유저입니다.
	PK_BOOK_ERROR_ALREADY_MONITORRING_USER_ADDED = 8416,

	// PK북 - 자신을 등록할 수 없습니다.
	PK_BOOK_ERROR_CANNOT_ADD_MYSELF = 8417,

	// PK북 - GM을 등록할 수 없습니다..
	PK_BOOK_ERROR_CANNOT_ADD_GM = 8418,

	// PK북 - 해당 유저에게 대응을 할 수 없습니다.
	PK_BOOK_ERROR_CANNOT_ACTION_TO_USER = 8419,

	// 운영툴 - 에러 시작 한 200개정도 예약하자.
	GMTOOL_ERROR_BEGIN = 8500,

	// 운영툴 - 알 수 없는 에러.
	GMTOOL_ERROR_UNKNOWN = 8501,

	// 운영툴 - 이벤트 등록 실패 (DB에러)
	GMTOOL_CONTENTS_EVENT_REG_ERROR_DB = 8502,

	// 운영툴 - 이벤트 등록 실패 (이미 등록되어있다)
	GMTOOL_CONTENTS_EVENT_REG_ALREADY = 8503,

	// 운영툴 - 이벤트 등록 실패 (이벤트기간 지남)
	GMTOOL_CONTENTS_EVENT_REG_EXPIRED_DATE = 8504,

	// 운영툴 - 이벤트 등록 실패 (데이터 시트에 존재하지 않음)
	GMTOOL_CONTENTS_EVENT_REG_NOT_EXIST_DATA_SHEET = 8505,

	// 운영툴 - 이벤트 삭제 실패 (등록(예약) 안된 이벤트)
	GMTOOL_CONTENTS_EVENT_REMOVE_NOT_REG = 8506,

	// 운영툴 - 이벤트 삭제 실패 (DB에러)
	GMTOOL_CONTENTS_EVENT_REMOVE_ERROR_DB = 8507,

	// 운영툴 - 에러 유저 캐싱 실패.
	GMTOOL_GM_USER_CACHE_MISSING = 8508,

	// 앱가드 - 유효하지 않은 플랫폼을 사용
	APP_GUARD_ERROR_INVALID_PLATFORM = 8600,

	// 앱가드 - 유효하지 않은 활성 모드
	APP_GUARD_ERROR_INVALID_ACTIVE_MODE = 8601,

	// 계정삭제(하이브 연결 에러)
	DELETE_ACCOUNT_HIVE_CONNECT_ERROR = 8700,

	// 계정삭제(서버 에러)
	DELETE_ACCOUNT_ERROR = 8701,

	// 계정삭제(서버 DB에러)
	DELETE_ACCOUNT_DB_ERROR = 8702,

	// 하이브 계정삭제
	HIVE_DELETE_ACCOUNT_ERROR = 8703,

	// 하이브 Hive 인증키 에러(4002)
	HIVE_DELETE_ACCOUNT_CERTIFICATION_KEY_ERROR = 8704,

	// 하이브 Hive 토큰 에러(7000)
	HIVE_DELETE_ACCOUNT_TOKEN_ERROR = 8705,

	// 하이브 계정 삭제 parsing 에러
	HIVE_DELETE_ACCOUNT_PARSE_ERROR = 8706,

	// 캐릭터 초기위치 세팅 실패 - DB 에러
	CREATE_USER_ERROR_DB_POSITION_ERROR = 8707,

	// 캐릭터 커스터마이징 티켓 사용 에러 - 알 수 없는 에러.
	CHARACTER_CUSTOMIZING_ERROR_UNKNOWN = 8800,

	// 캐릭터 커스터마이징 티켓 사용 에러 - 티켓 최대 갯수 초과.
	CHARACTER_CUSTOMIZING_ERROR_EXCEED_MAX_COUNT = 8801,

	// 캐릭터 커스터마이징 티켓 사용 에러 - 티켓 갯수 부족.
	CHARACTER_CUSTOMIZING_ERROR_NOT_ENOUGH_COUNT = 8802,

	// 이미 활성화 된 데이터
	SOCIAL_EMOTION_ACTIVE = 8900,

	// 봉인(문양) - 알 수 없는 에러.
	SEAL_ERROR_UNKNOWN = 9000,

	// 봉인(문양) - 유효하지 않는 상태.
	SEAL_ERROR_INVALID_STATUS = 9001,

	// 봉인(문양) - 봉인 슬롯 잠금 최대치 초과.
	SEAL_ERROR_SLOT_LOCK_EXCEED_MAX = 9002,

	// 봉인(문양) - 해금되지 않은 봉인(문양).
	SEAL_ERROR_NOT_ENOUGH_LEVEL = 9003,

	// 봉인(문양) - 해금되지 않은 슬롯.
	SEAL_ERROR_SLOT_NOT_USABLE_LEVEL = 9004,

	// 봉인(문양) - 최대 레벨 초과.
	SEAL_ERROR_LEVEL_EXCEED_MAX = 9005,

	// 봉인(문양) - 유효하지 않은 스탯풀 아이디.
	SEAL_ERROR_INVALID_STAT_POOL_ID = 9006,

	// QUEST - 기사단 퀘스트 요청에 실패 했습니다.
	QUEST_ERROR_REQ_GUILD_QUEST_INFO = 9100,

	// QUEST - 기사단 퀘스트 설정 데이터에서 문제가 발생했습니다.
	QUEST_ERROR_GUILD_QUEST_CONFIG_DATA = 9101,

	// QUEST - 기사단 멤버가 아닙니다.
	QUEST_ERROR_USER_IS_NOT_GUILD_MEMBER = 9102,

	// QUEST - 기사단 퀘스트 레벨 조건을 만족하지 않습니다.
	QUEST_ERROR_NOT_ENOUGH_GUILD_LEVEL = 9103,

	// QUEST - 기사단 퀘스트 생성에 실패 했습니다.
	QUEST_ERROR_GUILD_QUEST_GENERATE = 9104,

	// QUEST - 기사단 퀘스트 디비 생성에 실패 했습니다.
	QUEST_ERROR_GUILD_QUEST_GENERATE_IN_DB = 9105,

	// QUEST - 캐릭터의 상태가 유효하지 않습니다.
	QUEST_ERROR_INVALID_USER_STATUS = 9106,

	// QUEST - 찾을 수 없는 기사단 퀘스트 아이디 입니다.
	QUEST_ERROR_NOT_FOUND_GUILD_QUEST_ID_IN_DATA_SHEET = 9107,

	// QUEST - 찾을 수 없는 기사단 퀘스트 정보 입니다.
	QUEST_ERROR_NOT_FOUND_GUILD_QUEST_INFO_IN_DATA_SHEET = 9108,

	// QUEST - 해당 퀘스트는 기사단 퀘스트가 아닙니다.
	QUEST_ERROR_NOT_GUILD_QUEST = 9109,

	// QUEST - 이미 수행중인 기사단 퀘스트 입니다.
	QUEST_ERROR_ALREADY_ACCEPT_GUILD_QUEST = 9110,

	// QUEST - 이미 수행한 기사단 퀘스트 입니다.
	QUEST_ERROR_ALREADY_ACCEPTED_GUILD_QUEST = 9111,

	// QUEST - 가입 당일에는 기사단 퀘스트를 수행 할 수 없습니다.
	QUEST_ERROR_GUILD_NOT_ACCEPTABLE_TODAY = 9112,

	// QUEST - 최대 기사단 퀘스트 수락 횟수를 초과 했습니다.
	QUEST_ERROR_GUILD_EXCEED_ACCEPT_COUNT = 9113,

	// QUEST - 보상 요청한 기사단 퀘스트가 유효하지 않습니다.
	QUEST_ERROR_INVALID_REWARD_GUILD_QUEST_ID = 9114,

	// QUEST - 보상 요청한 기사단 퀘스트가 마지막 퀘스트가 아닙니다.
	QUEST_ERROR_NOT_LAST_REWARD_GUILD_QUEST_ID = 9115,

	// QUEST - 이미 보상을 수령한 기사단 퀘스트 입니다.
	QUEST_ERROR_ALREADY_REWARDED_GUILD_QUEST_ID = 9116,

	// QUEST - 아직 완료되지 않은 기사단 퀘스트 입니다.
	QUEST_ERROR_NOT_COMPLETED_GUILD_QUEST = 9117,

	// QUEST - 유효하지 않은 기사단 퀘스트 입니다.
	QUEST_ERROR_INVALID_GUILD_QUEST = 9118,

	// QUEST - 수행중이 아닌 기사단 퀘스트 입니다.
	QUEST_ERROR_NOT_ACCEPTED_GUILD_QUEST = 9119,

	// QUEST - 기사단 퀘스트 새로고침 횟수를 초과 했습니다.
	QUEST_ERROR_EXCEED_GUILD_QUEST_REFRESH_COUNT = 9120,

	// QUEST - 기사단 퀘스트 새로고침 재화가 충분하지 않습니다.
	QUEST_ERROR_NOT_ENOUGH_GUILD_QUEST_REFRESH_CURRENCY = 9121,

	// QUEST - 기사단 퀘스트 새로고침 정보를 찾을 수 없습니다.
	QUEST_ERROR_NOT_FOUND_GUILD_QUEST_REFRESH_INFO = 9122,

	// QUEST - 새로고침 할 기사단 퀘스트가 없습니다.
	QUEST_ERROR_NO_REFRESHABLE_GUILD_QUEST = 9123,

	// QUEST - 기사단 퀘스트가 만료되었습니다.
	QUEST_ERROR_EXPIRED_GUILD_QUEST = 9124,

	// QUEST - 수행 할 수 있는 기사단 퀘스트가 아닙니다.
	QUEST_ERROR_GUILD_NOT_ACCEPTABLE_QUEST = 9125,

	// QUEST - 기사단 퀘스트 보상 아이템 생성에 실패 했습니다.
	QUEST_ERROR_GUILD_REWARD_ITEM_CREATE = 9126,

	// QUEST - 보상을 받을 수 없는 기사단 퀘스트 입니다.
	QUEST_ERROR_GUILD_NOT_REWARDABLE_QUEST = 9127,

	// QUEST - 수행중인 기사단 퀘스트가 아닙니다.
	QUEST_ERROR_GUILD_ACCEPTED_QUEST = 9128,

	// QUEST - 기사단 퀘스트 새로고침이 실패 했습니다.
	QUEST_ERROR_GUILD_REFRESH = 9129,

	// QUEST - 기사단 퀘스트 새로고침 DB요청에 실패 했습니다.
	QUEST_ERROR_GUILD_REFRESH_REQ_DB = 9130,

	// QUEST - 기사단 퀘스트 완료 횟수를 초과 했습니다.
	QUEST_ERROR_GUILD_EXCEED_COMPLETE_COUNT = 9131,

	// 개인거래 - 유효하지 않은 캐릭터 상태
	PERSONAL_TRADING_INVALID_USER_STATUS = 9200,

	// 개인거래 - 거래 대상을 찾을수 없다
	PERSONAL_TRADING_NOT_FOUND_USER = 9201,

	// 개인거래 - 거래가 불가능한 유저(게임옵션:자동거절)
	PERSONAL_TRADING_IMPOSSIBLE_USER = 9202,

	// 개인거래 - 대상이 거래 가능한 거리가 아니다
	PERSONAL_TRADING_ERROR_INVALID_RANGE = 9203,

	// 개인거래 - 대상이 유효한 상태가 아니다
	PERSONAL_TRADING_INVALID_OTHER_USER_STATUS = 9204,

	// 개인거래 - 거래중엔 행동 할수 없다
	PERSONAL_TRADING_DONT_ACTION = 9205,

	// 개인거래 - 사람하고 거래중
	PERSONAL_TRADING_ERROR_OHTER_TRADING = 9206,

	// 개인거래 - 대상 유저가 다른 사람하고 거래중
	PERSONAL_TRADING_ERROR_TRAGET_USER_OHTER_TRADING = 9207,

	// 개인거래 - 서로 다른 장소에 있다..
	PERSONAL_TRADING_ERROR_SAME_POS = 9208,

	// 개인거래 - 컨펌을 할수있는 상태가 아니다(LOCK)상태가 아닙
	PERSONAL_TRADING_ERROR_TRY_CONFIRM = 9209,

	// 개인거래 - 거래중인 상태가 아니다
	PERSONAL_TRADING_ERROR_NOT_TRADING = 9210,

	// 개인거래 - 아이템 등록 실패
	PERSONAL_TRADING_ERROR_ITEM_ADD = 9211,

	// 개인거래 - 다이아 부족
	PERSONAL_TRADING_ERROR_NOT_ENOUGH_DIA = 9212,

	// 개인거래 - 다이아 부족
	PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_DIA = 9213,

	// 개인거래 - 다이아 꽉참
	PERSONAL_TRADING_ERROR_NOT_DIA_FULLY = 9214,

	// 개인거래 - 충분하지 못한 인벤 슬롯
	PERSONAL_TRADING_ERROR_NOT_ENOUGH_INVENTORY_SLOT = 9215,

	// 개인거래 - 상대방이 충분하지 못한 인벤 슬롯
	PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_INVENTORY_SLOT = 9216,

	// 개인거래 - 인벤 무게 꽉참
	PERSONAL_TRADING_ERROR_WEIGHT_FULL = 9217,

	// 개인거래 - 상대방이 인벤 무게 꽉참
	PERSONAL_TRADING_ERROR_TARGET_WEIGHT_FULL = 9218,

	// 개인거래 - 요청 시간 만료
	PERSONAL_TRADING_ERROR_TIME_OUT = 9219,

	// 개인거래 - 이용가능한 레벨이 아님
	PERSONAL_TRADING_ERROR_LEVEL = 9220,

	// 개인거래 - 상대방이 이용가능한 레벨이 아님
	PERSONAL_TRADING_ERROR_TARGET_LEVEL = 9221,

	// 개인거래 - 이용가능한 횟수 초과
	PERSONAL_TRADING_ERROR_TRY_COUNT = 9222,

	// 개인거래 - 상대방이 이용가능한 횟수 초과
	PERSONAL_TRADING_ERROR_TRAGET_TRY_COUNT = 9223,

	// 개인거래 - 알수 없는 오류
	PERSONAL_TRADING_ERROR_UNKNOWN = 9224,

	// 개인거래 - 유효하지 않은 아이템거래
	PERSONAL_TRADING_ERROR_INVALID_ITEM = 9225,

	// 개인거래 - 수락 취소 실패 이미 거래 중이다
	PERSONAL_TRADING_ALREADY_TRADING = 9226,

	// 개인거래 - 안전거래 지역에서만 거래해야된다
	PERSONAL_TRADING_ERROR_USABLE_IN_SAFE_ZONE = 9227,

	// 개인거래 - GM은 거래불가합니다.
	PERSONAL_TRADING_ERROR_CANNOT_USABLE_GM = 9228,

	// 개인거래 - 상대방이 안전거래 지역애  있지 않다
	PERSONAL_TRADING_ERROR_OTHER_USABLE_IN_SAFE_ZONE = 9229,

	// 개인거래 - 상대방이 안전거래 디바이스 등록을 안함
	PERSONAL_TRADING_ERROR_NO_DEVICE_REGISTERED = 9230,

	// 커뮤니티 소녀 미션 이벤트 - 유효하지 않은 캐릭터 상태
	COMMUNITY_EVENT_MISSION_INVALID_USER_STATUS = 9300,

	// 커뮤니티 소녀 미션 이벤트 - 유효하지 않은 데이터
	COMMUNITY_EVENT_MISSION_INVALID_DATA = 9301,

	// 커뮤니티 소녀 미션 이벤트 - 이벤트 기간이 아님
	COMMUNITY_EVENT_MISSION_NOT_EVENT_PERIOD = 9302,

	// 커뮤니티 소녀 미션 이벤트 - 탭 오픈 시간이 아님
	COMMUNITY_EVENT_MISSION_NOT_TAB_OPEN_PERIOD = 9303,

	// 커뮤니티 소녀 미션 이벤트 - 미션 데이터가 존재하지 않음
	COMMUNITY_EVENT_MISSION_NOT_EXIST_MISSION_DATA = 9304,

	// 커뮤니티 소녀 미션 이벤트 - 미션 카운트 부족
	COMMUNITY_EVENT_MISSION_NOT_MEET_THE_CONDITION_MISSION_COUNT = 9305,

	// 커뮤니티 소녀 미션 이벤트 - 이미 보상받은 미션
	COMMUNITY_EVENT_MISSION_IS_ALREADY_REWARDED_MISSION = 9306,

	// 기사단 경매- 유효하지 경매장 번호
	GUILD_AUCTION_ERROR_INVALID_ID = 9400,

	// 기사단 경매- 시간 만료
	GUILD_AUCTION_ERROR_EXPIRED_DATE_TIME = 9401,

	// 기사단 경매- 아이템 분배 설정 오류
	GUILD_AUCTION_ERROR_DIVISION_STATE = 9402,

	// 기사단 경매- 재화 부족
	GUILD_AUCTION_ERROR_ENOUGH_CURRENCY = 9403,

	// 기사단 경매- 지정자 오류
	GUILD_AUCTION_ERROR_APPOINTMENT = 9404,

	// 기사단 경매- 지정자 오류
	GUILD_AUCTION_ERROR_PARTICIPANTS = 9405,

	// 기사단 경매- 유효하지 경매장 디버 오류
	GUILD_AUCTION_ERROR_DB = 9406,

	// 기사단 경매- 유효하지 이미 구매 입찰 종료된 상태
	GUILD_AUCTION_ERROR_BUY_STATE = 9407,

	// 기사단 경매- 유효하지 중복 재화 회수
	GUILD_AUCTION_ERROR_RETURN_CURRENCY = 9408,

	// 기사단 경매- 유효하지 중복 경매장 등록
	GUILD_AUCTION_ERROR_DUPLICATION_REGISTER = 9409,

	// 기사단 탈퇴 할수 없음 입찰 및 회수 할금액이 있는 경우
	GUILD_LEAVE_ERROR_AUCTION_DATA = 9410,

	// 기사단 가입 시간 오류
	GUILD_AUCTION_ERROR_JOINED_TIME = 9411,

	// 기사단 경매장에 등록 불가 <최대치>
	GUILD_AUCTION_ERROR_FULL_REGISTRATION = 9412,

	// 기기사단 경매는 상위입찰만 가능합니다.
	GUILD_AUCTION_ERROR_HIGHER_BID = 9413,

	// 경매 가능한 아이템이 없습니다
	GUILD_AUCTION_ERROR_EMPTY_ITEM = 9414,

	// 경매 창고 빈슬롯이 없는 경우
	GUILD_AUCTION_ERROR_SAFE_CAPACITY_FULL = 9415,

	// 기사단 SNS URL 저장 실패 - 유효하지 않은 URL ID
	GUILD_SNS_URL_ERROR_INVALID_ID = 9500,

	// 기사단 SNS URL 저장 실패 - 유효하지 않은 URL 길이
	GUILD_SNS_URL_ERROR_INVALID_LEN = 9501,

	// 닉네임에 허용되지 않은 문자이다
	NICKNAME_CHANGE_ERROR_NOT_ALLOWED_NAME = 9502,

	// 닉네임의 최대 허용 길이가 부족하거나 넘었음
	NICKNAME_CHANGE_ERROR_INVALID_LENGTH_NAME = 9503,

	// 중복된 닉네임이 존재
	NICKNAME_CHANGE_ERROR_DUPLICATION_NAME = 9504,

	// 닉네임 변경 - 마을 오류
	NICKNAME_CHANGE_ERROR_NOT_TOWN = 9505,

	// 닉네임 변경 - 길드 가입 오류
	NICKNAME_CHANGE_ERROR_GUILD_APPLICANTS = 9506,

	// 닉네임 변경 - DB  오류
	NICKNAME_CHANGE_ERROR_DB = 9507,

	// 침공 에러 - 아직 포털아 준비가 안되었습니다. 
	INVADE_ERROR_PORTAL_SPAWNING = 9508,

	// 서버 이전 - 알 수 없는 에러
	SERVER_MIGRATE_ERROR_UNKNOWN_ERROR = 9600,

	// 서버 이전 - 올바르지 않은 유저 상태
	SERVER_MIGRATE_ERROR_INVALID_USER_STATUS = 9601,

	// 서버 이전 - 서버 이전 조건에 부합하지 않음
	SERVER_MIGRATE_ERROR_NOT_MEET_THE_CONDITION = 9602,

	// 서버 이전 - 서버 이전 요청 진행중
	SERVER_MIGRATE_ERROR_ALREADY_IN_PROCESS = 9603,

	// 서버 이전 - 올바르지 않은 서버 세션 상태
	SERVER_MIGRATE_ERROR_SERVER_SESSION_IS_INVALID = 9604,

	// 서버 이전 - DB 쿼리 실패
	SERVER_MIGRATE_ERROR_DB_QUERY_FAILED = 9605,

	// 서버 이전 - 서버 이전 데이터 전송 진행중
	SERVER_MIGRATE_ERROR_DATA_TRANSFER_IN_PROCESS = 9606,

	// 서버 이전 조건 체크 - 계정 정보를 찾을 수 없음.
	SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_ACCOUNT = 9607,

	// 서버 이전 조건 체크 - 유저 정보를 찾을 수 없음.
	SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_USER = 9608,

	// 서버 이전 조건 체크 - 기사단에 가입 상태
	SERVER_MIGRATE_ERROR_CHECK_CONDITION_GUILD_MEMBER = 9609,

	// 서버 이전 - 이미 닉네임 변경 완료 상태
	SERVER_MIGRATE_ERROR_NICKNAME_ALREADY_CHANGED = 9610,

	// 서버 이전 조건 체크 - 이미 기사단 이름 변경 완료 상태
	SERVER_MIGRATE_ERROR_GUILD_NAME_ALREADY_CHANGED = 9611,

	// 서버 이전 - 이전 하려는 서버에 인원이 가득 찼습니다.
	SERVER_MIGRATE_ERROR_TARGET_SERVER_EXCEED_INCOMING_COUNT = 9612,

	// 서버 이전 - 현재 서버에 이전 가능 인원이 초과하였습니다.
	SERVER_MIGRATE_ERROR_CURRENT_SERVER_EXCEED_OUTGOING_COUNT = 9613,

	// 서버 이전 - 보유 재화량이 부족합니다.
	SERVER_MIGRATE_ERROR_NOT_ENOUGH_CURRENCY = 9614,

	//  어뷰징 - 패킥 전송 간격이 매우 짧음
	ABUSE_PACKET_SHORT_TICK = 9700,

	// 기사단 길드전 알수없는 에러.
	GUILD_WAR_UNKNOWN_ERROR = 9800,

	// 기사단 길드전 상대방을 찾기에 실패했습니다.
	GUILD_WAR_MATCH_NOT_FOUND_ENEMY_GUILD = 9801,

	// 기사단 길드전 매칭 최소인원 제한에 걸림
	GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MINIMUM_LIMIT = 9802,

	// 기사단 길드전 입장실패 - 참가신청을 하지 않은 유저
	GUILD_WAR_ENTER_FAIL_NOT_REGISTERED_IN_ENTRY = 9803,

	// 기사단 길드전 입장실패 - 게임입장단계가 아님
	GUILD_WAR_ENTER_FAIL_INVALID_STATE = 9804,

	// 기사단 길드전 매칭 이미 등록함
	GUILD_WAR_MATCH_REGISTER_FAIL_ALREADY_REGISTER = 9805,

	// 기사단 길드전 매칭 참가신청이 상태가 아닙니다.
	GUILD_WAR_MATCH_REGISTER_FAIL_STATE = 9806,

	// 기사단 길드전 매칭 최대인원 제한에 걸림
	GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MAX_LIMIT = 9807,

	// 기사단 길드전 매칭 멤버등급 제한에 걸림
	GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_GRADE_LIMIT = 9808,

	// 기사단 길드전 매칭 실패 - 기사단 레벨이 부족
	GUILD_WAR_MATCH_REGISTER_FAIL_LEVEL = 9809,

	// 기사단 길드전 신청- 가입후 일정 기간 지나지 않았다
	GUILD_WAR_ERROR_THE_DAY_JOINED_GUILD = 9810,

	// 영지 공략 던전 알수없는 에러.
	AGIT_DUNGEON_RAID_UNKNOWN_ERROR = 9900,

	// 영지 공략 던전 알수없는 에러.(월드)
	AGIT_DUNGEON_RAID_WORLD_UNKNOWN_ERROR = 9901,

	// 영지 공략 던전 계정 데이터 오류
	AGIT_DUNGEON_RAID_ACCOUNT_DATA = 9902,

	// 영지 공략 던전 데이터 초기화 되지 않음(데이터 갱신 패킷 재요청 필요)
	AGIT_DUNGEON_RAID_NOT_YET_RESET = 9903,

	// 영지 공략 던전 세부 정보가 없음
	AGIT_DUNGEON_RAID_NOT_EXIST_DETAIL_DATA = 9904,

	// 영지 공략 던전 유저 상태 에러
	AGIT_DUNGEON_RAID_INVALID_USER_STATE = 9905,

	// 영지 공략 던전 요청 데이터 오류
	AGIT_DUNGEON_RAID_INVALID_PARRAM = 9906,

	// 영지 공략 던전 기사단 가입 유저가 아님
	AGIT_DUNGEON_RAID_NOT_GUILD_MEMBER = 9907,

	// 영지 공략 던전 오픈 가능 등급이 아님
	AGIT_DUNGEON_RAID_OPEN_GRADE_ERROR = 9908,

	// 영지 공략 던전 설정 정보 오류
	AGIT_DUNGEON_RAID_CONFIG_DATA_ERROR = 9909,

	// 영지 공략 던전 컨텐츠 이용 가능 기사단 레벨이 아님
	AGIT_DUNGEON_RAID_CONTENTS_GUILD_LEVEL_ERROR = 9910,

	// 영지 공략 던전 오픈 가능 기사단 레벨이 아님
	AGIT_DUNGEON_RAID_GUILD_LEVEL_ERROR = 9911,

	// 영지 공략 던전 세부 설정 정보 오류
	AGIT_DUNGEON_RAID_DETAIL_DATA_ERROR = 9912,

	// 영지 공략 던전 이미 열린 던전이 있음
	AGIT_DUNGEON_RAID_ALREADY_OPENED = 9913,

	// 영지 공략 던전 맵 정보가 없음
	AGIT_DUNGEON_RAID_NO_MAP_DATA = 9914,

	// 영지 공략 던전 기사단 운영비 부족
	AGIT_DUNGEON_RAID_NOT_ENOUGH_GUILD_ASSET = 9915,

	// 영지 공략 던전 채널 생성 실패
	AGIT_DUNGEON_RAID_FAIL_TO_CREATE_CHANNEL = 9916,

	// 영지 공략 던전 유저 데이터가 없음
	AGIT_DUNGEON_RAID_NO_USER_DATA = 9917,

	// 영지 공략 던전 기사단 가입 시간 제한 에러(가입후 일정 기간 지나지 않았다)
	AGIT_DUNGEON_RAID_GUILD_JOINED_DAY_ERROR = 9918,

	// 영지 공략 던전 최대 던전 클리어 횟수 초과
	AGIT_DUNGEON_RAID_MAX_CLEAR_COUNT = 9919,

	// 영지 공략 던전 이용 가능 시간이 아닙니다.
	AGIT_DUNGEON_RAID_NOT_AVAILABLE_TIME = 9920,

	// 영지 공략 던전 던전이 닫혀있지 않습니다.
	AGIT_DUNGEON_RAID_DUNGEON_NOT_CLOSED = 9921,

	// 영지 공략 던전 디비 처리 실패.
	AGIT_DUNGEON_RAID_DB_OPERATION_ERROR = 9922,

	// 영지 공략 던전 닫을 던전 정보가 없습니다.
	AGIT_DUNGEON_RAID_NO_MAP_TO_CLOSE = 9923,

	// 영지 공략 던전 닫을 채널 정보가 없습니다.
	AGIT_DUNGEON_RAID_NO_CHANNEL_TO_CLOSE = 9924,

	// 영지 공략 던전 데이터 생성 실패
	AGIT_DUNGEON_RAID_FAIL_TO_CREATE_RAID_DATA = 9925,

	// 영지 공략 던전 존재하지 않는 던전 데이터
	AGIT_DUNGEON_RAID_NOT_EXIST_RAID_DATA = 9926,

	// 영지 공략 던전 열려있지 않은 던전 입니다.
	AGIT_DUNGEON_RAID_DUNGEON_NOT_OPENED = 9927,

	// 영지 공략 던전 유저 채널이 존재하지 않음
	AGIT_DUNGEON_RAID_NOT_EXIST_USER_CHANNEL = 9928,

	// 영지 공략 던전 맵 타입이 영지 공략 던전이 아님
	AGIT_DUNGEON_RAID_MAP_TYPE_ERROR = 9929,

	// 영지 공략 던전 접속할 채널이 없음
	AGIT_DUNGEON_NO_CHANNEL_TO_ENTER = 9930,

	// 영지 공략 던전 행동 불가 상태로 워프 불가능
	AGIT_DUNGEON_RAID_WARP_ERROR_ABNORMALITY_EFFECT = 9931,

	// 영지 공략 던전 입장 유저 티켓 개수 부족
	AGIT_DUNGEON_RAID_NOT_ENOUGH_USER_TICKET_COUNT = 9932,

	// 영지 공략 던전 기사단 정보가 없음
	AGIT_DUNGEON_RAID_NOT_FOUND_GUILD = 9933,

	// 영지 공략 던전 던전이 파괴중 입니다.
	AGIT_DUNGEON_RAID_DESTROYING = 9934,

	// 영지 공략 던전 던전에 유저가 존재하기 때문에 던전을 닫을 수 없습니다.
	AGIT_DUNGEON_RAID_FAIL_TO_CLOSE_USER_EXISTS = 9935,

	// 길드 그룹 채팅 - 유효 하지 않은 채팅방 이름
	CHAT_GUILD_GROUP_INVALID_ROOM_NAME = 9936,

	// 길드 그룹 채팅 - 가입 할 수 있는 채팅 방 수를 넘었다
	CHAT_GUILD_GROUP_JOIN_OVER_LIMIT = 9937,

	// 길드 그룹 채팅 - 참여 가능한 최대 기사단 수 를 넘었다
	CHAT_GUILD_GROUP_MEMBER_OVER_LIMIT = 9938,

	// 길드 그룹 채팅 - 초대 가능한 최대 기사단 수 를 넘었다
	CHAT_GUILD_GROUP_INVITE_OVER_LIMIT = 9939,

	// 길드 그룹 채팅 - 유효 하지 않은 채팅방 ID
	CHAT_GUILD_GROUP_INVALID_ROOM_ID = 9940,

	// 길드 그룹 채팅 - 이미 초대 한 길드
	CHAT_GUILD_GROUP_ALREADY_INVITED = 9941,

	// 길드 그룹 채팅 - 초대 받은 적 없다
	CHAT_GUILD_GROUP_NOT_INVITED = 9942,

	// 길드 그룹 채팅 - 이미 가입한 그룹이다
	CHAT_GUILD_GROUP_ALREADY_JOINED = 9943,

	// 길드 그룹 채팅 - DB 오류
	CHAT_GUILD_GROUP_DB_FAILED = 9944,

	// 길드 그룹 채팅 - 리더 길드가 아니다
	CHAT_GUILD_GROUP_NOT_LEADER_GUILD = 9945,

	// 길드 그룹 채팅 - 가입된 길드가 아니다
	CHAT_GUILD_GROUP_NOT_JOIN_GUILD = 9946,

	// 길드 그룹 채팅 - 탈퇴 할 수 없다
	CHAT_GUILD_GROUP_DO_NOT_WITHDRAWAL = 9947,

	// 길드 그룹 채팅 - 길드 아이디가 잘못 되었다 
	CHAT_GUILD_GROUP_DO_WRONG_GUILD_ID = 9948,

	// 길드 그룹 채팅 - 데이터 비교가 잘못되었다
	CHAT_GUILD_GROUP_DATA_DIFF = 9949,

	PACKET_RESULT_MAX = 9950,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Result ResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("RESULT_SUCCESS"), ESearchCase::IgnoreCase) == 0)	return Result::RESULT_SUCCESS;
	if (str.Compare(TEXT("LOGIN_ERROR_RETRY"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_RETRY;
	if (str.Compare(TEXT("LOGIN_ERROR_RESTART"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_RESTART;
	if (str.Compare(TEXT("LOGIN_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_UNKNOWN;
	if (str.Compare(TEXT("LOGIN_ERROR_WRONG_WZ_PACKET_VERSION"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_WRONG_WZ_PACKET_VERSION;
	if (str.Compare(TEXT("LOGIN_ERROR_INTERNAL_SERVER_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_INTERNAL_SERVER_ERROR;
	if (str.Compare(TEXT("LOGIN_ERROR_INTERNAL_DB_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_INTERNAL_DB_ERROR;
	if (str.Compare(TEXT("LOGIN_ERROR_NEED_UPDATE_ACCOUNT_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_NEED_UPDATE_ACCOUNT_NAME;
	if (str.Compare(TEXT("LOGIN_ERROR_ID_PW_AUTH_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_ID_PW_AUTH_FAILED;
	if (str.Compare(TEXT("LOGIN_ERROR_INVALID_AUTHORIZATION"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_INVALID_AUTHORIZATION;
	if (str.Compare(TEXT("LOGIN_ERROR_GETTING_READY_FOR_GATEWAYSERVER"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_GETTING_READY_FOR_GATEWAYSERVER;
	if (str.Compare(TEXT("LOGIN_ERROR_INVALID_ACCOUNT_ID"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_INVALID_ACCOUNT_ID;
	if (str.Compare(TEXT("LOGIN_ERROR_INVALID_VID"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_INVALID_VID;
	if (str.Compare(TEXT("LOGIN_ERROR_INCORRECT_TOKEN"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_INCORRECT_TOKEN;
	if (str.Compare(TEXT("LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB;
	if (str.Compare(TEXT("LOGIN_ERROR_TIME_OUT"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_TIME_OUT;
	if (str.Compare(TEXT("LOGIN_ERROR_INVALID_AUTH_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_INVALID_AUTH_STATUS;
	if (str.Compare(TEXT("LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN;
	if (str.Compare(TEXT("LOGIN_ERROR_SERVER_MAINTENANCE"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_SERVER_MAINTENANCE;
	if (str.Compare(TEXT("LOGIN_ERROR_NOT_MATCHED_VID_PLANETWORLD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_NOT_MATCHED_VID_PLANETWORLD_ID;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_VERIFY_TOKEN_SETTING"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_VERIFY_TOKEN_SETTING;
	if (str.Compare(TEXT("LOGIN_ERROR_DELETED_ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_DELETED_ACCOUNT;
	if (str.Compare(TEXT("SELECT_SERVER_ERROR_INACCESSIBLE"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_SERVER_ERROR_INACCESSIBLE;
	if (str.Compare(TEXT("SELECT_SERVER_ERROR_OVERLOAD"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_SERVER_ERROR_OVERLOAD;
	if (str.Compare(TEXT("SELECT_SERVER_ERROR_NOT_FOUND"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_SERVER_ERROR_NOT_FOUND;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_REGISTER_DEVICE_SETTING"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_REGISTER_DEVICE_SETTING;
	if (str.Compare(TEXT("LOGIN_ERROR_NO_CHARACTER_CREATED"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_NO_CHARACTER_CREATED;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_INVALID_PARAMETER"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_INVALID_PARAMETER;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_REQUEST_JSON"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_REQUEST_JSON;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_DB_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_DB_ERROR;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_INVALID_TOKEN_AWS"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_INVALID_TOKEN_AWS;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_INVALID_APPID"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_INVALID_APPID;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_IDP_TOKEN_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_IDP_TOKEN_ERROR;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_INVALID_TOKEN"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_INVALID_TOKEN;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_NO_TOKEN_HEADER"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_NO_TOKEN_HEADER;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_UNKNOWN_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_UNKNOWN_ERROR;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_UNABLE_TO_CONNECT"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_UNABLE_TO_CONNECT;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_CONNECT"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_CONNECT;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_PARSE"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_PARSE;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_NO_RESULT_CODE"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_NO_RESULT_CODE;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_UNKNOWN_RESULT_CODE"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_UNKNOWN_RESULT_CODE;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_NO_REGISTER_DEVICE_PARAMETER"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_NO_REGISTER_DEVICE_PARAMETER;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_CONNECT_REGISTER_DEVICE"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_CONNECT_REGISTER_DEVICE;
	if (str.Compare(TEXT("LOGIN_ERROR_HIVE_PARSE_REGISTER_DEVICE"), ESearchCase::IgnoreCase) == 0)	return Result::LOGIN_ERROR_HIVE_PARSE_REGISTER_DEVICE;
	if (str.Compare(TEXT("SELECT_CHANNEL_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_CHANNEL_ERROR_UNKNOWN;
	if (str.Compare(TEXT("SELECT_CHANNEL_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_CHANNEL_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("SELECT_CHANNEL_ERROR_INVALID_AREA"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_CHANNEL_ERROR_INVALID_AREA;
	if (str.Compare(TEXT("SELECT_CHANNEL_ERROR_SAME_AREA_CHANNEL"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_CHANNEL_ERROR_SAME_AREA_CHANNEL;
	if (str.Compare(TEXT("SELECT_CHANNEL_ERROR_MAP_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_CHANNEL_ERROR_MAP_TYPE;
	if (str.Compare(TEXT("SELECT_CHANNEL_ERROR_IN_COMBAT"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_CHANNEL_ERROR_IN_COMBAT;
	if (str.Compare(TEXT("SELECT_CHANNEL_ERROR_IN_SHUTTLE"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_CHANNEL_ERROR_IN_SHUTTLE;
	if (str.Compare(TEXT("SELECT_CHANNEL_ERROR_INVALID_CHANNEL"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_CHANNEL_ERROR_INVALID_CHANNEL;
	if (str.Compare(TEXT("SELECT_CHANNEL_ERROR_CONGESTION_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_CHANNEL_ERROR_CONGESTION_FULL;
	if (str.Compare(TEXT("ZPAY_LOGIN_ERROR_INVALID_AUTH"), ESearchCase::IgnoreCase) == 0)	return Result::ZPAY_LOGIN_ERROR_INVALID_AUTH;
	if (str.Compare(TEXT("ZPAY_LOGIN_ERROR_ENTER_SERVER"), ESearchCase::IgnoreCase) == 0)	return Result::ZPAY_LOGIN_ERROR_ENTER_SERVER;
	if (str.Compare(TEXT("ZPAY_LOGIN_ERROR_PLAY_USER"), ESearchCase::IgnoreCase) == 0)	return Result::ZPAY_LOGIN_ERROR_PLAY_USER;
	if (str.Compare(TEXT("ZPAY_LOGIN_ERROR_UNLOCK_CONTENTS"), ESearchCase::IgnoreCase) == 0)	return Result::ZPAY_LOGIN_ERROR_UNLOCK_CONTENTS;
	if (str.Compare(TEXT("ITEM_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_UNKNOWN;
	if (str.Compare(TEXT("ITEM_ERROR_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("ITEM_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("ITEM_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_INVALID_USER;
	if (str.Compare(TEXT("ITEM_ERROR_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_INVALID_USER_STATUS;
	if (str.Compare(TEXT("ITEM_ERROR_NOT_ALLOWED_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_NOT_ALLOWED_LEVEL;
	if (str.Compare(TEXT("ITEM_ERROR_ITEM_NOT_FOUND"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ITEM_NOT_FOUND;
	if (str.Compare(TEXT("ITEM_ERROR_INVALID_ITEM_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_INVALID_ITEM_ID;
	if (str.Compare(TEXT("ITEM_ERROR_INVALID_ITEM_DB_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_INVALID_ITEM_DB_ID;
	if (str.Compare(TEXT("ITEM_ERROR_ADD_INVENTORY_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ADD_INVENTORY_FULL;
	if (str.Compare(TEXT("ITEM_ERROR_ADD_NOT_ALLOWED_GENDER"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ADD_NOT_ALLOWED_GENDER;
	if (str.Compare(TEXT("ITEM_ERROR_ADD_INVALID_STACK_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ADD_INVALID_STACK_COUNT;
	if (str.Compare(TEXT("ITEM_ERROR_ADD_WEIGHT_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ADD_WEIGHT_FULL;
	if (str.Compare(TEXT("ITEM_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("ITEM_ERROR_OVERFLOW_MAX_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_OVERFLOW_MAX_MONEY;
	if (str.Compare(TEXT("ITEM_ERROR_DELETE_LOCKED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_DELETE_LOCKED;
	if (str.Compare(TEXT("ITEM_ERROR_LOCK_ALREADY_LOCKED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_LOCK_ALREADY_LOCKED;
	if (str.Compare(TEXT("ITEM_ERROR_LOCK_ALREADY_UNLOCKED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_LOCK_ALREADY_UNLOCKED;
	if (str.Compare(TEXT("ITEM_ERROR_LOCK_NOT_LOCKABLE"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_LOCK_NOT_LOCKABLE;
	if (str.Compare(TEXT("ITEM_ERROR_DECREASE_INSUFFICIENT"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_DECREASE_INSUFFICIENT;
	if (str.Compare(TEXT("ITEM_ERROR_EQUIP_NOT_EQUIP_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_EQUIP_NOT_EQUIP_ITEM;
	if (str.Compare(TEXT("ITEM_ERROR_EQUIP_ALREADY_EQUIPPED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_EQUIP_ALREADY_EQUIPPED;
	if (str.Compare(TEXT("ITEM_ERROR_EQUIP_ALREADY_UNEQUIPPED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_EQUIP_ALREADY_UNEQUIPPED;
	if (str.Compare(TEXT("ITEM_ERROR_EQUIP_INVALID_PRESET_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_EQUIP_INVALID_PRESET_ID;
	if (str.Compare(TEXT("ITEM_ERROR_EQUIP_INVALID_PRESET_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_EQUIP_INVALID_PRESET_NAME;
	if (str.Compare(TEXT("ITEM_ERROR_EQUIP_CHANGE_WEAPON_TPYE"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_EQUIP_CHANGE_WEAPON_TPYE;
	if (str.Compare(TEXT("ITEM_ERROR_EQUIP_CAN_NOT_UNEQUIP_WEAPON"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_EQUIP_CAN_NOT_UNEQUIP_WEAPON;
	if (str.Compare(TEXT("ITEM_ERROR_SOCKET_INVALID_TARGET"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_SOCKET_INVALID_TARGET;
	if (str.Compare(TEXT("ITEM_ERROR_SOCKET_ALREADY_SOCKETED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_SOCKET_ALREADY_SOCKETED;
	if (str.Compare(TEXT("ITEM_ERROR_SOCKET_IS_NOT_EMPTY"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_SOCKET_IS_NOT_EMPTY;
	if (str.Compare(TEXT("ITEM_ERROR_USE_NOT_USABLE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_NOT_USABLE_ITEM;
	if (str.Compare(TEXT("ITEM_ERROR_USE_INVALID_EFFECT_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_INVALID_EFFECT_TYPE;
	if (str.Compare(TEXT("ITEM_ERROR_USE_INVALID_EFFECT_PARAM"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_INVALID_EFFECT_PARAM;
	if (str.Compare(TEXT("ITEM_ERROR_USE_NOT_ENOUGH_ITEMS"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_NOT_ENOUGH_ITEMS;
	if (str.Compare(TEXT("ITEM_ERROR_USE_IN_COOLDOWN"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_IN_COOLDOWN;
	if (str.Compare(TEXT("ITEM_ERROR_USE_POTION_HP_MP_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_POTION_HP_MP_FULL;
	if (str.Compare(TEXT("ITEM_ERROR_USE_LOCKED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_LOCKED;
	if (str.Compare(TEXT("ITEM_ERROR_USE_ALREADY_LEARN_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_ALREADY_LEARN_SKILL;
	if (str.Compare(TEXT("ITEM_ERROR_USE_MAX_USED_BONUS_STAT_POINT_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_MAX_USED_BONUS_STAT_POINT_ITEM;
	if (str.Compare(TEXT("ITEM_ERROR_USE_PVP_ALIGNMENT"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USE_PVP_ALIGNMENT;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_INVALID_SCROLL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_INVALID_SCROLL;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_INVALID_AMOUNT"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_INVALID_AMOUNT;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_RATE_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_RATE_FAILED;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_EQUIPED_ITEM_CANNOT_OVER_SAFE_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_EQUIPED_ITEM_CANNOT_OVER_SAFE_LEVEL;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_LOCKED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_LOCKED;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_INVALID_TARGET"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_INVALID_TARGET;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_INVALID_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_INVALID_LEVEL;
	if (str.Compare(TEXT("ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY;
	if (str.Compare(TEXT("ITEM_ERROR_DECOMPOSE_INVALID_EQUIPED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_DECOMPOSE_INVALID_EQUIPED;
	if (str.Compare(TEXT("ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY_EQUIP"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY_EQUIP;
	if (str.Compare(TEXT("ITEM_ERROR_DECOMPOSE_PAID_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_DECOMPOSE_PAID_ITEM;
	if (str.Compare(TEXT("ITEM_ERROR_DECOMPOSE_INVALID_REWARD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_DECOMPOSE_INVALID_REWARD_ID;
	if (str.Compare(TEXT("ITEM_ERROR_NPCSHOP_UNABLE_TO_SELL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_NPCSHOP_UNABLE_TO_SELL;
	if (str.Compare(TEXT("ITEM_ERROR_ADD_STORE_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ADD_STORE_FULL;
	if (str.Compare(TEXT("ITEM_ERROR_ADD_STORAGE_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ADD_STORAGE_FULL;
	if (str.Compare(TEXT("ITEM_ERROR_EXPIRED_DATE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_EXPIRED_DATE_TIME;
	if (str.Compare(TEXT("ITEM_ERROR_UNLOCK"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_UNLOCK;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_TARGET_CATEGORY"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_TARGET_CATEGORY;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_LEVEL;
	if (str.Compare(TEXT("ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_SCROLL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_SCROLL;
	if (str.Compare(TEXT("ITEM_ERROR_USABLE_IN_SAFE_ZONE"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_USABLE_IN_SAFE_ZONE;
	if (str.Compare(TEXT("ITEM_ERROR_COMBAT_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_COMBAT_STATUS;
	if (str.Compare(TEXT("ITEM_ERROR_DELETE_LIST_EMPTY"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_DELETE_LIST_EMPTY;
	if (str.Compare(TEXT("ITEM_ERROR_CREATE_LIST_EMPTY"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_CREATE_LIST_EMPTY;
	if (str.Compare(TEXT("ITEM_ERROR_ABNORMALITY_STAT_ALREADY_IN_EFFECT"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_ABNORMALITY_STAT_ALREADY_IN_EFFECT;
	if (str.Compare(TEXT("ITEM_ERROR_FUSE_DIFFERENT_ITEMS"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_FUSE_DIFFERENT_ITEMS;
	if (str.Compare(TEXT("ITEM_ERROR_FUSE_CHARM_SAME_TEMPLATE_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_FUSE_CHARM_SAME_TEMPLATE_ID;
	if (str.Compare(TEXT("ITEM_ERROR_FUSE_DIFFERENT_EQUIP_GROUP_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_FUSE_DIFFERENT_EQUIP_GROUP_ID;
	if (str.Compare(TEXT("ITEM_ERROR_FUSE_NOT_EQUIP_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_FUSE_NOT_EQUIP_ITEM;
	if (str.Compare(TEXT("ITEM_ERROR_FUSE_INVALID_FUSE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_FUSE_INVALID_FUSE_ITEM;
	if (str.Compare(TEXT("ITEM_ERROR_FUSE_LESS_THAN_MIN_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_FUSE_LESS_THAN_MIN_LEVEL;
	if (str.Compare(TEXT("ITEM_ERROR_FUSE_NOT_SAME_ENCHANT_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_FUSE_NOT_SAME_ENCHANT_LEVEL;
	if (str.Compare(TEXT("ITEM_ERROR_FUSE_REFINE_DATA_NOT_EXISTS"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_FUSE_REFINE_DATA_NOT_EXISTS;
	if (str.Compare(TEXT("ITEM_ERROR_FUSE_SELECT_REFINE_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_FUSE_SELECT_REFINE_DATA;
	if (str.Compare(TEXT("ITEM_ERROR_INVALID_USER_DEATH"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_INVALID_USER_DEATH;
	if (str.Compare(TEXT("ITEM_ERROR_INVALID_MAGICAL_FORGE"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_INVALID_MAGICAL_FORGE;
	if (str.Compare(TEXT("ITEM_ERROR_INVALID_MAGICAL_FORGE_MATIRAL_ITEM_EFFECT_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_INVALID_MAGICAL_FORGE_MATIRAL_ITEM_EFFECT_TYPE;
	if (str.Compare(TEXT("ITEM_ERROR_MAGICAL_FORGE_INVALID_TARGET"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_MAGICAL_FORGE_INVALID_TARGET;
	if (str.Compare(TEXT("ITEM_ERROR_MAGICAL_FORGE_INVALID_MATARIAL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_MAGICAL_FORGE_INVALID_MATARIAL;
	if (str.Compare(TEXT("ITEM_ERROR_MAGICAL_FORGE_MAX_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_MAGICAL_FORGE_MAX_LEVEL;
	if (str.Compare(TEXT("ITEM_ERROR_MAGICAL_FORGE_LOCKED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_MAGICAL_FORGE_LOCKED;
	if (str.Compare(TEXT("ITEM_ERROR_MAGICAL_FORGE_INVALID_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_MAGICAL_FORGE_INVALID_GRADE;
	if (str.Compare(TEXT("ITEM_ERROR_NOT_ALLOWED_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_NOT_ALLOWED_MAP;
	if (str.Compare(TEXT("REWARD_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::REWARD_ERROR_UNKNOWN;
	if (str.Compare(TEXT("REWARD_ERROR_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::REWARD_ERROR_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("REWARD_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::REWARD_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("REWARD_ERROR_INVALID_REWARD"), ESearchCase::IgnoreCase) == 0)	return Result::REWARD_ERROR_INVALID_REWARD;
	if (str.Compare(TEXT("REWARD_ERROR_INVALID_REWARD_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::REWARD_ERROR_INVALID_REWARD_TYPE;
	if (str.Compare(TEXT("REWARD_ERROR_CURRENCY_EXCEEDED"), ESearchCase::IgnoreCase) == 0)	return Result::REWARD_ERROR_CURRENCY_EXCEEDED;
	if (str.Compare(TEXT("VEHICLE_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::VEHICLE_ERROR_UNKNOWN;
	if (str.Compare(TEXT("VEHICLE_ERROR_INVALID_ID"), ESearchCase::IgnoreCase) == 0)	return Result::VEHICLE_ERROR_INVALID_ID;
	if (str.Compare(TEXT("VEHICLE_ERROR_ALREADY_MOUNTED"), ESearchCase::IgnoreCase) == 0)	return Result::VEHICLE_ERROR_ALREADY_MOUNTED;
	if (str.Compare(TEXT("VEHICLE_ERROR_IN_TRANSFORM"), ESearchCase::IgnoreCase) == 0)	return Result::VEHICLE_ERROR_IN_TRANSFORM;
	if (str.Compare(TEXT("VEHICLE_ERROR_FORBIDDEN_AREA"), ESearchCase::IgnoreCase) == 0)	return Result::VEHICLE_ERROR_FORBIDDEN_AREA;
	if (str.Compare(TEXT("VEHICLE_ERROR_DISABLE_BEHAVIOR"), ESearchCase::IgnoreCase) == 0)	return Result::VEHICLE_ERROR_DISABLE_BEHAVIOR;
	if (str.Compare(TEXT("VEHICLE_ERROR_COMBAT_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::VEHICLE_ERROR_COMBAT_STATUS;
	if (str.Compare(TEXT("VEHICLE_ERROR_NOT_YET_MOVE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::VEHICLE_ERROR_NOT_YET_MOVE_TIME;
	if (str.Compare(TEXT("MAIL_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_UNKNOWN;
	if (str.Compare(TEXT("MAIL_ERROR_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("MAIL_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("MAIL_ERROR_CURRENCY_EXCEEDED"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_CURRENCY_EXCEEDED;
	if (str.Compare(TEXT("MAIL_ERROR_INVALID_MAIL_BOX_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_INVALID_MAIL_BOX_TYPE;
	if (str.Compare(TEXT("MAIL_ERROR_INVALID_CATEGORY_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_INVALID_CATEGORY_TYPE;
	if (str.Compare(TEXT("MAIL_ERROR_INVALID_MAIL_DB_ID"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_INVALID_MAIL_DB_ID;
	if (str.Compare(TEXT("MAIL_ERROR_RECV_ALREADY_EXPIRED"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_RECV_ALREADY_EXPIRED;
	if (str.Compare(TEXT("MAIL_ERROR_RECV_INVENTORY_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_RECV_INVENTORY_FULL;
	if (str.Compare(TEXT("MAIL_ERROR_ALREADY_READ_MAIL"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_ALREADY_READ_MAIL;
	if (str.Compare(TEXT("MAIL_ERROR_INVALID_CURRENCY_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_INVALID_CURRENCY_TYPE;
	if (str.Compare(TEXT("MAIL_ERROR_INVALID_MAIL_STRING_ID"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_INVALID_MAIL_STRING_ID;
	if (str.Compare(TEXT("MAIL_ERROR_EMPTY_DELETED_MAIL_DB_ID_LIST"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_EMPTY_DELETED_MAIL_DB_ID_LIST;
	if (str.Compare(TEXT("MAIL_ERROR_DB_OPERATION_MAIL_LIST_DELETE"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_DB_OPERATION_MAIL_LIST_DELETE;
	if (str.Compare(TEXT("MAIL_ERROR_ALREADY_OHTER_SERVER_RECV"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_ALREADY_OHTER_SERVER_RECV;
	if (str.Compare(TEXT("MAIL_ERROR_INVALID_MAIL_TYPE_ID"), ESearchCase::IgnoreCase) == 0)	return Result::MAIL_ERROR_INVALID_MAIL_TYPE_ID;
	if (str.Compare(TEXT("SKILL_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_UNKNOWN;
	if (str.Compare(TEXT("SKILL_ERROR_INVALID_POS"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_INVALID_POS;
	if (str.Compare(TEXT("SKILL_ERROR_INVALID_INFO"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_INVALID_INFO;
	if (str.Compare(TEXT("SKILL_ERROR_IN_COOL_DOWN"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_IN_COOL_DOWN;
	if (str.Compare(TEXT("SKILL_ERROR_INVALID_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_INVALID_STATE;
	if (str.Compare(TEXT("SKILL_ERROR_DISABLED_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_DISABLED_SKILL;
	if (str.Compare(TEXT("SKILL_ERROR_OTHER_SKILL_IN_PROGRESS"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_OTHER_SKILL_IN_PROGRESS;
	if (str.Compare(TEXT("SKILL_ERROR_CANNOT_USE_PASSIVE"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_CANNOT_USE_PASSIVE;
	if (str.Compare(TEXT("SKILL_ERROR_INVALID_PREV_SKILL_INFO"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_INVALID_PREV_SKILL_INFO;
	if (str.Compare(TEXT("SKILL_ERROR_INVALID_CHAIN_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_INVALID_CHAIN_CONDITION;
	if (str.Compare(TEXT("SKILL_ERROR_HP_LESS"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_HP_LESS;
	if (str.Compare(TEXT("SKILL_ERROR_HP_GREATER"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_HP_GREATER;
	if (str.Compare(TEXT("SKILL_ERROR_MP_LESS"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_MP_LESS;
	if (str.Compare(TEXT("SKILL_ERROR_MP_GREATER"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_MP_GREATER;
	if (str.Compare(TEXT("SKILL_ERROR_ONLY_USER"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_ONLY_USER;
	if (str.Compare(TEXT("SKILL_ERROR_SP_LESS"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_SP_LESS;
	if (str.Compare(TEXT("SKILL_ERROR_NOT_ALLOWED_CLASS"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_NOT_ALLOWED_CLASS;
	if (str.Compare(TEXT("SKILL_ERROR_NOT_ALLOWED_TARGET"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_NOT_ALLOWED_TARGET;
	if (str.Compare(TEXT("SKILL_ERROR_NO_REQUIRED_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_NO_REQUIRED_ITEM;
	if (str.Compare(TEXT("SKILL_ERROR_TARGET_NOT_IN_RANGE"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_TARGET_NOT_IN_RANGE;
	if (str.Compare(TEXT("SKILL_ERROR_CURSOR_OUT_OF_RANGE"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_CURSOR_OUT_OF_RANGE;
	if (str.Compare(TEXT("SKILL_ERROR_INVALID_SKILL_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_INVALID_SKILL_ID;
	if (str.Compare(TEXT("SKILL_ERROR_ENCHANT_MAX_SKILL_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_ENCHANT_MAX_SKILL_LEVEL;
	if (str.Compare(TEXT("SKILL_ERROR_INVALID_COST_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_INVALID_COST_DATA;
	if (str.Compare(TEXT("SKILL_ERROR_INVALID_CHAIN_SKILL_HIT_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_INVALID_CHAIN_SKILL_HIT_CONDITION;
	if (str.Compare(TEXT("SKILL_ERROR_LEARN_NOT_EXIST_ROOT_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_LEARN_NOT_EXIST_ROOT_SKILL;
	if (str.Compare(TEXT("SKILL_ERROR_LEARN_INVALID_ESSENTIAL_PASSIVITY"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_LEARN_INVALID_ESSENTIAL_PASSIVITY;
	if (str.Compare(TEXT("SKILL_ERROR_LEARN_ALREADY_LEARN_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_LEARN_ALREADY_LEARN_SKILL;
	if (str.Compare(TEXT("SKILL_ERROR_NOT_MATCH_TRIGGER_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_NOT_MATCH_TRIGGER_CONDITION;
	if (str.Compare(TEXT("SKILL_ERROR_NOT_USABLE_IN_SAFE_ZONE"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_NOT_USABLE_IN_SAFE_ZONE;
	if (str.Compare(TEXT("SKILL_ERROR_UNREACHABLE_STRAIGHT"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_UNREACHABLE_STRAIGHT;
	if (str.Compare(TEXT("SKILL_ERROR_CHECK_COLLISION_FOR_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_ERROR_CHECK_COLLISION_FOR_SKILL;
	if (str.Compare(TEXT("ABNORMALITY_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::ABNORMALITY_ERROR_UNKNOWN;
	if (str.Compare(TEXT("ABNORMALITY_ERROR_IGNORE_BY_PRIORITY"), ESearchCase::IgnoreCase) == 0)	return Result::ABNORMALITY_ERROR_IGNORE_BY_PRIORITY;
	if (str.Compare(TEXT("ABNORMALITY_ERROR_EXCEED_STACK_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::ABNORMALITY_ERROR_EXCEED_STACK_COUNT;
	if (str.Compare(TEXT("RESURRECT_RESULT_FAILED_NOT_SPAWNED"), ESearchCase::IgnoreCase) == 0)	return Result::RESURRECT_RESULT_FAILED_NOT_SPAWNED;
	if (str.Compare(TEXT("RESURRECT_RESULT_FAILED_ALIVE"), ESearchCase::IgnoreCase) == 0)	return Result::RESURRECT_RESULT_FAILED_ALIVE;
	if (str.Compare(TEXT("RESURRECT_RESULT_FAILED_NOT_PAST_MIN_TICK"), ESearchCase::IgnoreCase) == 0)	return Result::RESURRECT_RESULT_FAILED_NOT_PAST_MIN_TICK;
	if (str.Compare(TEXT("QUEST_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_DATA_SHEET"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_DATA_SHEET;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_CHPATER_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_CHPATER_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_STORY_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_STORY_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_TYPE;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_FIRST_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_NOT_FIRST_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_LAST_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_NOT_LAST_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_REWARED"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_NOT_REWARED;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_STORY"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_STORY;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_PRE_EVENT_ACTION"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_PRE_EVENT_ACTION;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_POST_EVENT_ACTION"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_POST_EVENT_ACTION;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_ACTIVE_COUNT_MAX"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_ACTIVE_COUNT_MAX;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_REPEAT_RANDOM_PICK"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_REPEAT_RANDOM_PICK;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_WARP_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_WARP_STATUS;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_STATUS;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_OBJECTIVE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_OBJECTIVE;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_TELEPORT"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_TELEPORT;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_INSTANCE_FIELD"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_INSTANCE_FIELD;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_LAST_PUBLIC_FIELD"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_LAST_PUBLIC_FIELD;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_RANGE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_RANGE;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_WARP_TO_INSTANCE_FIELD"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_WARP_TO_INSTANCE_FIELD;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_UPDATE_WARP_TO_PUBLIC_FIELD"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_UPDATE_WARP_TO_PUBLIC_FIELD;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_REWARD_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_REWARD_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_REWARD_NOT_PROCESS_POST_EVENT_ACTION"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_REWARD_NOT_PROCESS_POST_EVENT_ACTION;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_REWARD_NOT_LAST_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_REWARD_NOT_LAST_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_ALREADY_REWARD_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_ALREADY_REWARD_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_PRE_EVENT_ACTION"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_PRE_EVENT_ACTION;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_POST_EVENT_ACTION"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_POST_EVENT_ACTION;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_REWARDED"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_REWARDED;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_ENOUGH_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_ENOUGH_LEVEL;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_ENOUGH_MAIN_STORY_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_ENOUGH_MAIN_STORY_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_WARP_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_WARP_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE_NOT_ACCEPTED"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE_NOT_ACCEPTED;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("QUEST_ERROR_GIVE_UP_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GIVE_UP_STATE;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_REWARD_QUEST_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_REWARD_QUEST_TYPE;
	if (str.Compare(TEXT("QUEST_ERROR_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("QUEST_ERROR_ALREADY_COMPLETE_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_ALREADY_COMPLETE_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_RPOGRESS_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_RPOGRESS_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_USER_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_USER_DATA;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_ACCEPTABLE_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_ACCEPTABLE_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_REFRESH_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_REFRESH_COUNT;
	if (str.Compare(TEXT("QUEST_ERROR_QUEST_BOARD_NOT_QUEST_BOARD_NPC"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_QUEST_BOARD_NOT_QUEST_BOARD_NPC;
	if (str.Compare(TEXT("QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_ACCEPT_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_ACCEPT_COUNT;
	if (str.Compare(TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_NPC"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_QUEST_BOARD_INVALID_NPC;
	if (str.Compare(TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_ID;
	if (str.Compare(TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_DATA;
	if (str.Compare(TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_QUEST_BOARD_INVALID_TYPE;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_QUEST_INDEX"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_QUEST_INDEX;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_SCROLL_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_SCROLL_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_REMAIN_REFRESH"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_REMAIN_REFRESH;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_WARP_NOT_MAP_WORLD"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_WARP_NOT_MAP_WORLD;
	if (str.Compare(TEXT("QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID_NOT_POP_UP"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID_NOT_POP_UP;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_VOLUME"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_VOLUME;
	if (str.Compare(TEXT("WARP_ERROR_NOT_IN_VOLUME"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NOT_IN_VOLUME;
	if (str.Compare(TEXT("WARP_ERROR_LEAVE_INSTANCE_SINGLE"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_LEAVE_INSTANCE_SINGLE;
	if (str.Compare(TEXT("WARP_ERROR_IN_COMBAT"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_IN_COMBAT;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_SPOT_INFO"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_SPOT_INFO;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_MAP;
	if (str.Compare(TEXT("WARP_ERROR_NOT_SPAWNED"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NOT_SPAWNED;
	if (str.Compare(TEXT("WARP_ERROR_NOT_ALIVE"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NOT_ALIVE;
	if (str.Compare(TEXT("WARP_ERROR_CAN_NOT_DO_ACTION_WARP"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_CAN_NOT_DO_ACTION_WARP;
	if (str.Compare(TEXT("WARP_ERROR_NOT_ENOUGH_DUNGEON_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NOT_ENOUGH_DUNGEON_TIME;
	if (str.Compare(TEXT("WARP_ERROR_NOT_OPEN_DUNGEON_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NOT_OPEN_DUNGEON_TIME;
	if (str.Compare(TEXT("WARP_ERROR_NOT_ENOUGH_DUNGEON_COST"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NOT_ENOUGH_DUNGEON_COST;
	if (str.Compare(TEXT("WARP_ERROR_NEED_INSTANCE_SINGLE_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NEED_INSTANCE_SINGLE_MAP;
	if (str.Compare(TEXT("WARP_ERROR_TOPO_MAP_NOT_EXIST"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_TOPO_MAP_NOT_EXIST;
	if (str.Compare(TEXT("WARP_ERROR_RANDOM_POS_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_RANDOM_POS_FAILED;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_MAP_ID"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_MAP_ID;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_CHANNEL_ID"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_CHANNEL_ID;
	if (str.Compare(TEXT("WARP_ERROR_CHANNEL_CONGESTION_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_CHANNEL_CONGESTION_FULL;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_MAIN_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_MAIN_QUEST;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_LEVEL;
	if (str.Compare(TEXT("WARP_ERROR_WAIT_ACK_DESPAWN_READYING"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_WAIT_ACK_DESPAWN_READYING;
	if (str.Compare(TEXT("WARP_ERROR_NOT_OPEN_INVADE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NOT_OPEN_INVADE_TIME;
	if (str.Compare(TEXT("WARP_ERROR_NOT_ENOUGH_INVADE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NOT_ENOUGH_INVADE_TIME;
	if (str.Compare(TEXT("WARP_ERROR_NOT_ENOUGH_INVADE_COST"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_NOT_ENOUGH_INVADE_COST;
	if (str.Compare(TEXT("WARP_ERROR_WORLD_BOSS_OPEN"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_WORLD_BOSS_OPEN;
	if (str.Compare(TEXT("WARP_ERROR_WORLD_BOSS_ENOUGH_COST"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_WORLD_BOSS_ENOUGH_COST;
	if (str.Compare(TEXT("WARP_ERROR_WORLD_BOSS_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_WORLD_BOSS_INVALID_STATUS;
	if (str.Compare(TEXT("WARP_ERROR_WORLD_BOSS_ALREADY_ENTER"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_WORLD_BOSS_ALREADY_ENTER;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_SPAWN_GROUP_ID"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_SPAWN_GROUP_ID;
	if (str.Compare(TEXT("WARP_ERROR_LEAVE_INSTANCE_SINGLE_DELAY_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_LEAVE_INSTANCE_SINGLE_DELAY_TIME;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_NPC"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_NPC;
	if (str.Compare(TEXT("WARP_ERROR_INVALID_NPC_IN_SPAWN_GROUP"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_INVALID_NPC_IN_SPAWN_GROUP;
	if (str.Compare(TEXT("WARP_ERROR_UNLOCK"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_UNLOCK;
	if (str.Compare(TEXT("WARP_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::WARP_ERROR_UNKNOWN;
	if (str.Compare(TEXT("PARTY_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_UNKNOWN;
	if (str.Compare(TEXT("PARTY_ERROR_INVALID_PARTY"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_INVALID_PARTY;
	if (str.Compare(TEXT("PARTY_ERROR_ALREADY_PARTY_MEMBER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_ALREADY_PARTY_MEMBER;
	if (str.Compare(TEXT("PARTY_ERROR_ALREADY_PARTY_INVITED"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_ALREADY_PARTY_INVITED;
	if (str.Compare(TEXT("PARTY_ERROR_NOT_JOINED"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_NOT_JOINED;
	if (str.Compare(TEXT("PARTY_ERROR_NOT_LEADER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_NOT_LEADER;
	if (str.Compare(TEXT("PARTY_ERROR_NOT_MEMBER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_NOT_MEMBER;
	if (str.Compare(TEXT("PARTY_ERROR_NOT_FOUND_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_NOT_FOUND_USER;
	if (str.Compare(TEXT("PARTY_ERROR_NOT_LOGIN_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_NOT_LOGIN_USER;
	if (str.Compare(TEXT("PARTY_ERROR_MEMBER_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_MEMBER_FULL;
	if (str.Compare(TEXT("PARTY_ERROR_NOT_INVITED"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_NOT_INVITED;
	if (str.Compare(TEXT("PARTY_ERROR_CANNOT_INVITE_SELF"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_CANNOT_INVITE_SELF;
	if (str.Compare(TEXT("PARTY_ERROR_CANNOT_KICK_SELF"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_CANNOT_KICK_SELF;
	if (str.Compare(TEXT("PARTY_ERROR_ALREADY_LEADER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_ALREADY_LEADER;
	if (str.Compare(TEXT("PARTY_ERROR_INVALID_DROP_OWNERSHIP_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_INVALID_DROP_OWNERSHIP_TYPE;
	if (str.Compare(TEXT("PARTY_ERROR_WARP_TO_LEADER_MYSELF"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_WARP_TO_LEADER_MYSELF;
	if (str.Compare(TEXT("PARTY_ERROR_INVALID_LEADER_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_INVALID_LEADER_MAP;
	if (str.Compare(TEXT("PARTY_ERROR_INVALID_LEADER_MAP_SPWAN_SPOT"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_INVALID_LEADER_MAP_SPWAN_SPOT;
	if (str.Compare(TEXT("PARTY_ERROR_LEADER_IN_INSTANCE_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_LEADER_IN_INSTANCE_MAP;
	if (str.Compare(TEXT("PARTY_ERROR_NOT_ALL_READY"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_NOT_ALL_READY;
	if (str.Compare(TEXT("PARTY_ERROR_USER_IN_INSTANCE_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_USER_IN_INSTANCE_MAP;
	if (str.Compare(TEXT("PARTY_ERROR_SAME_PARTY"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_SAME_PARTY;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_ALREADY_PARTY_MATCHING_WAIT"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_ALREADY_PARTY_MATCHING_WAIT;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_NOT_PARTY_MATCHING_WAIT"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_NOT_PARTY_MATCHING_WAIT;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_MATCHING_TIMEOUT"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_MATCHING_TIMEOUT;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_NOT_PLAYING"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_NOT_PLAYING;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_NOT_HELPCALL"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_NOT_HELPCALL;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_INVALID_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_INVALID_MAP;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_NO_REMAIN_HELP_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_NO_REMAIN_HELP_COUNT;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_ALREADY_HELP_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_ALREADY_HELP_STATUS;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_PLAYING"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_PLAYING;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_JUST_BEFORE_END"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_JUST_BEFORE_END;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY_MEMBER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY_MEMBER;
	if (str.Compare(TEXT("PARTY_ERROR_NOT_INVITABLE_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_NOT_INVITABLE_USER;
	if (str.Compare(TEXT("PARTY_ERROR_NOT_INVITABLE_OTHER_SERVER_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_NOT_INVITABLE_OTHER_SERVER_USER;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_CANNOT_INVITE"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_CANNOT_INVITE;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_CANNOT_INTER_SERVER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_CANNOT_INTER_SERVER;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_INVALID_AREA"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_INVALID_AREA;
	if (str.Compare(TEXT("PARTY_ERROR_INVALID_HOME_WORLD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_INVALID_HOME_WORLD_ID;
	if (str.Compare(TEXT("PARTY_ERROR_CANNOT_INVITE_OTHER_ENTER_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_ERROR_CANNOT_INVITE_OTHER_ENTER_USER;
	if (str.Compare(TEXT("CHANGE_WEAPON_TYPE_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::CHANGE_WEAPON_TYPE_ERROR_UNKNOWN;
	if (str.Compare(TEXT("CHANGE_WEAPON_TYPE_ERROR_INVALID_USER_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::CHANGE_WEAPON_TYPE_ERROR_INVALID_USER_STATE;
	if (str.Compare(TEXT("CHANGE_WEAPON_TYPE_ERROR_INVALID_DURATION"), ESearchCase::IgnoreCase) == 0)	return Result::CHANGE_WEAPON_TYPE_ERROR_INVALID_DURATION;
	if (str.Compare(TEXT("CHANGE_WEAPON_TYPE_ERROR_IN_CHANGE_CLASS"), ESearchCase::IgnoreCase) == 0)	return Result::CHANGE_WEAPON_TYPE_ERROR_IN_CHANGE_CLASS;
	if (str.Compare(TEXT("CHANGE_WEAPON_TYPE_ERROR_COOLDOWN_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::CHANGE_WEAPON_TYPE_ERROR_COOLDOWN_TIME;
	if (str.Compare(TEXT("PRESET_ERROR_COOLDOWN_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::PRESET_ERROR_COOLDOWN_TIME;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_MAP;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_SPOT"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_SPOT;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_REQUEST"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_REQUEST;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_MAIN_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_MAIN_QUEST;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_QUESTID"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_QUESTID;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_POST_EVENT_ACTION_QUEST_NOT_COMPLETED"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_POST_EVENT_ACTION_QUEST_NOT_COMPLETED;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_INDEX"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_INDEX;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_EVENT_ACTION_GROUP_ID"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_EVENT_ACTION_GROUP_ID;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_INVALID_PARAM"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_INVALID_PARAM;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_ALL_COMPLETED"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_ALL_COMPLETED;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_NOT_ALIVE"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_NOT_ALIVE;
	if (str.Compare(TEXT("EVENT_ACTION_ERROR_USER_WARP"), ESearchCase::IgnoreCase) == 0)	return Result::EVENT_ACTION_ERROR_USER_WARP;
	if (str.Compare(TEXT("TERRITORY_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::TERRITORY_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("TERRITORY_ERROR_INVALID_ID"), ESearchCase::IgnoreCase) == 0)	return Result::TERRITORY_ERROR_INVALID_ID;
	if (str.Compare(TEXT("TERRITORY_ERROR_INVALID_POSITION"), ESearchCase::IgnoreCase) == 0)	return Result::TERRITORY_ERROR_INVALID_POSITION;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_ERROR_UNKNOWN;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_ERROR_INVALID_USER_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_ERROR_INVALID_USER_STATE;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_ERROR_INVALID_DROP_OBJECT"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_ERROR_INVALID_DROP_OBJECT;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_ERROR_INVALID_DISTANCE"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_ERROR_INVALID_DISTANCE;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_ERROR_ALREADY_IN_LOOT"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_ERROR_ALREADY_IN_LOOT;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_ERROR_NOT_OWNERSHIP"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_ERROR_NOT_OWNERSHIP;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_ERROR_NOT_MATCH_GAME_OPTION"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_ERROR_NOT_MATCH_GAME_OPTION;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_ERROR_NOT_MY_ITEM_PARTY_LOOT"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_ERROR_NOT_MY_ITEM_PARTY_LOOT;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_GUILD_STORE_OWNERSHIP"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_GUILD_STORE_OWNERSHIP;
	if (str.Compare(TEXT("DROP_OBJECT_LOOT_ERROR_TRY_AGAIN"), ESearchCase::IgnoreCase) == 0)	return Result::DROP_OBJECT_LOOT_ERROR_TRY_AGAIN;
	if (str.Compare(TEXT("BOOKMARK_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_UNKNOWN;
	if (str.Compare(TEXT("BOOKMARK_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("BOOKMARK_ERROR_LIST_MAX"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_LIST_MAX;
	if (str.Compare(TEXT("BOOKMARK_ERROR_INVALID_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_INVALID_MAP;
	if (str.Compare(TEXT("BOOKMARK_ERROR_INVALID_TITLE_LEN"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_INVALID_TITLE_LEN;
	if (str.Compare(TEXT("BOOKMARK_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("BOOKMARK_ERROR_TELEPORT_INVALID_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_TELEPORT_INVALID_ITEM;
	if (str.Compare(TEXT("BOOKMARK_ERROR_INVALID_SHARE_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_INVALID_SHARE_TYPE;
	if (str.Compare(TEXT("BOOKMARK_ERROR_NOT_GUILD_MEMBER"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_NOT_GUILD_MEMBER;
	if (str.Compare(TEXT("BOOKMARK_ERROR_NO_SHARE_RIGHT"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_NO_SHARE_RIGHT;
	if (str.Compare(TEXT("BOOKMARK_ERROR_CANNOT_INTER_SERVER"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_CANNOT_INTER_SERVER;
	if (str.Compare(TEXT("BOOKMARK_ERROR_NOT_SHARE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_NOT_SHARE_TIME;
	if (str.Compare(TEXT("BOOKMARK_ERROR_NO_PARTY_MEMBERS"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_NO_PARTY_MEMBERS;
	if (str.Compare(TEXT("BOOKMARK_ERROR_NO_GUILD_MEMBERS"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_NO_GUILD_MEMBERS;
	if (str.Compare(TEXT("BOOKMARK_ERROR_INVALID_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_INVALID_TYPE;
	if (str.Compare(TEXT("BOOKMARK_ERROR_ALREADY_REGISTERED"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_ALREADY_REGISTERED;
	if (str.Compare(TEXT("BOOKMARK_ERROR_CANNOT_WARP_INTER_SERVER"), ESearchCase::IgnoreCase) == 0)	return Result::BOOKMARK_ERROR_CANNOT_WARP_INTER_SERVER;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_STATE;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_NPC"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_NPC;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_NPC_RANGE"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_NPC_RANGE;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_PROP_START_USERCOUNT_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_PROP_START_USERCOUNT_OVER;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_PROP_MAX_USERCOUNT_LIMIT_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_PROP_MAX_USERCOUNT_LIMIT_OVER;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_PROP_START_ALREADY"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_PROP_START_ALREADY;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_PROP_NOT_ENDABLE"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_PROP_NOT_ENDABLE;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_PROP_STATUS_QUEST_NOT_ENDABLE"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_PROP_STATUS_QUEST_NOT_ENDABLE;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_PROP_STATUS_NOT_CANCEL_ABLE"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_PROP_STATUS_NOT_CANCEL_ABLE;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_PROP"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_PROP;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_QUEST_PROP"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_QUEST_PROP;
	if (str.Compare(TEXT("INTERACT_ERROR_NOT_SPAWNED_PROP"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_NOT_SPAWNED_PROP;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_PROP_RANGE"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_PROP_RANGE;
	if (str.Compare(TEXT("INTERACT_ERROR_DEACTIVATE_EVENT_PRAYER"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_DEACTIVATE_EVENT_PRAYER;
	if (str.Compare(TEXT("INTERACT_ERROR_INVADE_PORTAL_PROP_REGISTRATION_ALREADY"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVADE_PORTAL_PROP_REGISTRATION_ALREADY;
	if (str.Compare(TEXT("INTERACT_ERROR_INVALID_PROP_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_INVALID_PROP_TYPE;
	if (str.Compare(TEXT("INTERACT_ERROR_NOT_ALIVE"), ESearchCase::IgnoreCase) == 0)	return Result::INTERACT_ERROR_NOT_ALIVE;
	if (str.Compare(TEXT("BONUS_STAT_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::BONUS_STAT_ERROR_UNKNOWN;
	if (str.Compare(TEXT("BONUS_STAT_ERROR_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::BONUS_STAT_ERROR_INVALID_USER_STATUS;
	if (str.Compare(TEXT("BONUS_STAT_ERROR_INVALID_PKT_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::BONUS_STAT_ERROR_INVALID_PKT_DATA;
	if (str.Compare(TEXT("BONUS_STAT_ERROR_INVALID_STAT_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::BONUS_STAT_ERROR_INVALID_STAT_TYPE;
	if (str.Compare(TEXT("BONUS_STAT_ERROR_DB_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::BONUS_STAT_ERROR_DB_FAILED;
	if (str.Compare(TEXT("BONUS_STAT_ERROR_DUPLICATE_STAT_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::BONUS_STAT_ERROR_DUPLICATE_STAT_TYPE;
	if (str.Compare(TEXT("BONUS_STAT_ERROR_NOT_ENOUGH_STAT_POINT"), ESearchCase::IgnoreCase) == 0)	return Result::BONUS_STAT_ERROR_NOT_ENOUGH_STAT_POINT;
	if (str.Compare(TEXT("BONUS_STAT_ERROR_MAX_STAT"), ESearchCase::IgnoreCase) == 0)	return Result::BONUS_STAT_ERROR_MAX_STAT;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_UNKNOWN;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_INVALID_USER_STATUS;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_INVALID_PKT_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_INVALID_PKT_DATA;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_INVALID_SKILL_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_INVALID_SKILL_ID;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_SAME_SLOT"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_SAME_SLOT;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_INVALID_SKILL_SLOT_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_INVALID_SKILL_SLOT_ID;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_INVALID_WEAPON_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_INVALID_WEAPON_TYPE;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_SAME_AUTO_FLAG"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_SAME_AUTO_FLAG;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_NOT_RESIST_SLOT"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_NOT_RESIST_SLOT;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_DB_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_DB_FAILED;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_DUPLICATE_SKILL_GROUP"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_DUPLICATE_SKILL_GROUP;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_SAME_INTERVAL_VALUE"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_SAME_INTERVAL_VALUE;
	if (str.Compare(TEXT("SKILL_SLOT_ERROR_OVER_INTERVAL_SECOND"), ESearchCase::IgnoreCase) == 0)	return Result::SKILL_SLOT_ERROR_OVER_INTERVAL_SECOND;
	if (str.Compare(TEXT("USER_OPTIPON_ERROR_DB_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::USER_OPTIPON_ERROR_DB_FAILED;
	if (str.Compare(TEXT("PORTAL_WARP_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::PORTAL_WARP_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("PORTAL_WARP_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::PORTAL_WARP_ERROR_UNKNOWN;
	if (str.Compare(TEXT("PORTAL_WARP_ERROR_INVALID_SPOT_INFO"), ESearchCase::IgnoreCase) == 0)	return Result::PORTAL_WARP_ERROR_INVALID_SPOT_INFO;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_UNKNOWN;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_INVALID_USER_STATUS;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_OVERFLOW_MAX_PAGE_SIZE"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_OVERFLOW_MAX_PAGE_SIZE;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_ITEM_NOT_EXIST_TARGET"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_ITEM_NOT_EXIST_TARGET;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_INVALID_QUICK_SLOT_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_INVALID_QUICK_SLOT_ID;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_QUICK_SLOT_ID_DIFF_ITEM_DB_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_QUICK_SLOT_ID_DIFF_ITEM_DB_ID;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_SET_DB_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_SET_DB_ERROR;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_AUTO_DB_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_AUTO_DB_ERROR;
	if (str.Compare(TEXT("QUICK_SLOT_ERROR_INVALID_ITEM_TYPE_AUTO"), ESearchCase::IgnoreCase) == 0)	return Result::QUICK_SLOT_ERROR_INVALID_ITEM_TYPE_AUTO;
	if (str.Compare(TEXT("FAIRY_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("FAIRY_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ERROR_UNKNOWN;
	if (str.Compare(TEXT("FAIRY_ERROR_INVALID_FAIRY_ID"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ERROR_INVALID_FAIRY_ID;
	if (str.Compare(TEXT("FAIRY_ERROR_NOT_HAVE_FAIRY"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ERROR_NOT_HAVE_FAIRY;
	if (str.Compare(TEXT("FAIRY_ERROR_NOT_ENOUGH_FAIRY"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ERROR_NOT_ENOUGH_FAIRY;
	if (str.Compare(TEXT("FAIRY_ERROR_MAX_AMOUNT_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ERROR_MAX_AMOUNT_OVER;
	if (str.Compare(TEXT("FAIRY_ERROR_ALREADY_EQUIP_FAIRY"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ERROR_ALREADY_EQUIP_FAIRY;
	if (str.Compare(TEXT("FAIRY_ERROR_INVALID_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ERROR_INVALID_DATA;
	if (str.Compare(TEXT("FAIRY_ERROR_INVALID_PACKET_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ERROR_INVALID_PACKET_DATA;
	if (str.Compare(TEXT("FAIRY_SUMMON_ERROR_INVALID_FAIRY_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_SUMMON_ERROR_INVALID_FAIRY_GRADE;
	if (str.Compare(TEXT("FAIRY_SUMMON_ERROR_MAX_CONFIRM_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_SUMMON_ERROR_MAX_CONFIRM_COUNT;
	if (str.Compare(TEXT("FAIRY_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM;
	if (str.Compare(TEXT("FAIRY_CONFRIM_ERROR_INVALID_FAIRY_CONFIRM_ID"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_CONFRIM_ERROR_INVALID_FAIRY_CONFIRM_ID;
	if (str.Compare(TEXT("FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT;
	if (str.Compare(TEXT("FAIRY_CONFRIM_ERROR_EXPIRED"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_CONFRIM_ERROR_EXPIRED;
	if (str.Compare(TEXT("FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("FAIRY_ENCHANT_ERROR_MAX_LEVEL_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ENCHANT_ERROR_MAX_LEVEL_OVER;
	if (str.Compare(TEXT("FAIRY_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL;
	if (str.Compare(TEXT("FAIRY_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("FAIRY_COMPOSE_ERROR_NOT_SAME_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_COMPOSE_ERROR_NOT_SAME_GRADE;
	if (str.Compare(TEXT("FAIRY_COMPOSE_ERROR_MAX_COMPOSE_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_COMPOSE_ERROR_MAX_COMPOSE_COUNT;
	if (str.Compare(TEXT("FAIRY_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT;
	if (str.Compare(TEXT("FAIRY_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("FAIRY_COMPOSE_ERROR_LEGEND_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::FAIRY_COMPOSE_ERROR_LEGEND_GRADE;
	if (str.Compare(TEXT("CHAT_ERROR_INVALID_CHAT_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_INVALID_CHAT_TYPE;
	if (str.Compare(TEXT("CHAT_ERROR_MESSAGE_LEN_ZERO"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_MESSAGE_LEN_ZERO;
	if (str.Compare(TEXT("CHAT_ERROR_MESSAGE_LEN_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_MESSAGE_LEN_OVER;
	if (str.Compare(TEXT("CHAT_ERROR_NOT_FOUND_USER"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_NOT_FOUND_USER;
	if (str.Compare(TEXT("CHAT_ERROR_CAN_NOT_SEND_TO_ME"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_CAN_NOT_SEND_TO_ME;
	if (str.Compare(TEXT("CHAT_ERROR_DETECTED_AS_SPAM"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_DETECTED_AS_SPAM;
	if (str.Compare(TEXT("CHAT_ERROR_EXCEED_FREE_INVADE_CHAT"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_EXCEED_FREE_INVADE_CHAT;
	if (str.Compare(TEXT("CHAT_ERROR_RESET_FREE_INVADE_CHAT"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_RESET_FREE_INVADE_CHAT;
	if (str.Compare(TEXT("CHAT_ERROR_USER_BANNED"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_USER_BANNED;
	if (str.Compare(TEXT("CHAT_ERROR_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_SERVER_CONNECTION;
	if (str.Compare(TEXT("CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_TYPE;
	if (str.Compare(TEXT("CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_AMOUNT"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_AMOUNT;
	if (str.Compare(TEXT("CHAT_ERROR_PAID_INVADE_CHAT_NOT_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_PAID_INVADE_CHAT_NOT_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("CHAT_ERROR_PAID_INVADE_CHAT_NO_USER_IN_WORLD"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_PAID_INVADE_CHAT_NO_USER_IN_WORLD;
	if (str.Compare(TEXT("CHAT_ERROR_PAID_INVADE_CHAT_FAIL_REQ_PAY"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_PAID_INVADE_CHAT_FAIL_REQ_PAY;
	if (str.Compare(TEXT("CHAT_ERROR_INVADE_CHAT_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_INVADE_CHAT_LEVEL;
	if (str.Compare(TEXT("CHAT_ERROR_INVADE_CHAT_DB_UPDATE_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_INVADE_CHAT_DB_UPDATE_ERROR;
	if (str.Compare(TEXT("CHAT_ERROR_INTER_CHAT_NOT_INTER_SERVER_USER"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_INTER_CHAT_NOT_INTER_SERVER_USER;
	if (str.Compare(TEXT("CHAT_ERROR_INTER_CHAT_NO_INTER_SERVER_SESSION"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_ERROR_INTER_CHAT_NO_INTER_SERVER_SESSION;
	if (str.Compare(TEXT("RESURRECT_ERROR_INVALID_RESURRECT_ID"), ESearchCase::IgnoreCase) == 0)	return Result::RESURRECT_ERROR_INVALID_RESURRECT_ID;
	if (str.Compare(TEXT("RESURRECT_ERROR_COST_VALUE"), ESearchCase::IgnoreCase) == 0)	return Result::RESURRECT_ERROR_COST_VALUE;
	if (str.Compare(TEXT("RESURRECT_ERROR_INVASIONOUT"), ESearchCase::IgnoreCase) == 0)	return Result::RESURRECT_ERROR_INVASIONOUT;
	if (str.Compare(TEXT("DEATH_ERROR_INVALID_ID"), ESearchCase::IgnoreCase) == 0)	return Result::DEATH_ERROR_INVALID_ID;
	if (str.Compare(TEXT("DEATH_ERROR_RECOVERY_GOLD_COST"), ESearchCase::IgnoreCase) == 0)	return Result::DEATH_ERROR_RECOVERY_GOLD_COST;
	if (str.Compare(TEXT("DEATH_ERROR_RECOVERY_DIA_COST"), ESearchCase::IgnoreCase) == 0)	return Result::DEATH_ERROR_RECOVERY_DIA_COST;
	if (str.Compare(TEXT("DEATH_ERRORR_RECOVERY_ITEM_COST"), ESearchCase::IgnoreCase) == 0)	return Result::DEATH_ERRORR_RECOVERY_ITEM_COST;
	if (str.Compare(TEXT("DEATH_ERROR_RECOVERY_TIME_OUT"), ESearchCase::IgnoreCase) == 0)	return Result::DEATH_ERROR_RECOVERY_TIME_OUT;
	if (str.Compare(TEXT("DEATH_ERROR_RECOVERY_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::DEATH_ERROR_RECOVERY_COUNT;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_INVALID_NAME_LENGTH"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_INVALID_NAME_LENGTH;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_INVALID_DESC_LENGTH"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_INVALID_DESC_LENGTH;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_NOT_ALLOWED_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_NOT_ALLOWED_NAME;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_DUPLICATION_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_DUPLICATION_NAME;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_HAD_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_HAD_GUILD;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_NO_GOLD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_NO_GOLD;
	if (str.Compare(TEXT("GUILD_ERROR_HAD_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_HAD_GUILD;
	if (str.Compare(TEXT("GUILD_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_DB;
	if (str.Compare(TEXT("GUILD_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_UNKNOWN;
	if (str.Compare(TEXT("GUILD_ERROR_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("GUILD_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("GUILD_DELETE_ERROR_MEMBER_CNT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DELETE_ERROR_MEMBER_CNT;
	if (str.Compare(TEXT("GUILD_ERROR_NOT_EXSIT_MEMEBER"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_NOT_EXSIT_MEMEBER;
	if (str.Compare(TEXT("GUILD_ERROR_NOT_REQUEST_JOIN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_NOT_REQUEST_JOIN;
	if (str.Compare(TEXT("GUILD_JOIN_ERROR_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_JOIN_ERROR_FULL;
	if (str.Compare(TEXT("GUILD_JOIN_ERROR_COOLDOWN_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_JOIN_ERROR_COOLDOWN_TIME;
	if (str.Compare(TEXT("GUILD_JOIN_ERROR_INVALID_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_JOIN_ERROR_INVALID_GUILD;
	if (str.Compare(TEXT("GUILD_JOIN_ERROR_USER_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_JOIN_ERROR_USER_LEVEL;
	if (str.Compare(TEXT("GUILD_LEAVE_ERROR_CAPTAIN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_LEAVE_ERROR_CAPTAIN;
	if (str.Compare(TEXT("GUILD_ERROR_FULL_GRADE_NUM"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_FULL_GRADE_NUM;
	if (str.Compare(TEXT("GUILD_DELETE_ERROR_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DELETE_ERROR_GRADE;
	if (str.Compare(TEXT("GUILD_ERROR_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_GRADE;
	if (str.Compare(TEXT("GUILD_ERROR_NOT_FIND"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_NOT_FIND;
	if (str.Compare(TEXT("GUILD_ERROR_DAILY_DONATE_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_DAILY_DONATE_OVER;
	if (str.Compare(TEXT("GUILD_ERROR_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("GUILD_ERROR_GUILD_COIN_FULLY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_GUILD_COIN_FULLY;
	if (str.Compare(TEXT("GUILD_ERROR_NOT_ENOUGH_GUILD_ASSET"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_NOT_ENOUGH_GUILD_ASSET;
	if (str.Compare(TEXT("GUILD_ERROR_ATTENDANCE_ALREADY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_ATTENDANCE_ALREADY;
	if (str.Compare(TEXT("GUILD_GRADE_ERROR_INAVLID_MEMBER"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_GRADE_ERROR_INAVLID_MEMBER;
	if (str.Compare(TEXT("GUILD_GRADE_ERROR_DO_NOT_CHANGE_GRADE_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_GRADE_ERROR_DO_NOT_CHANGE_GRADE_LEVEL;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_INAVLID_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_INAVLID_ID;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_DB"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_DB;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_AUTH"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_AUTH;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_OPEN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_OPEN;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_UPGRADE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_UPGRADE;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_DATE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_DATE;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_LEVEL;
	if (str.Compare(TEXT("GUILD_ERROR_GUILD_CAMP_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_GUILD_CAMP_LEVEL;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_MAX_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_MAX_LEVEL;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_ASSET"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_ASSET;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_PASSIVITY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_PASSIVITY;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_UNAUTHORIZED_LEVELUP"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_UNAUTHORIZED_LEVELUP;
	if (str.Compare(TEXT("GUILD_ERROR_UNABLE_GUILD_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_UNABLE_GUILD_LEVEL;
	if (str.Compare(TEXT("GUILD_BUY_SHOP_ERROR_UNABLE_GUILD_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_BUY_SHOP_ERROR_UNABLE_GUILD_LEVEL;
	if (str.Compare(TEXT("GUILD_BUY_SHOP_INVALID_PRODUCT_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_BUY_SHOP_INVALID_PRODUCT_ID;
	if (str.Compare(TEXT("GUILD_BUY_SHOP_ERROR_OVER_CNT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_BUY_SHOP_ERROR_OVER_CNT;
	if (str.Compare(TEXT("GUILD_SHOP_ERROR_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_SHOP_ERROR_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("GUILD_ERROR_CHANGE_MEMBER_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_CHANGE_MEMBER_GRADE;
	if (str.Compare(TEXT("GUILD_JOIN_ERROR_FULL_OF_APPLICANTS"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_JOIN_ERROR_FULL_OF_APPLICANTS;
	if (str.Compare(TEXT("GUILD_JOIN_ERROR_OVER_REQUEST_CNT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_JOIN_ERROR_OVER_REQUEST_CNT;
	if (str.Compare(TEXT("GUILD_ACTIVITY_ERROR_GUILD_JOIN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ACTIVITY_ERROR_GUILD_JOIN;
	if (str.Compare(TEXT("GUILD_ERROR_EXIST_HOSTILE_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_EXIST_HOSTILE_GUILD;
	if (str.Compare(TEXT("GUILD_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_INVALID_USER;
	if (str.Compare(TEXT("GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID;
	if (str.Compare(TEXT("GUILD_ERROR_ALLY_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_ALLY_GUILD_ID;
	if (str.Compare(TEXT("GUILD_ERROR_ALLY_REQUEST_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_ALLY_REQUEST_GUILD_ID;
	if (str.Compare(TEXT("GUILD_ERROR_UNKNOWN_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_UNKNOWN_GUILD_ID;
	if (str.Compare(TEXT("GUILD_ERROR_MAX_REQUESTED"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_MAX_REQUESTED;
	if (str.Compare(TEXT("GUILD_JOIN_ERROR_UNABLE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_JOIN_ERROR_UNABLE;
	if (str.Compare(TEXT("GUILD_JOIN_ERROR_EXIST_LIST"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_JOIN_ERROR_EXIST_LIST;
	if (str.Compare(TEXT("GUILD_ERROR_NOT_JOIN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_NOT_JOIN;
	if (str.Compare(TEXT("GUILD_ERROR_INVALID_GUILD_CHAT_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_INVALID_GUILD_CHAT_ID;
	if (str.Compare(TEXT("GUILD_SEARCH_ERROR_COOLDOWN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_SEARCH_ERROR_COOLDOWN;
	if (str.Compare(TEXT("GUILD_ERROR_GUILD_ASSET_FULLY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_GUILD_ASSET_FULLY;
	if (str.Compare(TEXT("GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_HOSTILE_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_HOSTILE_GUILD;
	if (str.Compare(TEXT("GUILD_SEARCH_DILPLOMACY_ERROR_REQUEST_ALLY_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_SEARCH_DILPLOMACY_ERROR_REQUEST_ALLY_GUILD;
	if (str.Compare(TEXT("GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_ALLY_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_ALLY_GUILD;
	if (str.Compare(TEXT("GUILD_SEARCH_ERROR_MATCHING"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_SEARCH_ERROR_MATCHING;
	if (str.Compare(TEXT("GUILD_ERROR_EXIST_ALLY_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_EXIST_ALLY_GUILD;
	if (str.Compare(TEXT("GUILD_ERROR_EXIST_ALLY_REQUEST_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_EXIST_ALLY_REQUEST_GUILD;
	if (str.Compare(TEXT("GUILD_ERROR_SAME_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_SAME_GUILD_ID;
	if (str.Compare(TEXT("GUILD_ERROR_UNAUTHORIZED_DIPLOMACY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_UNAUTHORIZED_DIPLOMACY;
	if (str.Compare(TEXT("GUILD_ERROR_HOSTILE_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_HOSTILE_GUILD_ID;
	if (str.Compare(TEXT("GUILD_ERROR_ERROR_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_ERROR_FULL;
	if (str.Compare(TEXT("GUILD_ERROR_OPPONENT_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_OPPONENT_FULL;
	if (str.Compare(TEXT("GUILD_ERROR_BUILDING_REACTIVATE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_BUILDING_REACTIVATE_TIME;
	if (str.Compare(TEXT("GUILD_GIFT_ERROR_INVALID_ITEM_EFFECT_PARAM"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_GIFT_ERROR_INVALID_ITEM_EFFECT_PARAM;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_INSERT_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_INSERT_GUILD;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_INSERT_GUILD_ROW_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_INSERT_GUILD_ROW_COUNT;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_INSERT_GUILD_MEMBER"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_INSERT_GUILD_MEMBER;
	if (str.Compare(TEXT("GUILD_CREATE_ERROR_DELETE_GUILD_REQUEST_JOIN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_CREATE_ERROR_DELETE_GUILD_REQUEST_JOIN;
	if (str.Compare(TEXT("GUILD_DUNGEON_WARP_ERROR_NOT_ENOUGH_TICKET"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WARP_ERROR_NOT_ENOUGH_TICKET;
	if (str.Compare(TEXT("GUILD_DUNGEON_WARP_ERROR_IN_GUILD_DUNGEON_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WARP_ERROR_IN_GUILD_DUNGEON_MAP;
	if (str.Compare(TEXT("GUILD_DUNGEON_WARP_ERROR_STORE_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WARP_ERROR_STORE_FULL;
	if (str.Compare(TEXT("GUILD_DUNGEON_WARP_ERROR_DONT_DESTROY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WARP_ERROR_DONT_DESTROY;
	if (str.Compare(TEXT("GUILD_DUNGEON_WARP_ERROR_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WARP_ERROR_LEVEL;
	if (str.Compare(TEXT("GUILD_DUNGEON_WARP_ERROR_ABNORMALITY_EFFECT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WARP_ERROR_ABNORMALITY_EFFECT;
	if (str.Compare(TEXT("GUILD_DUNGEON_WATCHING_ERROR_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WATCHING_ERROR_MAP;
	if (str.Compare(TEXT("GUILD_DUNGEON_WATCHING_NO_ONE_HERE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WATCHING_NO_ONE_HERE;
	if (str.Compare(TEXT("GUILD_DUNGEON_RESET_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_RESET_TIME;
	if (str.Compare(TEXT("GUILD_DUNGEON_WARP_ERROR_ENOUGH_GUILD_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WARP_ERROR_ENOUGH_GUILD_LEVEL;
	if (str.Compare(TEXT("GUILD_DUNGEON_WARP_ERROR_THE_DAY_JOINED_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WARP_ERROR_THE_DAY_JOINED_GUILD;
	if (str.Compare(TEXT("GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER;
	if (str.Compare(TEXT("GUILD_DUNGEON_WARP_ERROR_ANOTHER_GUILD_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_DUNGEON_WARP_ERROR_ANOTHER_GUILD_LEVEL;
	if (str.Compare(TEXT("GUILD_ERROR_INTER_ALLY_PREPARING"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_INTER_ALLY_PREPARING;
	if (str.Compare(TEXT("GUILD_ERROR_INTER_ALLY_REQUEST_UNKNOWN_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_INTER_ALLY_REQUEST_UNKNOWN_GUILD_ID;
	if (str.Compare(TEXT("GUILD_ERROR_INTER_ALLY_REQUEST_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_INTER_ALLY_REQUEST_ERROR_DB;
	if (str.Compare(TEXT("GUILD_ERROR_INTER_ALLY_ACCEPT_UNKNOWN_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_INTER_ALLY_ACCEPT_UNKNOWN_GUILD_ID;
	if (str.Compare(TEXT("GUILD_ERROR_INTER_ALLY_ACCEPT_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ERROR_INTER_ALLY_ACCEPT_ERROR_DB;
	if (str.Compare(TEXT("GUILD_LEAVE_ERROR_CANNOT_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_LEAVE_ERROR_CANNOT_STATE;
	if (str.Compare(TEXT("AGIT_WARP_ERROR_IN_AGIT_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_WARP_ERROR_IN_AGIT_MAP;
	if (str.Compare(TEXT("AGIT_WARP_ERROR_EXPIRE"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_WARP_ERROR_EXPIRE;
	if (str.Compare(TEXT("AGIT_WARP_ERROR_NOT_AGIT_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_WARP_ERROR_NOT_AGIT_MAP;
	if (str.Compare(TEXT("AGIT_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_INVALID_USER;
	if (str.Compare(TEXT("AGIT_ERROR_NO_MORE_RENTAL_EXPANSION"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_ERROR_NO_MORE_RENTAL_EXPANSION;
	if (str.Compare(TEXT("AGIT_INVALID_RENTAL"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_INVALID_RENTAL;
	if (str.Compare(TEXT("AGIT_ERROR_NOT_RENTAING"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_ERROR_NOT_RENTAING;
	if (str.Compare(TEXT("ITEM_ERROR_REFINE_INVALID_TARGET"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_REFINE_INVALID_TARGET;
	if (str.Compare(TEXT("ITEM_ERROR_REFINE_INVALID_STAT_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_REFINE_INVALID_STAT_DATA;
	if (str.Compare(TEXT("ITEM_ERROR_REFINE_OPTION_STONE_NOT_MEET_THE_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_REFINE_OPTION_STONE_NOT_MEET_THE_CONDITION;
	if (str.Compare(TEXT("ITEM_ERROR_REFINE_LOCKED"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_ERROR_REFINE_LOCKED;
	if (str.Compare(TEXT("ARCAS_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("ARCAS_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("ARCAS_ERROR_MEDITATION"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_MEDITATION;
	if (str.Compare(TEXT("ARCAS_ERROR_NOT_MEDITATION"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_NOT_MEDITATION;
	if (str.Compare(TEXT("ARCAS_ERROR_NOT_REMOVE_ARCAS_BUFF"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_NOT_REMOVE_ARCAS_BUFF;
	if (str.Compare(TEXT("ARCAS_ERROR_ARCAS_BUFF_ON"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_ARCAS_BUFF_ON;
	if (str.Compare(TEXT("ARCAS_ERROR_NOT_USE_ARCAS_POINT_GAIN"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_NOT_USE_ARCAS_POINT_GAIN;
	if (str.Compare(TEXT("ARCAS_ERROR_INVALID_ARCAS_POINT_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_INVALID_ARCAS_POINT_DATA;
	if (str.Compare(TEXT("ARCAS_ERROR_NOT_ENOUGH_ARCAS_POINT"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_NOT_ENOUGH_ARCAS_POINT;
	if (str.Compare(TEXT("ARCAS_ERROR_INVALID_ARCAS_POINT_AND_NOT_EXIST_FAIRY_BUFF_ABNORMALITY"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_INVALID_ARCAS_POINT_AND_NOT_EXIST_FAIRY_BUFF_ABNORMALITY;
	if (str.Compare(TEXT("ARCAS_ERROR_INVALID_PASSIVITY_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_INVALID_PASSIVITY_ID;
	if (str.Compare(TEXT("ARCAS_ERROR_NOT_ITEM_USE_MAX_ARCAS_POINT"), ESearchCase::IgnoreCase) == 0)	return Result::ARCAS_ERROR_NOT_ITEM_USE_MAX_ARCAS_POINT;
	if (str.Compare(TEXT("USER_COMMON_ERROR_INVALID_STAT_POINT_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::USER_COMMON_ERROR_INVALID_STAT_POINT_DATA;
	if (str.Compare(TEXT("USER_COMMON_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::USER_COMMON_ERROR_INVALID_USER;
	if (str.Compare(TEXT("COSTUME_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ERROR_UNKNOWN;
	if (str.Compare(TEXT("COSTUME_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("COSTUME_ERROR_INVALID_COSTUME_ID"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ERROR_INVALID_COSTUME_ID;
	if (str.Compare(TEXT("COSTUME_ERROR_NOT_HAVE_COSTUME"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ERROR_NOT_HAVE_COSTUME;
	if (str.Compare(TEXT("COSTUME_ERROR_ALREADY_EQUIP_COSTUME"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ERROR_ALREADY_EQUIP_COSTUME;
	if (str.Compare(TEXT("COSTUME_ERROR_NOT_ENOUGH_COSTUME"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ERROR_NOT_ENOUGH_COSTUME;
	if (str.Compare(TEXT("COSTUME_ERROR_MAX_AMOUNT_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ERROR_MAX_AMOUNT_OVER;
	if (str.Compare(TEXT("COSTUME_ERROR_INVALID_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ERROR_INVALID_DATA;
	if (str.Compare(TEXT("COSTUME_ERROR_INVALID_PACKET_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ERROR_INVALID_PACKET_DATA;
	if (str.Compare(TEXT("COSTUME_SUMMON_ERROR_INVALID_COSTUME_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SUMMON_ERROR_INVALID_COSTUME_GRADE;
	if (str.Compare(TEXT("COSTUME_SUMMON_ERROR_MAX_CONFIRM_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SUMMON_ERROR_MAX_CONFIRM_COUNT;
	if (str.Compare(TEXT("COSTUME_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM;
	if (str.Compare(TEXT("COSTUME_CONFRIM_ERROR_INVALID_COSTUME_CONFIRM_ID"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CONFRIM_ERROR_INVALID_COSTUME_CONFIRM_ID;
	if (str.Compare(TEXT("COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT;
	if (str.Compare(TEXT("COSTUME_CONFRIM_ERROR_EXPIRED"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CONFRIM_ERROR_EXPIRED;
	if (str.Compare(TEXT("COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("COSTUME_ENCHANT_ERROR_MAX_LEVEL_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ENCHANT_ERROR_MAX_LEVEL_OVER;
	if (str.Compare(TEXT("COSTUME_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL;
	if (str.Compare(TEXT("COSTUME_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("COSTUME_COMPOSE_ERROR_NOT_SAME_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_COMPOSE_ERROR_NOT_SAME_GRADE;
	if (str.Compare(TEXT("COSTUME_COMPOSE_ERROR_MAX_COMPOSE_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_COMPOSE_ERROR_MAX_COMPOSE_COUNT;
	if (str.Compare(TEXT("COSTUME_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT;
	if (str.Compare(TEXT("COSTUME_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("COSTUME_COMPOSE_ERROR_LEGEND_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_COMPOSE_ERROR_LEGEND_GRADE;
	if (str.Compare(TEXT("COSTUME_SKILL_ERROR_NOT_COSTUME_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SKILL_ERROR_NOT_COSTUME_SKILL;
	if (str.Compare(TEXT("COSTUME_SKILL_ERROR_NOT_COSTUME_SKILl_ID"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SKILL_ERROR_NOT_COSTUME_SKILl_ID;
	if (str.Compare(TEXT("COSTUME_SKILL_ERROR_ALREADY_LEARNED_COSTUME_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SKILL_ERROR_ALREADY_LEARNED_COSTUME_SKILL;
	if (str.Compare(TEXT("COSTUME_SKILL_ERROR_NOT_LEARNED_COSTUME_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SKILL_ERROR_NOT_LEARNED_COSTUME_SKILL;
	if (str.Compare(TEXT("COSTUME_SKILL_ERROR_INVALID_SKILL_ID"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SKILL_ERROR_INVALID_SKILL_ID;
	if (str.Compare(TEXT("COSTUME_SKILL_ERROR_FAIL_TO_SET_SKILL_SLOT"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SKILL_ERROR_FAIL_TO_SET_SKILL_SLOT;
	if (str.Compare(TEXT("COSTUME_SKILL_ERROR_NO_COSTUME_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SKILL_ERROR_NO_COSTUME_SKILL;
	if (str.Compare(TEXT("COSTUME_SKILL_ERROR_NO_EQUIPED_COSTUME_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_SKILL_ERROR_NO_EQUIPED_COSTUME_SKILL;
	if (str.Compare(TEXT("COSTUME_CHANGE_ERROR_INVALID_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CHANGE_ERROR_INVALID_GRADE;
	if (str.Compare(TEXT("COSTUME_CHANGE_ERROR_EXCLUDE_ID"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CHANGE_ERROR_EXCLUDE_ID;
	if (str.Compare(TEXT("COSTUME_CHANGE_ERROR_SAME_COSTUME_ID"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CHANGE_ERROR_SAME_COSTUME_ID;
	if (str.Compare(TEXT("COSTUME_CHANGE_ERROR_NOT_IN_PROBABILITY_POOL"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CHANGE_ERROR_NOT_IN_PROBABILITY_POOL;
	if (str.Compare(TEXT("COSTUME_CHANGE_ERROR_SELECT_COSTUME"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CHANGE_ERROR_SELECT_COSTUME;
	if (str.Compare(TEXT("COSTUME_CHANGE_ERROR_NOT_SELECT_COSTUME"), ESearchCase::IgnoreCase) == 0)	return Result::COSTUME_CHANGE_ERROR_NOT_SELECT_COSTUME;
	if (str.Compare(TEXT("DUNGEON_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::DUNGEON_ERROR_UNKNOWN;
	if (str.Compare(TEXT("DUNGEON_ERROR_INVALID_DUNGEON_GROUP_ID"), ESearchCase::IgnoreCase) == 0)	return Result::DUNGEON_ERROR_INVALID_DUNGEON_GROUP_ID;
	if (str.Compare(TEXT("DUNGEON_ERROR_INVALID_DUNGEON_LIST_IDX"), ESearchCase::IgnoreCase) == 0)	return Result::DUNGEON_ERROR_INVALID_DUNGEON_LIST_IDX;
	if (str.Compare(TEXT("DUNGEON_ERROR_NOT_ENOUGH_COST"), ESearchCase::IgnoreCase) == 0)	return Result::DUNGEON_ERROR_NOT_ENOUGH_COST;
	if (str.Compare(TEXT("DUNGEON_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::DUNGEON_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("DUNGEON_ERROR_NOT_ENOUGH_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::DUNGEON_ERROR_NOT_ENOUGH_TIME;
	if (str.Compare(TEXT("DUNGEON_ERROR_TIME_OVERFLOW"), ESearchCase::IgnoreCase) == 0)	return Result::DUNGEON_ERROR_TIME_OVERFLOW;
	if (str.Compare(TEXT("DUNGEON_ERROR_NOT_OPEN_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::DUNGEON_ERROR_NOT_OPEN_TIME;
	if (str.Compare(TEXT("DUNGEON_ERROR_DISABLE_TIME_CHARGE"), ESearchCase::IgnoreCase) == 0)	return Result::DUNGEON_ERROR_DISABLE_TIME_CHARGE;
	if (str.Compare(TEXT("DEPOT_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_ERROR_UNKNOWN;
	if (str.Compare(TEXT("DEPOT_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("DEPOT_EXTEND_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_EXTEND_ERROR_UNKNOWN;
	if (str.Compare(TEXT("DEPOT_EXTEND_ERROR_MAX_SLOT"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_EXTEND_ERROR_MAX_SLOT;
	if (str.Compare(TEXT("DEPOT_EXTEND_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_EXTEND_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("DEPOT_ERROR_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_ERROR_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("DEPOT_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("DEPOT_ERROR_INVALID_USER_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_ERROR_INVALID_USER_STATE;
	if (str.Compare(TEXT("DEPOT_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_ERROR_INVALID_USER;
	if (str.Compare(TEXT("DEPOT_ERROR_DONT_MOVE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::DEPOT_ERROR_DONT_MOVE_ITEM;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_INVALID_COLLECTION_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_INVALID_COLLECTION_ID;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_INVALID_COLLECTION_CONDITION_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_INVALID_COLLECTION_CONDITION_ID;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_NOT_SUITABLE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_NOT_SUITABLE_ITEM;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_ITEM;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_INVALID_CATEGORY_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_INVALID_CATEGORY_ID;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_INVALID_UNLOCK_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_INVALID_UNLOCK_TYPE;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_DOES_NOT_MEET_THE_UNLOCK_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_DOES_NOT_MEET_THE_UNLOCK_CONDITION;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_INVALID_QUEST_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_INVALID_QUEST_TYPE;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_EXPIRE_DATE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_EXPIRE_DATE_TIME;
	if (str.Compare(TEXT("ITEM_COLLECTION_ERROR_INVALID_BOOKMARK"), ESearchCase::IgnoreCase) == 0)	return Result::ITEM_COLLECTION_ERROR_INVALID_BOOKMARK;
	if (str.Compare(TEXT("SPIRIT_SHOT_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("SPIRIT_SHOT_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE;
	if (str.Compare(TEXT("SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_CATEGORY_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_CATEGORY_ID;
	if (str.Compare(TEXT("SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_LEVEL_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_LEVEL_ID;
	if (str.Compare(TEXT("SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_NEXT_LEVEL_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_NEXT_LEVEL_ID;
	if (str.Compare(TEXT("SPIRIT_SHOT_ERROR_INVALID_MODIFY_ENCHANT_RATE_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ERROR_INVALID_MODIFY_ENCHANT_RATE_COUNT;
	if (str.Compare(TEXT("SPIRIT_SHOT_ERROR_INVALID_ITEM_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ERROR_INVALID_ITEM_COUNT;
	if (str.Compare(TEXT("SPIRIT_SHOT_ERROR_ALREADY_SAME_STATE_AS_BEFORE"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ERROR_ALREADY_SAME_STATE_AS_BEFORE;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ENCHANT_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ENCHANT_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT_ERROR_ALREADY_SAME_STATE_AS_BEFORE"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ENCHANT_ERROR_ALREADY_SAME_STATE_AS_BEFORE;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT_ERROR_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ENCHANT_ERROR_LEVEL;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT_ERROR_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ENCHANT_ERROR_ITEM;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT_ERROR_UNLOCK_CONTENTS"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ENCHANT_ERROR_UNLOCK_CONTENTS;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::SPIRIT_SHOT_ENCHANT_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("POTION_AUTO_USE_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::POTION_AUTO_USE_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("POTION_AUTO_USE_ERROR_ALREADY_SAME_STATE_AS_BEFORE"), ESearchCase::IgnoreCase) == 0)	return Result::POTION_AUTO_USE_ERROR_ALREADY_SAME_STATE_AS_BEFORE;
	if (str.Compare(TEXT("BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("BM_SHOP_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_UNKNOWN;
	if (str.Compare(TEXT("BM_SHOP_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("BM_SHOP_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_INVALID_USER;
	if (str.Compare(TEXT("BM_SHOP_ERROR_IDENDTIFIER"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_IDENDTIFIER;
	if (str.Compare(TEXT("BM_SHOP_ERROR_NOT_HIVE_SYNC_MARKET_PID"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_NOT_HIVE_SYNC_MARKET_PID;
	if (str.Compare(TEXT("BM_SHOP_ERROR_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_INVALID_USER_STATUS;
	if (str.Compare(TEXT("BM_SHOP_ERROR_NOT_ALLOWED_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_NOT_ALLOWED_LEVEL;
	if (str.Compare(TEXT("BM_SHOP_ERROR_NOT_ALLOWED_GUILD_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_NOT_ALLOWED_GUILD_LEVEL;
	if (str.Compare(TEXT("BM_SHOP_ERROR_ITEM_NOT_FOUND"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_ITEM_NOT_FOUND;
	if (str.Compare(TEXT("BM_SHOP_ERROR_INVALID_ITEM_ID"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_INVALID_ITEM_ID;
	if (str.Compare(TEXT("BM_SHOP_ERROR_PRODCUT_NOT_FOUND"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_PRODCUT_NOT_FOUND;
	if (str.Compare(TEXT("BM_SHOP_ERROR_INVALID_PRODCUT_ID"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_INVALID_PRODCUT_ID;
	if (str.Compare(TEXT("BM_SHOP_ERROR_INVALID_TAB_ID"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_INVALID_TAB_ID;
	if (str.Compare(TEXT("BM_SHOP_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("BM_SHOP_ERROR_OVERFLOW_MAX_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_OVERFLOW_MAX_MONEY;
	if (str.Compare(TEXT("BM_SHOP_ERROR_OVER_QUANTIY"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_OVER_QUANTIY;
	if (str.Compare(TEXT("BM_SHOP_ERROR_SALE_DATETIME"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_SALE_DATETIME;
	if (str.Compare(TEXT("BM_SHOP_ERROR_CASH_PURCHASE"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_CASH_PURCHASE;
	if (str.Compare(TEXT("BM_SHOP_ERROR_PURCHASE"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_PURCHASE;
	if (str.Compare(TEXT("BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID;
	if (str.Compare(TEXT("BM_SHOP_ERROR_DB_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_DB_OPERATION_FAILED;
	if (str.Compare(TEXT("BM_SHOP_ERROR_HIVE_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_HIVE_OPERATION_FAILED;
	if (str.Compare(TEXT("BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT;
	if (str.Compare(TEXT("BM_SHOP_ERROR_IMPOSSIBLE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_IMPOSSIBLE_ITEM;
	if (str.Compare(TEXT("BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY;
	if (str.Compare(TEXT("BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT;
	if (str.Compare(TEXT("BM_SHOP_DONT_TRY_SAVE_DB"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_DONT_TRY_SAVE_DB;
	if (str.Compare(TEXT("BM_SHOP_ERROR_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_MONEY;
	if (str.Compare(TEXT("BM_SHOP_ERROR_SAVE_BULK_SALE"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_SAVE_BULK_SALE;
	if (str.Compare(TEXT("BM_SHOP_ERROR_CASH_ITEM_ONE_AT_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_CASH_ITEM_ONE_AT_TIME;
	if (str.Compare(TEXT("BM_SHOP_ERROR_BULK_SALE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_BULK_SALE_ITEM;
	if (str.Compare(TEXT("BM_SHOP_ERROR_POPUP_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_POPUP_FAILED;
	if (str.Compare(TEXT("BM_SHOP_ERROR_STEPUP_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_STEPUP_FAILED;
	if (str.Compare(TEXT("BM_SHOP_DUPLICATION_COST_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_DUPLICATION_COST_ITEM;
	if (str.Compare(TEXT("BM_SHOP_COUPON_ITEM_INVALID"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_COUPON_ITEM_INVALID;
	if (str.Compare(TEXT("BM_SHOP_CAN_NOT_COUPON_ITEM_FOR_CASH"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_CAN_NOT_COUPON_ITEM_FOR_CASH;
	if (str.Compare(TEXT("BM_SHOP_CAN_NOT_COUPON_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_CAN_NOT_COUPON_ITEM;
	if (str.Compare(TEXT("BM_SHOP_ERROR_PURCHASE_OHTER_ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_PURCHASE_OHTER_ACCOUNT;
	if (str.Compare(TEXT("BM_SHOP_ERROR_PRODCUT_OOAP_NOT_FOUND"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_PRODCUT_OOAP_NOT_FOUND;
	if (str.Compare(TEXT("BM_SHOP_RESTORE_ERROR_ITEM_NOT_FOUND"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_RESTORE_ERROR_ITEM_NOT_FOUND;
	if (str.Compare(TEXT("BM_SHOP_RESTORE_ERROR_PRODCUT_NOT_FOUND"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_RESTORE_ERROR_PRODCUT_NOT_FOUND;
	if (str.Compare(TEXT("BM_SHOP_RESTORE_ERROR_NOT_HIVE_SYNC_MARKET_PID"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_RESTORE_ERROR_NOT_HIVE_SYNC_MARKET_PID;
	if (str.Compare(TEXT("BM_SHOP_ERROR_GATEWAY_FIND_USER"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_GATEWAY_FIND_USER;
	if (str.Compare(TEXT("BM_SHOP_ERROR_GATEWAY_INVALID_APPID"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_GATEWAY_INVALID_APPID;
	if (str.Compare(TEXT("BM_SHOP_ERROR_PG_UNLOCK"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_PG_UNLOCK;
	if (str.Compare(TEXT("BM_SHOP_ERROR_PG_UNCONSUMED_COOLTIME"), ESearchCase::IgnoreCase) == 0)	return Result::BM_SHOP_ERROR_PG_UNCONSUMED_COOLTIME;
	if (str.Compare(TEXT("SPACE_CRACK_ERROR_PLAY_LIMIT_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::SPACE_CRACK_ERROR_PLAY_LIMIT_COUNT;
	if (str.Compare(TEXT("SPACE_CRACK_ERROR_REWARD_COMPLETED"), ESearchCase::IgnoreCase) == 0)	return Result::SPACE_CRACK_ERROR_REWARD_COMPLETED;
	if (str.Compare(TEXT("SPACE_CRACK_ERROR_REWARD_DATA_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SPACE_CRACK_ERROR_REWARD_DATA_ID;
	if (str.Compare(TEXT("SPACE_CRACK_ERROR_REWARD_BOX_DATA_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SPACE_CRACK_ERROR_REWARD_BOX_DATA_ID;
	if (str.Compare(TEXT("SPACE_CRACK_ERROR_ENTRY_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::SPACE_CRACK_ERROR_ENTRY_TIME;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_PROMOTION_NOT_MEET_THE_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_PROMOTION_NOT_MEET_THE_CONDITION;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_PROMOTION_PROCESS_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_PROMOTION_PROCESS_FAILED;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_MEET_THE_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_MEET_THE_CONDITION;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_ACCEPT_QUEST_NOT_ENOUGH_REQUIRE_EXP"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_ACCEPT_QUEST_NOT_ENOUGH_REQUIRE_EXP;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_ACCEPT_BEFORE_RANK_NOT_CLEAR_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_ACCEPT_BEFORE_RANK_NOT_CLEAR_STATE;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_ACCEPT_ALREADY_ACCEPTED_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_ACCEPT_ALREADY_ACCEPTED_QUEST;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_REWARD_RANK_NOT_MEET_THE_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_REWARD_RANK_NOT_MEET_THE_CONDITION;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ERROR_ALREADY_RECEIVED_REWARD_RANK"), ESearchCase::IgnoreCase) == 0)	return Result::MULTI_LEVEL_RANK_ERROR_ALREADY_RECEIVED_REWARD_RANK;
	if (str.Compare(TEXT("MARKET_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_UNKNOWN;
	if (str.Compare(TEXT("MARKET_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("MARKET_ERROR_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("MARKET_ERROR_NOT_TRADABLE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_NOT_TRADABLE_ITEM;
	if (str.Compare(TEXT("MARKET_ERROR_ITEM_REGISTER_COUNT_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_ITEM_REGISTER_COUNT_OVER;
	if (str.Compare(TEXT("MARKET_ERROR_NOT_ENOUGH_ITEM_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_NOT_ENOUGH_ITEM_COUNT;
	if (str.Compare(TEXT("MARKET_ERROR_CANT_REGISTER_EQUIPED_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CANT_REGISTER_EQUIPED_ITEM;
	if (str.Compare(TEXT("MARKET_ERROR_NOT_ENOUGH_REGISTER_FEE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_NOT_ENOUGH_REGISTER_FEE;
	if (str.Compare(TEXT("MARKET_ERROR_INVALID_REGISTER_PRICE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_INVALID_REGISTER_PRICE;
	if (str.Compare(TEXT("MARKET_ERROR_INVALID_MARKET_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_INVALID_MARKET_TRANSACTION;
	if (str.Compare(TEXT("MARKET_ERROR_EXCESS_DIA"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_EXCESS_DIA;
	if (str.Compare(TEXT("MARKET_ERROR_NOT_FOUND_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_NOT_FOUND_TRANSACTION;
	if (str.Compare(TEXT("MARKET_ERROR_CANT_PURCHASE_MY_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CANT_PURCHASE_MY_TRANSACTION;
	if (str.Compare(TEXT("MARKET_ERROR_NOT_ENOUGH_DIA"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_NOT_ENOUGH_DIA;
	if (str.Compare(TEXT("MARKET_ERROR_CANT_CANCEL_OTHER_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CANT_CANCEL_OTHER_TRANSACTION;
	if (str.Compare(TEXT("MARKET_ERROR_INVALIDE_MARKET_CATEGORY"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_INVALIDE_MARKET_CATEGORY;
	if (str.Compare(TEXT("MARKET_ERROR_INVALIDE_ITEM_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_INVALIDE_ITEM_GRADE;
	if (str.Compare(TEXT("MARKET_ERROR_INVALIDE_PLAYING_USER"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_INVALIDE_PLAYING_USER;
	if (str.Compare(TEXT("MARKET_ERROR_INVALIDE_ITEM_ID"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_INVALIDE_ITEM_ID;
	if (str.Compare(TEXT("MARKET_ERROR_NOT_EXIST_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_NOT_EXIST_ITEM;
	if (str.Compare(TEXT("MARKET_ERROR_INVALID_ITEM_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_INVALID_ITEM_LEVEL;
	if (str.Compare(TEXT("MARKET_ERROR_IS_NOT_EQUIP_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_IS_NOT_EQUIP_ITEM;
	if (str.Compare(TEXT("MARKET_ERROR_NO_UPDATED_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_NO_UPDATED_TRANSACTION;
	if (str.Compare(TEXT("MARKET_ERROR_BOOKMARK_LIMIT_EXCEEDED"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_BOOKMARK_LIMIT_EXCEEDED;
	if (str.Compare(TEXT("MARKET_ERROR_DB_REQUEST_CALCULATE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_REQUEST_CALCULATE;
	if (str.Compare(TEXT("MARKET_ERROR_CALCULATE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CALCULATE;
	if (str.Compare(TEXT("MARKET_ERROR_CALCULATE_NOT_PERCHASED_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CALCULATE_NOT_PERCHASED_TRANSACTION;
	if (str.Compare(TEXT("MARKET_ERROR_CALCULATE_TRANSACTION_CACULATE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CALCULATE_TRANSACTION_CACULATE;
	if (str.Compare(TEXT("MARKET_ERROR_CALCULATE_CURRENCY_UPDATE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CALCULATE_CURRENCY_UPDATE;
	if (str.Compare(TEXT("MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_COMMIT"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_COMMIT;
	if (str.Compare(TEXT("MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_ROLLBACK"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_ROLLBACK;
	if (str.Compare(TEXT("MARKET_ERROR_PURCHASE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_PURCHASE;
	if (str.Compare(TEXT("MARKET_ERROR_DB_REQUEST_MOVE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_REQUEST_MOVE_ITEM;
	if (str.Compare(TEXT("MARKET_ERROR_DB_ITEM_MOVE_ON_INVENTORY"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_ITEM_MOVE_ON_INVENTORY;
	if (str.Compare(TEXT("MARKET_ERROR_DB_MOVE_ITEM_BY_CREATE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_MOVE_ITEM_BY_CREATE;
	if (str.Compare(TEXT("MARKET_ERROR_DB_MOVE_ITEM_BY_INCREASE"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_MOVE_ITEM_BY_INCREASE;
	if (str.Compare(TEXT("MARKET_ERROR_DB_CANCEL_DELETE_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_CANCEL_DELETE_TRANSACTION;
	if (str.Compare(TEXT("MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION;
	if (str.Compare(TEXT("MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_DELETED"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_DELETED;
	if (str.Compare(TEXT("MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_PURCHASED"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_PURCHASED;
	if (str.Compare(TEXT("MARKET_ERROR_DB_PURCHASE_DELETE_MARKET"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_PURCHASE_DELETE_MARKET;
	if (str.Compare(TEXT("MARKET_ERROR_DB_PURCHASE_DB_OPERATION"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_PURCHASE_DB_OPERATION;
	if (str.Compare(TEXT("MARKET_ERROR_CANCEL"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CANCEL;
	if (str.Compare(TEXT("MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_COMMIT"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_COMMIT;
	if (str.Compare(TEXT("MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_ROLLBACK"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_ROLLBACK;
	if (str.Compare(TEXT("MARKET_ERROR_CANNOT_USE_GM"), ESearchCase::IgnoreCase) == 0)	return Result::MARKET_ERROR_CANNOT_USE_GM;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_INVALID_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_INVALID_ID;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_EXPIRED"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_EXPIRED;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_ALREADY_HAVE"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_ALREADY_HAVE;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_NO_HAVE"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_NO_HAVE;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_ALREADY_CHECK_IN"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_ALREADY_CHECK_IN;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_NOT_ENOUGH_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_NOT_ENOUGH_ITEM;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_OVERFLOW_MAX_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_OVERFLOW_MAX_MONEY;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_NOT_OPEN"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_NOT_OPEN;
	if (str.Compare(TEXT("ATTENDANCE_ERROR_RECEIVE_DURATION"), ESearchCase::IgnoreCase) == 0)	return Result::ATTENDANCE_ERROR_RECEIVE_DURATION;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_UNKNOWN;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_INVALID_USER;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_INVALID_USER_STATUS;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_NOT_ALLOWED_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_NOT_ALLOWED_LEVEL;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_ITEM_NOT_FOUND"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_ITEM_NOT_FOUND;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_INVALID_ITEM_ID"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_INVALID_ITEM_ID;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_INVALID_NPC"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_INVALID_NPC;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_DIFFERENT_CHANNEL"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_DIFFERENT_CHANNEL;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_OVERFLOW_MAX_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_OVERFLOW_MAX_MONEY;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_OVERFLOW_CURRENCY_DELTA"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_OVERFLOW_CURRENCY_DELTA;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_OVER_QUANTIY"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_OVER_QUANTIY;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_GUILD_MEMBER_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_GUILD_MEMBER_GRADE;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_GUILD_ID;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_GUILD_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_GUILD_LEVEL;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_SALE_DATETIME"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_SALE_DATETIME;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_ERROR_PURCHASE"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_ERROR_PURCHASE;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_DUPLICATION_COST_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::NPC_EVENT_SHOP_DUPLICATION_COST_ITEM;
	if (str.Compare(TEXT("OFF_LINE_PLAY_AUTO_ATTACK"), ESearchCase::IgnoreCase) == 0)	return Result::OFF_LINE_PLAY_AUTO_ATTACK;
	if (str.Compare(TEXT("OFF_LINE_PLAY_USER_DEATH"), ESearchCase::IgnoreCase) == 0)	return Result::OFF_LINE_PLAY_USER_DEATH;
	if (str.Compare(TEXT("OFF_LINE_PLAY_EXPIRY_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::OFF_LINE_PLAY_EXPIRY_TIME;
	if (str.Compare(TEXT("OFF_LINE_PLAY_AREA"), ESearchCase::IgnoreCase) == 0)	return Result::OFF_LINE_PLAY_AREA;
	if (str.Compare(TEXT("OFF_LINE_LOAD_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::OFF_LINE_LOAD_ERROR;
	if (str.Compare(TEXT("CLIENT_CINEMATIC_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::CLIENT_CINEMATIC_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("CLIENT_CINEMATIC_ALREADY"), ESearchCase::IgnoreCase) == 0)	return Result::CLIENT_CINEMATIC_ALREADY;
	if (str.Compare(TEXT("CLIENT_CINEMATIC_END_ERROR_SEQUENCE_RES_ID"), ESearchCase::IgnoreCase) == 0)	return Result::CLIENT_CINEMATIC_END_ERROR_SEQUENCE_RES_ID;
	if (str.Compare(TEXT("CLIENT_CINEMATIC_ERROR_NOT_PLAYING"), ESearchCase::IgnoreCase) == 0)	return Result::CLIENT_CINEMATIC_ERROR_NOT_PLAYING;
	if (str.Compare(TEXT("CLIENT_CINEMATIC_ERROR_WARP"), ESearchCase::IgnoreCase) == 0)	return Result::CLIENT_CINEMATIC_ERROR_WARP;
	if (str.Compare(TEXT("CRAFT_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::CRAFT_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("CRAFT_ERROR_EXCEED_LIMIT_COUNT_ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return Result::CRAFT_ERROR_EXCEED_LIMIT_COUNT_ACCOUNT;
	if (str.Compare(TEXT("CRAFT_ERROR_LACK_OF_MATERIAL"), ESearchCase::IgnoreCase) == 0)	return Result::CRAFT_ERROR_LACK_OF_MATERIAL;
	if (str.Compare(TEXT("CRAFT_ERROR_EXPIRE_DATE_ITEM_CRAFT"), ESearchCase::IgnoreCase) == 0)	return Result::CRAFT_ERROR_EXPIRE_DATE_ITEM_CRAFT;
	if (str.Compare(TEXT("CRAFT_ERROR_EXCEED_LIMIT_COUNT_SERVER"), ESearchCase::IgnoreCase) == 0)	return Result::CRAFT_ERROR_EXCEED_LIMIT_COUNT_SERVER;
	if (str.Compare(TEXT("CRAFT_ERROR_NOT_MEET_THE_UNLOCK_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::CRAFT_ERROR_NOT_MEET_THE_UNLOCK_CONDITION;
	if (str.Compare(TEXT("CRAFT_ERROR_EXCEED_MAX_CRAFT_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::CRAFT_ERROR_EXCEED_MAX_CRAFT_COUNT;
	if (str.Compare(TEXT("CRAFT_ERROR_ONLY_SOME_ITEMS_WERE_CRAFTED"), ESearchCase::IgnoreCase) == 0)	return Result::CRAFT_ERROR_ONLY_SOME_ITEMS_WERE_CRAFTED;
	if (str.Compare(TEXT("GUILD_STORE_UNKNOWN_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_UNKNOWN_ERROR;
	if (str.Compare(TEXT("GUILD_STORE_ERROR_UNKNOWN_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_ERROR_UNKNOWN_GUILD_ID;
	if (str.Compare(TEXT("GUILD_STORE_ERROR_NOT_HAVE_GRADE_TO_DISTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_ERROR_NOT_HAVE_GRADE_TO_DISTRIBUTE;
	if (str.Compare(TEXT("GUILD_STORE_ERROR_NOT_GUILD_MEMBER_TO_DIRTIBUTE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_ERROR_NOT_GUILD_MEMBER_TO_DIRTIBUTE;
	if (str.Compare(TEXT("GUILD_STORE_ERROR_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_ERROR_GRADE;
	if (str.Compare(TEXT("GUILD_STORE_ERROR_NO_ITEM_TO_DISTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_ERROR_NO_ITEM_TO_DISTRIBUTE;
	if (str.Compare(TEXT("GUILD_STORE_ERROR_NO_GUILD_MEMBER_TO_RECEIVE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_ERROR_NO_GUILD_MEMBER_TO_RECEIVE_ITEM;
	if (str.Compare(TEXT("GUILD_STORE_ERROR_ITEM_AMOUNT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_ERROR_ITEM_AMOUNT;
	if (str.Compare(TEXT("GUILD_STORE_ERROR_EXPIRED_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_ERROR_EXPIRED_ITEM;
	if (str.Compare(TEXT("GUILD_STORE_ERROR_TRY_AGAIN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_STORE_ERROR_TRY_AGAIN;
	if (str.Compare(TEXT("GUILD_EXTEND_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_EXTEND_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("GUILD_EXTEND_ERROR_MAX_SLOT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_EXTEND_ERROR_MAX_SLOT;
	if (str.Compare(TEXT("GUILD_EXTEND_ERROR_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_EXTEND_ERROR_FULL;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ITEM_COLLECTION_ERROR_UNKNOWN;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_CONDITION;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_INVALID_CONDITION_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ITEM_COLLECTION_INVALID_CONDITION_ID;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_USER_REGISTER_COUNT_LIMIT_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ITEM_COLLECTION_USER_REGISTER_COUNT_LIMIT_OVER;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_INACTIVE_EVENT_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ITEM_COLLECTION_INACTIVE_EVENT_COLLECTION;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_ALREADY_CLEAR_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ITEM_COLLECTION_ALREADY_CLEAR_CONDITION;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_ALREADY_CLEAR_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ITEM_COLLECTION_ALREADY_CLEAR_COLLECTION;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_NOT_SUITABLE_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_ITEM_COLLECTION_NOT_SUITABLE_ITEM;
	if (str.Compare(TEXT("TREASURE_MONSTER_ERROR_SAFE_TYTERRITORY"), ESearchCase::IgnoreCase) == 0)	return Result::TREASURE_MONSTER_ERROR_SAFE_TYTERRITORY;
	if (str.Compare(TEXT("TREASURE_MONSTER_ERROR_ALREADY_CREATED"), ESearchCase::IgnoreCase) == 0)	return Result::TREASURE_MONSTER_ERROR_ALREADY_CREATED;
	if (str.Compare(TEXT("TREASURE_MONSTER_ERROR_INVALID_AREA"), ESearchCase::IgnoreCase) == 0)	return Result::TREASURE_MONSTER_ERROR_INVALID_AREA;
	if (str.Compare(TEXT("ACHIEVEMENT_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::ACHIEVEMENT_ERROR_UNKNOWN;
	if (str.Compare(TEXT("ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_ID;
	if (str.Compare(TEXT("ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_THEME_ID"), ESearchCase::IgnoreCase) == 0)	return Result::ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_THEME_ID;
	if (str.Compare(TEXT("ACHIEVEMENT_ERROR_NOT_MEET_THE_REWARD_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::ACHIEVEMENT_ERROR_NOT_MEET_THE_REWARD_CONDITION;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_INVALID_USER;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_CANT_LEVEL_UP_KNOWLEDGE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_CANT_LEVEL_UP_KNOWLEDGE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_INVALID_ITEM_AMOUNT"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_INVALID_ITEM_AMOUNT;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_GAIN_EXP_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_NOT_GAIN_EXP_ITEM;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_NO_KNOWLEDGE_LEVEL_UP_AVAILABLE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_NO_KNOWLEDGE_LEVEL_UP_AVAILABLE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_CANT_SLATE_SLOT_ACTIVATE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_CANT_SLATE_SLOT_ACTIVATE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_RESET_SLATE_SLOT_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_NOT_RESET_SLATE_SLOT_ITEM;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE_EXCEEDED_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE_EXCEEDED_CURRENCY;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_EFFECT_PARAM"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_EFFECT_PARAM;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_TEMPLATE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_TEMPLATE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_SLATE_SLOT_OPEN_COST"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_SLATE_SLOT_OPEN_COST;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_ENOUGHT_MONSTER_BOOK_POINT"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_NOT_ENOUGHT_MONSTER_BOOK_POINT;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_EFFECT_PARAM"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_EFFECT_PARAM;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_TEMPLATE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_TEMPLATE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_ALREADY_UNLOCK_SLATE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_UNLOCK_SLATE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_RESET_SLATE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_RESET_SLATE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_LEVEL;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_EXP"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_EXP;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_KNOWLEDGE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_KNOWLEDGE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_LOCKED_SLATE"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_LOCKED_SLATE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_ERROR_INVALID_KNOWLEDGE_MAX_EXP"), ESearchCase::IgnoreCase) == 0)	return Result::MONSTER_KNOWLEDGE_ERROR_INVALID_KNOWLEDGE_MAX_EXP;
	if (str.Compare(TEXT("SLATE_PROBABILITY_FAIL_TO_ACTIVATE_SPECIAL_NODE"), ESearchCase::IgnoreCase) == 0)	return Result::SLATE_PROBABILITY_FAIL_TO_ACTIVATE_SPECIAL_NODE;
	if (str.Compare(TEXT("SLATE_ERROR_INVALID_SLATE_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SLATE_ERROR_INVALID_SLATE_ID;
	if (str.Compare(TEXT("SLATE_ERROR_INVALID_SPECIAL_NODE_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SLATE_ERROR_INVALID_SPECIAL_NODE_ID;
	if (str.Compare(TEXT("SLATE_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::SLATE_ERROR_INVALID_USER;
	if (str.Compare(TEXT("SLATE_ERROR_LOCKED_SLATE"), ESearchCase::IgnoreCase) == 0)	return Result::SLATE_ERROR_LOCKED_SLATE;
	if (str.Compare(TEXT("SLATE_ERROR_CANT_ACTIVATE_SPECIAL_NODE_ANYMORE"), ESearchCase::IgnoreCase) == 0)	return Result::SLATE_ERROR_CANT_ACTIVATE_SPECIAL_NODE_ANYMORE;
	if (str.Compare(TEXT("SLATE_ERROR_DB_REQUEST_FAILURE"), ESearchCase::IgnoreCase) == 0)	return Result::SLATE_ERROR_DB_REQUEST_FAILURE;
	if (str.Compare(TEXT("CODEX_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_INVALID_USER;
	if (str.Compare(TEXT("CODEX_ERROR_INVALID_CATEGORY_ID"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_INVALID_CATEGORY_ID;
	if (str.Compare(TEXT("CODEX_ERROR_NOT_ENOUGTH_MONSTER_KNOWLEDGE"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_NOT_ENOUGTH_MONSTER_KNOWLEDGE;
	if (str.Compare(TEXT("CODEX_ERROR_INVALID_CATEGORY_BALANCE_ID"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_INVALID_CATEGORY_BALANCE_ID;
	if (str.Compare(TEXT("CODEX_ERROR_NOT_ENOUGH_MAP_GROUP_LEVEL_UP_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_NOT_ENOUGH_MAP_GROUP_LEVEL_UP_COUNT;
	if (str.Compare(TEXT("CODEX_CANT_EXCHANGE_ANYMORE_MAX_POINT"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_CANT_EXCHANGE_ANYMORE_MAX_POINT;
	if (str.Compare(TEXT("CODEX_ERROR_CATEGORY_LEVEL_UP_ZERO"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_CATEGORY_LEVEL_UP_ZERO;
	if (str.Compare(TEXT("CODEX_ERROR_CATEGORY_LEVEL_UP_SAME_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_CATEGORY_LEVEL_UP_SAME_LEVEL;
	if (str.Compare(TEXT("CODEX_ERROR_REQUEST_ITEM_AMOUNT_NOT_ENOUGH"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_REQUEST_ITEM_AMOUNT_NOT_ENOUGH;
	if (str.Compare(TEXT("CODEX_ERROR_NOT_GAIN_EXP_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_NOT_GAIN_EXP_ITEM;
	if (str.Compare(TEXT("CODEX_ERROR_INVALID_MAP_GROUP_ID"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_INVALID_MAP_GROUP_ID;
	if (str.Compare(TEXT("CODEX_ERROR_NOT_INGREDIENT_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_NOT_INGREDIENT_ITEM;
	if (str.Compare(TEXT("CODEX_ERROR_MAP_GROUP_MAX_EXP"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_MAP_GROUP_MAX_EXP;
	if (str.Compare(TEXT("CODEX_ERROR_MAX_MONSTER_BOOK_POINT"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_MAX_MONSTER_BOOK_POINT;
	if (str.Compare(TEXT("CODEX_ERROR_DB_REQUEST_FAILURE"), ESearchCase::IgnoreCase) == 0)	return Result::CODEX_ERROR_DB_REQUEST_FAILURE;
	if (str.Compare(TEXT("DEATH_PENALTY_ERROR_USER_LEVEL_DATA_TABLE"), ESearchCase::IgnoreCase) == 0)	return Result::DEATH_PENALTY_ERROR_USER_LEVEL_DATA_TABLE;
	if (str.Compare(TEXT("DEATH_ENALTY_ERROR_RESURREC_DATA_TABLE"), ESearchCase::IgnoreCase) == 0)	return Result::DEATH_ENALTY_ERROR_RESURREC_DATA_TABLE;
	if (str.Compare(TEXT("RECONNECT_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::RECONNECT_ERROR_UNKNOWN;
	if (str.Compare(TEXT("RECONNECT_ERROR_INVALID_ACCOUNT_ID"), ESearchCase::IgnoreCase) == 0)	return Result::RECONNECT_ERROR_INVALID_ACCOUNT_ID;
	if (str.Compare(TEXT("RECONNECT_ERROR_INVALID_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::RECONNECT_ERROR_INVALID_STATE;
	if (str.Compare(TEXT("RECONNECT_ERROR_INCORRECT_TOKEN"), ESearchCase::IgnoreCase) == 0)	return Result::RECONNECT_ERROR_INCORRECT_TOKEN;
	if (str.Compare(TEXT("RECONNECT_ERROR_NOT_DISCONNECTED_YET"), ESearchCase::IgnoreCase) == 0)	return Result::RECONNECT_ERROR_NOT_DISCONNECTED_YET;
	if (str.Compare(TEXT("CREATE_USER_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_UNKNOWN;
	if (str.Compare(TEXT("CREATE_USER_ERROR_INTERNAL_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_INTERNAL_ERROR;
	if (str.Compare(TEXT("CREATE_USER_ERROR_DB_OPERATION_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_DB_OPERATION_ERROR;
	if (str.Compare(TEXT("CREATE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER;
	if (str.Compare(TEXT("CREATE_USER_ERROR_ALREADY_EXIST_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_ALREADY_EXIST_NAME;
	if (str.Compare(TEXT("CREATE_USER_ERROR_INVALID_LENGTH_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_INVALID_LENGTH_NAME;
	if (str.Compare(TEXT("CREATE_USER_ERROR_NOT_ALLOWED_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_NOT_ALLOWED_NAME;
	if (str.Compare(TEXT("CREATE_USER_ERROR_INVALID_GENDER_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_INVALID_GENDER_TYPE;
	if (str.Compare(TEXT("CREATE_USER_ERROR_INVALID_WEAPON_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_INVALID_WEAPON_TYPE;
	if (str.Compare(TEXT("CREATE_USER_ERROR_PUSH_INITIAL_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_PUSH_INITIAL_ITEM;
	if (str.Compare(TEXT("CREATE_USER_ERROR_SET_INITIAL_SKILL"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_SET_INITIAL_SKILL;
	if (str.Compare(TEXT("CREATE_USER_ERROR_SET_INITIAL_FAIRY"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_SET_INITIAL_FAIRY;
	if (str.Compare(TEXT("CREATE_USER_ERROR_NO_MORE_USER_SOCKET"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_NO_MORE_USER_SOCKET;
	if (str.Compare(TEXT("CREATE_USER_ERROR_PUSH_INITIAL_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_PUSH_INITIAL_CURRENCY;
	if (str.Compare(TEXT("CREATE_USER_ERROR_SET_INITIAL_COSTUME"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_SET_INITIAL_COSTUME;
	if (str.Compare(TEXT("CREATE_USER_ERROR_ADD_INITIAL_MAIL"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_ADD_INITIAL_MAIL;
	if (str.Compare(TEXT("CREATE_USER_ERROR_SET_NEW_USER_ATTENDANCE"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_SET_NEW_USER_ATTENDANCE;
	if (str.Compare(TEXT("CREATE_USER_ERROR_SET_NEW_USER_COMMUNITY_EVENT_MISSION"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_SET_NEW_USER_COMMUNITY_EVENT_MISSION;
	if (str.Compare(TEXT("CREATE_USER_ERROR_PENALTY_FREE"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_PENALTY_FREE;
	if (str.Compare(TEXT("CREATE_USER_ERROR_ARCAS"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_ARCAS;
	if (str.Compare(TEXT("CREATE_USER_ERROR_QUEST_MAIN"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_QUEST_MAIN;
	if (str.Compare(TEXT("CREATE_USER_ERROR_COMBAT_OPTION"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_COMBAT_OPTION;
	if (str.Compare(TEXT("CREATE_USER_ERROR_OFF_LINE"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_OFF_LINE;
	if (str.Compare(TEXT("CREATE_USER_ERROR_SET_ISSUED_USER_ID"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_SET_ISSUED_USER_ID;
	if (str.Compare(TEXT("CREATE_USER_ERROR_INSERT_USER"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_INSERT_USER;
	if (str.Compare(TEXT("CREATE_USER_ERROR_INSERT_USER_ROW_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_INSERT_USER_ROW_COUNT;
	if (str.Compare(TEXT("CREATE_USER_ERROR_CALL_END_TRANSACTION_COMMIT"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_CALL_END_TRANSACTION_COMMIT;
	if (str.Compare(TEXT("CREATE_USER_ERROR_CALL_END_TRANSACTION_ROLLBACK"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_CALL_END_TRANSACTION_ROLLBACK;
	if (str.Compare(TEXT("CREATE_USER_ERROR_SET_NEW_USER_SOCIAL_ACTON"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_SET_NEW_USER_SOCIAL_ACTON;
	if (str.Compare(TEXT("CREATE_USER_ERROR_CREATION_LIMIT"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_CREATION_LIMIT;
	if (str.Compare(TEXT("DELETE_USER_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_USER_ERROR_UNKNOWN;
	if (str.Compare(TEXT("DELETE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER;
	if (str.Compare(TEXT("DELETE_USER_ERROR_INVALID_USER_DB_ID"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_USER_ERROR_INVALID_USER_DB_ID;
	if (str.Compare(TEXT("DELETE_USER_ERROR_DB_OPERATION_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_USER_ERROR_DB_OPERATION_ERROR;
	if (str.Compare(TEXT("DELETE_USER_ERROR_GUILD_MEMBER_CANNOT"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_USER_ERROR_GUILD_MEMBER_CANNOT;
	if (str.Compare(TEXT("DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL;
	if (str.Compare(TEXT("DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION;
	if (str.Compare(TEXT("DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION;
	if (str.Compare(TEXT("SELECT_USER_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_USER_ERROR_UNKNOWN;
	if (str.Compare(TEXT("SELECT_USER_ERROR_WORLD_SERVER_DOWN"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_USER_ERROR_WORLD_SERVER_DOWN;
	if (str.Compare(TEXT("SELECT_USER_ERROR_MUST_CHANGE_USER_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_USER_ERROR_MUST_CHANGE_USER_NAME;
	if (str.Compare(TEXT("SELECT_USER_ERROR_LEAVING_WORLD"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_USER_ERROR_LEAVING_WORLD;
	if (str.Compare(TEXT("SELECT_USER_ERROR_INVALID_LOGIN_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_USER_ERROR_INVALID_LOGIN_STATE;
	if (str.Compare(TEXT("SELECT_USER_ERROR_BLOCK_USER"), ESearchCase::IgnoreCase) == 0)	return Result::SELECT_USER_ERROR_BLOCK_USER;
	if (str.Compare(TEXT("LOGOUT_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::LOGOUT_ERROR_UNKNOWN;
	if (str.Compare(TEXT("LOGOUT_ERROR_COMBAT_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::LOGOUT_ERROR_COMBAT_STATE;
	if (str.Compare(TEXT("BUDDY_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_UNKNOWN;
	if (str.Compare(TEXT("BUDDY_ERROR_INVALID_NAME_LENGTH"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_INVALID_NAME_LENGTH;
	if (str.Compare(TEXT("BUDDY_ERROR_REQUEST_MY_SELF"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_REQUEST_MY_SELF;
	if (str.Compare(TEXT("BUDDY_ERROR_NOT_FOUND_USER"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_NOT_FOUND_USER;
	if (str.Compare(TEXT("BUDDY_ERROR_INVALID_USER_ID"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_INVALID_USER_ID;
	if (str.Compare(TEXT("BUDDY_ERROR_DB_OPERATION_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_DB_OPERATION_ERROR;
	if (str.Compare(TEXT("BUDDY_ERROR_INVALID_INVITATION"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_INVALID_INVITATION;
	if (str.Compare(TEXT("BUDDY_ERROR_ALREADY_BUDDY"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_ALREADY_BUDDY;
	if (str.Compare(TEXT("BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY;
	if (str.Compare(TEXT("BUDDY_ERROR_ALREADY_INVITATION_SENT"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_ALREADY_INVITATION_SENT;
	if (str.Compare(TEXT("BUDDY_ERROR_ALREADY_INVITATION_RECV"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_ALREADY_INVITATION_RECV;
	if (str.Compare(TEXT("BUDDY_ERROR_SEND_INVITATION_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_SEND_INVITATION_FULL;
	if (str.Compare(TEXT("BUDDY_ERROR_REACHED_MAX_BUDDY"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_REACHED_MAX_BUDDY;
	if (str.Compare(TEXT("BUDDY_ERROR_NOT_BUDDY"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_NOT_BUDDY;
	if (str.Compare(TEXT("BUDDY_ERROR_DELETE_SEND_INVITATION"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_DELETE_SEND_INVITATION;
	if (str.Compare(TEXT("BUDDY_ERROR_DELETE_RECV_INVITATION"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_DELETE_RECV_INVITATION;
	if (str.Compare(TEXT("BUDDY_ERROR_TARGET_RECV_INVITATION_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_TARGET_RECV_INVITATION_FULL;
	if (str.Compare(TEXT("BUDDY_ERROR_INVALID_BUDDY_TAB_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_INVALID_BUDDY_TAB_TYPE;
	if (str.Compare(TEXT("BUDDY_ERROR_INSERT_BUDDY_TO_ME"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_INSERT_BUDDY_TO_ME;
	if (str.Compare(TEXT("BUDDY_ERROR_INSERT_ME_TO_BUDDY"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_INSERT_ME_TO_BUDDY;
	if (str.Compare(TEXT("BUDDY_ERROR_DELETE_REQUEST_INVITATION"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_DELETE_REQUEST_INVITATION;
	if (str.Compare(TEXT("BUDDY_ERROR_INSERT_REQUEST_INVITATION"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_INSERT_REQUEST_INVITATION;
	if (str.Compare(TEXT("BUDDY_ERROR_INVALID_INVITATION_SAME_ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_INVALID_INVITATION_SAME_ACCOUNT;
	if (str.Compare(TEXT("BUDDY_ERROR_CACHE_MISSING"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_CACHE_MISSING;
	if (str.Compare(TEXT("BUDDY_ERROR_WORLD_SERVER_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::BUDDY_ERROR_WORLD_SERVER_ITEM;
	if (str.Compare(TEXT("CHAT_BLACKLIST_ADD_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_BLACKLIST_ADD_ERROR_DB;
	if (str.Compare(TEXT("CHAT_BLACKLIST_ADD_ERROR_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_BLACKLIST_ADD_ERROR_OVER;
	if (str.Compare(TEXT("CHAT_BLACKLIST_ADD_ERROR_UNKNOWN_USER"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_BLACKLIST_ADD_ERROR_UNKNOWN_USER;
	if (str.Compare(TEXT("CHAT_BLACKLIST_ADD_ERROR_DUPLICATION"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_BLACKLIST_ADD_ERROR_DUPLICATION;
	if (str.Compare(TEXT("CHAT_BLACKLIST_REMOVE_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_BLACKLIST_REMOVE_ERROR_DB;
	if (str.Compare(TEXT("REPORT_BAD_CHAT_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::REPORT_BAD_CHAT_ERROR_DB;
	if (str.Compare(TEXT("REPORT_BAD_CHAT_ERROR_OVER"), ESearchCase::IgnoreCase) == 0)	return Result::REPORT_BAD_CHAT_ERROR_OVER;
	if (str.Compare(TEXT("REPORT_BAD_CHAT_ERROR_UNKNOWN_USER"), ESearchCase::IgnoreCase) == 0)	return Result::REPORT_BAD_CHAT_ERROR_UNKNOWN_USER;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_EXIST_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_EXIST_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_GUILD_QUEST_DB_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_GUILD_QUEST_DB_ID;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_QUEST_DB_OPERATION"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_QUEST_DB_OPERATION;
	if (str.Compare(TEXT("REGISTER_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::REGISTER_ERROR_UNKNOWN;
	if (str.Compare(TEXT("REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID;
	if (str.Compare(TEXT("HIVE_RECEIPT_VERIFY_ERROR_SETTING"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_RECEIPT_VERIFY_ERROR_SETTING;
	if (str.Compare(TEXT("HIVE_RECEIPT_VERIFY_ERROR_HOST"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_RECEIPT_VERIFY_ERROR_HOST;
	if (str.Compare(TEXT("HIVE_RECEIPT_VERIFY_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_RECEIPT_VERIFY_ERROR;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_PARSE"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_PARSE;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_DIFF_PAYLOAD"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_DIFF_PAYLOAD;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_PAYLOAD_NONE"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_RECEIPT_PAYLOAD_NONE;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_SETTING"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_ITEM_RESULT_SETTING;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_HOST"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_ITEM_RESULT_HOST;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_ITEM_RESULT;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_PARSE"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_ITEM_RESULT_PARSE;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN;
	if (str.Compare(TEXT("HIVE_ERROR_TIME_ZONE_RESULT_FAIL"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_TIME_ZONE_RESULT_FAIL;
	if (str.Compare(TEXT("HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN;
	if (str.Compare(TEXT("HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO;
	if (str.Compare(TEXT("INVENTORY_EXTEND_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::INVENTORY_EXTEND_ERROR_UNKNOWN;
	if (str.Compare(TEXT("INVENTORY_EXTEND_ERROR_MAX_SLOT"), ESearchCase::IgnoreCase) == 0)	return Result::INVENTORY_EXTEND_ERROR_MAX_SLOT;
	if (str.Compare(TEXT("INVENTORY_EXTEND_ERROR_NOT_ENOUGH_MONEY"), ESearchCase::IgnoreCase) == 0)	return Result::INVENTORY_EXTEND_ERROR_NOT_ENOUGH_MONEY;
	if (str.Compare(TEXT("CHECK_USER_EXIST_ERROR_USER_NAME_EMPTY"), ESearchCase::IgnoreCase) == 0)	return Result::CHECK_USER_EXIST_ERROR_USER_NAME_EMPTY;
	if (str.Compare(TEXT("CHECK_USER_EXIST_ERROR_USER_NAME_LENGTH"), ESearchCase::IgnoreCase) == 0)	return Result::CHECK_USER_EXIST_ERROR_USER_NAME_LENGTH;
	if (str.Compare(TEXT("CHECK_USER_EXIST_ERROR_USER_NOT_EXIST"), ESearchCase::IgnoreCase) == 0)	return Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST;
	if (str.Compare(TEXT("CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD"), ESearchCase::IgnoreCase) == 0)	return Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD;
	if (str.Compare(TEXT("TITLE_ERROR_INVALID_EQUIP_SLOT"), ESearchCase::IgnoreCase) == 0)	return Result::TITLE_ERROR_INVALID_EQUIP_SLOT;
	if (str.Compare(TEXT("TITLE_ERROR_INVALID_TITLE_ID"), ESearchCase::IgnoreCase) == 0)	return Result::TITLE_ERROR_INVALID_TITLE_ID;
	if (str.Compare(TEXT("TITLE_ERROR_NOT_ACTIVED_TITLE"), ESearchCase::IgnoreCase) == 0)	return Result::TITLE_ERROR_NOT_ACTIVED_TITLE;
	if (str.Compare(TEXT("TITLE_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::TITLE_ERROR_UNKNOWN;
	if (str.Compare(TEXT("REGISTER_USER_UNLOCKINFO_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::REGISTER_USER_UNLOCKINFO_ERROR_UNKNOWN;
	if (str.Compare(TEXT("STATISTICS_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::STATISTICS_ERROR_UNKNOWN;
	if (str.Compare(TEXT("STATISTICS_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::STATISTICS_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("CAMP_ERROR_INVALID"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_INVALID;
	if (str.Compare(TEXT("CAMP_ERROR_GUILD_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_GUILD_GRADE;
	if (str.Compare(TEXT("CAMP_ERROR_GUILD_BUILDING"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_GUILD_BUILDING;
	if (str.Compare(TEXT("CAMP_ERROR_SAFE_TYPE_TERRITORY"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_SAFE_TYPE_TERRITORY;
	if (str.Compare(TEXT("CAMP_ERROR_INVALID_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_INVALID_MAP;
	if (str.Compare(TEXT("CAMP_ERROR_INVALID_AREA"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_INVALID_AREA;
	if (str.Compare(TEXT("CAMP_ERROR_INVALID_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_INVALID_ITEM;
	if (str.Compare(TEXT("CAMP_ERROR_INVALID_NPC"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_INVALID_NPC;
	if (str.Compare(TEXT("CAMP_ERROR_SPAWN_CAMP"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_SPAWN_CAMP;
	if (str.Compare(TEXT("CAMP_ERROR_UNABLE_INTERACTION"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_UNABLE_INTERACTION;
	if (str.Compare(TEXT("CAMP_ERROR_NEAR_CAMPS_EXIST"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_NEAR_CAMPS_EXIST;
	if (str.Compare(TEXT("CAMP_ERROR_CAMP_DESTROYING"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_CAMP_DESTROYING;
	if (str.Compare(TEXT("CAMP_ERROR_INSTALLATION_LIMIT_DISTANCE"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_INSTALLATION_LIMIT_DISTANCE;
	if (str.Compare(TEXT("CAMP_ERROR_ALREADY_INSTALLED"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_ALREADY_INSTALLED;
	if (str.Compare(TEXT("CAMP_ERROR_INVADE_WORLD"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_INVADE_WORLD;
	if (str.Compare(TEXT("CAMP_ERROR_UNAUTHORIZED"), ESearchCase::IgnoreCase) == 0)	return Result::CAMP_ERROR_UNAUTHORIZED;
	if (str.Compare(TEXT("SANCTUM_ERROR_GUILD_GRADE"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_GUILD_GRADE;
	if (str.Compare(TEXT("SANCTUM_ERROR_GUILD_BUILDING"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_GUILD_BUILDING;
	if (str.Compare(TEXT("SANCTUM_ERROR_INVALID_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_INVALID_MAP;
	if (str.Compare(TEXT("SANCTUM_ERROR_INVALID_AREA"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_INVALID_AREA;
	if (str.Compare(TEXT("SANCTUM_ERROR_INVALID_NPC"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_INVALID_NPC;
	if (str.Compare(TEXT("SANCTUM_ERROR_SPAWN_SANCTUM"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_SPAWN_SANCTUM;
	if (str.Compare(TEXT("SANCTUM_ERROR_UNABLE_INTERACTION"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_UNABLE_INTERACTION;
	if (str.Compare(TEXT("SANCTUM_ERROR_NEAR_SANCTUMS_EXIST"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_NEAR_SANCTUMS_EXIST;
	if (str.Compare(TEXT("SANCTUM_ERROR_SANCTUM_DESTROYING"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_SANCTUM_DESTROYING;
	if (str.Compare(TEXT("SANCTUM_ERROR_INVADE_WORLD"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_INVADE_WORLD;
	if (str.Compare(TEXT("SANCTUM_ERROR_UNAUTHORIZED"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_UNAUTHORIZED;
	if (str.Compare(TEXT("SANCTUM_ERROR_ALREADY_INSTALLED"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_ALREADY_INSTALLED;
	if (str.Compare(TEXT("SANCTUM_ERROR_BATTLE_BEGIN"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_BATTLE_BEGIN;
	if (str.Compare(TEXT("SANCTUM_ERROR_EXIST_NEXUS"), ESearchCase::IgnoreCase) == 0)	return Result::SANCTUM_ERROR_EXIST_NEXUS;
	if (str.Compare(TEXT("PROFILE_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_UNKNOWN;
	if (str.Compare(TEXT("PROFILE_ERROR_INVALID_LENGTH_TEXT"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_INVALID_LENGTH_TEXT;
	if (str.Compare(TEXT("PROFILE_ERROR_INTRO_DB_FAIL"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_INTRO_DB_FAIL;
	if (str.Compare(TEXT("PROFILE_ERROR_NOT_ALLOWED_TEXT"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_NOT_ALLOWED_TEXT;
	if (str.Compare(TEXT("PROFILE_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_INVALID_USER;
	if (str.Compare(TEXT("PROFILE_ERROR_DATA_CACHE_FAIL"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_DATA_CACHE_FAIL;
	if (str.Compare(TEXT("PROFILE_ERROR_COMMENT_DB_FAIL"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_COMMENT_DB_FAIL;
	if (str.Compare(TEXT("PROFILE_ERROR_COMMENT_DELETED"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_COMMENT_DELETED;
	if (str.Compare(TEXT("PROFILE_ERROR_COMMENT_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_COMMENT_FULL;
	if (str.Compare(TEXT("PROFILE_ERROR_COMMENT_INPUT_SELF"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_COMMENT_INPUT_SELF;
	if (str.Compare(TEXT("PROFILE_ERROR_COMMENT_DELETE_INVALID"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_COMMENT_DELETE_INVALID;
	if (str.Compare(TEXT("PROFILE_ERROR_INVALID_KEYWORD"), ESearchCase::IgnoreCase) == 0)	return Result::PROFILE_ERROR_INVALID_KEYWORD;
	if (str.Compare(TEXT("INVADE_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_UNKNOWN;
	if (str.Compare(TEXT("INVADE_ERROR_INVALID_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_INVALID_TIME;
	if (str.Compare(TEXT("INVADE_ERROR_PORTAL_MAX_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_PORTAL_MAX_COUNT;
	if (str.Compare(TEXT("INVADE_ERROR_INVALID_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_INVALID_LEVEL;
	if (str.Compare(TEXT("INVADE_ERROR_ENTER_MAX_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_ENTER_MAX_COUNT;
	if (str.Compare(TEXT("INVADE_ERROR_NOT_ENOUGH_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_NOT_ENOUGH_TIME;
	if (str.Compare(TEXT("INVADE_ERROR_INVALID_LOAD_USER_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_INVALID_LOAD_USER_DATA;
	if (str.Compare(TEXT("INVADE_ERROR_INVALID_SAVE_USER_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_INVALID_SAVE_USER_DATA;
	if (str.Compare(TEXT("INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA;
	if (str.Compare(TEXT("INVADE_ERROR_UNLOCK"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_UNLOCK;
	if (str.Compare(TEXT("INVADE_ERROR_INVALID_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_INVALID_MAP;
	if (str.Compare(TEXT("USER_CONTEXT_UI_ERROR_INVALID_PROFILE_USER"), ESearchCase::IgnoreCase) == 0)	return Result::USER_CONTEXT_UI_ERROR_INVALID_PROFILE_USER;
	if (str.Compare(TEXT("USER_CONTEXT_UI_ERROR_IS_GM_USER"), ESearchCase::IgnoreCase) == 0)	return Result::USER_CONTEXT_UI_ERROR_IS_GM_USER;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_UNCONSUMED_SETTING"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_UNCONSUMED_SETTING;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_UNCONSUMED_HOST_SETTING"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_UNCONSUMED_HOST_SETTING;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_UNCONSUMED_NO_APP_ID"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_UNCONSUMED_NO_APP_ID;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_UNCONSUMED_UNABLE_TO_CONNECT"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_UNCONSUMED_UNABLE_TO_CONNECT;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_UNCONSUMED_PARSE"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_UNCONSUMED_PARSE;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_UNCONSUMED_NO_RESULT"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_UNCONSUMED_NO_RESULT;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_UNCONSUMED_RESULT_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_UNCONSUMED_RESULT_ERROR;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_UNCONSUMED_NO_REQUIRED_PARAMETER"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_ERROR_IAP_UNCONSUMED_NO_REQUIRED_PARAMETER;
	if (str.Compare(TEXT("TUTORIAL_ERROR_INTERNAL_DB_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::TUTORIAL_ERROR_INTERNAL_DB_ERROR;
	if (str.Compare(TEXT("TUTORIAL_ERROR_INVALID_TUTORIAL_ID"), ESearchCase::IgnoreCase) == 0)	return Result::TUTORIAL_ERROR_INVALID_TUTORIAL_ID;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_UNKNOWN;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_ENTER_MAX_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_ENTER_MAX_COUNT;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_INVALID_LOAD_USER_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_INVALID_LOAD_USER_DATA;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_INVALID_SAVE_USER_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_INVALID_SAVE_USER_DATA;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_NOT_FOUND_PARTY_DUNGEON_MAP_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_NOT_FOUND_PARTY_DUNGEON_MAP_DATA;
	if (str.Compare(TEXT("PARTY_DUNGEON_ERROR_UNLOCK"), ESearchCase::IgnoreCase) == 0)	return Result::PARTY_DUNGEON_ERROR_UNLOCK;
	if (str.Compare(TEXT("DUEL_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_UNKNOWN;
	if (str.Compare(TEXT("DUEL_ERROR_ALEREADY_INVITE_OTHER_USER"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_ALEREADY_INVITE_OTHER_USER;
	if (str.Compare(TEXT("DUEL_ERROR_ALEREADY_PLAYING"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_ALEREADY_PLAYING;
	if (str.Compare(TEXT("DUEL_ERROR_CANNOT_DUEL_TERRITORY"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_CANNOT_DUEL_TERRITORY;
	if (str.Compare(TEXT("DUEL_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_INVALID_USER;
	if (str.Compare(TEXT("DUEL_ERROR_CANNOT_DUEL_TERRITORY_OTHER_USER"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_CANNOT_DUEL_TERRITORY_OTHER_USER;
	if (str.Compare(TEXT("DUEL_ERROR_ALEREADY_INVITED_BY_OTHER_USER"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_ALEREADY_INVITED_BY_OTHER_USER;
	if (str.Compare(TEXT("DUEL_ERROR_NOT_INVITE_USER"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_NOT_INVITE_USER;
	if (str.Compare(TEXT("DUEL_ERROR_NOT_INVITED"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_NOT_INVITED;
	if (str.Compare(TEXT("DUEL_ERROR_NOT_SAME_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_NOT_SAME_MAP;
	if (str.Compare(TEXT("DUEL_ERROR_ARENA_ALREADY_PLAYING"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_ARENA_ALREADY_PLAYING;
	if (str.Compare(TEXT("DUEL_ERROR_EXPIRED_INVITE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::DUEL_ERROR_EXPIRED_INVITE_TIME;
	if (str.Compare(TEXT("CHARACTER_SLOT_UNLOCK_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::CHARACTER_SLOT_UNLOCK_ERROR_UNKNOWN;
	if (str.Compare(TEXT("CHARACTER_SLOT_UNLOCK_ERROR_EXCEEDED"), ESearchCase::IgnoreCase) == 0)	return Result::CHARACTER_SLOT_UNLOCK_ERROR_EXCEEDED;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_UNKNOWN;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_BATTLE_PASS_NOT_EXISTS"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_BATTLE_PASS_NOT_EXISTS;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_NOT_MEET_THE_REWARD_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_NOT_MEET_THE_REWARD_CONDITION;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_ALREADY_REWARDED"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_ALREADY_REWARDED;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_TARGET_LEVEL_LESS_THAN_CURRENT_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_TARGET_LEVEL_LESS_THAN_CURRENT_LEVEL;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_EXCEED_MAX_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_EXCEED_MAX_LEVEL;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_BUY_LEVEL_CURRENCY_NOT_ENOUGH"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_BUY_LEVEL_CURRENCY_NOT_ENOUGH;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_ADDITONAL_REWARD_NOT_ACTIVATED"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_ADDITONAL_REWARD_NOT_ACTIVATED;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_ADDITONAL_REWARD_IS_ALREADY_ACTIVATED"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_ADDITONAL_REWARD_IS_ALREADY_ACTIVATED;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_ADDITONAL_ITEM_CONDITION_NOT_MEETED"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_ADDITONAL_ITEM_CONDITION_NOT_MEETED;
	if (str.Compare(TEXT("BATTLE_PASS_ERROR_ACTIVATE_REWARD_CURRENCY_NOT_ENOUGH"), ESearchCase::IgnoreCase) == 0)	return Result::BATTLE_PASS_ERROR_ACTIVATE_REWARD_CURRENCY_NOT_ENOUGH;
	if (str.Compare(TEXT("DELIVERY_BOX_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::DELIVERY_BOX_ERROR_UNKNOWN;
	if (str.Compare(TEXT("DELIVERY_BOX_ERROR_NOT_EVENT_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::DELIVERY_BOX_ERROR_NOT_EVENT_TIME;
	if (str.Compare(TEXT("DELIVERY_BOX_ERROR_NOT_MEET_THE_EVENT_TYPE"), ESearchCase::IgnoreCase) == 0)	return Result::DELIVERY_BOX_ERROR_NOT_MEET_THE_EVENT_TYPE;
	if (str.Compare(TEXT("DELIVERY_BOX_ERROR_ALREADY_RECOVERED"), ESearchCase::IgnoreCase) == 0)	return Result::DELIVERY_BOX_ERROR_ALREADY_RECOVERED;
	if (str.Compare(TEXT("DELIVERY_BOX_ERROR_NOT_MEET_THE_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::DELIVERY_BOX_ERROR_NOT_MEET_THE_CONDITION;
	if (str.Compare(TEXT("COMMON_EFFECT_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::COMMON_EFFECT_ERROR_UNKNOWN;
	if (str.Compare(TEXT("COMMON_ERROR_NO_GATEWAY_SERVER_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return Result::COMMON_ERROR_NO_GATEWAY_SERVER_CONNECTION;
	if (str.Compare(TEXT("COMMON_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::COMMON_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("KEYBOARD_MAPPING_ERROR_SAVE"), ESearchCase::IgnoreCase) == 0)	return Result::KEYBOARD_MAPPING_ERROR_SAVE;
	if (str.Compare(TEXT("KEYBOARD_MAPPING_ERROR_LOAD"), ESearchCase::IgnoreCase) == 0)	return Result::KEYBOARD_MAPPING_ERROR_LOAD;
	if (str.Compare(TEXT("PK_BOOK_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_UNKNOWN;
	if (str.Compare(TEXT("PK_BOOK_ERROR_INVALID_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_INVALID_DATA;
	if (str.Compare(TEXT("PK_BOOK_ERROR_GATEWAY_FAIL"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_GATEWAY_FAIL;
	if (str.Compare(TEXT("PK_BOOK_ERROR_NOT_DIE_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_NOT_DIE_DATA;
	if (str.Compare(TEXT("PK_BOOK_ERROR_NOT_KILL_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_NOT_KILL_DATA;
	if (str.Compare(TEXT("PK_BOOK_ERROR_NOT_REMAIN_ACTION_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_NOT_REMAIN_ACTION_COUNT;
	if (str.Compare(TEXT("PK_BOOK_ERROR_EXPIRED_ACTION_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_EXPIRED_ACTION_TIME;
	if (str.Compare(TEXT("PK_BOOK_ERROR_DISABLE_ACTION_TO_NPC"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_DISABLE_ACTION_TO_NPC;
	if (str.Compare(TEXT("PK_BOOK_ERROR_NOT_WARPABLE_MAP"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_NOT_WARPABLE_MAP;
	if (str.Compare(TEXT("PK_BOOK_ERROR_NOT_LOGINED_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_NOT_LOGINED_USER;
	if (str.Compare(TEXT("PK_BOOK_ERROR_NOT_ENOUGH_ACTION_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_NOT_ENOUGH_ACTION_CURRENCY;
	if (str.Compare(TEXT("PK_BOOK_ERROR_PK_MESSAGE_INVALID_TEXT"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_PK_MESSAGE_INVALID_TEXT;
	if (str.Compare(TEXT("PK_BOOK_ERROR_MONITORRING_USER_LIST_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_MONITORRING_USER_LIST_FULL;
	if (str.Compare(TEXT("PK_BOOK_ERROR_ANOTER_SERVER_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_ANOTER_SERVER_USER;
	if (str.Compare(TEXT("PK_BOOK_ERROR_NO_WAPABLE_TO_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_NO_WAPABLE_TO_USER;
	if (str.Compare(TEXT("PK_BOOK_ERROR_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_INVALID_USER;
	if (str.Compare(TEXT("PK_BOOK_ERROR_ALREADY_MONITORRING_USER_ADDED"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_ALREADY_MONITORRING_USER_ADDED;
	if (str.Compare(TEXT("PK_BOOK_ERROR_CANNOT_ADD_MYSELF"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_CANNOT_ADD_MYSELF;
	if (str.Compare(TEXT("PK_BOOK_ERROR_CANNOT_ADD_GM"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_CANNOT_ADD_GM;
	if (str.Compare(TEXT("PK_BOOK_ERROR_CANNOT_ACTION_TO_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PK_BOOK_ERROR_CANNOT_ACTION_TO_USER;
	if (str.Compare(TEXT("GMTOOL_ERROR_BEGIN"), ESearchCase::IgnoreCase) == 0)	return Result::GMTOOL_ERROR_BEGIN;
	if (str.Compare(TEXT("GMTOOL_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::GMTOOL_ERROR_UNKNOWN;
	if (str.Compare(TEXT("GMTOOL_CONTENTS_EVENT_REG_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::GMTOOL_CONTENTS_EVENT_REG_ERROR_DB;
	if (str.Compare(TEXT("GMTOOL_CONTENTS_EVENT_REG_ALREADY"), ESearchCase::IgnoreCase) == 0)	return Result::GMTOOL_CONTENTS_EVENT_REG_ALREADY;
	if (str.Compare(TEXT("GMTOOL_CONTENTS_EVENT_REG_EXPIRED_DATE"), ESearchCase::IgnoreCase) == 0)	return Result::GMTOOL_CONTENTS_EVENT_REG_EXPIRED_DATE;
	if (str.Compare(TEXT("GMTOOL_CONTENTS_EVENT_REG_NOT_EXIST_DATA_SHEET"), ESearchCase::IgnoreCase) == 0)	return Result::GMTOOL_CONTENTS_EVENT_REG_NOT_EXIST_DATA_SHEET;
	if (str.Compare(TEXT("GMTOOL_CONTENTS_EVENT_REMOVE_NOT_REG"), ESearchCase::IgnoreCase) == 0)	return Result::GMTOOL_CONTENTS_EVENT_REMOVE_NOT_REG;
	if (str.Compare(TEXT("GMTOOL_CONTENTS_EVENT_REMOVE_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::GMTOOL_CONTENTS_EVENT_REMOVE_ERROR_DB;
	if (str.Compare(TEXT("GMTOOL_GM_USER_CACHE_MISSING"), ESearchCase::IgnoreCase) == 0)	return Result::GMTOOL_GM_USER_CACHE_MISSING;
	if (str.Compare(TEXT("APP_GUARD_ERROR_INVALID_PLATFORM"), ESearchCase::IgnoreCase) == 0)	return Result::APP_GUARD_ERROR_INVALID_PLATFORM;
	if (str.Compare(TEXT("APP_GUARD_ERROR_INVALID_ACTIVE_MODE"), ESearchCase::IgnoreCase) == 0)	return Result::APP_GUARD_ERROR_INVALID_ACTIVE_MODE;
	if (str.Compare(TEXT("DELETE_ACCOUNT_HIVE_CONNECT_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_ACCOUNT_HIVE_CONNECT_ERROR;
	if (str.Compare(TEXT("DELETE_ACCOUNT_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_ACCOUNT_ERROR;
	if (str.Compare(TEXT("DELETE_ACCOUNT_DB_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::DELETE_ACCOUNT_DB_ERROR;
	if (str.Compare(TEXT("HIVE_DELETE_ACCOUNT_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_DELETE_ACCOUNT_ERROR;
	if (str.Compare(TEXT("HIVE_DELETE_ACCOUNT_CERTIFICATION_KEY_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_DELETE_ACCOUNT_CERTIFICATION_KEY_ERROR;
	if (str.Compare(TEXT("HIVE_DELETE_ACCOUNT_TOKEN_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_DELETE_ACCOUNT_TOKEN_ERROR;
	if (str.Compare(TEXT("HIVE_DELETE_ACCOUNT_PARSE_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::HIVE_DELETE_ACCOUNT_PARSE_ERROR;
	if (str.Compare(TEXT("CREATE_USER_ERROR_DB_POSITION_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::CREATE_USER_ERROR_DB_POSITION_ERROR;
	if (str.Compare(TEXT("CHARACTER_CUSTOMIZING_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::CHARACTER_CUSTOMIZING_ERROR_UNKNOWN;
	if (str.Compare(TEXT("CHARACTER_CUSTOMIZING_ERROR_EXCEED_MAX_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::CHARACTER_CUSTOMIZING_ERROR_EXCEED_MAX_COUNT;
	if (str.Compare(TEXT("CHARACTER_CUSTOMIZING_ERROR_NOT_ENOUGH_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::CHARACTER_CUSTOMIZING_ERROR_NOT_ENOUGH_COUNT;
	if (str.Compare(TEXT("SOCIAL_EMOTION_ACTIVE"), ESearchCase::IgnoreCase) == 0)	return Result::SOCIAL_EMOTION_ACTIVE;
	if (str.Compare(TEXT("SEAL_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::SEAL_ERROR_UNKNOWN;
	if (str.Compare(TEXT("SEAL_ERROR_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::SEAL_ERROR_INVALID_STATUS;
	if (str.Compare(TEXT("SEAL_ERROR_SLOT_LOCK_EXCEED_MAX"), ESearchCase::IgnoreCase) == 0)	return Result::SEAL_ERROR_SLOT_LOCK_EXCEED_MAX;
	if (str.Compare(TEXT("SEAL_ERROR_NOT_ENOUGH_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::SEAL_ERROR_NOT_ENOUGH_LEVEL;
	if (str.Compare(TEXT("SEAL_ERROR_SLOT_NOT_USABLE_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::SEAL_ERROR_SLOT_NOT_USABLE_LEVEL;
	if (str.Compare(TEXT("SEAL_ERROR_LEVEL_EXCEED_MAX"), ESearchCase::IgnoreCase) == 0)	return Result::SEAL_ERROR_LEVEL_EXCEED_MAX;
	if (str.Compare(TEXT("SEAL_ERROR_INVALID_STAT_POOL_ID"), ESearchCase::IgnoreCase) == 0)	return Result::SEAL_ERROR_INVALID_STAT_POOL_ID;
	if (str.Compare(TEXT("QUEST_ERROR_REQ_GUILD_QUEST_INFO"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_REQ_GUILD_QUEST_INFO;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_QUEST_CONFIG_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_QUEST_CONFIG_DATA;
	if (str.Compare(TEXT("QUEST_ERROR_USER_IS_NOT_GUILD_MEMBER"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_USER_IS_NOT_GUILD_MEMBER;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_ENOUGH_GUILD_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_ENOUGH_GUILD_LEVEL;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_QUEST_GENERATE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_QUEST_GENERATE;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_QUEST_GENERATE_IN_DB"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_QUEST_GENERATE_IN_DB;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_USER_STATUS;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_FOUND_GUILD_QUEST_ID_IN_DATA_SHEET"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_FOUND_GUILD_QUEST_ID_IN_DATA_SHEET;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_FOUND_GUILD_QUEST_INFO_IN_DATA_SHEET"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_FOUND_GUILD_QUEST_INFO_IN_DATA_SHEET;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_ALREADY_ACCEPT_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_ALREADY_ACCEPT_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_ALREADY_ACCEPTED_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_ALREADY_ACCEPTED_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_NOT_ACCEPTABLE_TODAY"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_NOT_ACCEPTABLE_TODAY;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_EXCEED_ACCEPT_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_EXCEED_ACCEPT_COUNT;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_REWARD_GUILD_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_REWARD_GUILD_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_LAST_REWARD_GUILD_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_LAST_REWARD_GUILD_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_ALREADY_REWARDED_GUILD_QUEST_ID"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_ALREADY_REWARDED_GUILD_QUEST_ID;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_COMPLETED_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_COMPLETED_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_INVALID_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_INVALID_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_ACCEPTED_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_ACCEPTED_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_EXCEED_GUILD_QUEST_REFRESH_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_EXCEED_GUILD_QUEST_REFRESH_COUNT;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_ENOUGH_GUILD_QUEST_REFRESH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_ENOUGH_GUILD_QUEST_REFRESH_CURRENCY;
	if (str.Compare(TEXT("QUEST_ERROR_NOT_FOUND_GUILD_QUEST_REFRESH_INFO"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NOT_FOUND_GUILD_QUEST_REFRESH_INFO;
	if (str.Compare(TEXT("QUEST_ERROR_NO_REFRESHABLE_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_NO_REFRESHABLE_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_EXPIRED_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_EXPIRED_GUILD_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_NOT_ACCEPTABLE_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_NOT_ACCEPTABLE_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_REWARD_ITEM_CREATE"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_REWARD_ITEM_CREATE;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_NOT_REWARDABLE_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_NOT_REWARDABLE_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_ACCEPTED_QUEST"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_ACCEPTED_QUEST;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_REFRESH"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_REFRESH;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_REFRESH_REQ_DB"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_REFRESH_REQ_DB;
	if (str.Compare(TEXT("QUEST_ERROR_GUILD_EXCEED_COMPLETE_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::QUEST_ERROR_GUILD_EXCEED_COMPLETE_COUNT;
	if (str.Compare(TEXT("PERSONAL_TRADING_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_INVALID_USER_STATUS;
	if (str.Compare(TEXT("PERSONAL_TRADING_NOT_FOUND_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_NOT_FOUND_USER;
	if (str.Compare(TEXT("PERSONAL_TRADING_IMPOSSIBLE_USER"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_IMPOSSIBLE_USER;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_INVALID_RANGE"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_INVALID_RANGE;
	if (str.Compare(TEXT("PERSONAL_TRADING_INVALID_OTHER_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_INVALID_OTHER_USER_STATUS;
	if (str.Compare(TEXT("PERSONAL_TRADING_DONT_ACTION"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_DONT_ACTION;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_OHTER_TRADING"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_OHTER_TRADING;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_TRAGET_USER_OHTER_TRADING"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_TRAGET_USER_OHTER_TRADING;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_SAME_POS"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_SAME_POS;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_TRY_CONFIRM"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_TRY_CONFIRM;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_NOT_TRADING"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_NOT_TRADING;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_ITEM_ADD"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_ITEM_ADD;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_NOT_ENOUGH_DIA"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_DIA;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_DIA"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_DIA;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_NOT_DIA_FULLY"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_NOT_DIA_FULLY;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_NOT_ENOUGH_INVENTORY_SLOT"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_INVENTORY_SLOT;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_INVENTORY_SLOT"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_INVENTORY_SLOT;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_WEIGHT_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_WEIGHT_FULL;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_TARGET_WEIGHT_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_TARGET_WEIGHT_FULL;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_TIME_OUT"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_TIME_OUT;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_LEVEL;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_TARGET_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_TARGET_LEVEL;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_TRY_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_TRY_COUNT;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_TRAGET_TRY_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_TRAGET_TRY_COUNT;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_UNKNOWN;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_INVALID_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_INVALID_ITEM;
	if (str.Compare(TEXT("PERSONAL_TRADING_ALREADY_TRADING"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ALREADY_TRADING;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_USABLE_IN_SAFE_ZONE"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_USABLE_IN_SAFE_ZONE;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_CANNOT_USABLE_GM"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_CANNOT_USABLE_GM;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_OTHER_USABLE_IN_SAFE_ZONE"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_OTHER_USABLE_IN_SAFE_ZONE;
	if (str.Compare(TEXT("PERSONAL_TRADING_ERROR_NO_DEVICE_REGISTERED"), ESearchCase::IgnoreCase) == 0)	return Result::PERSONAL_TRADING_ERROR_NO_DEVICE_REGISTERED;
	if (str.Compare(TEXT("COMMUNITY_EVENT_MISSION_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::COMMUNITY_EVENT_MISSION_INVALID_USER_STATUS;
	if (str.Compare(TEXT("COMMUNITY_EVENT_MISSION_INVALID_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::COMMUNITY_EVENT_MISSION_INVALID_DATA;
	if (str.Compare(TEXT("COMMUNITY_EVENT_MISSION_NOT_EVENT_PERIOD"), ESearchCase::IgnoreCase) == 0)	return Result::COMMUNITY_EVENT_MISSION_NOT_EVENT_PERIOD;
	if (str.Compare(TEXT("COMMUNITY_EVENT_MISSION_NOT_TAB_OPEN_PERIOD"), ESearchCase::IgnoreCase) == 0)	return Result::COMMUNITY_EVENT_MISSION_NOT_TAB_OPEN_PERIOD;
	if (str.Compare(TEXT("COMMUNITY_EVENT_MISSION_NOT_EXIST_MISSION_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::COMMUNITY_EVENT_MISSION_NOT_EXIST_MISSION_DATA;
	if (str.Compare(TEXT("COMMUNITY_EVENT_MISSION_NOT_MEET_THE_CONDITION_MISSION_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::COMMUNITY_EVENT_MISSION_NOT_MEET_THE_CONDITION_MISSION_COUNT;
	if (str.Compare(TEXT("COMMUNITY_EVENT_MISSION_IS_ALREADY_REWARDED_MISSION"), ESearchCase::IgnoreCase) == 0)	return Result::COMMUNITY_EVENT_MISSION_IS_ALREADY_REWARDED_MISSION;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_INVALID_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_INVALID_ID;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_EXPIRED_DATE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_EXPIRED_DATE_TIME;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_DIVISION_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_DIVISION_STATE;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_APPOINTMENT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_APPOINTMENT;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_PARTICIPANTS"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_PARTICIPANTS;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_DB;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_BUY_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_BUY_STATE;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_RETURN_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_RETURN_CURRENCY;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_DUPLICATION_REGISTER"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_DUPLICATION_REGISTER;
	if (str.Compare(TEXT("GUILD_LEAVE_ERROR_AUCTION_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_LEAVE_ERROR_AUCTION_DATA;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_JOINED_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_JOINED_TIME;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_FULL_REGISTRATION"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_FULL_REGISTRATION;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_HIGHER_BID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_HIGHER_BID;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_EMPTY_ITEM"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_EMPTY_ITEM;
	if (str.Compare(TEXT("GUILD_AUCTION_ERROR_SAFE_CAPACITY_FULL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_AUCTION_ERROR_SAFE_CAPACITY_FULL;
	if (str.Compare(TEXT("GUILD_SNS_URL_ERROR_INVALID_ID"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_SNS_URL_ERROR_INVALID_ID;
	if (str.Compare(TEXT("GUILD_SNS_URL_ERROR_INVALID_LEN"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_SNS_URL_ERROR_INVALID_LEN;
	if (str.Compare(TEXT("NICKNAME_CHANGE_ERROR_NOT_ALLOWED_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::NICKNAME_CHANGE_ERROR_NOT_ALLOWED_NAME;
	if (str.Compare(TEXT("NICKNAME_CHANGE_ERROR_INVALID_LENGTH_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::NICKNAME_CHANGE_ERROR_INVALID_LENGTH_NAME;
	if (str.Compare(TEXT("NICKNAME_CHANGE_ERROR_DUPLICATION_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::NICKNAME_CHANGE_ERROR_DUPLICATION_NAME;
	if (str.Compare(TEXT("NICKNAME_CHANGE_ERROR_NOT_TOWN"), ESearchCase::IgnoreCase) == 0)	return Result::NICKNAME_CHANGE_ERROR_NOT_TOWN;
	if (str.Compare(TEXT("NICKNAME_CHANGE_ERROR_GUILD_APPLICANTS"), ESearchCase::IgnoreCase) == 0)	return Result::NICKNAME_CHANGE_ERROR_GUILD_APPLICANTS;
	if (str.Compare(TEXT("NICKNAME_CHANGE_ERROR_DB"), ESearchCase::IgnoreCase) == 0)	return Result::NICKNAME_CHANGE_ERROR_DB;
	if (str.Compare(TEXT("INVADE_ERROR_PORTAL_SPAWNING"), ESearchCase::IgnoreCase) == 0)	return Result::INVADE_ERROR_PORTAL_SPAWNING;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_UNKNOWN_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_UNKNOWN_ERROR;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_INVALID_USER_STATUS"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_INVALID_USER_STATUS;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_NOT_MEET_THE_CONDITION"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_NOT_MEET_THE_CONDITION;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_ALREADY_IN_PROCESS"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_ALREADY_IN_PROCESS;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_SERVER_SESSION_IS_INVALID"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_SERVER_SESSION_IS_INVALID;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_DB_QUERY_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_DB_QUERY_FAILED;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_DATA_TRANSFER_IN_PROCESS"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_DATA_TRANSFER_IN_PROCESS;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_ACCOUNT;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_USER"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_USER;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_CHECK_CONDITION_GUILD_MEMBER"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_CHECK_CONDITION_GUILD_MEMBER;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_NICKNAME_ALREADY_CHANGED"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_NICKNAME_ALREADY_CHANGED;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_GUILD_NAME_ALREADY_CHANGED"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_GUILD_NAME_ALREADY_CHANGED;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_TARGET_SERVER_EXCEED_INCOMING_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_TARGET_SERVER_EXCEED_INCOMING_COUNT;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_CURRENT_SERVER_EXCEED_OUTGOING_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_CURRENT_SERVER_EXCEED_OUTGOING_COUNT;
	if (str.Compare(TEXT("SERVER_MIGRATE_ERROR_NOT_ENOUGH_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return Result::SERVER_MIGRATE_ERROR_NOT_ENOUGH_CURRENCY;
	if (str.Compare(TEXT("ABUSE_PACKET_SHORT_TICK"), ESearchCase::IgnoreCase) == 0)	return Result::ABUSE_PACKET_SHORT_TICK;
	if (str.Compare(TEXT("GUILD_WAR_UNKNOWN_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_UNKNOWN_ERROR;
	if (str.Compare(TEXT("GUILD_WAR_MATCH_NOT_FOUND_ENEMY_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_MATCH_NOT_FOUND_ENEMY_GUILD;
	if (str.Compare(TEXT("GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MINIMUM_LIMIT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MINIMUM_LIMIT;
	if (str.Compare(TEXT("GUILD_WAR_ENTER_FAIL_NOT_REGISTERED_IN_ENTRY"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_ENTER_FAIL_NOT_REGISTERED_IN_ENTRY;
	if (str.Compare(TEXT("GUILD_WAR_ENTER_FAIL_INVALID_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_ENTER_FAIL_INVALID_STATE;
	if (str.Compare(TEXT("GUILD_WAR_MATCH_REGISTER_FAIL_ALREADY_REGISTER"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_MATCH_REGISTER_FAIL_ALREADY_REGISTER;
	if (str.Compare(TEXT("GUILD_WAR_MATCH_REGISTER_FAIL_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_MATCH_REGISTER_FAIL_STATE;
	if (str.Compare(TEXT("GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MAX_LIMIT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MAX_LIMIT;
	if (str.Compare(TEXT("GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_GRADE_LIMIT"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_GRADE_LIMIT;
	if (str.Compare(TEXT("GUILD_WAR_MATCH_REGISTER_FAIL_LEVEL"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_MATCH_REGISTER_FAIL_LEVEL;
	if (str.Compare(TEXT("GUILD_WAR_ERROR_THE_DAY_JOINED_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::GUILD_WAR_ERROR_THE_DAY_JOINED_GUILD;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_UNKNOWN_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_UNKNOWN_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_WORLD_UNKNOWN_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_WORLD_UNKNOWN_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_ACCOUNT_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_ACCOUNT_DATA;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NOT_YET_RESET"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NOT_YET_RESET;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NOT_EXIST_DETAIL_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NOT_EXIST_DETAIL_DATA;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_INVALID_USER_STATE"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_INVALID_USER_STATE;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_INVALID_PARRAM"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_INVALID_PARRAM;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NOT_GUILD_MEMBER"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NOT_GUILD_MEMBER;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_OPEN_GRADE_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_OPEN_GRADE_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_CONFIG_DATA_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_CONFIG_DATA_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_CONTENTS_GUILD_LEVEL_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_CONTENTS_GUILD_LEVEL_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_GUILD_LEVEL_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_GUILD_LEVEL_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_DETAIL_DATA_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_DETAIL_DATA_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_ALREADY_OPENED"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_ALREADY_OPENED;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NO_MAP_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NO_MAP_DATA;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NOT_ENOUGH_GUILD_ASSET"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NOT_ENOUGH_GUILD_ASSET;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_FAIL_TO_CREATE_CHANNEL"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_FAIL_TO_CREATE_CHANNEL;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NO_USER_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NO_USER_DATA;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_GUILD_JOINED_DAY_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_GUILD_JOINED_DAY_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_MAX_CLEAR_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_MAX_CLEAR_COUNT;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NOT_AVAILABLE_TIME"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NOT_AVAILABLE_TIME;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_DUNGEON_NOT_CLOSED"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_DUNGEON_NOT_CLOSED;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_DB_OPERATION_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_DB_OPERATION_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NO_MAP_TO_CLOSE"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NO_MAP_TO_CLOSE;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NO_CHANNEL_TO_CLOSE"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NO_CHANNEL_TO_CLOSE;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_FAIL_TO_CREATE_RAID_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_FAIL_TO_CREATE_RAID_DATA;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NOT_EXIST_RAID_DATA"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NOT_EXIST_RAID_DATA;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_DUNGEON_NOT_OPENED"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_DUNGEON_NOT_OPENED;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NOT_EXIST_USER_CHANNEL"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NOT_EXIST_USER_CHANNEL;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_MAP_TYPE_ERROR"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_MAP_TYPE_ERROR;
	if (str.Compare(TEXT("AGIT_DUNGEON_NO_CHANNEL_TO_ENTER"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_NO_CHANNEL_TO_ENTER;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_WARP_ERROR_ABNORMALITY_EFFECT"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_WARP_ERROR_ABNORMALITY_EFFECT;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NOT_ENOUGH_USER_TICKET_COUNT"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NOT_ENOUGH_USER_TICKET_COUNT;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_NOT_FOUND_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_NOT_FOUND_GUILD;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_DESTROYING"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_DESTROYING;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_FAIL_TO_CLOSE_USER_EXISTS"), ESearchCase::IgnoreCase) == 0)	return Result::AGIT_DUNGEON_RAID_FAIL_TO_CLOSE_USER_EXISTS;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_INVALID_ROOM_NAME"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_INVALID_ROOM_NAME;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_JOIN_OVER_LIMIT"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_JOIN_OVER_LIMIT;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_MEMBER_OVER_LIMIT"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_MEMBER_OVER_LIMIT;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_INVITE_OVER_LIMIT"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_INVITE_OVER_LIMIT;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_INVALID_ROOM_ID"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_INVALID_ROOM_ID;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_ALREADY_INVITED"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_ALREADY_INVITED;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_NOT_INVITED"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_NOT_INVITED;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_ALREADY_JOINED"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_ALREADY_JOINED;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_DB_FAILED"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_DB_FAILED;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_NOT_LEADER_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_NOT_LEADER_GUILD;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_NOT_JOIN_GUILD"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_NOT_JOIN_GUILD;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_DO_NOT_WITHDRAWAL"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_DO_NOT_WITHDRAWAL;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_DO_WRONG_GUILD_ID"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_DO_WRONG_GUILD_ID;
	if (str.Compare(TEXT("CHAT_GUILD_GROUP_DATA_DIFF"), ESearchCase::IgnoreCase) == 0)	return Result::CHAT_GUILD_GROUP_DATA_DIFF;
	return Result::PACKET_RESULT_MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString ResultEnumToString(Result value)
{
	switch (value)
	{
		case Result::RESULT_SUCCESS: return TEXT("RESULT_SUCCESS");
		case Result::LOGIN_ERROR_RETRY: return TEXT("LOGIN_ERROR_RETRY");
		case Result::LOGIN_ERROR_RESTART: return TEXT("LOGIN_ERROR_RESTART");
		case Result::LOGIN_ERROR_UNKNOWN: return TEXT("LOGIN_ERROR_UNKNOWN");
		case Result::LOGIN_ERROR_WRONG_WZ_PACKET_VERSION: return TEXT("LOGIN_ERROR_WRONG_WZ_PACKET_VERSION");
		case Result::LOGIN_ERROR_INTERNAL_SERVER_ERROR: return TEXT("LOGIN_ERROR_INTERNAL_SERVER_ERROR");
		case Result::LOGIN_ERROR_INTERNAL_DB_ERROR: return TEXT("LOGIN_ERROR_INTERNAL_DB_ERROR");
		case Result::LOGIN_ERROR_NEED_UPDATE_ACCOUNT_NAME: return TEXT("LOGIN_ERROR_NEED_UPDATE_ACCOUNT_NAME");
		case Result::LOGIN_ERROR_ID_PW_AUTH_FAILED: return TEXT("LOGIN_ERROR_ID_PW_AUTH_FAILED");
		case Result::LOGIN_ERROR_INVALID_AUTHORIZATION: return TEXT("LOGIN_ERROR_INVALID_AUTHORIZATION");
		case Result::LOGIN_ERROR_GETTING_READY_FOR_GATEWAYSERVER: return TEXT("LOGIN_ERROR_GETTING_READY_FOR_GATEWAYSERVER");
		case Result::LOGIN_ERROR_INVALID_ACCOUNT_ID: return TEXT("LOGIN_ERROR_INVALID_ACCOUNT_ID");
		case Result::LOGIN_ERROR_INVALID_VID: return TEXT("LOGIN_ERROR_INVALID_VID");
		case Result::LOGIN_ERROR_INCORRECT_TOKEN: return TEXT("LOGIN_ERROR_INCORRECT_TOKEN");
		case Result::LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB: return TEXT("LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB");
		case Result::LOGIN_ERROR_TIME_OUT: return TEXT("LOGIN_ERROR_TIME_OUT");
		case Result::LOGIN_ERROR_INVALID_AUTH_STATUS: return TEXT("LOGIN_ERROR_INVALID_AUTH_STATUS");
		case Result::LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN: return TEXT("LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN");
		case Result::LOGIN_ERROR_SERVER_MAINTENANCE: return TEXT("LOGIN_ERROR_SERVER_MAINTENANCE");
		case Result::LOGIN_ERROR_NOT_MATCHED_VID_PLANETWORLD_ID: return TEXT("LOGIN_ERROR_NOT_MATCHED_VID_PLANETWORLD_ID");
		case Result::LOGIN_ERROR_HIVE_VERIFY_TOKEN_SETTING: return TEXT("LOGIN_ERROR_HIVE_VERIFY_TOKEN_SETTING");
		case Result::LOGIN_ERROR_DELETED_ACCOUNT: return TEXT("LOGIN_ERROR_DELETED_ACCOUNT");
		case Result::SELECT_SERVER_ERROR_INACCESSIBLE: return TEXT("SELECT_SERVER_ERROR_INACCESSIBLE");
		case Result::SELECT_SERVER_ERROR_OVERLOAD: return TEXT("SELECT_SERVER_ERROR_OVERLOAD");
		case Result::SELECT_SERVER_ERROR_NOT_FOUND: return TEXT("SELECT_SERVER_ERROR_NOT_FOUND");
		case Result::LOGIN_ERROR_HIVE_REGISTER_DEVICE_SETTING: return TEXT("LOGIN_ERROR_HIVE_REGISTER_DEVICE_SETTING");
		case Result::LOGIN_ERROR_NO_CHARACTER_CREATED: return TEXT("LOGIN_ERROR_NO_CHARACTER_CREATED");
		case Result::LOGIN_ERROR_HIVE_INVALID_PARAMETER: return TEXT("LOGIN_ERROR_HIVE_INVALID_PARAMETER");
		case Result::LOGIN_ERROR_HIVE_REQUEST_JSON: return TEXT("LOGIN_ERROR_HIVE_REQUEST_JSON");
		case Result::LOGIN_ERROR_HIVE_DB_ERROR: return TEXT("LOGIN_ERROR_HIVE_DB_ERROR");
		case Result::LOGIN_ERROR_HIVE_INVALID_TOKEN_AWS: return TEXT("LOGIN_ERROR_HIVE_INVALID_TOKEN_AWS");
		case Result::LOGIN_ERROR_HIVE_INVALID_APPID: return TEXT("LOGIN_ERROR_HIVE_INVALID_APPID");
		case Result::LOGIN_ERROR_HIVE_IDP_TOKEN_ERROR: return TEXT("LOGIN_ERROR_HIVE_IDP_TOKEN_ERROR");
		case Result::LOGIN_ERROR_HIVE_INVALID_TOKEN: return TEXT("LOGIN_ERROR_HIVE_INVALID_TOKEN");
		case Result::LOGIN_ERROR_HIVE_NO_TOKEN_HEADER: return TEXT("LOGIN_ERROR_HIVE_NO_TOKEN_HEADER");
		case Result::LOGIN_ERROR_HIVE_UNKNOWN_ERROR: return TEXT("LOGIN_ERROR_HIVE_UNKNOWN_ERROR");
		case Result::LOGIN_ERROR_HIVE_UNABLE_TO_CONNECT: return TEXT("LOGIN_ERROR_HIVE_UNABLE_TO_CONNECT");
		case Result::LOGIN_ERROR_HIVE_CONNECT: return TEXT("LOGIN_ERROR_HIVE_CONNECT");
		case Result::LOGIN_ERROR_HIVE_PARSE: return TEXT("LOGIN_ERROR_HIVE_PARSE");
		case Result::LOGIN_ERROR_HIVE_NO_RESULT_CODE: return TEXT("LOGIN_ERROR_HIVE_NO_RESULT_CODE");
		case Result::LOGIN_ERROR_HIVE_UNKNOWN_RESULT_CODE: return TEXT("LOGIN_ERROR_HIVE_UNKNOWN_RESULT_CODE");
		case Result::LOGIN_ERROR_HIVE_NO_REGISTER_DEVICE_PARAMETER: return TEXT("LOGIN_ERROR_HIVE_NO_REGISTER_DEVICE_PARAMETER");
		case Result::LOGIN_ERROR_HIVE_CONNECT_REGISTER_DEVICE: return TEXT("LOGIN_ERROR_HIVE_CONNECT_REGISTER_DEVICE");
		case Result::LOGIN_ERROR_HIVE_PARSE_REGISTER_DEVICE: return TEXT("LOGIN_ERROR_HIVE_PARSE_REGISTER_DEVICE");
		case Result::SELECT_CHANNEL_ERROR_UNKNOWN: return TEXT("SELECT_CHANNEL_ERROR_UNKNOWN");
		case Result::SELECT_CHANNEL_ERROR_INVALID_STATUS: return TEXT("SELECT_CHANNEL_ERROR_INVALID_STATUS");
		case Result::SELECT_CHANNEL_ERROR_INVALID_AREA: return TEXT("SELECT_CHANNEL_ERROR_INVALID_AREA");
		case Result::SELECT_CHANNEL_ERROR_SAME_AREA_CHANNEL: return TEXT("SELECT_CHANNEL_ERROR_SAME_AREA_CHANNEL");
		case Result::SELECT_CHANNEL_ERROR_MAP_TYPE: return TEXT("SELECT_CHANNEL_ERROR_MAP_TYPE");
		case Result::SELECT_CHANNEL_ERROR_IN_COMBAT: return TEXT("SELECT_CHANNEL_ERROR_IN_COMBAT");
		case Result::SELECT_CHANNEL_ERROR_IN_SHUTTLE: return TEXT("SELECT_CHANNEL_ERROR_IN_SHUTTLE");
		case Result::SELECT_CHANNEL_ERROR_INVALID_CHANNEL: return TEXT("SELECT_CHANNEL_ERROR_INVALID_CHANNEL");
		case Result::SELECT_CHANNEL_ERROR_CONGESTION_FULL: return TEXT("SELECT_CHANNEL_ERROR_CONGESTION_FULL");
		case Result::ZPAY_LOGIN_ERROR_INVALID_AUTH: return TEXT("ZPAY_LOGIN_ERROR_INVALID_AUTH");
		case Result::ZPAY_LOGIN_ERROR_ENTER_SERVER: return TEXT("ZPAY_LOGIN_ERROR_ENTER_SERVER");
		case Result::ZPAY_LOGIN_ERROR_PLAY_USER: return TEXT("ZPAY_LOGIN_ERROR_PLAY_USER");
		case Result::ZPAY_LOGIN_ERROR_UNLOCK_CONTENTS: return TEXT("ZPAY_LOGIN_ERROR_UNLOCK_CONTENTS");
		case Result::ITEM_ERROR_UNKNOWN: return TEXT("ITEM_ERROR_UNKNOWN");
		case Result::ITEM_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("ITEM_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::ITEM_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("ITEM_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::ITEM_ERROR_INVALID_USER: return TEXT("ITEM_ERROR_INVALID_USER");
		case Result::ITEM_ERROR_INVALID_USER_STATUS: return TEXT("ITEM_ERROR_INVALID_USER_STATUS");
		case Result::ITEM_ERROR_NOT_ALLOWED_LEVEL: return TEXT("ITEM_ERROR_NOT_ALLOWED_LEVEL");
		case Result::ITEM_ERROR_ITEM_NOT_FOUND: return TEXT("ITEM_ERROR_ITEM_NOT_FOUND");
		case Result::ITEM_ERROR_INVALID_ITEM_ID: return TEXT("ITEM_ERROR_INVALID_ITEM_ID");
		case Result::ITEM_ERROR_INVALID_ITEM_DB_ID: return TEXT("ITEM_ERROR_INVALID_ITEM_DB_ID");
		case Result::ITEM_ERROR_ADD_INVENTORY_FULL: return TEXT("ITEM_ERROR_ADD_INVENTORY_FULL");
		case Result::ITEM_ERROR_ADD_NOT_ALLOWED_GENDER: return TEXT("ITEM_ERROR_ADD_NOT_ALLOWED_GENDER");
		case Result::ITEM_ERROR_ADD_INVALID_STACK_COUNT: return TEXT("ITEM_ERROR_ADD_INVALID_STACK_COUNT");
		case Result::ITEM_ERROR_ADD_WEIGHT_FULL: return TEXT("ITEM_ERROR_ADD_WEIGHT_FULL");
		case Result::ITEM_ERROR_NOT_ENOUGH_MONEY: return TEXT("ITEM_ERROR_NOT_ENOUGH_MONEY");
		case Result::ITEM_ERROR_OVERFLOW_MAX_MONEY: return TEXT("ITEM_ERROR_OVERFLOW_MAX_MONEY");
		case Result::ITEM_ERROR_DELETE_LOCKED: return TEXT("ITEM_ERROR_DELETE_LOCKED");
		case Result::ITEM_ERROR_LOCK_ALREADY_LOCKED: return TEXT("ITEM_ERROR_LOCK_ALREADY_LOCKED");
		case Result::ITEM_ERROR_LOCK_ALREADY_UNLOCKED: return TEXT("ITEM_ERROR_LOCK_ALREADY_UNLOCKED");
		case Result::ITEM_ERROR_LOCK_NOT_LOCKABLE: return TEXT("ITEM_ERROR_LOCK_NOT_LOCKABLE");
		case Result::ITEM_ERROR_DECREASE_INSUFFICIENT: return TEXT("ITEM_ERROR_DECREASE_INSUFFICIENT");
		case Result::ITEM_ERROR_EQUIP_NOT_EQUIP_ITEM: return TEXT("ITEM_ERROR_EQUIP_NOT_EQUIP_ITEM");
		case Result::ITEM_ERROR_EQUIP_ALREADY_EQUIPPED: return TEXT("ITEM_ERROR_EQUIP_ALREADY_EQUIPPED");
		case Result::ITEM_ERROR_EQUIP_ALREADY_UNEQUIPPED: return TEXT("ITEM_ERROR_EQUIP_ALREADY_UNEQUIPPED");
		case Result::ITEM_ERROR_EQUIP_INVALID_PRESET_ID: return TEXT("ITEM_ERROR_EQUIP_INVALID_PRESET_ID");
		case Result::ITEM_ERROR_EQUIP_INVALID_PRESET_NAME: return TEXT("ITEM_ERROR_EQUIP_INVALID_PRESET_NAME");
		case Result::ITEM_ERROR_EQUIP_CHANGE_WEAPON_TPYE: return TEXT("ITEM_ERROR_EQUIP_CHANGE_WEAPON_TPYE");
		case Result::ITEM_ERROR_EQUIP_CAN_NOT_UNEQUIP_WEAPON: return TEXT("ITEM_ERROR_EQUIP_CAN_NOT_UNEQUIP_WEAPON");
		case Result::ITEM_ERROR_SOCKET_INVALID_TARGET: return TEXT("ITEM_ERROR_SOCKET_INVALID_TARGET");
		case Result::ITEM_ERROR_SOCKET_ALREADY_SOCKETED: return TEXT("ITEM_ERROR_SOCKET_ALREADY_SOCKETED");
		case Result::ITEM_ERROR_SOCKET_IS_NOT_EMPTY: return TEXT("ITEM_ERROR_SOCKET_IS_NOT_EMPTY");
		case Result::ITEM_ERROR_USE_NOT_USABLE_ITEM: return TEXT("ITEM_ERROR_USE_NOT_USABLE_ITEM");
		case Result::ITEM_ERROR_USE_INVALID_EFFECT_TYPE: return TEXT("ITEM_ERROR_USE_INVALID_EFFECT_TYPE");
		case Result::ITEM_ERROR_USE_INVALID_EFFECT_PARAM: return TEXT("ITEM_ERROR_USE_INVALID_EFFECT_PARAM");
		case Result::ITEM_ERROR_USE_NOT_ENOUGH_ITEMS: return TEXT("ITEM_ERROR_USE_NOT_ENOUGH_ITEMS");
		case Result::ITEM_ERROR_USE_IN_COOLDOWN: return TEXT("ITEM_ERROR_USE_IN_COOLDOWN");
		case Result::ITEM_ERROR_USE_POTION_HP_MP_FULL: return TEXT("ITEM_ERROR_USE_POTION_HP_MP_FULL");
		case Result::ITEM_ERROR_USE_LOCKED: return TEXT("ITEM_ERROR_USE_LOCKED");
		case Result::ITEM_ERROR_USE_ALREADY_LEARN_SKILL: return TEXT("ITEM_ERROR_USE_ALREADY_LEARN_SKILL");
		case Result::ITEM_ERROR_USE_MAX_USED_BONUS_STAT_POINT_ITEM: return TEXT("ITEM_ERROR_USE_MAX_USED_BONUS_STAT_POINT_ITEM");
		case Result::ITEM_ERROR_USE_PVP_ALIGNMENT: return TEXT("ITEM_ERROR_USE_PVP_ALIGNMENT");
		case Result::ITEM_ERROR_ENCHANT_INVALID_SCROLL: return TEXT("ITEM_ERROR_ENCHANT_INVALID_SCROLL");
		case Result::ITEM_ERROR_ENCHANT_INVALID_AMOUNT: return TEXT("ITEM_ERROR_ENCHANT_INVALID_AMOUNT");
		case Result::ITEM_ERROR_ENCHANT_RATE_FAILED: return TEXT("ITEM_ERROR_ENCHANT_RATE_FAILED");
		case Result::ITEM_ERROR_ENCHANT_EQUIPED_ITEM_CANNOT_OVER_SAFE_LEVEL: return TEXT("ITEM_ERROR_ENCHANT_EQUIPED_ITEM_CANNOT_OVER_SAFE_LEVEL");
		case Result::ITEM_ERROR_ENCHANT_LOCKED: return TEXT("ITEM_ERROR_ENCHANT_LOCKED");
		case Result::ITEM_ERROR_ENCHANT_INVALID_TARGET: return TEXT("ITEM_ERROR_ENCHANT_INVALID_TARGET");
		case Result::ITEM_ERROR_ENCHANT_INVALID_LEVEL: return TEXT("ITEM_ERROR_ENCHANT_INVALID_LEVEL");
		case Result::ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY: return TEXT("ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY");
		case Result::ITEM_ERROR_DECOMPOSE_INVALID_EQUIPED: return TEXT("ITEM_ERROR_DECOMPOSE_INVALID_EQUIPED");
		case Result::ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY_EQUIP: return TEXT("ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY_EQUIP");
		case Result::ITEM_ERROR_DECOMPOSE_PAID_ITEM: return TEXT("ITEM_ERROR_DECOMPOSE_PAID_ITEM");
		case Result::ITEM_ERROR_DECOMPOSE_INVALID_REWARD_ID: return TEXT("ITEM_ERROR_DECOMPOSE_INVALID_REWARD_ID");
		case Result::ITEM_ERROR_NPCSHOP_UNABLE_TO_SELL: return TEXT("ITEM_ERROR_NPCSHOP_UNABLE_TO_SELL");
		case Result::ITEM_ERROR_ADD_STORE_FULL: return TEXT("ITEM_ERROR_ADD_STORE_FULL");
		case Result::ITEM_ERROR_ADD_STORAGE_FULL: return TEXT("ITEM_ERROR_ADD_STORAGE_FULL");
		case Result::ITEM_ERROR_EXPIRED_DATE_TIME: return TEXT("ITEM_ERROR_EXPIRED_DATE_TIME");
		case Result::ITEM_ERROR_UNLOCK: return TEXT("ITEM_ERROR_UNLOCK");
		case Result::ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_TARGET_CATEGORY: return TEXT("ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_TARGET_CATEGORY");
		case Result::ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_LEVEL: return TEXT("ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_LEVEL");
		case Result::ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_SCROLL: return TEXT("ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_SCROLL");
		case Result::ITEM_ERROR_USABLE_IN_SAFE_ZONE: return TEXT("ITEM_ERROR_USABLE_IN_SAFE_ZONE");
		case Result::ITEM_ERROR_COMBAT_STATUS: return TEXT("ITEM_ERROR_COMBAT_STATUS");
		case Result::ITEM_ERROR_DELETE_LIST_EMPTY: return TEXT("ITEM_ERROR_DELETE_LIST_EMPTY");
		case Result::ITEM_ERROR_CREATE_LIST_EMPTY: return TEXT("ITEM_ERROR_CREATE_LIST_EMPTY");
		case Result::ITEM_ERROR_ABNORMALITY_STAT_ALREADY_IN_EFFECT: return TEXT("ITEM_ERROR_ABNORMALITY_STAT_ALREADY_IN_EFFECT");
		case Result::ITEM_ERROR_FUSE_DIFFERENT_ITEMS: return TEXT("ITEM_ERROR_FUSE_DIFFERENT_ITEMS");
		case Result::ITEM_ERROR_FUSE_CHARM_SAME_TEMPLATE_ID: return TEXT("ITEM_ERROR_FUSE_CHARM_SAME_TEMPLATE_ID");
		case Result::ITEM_ERROR_FUSE_DIFFERENT_EQUIP_GROUP_ID: return TEXT("ITEM_ERROR_FUSE_DIFFERENT_EQUIP_GROUP_ID");
		case Result::ITEM_ERROR_FUSE_NOT_EQUIP_ITEM: return TEXT("ITEM_ERROR_FUSE_NOT_EQUIP_ITEM");
		case Result::ITEM_ERROR_FUSE_INVALID_FUSE_ITEM: return TEXT("ITEM_ERROR_FUSE_INVALID_FUSE_ITEM");
		case Result::ITEM_ERROR_FUSE_LESS_THAN_MIN_LEVEL: return TEXT("ITEM_ERROR_FUSE_LESS_THAN_MIN_LEVEL");
		case Result::ITEM_ERROR_FUSE_NOT_SAME_ENCHANT_LEVEL: return TEXT("ITEM_ERROR_FUSE_NOT_SAME_ENCHANT_LEVEL");
		case Result::ITEM_ERROR_FUSE_REFINE_DATA_NOT_EXISTS: return TEXT("ITEM_ERROR_FUSE_REFINE_DATA_NOT_EXISTS");
		case Result::ITEM_ERROR_FUSE_SELECT_REFINE_DATA: return TEXT("ITEM_ERROR_FUSE_SELECT_REFINE_DATA");
		case Result::ITEM_ERROR_INVALID_USER_DEATH: return TEXT("ITEM_ERROR_INVALID_USER_DEATH");
		case Result::ITEM_ERROR_INVALID_MAGICAL_FORGE: return TEXT("ITEM_ERROR_INVALID_MAGICAL_FORGE");
		case Result::ITEM_ERROR_INVALID_MAGICAL_FORGE_MATIRAL_ITEM_EFFECT_TYPE: return TEXT("ITEM_ERROR_INVALID_MAGICAL_FORGE_MATIRAL_ITEM_EFFECT_TYPE");
		case Result::ITEM_ERROR_MAGICAL_FORGE_INVALID_TARGET: return TEXT("ITEM_ERROR_MAGICAL_FORGE_INVALID_TARGET");
		case Result::ITEM_ERROR_MAGICAL_FORGE_INVALID_MATARIAL: return TEXT("ITEM_ERROR_MAGICAL_FORGE_INVALID_MATARIAL");
		case Result::ITEM_ERROR_MAGICAL_FORGE_MAX_LEVEL: return TEXT("ITEM_ERROR_MAGICAL_FORGE_MAX_LEVEL");
		case Result::ITEM_ERROR_MAGICAL_FORGE_LOCKED: return TEXT("ITEM_ERROR_MAGICAL_FORGE_LOCKED");
		case Result::ITEM_ERROR_MAGICAL_FORGE_INVALID_GRADE: return TEXT("ITEM_ERROR_MAGICAL_FORGE_INVALID_GRADE");
		case Result::ITEM_ERROR_NOT_ALLOWED_MAP: return TEXT("ITEM_ERROR_NOT_ALLOWED_MAP");
		case Result::REWARD_ERROR_UNKNOWN: return TEXT("REWARD_ERROR_UNKNOWN");
		case Result::REWARD_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("REWARD_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::REWARD_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("REWARD_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::REWARD_ERROR_INVALID_REWARD: return TEXT("REWARD_ERROR_INVALID_REWARD");
		case Result::REWARD_ERROR_INVALID_REWARD_TYPE: return TEXT("REWARD_ERROR_INVALID_REWARD_TYPE");
		case Result::REWARD_ERROR_CURRENCY_EXCEEDED: return TEXT("REWARD_ERROR_CURRENCY_EXCEEDED");
		case Result::VEHICLE_ERROR_UNKNOWN: return TEXT("VEHICLE_ERROR_UNKNOWN");
		case Result::VEHICLE_ERROR_INVALID_ID: return TEXT("VEHICLE_ERROR_INVALID_ID");
		case Result::VEHICLE_ERROR_ALREADY_MOUNTED: return TEXT("VEHICLE_ERROR_ALREADY_MOUNTED");
		case Result::VEHICLE_ERROR_IN_TRANSFORM: return TEXT("VEHICLE_ERROR_IN_TRANSFORM");
		case Result::VEHICLE_ERROR_FORBIDDEN_AREA: return TEXT("VEHICLE_ERROR_FORBIDDEN_AREA");
		case Result::VEHICLE_ERROR_DISABLE_BEHAVIOR: return TEXT("VEHICLE_ERROR_DISABLE_BEHAVIOR");
		case Result::VEHICLE_ERROR_COMBAT_STATUS: return TEXT("VEHICLE_ERROR_COMBAT_STATUS");
		case Result::VEHICLE_ERROR_NOT_YET_MOVE_TIME: return TEXT("VEHICLE_ERROR_NOT_YET_MOVE_TIME");
		case Result::MAIL_ERROR_UNKNOWN: return TEXT("MAIL_ERROR_UNKNOWN");
		case Result::MAIL_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("MAIL_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::MAIL_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("MAIL_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::MAIL_ERROR_CURRENCY_EXCEEDED: return TEXT("MAIL_ERROR_CURRENCY_EXCEEDED");
		case Result::MAIL_ERROR_INVALID_MAIL_BOX_TYPE: return TEXT("MAIL_ERROR_INVALID_MAIL_BOX_TYPE");
		case Result::MAIL_ERROR_INVALID_CATEGORY_TYPE: return TEXT("MAIL_ERROR_INVALID_CATEGORY_TYPE");
		case Result::MAIL_ERROR_INVALID_MAIL_DB_ID: return TEXT("MAIL_ERROR_INVALID_MAIL_DB_ID");
		case Result::MAIL_ERROR_RECV_ALREADY_EXPIRED: return TEXT("MAIL_ERROR_RECV_ALREADY_EXPIRED");
		case Result::MAIL_ERROR_RECV_INVENTORY_FULL: return TEXT("MAIL_ERROR_RECV_INVENTORY_FULL");
		case Result::MAIL_ERROR_ALREADY_READ_MAIL: return TEXT("MAIL_ERROR_ALREADY_READ_MAIL");
		case Result::MAIL_ERROR_INVALID_CURRENCY_TYPE: return TEXT("MAIL_ERROR_INVALID_CURRENCY_TYPE");
		case Result::MAIL_ERROR_INVALID_MAIL_STRING_ID: return TEXT("MAIL_ERROR_INVALID_MAIL_STRING_ID");
		case Result::MAIL_ERROR_EMPTY_DELETED_MAIL_DB_ID_LIST: return TEXT("MAIL_ERROR_EMPTY_DELETED_MAIL_DB_ID_LIST");
		case Result::MAIL_ERROR_DB_OPERATION_MAIL_LIST_DELETE: return TEXT("MAIL_ERROR_DB_OPERATION_MAIL_LIST_DELETE");
		case Result::MAIL_ERROR_ALREADY_OHTER_SERVER_RECV: return TEXT("MAIL_ERROR_ALREADY_OHTER_SERVER_RECV");
		case Result::MAIL_ERROR_INVALID_MAIL_TYPE_ID: return TEXT("MAIL_ERROR_INVALID_MAIL_TYPE_ID");
		case Result::SKILL_ERROR_UNKNOWN: return TEXT("SKILL_ERROR_UNKNOWN");
		case Result::SKILL_ERROR_INVALID_POS: return TEXT("SKILL_ERROR_INVALID_POS");
		case Result::SKILL_ERROR_INVALID_INFO: return TEXT("SKILL_ERROR_INVALID_INFO");
		case Result::SKILL_ERROR_IN_COOL_DOWN: return TEXT("SKILL_ERROR_IN_COOL_DOWN");
		case Result::SKILL_ERROR_INVALID_STATE: return TEXT("SKILL_ERROR_INVALID_STATE");
		case Result::SKILL_ERROR_DISABLED_SKILL: return TEXT("SKILL_ERROR_DISABLED_SKILL");
		case Result::SKILL_ERROR_OTHER_SKILL_IN_PROGRESS: return TEXT("SKILL_ERROR_OTHER_SKILL_IN_PROGRESS");
		case Result::SKILL_ERROR_CANNOT_USE_PASSIVE: return TEXT("SKILL_ERROR_CANNOT_USE_PASSIVE");
		case Result::SKILL_ERROR_INVALID_PREV_SKILL_INFO: return TEXT("SKILL_ERROR_INVALID_PREV_SKILL_INFO");
		case Result::SKILL_ERROR_INVALID_CHAIN_CONDITION: return TEXT("SKILL_ERROR_INVALID_CHAIN_CONDITION");
		case Result::SKILL_ERROR_HP_LESS: return TEXT("SKILL_ERROR_HP_LESS");
		case Result::SKILL_ERROR_HP_GREATER: return TEXT("SKILL_ERROR_HP_GREATER");
		case Result::SKILL_ERROR_MP_LESS: return TEXT("SKILL_ERROR_MP_LESS");
		case Result::SKILL_ERROR_MP_GREATER: return TEXT("SKILL_ERROR_MP_GREATER");
		case Result::SKILL_ERROR_ONLY_USER: return TEXT("SKILL_ERROR_ONLY_USER");
		case Result::SKILL_ERROR_SP_LESS: return TEXT("SKILL_ERROR_SP_LESS");
		case Result::SKILL_ERROR_NOT_ALLOWED_CLASS: return TEXT("SKILL_ERROR_NOT_ALLOWED_CLASS");
		case Result::SKILL_ERROR_NOT_ALLOWED_TARGET: return TEXT("SKILL_ERROR_NOT_ALLOWED_TARGET");
		case Result::SKILL_ERROR_NO_REQUIRED_ITEM: return TEXT("SKILL_ERROR_NO_REQUIRED_ITEM");
		case Result::SKILL_ERROR_TARGET_NOT_IN_RANGE: return TEXT("SKILL_ERROR_TARGET_NOT_IN_RANGE");
		case Result::SKILL_ERROR_CURSOR_OUT_OF_RANGE: return TEXT("SKILL_ERROR_CURSOR_OUT_OF_RANGE");
		case Result::SKILL_ERROR_INVALID_SKILL_ID: return TEXT("SKILL_ERROR_INVALID_SKILL_ID");
		case Result::SKILL_ERROR_ENCHANT_MAX_SKILL_LEVEL: return TEXT("SKILL_ERROR_ENCHANT_MAX_SKILL_LEVEL");
		case Result::SKILL_ERROR_INVALID_COST_DATA: return TEXT("SKILL_ERROR_INVALID_COST_DATA");
		case Result::SKILL_ERROR_INVALID_CHAIN_SKILL_HIT_CONDITION: return TEXT("SKILL_ERROR_INVALID_CHAIN_SKILL_HIT_CONDITION");
		case Result::SKILL_ERROR_LEARN_NOT_EXIST_ROOT_SKILL: return TEXT("SKILL_ERROR_LEARN_NOT_EXIST_ROOT_SKILL");
		case Result::SKILL_ERROR_LEARN_INVALID_ESSENTIAL_PASSIVITY: return TEXT("SKILL_ERROR_LEARN_INVALID_ESSENTIAL_PASSIVITY");
		case Result::SKILL_ERROR_LEARN_ALREADY_LEARN_SKILL: return TEXT("SKILL_ERROR_LEARN_ALREADY_LEARN_SKILL");
		case Result::SKILL_ERROR_NOT_MATCH_TRIGGER_CONDITION: return TEXT("SKILL_ERROR_NOT_MATCH_TRIGGER_CONDITION");
		case Result::SKILL_ERROR_NOT_USABLE_IN_SAFE_ZONE: return TEXT("SKILL_ERROR_NOT_USABLE_IN_SAFE_ZONE");
		case Result::SKILL_ERROR_UNREACHABLE_STRAIGHT: return TEXT("SKILL_ERROR_UNREACHABLE_STRAIGHT");
		case Result::SKILL_ERROR_CHECK_COLLISION_FOR_SKILL: return TEXT("SKILL_ERROR_CHECK_COLLISION_FOR_SKILL");
		case Result::ABNORMALITY_ERROR_UNKNOWN: return TEXT("ABNORMALITY_ERROR_UNKNOWN");
		case Result::ABNORMALITY_ERROR_IGNORE_BY_PRIORITY: return TEXT("ABNORMALITY_ERROR_IGNORE_BY_PRIORITY");
		case Result::ABNORMALITY_ERROR_EXCEED_STACK_COUNT: return TEXT("ABNORMALITY_ERROR_EXCEED_STACK_COUNT");
		case Result::RESURRECT_RESULT_FAILED_NOT_SPAWNED: return TEXT("RESURRECT_RESULT_FAILED_NOT_SPAWNED");
		case Result::RESURRECT_RESULT_FAILED_ALIVE: return TEXT("RESURRECT_RESULT_FAILED_ALIVE");
		case Result::RESURRECT_RESULT_FAILED_NOT_PAST_MIN_TICK: return TEXT("RESURRECT_RESULT_FAILED_NOT_PAST_MIN_TICK");
		case Result::QUEST_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("QUEST_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::QUEST_ERROR_INVALID_STATUS: return TEXT("QUEST_ERROR_INVALID_STATUS");
		case Result::QUEST_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE: return TEXT("QUEST_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE");
		case Result::QUEST_ERROR_INVALID_DATA_SHEET: return TEXT("QUEST_ERROR_INVALID_DATA_SHEET");
		case Result::QUEST_ERROR_INVALID_ACCEPT_CHPATER_ID: return TEXT("QUEST_ERROR_INVALID_ACCEPT_CHPATER_ID");
		case Result::QUEST_ERROR_INVALID_ACCEPT_STORY_ID: return TEXT("QUEST_ERROR_INVALID_ACCEPT_STORY_ID");
		case Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_TYPE: return TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_TYPE");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_FIRST_QUEST: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_FIRST_QUEST");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_LAST_QUEST: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_LAST_QUEST");
		case Result::QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID: return TEXT("QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_REWARED: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_REWARED");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_STORY: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_STORY");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_QUEST: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_QUEST");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_PRE_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_PRE_EVENT_ACTION");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_POST_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_POST_EVENT_ACTION");
		case Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_ACTIVE_COUNT_MAX: return TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_ACTIVE_COUNT_MAX");
		case Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_REPEAT_RANDOM_PICK: return TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_REPEAT_RANDOM_PICK");
		case Result::QUEST_ERROR_INVALID_UPDATE_WARP_STATUS: return TEXT("QUEST_ERROR_INVALID_UPDATE_WARP_STATUS");
		case Result::QUEST_ERROR_INVALID_UPDATE_STATUS: return TEXT("QUEST_ERROR_INVALID_UPDATE_STATUS");
		case Result::QUEST_ERROR_INVALID_UPDATE_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_UPDATE_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_UPDATE_OBJECTIVE: return TEXT("QUEST_ERROR_INVALID_UPDATE_OBJECTIVE");
		case Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED: return TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED");
		case Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_TELEPORT: return TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_TELEPORT");
		case Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_INSTANCE_FIELD: return TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_INSTANCE_FIELD");
		case Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_LAST_PUBLIC_FIELD: return TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_LAST_PUBLIC_FIELD");
		case Result::QUEST_ERROR_INVALID_UPDATE_RANGE: return TEXT("QUEST_ERROR_INVALID_UPDATE_RANGE");
		case Result::QUEST_ERROR_INVALID_UPDATE_WARP_TO_INSTANCE_FIELD: return TEXT("QUEST_ERROR_INVALID_UPDATE_WARP_TO_INSTANCE_FIELD");
		case Result::QUEST_ERROR_INVALID_UPDATE_WARP_TO_PUBLIC_FIELD: return TEXT("QUEST_ERROR_INVALID_UPDATE_WARP_TO_PUBLIC_FIELD");
		case Result::QUEST_ERROR_INVALID_REWARD_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_REWARD_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_PROCESS_POST_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_PROCESS_POST_EVENT_ACTION");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_LAST_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_LAST_QUEST_ID");
		case Result::QUEST_ERROR_ALREADY_REWARD_QUEST_ID: return TEXT("QUEST_ERROR_ALREADY_REWARD_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_PRE_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_PRE_EVENT_ACTION");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_POST_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_POST_EVENT_ACTION");
		case Result::QUEST_ERROR_NOT_REWARDED: return TEXT("QUEST_ERROR_NOT_REWARDED");
		case Result::QUEST_ERROR_NOT_ENOUGH_LEVEL: return TEXT("QUEST_ERROR_NOT_ENOUGH_LEVEL");
		case Result::QUEST_ERROR_NOT_ENOUGH_MAIN_STORY_ID: return TEXT("QUEST_ERROR_NOT_ENOUGH_MAIN_STORY_ID");
		case Result::QUEST_ERROR_INVALID_WARP_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_WARP_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE: return TEXT("QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE");
		case Result::QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE_NOT_ACCEPTED: return TEXT("QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE_NOT_ACCEPTED");
		case Result::QUEST_ERROR_NOT_ENOUGH_MONEY: return TEXT("QUEST_ERROR_NOT_ENOUGH_MONEY");
		case Result::QUEST_ERROR_GIVE_UP_STATE: return TEXT("QUEST_ERROR_GIVE_UP_STATE");
		case Result::QUEST_ERROR_INVALID_REWARD_QUEST_TYPE: return TEXT("QUEST_ERROR_INVALID_REWARD_QUEST_TYPE");
		case Result::QUEST_ERROR_GATEWAY_SERVER_CONNECTION: return TEXT("QUEST_ERROR_GATEWAY_SERVER_CONNECTION");
		case Result::QUEST_ERROR_ALREADY_COMPLETE_QUEST_ID: return TEXT("QUEST_ERROR_ALREADY_COMPLETE_QUEST_ID");
		case Result::QUEST_ERROR_NOT_RPOGRESS_QUEST_ID: return TEXT("QUEST_ERROR_NOT_RPOGRESS_QUEST_ID");
		case Result::QUEST_ERROR_QUEST_ID: return TEXT("QUEST_ERROR_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_USER_DATA: return TEXT("QUEST_ERROR_INVALID_USER_DATA");
		case Result::QUEST_ERROR_NOT_ACCEPTABLE_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_ACCEPTABLE_GUILD_QUEST");
		case Result::QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_MONEY: return TEXT("QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_MONEY");
		case Result::QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_REFRESH_COUNT: return TEXT("QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_REFRESH_COUNT");
		case Result::QUEST_ERROR_QUEST_BOARD_NOT_QUEST_BOARD_NPC: return TEXT("QUEST_ERROR_QUEST_BOARD_NOT_QUEST_BOARD_NPC");
		case Result::QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_ACCEPT_COUNT: return TEXT("QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_ACCEPT_COUNT");
		case Result::QUEST_ERROR_QUEST_BOARD_INVALID_NPC: return TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_NPC");
		case Result::QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_ID: return TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_ID");
		case Result::QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_DATA: return TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_DATA");
		case Result::QUEST_ERROR_QUEST_BOARD_INVALID_TYPE: return TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_TYPE");
		case Result::QUEST_ERROR_INVALID_QUEST_INDEX: return TEXT("QUEST_ERROR_INVALID_QUEST_INDEX");
		case Result::QUEST_ERROR_NOT_SCROLL_QUEST: return TEXT("QUEST_ERROR_NOT_SCROLL_QUEST");
		case Result::QUEST_ERROR_NOT_REMAIN_REFRESH: return TEXT("QUEST_ERROR_NOT_REMAIN_REFRESH");
		case Result::QUEST_ERROR_INVALID_WARP_NOT_MAP_WORLD: return TEXT("QUEST_ERROR_INVALID_WARP_NOT_MAP_WORLD");
		case Result::QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID_NOT_POP_UP: return TEXT("QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID_NOT_POP_UP");
		case Result::WARP_ERROR_INVALID_STATUS: return TEXT("WARP_ERROR_INVALID_STATUS");
		case Result::WARP_ERROR_INVALID_VOLUME: return TEXT("WARP_ERROR_INVALID_VOLUME");
		case Result::WARP_ERROR_NOT_IN_VOLUME: return TEXT("WARP_ERROR_NOT_IN_VOLUME");
		case Result::WARP_ERROR_LEAVE_INSTANCE_SINGLE: return TEXT("WARP_ERROR_LEAVE_INSTANCE_SINGLE");
		case Result::WARP_ERROR_IN_COMBAT: return TEXT("WARP_ERROR_IN_COMBAT");
		case Result::WARP_ERROR_INVALID_SPOT_INFO: return TEXT("WARP_ERROR_INVALID_SPOT_INFO");
		case Result::WARP_ERROR_INVALID_MAP: return TEXT("WARP_ERROR_INVALID_MAP");
		case Result::WARP_ERROR_NOT_SPAWNED: return TEXT("WARP_ERROR_NOT_SPAWNED");
		case Result::WARP_ERROR_NOT_ALIVE: return TEXT("WARP_ERROR_NOT_ALIVE");
		case Result::WARP_ERROR_CAN_NOT_DO_ACTION_WARP: return TEXT("WARP_ERROR_CAN_NOT_DO_ACTION_WARP");
		case Result::WARP_ERROR_NOT_ENOUGH_DUNGEON_TIME: return TEXT("WARP_ERROR_NOT_ENOUGH_DUNGEON_TIME");
		case Result::WARP_ERROR_NOT_OPEN_DUNGEON_TIME: return TEXT("WARP_ERROR_NOT_OPEN_DUNGEON_TIME");
		case Result::WARP_ERROR_NOT_ENOUGH_DUNGEON_COST: return TEXT("WARP_ERROR_NOT_ENOUGH_DUNGEON_COST");
		case Result::WARP_ERROR_NEED_INSTANCE_SINGLE_MAP: return TEXT("WARP_ERROR_NEED_INSTANCE_SINGLE_MAP");
		case Result::WARP_ERROR_TOPO_MAP_NOT_EXIST: return TEXT("WARP_ERROR_TOPO_MAP_NOT_EXIST");
		case Result::WARP_ERROR_RANDOM_POS_FAILED: return TEXT("WARP_ERROR_RANDOM_POS_FAILED");
		case Result::WARP_ERROR_INVALID_MAP_ID: return TEXT("WARP_ERROR_INVALID_MAP_ID");
		case Result::WARP_ERROR_INVALID_CHANNEL_ID: return TEXT("WARP_ERROR_INVALID_CHANNEL_ID");
		case Result::WARP_ERROR_CHANNEL_CONGESTION_FULL: return TEXT("WARP_ERROR_CHANNEL_CONGESTION_FULL");
		case Result::WARP_ERROR_INVALID_MAIN_QUEST: return TEXT("WARP_ERROR_INVALID_MAIN_QUEST");
		case Result::WARP_ERROR_INVALID_LEVEL: return TEXT("WARP_ERROR_INVALID_LEVEL");
		case Result::WARP_ERROR_WAIT_ACK_DESPAWN_READYING: return TEXT("WARP_ERROR_WAIT_ACK_DESPAWN_READYING");
		case Result::WARP_ERROR_NOT_OPEN_INVADE_TIME: return TEXT("WARP_ERROR_NOT_OPEN_INVADE_TIME");
		case Result::WARP_ERROR_NOT_ENOUGH_INVADE_TIME: return TEXT("WARP_ERROR_NOT_ENOUGH_INVADE_TIME");
		case Result::WARP_ERROR_NOT_ENOUGH_INVADE_COST: return TEXT("WARP_ERROR_NOT_ENOUGH_INVADE_COST");
		case Result::WARP_ERROR_WORLD_BOSS_OPEN: return TEXT("WARP_ERROR_WORLD_BOSS_OPEN");
		case Result::WARP_ERROR_WORLD_BOSS_ENOUGH_COST: return TEXT("WARP_ERROR_WORLD_BOSS_ENOUGH_COST");
		case Result::WARP_ERROR_WORLD_BOSS_INVALID_STATUS: return TEXT("WARP_ERROR_WORLD_BOSS_INVALID_STATUS");
		case Result::WARP_ERROR_WORLD_BOSS_ALREADY_ENTER: return TEXT("WARP_ERROR_WORLD_BOSS_ALREADY_ENTER");
		case Result::WARP_ERROR_INVALID_SPAWN_GROUP_ID: return TEXT("WARP_ERROR_INVALID_SPAWN_GROUP_ID");
		case Result::WARP_ERROR_LEAVE_INSTANCE_SINGLE_DELAY_TIME: return TEXT("WARP_ERROR_LEAVE_INSTANCE_SINGLE_DELAY_TIME");
		case Result::WARP_ERROR_INVALID_NPC: return TEXT("WARP_ERROR_INVALID_NPC");
		case Result::WARP_ERROR_INVALID_NPC_IN_SPAWN_GROUP: return TEXT("WARP_ERROR_INVALID_NPC_IN_SPAWN_GROUP");
		case Result::WARP_ERROR_UNLOCK: return TEXT("WARP_ERROR_UNLOCK");
		case Result::WARP_ERROR_UNKNOWN: return TEXT("WARP_ERROR_UNKNOWN");
		case Result::PARTY_ERROR_UNKNOWN: return TEXT("PARTY_ERROR_UNKNOWN");
		case Result::PARTY_ERROR_INVALID_PARTY: return TEXT("PARTY_ERROR_INVALID_PARTY");
		case Result::PARTY_ERROR_ALREADY_PARTY_MEMBER: return TEXT("PARTY_ERROR_ALREADY_PARTY_MEMBER");
		case Result::PARTY_ERROR_ALREADY_PARTY_INVITED: return TEXT("PARTY_ERROR_ALREADY_PARTY_INVITED");
		case Result::PARTY_ERROR_NOT_JOINED: return TEXT("PARTY_ERROR_NOT_JOINED");
		case Result::PARTY_ERROR_NOT_LEADER: return TEXT("PARTY_ERROR_NOT_LEADER");
		case Result::PARTY_ERROR_NOT_MEMBER: return TEXT("PARTY_ERROR_NOT_MEMBER");
		case Result::PARTY_ERROR_NOT_FOUND_USER: return TEXT("PARTY_ERROR_NOT_FOUND_USER");
		case Result::PARTY_ERROR_NOT_LOGIN_USER: return TEXT("PARTY_ERROR_NOT_LOGIN_USER");
		case Result::PARTY_ERROR_MEMBER_FULL: return TEXT("PARTY_ERROR_MEMBER_FULL");
		case Result::PARTY_ERROR_NOT_INVITED: return TEXT("PARTY_ERROR_NOT_INVITED");
		case Result::PARTY_ERROR_CANNOT_INVITE_SELF: return TEXT("PARTY_ERROR_CANNOT_INVITE_SELF");
		case Result::PARTY_ERROR_CANNOT_KICK_SELF: return TEXT("PARTY_ERROR_CANNOT_KICK_SELF");
		case Result::PARTY_ERROR_ALREADY_LEADER: return TEXT("PARTY_ERROR_ALREADY_LEADER");
		case Result::PARTY_ERROR_INVALID_DROP_OWNERSHIP_TYPE: return TEXT("PARTY_ERROR_INVALID_DROP_OWNERSHIP_TYPE");
		case Result::PARTY_ERROR_WARP_TO_LEADER_MYSELF: return TEXT("PARTY_ERROR_WARP_TO_LEADER_MYSELF");
		case Result::PARTY_ERROR_INVALID_LEADER_MAP: return TEXT("PARTY_ERROR_INVALID_LEADER_MAP");
		case Result::PARTY_ERROR_INVALID_LEADER_MAP_SPWAN_SPOT: return TEXT("PARTY_ERROR_INVALID_LEADER_MAP_SPWAN_SPOT");
		case Result::PARTY_ERROR_LEADER_IN_INSTANCE_MAP: return TEXT("PARTY_ERROR_LEADER_IN_INSTANCE_MAP");
		case Result::PARTY_ERROR_NOT_ALL_READY: return TEXT("PARTY_ERROR_NOT_ALL_READY");
		case Result::PARTY_ERROR_USER_IN_INSTANCE_MAP: return TEXT("PARTY_ERROR_USER_IN_INSTANCE_MAP");
		case Result::PARTY_ERROR_SAME_PARTY: return TEXT("PARTY_ERROR_SAME_PARTY");
		case Result::PARTY_DUNGEON_ERROR_ALREADY_PARTY_MATCHING_WAIT: return TEXT("PARTY_DUNGEON_ERROR_ALREADY_PARTY_MATCHING_WAIT");
		case Result::PARTY_DUNGEON_ERROR_NOT_PARTY_MATCHING_WAIT: return TEXT("PARTY_DUNGEON_ERROR_NOT_PARTY_MATCHING_WAIT");
		case Result::PARTY_DUNGEON_ERROR_MATCHING_TIMEOUT: return TEXT("PARTY_DUNGEON_ERROR_MATCHING_TIMEOUT");
		case Result::PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY: return TEXT("PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY");
		case Result::PARTY_DUNGEON_ERROR_NOT_PLAYING: return TEXT("PARTY_DUNGEON_ERROR_NOT_PLAYING");
		case Result::PARTY_DUNGEON_ERROR_NOT_HELPCALL: return TEXT("PARTY_DUNGEON_ERROR_NOT_HELPCALL");
		case Result::PARTY_DUNGEON_ERROR_INVALID_MAP: return TEXT("PARTY_DUNGEON_ERROR_INVALID_MAP");
		case Result::PARTY_DUNGEON_ERROR_NO_REMAIN_HELP_COUNT: return TEXT("PARTY_DUNGEON_ERROR_NO_REMAIN_HELP_COUNT");
		case Result::PARTY_DUNGEON_ERROR_ALREADY_HELP_STATUS: return TEXT("PARTY_DUNGEON_ERROR_ALREADY_HELP_STATUS");
		case Result::PARTY_DUNGEON_ERROR_PLAYING: return TEXT("PARTY_DUNGEON_ERROR_PLAYING");
		case Result::PARTY_DUNGEON_ERROR_JUST_BEFORE_END: return TEXT("PARTY_DUNGEON_ERROR_JUST_BEFORE_END");
		case Result::PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY_MEMBER: return TEXT("PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY_MEMBER");
		case Result::PARTY_ERROR_NOT_INVITABLE_USER: return TEXT("PARTY_ERROR_NOT_INVITABLE_USER");
		case Result::PARTY_ERROR_NOT_INVITABLE_OTHER_SERVER_USER: return TEXT("PARTY_ERROR_NOT_INVITABLE_OTHER_SERVER_USER");
		case Result::PARTY_DUNGEON_ERROR_CANNOT_INVITE: return TEXT("PARTY_DUNGEON_ERROR_CANNOT_INVITE");
		case Result::PARTY_DUNGEON_ERROR_CANNOT_INTER_SERVER: return TEXT("PARTY_DUNGEON_ERROR_CANNOT_INTER_SERVER");
		case Result::PARTY_DUNGEON_ERROR_INVALID_AREA: return TEXT("PARTY_DUNGEON_ERROR_INVALID_AREA");
		case Result::PARTY_ERROR_INVALID_HOME_WORLD_ID: return TEXT("PARTY_ERROR_INVALID_HOME_WORLD_ID");
		case Result::PARTY_ERROR_CANNOT_INVITE_OTHER_ENTER_USER: return TEXT("PARTY_ERROR_CANNOT_INVITE_OTHER_ENTER_USER");
		case Result::CHANGE_WEAPON_TYPE_ERROR_UNKNOWN: return TEXT("CHANGE_WEAPON_TYPE_ERROR_UNKNOWN");
		case Result::CHANGE_WEAPON_TYPE_ERROR_INVALID_USER_STATE: return TEXT("CHANGE_WEAPON_TYPE_ERROR_INVALID_USER_STATE");
		case Result::CHANGE_WEAPON_TYPE_ERROR_INVALID_DURATION: return TEXT("CHANGE_WEAPON_TYPE_ERROR_INVALID_DURATION");
		case Result::CHANGE_WEAPON_TYPE_ERROR_IN_CHANGE_CLASS: return TEXT("CHANGE_WEAPON_TYPE_ERROR_IN_CHANGE_CLASS");
		case Result::CHANGE_WEAPON_TYPE_ERROR_COOLDOWN_TIME: return TEXT("CHANGE_WEAPON_TYPE_ERROR_COOLDOWN_TIME");
		case Result::PRESET_ERROR_COOLDOWN_TIME: return TEXT("PRESET_ERROR_COOLDOWN_TIME");
		case Result::EVENT_ACTION_ERROR_INVALID_STATUS: return TEXT("EVENT_ACTION_ERROR_INVALID_STATUS");
		case Result::EVENT_ACTION_ERROR_INVALID_MAP: return TEXT("EVENT_ACTION_ERROR_INVALID_MAP");
		case Result::EVENT_ACTION_ERROR_INVALID_SPOT: return TEXT("EVENT_ACTION_ERROR_INVALID_SPOT");
		case Result::EVENT_ACTION_ERROR_INVALID_REQUEST: return TEXT("EVENT_ACTION_ERROR_INVALID_REQUEST");
		case Result::EVENT_ACTION_ERROR_INVALID_MAIN_QUEST: return TEXT("EVENT_ACTION_ERROR_INVALID_MAIN_QUEST");
		case Result::EVENT_ACTION_ERROR_INVALID_QUESTID: return TEXT("EVENT_ACTION_ERROR_INVALID_QUESTID");
		case Result::EVENT_ACTION_ERROR_INVALID_POST_EVENT_ACTION_QUEST_NOT_COMPLETED: return TEXT("EVENT_ACTION_ERROR_INVALID_POST_EVENT_ACTION_QUEST_NOT_COMPLETED");
		case Result::EVENT_ACTION_ERROR_INVALID_INDEX: return TEXT("EVENT_ACTION_ERROR_INVALID_INDEX");
		case Result::EVENT_ACTION_ERROR_INVALID_EVENT_ACTION_GROUP_ID: return TEXT("EVENT_ACTION_ERROR_INVALID_EVENT_ACTION_GROUP_ID");
		case Result::EVENT_ACTION_ERROR_INVALID_PARAM: return TEXT("EVENT_ACTION_ERROR_INVALID_PARAM");
		case Result::EVENT_ACTION_ERROR_ALL_COMPLETED: return TEXT("EVENT_ACTION_ERROR_ALL_COMPLETED");
		case Result::EVENT_ACTION_ERROR_NOT_ALIVE: return TEXT("EVENT_ACTION_ERROR_NOT_ALIVE");
		case Result::EVENT_ACTION_ERROR_USER_WARP: return TEXT("EVENT_ACTION_ERROR_USER_WARP");
		case Result::TERRITORY_ERROR_INVALID_STATUS: return TEXT("TERRITORY_ERROR_INVALID_STATUS");
		case Result::TERRITORY_ERROR_INVALID_ID: return TEXT("TERRITORY_ERROR_INVALID_ID");
		case Result::TERRITORY_ERROR_INVALID_POSITION: return TEXT("TERRITORY_ERROR_INVALID_POSITION");
		case Result::DROP_OBJECT_LOOT_ERROR_UNKNOWN: return TEXT("DROP_OBJECT_LOOT_ERROR_UNKNOWN");
		case Result::DROP_OBJECT_LOOT_ERROR_INVALID_USER_STATE: return TEXT("DROP_OBJECT_LOOT_ERROR_INVALID_USER_STATE");
		case Result::DROP_OBJECT_LOOT_ERROR_INVALID_DROP_OBJECT: return TEXT("DROP_OBJECT_LOOT_ERROR_INVALID_DROP_OBJECT");
		case Result::DROP_OBJECT_LOOT_ERROR_INVALID_DISTANCE: return TEXT("DROP_OBJECT_LOOT_ERROR_INVALID_DISTANCE");
		case Result::DROP_OBJECT_LOOT_ERROR_ALREADY_IN_LOOT: return TEXT("DROP_OBJECT_LOOT_ERROR_ALREADY_IN_LOOT");
		case Result::DROP_OBJECT_LOOT_ERROR_NOT_OWNERSHIP: return TEXT("DROP_OBJECT_LOOT_ERROR_NOT_OWNERSHIP");
		case Result::DROP_OBJECT_LOOT_ERROR_NOT_MATCH_GAME_OPTION: return TEXT("DROP_OBJECT_LOOT_ERROR_NOT_MATCH_GAME_OPTION");
		case Result::DROP_OBJECT_LOOT_ERROR_NOT_MY_ITEM_PARTY_LOOT: return TEXT("DROP_OBJECT_LOOT_ERROR_NOT_MY_ITEM_PARTY_LOOT");
		case Result::DROP_OBJECT_LOOT_GUILD_STORE_OWNERSHIP: return TEXT("DROP_OBJECT_LOOT_GUILD_STORE_OWNERSHIP");
		case Result::DROP_OBJECT_LOOT_ERROR_TRY_AGAIN: return TEXT("DROP_OBJECT_LOOT_ERROR_TRY_AGAIN");
		case Result::BOOKMARK_ERROR_UNKNOWN: return TEXT("BOOKMARK_ERROR_UNKNOWN");
		case Result::BOOKMARK_ERROR_INVALID_STATUS: return TEXT("BOOKMARK_ERROR_INVALID_STATUS");
		case Result::BOOKMARK_ERROR_LIST_MAX: return TEXT("BOOKMARK_ERROR_LIST_MAX");
		case Result::BOOKMARK_ERROR_INVALID_MAP: return TEXT("BOOKMARK_ERROR_INVALID_MAP");
		case Result::BOOKMARK_ERROR_INVALID_TITLE_LEN: return TEXT("BOOKMARK_ERROR_INVALID_TITLE_LEN");
		case Result::BOOKMARK_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("BOOKMARK_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::BOOKMARK_ERROR_TELEPORT_INVALID_ITEM: return TEXT("BOOKMARK_ERROR_TELEPORT_INVALID_ITEM");
		case Result::BOOKMARK_ERROR_INVALID_SHARE_TYPE: return TEXT("BOOKMARK_ERROR_INVALID_SHARE_TYPE");
		case Result::BOOKMARK_ERROR_NOT_GUILD_MEMBER: return TEXT("BOOKMARK_ERROR_NOT_GUILD_MEMBER");
		case Result::BOOKMARK_ERROR_NO_SHARE_RIGHT: return TEXT("BOOKMARK_ERROR_NO_SHARE_RIGHT");
		case Result::BOOKMARK_ERROR_CANNOT_INTER_SERVER: return TEXT("BOOKMARK_ERROR_CANNOT_INTER_SERVER");
		case Result::BOOKMARK_ERROR_NOT_SHARE_TIME: return TEXT("BOOKMARK_ERROR_NOT_SHARE_TIME");
		case Result::BOOKMARK_ERROR_NO_PARTY_MEMBERS: return TEXT("BOOKMARK_ERROR_NO_PARTY_MEMBERS");
		case Result::BOOKMARK_ERROR_NO_GUILD_MEMBERS: return TEXT("BOOKMARK_ERROR_NO_GUILD_MEMBERS");
		case Result::BOOKMARK_ERROR_INVALID_TYPE: return TEXT("BOOKMARK_ERROR_INVALID_TYPE");
		case Result::BOOKMARK_ERROR_ALREADY_REGISTERED: return TEXT("BOOKMARK_ERROR_ALREADY_REGISTERED");
		case Result::BOOKMARK_ERROR_CANNOT_WARP_INTER_SERVER: return TEXT("BOOKMARK_ERROR_CANNOT_WARP_INTER_SERVER");
		case Result::INTERACT_ERROR_INVALID_STATE: return TEXT("INTERACT_ERROR_INVALID_STATE");
		case Result::INTERACT_ERROR_INVALID_NPC: return TEXT("INTERACT_ERROR_INVALID_NPC");
		case Result::INTERACT_ERROR_INVALID_NPC_RANGE: return TEXT("INTERACT_ERROR_INVALID_NPC_RANGE");
		case Result::INTERACT_ERROR_INVALID_PROP_START_USERCOUNT_OVER: return TEXT("INTERACT_ERROR_INVALID_PROP_START_USERCOUNT_OVER");
		case Result::INTERACT_ERROR_INVALID_PROP_MAX_USERCOUNT_LIMIT_OVER: return TEXT("INTERACT_ERROR_INVALID_PROP_MAX_USERCOUNT_LIMIT_OVER");
		case Result::INTERACT_ERROR_INVALID_PROP_START_ALREADY: return TEXT("INTERACT_ERROR_INVALID_PROP_START_ALREADY");
		case Result::INTERACT_ERROR_INVALID_PROP_NOT_ENDABLE: return TEXT("INTERACT_ERROR_INVALID_PROP_NOT_ENDABLE");
		case Result::INTERACT_ERROR_INVALID_PROP_STATUS_QUEST_NOT_ENDABLE: return TEXT("INTERACT_ERROR_INVALID_PROP_STATUS_QUEST_NOT_ENDABLE");
		case Result::INTERACT_ERROR_INVALID_PROP_STATUS_NOT_CANCEL_ABLE: return TEXT("INTERACT_ERROR_INVALID_PROP_STATUS_NOT_CANCEL_ABLE");
		case Result::INTERACT_ERROR_INVALID_PROP: return TEXT("INTERACT_ERROR_INVALID_PROP");
		case Result::INTERACT_ERROR_INVALID_QUEST_PROP: return TEXT("INTERACT_ERROR_INVALID_QUEST_PROP");
		case Result::INTERACT_ERROR_NOT_SPAWNED_PROP: return TEXT("INTERACT_ERROR_NOT_SPAWNED_PROP");
		case Result::INTERACT_ERROR_INVALID_PROP_RANGE: return TEXT("INTERACT_ERROR_INVALID_PROP_RANGE");
		case Result::INTERACT_ERROR_DEACTIVATE_EVENT_PRAYER: return TEXT("INTERACT_ERROR_DEACTIVATE_EVENT_PRAYER");
		case Result::INTERACT_ERROR_INVADE_PORTAL_PROP_REGISTRATION_ALREADY: return TEXT("INTERACT_ERROR_INVADE_PORTAL_PROP_REGISTRATION_ALREADY");
		case Result::INTERACT_ERROR_INVALID_PROP_TYPE: return TEXT("INTERACT_ERROR_INVALID_PROP_TYPE");
		case Result::INTERACT_ERROR_NOT_ALIVE: return TEXT("INTERACT_ERROR_NOT_ALIVE");
		case Result::BONUS_STAT_ERROR_UNKNOWN: return TEXT("BONUS_STAT_ERROR_UNKNOWN");
		case Result::BONUS_STAT_ERROR_INVALID_USER_STATUS: return TEXT("BONUS_STAT_ERROR_INVALID_USER_STATUS");
		case Result::BONUS_STAT_ERROR_INVALID_PKT_DATA: return TEXT("BONUS_STAT_ERROR_INVALID_PKT_DATA");
		case Result::BONUS_STAT_ERROR_INVALID_STAT_TYPE: return TEXT("BONUS_STAT_ERROR_INVALID_STAT_TYPE");
		case Result::BONUS_STAT_ERROR_DB_FAILED: return TEXT("BONUS_STAT_ERROR_DB_FAILED");
		case Result::BONUS_STAT_ERROR_DUPLICATE_STAT_TYPE: return TEXT("BONUS_STAT_ERROR_DUPLICATE_STAT_TYPE");
		case Result::BONUS_STAT_ERROR_NOT_ENOUGH_STAT_POINT: return TEXT("BONUS_STAT_ERROR_NOT_ENOUGH_STAT_POINT");
		case Result::BONUS_STAT_ERROR_MAX_STAT: return TEXT("BONUS_STAT_ERROR_MAX_STAT");
		case Result::SKILL_SLOT_ERROR_UNKNOWN: return TEXT("SKILL_SLOT_ERROR_UNKNOWN");
		case Result::SKILL_SLOT_ERROR_INVALID_USER_STATUS: return TEXT("SKILL_SLOT_ERROR_INVALID_USER_STATUS");
		case Result::SKILL_SLOT_ERROR_INVALID_PKT_DATA: return TEXT("SKILL_SLOT_ERROR_INVALID_PKT_DATA");
		case Result::SKILL_SLOT_ERROR_INVALID_SKILL_ID: return TEXT("SKILL_SLOT_ERROR_INVALID_SKILL_ID");
		case Result::SKILL_SLOT_ERROR_SAME_SLOT: return TEXT("SKILL_SLOT_ERROR_SAME_SLOT");
		case Result::SKILL_SLOT_ERROR_INVALID_SKILL_SLOT_ID: return TEXT("SKILL_SLOT_ERROR_INVALID_SKILL_SLOT_ID");
		case Result::SKILL_SLOT_ERROR_INVALID_WEAPON_TYPE: return TEXT("SKILL_SLOT_ERROR_INVALID_WEAPON_TYPE");
		case Result::SKILL_SLOT_ERROR_SAME_AUTO_FLAG: return TEXT("SKILL_SLOT_ERROR_SAME_AUTO_FLAG");
		case Result::SKILL_SLOT_ERROR_NOT_RESIST_SLOT: return TEXT("SKILL_SLOT_ERROR_NOT_RESIST_SLOT");
		case Result::SKILL_SLOT_ERROR_DB_FAILED: return TEXT("SKILL_SLOT_ERROR_DB_FAILED");
		case Result::SKILL_SLOT_ERROR_DUPLICATE_SKILL_GROUP: return TEXT("SKILL_SLOT_ERROR_DUPLICATE_SKILL_GROUP");
		case Result::SKILL_SLOT_ERROR_SAME_INTERVAL_VALUE: return TEXT("SKILL_SLOT_ERROR_SAME_INTERVAL_VALUE");
		case Result::SKILL_SLOT_ERROR_OVER_INTERVAL_SECOND: return TEXT("SKILL_SLOT_ERROR_OVER_INTERVAL_SECOND");
		case Result::USER_OPTIPON_ERROR_DB_FAILED: return TEXT("USER_OPTIPON_ERROR_DB_FAILED");
		case Result::PORTAL_WARP_ERROR_INVALID_STATUS: return TEXT("PORTAL_WARP_ERROR_INVALID_STATUS");
		case Result::PORTAL_WARP_ERROR_UNKNOWN: return TEXT("PORTAL_WARP_ERROR_UNKNOWN");
		case Result::PORTAL_WARP_ERROR_INVALID_SPOT_INFO: return TEXT("PORTAL_WARP_ERROR_INVALID_SPOT_INFO");
		case Result::QUICK_SLOT_ERROR_UNKNOWN: return TEXT("QUICK_SLOT_ERROR_UNKNOWN");
		case Result::QUICK_SLOT_ERROR_INVALID_USER_STATUS: return TEXT("QUICK_SLOT_ERROR_INVALID_USER_STATUS");
		case Result::QUICK_SLOT_ERROR_OVERFLOW_MAX_PAGE_SIZE: return TEXT("QUICK_SLOT_ERROR_OVERFLOW_MAX_PAGE_SIZE");
		case Result::QUICK_SLOT_ERROR_ITEM_NOT_EXIST_TARGET: return TEXT("QUICK_SLOT_ERROR_ITEM_NOT_EXIST_TARGET");
		case Result::QUICK_SLOT_ERROR_INVALID_QUICK_SLOT_ID: return TEXT("QUICK_SLOT_ERROR_INVALID_QUICK_SLOT_ID");
		case Result::QUICK_SLOT_ERROR_QUICK_SLOT_ID_DIFF_ITEM_DB_ID: return TEXT("QUICK_SLOT_ERROR_QUICK_SLOT_ID_DIFF_ITEM_DB_ID");
		case Result::QUICK_SLOT_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("QUICK_SLOT_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::QUICK_SLOT_ERROR_SET_DB_ERROR: return TEXT("QUICK_SLOT_ERROR_SET_DB_ERROR");
		case Result::QUICK_SLOT_ERROR_AUTO_DB_ERROR: return TEXT("QUICK_SLOT_ERROR_AUTO_DB_ERROR");
		case Result::QUICK_SLOT_ERROR_INVALID_ITEM_TYPE_AUTO: return TEXT("QUICK_SLOT_ERROR_INVALID_ITEM_TYPE_AUTO");
		case Result::FAIRY_ERROR_INVALID_STATUS: return TEXT("FAIRY_ERROR_INVALID_STATUS");
		case Result::FAIRY_ERROR_UNKNOWN: return TEXT("FAIRY_ERROR_UNKNOWN");
		case Result::FAIRY_ERROR_INVALID_FAIRY_ID: return TEXT("FAIRY_ERROR_INVALID_FAIRY_ID");
		case Result::FAIRY_ERROR_NOT_HAVE_FAIRY: return TEXT("FAIRY_ERROR_NOT_HAVE_FAIRY");
		case Result::FAIRY_ERROR_NOT_ENOUGH_FAIRY: return TEXT("FAIRY_ERROR_NOT_ENOUGH_FAIRY");
		case Result::FAIRY_ERROR_MAX_AMOUNT_OVER: return TEXT("FAIRY_ERROR_MAX_AMOUNT_OVER");
		case Result::FAIRY_ERROR_ALREADY_EQUIP_FAIRY: return TEXT("FAIRY_ERROR_ALREADY_EQUIP_FAIRY");
		case Result::FAIRY_ERROR_INVALID_DATA: return TEXT("FAIRY_ERROR_INVALID_DATA");
		case Result::FAIRY_ERROR_INVALID_PACKET_DATA: return TEXT("FAIRY_ERROR_INVALID_PACKET_DATA");
		case Result::FAIRY_SUMMON_ERROR_INVALID_FAIRY_GRADE: return TEXT("FAIRY_SUMMON_ERROR_INVALID_FAIRY_GRADE");
		case Result::FAIRY_SUMMON_ERROR_MAX_CONFIRM_COUNT: return TEXT("FAIRY_SUMMON_ERROR_MAX_CONFIRM_COUNT");
		case Result::FAIRY_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM: return TEXT("FAIRY_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM");
		case Result::FAIRY_CONFRIM_ERROR_INVALID_FAIRY_CONFIRM_ID: return TEXT("FAIRY_CONFRIM_ERROR_INVALID_FAIRY_CONFIRM_ID");
		case Result::FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT: return TEXT("FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT");
		case Result::FAIRY_CONFRIM_ERROR_EXPIRED: return TEXT("FAIRY_CONFRIM_ERROR_EXPIRED");
		case Result::FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::FAIRY_ENCHANT_ERROR_MAX_LEVEL_OVER: return TEXT("FAIRY_ENCHANT_ERROR_MAX_LEVEL_OVER");
		case Result::FAIRY_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL: return TEXT("FAIRY_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL");
		case Result::FAIRY_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("FAIRY_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::FAIRY_COMPOSE_ERROR_NOT_SAME_GRADE: return TEXT("FAIRY_COMPOSE_ERROR_NOT_SAME_GRADE");
		case Result::FAIRY_COMPOSE_ERROR_MAX_COMPOSE_COUNT: return TEXT("FAIRY_COMPOSE_ERROR_MAX_COMPOSE_COUNT");
		case Result::FAIRY_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT: return TEXT("FAIRY_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT");
		case Result::FAIRY_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("FAIRY_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::FAIRY_COMPOSE_ERROR_LEGEND_GRADE: return TEXT("FAIRY_COMPOSE_ERROR_LEGEND_GRADE");
		case Result::CHAT_ERROR_INVALID_CHAT_TYPE: return TEXT("CHAT_ERROR_INVALID_CHAT_TYPE");
		case Result::CHAT_ERROR_MESSAGE_LEN_ZERO: return TEXT("CHAT_ERROR_MESSAGE_LEN_ZERO");
		case Result::CHAT_ERROR_MESSAGE_LEN_OVER: return TEXT("CHAT_ERROR_MESSAGE_LEN_OVER");
		case Result::CHAT_ERROR_NOT_FOUND_USER: return TEXT("CHAT_ERROR_NOT_FOUND_USER");
		case Result::CHAT_ERROR_CAN_NOT_SEND_TO_ME: return TEXT("CHAT_ERROR_CAN_NOT_SEND_TO_ME");
		case Result::CHAT_ERROR_DETECTED_AS_SPAM: return TEXT("CHAT_ERROR_DETECTED_AS_SPAM");
		case Result::CHAT_ERROR_EXCEED_FREE_INVADE_CHAT: return TEXT("CHAT_ERROR_EXCEED_FREE_INVADE_CHAT");
		case Result::CHAT_ERROR_RESET_FREE_INVADE_CHAT: return TEXT("CHAT_ERROR_RESET_FREE_INVADE_CHAT");
		case Result::CHAT_ERROR_USER_BANNED: return TEXT("CHAT_ERROR_USER_BANNED");
		case Result::CHAT_ERROR_SERVER_CONNECTION: return TEXT("CHAT_ERROR_SERVER_CONNECTION");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_TYPE: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_TYPE");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_AMOUNT: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_AMOUNT");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_NOT_ENOUGH_CURRENCY: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_NOT_ENOUGH_CURRENCY");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_NO_USER_IN_WORLD: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_NO_USER_IN_WORLD");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_FAIL_REQ_PAY: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_FAIL_REQ_PAY");
		case Result::CHAT_ERROR_INVADE_CHAT_LEVEL: return TEXT("CHAT_ERROR_INVADE_CHAT_LEVEL");
		case Result::CHAT_ERROR_INVADE_CHAT_DB_UPDATE_ERROR: return TEXT("CHAT_ERROR_INVADE_CHAT_DB_UPDATE_ERROR");
		case Result::CHAT_ERROR_INTER_CHAT_NOT_INTER_SERVER_USER: return TEXT("CHAT_ERROR_INTER_CHAT_NOT_INTER_SERVER_USER");
		case Result::CHAT_ERROR_INTER_CHAT_NO_INTER_SERVER_SESSION: return TEXT("CHAT_ERROR_INTER_CHAT_NO_INTER_SERVER_SESSION");
		case Result::RESURRECT_ERROR_INVALID_RESURRECT_ID: return TEXT("RESURRECT_ERROR_INVALID_RESURRECT_ID");
		case Result::RESURRECT_ERROR_COST_VALUE: return TEXT("RESURRECT_ERROR_COST_VALUE");
		case Result::RESURRECT_ERROR_INVASIONOUT: return TEXT("RESURRECT_ERROR_INVASIONOUT");
		case Result::DEATH_ERROR_INVALID_ID: return TEXT("DEATH_ERROR_INVALID_ID");
		case Result::DEATH_ERROR_RECOVERY_GOLD_COST: return TEXT("DEATH_ERROR_RECOVERY_GOLD_COST");
		case Result::DEATH_ERROR_RECOVERY_DIA_COST: return TEXT("DEATH_ERROR_RECOVERY_DIA_COST");
		case Result::DEATH_ERRORR_RECOVERY_ITEM_COST: return TEXT("DEATH_ERRORR_RECOVERY_ITEM_COST");
		case Result::DEATH_ERROR_RECOVERY_TIME_OUT: return TEXT("DEATH_ERROR_RECOVERY_TIME_OUT");
		case Result::DEATH_ERROR_RECOVERY_COUNT: return TEXT("DEATH_ERROR_RECOVERY_COUNT");
		case Result::GUILD_CREATE_ERROR_INVALID_NAME_LENGTH: return TEXT("GUILD_CREATE_ERROR_INVALID_NAME_LENGTH");
		case Result::GUILD_CREATE_ERROR_INVALID_DESC_LENGTH: return TEXT("GUILD_CREATE_ERROR_INVALID_DESC_LENGTH");
		case Result::GUILD_CREATE_ERROR_NOT_ALLOWED_NAME: return TEXT("GUILD_CREATE_ERROR_NOT_ALLOWED_NAME");
		case Result::GUILD_CREATE_ERROR_DUPLICATION_NAME: return TEXT("GUILD_CREATE_ERROR_DUPLICATION_NAME");
		case Result::GUILD_CREATE_ERROR_HAD_GUILD: return TEXT("GUILD_CREATE_ERROR_HAD_GUILD");
		case Result::GUILD_CREATE_ERROR_NO_GOLD: return TEXT("GUILD_CREATE_ERROR_NO_GOLD");
		case Result::GUILD_ERROR_HAD_GUILD: return TEXT("GUILD_ERROR_HAD_GUILD");
		case Result::GUILD_ERROR_DB: return TEXT("GUILD_ERROR_DB");
		case Result::GUILD_ERROR_UNKNOWN: return TEXT("GUILD_ERROR_UNKNOWN");
		case Result::GUILD_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("GUILD_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::GUILD_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("GUILD_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::GUILD_DELETE_ERROR_MEMBER_CNT: return TEXT("GUILD_DELETE_ERROR_MEMBER_CNT");
		case Result::GUILD_ERROR_NOT_EXSIT_MEMEBER: return TEXT("GUILD_ERROR_NOT_EXSIT_MEMEBER");
		case Result::GUILD_ERROR_NOT_REQUEST_JOIN: return TEXT("GUILD_ERROR_NOT_REQUEST_JOIN");
		case Result::GUILD_JOIN_ERROR_FULL: return TEXT("GUILD_JOIN_ERROR_FULL");
		case Result::GUILD_JOIN_ERROR_COOLDOWN_TIME: return TEXT("GUILD_JOIN_ERROR_COOLDOWN_TIME");
		case Result::GUILD_JOIN_ERROR_INVALID_GUILD: return TEXT("GUILD_JOIN_ERROR_INVALID_GUILD");
		case Result::GUILD_JOIN_ERROR_USER_LEVEL: return TEXT("GUILD_JOIN_ERROR_USER_LEVEL");
		case Result::GUILD_LEAVE_ERROR_CAPTAIN: return TEXT("GUILD_LEAVE_ERROR_CAPTAIN");
		case Result::GUILD_ERROR_FULL_GRADE_NUM: return TEXT("GUILD_ERROR_FULL_GRADE_NUM");
		case Result::GUILD_DELETE_ERROR_GRADE: return TEXT("GUILD_DELETE_ERROR_GRADE");
		case Result::GUILD_ERROR_GRADE: return TEXT("GUILD_ERROR_GRADE");
		case Result::GUILD_ERROR_NOT_FIND: return TEXT("GUILD_ERROR_NOT_FIND");
		case Result::GUILD_ERROR_DAILY_DONATE_OVER: return TEXT("GUILD_ERROR_DAILY_DONATE_OVER");
		case Result::GUILD_ERROR_ENOUGH_CURRENCY: return TEXT("GUILD_ERROR_ENOUGH_CURRENCY");
		case Result::GUILD_ERROR_GUILD_COIN_FULLY: return TEXT("GUILD_ERROR_GUILD_COIN_FULLY");
		case Result::GUILD_ERROR_NOT_ENOUGH_GUILD_ASSET: return TEXT("GUILD_ERROR_NOT_ENOUGH_GUILD_ASSET");
		case Result::GUILD_ERROR_ATTENDANCE_ALREADY: return TEXT("GUILD_ERROR_ATTENDANCE_ALREADY");
		case Result::GUILD_GRADE_ERROR_INAVLID_MEMBER: return TEXT("GUILD_GRADE_ERROR_INAVLID_MEMBER");
		case Result::GUILD_GRADE_ERROR_DO_NOT_CHANGE_GRADE_LEVEL: return TEXT("GUILD_GRADE_ERROR_DO_NOT_CHANGE_GRADE_LEVEL");
		case Result::GUILD_ERROR_BUILDING_INAVLID_ID: return TEXT("GUILD_ERROR_BUILDING_INAVLID_ID");
		case Result::GUILD_ERROR_BUILDING_DB: return TEXT("GUILD_ERROR_BUILDING_DB");
		case Result::GUILD_ERROR_BUILDING_AUTH: return TEXT("GUILD_ERROR_BUILDING_AUTH");
		case Result::GUILD_ERROR_BUILDING_OPEN: return TEXT("GUILD_ERROR_BUILDING_OPEN");
		case Result::GUILD_ERROR_BUILDING_UPGRADE: return TEXT("GUILD_ERROR_BUILDING_UPGRADE");
		case Result::GUILD_ERROR_BUILDING_DATE: return TEXT("GUILD_ERROR_BUILDING_DATE");
		case Result::GUILD_ERROR_BUILDING_LEVEL: return TEXT("GUILD_ERROR_BUILDING_LEVEL");
		case Result::GUILD_ERROR_GUILD_CAMP_LEVEL: return TEXT("GUILD_ERROR_GUILD_CAMP_LEVEL");
		case Result::GUILD_ERROR_BUILDING_MAX_LEVEL: return TEXT("GUILD_ERROR_BUILDING_MAX_LEVEL");
		case Result::GUILD_ERROR_BUILDING_ASSET: return TEXT("GUILD_ERROR_BUILDING_ASSET");
		case Result::GUILD_ERROR_BUILDING_PASSIVITY: return TEXT("GUILD_ERROR_BUILDING_PASSIVITY");
		case Result::GUILD_ERROR_BUILDING_UNAUTHORIZED_LEVELUP: return TEXT("GUILD_ERROR_BUILDING_UNAUTHORIZED_LEVELUP");
		case Result::GUILD_ERROR_UNABLE_GUILD_LEVEL: return TEXT("GUILD_ERROR_UNABLE_GUILD_LEVEL");
		case Result::GUILD_BUY_SHOP_ERROR_UNABLE_GUILD_LEVEL: return TEXT("GUILD_BUY_SHOP_ERROR_UNABLE_GUILD_LEVEL");
		case Result::GUILD_BUY_SHOP_INVALID_PRODUCT_ID: return TEXT("GUILD_BUY_SHOP_INVALID_PRODUCT_ID");
		case Result::GUILD_BUY_SHOP_ERROR_OVER_CNT: return TEXT("GUILD_BUY_SHOP_ERROR_OVER_CNT");
		case Result::GUILD_SHOP_ERROR_ENOUGH_CURRENCY: return TEXT("GUILD_SHOP_ERROR_ENOUGH_CURRENCY");
		case Result::GUILD_ERROR_CHANGE_MEMBER_GRADE: return TEXT("GUILD_ERROR_CHANGE_MEMBER_GRADE");
		case Result::GUILD_JOIN_ERROR_FULL_OF_APPLICANTS: return TEXT("GUILD_JOIN_ERROR_FULL_OF_APPLICANTS");
		case Result::GUILD_JOIN_ERROR_OVER_REQUEST_CNT: return TEXT("GUILD_JOIN_ERROR_OVER_REQUEST_CNT");
		case Result::GUILD_ACTIVITY_ERROR_GUILD_JOIN: return TEXT("GUILD_ACTIVITY_ERROR_GUILD_JOIN");
		case Result::GUILD_ERROR_EXIST_HOSTILE_GUILD: return TEXT("GUILD_ERROR_EXIST_HOSTILE_GUILD");
		case Result::GUILD_ERROR_INVALID_USER: return TEXT("GUILD_ERROR_INVALID_USER");
		case Result::GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID: return TEXT("GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID");
		case Result::GUILD_ERROR_ALLY_GUILD_ID: return TEXT("GUILD_ERROR_ALLY_GUILD_ID");
		case Result::GUILD_ERROR_ALLY_REQUEST_GUILD_ID: return TEXT("GUILD_ERROR_ALLY_REQUEST_GUILD_ID");
		case Result::GUILD_ERROR_UNKNOWN_GUILD_ID: return TEXT("GUILD_ERROR_UNKNOWN_GUILD_ID");
		case Result::GUILD_ERROR_MAX_REQUESTED: return TEXT("GUILD_ERROR_MAX_REQUESTED");
		case Result::GUILD_JOIN_ERROR_UNABLE: return TEXT("GUILD_JOIN_ERROR_UNABLE");
		case Result::GUILD_JOIN_ERROR_EXIST_LIST: return TEXT("GUILD_JOIN_ERROR_EXIST_LIST");
		case Result::GUILD_ERROR_NOT_JOIN: return TEXT("GUILD_ERROR_NOT_JOIN");
		case Result::GUILD_ERROR_INVALID_GUILD_CHAT_ID: return TEXT("GUILD_ERROR_INVALID_GUILD_CHAT_ID");
		case Result::GUILD_SEARCH_ERROR_COOLDOWN: return TEXT("GUILD_SEARCH_ERROR_COOLDOWN");
		case Result::GUILD_ERROR_GUILD_ASSET_FULLY: return TEXT("GUILD_ERROR_GUILD_ASSET_FULLY");
		case Result::GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_HOSTILE_GUILD: return TEXT("GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_HOSTILE_GUILD");
		case Result::GUILD_SEARCH_DILPLOMACY_ERROR_REQUEST_ALLY_GUILD: return TEXT("GUILD_SEARCH_DILPLOMACY_ERROR_REQUEST_ALLY_GUILD");
		case Result::GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_ALLY_GUILD: return TEXT("GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_ALLY_GUILD");
		case Result::GUILD_SEARCH_ERROR_MATCHING: return TEXT("GUILD_SEARCH_ERROR_MATCHING");
		case Result::GUILD_ERROR_EXIST_ALLY_GUILD: return TEXT("GUILD_ERROR_EXIST_ALLY_GUILD");
		case Result::GUILD_ERROR_EXIST_ALLY_REQUEST_GUILD: return TEXT("GUILD_ERROR_EXIST_ALLY_REQUEST_GUILD");
		case Result::GUILD_ERROR_SAME_GUILD_ID: return TEXT("GUILD_ERROR_SAME_GUILD_ID");
		case Result::GUILD_ERROR_UNAUTHORIZED_DIPLOMACY: return TEXT("GUILD_ERROR_UNAUTHORIZED_DIPLOMACY");
		case Result::GUILD_ERROR_HOSTILE_GUILD_ID: return TEXT("GUILD_ERROR_HOSTILE_GUILD_ID");
		case Result::GUILD_ERROR_ERROR_FULL: return TEXT("GUILD_ERROR_ERROR_FULL");
		case Result::GUILD_ERROR_OPPONENT_FULL: return TEXT("GUILD_ERROR_OPPONENT_FULL");
		case Result::GUILD_ERROR_BUILDING_REACTIVATE_TIME: return TEXT("GUILD_ERROR_BUILDING_REACTIVATE_TIME");
		case Result::GUILD_GIFT_ERROR_INVALID_ITEM_EFFECT_PARAM: return TEXT("GUILD_GIFT_ERROR_INVALID_ITEM_EFFECT_PARAM");
		case Result::GUILD_CREATE_ERROR_INSERT_GUILD: return TEXT("GUILD_CREATE_ERROR_INSERT_GUILD");
		case Result::GUILD_CREATE_ERROR_INSERT_GUILD_ROW_COUNT: return TEXT("GUILD_CREATE_ERROR_INSERT_GUILD_ROW_COUNT");
		case Result::GUILD_CREATE_ERROR_INSERT_GUILD_MEMBER: return TEXT("GUILD_CREATE_ERROR_INSERT_GUILD_MEMBER");
		case Result::GUILD_CREATE_ERROR_DELETE_GUILD_REQUEST_JOIN: return TEXT("GUILD_CREATE_ERROR_DELETE_GUILD_REQUEST_JOIN");
		case Result::GUILD_DUNGEON_WARP_ERROR_NOT_ENOUGH_TICKET: return TEXT("GUILD_DUNGEON_WARP_ERROR_NOT_ENOUGH_TICKET");
		case Result::GUILD_DUNGEON_WARP_ERROR_IN_GUILD_DUNGEON_MAP: return TEXT("GUILD_DUNGEON_WARP_ERROR_IN_GUILD_DUNGEON_MAP");
		case Result::GUILD_DUNGEON_WARP_ERROR_STORE_FULL: return TEXT("GUILD_DUNGEON_WARP_ERROR_STORE_FULL");
		case Result::GUILD_DUNGEON_WARP_ERROR_DONT_DESTROY: return TEXT("GUILD_DUNGEON_WARP_ERROR_DONT_DESTROY");
		case Result::GUILD_DUNGEON_WARP_ERROR_LEVEL: return TEXT("GUILD_DUNGEON_WARP_ERROR_LEVEL");
		case Result::GUILD_DUNGEON_WARP_ERROR_ABNORMALITY_EFFECT: return TEXT("GUILD_DUNGEON_WARP_ERROR_ABNORMALITY_EFFECT");
		case Result::GUILD_DUNGEON_WATCHING_ERROR_MAP: return TEXT("GUILD_DUNGEON_WATCHING_ERROR_MAP");
		case Result::GUILD_DUNGEON_WATCHING_NO_ONE_HERE: return TEXT("GUILD_DUNGEON_WATCHING_NO_ONE_HERE");
		case Result::GUILD_DUNGEON_RESET_TIME: return TEXT("GUILD_DUNGEON_RESET_TIME");
		case Result::GUILD_DUNGEON_WARP_ERROR_ENOUGH_GUILD_LEVEL: return TEXT("GUILD_DUNGEON_WARP_ERROR_ENOUGH_GUILD_LEVEL");
		case Result::GUILD_DUNGEON_WARP_ERROR_THE_DAY_JOINED_GUILD: return TEXT("GUILD_DUNGEON_WARP_ERROR_THE_DAY_JOINED_GUILD");
		case Result::GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER: return TEXT("GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER");
		case Result::GUILD_DUNGEON_WARP_ERROR_ANOTHER_GUILD_LEVEL: return TEXT("GUILD_DUNGEON_WARP_ERROR_ANOTHER_GUILD_LEVEL");
		case Result::GUILD_ERROR_INTER_ALLY_PREPARING: return TEXT("GUILD_ERROR_INTER_ALLY_PREPARING");
		case Result::GUILD_ERROR_INTER_ALLY_REQUEST_UNKNOWN_GUILD_ID: return TEXT("GUILD_ERROR_INTER_ALLY_REQUEST_UNKNOWN_GUILD_ID");
		case Result::GUILD_ERROR_INTER_ALLY_REQUEST_ERROR_DB: return TEXT("GUILD_ERROR_INTER_ALLY_REQUEST_ERROR_DB");
		case Result::GUILD_ERROR_INTER_ALLY_ACCEPT_UNKNOWN_GUILD_ID: return TEXT("GUILD_ERROR_INTER_ALLY_ACCEPT_UNKNOWN_GUILD_ID");
		case Result::GUILD_ERROR_INTER_ALLY_ACCEPT_ERROR_DB: return TEXT("GUILD_ERROR_INTER_ALLY_ACCEPT_ERROR_DB");
		case Result::GUILD_LEAVE_ERROR_CANNOT_STATE: return TEXT("GUILD_LEAVE_ERROR_CANNOT_STATE");
		case Result::AGIT_WARP_ERROR_IN_AGIT_MAP: return TEXT("AGIT_WARP_ERROR_IN_AGIT_MAP");
		case Result::AGIT_WARP_ERROR_EXPIRE: return TEXT("AGIT_WARP_ERROR_EXPIRE");
		case Result::AGIT_WARP_ERROR_NOT_AGIT_MAP: return TEXT("AGIT_WARP_ERROR_NOT_AGIT_MAP");
		case Result::AGIT_INVALID_USER: return TEXT("AGIT_INVALID_USER");
		case Result::AGIT_ERROR_NO_MORE_RENTAL_EXPANSION: return TEXT("AGIT_ERROR_NO_MORE_RENTAL_EXPANSION");
		case Result::AGIT_INVALID_RENTAL: return TEXT("AGIT_INVALID_RENTAL");
		case Result::AGIT_ERROR_NOT_RENTAING: return TEXT("AGIT_ERROR_NOT_RENTAING");
		case Result::ITEM_ERROR_REFINE_INVALID_TARGET: return TEXT("ITEM_ERROR_REFINE_INVALID_TARGET");
		case Result::ITEM_ERROR_REFINE_INVALID_STAT_DATA: return TEXT("ITEM_ERROR_REFINE_INVALID_STAT_DATA");
		case Result::ITEM_ERROR_REFINE_OPTION_STONE_NOT_MEET_THE_CONDITION: return TEXT("ITEM_ERROR_REFINE_OPTION_STONE_NOT_MEET_THE_CONDITION");
		case Result::ITEM_ERROR_REFINE_LOCKED: return TEXT("ITEM_ERROR_REFINE_LOCKED");
		case Result::ARCAS_ERROR_INVALID_STATUS: return TEXT("ARCAS_ERROR_INVALID_STATUS");
		case Result::ARCAS_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("ARCAS_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::ARCAS_ERROR_MEDITATION: return TEXT("ARCAS_ERROR_MEDITATION");
		case Result::ARCAS_ERROR_NOT_MEDITATION: return TEXT("ARCAS_ERROR_NOT_MEDITATION");
		case Result::ARCAS_ERROR_NOT_REMOVE_ARCAS_BUFF: return TEXT("ARCAS_ERROR_NOT_REMOVE_ARCAS_BUFF");
		case Result::ARCAS_ERROR_ARCAS_BUFF_ON: return TEXT("ARCAS_ERROR_ARCAS_BUFF_ON");
		case Result::ARCAS_ERROR_NOT_USE_ARCAS_POINT_GAIN: return TEXT("ARCAS_ERROR_NOT_USE_ARCAS_POINT_GAIN");
		case Result::ARCAS_ERROR_INVALID_ARCAS_POINT_DATA: return TEXT("ARCAS_ERROR_INVALID_ARCAS_POINT_DATA");
		case Result::ARCAS_ERROR_NOT_ENOUGH_ARCAS_POINT: return TEXT("ARCAS_ERROR_NOT_ENOUGH_ARCAS_POINT");
		case Result::ARCAS_ERROR_INVALID_ARCAS_POINT_AND_NOT_EXIST_FAIRY_BUFF_ABNORMALITY: return TEXT("ARCAS_ERROR_INVALID_ARCAS_POINT_AND_NOT_EXIST_FAIRY_BUFF_ABNORMALITY");
		case Result::ARCAS_ERROR_INVALID_PASSIVITY_ID: return TEXT("ARCAS_ERROR_INVALID_PASSIVITY_ID");
		case Result::ARCAS_ERROR_NOT_ITEM_USE_MAX_ARCAS_POINT: return TEXT("ARCAS_ERROR_NOT_ITEM_USE_MAX_ARCAS_POINT");
		case Result::USER_COMMON_ERROR_INVALID_STAT_POINT_DATA: return TEXT("USER_COMMON_ERROR_INVALID_STAT_POINT_DATA");
		case Result::USER_COMMON_ERROR_INVALID_USER: return TEXT("USER_COMMON_ERROR_INVALID_USER");
		case Result::COSTUME_ERROR_UNKNOWN: return TEXT("COSTUME_ERROR_UNKNOWN");
		case Result::COSTUME_ERROR_INVALID_STATUS: return TEXT("COSTUME_ERROR_INVALID_STATUS");
		case Result::COSTUME_ERROR_INVALID_COSTUME_ID: return TEXT("COSTUME_ERROR_INVALID_COSTUME_ID");
		case Result::COSTUME_ERROR_NOT_HAVE_COSTUME: return TEXT("COSTUME_ERROR_NOT_HAVE_COSTUME");
		case Result::COSTUME_ERROR_ALREADY_EQUIP_COSTUME: return TEXT("COSTUME_ERROR_ALREADY_EQUIP_COSTUME");
		case Result::COSTUME_ERROR_NOT_ENOUGH_COSTUME: return TEXT("COSTUME_ERROR_NOT_ENOUGH_COSTUME");
		case Result::COSTUME_ERROR_MAX_AMOUNT_OVER: return TEXT("COSTUME_ERROR_MAX_AMOUNT_OVER");
		case Result::COSTUME_ERROR_INVALID_DATA: return TEXT("COSTUME_ERROR_INVALID_DATA");
		case Result::COSTUME_ERROR_INVALID_PACKET_DATA: return TEXT("COSTUME_ERROR_INVALID_PACKET_DATA");
		case Result::COSTUME_SUMMON_ERROR_INVALID_COSTUME_GRADE: return TEXT("COSTUME_SUMMON_ERROR_INVALID_COSTUME_GRADE");
		case Result::COSTUME_SUMMON_ERROR_MAX_CONFIRM_COUNT: return TEXT("COSTUME_SUMMON_ERROR_MAX_CONFIRM_COUNT");
		case Result::COSTUME_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM: return TEXT("COSTUME_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM");
		case Result::COSTUME_CONFRIM_ERROR_INVALID_COSTUME_CONFIRM_ID: return TEXT("COSTUME_CONFRIM_ERROR_INVALID_COSTUME_CONFIRM_ID");
		case Result::COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT: return TEXT("COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT");
		case Result::COSTUME_CONFRIM_ERROR_EXPIRED: return TEXT("COSTUME_CONFRIM_ERROR_EXPIRED");
		case Result::COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::COSTUME_ENCHANT_ERROR_MAX_LEVEL_OVER: return TEXT("COSTUME_ENCHANT_ERROR_MAX_LEVEL_OVER");
		case Result::COSTUME_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL: return TEXT("COSTUME_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL");
		case Result::COSTUME_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("COSTUME_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::COSTUME_COMPOSE_ERROR_NOT_SAME_GRADE: return TEXT("COSTUME_COMPOSE_ERROR_NOT_SAME_GRADE");
		case Result::COSTUME_COMPOSE_ERROR_MAX_COMPOSE_COUNT: return TEXT("COSTUME_COMPOSE_ERROR_MAX_COMPOSE_COUNT");
		case Result::COSTUME_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT: return TEXT("COSTUME_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT");
		case Result::COSTUME_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("COSTUME_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::COSTUME_COMPOSE_ERROR_LEGEND_GRADE: return TEXT("COSTUME_COMPOSE_ERROR_LEGEND_GRADE");
		case Result::COSTUME_SKILL_ERROR_NOT_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_NOT_COSTUME_SKILL");
		case Result::COSTUME_SKILL_ERROR_NOT_COSTUME_SKILl_ID: return TEXT("COSTUME_SKILL_ERROR_NOT_COSTUME_SKILl_ID");
		case Result::COSTUME_SKILL_ERROR_ALREADY_LEARNED_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_ALREADY_LEARNED_COSTUME_SKILL");
		case Result::COSTUME_SKILL_ERROR_NOT_LEARNED_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_NOT_LEARNED_COSTUME_SKILL");
		case Result::COSTUME_SKILL_ERROR_INVALID_SKILL_ID: return TEXT("COSTUME_SKILL_ERROR_INVALID_SKILL_ID");
		case Result::COSTUME_SKILL_ERROR_FAIL_TO_SET_SKILL_SLOT: return TEXT("COSTUME_SKILL_ERROR_FAIL_TO_SET_SKILL_SLOT");
		case Result::COSTUME_SKILL_ERROR_NO_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_NO_COSTUME_SKILL");
		case Result::COSTUME_SKILL_ERROR_NO_EQUIPED_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_NO_EQUIPED_COSTUME_SKILL");
		case Result::COSTUME_CHANGE_ERROR_INVALID_GRADE: return TEXT("COSTUME_CHANGE_ERROR_INVALID_GRADE");
		case Result::COSTUME_CHANGE_ERROR_EXCLUDE_ID: return TEXT("COSTUME_CHANGE_ERROR_EXCLUDE_ID");
		case Result::COSTUME_CHANGE_ERROR_SAME_COSTUME_ID: return TEXT("COSTUME_CHANGE_ERROR_SAME_COSTUME_ID");
		case Result::COSTUME_CHANGE_ERROR_NOT_IN_PROBABILITY_POOL: return TEXT("COSTUME_CHANGE_ERROR_NOT_IN_PROBABILITY_POOL");
		case Result::COSTUME_CHANGE_ERROR_SELECT_COSTUME: return TEXT("COSTUME_CHANGE_ERROR_SELECT_COSTUME");
		case Result::COSTUME_CHANGE_ERROR_NOT_SELECT_COSTUME: return TEXT("COSTUME_CHANGE_ERROR_NOT_SELECT_COSTUME");
		case Result::DUNGEON_ERROR_UNKNOWN: return TEXT("DUNGEON_ERROR_UNKNOWN");
		case Result::DUNGEON_ERROR_INVALID_DUNGEON_GROUP_ID: return TEXT("DUNGEON_ERROR_INVALID_DUNGEON_GROUP_ID");
		case Result::DUNGEON_ERROR_INVALID_DUNGEON_LIST_IDX: return TEXT("DUNGEON_ERROR_INVALID_DUNGEON_LIST_IDX");
		case Result::DUNGEON_ERROR_NOT_ENOUGH_COST: return TEXT("DUNGEON_ERROR_NOT_ENOUGH_COST");
		case Result::DUNGEON_ERROR_INVALID_STATUS: return TEXT("DUNGEON_ERROR_INVALID_STATUS");
		case Result::DUNGEON_ERROR_NOT_ENOUGH_TIME: return TEXT("DUNGEON_ERROR_NOT_ENOUGH_TIME");
		case Result::DUNGEON_ERROR_TIME_OVERFLOW: return TEXT("DUNGEON_ERROR_TIME_OVERFLOW");
		case Result::DUNGEON_ERROR_NOT_OPEN_TIME: return TEXT("DUNGEON_ERROR_NOT_OPEN_TIME");
		case Result::DUNGEON_ERROR_DISABLE_TIME_CHARGE: return TEXT("DUNGEON_ERROR_DISABLE_TIME_CHARGE");
		case Result::DEPOT_ERROR_UNKNOWN: return TEXT("DEPOT_ERROR_UNKNOWN");
		case Result::DEPOT_ERROR_NOT_ENOUGH_MONEY: return TEXT("DEPOT_ERROR_NOT_ENOUGH_MONEY");
		case Result::DEPOT_EXTEND_ERROR_UNKNOWN: return TEXT("DEPOT_EXTEND_ERROR_UNKNOWN");
		case Result::DEPOT_EXTEND_ERROR_MAX_SLOT: return TEXT("DEPOT_EXTEND_ERROR_MAX_SLOT");
		case Result::DEPOT_EXTEND_ERROR_NOT_ENOUGH_MONEY: return TEXT("DEPOT_EXTEND_ERROR_NOT_ENOUGH_MONEY");
		case Result::DEPOT_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("DEPOT_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::DEPOT_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("DEPOT_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::DEPOT_ERROR_INVALID_USER_STATE: return TEXT("DEPOT_ERROR_INVALID_USER_STATE");
		case Result::DEPOT_ERROR_INVALID_USER: return TEXT("DEPOT_ERROR_INVALID_USER");
		case Result::DEPOT_ERROR_DONT_MOVE_ITEM: return TEXT("DEPOT_ERROR_DONT_MOVE_ITEM");
		case Result::ITEM_COLLECTION_ERROR_INVALID_COLLECTION_ID: return TEXT("ITEM_COLLECTION_ERROR_INVALID_COLLECTION_ID");
		case Result::ITEM_COLLECTION_ERROR_INVALID_COLLECTION_CONDITION_ID: return TEXT("ITEM_COLLECTION_ERROR_INVALID_COLLECTION_CONDITION_ID");
		case Result::ITEM_COLLECTION_ERROR_NOT_SUITABLE_ITEM: return TEXT("ITEM_COLLECTION_ERROR_NOT_SUITABLE_ITEM");
		case Result::ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_ITEM: return TEXT("ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_ITEM");
		case Result::ITEM_COLLECTION_ERROR_INVALID_CATEGORY_ID: return TEXT("ITEM_COLLECTION_ERROR_INVALID_CATEGORY_ID");
		case Result::ITEM_COLLECTION_ERROR_INVALID_UNLOCK_TYPE: return TEXT("ITEM_COLLECTION_ERROR_INVALID_UNLOCK_TYPE");
		case Result::ITEM_COLLECTION_ERROR_DOES_NOT_MEET_THE_UNLOCK_CONDITION: return TEXT("ITEM_COLLECTION_ERROR_DOES_NOT_MEET_THE_UNLOCK_CONDITION");
		case Result::ITEM_COLLECTION_ERROR_INVALID_QUEST_TYPE: return TEXT("ITEM_COLLECTION_ERROR_INVALID_QUEST_TYPE");
		case Result::ITEM_COLLECTION_ERROR_EXPIRE_DATE_TIME: return TEXT("ITEM_COLLECTION_ERROR_EXPIRE_DATE_TIME");
		case Result::ITEM_COLLECTION_ERROR_INVALID_BOOKMARK: return TEXT("ITEM_COLLECTION_ERROR_INVALID_BOOKMARK");
		case Result::SPIRIT_SHOT_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("SPIRIT_SHOT_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::SPIRIT_SHOT_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE: return TEXT("SPIRIT_SHOT_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE");
		case Result::SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_CATEGORY_ID: return TEXT("SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_CATEGORY_ID");
		case Result::SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_LEVEL_ID: return TEXT("SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_LEVEL_ID");
		case Result::SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_NEXT_LEVEL_ID: return TEXT("SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_NEXT_LEVEL_ID");
		case Result::SPIRIT_SHOT_ERROR_INVALID_MODIFY_ENCHANT_RATE_COUNT: return TEXT("SPIRIT_SHOT_ERROR_INVALID_MODIFY_ENCHANT_RATE_COUNT");
		case Result::SPIRIT_SHOT_ERROR_INVALID_ITEM_COUNT: return TEXT("SPIRIT_SHOT_ERROR_INVALID_ITEM_COUNT");
		case Result::SPIRIT_SHOT_ERROR_ALREADY_SAME_STATE_AS_BEFORE: return TEXT("SPIRIT_SHOT_ERROR_ALREADY_SAME_STATE_AS_BEFORE");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_INVALID_STATUS: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_INVALID_STATUS");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_ALREADY_SAME_STATE_AS_BEFORE: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_ALREADY_SAME_STATE_AS_BEFORE");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_LEVEL: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_LEVEL");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_ITEM: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_ITEM");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_UNLOCK_CONTENTS: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_UNLOCK_CONTENTS");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_NOT_ENOUGH_MONEY: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_NOT_ENOUGH_MONEY");
		case Result::POTION_AUTO_USE_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("POTION_AUTO_USE_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::POTION_AUTO_USE_ERROR_ALREADY_SAME_STATE_AS_BEFORE: return TEXT("POTION_AUTO_USE_ERROR_ALREADY_SAME_STATE_AS_BEFORE");
		case Result::BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::BM_SHOP_ERROR_UNKNOWN: return TEXT("BM_SHOP_ERROR_UNKNOWN");
		case Result::BM_SHOP_NO_GATEWAY_SERVER_CONNECTION: return TEXT("BM_SHOP_NO_GATEWAY_SERVER_CONNECTION");
		case Result::BM_SHOP_ERROR_INVALID_USER: return TEXT("BM_SHOP_ERROR_INVALID_USER");
		case Result::BM_SHOP_ERROR_IDENDTIFIER: return TEXT("BM_SHOP_ERROR_IDENDTIFIER");
		case Result::BM_SHOP_ERROR_NOT_HIVE_SYNC_MARKET_PID: return TEXT("BM_SHOP_ERROR_NOT_HIVE_SYNC_MARKET_PID");
		case Result::BM_SHOP_ERROR_INVALID_USER_STATUS: return TEXT("BM_SHOP_ERROR_INVALID_USER_STATUS");
		case Result::BM_SHOP_ERROR_NOT_ALLOWED_LEVEL: return TEXT("BM_SHOP_ERROR_NOT_ALLOWED_LEVEL");
		case Result::BM_SHOP_ERROR_NOT_ALLOWED_GUILD_LEVEL: return TEXT("BM_SHOP_ERROR_NOT_ALLOWED_GUILD_LEVEL");
		case Result::BM_SHOP_ERROR_ITEM_NOT_FOUND: return TEXT("BM_SHOP_ERROR_ITEM_NOT_FOUND");
		case Result::BM_SHOP_ERROR_INVALID_ITEM_ID: return TEXT("BM_SHOP_ERROR_INVALID_ITEM_ID");
		case Result::BM_SHOP_ERROR_PRODCUT_NOT_FOUND: return TEXT("BM_SHOP_ERROR_PRODCUT_NOT_FOUND");
		case Result::BM_SHOP_ERROR_INVALID_PRODCUT_ID: return TEXT("BM_SHOP_ERROR_INVALID_PRODCUT_ID");
		case Result::BM_SHOP_ERROR_INVALID_TAB_ID: return TEXT("BM_SHOP_ERROR_INVALID_TAB_ID");
		case Result::BM_SHOP_ERROR_NOT_ENOUGH_MONEY: return TEXT("BM_SHOP_ERROR_NOT_ENOUGH_MONEY");
		case Result::BM_SHOP_ERROR_OVERFLOW_MAX_MONEY: return TEXT("BM_SHOP_ERROR_OVERFLOW_MAX_MONEY");
		case Result::BM_SHOP_ERROR_OVER_QUANTIY: return TEXT("BM_SHOP_ERROR_OVER_QUANTIY");
		case Result::BM_SHOP_ERROR_SALE_DATETIME: return TEXT("BM_SHOP_ERROR_SALE_DATETIME");
		case Result::BM_SHOP_ERROR_CASH_PURCHASE: return TEXT("BM_SHOP_ERROR_CASH_PURCHASE");
		case Result::BM_SHOP_ERROR_PURCHASE: return TEXT("BM_SHOP_ERROR_PURCHASE");
		case Result::BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID: return TEXT("BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID");
		case Result::BM_SHOP_ERROR_DB_OPERATION_FAILED: return TEXT("BM_SHOP_ERROR_DB_OPERATION_FAILED");
		case Result::BM_SHOP_ERROR_HIVE_OPERATION_FAILED: return TEXT("BM_SHOP_ERROR_HIVE_OPERATION_FAILED");
		case Result::BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT: return TEXT("BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT");
		case Result::BM_SHOP_ERROR_IMPOSSIBLE_ITEM: return TEXT("BM_SHOP_ERROR_IMPOSSIBLE_ITEM");
		case Result::BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY: return TEXT("BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY");
		case Result::BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT: return TEXT("BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT");
		case Result::BM_SHOP_DONT_TRY_SAVE_DB: return TEXT("BM_SHOP_DONT_TRY_SAVE_DB");
		case Result::BM_SHOP_ERROR_MONEY: return TEXT("BM_SHOP_ERROR_MONEY");
		case Result::BM_SHOP_ERROR_SAVE_BULK_SALE: return TEXT("BM_SHOP_ERROR_SAVE_BULK_SALE");
		case Result::BM_SHOP_ERROR_CASH_ITEM_ONE_AT_TIME: return TEXT("BM_SHOP_ERROR_CASH_ITEM_ONE_AT_TIME");
		case Result::BM_SHOP_ERROR_BULK_SALE_ITEM: return TEXT("BM_SHOP_ERROR_BULK_SALE_ITEM");
		case Result::BM_SHOP_ERROR_POPUP_FAILED: return TEXT("BM_SHOP_ERROR_POPUP_FAILED");
		case Result::BM_SHOP_ERROR_STEPUP_FAILED: return TEXT("BM_SHOP_ERROR_STEPUP_FAILED");
		case Result::BM_SHOP_DUPLICATION_COST_ITEM: return TEXT("BM_SHOP_DUPLICATION_COST_ITEM");
		case Result::BM_SHOP_COUPON_ITEM_INVALID: return TEXT("BM_SHOP_COUPON_ITEM_INVALID");
		case Result::BM_SHOP_CAN_NOT_COUPON_ITEM_FOR_CASH: return TEXT("BM_SHOP_CAN_NOT_COUPON_ITEM_FOR_CASH");
		case Result::BM_SHOP_CAN_NOT_COUPON_ITEM: return TEXT("BM_SHOP_CAN_NOT_COUPON_ITEM");
		case Result::BM_SHOP_ERROR_PURCHASE_OHTER_ACCOUNT: return TEXT("BM_SHOP_ERROR_PURCHASE_OHTER_ACCOUNT");
		case Result::BM_SHOP_ERROR_PRODCUT_OOAP_NOT_FOUND: return TEXT("BM_SHOP_ERROR_PRODCUT_OOAP_NOT_FOUND");
		case Result::BM_SHOP_RESTORE_ERROR_ITEM_NOT_FOUND: return TEXT("BM_SHOP_RESTORE_ERROR_ITEM_NOT_FOUND");
		case Result::BM_SHOP_RESTORE_ERROR_PRODCUT_NOT_FOUND: return TEXT("BM_SHOP_RESTORE_ERROR_PRODCUT_NOT_FOUND");
		case Result::BM_SHOP_RESTORE_ERROR_NOT_HIVE_SYNC_MARKET_PID: return TEXT("BM_SHOP_RESTORE_ERROR_NOT_HIVE_SYNC_MARKET_PID");
		case Result::BM_SHOP_ERROR_GATEWAY_FIND_USER: return TEXT("BM_SHOP_ERROR_GATEWAY_FIND_USER");
		case Result::BM_SHOP_ERROR_GATEWAY_INVALID_APPID: return TEXT("BM_SHOP_ERROR_GATEWAY_INVALID_APPID");
		case Result::BM_SHOP_ERROR_PG_UNLOCK: return TEXT("BM_SHOP_ERROR_PG_UNLOCK");
		case Result::BM_SHOP_ERROR_PG_UNCONSUMED_COOLTIME: return TEXT("BM_SHOP_ERROR_PG_UNCONSUMED_COOLTIME");
		case Result::SPACE_CRACK_ERROR_PLAY_LIMIT_COUNT: return TEXT("SPACE_CRACK_ERROR_PLAY_LIMIT_COUNT");
		case Result::SPACE_CRACK_ERROR_REWARD_COMPLETED: return TEXT("SPACE_CRACK_ERROR_REWARD_COMPLETED");
		case Result::SPACE_CRACK_ERROR_REWARD_DATA_ID: return TEXT("SPACE_CRACK_ERROR_REWARD_DATA_ID");
		case Result::SPACE_CRACK_ERROR_REWARD_BOX_DATA_ID: return TEXT("SPACE_CRACK_ERROR_REWARD_BOX_DATA_ID");
		case Result::SPACE_CRACK_ERROR_ENTRY_TIME: return TEXT("SPACE_CRACK_ERROR_ENTRY_TIME");
		case Result::MULTI_LEVEL_RANK_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("MULTI_LEVEL_RANK_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::MULTI_LEVEL_RANK_ERROR_INVALID_STATUS: return TEXT("MULTI_LEVEL_RANK_ERROR_INVALID_STATUS");
		case Result::MULTI_LEVEL_RANK_ERROR_PROMOTION_NOT_MEET_THE_CONDITION: return TEXT("MULTI_LEVEL_RANK_ERROR_PROMOTION_NOT_MEET_THE_CONDITION");
		case Result::MULTI_LEVEL_RANK_ERROR_PROMOTION_PROCESS_FAILED: return TEXT("MULTI_LEVEL_RANK_ERROR_PROMOTION_PROCESS_FAILED");
		case Result::MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_MEET_THE_CONDITION: return TEXT("MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_MEET_THE_CONDITION");
		case Result::MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_ENOUGH_MONEY: return TEXT("MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_ENOUGH_MONEY");
		case Result::MULTI_LEVEL_RANK_ERROR_ACCEPT_QUEST_NOT_ENOUGH_REQUIRE_EXP: return TEXT("MULTI_LEVEL_RANK_ERROR_ACCEPT_QUEST_NOT_ENOUGH_REQUIRE_EXP");
		case Result::MULTI_LEVEL_RANK_ERROR_ACCEPT_BEFORE_RANK_NOT_CLEAR_STATE: return TEXT("MULTI_LEVEL_RANK_ERROR_ACCEPT_BEFORE_RANK_NOT_CLEAR_STATE");
		case Result::MULTI_LEVEL_RANK_ERROR_ACCEPT_ALREADY_ACCEPTED_QUEST: return TEXT("MULTI_LEVEL_RANK_ERROR_ACCEPT_ALREADY_ACCEPTED_QUEST");
		case Result::MULTI_LEVEL_RANK_ERROR_REWARD_RANK_NOT_MEET_THE_CONDITION: return TEXT("MULTI_LEVEL_RANK_ERROR_REWARD_RANK_NOT_MEET_THE_CONDITION");
		case Result::MULTI_LEVEL_RANK_ERROR_ALREADY_RECEIVED_REWARD_RANK: return TEXT("MULTI_LEVEL_RANK_ERROR_ALREADY_RECEIVED_REWARD_RANK");
		case Result::MARKET_ERROR_UNKNOWN: return TEXT("MARKET_ERROR_UNKNOWN");
		case Result::MARKET_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("MARKET_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::MARKET_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("MARKET_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::MARKET_ERROR_NOT_TRADABLE_ITEM: return TEXT("MARKET_ERROR_NOT_TRADABLE_ITEM");
		case Result::MARKET_ERROR_ITEM_REGISTER_COUNT_OVER: return TEXT("MARKET_ERROR_ITEM_REGISTER_COUNT_OVER");
		case Result::MARKET_ERROR_NOT_ENOUGH_ITEM_COUNT: return TEXT("MARKET_ERROR_NOT_ENOUGH_ITEM_COUNT");
		case Result::MARKET_ERROR_CANT_REGISTER_EQUIPED_ITEM: return TEXT("MARKET_ERROR_CANT_REGISTER_EQUIPED_ITEM");
		case Result::MARKET_ERROR_NOT_ENOUGH_REGISTER_FEE: return TEXT("MARKET_ERROR_NOT_ENOUGH_REGISTER_FEE");
		case Result::MARKET_ERROR_INVALID_REGISTER_PRICE: return TEXT("MARKET_ERROR_INVALID_REGISTER_PRICE");
		case Result::MARKET_ERROR_INVALID_MARKET_TRANSACTION: return TEXT("MARKET_ERROR_INVALID_MARKET_TRANSACTION");
		case Result::MARKET_ERROR_EXCESS_DIA: return TEXT("MARKET_ERROR_EXCESS_DIA");
		case Result::MARKET_ERROR_NOT_FOUND_TRANSACTION: return TEXT("MARKET_ERROR_NOT_FOUND_TRANSACTION");
		case Result::MARKET_ERROR_CANT_PURCHASE_MY_TRANSACTION: return TEXT("MARKET_ERROR_CANT_PURCHASE_MY_TRANSACTION");
		case Result::MARKET_ERROR_NOT_ENOUGH_DIA: return TEXT("MARKET_ERROR_NOT_ENOUGH_DIA");
		case Result::MARKET_ERROR_CANT_CANCEL_OTHER_TRANSACTION: return TEXT("MARKET_ERROR_CANT_CANCEL_OTHER_TRANSACTION");
		case Result::MARKET_ERROR_INVALIDE_MARKET_CATEGORY: return TEXT("MARKET_ERROR_INVALIDE_MARKET_CATEGORY");
		case Result::MARKET_ERROR_INVALIDE_ITEM_GRADE: return TEXT("MARKET_ERROR_INVALIDE_ITEM_GRADE");
		case Result::MARKET_ERROR_INVALIDE_PLAYING_USER: return TEXT("MARKET_ERROR_INVALIDE_PLAYING_USER");
		case Result::MARKET_ERROR_INVALIDE_ITEM_ID: return TEXT("MARKET_ERROR_INVALIDE_ITEM_ID");
		case Result::MARKET_ERROR_NOT_EXIST_ITEM: return TEXT("MARKET_ERROR_NOT_EXIST_ITEM");
		case Result::MARKET_ERROR_INVALID_ITEM_LEVEL: return TEXT("MARKET_ERROR_INVALID_ITEM_LEVEL");
		case Result::MARKET_ERROR_IS_NOT_EQUIP_ITEM: return TEXT("MARKET_ERROR_IS_NOT_EQUIP_ITEM");
		case Result::MARKET_ERROR_NO_UPDATED_TRANSACTION: return TEXT("MARKET_ERROR_NO_UPDATED_TRANSACTION");
		case Result::MARKET_ERROR_BOOKMARK_LIMIT_EXCEEDED: return TEXT("MARKET_ERROR_BOOKMARK_LIMIT_EXCEEDED");
		case Result::MARKET_ERROR_DB_REQUEST_CALCULATE: return TEXT("MARKET_ERROR_DB_REQUEST_CALCULATE");
		case Result::MARKET_ERROR_CALCULATE: return TEXT("MARKET_ERROR_CALCULATE");
		case Result::MARKET_ERROR_CALCULATE_NOT_PERCHASED_TRANSACTION: return TEXT("MARKET_ERROR_CALCULATE_NOT_PERCHASED_TRANSACTION");
		case Result::MARKET_ERROR_CALCULATE_TRANSACTION_CACULATE: return TEXT("MARKET_ERROR_CALCULATE_TRANSACTION_CACULATE");
		case Result::MARKET_ERROR_CALCULATE_CURRENCY_UPDATE: return TEXT("MARKET_ERROR_CALCULATE_CURRENCY_UPDATE");
		case Result::MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_COMMIT: return TEXT("MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_COMMIT");
		case Result::MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_ROLLBACK: return TEXT("MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_ROLLBACK");
		case Result::MARKET_ERROR_PURCHASE: return TEXT("MARKET_ERROR_PURCHASE");
		case Result::MARKET_ERROR_DB_REQUEST_MOVE_ITEM: return TEXT("MARKET_ERROR_DB_REQUEST_MOVE_ITEM");
		case Result::MARKET_ERROR_DB_ITEM_MOVE_ON_INVENTORY: return TEXT("MARKET_ERROR_DB_ITEM_MOVE_ON_INVENTORY");
		case Result::MARKET_ERROR_DB_MOVE_ITEM_BY_CREATE: return TEXT("MARKET_ERROR_DB_MOVE_ITEM_BY_CREATE");
		case Result::MARKET_ERROR_DB_MOVE_ITEM_BY_INCREASE: return TEXT("MARKET_ERROR_DB_MOVE_ITEM_BY_INCREASE");
		case Result::MARKET_ERROR_DB_CANCEL_DELETE_TRANSACTION: return TEXT("MARKET_ERROR_DB_CANCEL_DELETE_TRANSACTION");
		case Result::MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION: return TEXT("MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION");
		case Result::MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_DELETED: return TEXT("MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_DELETED");
		case Result::MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_PURCHASED: return TEXT("MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_PURCHASED");
		case Result::MARKET_ERROR_DB_PURCHASE_DELETE_MARKET: return TEXT("MARKET_ERROR_DB_PURCHASE_DELETE_MARKET");
		case Result::MARKET_ERROR_DB_PURCHASE_DB_OPERATION: return TEXT("MARKET_ERROR_DB_PURCHASE_DB_OPERATION");
		case Result::MARKET_ERROR_CANCEL: return TEXT("MARKET_ERROR_CANCEL");
		case Result::MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_COMMIT: return TEXT("MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_COMMIT");
		case Result::MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_ROLLBACK: return TEXT("MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_ROLLBACK");
		case Result::MARKET_ERROR_CANNOT_USE_GM: return TEXT("MARKET_ERROR_CANNOT_USE_GM");
		case Result::ATTENDANCE_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("ATTENDANCE_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::ATTENDANCE_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("ATTENDANCE_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::ATTENDANCE_ERROR_INVALID_ID: return TEXT("ATTENDANCE_ERROR_INVALID_ID");
		case Result::ATTENDANCE_ERROR_EXPIRED: return TEXT("ATTENDANCE_ERROR_EXPIRED");
		case Result::ATTENDANCE_ERROR_ALREADY_HAVE: return TEXT("ATTENDANCE_ERROR_ALREADY_HAVE");
		case Result::ATTENDANCE_ERROR_NO_HAVE: return TEXT("ATTENDANCE_ERROR_NO_HAVE");
		case Result::ATTENDANCE_ERROR_ALREADY_CHECK_IN: return TEXT("ATTENDANCE_ERROR_ALREADY_CHECK_IN");
		case Result::ATTENDANCE_ERROR_NOT_ENOUGH_MONEY: return TEXT("ATTENDANCE_ERROR_NOT_ENOUGH_MONEY");
		case Result::ATTENDANCE_ERROR_NOT_ENOUGH_ITEM: return TEXT("ATTENDANCE_ERROR_NOT_ENOUGH_ITEM");
		case Result::ATTENDANCE_ERROR_OVERFLOW_MAX_MONEY: return TEXT("ATTENDANCE_ERROR_OVERFLOW_MAX_MONEY");
		case Result::ATTENDANCE_ERROR_NOT_OPEN: return TEXT("ATTENDANCE_ERROR_NOT_OPEN");
		case Result::ATTENDANCE_ERROR_RECEIVE_DURATION: return TEXT("ATTENDANCE_ERROR_RECEIVE_DURATION");
		case Result::NPC_EVENT_SHOP_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("NPC_EVENT_SHOP_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::NPC_EVENT_SHOP_ERROR_UNKNOWN: return TEXT("NPC_EVENT_SHOP_ERROR_UNKNOWN");
		case Result::NPC_EVENT_SHOP_NO_GATEWAY_SERVER_CONNECTION: return TEXT("NPC_EVENT_SHOP_NO_GATEWAY_SERVER_CONNECTION");
		case Result::NPC_EVENT_SHOP_ERROR_INVALID_USER: return TEXT("NPC_EVENT_SHOP_ERROR_INVALID_USER");
		case Result::NPC_EVENT_SHOP_ERROR_INVALID_USER_STATUS: return TEXT("NPC_EVENT_SHOP_ERROR_INVALID_USER_STATUS");
		case Result::NPC_EVENT_SHOP_ERROR_NOT_ALLOWED_LEVEL: return TEXT("NPC_EVENT_SHOP_ERROR_NOT_ALLOWED_LEVEL");
		case Result::NPC_EVENT_SHOP_ERROR_ITEM_NOT_FOUND: return TEXT("NPC_EVENT_SHOP_ERROR_ITEM_NOT_FOUND");
		case Result::NPC_EVENT_SHOP_ERROR_INVALID_ITEM_ID: return TEXT("NPC_EVENT_SHOP_ERROR_INVALID_ITEM_ID");
		case Result::NPC_EVENT_SHOP_ERROR_INVALID_NPC: return TEXT("NPC_EVENT_SHOP_ERROR_INVALID_NPC");
		case Result::NPC_EVENT_SHOP_ERROR_DIFFERENT_CHANNEL: return TEXT("NPC_EVENT_SHOP_ERROR_DIFFERENT_CHANNEL");
		case Result::NPC_EVENT_SHOP_ERROR_NOT_ENOUGH_MONEY: return TEXT("NPC_EVENT_SHOP_ERROR_NOT_ENOUGH_MONEY");
		case Result::NPC_EVENT_SHOP_ERROR_OVERFLOW_MAX_MONEY: return TEXT("NPC_EVENT_SHOP_ERROR_OVERFLOW_MAX_MONEY");
		case Result::NPC_EVENT_SHOP_ERROR_OVERFLOW_CURRENCY_DELTA: return TEXT("NPC_EVENT_SHOP_ERROR_OVERFLOW_CURRENCY_DELTA");
		case Result::NPC_EVENT_SHOP_ERROR_OVER_QUANTIY: return TEXT("NPC_EVENT_SHOP_ERROR_OVER_QUANTIY");
		case Result::NPC_EVENT_SHOP_ERROR_GUILD_MEMBER_GRADE: return TEXT("NPC_EVENT_SHOP_ERROR_GUILD_MEMBER_GRADE");
		case Result::NPC_EVENT_SHOP_ERROR_GUILD_ID: return TEXT("NPC_EVENT_SHOP_ERROR_GUILD_ID");
		case Result::NPC_EVENT_SHOP_ERROR_GUILD_LEVEL: return TEXT("NPC_EVENT_SHOP_ERROR_GUILD_LEVEL");
		case Result::NPC_EVENT_SHOP_ERROR_SALE_DATETIME: return TEXT("NPC_EVENT_SHOP_ERROR_SALE_DATETIME");
		case Result::NPC_EVENT_SHOP_ERROR_PURCHASE: return TEXT("NPC_EVENT_SHOP_ERROR_PURCHASE");
		case Result::NPC_EVENT_SHOP_DUPLICATION_COST_ITEM: return TEXT("NPC_EVENT_SHOP_DUPLICATION_COST_ITEM");
		case Result::OFF_LINE_PLAY_AUTO_ATTACK: return TEXT("OFF_LINE_PLAY_AUTO_ATTACK");
		case Result::OFF_LINE_PLAY_USER_DEATH: return TEXT("OFF_LINE_PLAY_USER_DEATH");
		case Result::OFF_LINE_PLAY_EXPIRY_TIME: return TEXT("OFF_LINE_PLAY_EXPIRY_TIME");
		case Result::OFF_LINE_PLAY_AREA: return TEXT("OFF_LINE_PLAY_AREA");
		case Result::OFF_LINE_LOAD_ERROR: return TEXT("OFF_LINE_LOAD_ERROR");
		case Result::CLIENT_CINEMATIC_ERROR_INVALID_STATUS: return TEXT("CLIENT_CINEMATIC_ERROR_INVALID_STATUS");
		case Result::CLIENT_CINEMATIC_ALREADY: return TEXT("CLIENT_CINEMATIC_ALREADY");
		case Result::CLIENT_CINEMATIC_END_ERROR_SEQUENCE_RES_ID: return TEXT("CLIENT_CINEMATIC_END_ERROR_SEQUENCE_RES_ID");
		case Result::CLIENT_CINEMATIC_ERROR_NOT_PLAYING: return TEXT("CLIENT_CINEMATIC_ERROR_NOT_PLAYING");
		case Result::CLIENT_CINEMATIC_ERROR_WARP: return TEXT("CLIENT_CINEMATIC_ERROR_WARP");
		case Result::CRAFT_ERROR_INVALID_STATUS: return TEXT("CRAFT_ERROR_INVALID_STATUS");
		case Result::CRAFT_ERROR_EXCEED_LIMIT_COUNT_ACCOUNT: return TEXT("CRAFT_ERROR_EXCEED_LIMIT_COUNT_ACCOUNT");
		case Result::CRAFT_ERROR_LACK_OF_MATERIAL: return TEXT("CRAFT_ERROR_LACK_OF_MATERIAL");
		case Result::CRAFT_ERROR_EXPIRE_DATE_ITEM_CRAFT: return TEXT("CRAFT_ERROR_EXPIRE_DATE_ITEM_CRAFT");
		case Result::CRAFT_ERROR_EXCEED_LIMIT_COUNT_SERVER: return TEXT("CRAFT_ERROR_EXCEED_LIMIT_COUNT_SERVER");
		case Result::CRAFT_ERROR_NOT_MEET_THE_UNLOCK_CONDITION: return TEXT("CRAFT_ERROR_NOT_MEET_THE_UNLOCK_CONDITION");
		case Result::CRAFT_ERROR_EXCEED_MAX_CRAFT_COUNT: return TEXT("CRAFT_ERROR_EXCEED_MAX_CRAFT_COUNT");
		case Result::CRAFT_ERROR_ONLY_SOME_ITEMS_WERE_CRAFTED: return TEXT("CRAFT_ERROR_ONLY_SOME_ITEMS_WERE_CRAFTED");
		case Result::GUILD_STORE_UNKNOWN_ERROR: return TEXT("GUILD_STORE_UNKNOWN_ERROR");
		case Result::GUILD_STORE_ERROR_UNKNOWN_GUILD_ID: return TEXT("GUILD_STORE_ERROR_UNKNOWN_GUILD_ID");
		case Result::GUILD_STORE_ERROR_NOT_HAVE_GRADE_TO_DISTRIBUTE: return TEXT("GUILD_STORE_ERROR_NOT_HAVE_GRADE_TO_DISTRIBUTE");
		case Result::GUILD_STORE_ERROR_NOT_GUILD_MEMBER_TO_DIRTIBUTE: return TEXT("GUILD_STORE_ERROR_NOT_GUILD_MEMBER_TO_DIRTIBUTE");
		case Result::GUILD_STORE_ERROR_GRADE: return TEXT("GUILD_STORE_ERROR_GRADE");
		case Result::GUILD_STORE_ERROR_NO_ITEM_TO_DISTRIBUTE: return TEXT("GUILD_STORE_ERROR_NO_ITEM_TO_DISTRIBUTE");
		case Result::GUILD_STORE_ERROR_NO_GUILD_MEMBER_TO_RECEIVE_ITEM: return TEXT("GUILD_STORE_ERROR_NO_GUILD_MEMBER_TO_RECEIVE_ITEM");
		case Result::GUILD_STORE_ERROR_ITEM_AMOUNT: return TEXT("GUILD_STORE_ERROR_ITEM_AMOUNT");
		case Result::GUILD_STORE_ERROR_EXPIRED_ITEM: return TEXT("GUILD_STORE_ERROR_EXPIRED_ITEM");
		case Result::GUILD_STORE_ERROR_TRY_AGAIN: return TEXT("GUILD_STORE_ERROR_TRY_AGAIN");
		case Result::GUILD_EXTEND_ERROR_NOT_ENOUGH_MONEY: return TEXT("GUILD_EXTEND_ERROR_NOT_ENOUGH_MONEY");
		case Result::GUILD_EXTEND_ERROR_MAX_SLOT: return TEXT("GUILD_EXTEND_ERROR_MAX_SLOT");
		case Result::GUILD_EXTEND_ERROR_FULL: return TEXT("GUILD_EXTEND_ERROR_FULL");
		case Result::GUILD_ITEM_COLLECTION_ERROR_UNKNOWN: return TEXT("GUILD_ITEM_COLLECTION_ERROR_UNKNOWN");
		case Result::GUILD_ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_CONDITION: return TEXT("GUILD_ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_CONDITION");
		case Result::GUILD_ITEM_COLLECTION_INVALID_CONDITION_ID: return TEXT("GUILD_ITEM_COLLECTION_INVALID_CONDITION_ID");
		case Result::GUILD_ITEM_COLLECTION_USER_REGISTER_COUNT_LIMIT_OVER: return TEXT("GUILD_ITEM_COLLECTION_USER_REGISTER_COUNT_LIMIT_OVER");
		case Result::GUILD_ITEM_COLLECTION_INACTIVE_EVENT_COLLECTION: return TEXT("GUILD_ITEM_COLLECTION_INACTIVE_EVENT_COLLECTION");
		case Result::GUILD_ITEM_COLLECTION_ALREADY_CLEAR_CONDITION: return TEXT("GUILD_ITEM_COLLECTION_ALREADY_CLEAR_CONDITION");
		case Result::GUILD_ITEM_COLLECTION_ALREADY_CLEAR_COLLECTION: return TEXT("GUILD_ITEM_COLLECTION_ALREADY_CLEAR_COLLECTION");
		case Result::GUILD_ITEM_COLLECTION_NOT_SUITABLE_ITEM: return TEXT("GUILD_ITEM_COLLECTION_NOT_SUITABLE_ITEM");
		case Result::TREASURE_MONSTER_ERROR_SAFE_TYTERRITORY: return TEXT("TREASURE_MONSTER_ERROR_SAFE_TYTERRITORY");
		case Result::TREASURE_MONSTER_ERROR_ALREADY_CREATED: return TEXT("TREASURE_MONSTER_ERROR_ALREADY_CREATED");
		case Result::TREASURE_MONSTER_ERROR_INVALID_AREA: return TEXT("TREASURE_MONSTER_ERROR_INVALID_AREA");
		case Result::ACHIEVEMENT_ERROR_UNKNOWN: return TEXT("ACHIEVEMENT_ERROR_UNKNOWN");
		case Result::ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_ID: return TEXT("ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_ID");
		case Result::ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_THEME_ID: return TEXT("ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_THEME_ID");
		case Result::ACHIEVEMENT_ERROR_NOT_MEET_THE_REWARD_CONDITION: return TEXT("ACHIEVEMENT_ERROR_NOT_MEET_THE_REWARD_CONDITION");
		case Result::MONSTER_KNOWLEDGE_ERROR_INVALID_USER: return TEXT("MONSTER_KNOWLEDGE_ERROR_INVALID_USER");
		case Result::MONSTER_KNOWLEDGE_ERROR_CANT_LEVEL_UP_KNOWLEDGE: return TEXT("MONSTER_KNOWLEDGE_ERROR_CANT_LEVEL_UP_KNOWLEDGE");
		case Result::MONSTER_KNOWLEDGE_ERROR_INVALID_ITEM_AMOUNT: return TEXT("MONSTER_KNOWLEDGE_ERROR_INVALID_ITEM_AMOUNT");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_GAIN_EXP_ITEM: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_GAIN_EXP_ITEM");
		case Result::MONSTER_KNOWLEDGE_ERROR_NO_KNOWLEDGE_LEVEL_UP_AVAILABLE: return TEXT("MONSTER_KNOWLEDGE_ERROR_NO_KNOWLEDGE_LEVEL_UP_AVAILABLE");
		case Result::MONSTER_KNOWLEDGE_ERROR_CANT_SLATE_SLOT_ACTIVATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_CANT_SLATE_SLOT_ACTIVATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_RESET_SLATE_SLOT_ITEM: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_RESET_SLATE_SLOT_ITEM");
		case Result::MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE: return TEXT("MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE");
		case Result::MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE_EXCEEDED_CURRENCY: return TEXT("MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE_EXCEEDED_CURRENCY");
		case Result::MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_EFFECT_PARAM: return TEXT("MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_EFFECT_PARAM");
		case Result::MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_TEMPLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_TEMPLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_SLATE_SLOT_OPEN_COST: return TEXT("MONSTER_KNOWLEDGE_ERROR_SLATE_SLOT_OPEN_COST");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_ENOUGHT_MONSTER_BOOK_POINT: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_ENOUGHT_MONSTER_BOOK_POINT");
		case Result::MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_EFFECT_PARAM: return TEXT("MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_EFFECT_PARAM");
		case Result::MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_TEMPLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_TEMPLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_UNLOCK_SLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_ALREADY_UNLOCK_SLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_RESET_SLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_RESET_SLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_LEVEL: return TEXT("MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_LEVEL");
		case Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_EXP: return TEXT("MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_EXP");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_KNOWLEDGE: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_KNOWLEDGE");
		case Result::MONSTER_KNOWLEDGE_ERROR_LOCKED_SLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_LOCKED_SLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_INVALID_KNOWLEDGE_MAX_EXP: return TEXT("MONSTER_KNOWLEDGE_ERROR_INVALID_KNOWLEDGE_MAX_EXP");
		case Result::SLATE_PROBABILITY_FAIL_TO_ACTIVATE_SPECIAL_NODE: return TEXT("SLATE_PROBABILITY_FAIL_TO_ACTIVATE_SPECIAL_NODE");
		case Result::SLATE_ERROR_INVALID_SLATE_ID: return TEXT("SLATE_ERROR_INVALID_SLATE_ID");
		case Result::SLATE_ERROR_INVALID_SPECIAL_NODE_ID: return TEXT("SLATE_ERROR_INVALID_SPECIAL_NODE_ID");
		case Result::SLATE_ERROR_INVALID_USER: return TEXT("SLATE_ERROR_INVALID_USER");
		case Result::SLATE_ERROR_LOCKED_SLATE: return TEXT("SLATE_ERROR_LOCKED_SLATE");
		case Result::SLATE_ERROR_CANT_ACTIVATE_SPECIAL_NODE_ANYMORE: return TEXT("SLATE_ERROR_CANT_ACTIVATE_SPECIAL_NODE_ANYMORE");
		case Result::SLATE_ERROR_DB_REQUEST_FAILURE: return TEXT("SLATE_ERROR_DB_REQUEST_FAILURE");
		case Result::CODEX_ERROR_INVALID_USER: return TEXT("CODEX_ERROR_INVALID_USER");
		case Result::CODEX_ERROR_INVALID_CATEGORY_ID: return TEXT("CODEX_ERROR_INVALID_CATEGORY_ID");
		case Result::CODEX_ERROR_NOT_ENOUGTH_MONSTER_KNOWLEDGE: return TEXT("CODEX_ERROR_NOT_ENOUGTH_MONSTER_KNOWLEDGE");
		case Result::CODEX_ERROR_INVALID_CATEGORY_BALANCE_ID: return TEXT("CODEX_ERROR_INVALID_CATEGORY_BALANCE_ID");
		case Result::CODEX_ERROR_NOT_ENOUGH_MAP_GROUP_LEVEL_UP_COUNT: return TEXT("CODEX_ERROR_NOT_ENOUGH_MAP_GROUP_LEVEL_UP_COUNT");
		case Result::CODEX_CANT_EXCHANGE_ANYMORE_MAX_POINT: return TEXT("CODEX_CANT_EXCHANGE_ANYMORE_MAX_POINT");
		case Result::CODEX_ERROR_CATEGORY_LEVEL_UP_ZERO: return TEXT("CODEX_ERROR_CATEGORY_LEVEL_UP_ZERO");
		case Result::CODEX_ERROR_CATEGORY_LEVEL_UP_SAME_LEVEL: return TEXT("CODEX_ERROR_CATEGORY_LEVEL_UP_SAME_LEVEL");
		case Result::CODEX_ERROR_REQUEST_ITEM_AMOUNT_NOT_ENOUGH: return TEXT("CODEX_ERROR_REQUEST_ITEM_AMOUNT_NOT_ENOUGH");
		case Result::CODEX_ERROR_NOT_GAIN_EXP_ITEM: return TEXT("CODEX_ERROR_NOT_GAIN_EXP_ITEM");
		case Result::CODEX_ERROR_INVALID_MAP_GROUP_ID: return TEXT("CODEX_ERROR_INVALID_MAP_GROUP_ID");
		case Result::CODEX_ERROR_NOT_INGREDIENT_ITEM: return TEXT("CODEX_ERROR_NOT_INGREDIENT_ITEM");
		case Result::CODEX_ERROR_MAP_GROUP_MAX_EXP: return TEXT("CODEX_ERROR_MAP_GROUP_MAX_EXP");
		case Result::CODEX_ERROR_MAX_MONSTER_BOOK_POINT: return TEXT("CODEX_ERROR_MAX_MONSTER_BOOK_POINT");
		case Result::CODEX_ERROR_DB_REQUEST_FAILURE: return TEXT("CODEX_ERROR_DB_REQUEST_FAILURE");
		case Result::DEATH_PENALTY_ERROR_USER_LEVEL_DATA_TABLE: return TEXT("DEATH_PENALTY_ERROR_USER_LEVEL_DATA_TABLE");
		case Result::DEATH_ENALTY_ERROR_RESURREC_DATA_TABLE: return TEXT("DEATH_ENALTY_ERROR_RESURREC_DATA_TABLE");
		case Result::RECONNECT_ERROR_UNKNOWN: return TEXT("RECONNECT_ERROR_UNKNOWN");
		case Result::RECONNECT_ERROR_INVALID_ACCOUNT_ID: return TEXT("RECONNECT_ERROR_INVALID_ACCOUNT_ID");
		case Result::RECONNECT_ERROR_INVALID_STATE: return TEXT("RECONNECT_ERROR_INVALID_STATE");
		case Result::RECONNECT_ERROR_INCORRECT_TOKEN: return TEXT("RECONNECT_ERROR_INCORRECT_TOKEN");
		case Result::RECONNECT_ERROR_NOT_DISCONNECTED_YET: return TEXT("RECONNECT_ERROR_NOT_DISCONNECTED_YET");
		case Result::CREATE_USER_ERROR_UNKNOWN: return TEXT("CREATE_USER_ERROR_UNKNOWN");
		case Result::CREATE_USER_ERROR_INTERNAL_ERROR: return TEXT("CREATE_USER_ERROR_INTERNAL_ERROR");
		case Result::CREATE_USER_ERROR_DB_OPERATION_ERROR: return TEXT("CREATE_USER_ERROR_DB_OPERATION_ERROR");
		case Result::CREATE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER: return TEXT("CREATE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER");
		case Result::CREATE_USER_ERROR_ALREADY_EXIST_NAME: return TEXT("CREATE_USER_ERROR_ALREADY_EXIST_NAME");
		case Result::CREATE_USER_ERROR_INVALID_LENGTH_NAME: return TEXT("CREATE_USER_ERROR_INVALID_LENGTH_NAME");
		case Result::CREATE_USER_ERROR_NOT_ALLOWED_NAME: return TEXT("CREATE_USER_ERROR_NOT_ALLOWED_NAME");
		case Result::CREATE_USER_ERROR_INVALID_GENDER_TYPE: return TEXT("CREATE_USER_ERROR_INVALID_GENDER_TYPE");
		case Result::CREATE_USER_ERROR_INVALID_WEAPON_TYPE: return TEXT("CREATE_USER_ERROR_INVALID_WEAPON_TYPE");
		case Result::CREATE_USER_ERROR_PUSH_INITIAL_ITEM: return TEXT("CREATE_USER_ERROR_PUSH_INITIAL_ITEM");
		case Result::CREATE_USER_ERROR_SET_INITIAL_SKILL: return TEXT("CREATE_USER_ERROR_SET_INITIAL_SKILL");
		case Result::CREATE_USER_ERROR_SET_INITIAL_FAIRY: return TEXT("CREATE_USER_ERROR_SET_INITIAL_FAIRY");
		case Result::CREATE_USER_ERROR_NO_MORE_USER_SOCKET: return TEXT("CREATE_USER_ERROR_NO_MORE_USER_SOCKET");
		case Result::CREATE_USER_ERROR_PUSH_INITIAL_CURRENCY: return TEXT("CREATE_USER_ERROR_PUSH_INITIAL_CURRENCY");
		case Result::CREATE_USER_ERROR_SET_INITIAL_COSTUME: return TEXT("CREATE_USER_ERROR_SET_INITIAL_COSTUME");
		case Result::CREATE_USER_ERROR_ADD_INITIAL_MAIL: return TEXT("CREATE_USER_ERROR_ADD_INITIAL_MAIL");
		case Result::CREATE_USER_ERROR_SET_NEW_USER_ATTENDANCE: return TEXT("CREATE_USER_ERROR_SET_NEW_USER_ATTENDANCE");
		case Result::CREATE_USER_ERROR_SET_NEW_USER_COMMUNITY_EVENT_MISSION: return TEXT("CREATE_USER_ERROR_SET_NEW_USER_COMMUNITY_EVENT_MISSION");
		case Result::CREATE_USER_ERROR_PENALTY_FREE: return TEXT("CREATE_USER_ERROR_PENALTY_FREE");
		case Result::CREATE_USER_ERROR_ARCAS: return TEXT("CREATE_USER_ERROR_ARCAS");
		case Result::CREATE_USER_ERROR_QUEST_MAIN: return TEXT("CREATE_USER_ERROR_QUEST_MAIN");
		case Result::CREATE_USER_ERROR_COMBAT_OPTION: return TEXT("CREATE_USER_ERROR_COMBAT_OPTION");
		case Result::CREATE_USER_ERROR_OFF_LINE: return TEXT("CREATE_USER_ERROR_OFF_LINE");
		case Result::CREATE_USER_ERROR_SET_ISSUED_USER_ID: return TEXT("CREATE_USER_ERROR_SET_ISSUED_USER_ID");
		case Result::CREATE_USER_ERROR_INSERT_USER: return TEXT("CREATE_USER_ERROR_INSERT_USER");
		case Result::CREATE_USER_ERROR_INSERT_USER_ROW_COUNT: return TEXT("CREATE_USER_ERROR_INSERT_USER_ROW_COUNT");
		case Result::CREATE_USER_ERROR_CALL_END_TRANSACTION_COMMIT: return TEXT("CREATE_USER_ERROR_CALL_END_TRANSACTION_COMMIT");
		case Result::CREATE_USER_ERROR_CALL_END_TRANSACTION_ROLLBACK: return TEXT("CREATE_USER_ERROR_CALL_END_TRANSACTION_ROLLBACK");
		case Result::CREATE_USER_ERROR_SET_NEW_USER_SOCIAL_ACTON: return TEXT("CREATE_USER_ERROR_SET_NEW_USER_SOCIAL_ACTON");
		case Result::CREATE_USER_ERROR_CREATION_LIMIT: return TEXT("CREATE_USER_ERROR_CREATION_LIMIT");
		case Result::DELETE_USER_ERROR_UNKNOWN: return TEXT("DELETE_USER_ERROR_UNKNOWN");
		case Result::DELETE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER: return TEXT("DELETE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER");
		case Result::DELETE_USER_ERROR_INVALID_USER_DB_ID: return TEXT("DELETE_USER_ERROR_INVALID_USER_DB_ID");
		case Result::DELETE_USER_ERROR_DB_OPERATION_ERROR: return TEXT("DELETE_USER_ERROR_DB_OPERATION_ERROR");
		case Result::DELETE_USER_ERROR_GUILD_MEMBER_CANNOT: return TEXT("DELETE_USER_ERROR_GUILD_MEMBER_CANNOT");
		case Result::DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL: return TEXT("DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL");
		case Result::DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION: return TEXT("DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION");
		case Result::DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION: return TEXT("DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION");
		case Result::SELECT_USER_ERROR_UNKNOWN: return TEXT("SELECT_USER_ERROR_UNKNOWN");
		case Result::SELECT_USER_ERROR_WORLD_SERVER_DOWN: return TEXT("SELECT_USER_ERROR_WORLD_SERVER_DOWN");
		case Result::SELECT_USER_ERROR_MUST_CHANGE_USER_NAME: return TEXT("SELECT_USER_ERROR_MUST_CHANGE_USER_NAME");
		case Result::SELECT_USER_ERROR_LEAVING_WORLD: return TEXT("SELECT_USER_ERROR_LEAVING_WORLD");
		case Result::SELECT_USER_ERROR_INVALID_LOGIN_STATE: return TEXT("SELECT_USER_ERROR_INVALID_LOGIN_STATE");
		case Result::SELECT_USER_ERROR_BLOCK_USER: return TEXT("SELECT_USER_ERROR_BLOCK_USER");
		case Result::LOGOUT_ERROR_UNKNOWN: return TEXT("LOGOUT_ERROR_UNKNOWN");
		case Result::LOGOUT_ERROR_COMBAT_STATE: return TEXT("LOGOUT_ERROR_COMBAT_STATE");
		case Result::BUDDY_ERROR_UNKNOWN: return TEXT("BUDDY_ERROR_UNKNOWN");
		case Result::BUDDY_ERROR_INVALID_NAME_LENGTH: return TEXT("BUDDY_ERROR_INVALID_NAME_LENGTH");
		case Result::BUDDY_ERROR_REQUEST_MY_SELF: return TEXT("BUDDY_ERROR_REQUEST_MY_SELF");
		case Result::BUDDY_ERROR_NOT_FOUND_USER: return TEXT("BUDDY_ERROR_NOT_FOUND_USER");
		case Result::BUDDY_ERROR_INVALID_USER_ID: return TEXT("BUDDY_ERROR_INVALID_USER_ID");
		case Result::BUDDY_ERROR_DB_OPERATION_ERROR: return TEXT("BUDDY_ERROR_DB_OPERATION_ERROR");
		case Result::BUDDY_ERROR_INVALID_INVITATION: return TEXT("BUDDY_ERROR_INVALID_INVITATION");
		case Result::BUDDY_ERROR_ALREADY_BUDDY: return TEXT("BUDDY_ERROR_ALREADY_BUDDY");
		case Result::BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY: return TEXT("BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY");
		case Result::BUDDY_ERROR_ALREADY_INVITATION_SENT: return TEXT("BUDDY_ERROR_ALREADY_INVITATION_SENT");
		case Result::BUDDY_ERROR_ALREADY_INVITATION_RECV: return TEXT("BUDDY_ERROR_ALREADY_INVITATION_RECV");
		case Result::BUDDY_ERROR_SEND_INVITATION_FULL: return TEXT("BUDDY_ERROR_SEND_INVITATION_FULL");
		case Result::BUDDY_ERROR_REACHED_MAX_BUDDY: return TEXT("BUDDY_ERROR_REACHED_MAX_BUDDY");
		case Result::BUDDY_ERROR_NOT_BUDDY: return TEXT("BUDDY_ERROR_NOT_BUDDY");
		case Result::BUDDY_ERROR_DELETE_SEND_INVITATION: return TEXT("BUDDY_ERROR_DELETE_SEND_INVITATION");
		case Result::BUDDY_ERROR_DELETE_RECV_INVITATION: return TEXT("BUDDY_ERROR_DELETE_RECV_INVITATION");
		case Result::BUDDY_ERROR_TARGET_RECV_INVITATION_FULL: return TEXT("BUDDY_ERROR_TARGET_RECV_INVITATION_FULL");
		case Result::BUDDY_ERROR_INVALID_BUDDY_TAB_TYPE: return TEXT("BUDDY_ERROR_INVALID_BUDDY_TAB_TYPE");
		case Result::BUDDY_ERROR_INSERT_BUDDY_TO_ME: return TEXT("BUDDY_ERROR_INSERT_BUDDY_TO_ME");
		case Result::BUDDY_ERROR_INSERT_ME_TO_BUDDY: return TEXT("BUDDY_ERROR_INSERT_ME_TO_BUDDY");
		case Result::BUDDY_ERROR_DELETE_REQUEST_INVITATION: return TEXT("BUDDY_ERROR_DELETE_REQUEST_INVITATION");
		case Result::BUDDY_ERROR_INSERT_REQUEST_INVITATION: return TEXT("BUDDY_ERROR_INSERT_REQUEST_INVITATION");
		case Result::BUDDY_ERROR_INVALID_INVITATION_SAME_ACCOUNT: return TEXT("BUDDY_ERROR_INVALID_INVITATION_SAME_ACCOUNT");
		case Result::BUDDY_ERROR_CACHE_MISSING: return TEXT("BUDDY_ERROR_CACHE_MISSING");
		case Result::BUDDY_ERROR_WORLD_SERVER_ITEM: return TEXT("BUDDY_ERROR_WORLD_SERVER_ITEM");
		case Result::CHAT_BLACKLIST_ADD_ERROR_DB: return TEXT("CHAT_BLACKLIST_ADD_ERROR_DB");
		case Result::CHAT_BLACKLIST_ADD_ERROR_OVER: return TEXT("CHAT_BLACKLIST_ADD_ERROR_OVER");
		case Result::CHAT_BLACKLIST_ADD_ERROR_UNKNOWN_USER: return TEXT("CHAT_BLACKLIST_ADD_ERROR_UNKNOWN_USER");
		case Result::CHAT_BLACKLIST_ADD_ERROR_DUPLICATION: return TEXT("CHAT_BLACKLIST_ADD_ERROR_DUPLICATION");
		case Result::CHAT_BLACKLIST_REMOVE_ERROR_DB: return TEXT("CHAT_BLACKLIST_REMOVE_ERROR_DB");
		case Result::REPORT_BAD_CHAT_ERROR_DB: return TEXT("REPORT_BAD_CHAT_ERROR_DB");
		case Result::REPORT_BAD_CHAT_ERROR_OVER: return TEXT("REPORT_BAD_CHAT_ERROR_OVER");
		case Result::REPORT_BAD_CHAT_ERROR_UNKNOWN_USER: return TEXT("REPORT_BAD_CHAT_ERROR_UNKNOWN_USER");
		case Result::QUEST_ERROR_NOT_EXIST_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_EXIST_GUILD_QUEST");
		case Result::QUEST_ERROR_INVALID_GUILD_QUEST_DB_ID: return TEXT("QUEST_ERROR_INVALID_GUILD_QUEST_DB_ID");
		case Result::QUEST_ERROR_GUILD_QUEST_DB_OPERATION: return TEXT("QUEST_ERROR_GUILD_QUEST_DB_OPERATION");
		case Result::REGISTER_ERROR_UNKNOWN: return TEXT("REGISTER_ERROR_UNKNOWN");
		case Result::REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID: return TEXT("REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID");
		case Result::HIVE_RECEIPT_VERIFY_ERROR_SETTING: return TEXT("HIVE_RECEIPT_VERIFY_ERROR_SETTING");
		case Result::HIVE_RECEIPT_VERIFY_ERROR_HOST: return TEXT("HIVE_RECEIPT_VERIFY_ERROR_HOST");
		case Result::HIVE_RECEIPT_VERIFY_ERROR: return TEXT("HIVE_RECEIPT_VERIFY_ERROR");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE");
		case Result::HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE: return TEXT("HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_PARSE: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_PARSE");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN");
		case Result::HIVE_ERROR_RECEIPT_DIFF_PAYLOAD: return TEXT("HIVE_ERROR_RECEIPT_DIFF_PAYLOAD");
		case Result::HIVE_ERROR_RECEIPT_PAYLOAD_NONE: return TEXT("HIVE_ERROR_RECEIPT_PAYLOAD_NONE");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_SETTING: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_SETTING");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_HOST: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_HOST");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_PARSE: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_PARSE");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN");
		case Result::HIVE_ERROR_TIME_ZONE_RESULT_FAIL: return TEXT("HIVE_ERROR_TIME_ZONE_RESULT_FAIL");
		case Result::HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN: return TEXT("HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN");
		case Result::HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO: return TEXT("HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO");
		case Result::INVENTORY_EXTEND_ERROR_UNKNOWN: return TEXT("INVENTORY_EXTEND_ERROR_UNKNOWN");
		case Result::INVENTORY_EXTEND_ERROR_MAX_SLOT: return TEXT("INVENTORY_EXTEND_ERROR_MAX_SLOT");
		case Result::INVENTORY_EXTEND_ERROR_NOT_ENOUGH_MONEY: return TEXT("INVENTORY_EXTEND_ERROR_NOT_ENOUGH_MONEY");
		case Result::CHECK_USER_EXIST_ERROR_USER_NAME_EMPTY: return TEXT("CHECK_USER_EXIST_ERROR_USER_NAME_EMPTY");
		case Result::CHECK_USER_EXIST_ERROR_USER_NAME_LENGTH: return TEXT("CHECK_USER_EXIST_ERROR_USER_NAME_LENGTH");
		case Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST: return TEXT("CHECK_USER_EXIST_ERROR_USER_NOT_EXIST");
		case Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD: return TEXT("CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD");
		case Result::TITLE_ERROR_INVALID_EQUIP_SLOT: return TEXT("TITLE_ERROR_INVALID_EQUIP_SLOT");
		case Result::TITLE_ERROR_INVALID_TITLE_ID: return TEXT("TITLE_ERROR_INVALID_TITLE_ID");
		case Result::TITLE_ERROR_NOT_ACTIVED_TITLE: return TEXT("TITLE_ERROR_NOT_ACTIVED_TITLE");
		case Result::TITLE_ERROR_UNKNOWN: return TEXT("TITLE_ERROR_UNKNOWN");
		case Result::REGISTER_USER_UNLOCKINFO_ERROR_UNKNOWN: return TEXT("REGISTER_USER_UNLOCKINFO_ERROR_UNKNOWN");
		case Result::STATISTICS_ERROR_UNKNOWN: return TEXT("STATISTICS_ERROR_UNKNOWN");
		case Result::STATISTICS_ERROR_INVALID_STATUS: return TEXT("STATISTICS_ERROR_INVALID_STATUS");
		case Result::CAMP_ERROR_INVALID: return TEXT("CAMP_ERROR_INVALID");
		case Result::CAMP_ERROR_GUILD_GRADE: return TEXT("CAMP_ERROR_GUILD_GRADE");
		case Result::CAMP_ERROR_GUILD_BUILDING: return TEXT("CAMP_ERROR_GUILD_BUILDING");
		case Result::CAMP_ERROR_SAFE_TYPE_TERRITORY: return TEXT("CAMP_ERROR_SAFE_TYPE_TERRITORY");
		case Result::CAMP_ERROR_INVALID_MAP: return TEXT("CAMP_ERROR_INVALID_MAP");
		case Result::CAMP_ERROR_INVALID_AREA: return TEXT("CAMP_ERROR_INVALID_AREA");
		case Result::CAMP_ERROR_INVALID_ITEM: return TEXT("CAMP_ERROR_INVALID_ITEM");
		case Result::CAMP_ERROR_INVALID_NPC: return TEXT("CAMP_ERROR_INVALID_NPC");
		case Result::CAMP_ERROR_SPAWN_CAMP: return TEXT("CAMP_ERROR_SPAWN_CAMP");
		case Result::CAMP_ERROR_UNABLE_INTERACTION: return TEXT("CAMP_ERROR_UNABLE_INTERACTION");
		case Result::CAMP_ERROR_NEAR_CAMPS_EXIST: return TEXT("CAMP_ERROR_NEAR_CAMPS_EXIST");
		case Result::CAMP_ERROR_CAMP_DESTROYING: return TEXT("CAMP_ERROR_CAMP_DESTROYING");
		case Result::CAMP_ERROR_INSTALLATION_LIMIT_DISTANCE: return TEXT("CAMP_ERROR_INSTALLATION_LIMIT_DISTANCE");
		case Result::CAMP_ERROR_ALREADY_INSTALLED: return TEXT("CAMP_ERROR_ALREADY_INSTALLED");
		case Result::CAMP_ERROR_INVADE_WORLD: return TEXT("CAMP_ERROR_INVADE_WORLD");
		case Result::CAMP_ERROR_UNAUTHORIZED: return TEXT("CAMP_ERROR_UNAUTHORIZED");
		case Result::SANCTUM_ERROR_GUILD_GRADE: return TEXT("SANCTUM_ERROR_GUILD_GRADE");
		case Result::SANCTUM_ERROR_GUILD_BUILDING: return TEXT("SANCTUM_ERROR_GUILD_BUILDING");
		case Result::SANCTUM_ERROR_INVALID_MAP: return TEXT("SANCTUM_ERROR_INVALID_MAP");
		case Result::SANCTUM_ERROR_INVALID_AREA: return TEXT("SANCTUM_ERROR_INVALID_AREA");
		case Result::SANCTUM_ERROR_INVALID_NPC: return TEXT("SANCTUM_ERROR_INVALID_NPC");
		case Result::SANCTUM_ERROR_SPAWN_SANCTUM: return TEXT("SANCTUM_ERROR_SPAWN_SANCTUM");
		case Result::SANCTUM_ERROR_UNABLE_INTERACTION: return TEXT("SANCTUM_ERROR_UNABLE_INTERACTION");
		case Result::SANCTUM_ERROR_NEAR_SANCTUMS_EXIST: return TEXT("SANCTUM_ERROR_NEAR_SANCTUMS_EXIST");
		case Result::SANCTUM_ERROR_SANCTUM_DESTROYING: return TEXT("SANCTUM_ERROR_SANCTUM_DESTROYING");
		case Result::SANCTUM_ERROR_INVADE_WORLD: return TEXT("SANCTUM_ERROR_INVADE_WORLD");
		case Result::SANCTUM_ERROR_UNAUTHORIZED: return TEXT("SANCTUM_ERROR_UNAUTHORIZED");
		case Result::SANCTUM_ERROR_ALREADY_INSTALLED: return TEXT("SANCTUM_ERROR_ALREADY_INSTALLED");
		case Result::SANCTUM_ERROR_BATTLE_BEGIN: return TEXT("SANCTUM_ERROR_BATTLE_BEGIN");
		case Result::SANCTUM_ERROR_EXIST_NEXUS: return TEXT("SANCTUM_ERROR_EXIST_NEXUS");
		case Result::PROFILE_ERROR_UNKNOWN: return TEXT("PROFILE_ERROR_UNKNOWN");
		case Result::PROFILE_ERROR_INVALID_LENGTH_TEXT: return TEXT("PROFILE_ERROR_INVALID_LENGTH_TEXT");
		case Result::PROFILE_ERROR_INTRO_DB_FAIL: return TEXT("PROFILE_ERROR_INTRO_DB_FAIL");
		case Result::PROFILE_ERROR_NOT_ALLOWED_TEXT: return TEXT("PROFILE_ERROR_NOT_ALLOWED_TEXT");
		case Result::PROFILE_ERROR_INVALID_USER: return TEXT("PROFILE_ERROR_INVALID_USER");
		case Result::PROFILE_ERROR_DATA_CACHE_FAIL: return TEXT("PROFILE_ERROR_DATA_CACHE_FAIL");
		case Result::PROFILE_ERROR_COMMENT_DB_FAIL: return TEXT("PROFILE_ERROR_COMMENT_DB_FAIL");
		case Result::PROFILE_ERROR_COMMENT_DELETED: return TEXT("PROFILE_ERROR_COMMENT_DELETED");
		case Result::PROFILE_ERROR_COMMENT_FULL: return TEXT("PROFILE_ERROR_COMMENT_FULL");
		case Result::PROFILE_ERROR_COMMENT_INPUT_SELF: return TEXT("PROFILE_ERROR_COMMENT_INPUT_SELF");
		case Result::PROFILE_ERROR_COMMENT_DELETE_INVALID: return TEXT("PROFILE_ERROR_COMMENT_DELETE_INVALID");
		case Result::PROFILE_ERROR_INVALID_KEYWORD: return TEXT("PROFILE_ERROR_INVALID_KEYWORD");
		case Result::INVADE_ERROR_UNKNOWN: return TEXT("INVADE_ERROR_UNKNOWN");
		case Result::INVADE_ERROR_INVALID_TIME: return TEXT("INVADE_ERROR_INVALID_TIME");
		case Result::INVADE_ERROR_PORTAL_MAX_COUNT: return TEXT("INVADE_ERROR_PORTAL_MAX_COUNT");
		case Result::INVADE_ERROR_INVALID_LEVEL: return TEXT("INVADE_ERROR_INVALID_LEVEL");
		case Result::INVADE_ERROR_ENTER_MAX_COUNT: return TEXT("INVADE_ERROR_ENTER_MAX_COUNT");
		case Result::INVADE_ERROR_NOT_ENOUGH_TIME: return TEXT("INVADE_ERROR_NOT_ENOUGH_TIME");
		case Result::INVADE_ERROR_INVALID_LOAD_USER_DATA: return TEXT("INVADE_ERROR_INVALID_LOAD_USER_DATA");
		case Result::INVADE_ERROR_INVALID_SAVE_USER_DATA: return TEXT("INVADE_ERROR_INVALID_SAVE_USER_DATA");
		case Result::INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA: return TEXT("INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA");
		case Result::INVADE_ERROR_UNLOCK: return TEXT("INVADE_ERROR_UNLOCK");
		case Result::INVADE_ERROR_INVALID_MAP: return TEXT("INVADE_ERROR_INVALID_MAP");
		case Result::USER_CONTEXT_UI_ERROR_INVALID_PROFILE_USER: return TEXT("USER_CONTEXT_UI_ERROR_INVALID_PROFILE_USER");
		case Result::USER_CONTEXT_UI_ERROR_IS_GM_USER: return TEXT("USER_CONTEXT_UI_ERROR_IS_GM_USER");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_SETTING: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_SETTING");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_HOST_SETTING: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_HOST_SETTING");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_NO_APP_ID: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_NO_APP_ID");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_UNABLE_TO_CONNECT: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_UNABLE_TO_CONNECT");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_PARSE: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_PARSE");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_NO_RESULT: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_NO_RESULT");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_RESULT_ERROR: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_RESULT_ERROR");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_NO_REQUIRED_PARAMETER: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_NO_REQUIRED_PARAMETER");
		case Result::TUTORIAL_ERROR_INTERNAL_DB_ERROR: return TEXT("TUTORIAL_ERROR_INTERNAL_DB_ERROR");
		case Result::TUTORIAL_ERROR_INVALID_TUTORIAL_ID: return TEXT("TUTORIAL_ERROR_INVALID_TUTORIAL_ID");
		case Result::PARTY_DUNGEON_ERROR_UNKNOWN: return TEXT("PARTY_DUNGEON_ERROR_UNKNOWN");
		case Result::PARTY_DUNGEON_ERROR_ENTER_MAX_COUNT: return TEXT("PARTY_DUNGEON_ERROR_ENTER_MAX_COUNT");
		case Result::PARTY_DUNGEON_ERROR_INVALID_LOAD_USER_DATA: return TEXT("PARTY_DUNGEON_ERROR_INVALID_LOAD_USER_DATA");
		case Result::PARTY_DUNGEON_ERROR_INVALID_SAVE_USER_DATA: return TEXT("PARTY_DUNGEON_ERROR_INVALID_SAVE_USER_DATA");
		case Result::PARTY_DUNGEON_ERROR_NOT_FOUND_PARTY_DUNGEON_MAP_DATA: return TEXT("PARTY_DUNGEON_ERROR_NOT_FOUND_PARTY_DUNGEON_MAP_DATA");
		case Result::PARTY_DUNGEON_ERROR_UNLOCK: return TEXT("PARTY_DUNGEON_ERROR_UNLOCK");
		case Result::DUEL_ERROR_UNKNOWN: return TEXT("DUEL_ERROR_UNKNOWN");
		case Result::DUEL_ERROR_ALEREADY_INVITE_OTHER_USER: return TEXT("DUEL_ERROR_ALEREADY_INVITE_OTHER_USER");
		case Result::DUEL_ERROR_ALEREADY_PLAYING: return TEXT("DUEL_ERROR_ALEREADY_PLAYING");
		case Result::DUEL_ERROR_CANNOT_DUEL_TERRITORY: return TEXT("DUEL_ERROR_CANNOT_DUEL_TERRITORY");
		case Result::DUEL_ERROR_INVALID_USER: return TEXT("DUEL_ERROR_INVALID_USER");
		case Result::DUEL_ERROR_CANNOT_DUEL_TERRITORY_OTHER_USER: return TEXT("DUEL_ERROR_CANNOT_DUEL_TERRITORY_OTHER_USER");
		case Result::DUEL_ERROR_ALEREADY_INVITED_BY_OTHER_USER: return TEXT("DUEL_ERROR_ALEREADY_INVITED_BY_OTHER_USER");
		case Result::DUEL_ERROR_NOT_INVITE_USER: return TEXT("DUEL_ERROR_NOT_INVITE_USER");
		case Result::DUEL_ERROR_NOT_INVITED: return TEXT("DUEL_ERROR_NOT_INVITED");
		case Result::DUEL_ERROR_NOT_SAME_MAP: return TEXT("DUEL_ERROR_NOT_SAME_MAP");
		case Result::DUEL_ERROR_ARENA_ALREADY_PLAYING: return TEXT("DUEL_ERROR_ARENA_ALREADY_PLAYING");
		case Result::DUEL_ERROR_EXPIRED_INVITE_TIME: return TEXT("DUEL_ERROR_EXPIRED_INVITE_TIME");
		case Result::CHARACTER_SLOT_UNLOCK_ERROR_UNKNOWN: return TEXT("CHARACTER_SLOT_UNLOCK_ERROR_UNKNOWN");
		case Result::CHARACTER_SLOT_UNLOCK_ERROR_EXCEEDED: return TEXT("CHARACTER_SLOT_UNLOCK_ERROR_EXCEEDED");
		case Result::BATTLE_PASS_ERROR_UNKNOWN: return TEXT("BATTLE_PASS_ERROR_UNKNOWN");
		case Result::BATTLE_PASS_ERROR_BATTLE_PASS_NOT_EXISTS: return TEXT("BATTLE_PASS_ERROR_BATTLE_PASS_NOT_EXISTS");
		case Result::BATTLE_PASS_ERROR_NOT_MEET_THE_REWARD_CONDITION: return TEXT("BATTLE_PASS_ERROR_NOT_MEET_THE_REWARD_CONDITION");
		case Result::BATTLE_PASS_ERROR_ALREADY_REWARDED: return TEXT("BATTLE_PASS_ERROR_ALREADY_REWARDED");
		case Result::BATTLE_PASS_ERROR_TARGET_LEVEL_LESS_THAN_CURRENT_LEVEL: return TEXT("BATTLE_PASS_ERROR_TARGET_LEVEL_LESS_THAN_CURRENT_LEVEL");
		case Result::BATTLE_PASS_ERROR_EXCEED_MAX_LEVEL: return TEXT("BATTLE_PASS_ERROR_EXCEED_MAX_LEVEL");
		case Result::BATTLE_PASS_ERROR_BUY_LEVEL_CURRENCY_NOT_ENOUGH: return TEXT("BATTLE_PASS_ERROR_BUY_LEVEL_CURRENCY_NOT_ENOUGH");
		case Result::BATTLE_PASS_ERROR_ADDITONAL_REWARD_NOT_ACTIVATED: return TEXT("BATTLE_PASS_ERROR_ADDITONAL_REWARD_NOT_ACTIVATED");
		case Result::BATTLE_PASS_ERROR_ADDITONAL_REWARD_IS_ALREADY_ACTIVATED: return TEXT("BATTLE_PASS_ERROR_ADDITONAL_REWARD_IS_ALREADY_ACTIVATED");
		case Result::BATTLE_PASS_ERROR_ADDITONAL_ITEM_CONDITION_NOT_MEETED: return TEXT("BATTLE_PASS_ERROR_ADDITONAL_ITEM_CONDITION_NOT_MEETED");
		case Result::BATTLE_PASS_ERROR_ACTIVATE_REWARD_CURRENCY_NOT_ENOUGH: return TEXT("BATTLE_PASS_ERROR_ACTIVATE_REWARD_CURRENCY_NOT_ENOUGH");
		case Result::DELIVERY_BOX_ERROR_UNKNOWN: return TEXT("DELIVERY_BOX_ERROR_UNKNOWN");
		case Result::DELIVERY_BOX_ERROR_NOT_EVENT_TIME: return TEXT("DELIVERY_BOX_ERROR_NOT_EVENT_TIME");
		case Result::DELIVERY_BOX_ERROR_NOT_MEET_THE_EVENT_TYPE: return TEXT("DELIVERY_BOX_ERROR_NOT_MEET_THE_EVENT_TYPE");
		case Result::DELIVERY_BOX_ERROR_ALREADY_RECOVERED: return TEXT("DELIVERY_BOX_ERROR_ALREADY_RECOVERED");
		case Result::DELIVERY_BOX_ERROR_NOT_MEET_THE_CONDITION: return TEXT("DELIVERY_BOX_ERROR_NOT_MEET_THE_CONDITION");
		case Result::COMMON_EFFECT_ERROR_UNKNOWN: return TEXT("COMMON_EFFECT_ERROR_UNKNOWN");
		case Result::COMMON_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("COMMON_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::COMMON_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("COMMON_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::KEYBOARD_MAPPING_ERROR_SAVE: return TEXT("KEYBOARD_MAPPING_ERROR_SAVE");
		case Result::KEYBOARD_MAPPING_ERROR_LOAD: return TEXT("KEYBOARD_MAPPING_ERROR_LOAD");
		case Result::PK_BOOK_ERROR_UNKNOWN: return TEXT("PK_BOOK_ERROR_UNKNOWN");
		case Result::PK_BOOK_ERROR_INVALID_DATA: return TEXT("PK_BOOK_ERROR_INVALID_DATA");
		case Result::PK_BOOK_ERROR_GATEWAY_FAIL: return TEXT("PK_BOOK_ERROR_GATEWAY_FAIL");
		case Result::PK_BOOK_ERROR_NOT_DIE_DATA: return TEXT("PK_BOOK_ERROR_NOT_DIE_DATA");
		case Result::PK_BOOK_ERROR_NOT_KILL_DATA: return TEXT("PK_BOOK_ERROR_NOT_KILL_DATA");
		case Result::PK_BOOK_ERROR_NOT_REMAIN_ACTION_COUNT: return TEXT("PK_BOOK_ERROR_NOT_REMAIN_ACTION_COUNT");
		case Result::PK_BOOK_ERROR_EXPIRED_ACTION_TIME: return TEXT("PK_BOOK_ERROR_EXPIRED_ACTION_TIME");
		case Result::PK_BOOK_ERROR_DISABLE_ACTION_TO_NPC: return TEXT("PK_BOOK_ERROR_DISABLE_ACTION_TO_NPC");
		case Result::PK_BOOK_ERROR_NOT_WARPABLE_MAP: return TEXT("PK_BOOK_ERROR_NOT_WARPABLE_MAP");
		case Result::PK_BOOK_ERROR_NOT_LOGINED_USER: return TEXT("PK_BOOK_ERROR_NOT_LOGINED_USER");
		case Result::PK_BOOK_ERROR_NOT_ENOUGH_ACTION_CURRENCY: return TEXT("PK_BOOK_ERROR_NOT_ENOUGH_ACTION_CURRENCY");
		case Result::PK_BOOK_ERROR_PK_MESSAGE_INVALID_TEXT: return TEXT("PK_BOOK_ERROR_PK_MESSAGE_INVALID_TEXT");
		case Result::PK_BOOK_ERROR_MONITORRING_USER_LIST_FULL: return TEXT("PK_BOOK_ERROR_MONITORRING_USER_LIST_FULL");
		case Result::PK_BOOK_ERROR_ANOTER_SERVER_USER: return TEXT("PK_BOOK_ERROR_ANOTER_SERVER_USER");
		case Result::PK_BOOK_ERROR_NO_WAPABLE_TO_USER: return TEXT("PK_BOOK_ERROR_NO_WAPABLE_TO_USER");
		case Result::PK_BOOK_ERROR_INVALID_USER: return TEXT("PK_BOOK_ERROR_INVALID_USER");
		case Result::PK_BOOK_ERROR_ALREADY_MONITORRING_USER_ADDED: return TEXT("PK_BOOK_ERROR_ALREADY_MONITORRING_USER_ADDED");
		case Result::PK_BOOK_ERROR_CANNOT_ADD_MYSELF: return TEXT("PK_BOOK_ERROR_CANNOT_ADD_MYSELF");
		case Result::PK_BOOK_ERROR_CANNOT_ADD_GM: return TEXT("PK_BOOK_ERROR_CANNOT_ADD_GM");
		case Result::PK_BOOK_ERROR_CANNOT_ACTION_TO_USER: return TEXT("PK_BOOK_ERROR_CANNOT_ACTION_TO_USER");
		case Result::GMTOOL_ERROR_BEGIN: return TEXT("GMTOOL_ERROR_BEGIN");
		case Result::GMTOOL_ERROR_UNKNOWN: return TEXT("GMTOOL_ERROR_UNKNOWN");
		case Result::GMTOOL_CONTENTS_EVENT_REG_ERROR_DB: return TEXT("GMTOOL_CONTENTS_EVENT_REG_ERROR_DB");
		case Result::GMTOOL_CONTENTS_EVENT_REG_ALREADY: return TEXT("GMTOOL_CONTENTS_EVENT_REG_ALREADY");
		case Result::GMTOOL_CONTENTS_EVENT_REG_EXPIRED_DATE: return TEXT("GMTOOL_CONTENTS_EVENT_REG_EXPIRED_DATE");
		case Result::GMTOOL_CONTENTS_EVENT_REG_NOT_EXIST_DATA_SHEET: return TEXT("GMTOOL_CONTENTS_EVENT_REG_NOT_EXIST_DATA_SHEET");
		case Result::GMTOOL_CONTENTS_EVENT_REMOVE_NOT_REG: return TEXT("GMTOOL_CONTENTS_EVENT_REMOVE_NOT_REG");
		case Result::GMTOOL_CONTENTS_EVENT_REMOVE_ERROR_DB: return TEXT("GMTOOL_CONTENTS_EVENT_REMOVE_ERROR_DB");
		case Result::GMTOOL_GM_USER_CACHE_MISSING: return TEXT("GMTOOL_GM_USER_CACHE_MISSING");
		case Result::APP_GUARD_ERROR_INVALID_PLATFORM: return TEXT("APP_GUARD_ERROR_INVALID_PLATFORM");
		case Result::APP_GUARD_ERROR_INVALID_ACTIVE_MODE: return TEXT("APP_GUARD_ERROR_INVALID_ACTIVE_MODE");
		case Result::DELETE_ACCOUNT_HIVE_CONNECT_ERROR: return TEXT("DELETE_ACCOUNT_HIVE_CONNECT_ERROR");
		case Result::DELETE_ACCOUNT_ERROR: return TEXT("DELETE_ACCOUNT_ERROR");
		case Result::DELETE_ACCOUNT_DB_ERROR: return TEXT("DELETE_ACCOUNT_DB_ERROR");
		case Result::HIVE_DELETE_ACCOUNT_ERROR: return TEXT("HIVE_DELETE_ACCOUNT_ERROR");
		case Result::HIVE_DELETE_ACCOUNT_CERTIFICATION_KEY_ERROR: return TEXT("HIVE_DELETE_ACCOUNT_CERTIFICATION_KEY_ERROR");
		case Result::HIVE_DELETE_ACCOUNT_TOKEN_ERROR: return TEXT("HIVE_DELETE_ACCOUNT_TOKEN_ERROR");
		case Result::HIVE_DELETE_ACCOUNT_PARSE_ERROR: return TEXT("HIVE_DELETE_ACCOUNT_PARSE_ERROR");
		case Result::CREATE_USER_ERROR_DB_POSITION_ERROR: return TEXT("CREATE_USER_ERROR_DB_POSITION_ERROR");
		case Result::CHARACTER_CUSTOMIZING_ERROR_UNKNOWN: return TEXT("CHARACTER_CUSTOMIZING_ERROR_UNKNOWN");
		case Result::CHARACTER_CUSTOMIZING_ERROR_EXCEED_MAX_COUNT: return TEXT("CHARACTER_CUSTOMIZING_ERROR_EXCEED_MAX_COUNT");
		case Result::CHARACTER_CUSTOMIZING_ERROR_NOT_ENOUGH_COUNT: return TEXT("CHARACTER_CUSTOMIZING_ERROR_NOT_ENOUGH_COUNT");
		case Result::SOCIAL_EMOTION_ACTIVE: return TEXT("SOCIAL_EMOTION_ACTIVE");
		case Result::SEAL_ERROR_UNKNOWN: return TEXT("SEAL_ERROR_UNKNOWN");
		case Result::SEAL_ERROR_INVALID_STATUS: return TEXT("SEAL_ERROR_INVALID_STATUS");
		case Result::SEAL_ERROR_SLOT_LOCK_EXCEED_MAX: return TEXT("SEAL_ERROR_SLOT_LOCK_EXCEED_MAX");
		case Result::SEAL_ERROR_NOT_ENOUGH_LEVEL: return TEXT("SEAL_ERROR_NOT_ENOUGH_LEVEL");
		case Result::SEAL_ERROR_SLOT_NOT_USABLE_LEVEL: return TEXT("SEAL_ERROR_SLOT_NOT_USABLE_LEVEL");
		case Result::SEAL_ERROR_LEVEL_EXCEED_MAX: return TEXT("SEAL_ERROR_LEVEL_EXCEED_MAX");
		case Result::SEAL_ERROR_INVALID_STAT_POOL_ID: return TEXT("SEAL_ERROR_INVALID_STAT_POOL_ID");
		case Result::QUEST_ERROR_REQ_GUILD_QUEST_INFO: return TEXT("QUEST_ERROR_REQ_GUILD_QUEST_INFO");
		case Result::QUEST_ERROR_GUILD_QUEST_CONFIG_DATA: return TEXT("QUEST_ERROR_GUILD_QUEST_CONFIG_DATA");
		case Result::QUEST_ERROR_USER_IS_NOT_GUILD_MEMBER: return TEXT("QUEST_ERROR_USER_IS_NOT_GUILD_MEMBER");
		case Result::QUEST_ERROR_NOT_ENOUGH_GUILD_LEVEL: return TEXT("QUEST_ERROR_NOT_ENOUGH_GUILD_LEVEL");
		case Result::QUEST_ERROR_GUILD_QUEST_GENERATE: return TEXT("QUEST_ERROR_GUILD_QUEST_GENERATE");
		case Result::QUEST_ERROR_GUILD_QUEST_GENERATE_IN_DB: return TEXT("QUEST_ERROR_GUILD_QUEST_GENERATE_IN_DB");
		case Result::QUEST_ERROR_INVALID_USER_STATUS: return TEXT("QUEST_ERROR_INVALID_USER_STATUS");
		case Result::QUEST_ERROR_NOT_FOUND_GUILD_QUEST_ID_IN_DATA_SHEET: return TEXT("QUEST_ERROR_NOT_FOUND_GUILD_QUEST_ID_IN_DATA_SHEET");
		case Result::QUEST_ERROR_NOT_FOUND_GUILD_QUEST_INFO_IN_DATA_SHEET: return TEXT("QUEST_ERROR_NOT_FOUND_GUILD_QUEST_INFO_IN_DATA_SHEET");
		case Result::QUEST_ERROR_NOT_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_GUILD_QUEST");
		case Result::QUEST_ERROR_ALREADY_ACCEPT_GUILD_QUEST: return TEXT("QUEST_ERROR_ALREADY_ACCEPT_GUILD_QUEST");
		case Result::QUEST_ERROR_ALREADY_ACCEPTED_GUILD_QUEST: return TEXT("QUEST_ERROR_ALREADY_ACCEPTED_GUILD_QUEST");
		case Result::QUEST_ERROR_GUILD_NOT_ACCEPTABLE_TODAY: return TEXT("QUEST_ERROR_GUILD_NOT_ACCEPTABLE_TODAY");
		case Result::QUEST_ERROR_GUILD_EXCEED_ACCEPT_COUNT: return TEXT("QUEST_ERROR_GUILD_EXCEED_ACCEPT_COUNT");
		case Result::QUEST_ERROR_INVALID_REWARD_GUILD_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_REWARD_GUILD_QUEST_ID");
		case Result::QUEST_ERROR_NOT_LAST_REWARD_GUILD_QUEST_ID: return TEXT("QUEST_ERROR_NOT_LAST_REWARD_GUILD_QUEST_ID");
		case Result::QUEST_ERROR_ALREADY_REWARDED_GUILD_QUEST_ID: return TEXT("QUEST_ERROR_ALREADY_REWARDED_GUILD_QUEST_ID");
		case Result::QUEST_ERROR_NOT_COMPLETED_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_COMPLETED_GUILD_QUEST");
		case Result::QUEST_ERROR_INVALID_GUILD_QUEST: return TEXT("QUEST_ERROR_INVALID_GUILD_QUEST");
		case Result::QUEST_ERROR_NOT_ACCEPTED_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_ACCEPTED_GUILD_QUEST");
		case Result::QUEST_ERROR_EXCEED_GUILD_QUEST_REFRESH_COUNT: return TEXT("QUEST_ERROR_EXCEED_GUILD_QUEST_REFRESH_COUNT");
		case Result::QUEST_ERROR_NOT_ENOUGH_GUILD_QUEST_REFRESH_CURRENCY: return TEXT("QUEST_ERROR_NOT_ENOUGH_GUILD_QUEST_REFRESH_CURRENCY");
		case Result::QUEST_ERROR_NOT_FOUND_GUILD_QUEST_REFRESH_INFO: return TEXT("QUEST_ERROR_NOT_FOUND_GUILD_QUEST_REFRESH_INFO");
		case Result::QUEST_ERROR_NO_REFRESHABLE_GUILD_QUEST: return TEXT("QUEST_ERROR_NO_REFRESHABLE_GUILD_QUEST");
		case Result::QUEST_ERROR_EXPIRED_GUILD_QUEST: return TEXT("QUEST_ERROR_EXPIRED_GUILD_QUEST");
		case Result::QUEST_ERROR_GUILD_NOT_ACCEPTABLE_QUEST: return TEXT("QUEST_ERROR_GUILD_NOT_ACCEPTABLE_QUEST");
		case Result::QUEST_ERROR_GUILD_REWARD_ITEM_CREATE: return TEXT("QUEST_ERROR_GUILD_REWARD_ITEM_CREATE");
		case Result::QUEST_ERROR_GUILD_NOT_REWARDABLE_QUEST: return TEXT("QUEST_ERROR_GUILD_NOT_REWARDABLE_QUEST");
		case Result::QUEST_ERROR_GUILD_ACCEPTED_QUEST: return TEXT("QUEST_ERROR_GUILD_ACCEPTED_QUEST");
		case Result::QUEST_ERROR_GUILD_REFRESH: return TEXT("QUEST_ERROR_GUILD_REFRESH");
		case Result::QUEST_ERROR_GUILD_REFRESH_REQ_DB: return TEXT("QUEST_ERROR_GUILD_REFRESH_REQ_DB");
		case Result::QUEST_ERROR_GUILD_EXCEED_COMPLETE_COUNT: return TEXT("QUEST_ERROR_GUILD_EXCEED_COMPLETE_COUNT");
		case Result::PERSONAL_TRADING_INVALID_USER_STATUS: return TEXT("PERSONAL_TRADING_INVALID_USER_STATUS");
		case Result::PERSONAL_TRADING_NOT_FOUND_USER: return TEXT("PERSONAL_TRADING_NOT_FOUND_USER");
		case Result::PERSONAL_TRADING_IMPOSSIBLE_USER: return TEXT("PERSONAL_TRADING_IMPOSSIBLE_USER");
		case Result::PERSONAL_TRADING_ERROR_INVALID_RANGE: return TEXT("PERSONAL_TRADING_ERROR_INVALID_RANGE");
		case Result::PERSONAL_TRADING_INVALID_OTHER_USER_STATUS: return TEXT("PERSONAL_TRADING_INVALID_OTHER_USER_STATUS");
		case Result::PERSONAL_TRADING_DONT_ACTION: return TEXT("PERSONAL_TRADING_DONT_ACTION");
		case Result::PERSONAL_TRADING_ERROR_OHTER_TRADING: return TEXT("PERSONAL_TRADING_ERROR_OHTER_TRADING");
		case Result::PERSONAL_TRADING_ERROR_TRAGET_USER_OHTER_TRADING: return TEXT("PERSONAL_TRADING_ERROR_TRAGET_USER_OHTER_TRADING");
		case Result::PERSONAL_TRADING_ERROR_SAME_POS: return TEXT("PERSONAL_TRADING_ERROR_SAME_POS");
		case Result::PERSONAL_TRADING_ERROR_TRY_CONFIRM: return TEXT("PERSONAL_TRADING_ERROR_TRY_CONFIRM");
		case Result::PERSONAL_TRADING_ERROR_NOT_TRADING: return TEXT("PERSONAL_TRADING_ERROR_NOT_TRADING");
		case Result::PERSONAL_TRADING_ERROR_ITEM_ADD: return TEXT("PERSONAL_TRADING_ERROR_ITEM_ADD");
		case Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_DIA: return TEXT("PERSONAL_TRADING_ERROR_NOT_ENOUGH_DIA");
		case Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_DIA: return TEXT("PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_DIA");
		case Result::PERSONAL_TRADING_ERROR_NOT_DIA_FULLY: return TEXT("PERSONAL_TRADING_ERROR_NOT_DIA_FULLY");
		case Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_INVENTORY_SLOT: return TEXT("PERSONAL_TRADING_ERROR_NOT_ENOUGH_INVENTORY_SLOT");
		case Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_INVENTORY_SLOT: return TEXT("PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_INVENTORY_SLOT");
		case Result::PERSONAL_TRADING_ERROR_WEIGHT_FULL: return TEXT("PERSONAL_TRADING_ERROR_WEIGHT_FULL");
		case Result::PERSONAL_TRADING_ERROR_TARGET_WEIGHT_FULL: return TEXT("PERSONAL_TRADING_ERROR_TARGET_WEIGHT_FULL");
		case Result::PERSONAL_TRADING_ERROR_TIME_OUT: return TEXT("PERSONAL_TRADING_ERROR_TIME_OUT");
		case Result::PERSONAL_TRADING_ERROR_LEVEL: return TEXT("PERSONAL_TRADING_ERROR_LEVEL");
		case Result::PERSONAL_TRADING_ERROR_TARGET_LEVEL: return TEXT("PERSONAL_TRADING_ERROR_TARGET_LEVEL");
		case Result::PERSONAL_TRADING_ERROR_TRY_COUNT: return TEXT("PERSONAL_TRADING_ERROR_TRY_COUNT");
		case Result::PERSONAL_TRADING_ERROR_TRAGET_TRY_COUNT: return TEXT("PERSONAL_TRADING_ERROR_TRAGET_TRY_COUNT");
		case Result::PERSONAL_TRADING_ERROR_UNKNOWN: return TEXT("PERSONAL_TRADING_ERROR_UNKNOWN");
		case Result::PERSONAL_TRADING_ERROR_INVALID_ITEM: return TEXT("PERSONAL_TRADING_ERROR_INVALID_ITEM");
		case Result::PERSONAL_TRADING_ALREADY_TRADING: return TEXT("PERSONAL_TRADING_ALREADY_TRADING");
		case Result::PERSONAL_TRADING_ERROR_USABLE_IN_SAFE_ZONE: return TEXT("PERSONAL_TRADING_ERROR_USABLE_IN_SAFE_ZONE");
		case Result::PERSONAL_TRADING_ERROR_CANNOT_USABLE_GM: return TEXT("PERSONAL_TRADING_ERROR_CANNOT_USABLE_GM");
		case Result::PERSONAL_TRADING_ERROR_OTHER_USABLE_IN_SAFE_ZONE: return TEXT("PERSONAL_TRADING_ERROR_OTHER_USABLE_IN_SAFE_ZONE");
		case Result::PERSONAL_TRADING_ERROR_NO_DEVICE_REGISTERED: return TEXT("PERSONAL_TRADING_ERROR_NO_DEVICE_REGISTERED");
		case Result::COMMUNITY_EVENT_MISSION_INVALID_USER_STATUS: return TEXT("COMMUNITY_EVENT_MISSION_INVALID_USER_STATUS");
		case Result::COMMUNITY_EVENT_MISSION_INVALID_DATA: return TEXT("COMMUNITY_EVENT_MISSION_INVALID_DATA");
		case Result::COMMUNITY_EVENT_MISSION_NOT_EVENT_PERIOD: return TEXT("COMMUNITY_EVENT_MISSION_NOT_EVENT_PERIOD");
		case Result::COMMUNITY_EVENT_MISSION_NOT_TAB_OPEN_PERIOD: return TEXT("COMMUNITY_EVENT_MISSION_NOT_TAB_OPEN_PERIOD");
		case Result::COMMUNITY_EVENT_MISSION_NOT_EXIST_MISSION_DATA: return TEXT("COMMUNITY_EVENT_MISSION_NOT_EXIST_MISSION_DATA");
		case Result::COMMUNITY_EVENT_MISSION_NOT_MEET_THE_CONDITION_MISSION_COUNT: return TEXT("COMMUNITY_EVENT_MISSION_NOT_MEET_THE_CONDITION_MISSION_COUNT");
		case Result::COMMUNITY_EVENT_MISSION_IS_ALREADY_REWARDED_MISSION: return TEXT("COMMUNITY_EVENT_MISSION_IS_ALREADY_REWARDED_MISSION");
		case Result::GUILD_AUCTION_ERROR_INVALID_ID: return TEXT("GUILD_AUCTION_ERROR_INVALID_ID");
		case Result::GUILD_AUCTION_ERROR_EXPIRED_DATE_TIME: return TEXT("GUILD_AUCTION_ERROR_EXPIRED_DATE_TIME");
		case Result::GUILD_AUCTION_ERROR_DIVISION_STATE: return TEXT("GUILD_AUCTION_ERROR_DIVISION_STATE");
		case Result::GUILD_AUCTION_ERROR_ENOUGH_CURRENCY: return TEXT("GUILD_AUCTION_ERROR_ENOUGH_CURRENCY");
		case Result::GUILD_AUCTION_ERROR_APPOINTMENT: return TEXT("GUILD_AUCTION_ERROR_APPOINTMENT");
		case Result::GUILD_AUCTION_ERROR_PARTICIPANTS: return TEXT("GUILD_AUCTION_ERROR_PARTICIPANTS");
		case Result::GUILD_AUCTION_ERROR_DB: return TEXT("GUILD_AUCTION_ERROR_DB");
		case Result::GUILD_AUCTION_ERROR_BUY_STATE: return TEXT("GUILD_AUCTION_ERROR_BUY_STATE");
		case Result::GUILD_AUCTION_ERROR_RETURN_CURRENCY: return TEXT("GUILD_AUCTION_ERROR_RETURN_CURRENCY");
		case Result::GUILD_AUCTION_ERROR_DUPLICATION_REGISTER: return TEXT("GUILD_AUCTION_ERROR_DUPLICATION_REGISTER");
		case Result::GUILD_LEAVE_ERROR_AUCTION_DATA: return TEXT("GUILD_LEAVE_ERROR_AUCTION_DATA");
		case Result::GUILD_AUCTION_ERROR_JOINED_TIME: return TEXT("GUILD_AUCTION_ERROR_JOINED_TIME");
		case Result::GUILD_AUCTION_ERROR_FULL_REGISTRATION: return TEXT("GUILD_AUCTION_ERROR_FULL_REGISTRATION");
		case Result::GUILD_AUCTION_ERROR_HIGHER_BID: return TEXT("GUILD_AUCTION_ERROR_HIGHER_BID");
		case Result::GUILD_AUCTION_ERROR_EMPTY_ITEM: return TEXT("GUILD_AUCTION_ERROR_EMPTY_ITEM");
		case Result::GUILD_AUCTION_ERROR_SAFE_CAPACITY_FULL: return TEXT("GUILD_AUCTION_ERROR_SAFE_CAPACITY_FULL");
		case Result::GUILD_SNS_URL_ERROR_INVALID_ID: return TEXT("GUILD_SNS_URL_ERROR_INVALID_ID");
		case Result::GUILD_SNS_URL_ERROR_INVALID_LEN: return TEXT("GUILD_SNS_URL_ERROR_INVALID_LEN");
		case Result::NICKNAME_CHANGE_ERROR_NOT_ALLOWED_NAME: return TEXT("NICKNAME_CHANGE_ERROR_NOT_ALLOWED_NAME");
		case Result::NICKNAME_CHANGE_ERROR_INVALID_LENGTH_NAME: return TEXT("NICKNAME_CHANGE_ERROR_INVALID_LENGTH_NAME");
		case Result::NICKNAME_CHANGE_ERROR_DUPLICATION_NAME: return TEXT("NICKNAME_CHANGE_ERROR_DUPLICATION_NAME");
		case Result::NICKNAME_CHANGE_ERROR_NOT_TOWN: return TEXT("NICKNAME_CHANGE_ERROR_NOT_TOWN");
		case Result::NICKNAME_CHANGE_ERROR_GUILD_APPLICANTS: return TEXT("NICKNAME_CHANGE_ERROR_GUILD_APPLICANTS");
		case Result::NICKNAME_CHANGE_ERROR_DB: return TEXT("NICKNAME_CHANGE_ERROR_DB");
		case Result::INVADE_ERROR_PORTAL_SPAWNING: return TEXT("INVADE_ERROR_PORTAL_SPAWNING");
		case Result::SERVER_MIGRATE_ERROR_UNKNOWN_ERROR: return TEXT("SERVER_MIGRATE_ERROR_UNKNOWN_ERROR");
		case Result::SERVER_MIGRATE_ERROR_INVALID_USER_STATUS: return TEXT("SERVER_MIGRATE_ERROR_INVALID_USER_STATUS");
		case Result::SERVER_MIGRATE_ERROR_NOT_MEET_THE_CONDITION: return TEXT("SERVER_MIGRATE_ERROR_NOT_MEET_THE_CONDITION");
		case Result::SERVER_MIGRATE_ERROR_ALREADY_IN_PROCESS: return TEXT("SERVER_MIGRATE_ERROR_ALREADY_IN_PROCESS");
		case Result::SERVER_MIGRATE_ERROR_SERVER_SESSION_IS_INVALID: return TEXT("SERVER_MIGRATE_ERROR_SERVER_SESSION_IS_INVALID");
		case Result::SERVER_MIGRATE_ERROR_DB_QUERY_FAILED: return TEXT("SERVER_MIGRATE_ERROR_DB_QUERY_FAILED");
		case Result::SERVER_MIGRATE_ERROR_DATA_TRANSFER_IN_PROCESS: return TEXT("SERVER_MIGRATE_ERROR_DATA_TRANSFER_IN_PROCESS");
		case Result::SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_ACCOUNT: return TEXT("SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_ACCOUNT");
		case Result::SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_USER: return TEXT("SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_USER");
		case Result::SERVER_MIGRATE_ERROR_CHECK_CONDITION_GUILD_MEMBER: return TEXT("SERVER_MIGRATE_ERROR_CHECK_CONDITION_GUILD_MEMBER");
		case Result::SERVER_MIGRATE_ERROR_NICKNAME_ALREADY_CHANGED: return TEXT("SERVER_MIGRATE_ERROR_NICKNAME_ALREADY_CHANGED");
		case Result::SERVER_MIGRATE_ERROR_GUILD_NAME_ALREADY_CHANGED: return TEXT("SERVER_MIGRATE_ERROR_GUILD_NAME_ALREADY_CHANGED");
		case Result::SERVER_MIGRATE_ERROR_TARGET_SERVER_EXCEED_INCOMING_COUNT: return TEXT("SERVER_MIGRATE_ERROR_TARGET_SERVER_EXCEED_INCOMING_COUNT");
		case Result::SERVER_MIGRATE_ERROR_CURRENT_SERVER_EXCEED_OUTGOING_COUNT: return TEXT("SERVER_MIGRATE_ERROR_CURRENT_SERVER_EXCEED_OUTGOING_COUNT");
		case Result::SERVER_MIGRATE_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("SERVER_MIGRATE_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::ABUSE_PACKET_SHORT_TICK: return TEXT("ABUSE_PACKET_SHORT_TICK");
		case Result::GUILD_WAR_UNKNOWN_ERROR: return TEXT("GUILD_WAR_UNKNOWN_ERROR");
		case Result::GUILD_WAR_MATCH_NOT_FOUND_ENEMY_GUILD: return TEXT("GUILD_WAR_MATCH_NOT_FOUND_ENEMY_GUILD");
		case Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MINIMUM_LIMIT: return TEXT("GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MINIMUM_LIMIT");
		case Result::GUILD_WAR_ENTER_FAIL_NOT_REGISTERED_IN_ENTRY: return TEXT("GUILD_WAR_ENTER_FAIL_NOT_REGISTERED_IN_ENTRY");
		case Result::GUILD_WAR_ENTER_FAIL_INVALID_STATE: return TEXT("GUILD_WAR_ENTER_FAIL_INVALID_STATE");
		case Result::GUILD_WAR_MATCH_REGISTER_FAIL_ALREADY_REGISTER: return TEXT("GUILD_WAR_MATCH_REGISTER_FAIL_ALREADY_REGISTER");
		case Result::GUILD_WAR_MATCH_REGISTER_FAIL_STATE: return TEXT("GUILD_WAR_MATCH_REGISTER_FAIL_STATE");
		case Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MAX_LIMIT: return TEXT("GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MAX_LIMIT");
		case Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_GRADE_LIMIT: return TEXT("GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_GRADE_LIMIT");
		case Result::GUILD_WAR_MATCH_REGISTER_FAIL_LEVEL: return TEXT("GUILD_WAR_MATCH_REGISTER_FAIL_LEVEL");
		case Result::GUILD_WAR_ERROR_THE_DAY_JOINED_GUILD: return TEXT("GUILD_WAR_ERROR_THE_DAY_JOINED_GUILD");
		case Result::AGIT_DUNGEON_RAID_UNKNOWN_ERROR: return TEXT("AGIT_DUNGEON_RAID_UNKNOWN_ERROR");
		case Result::AGIT_DUNGEON_RAID_WORLD_UNKNOWN_ERROR: return TEXT("AGIT_DUNGEON_RAID_WORLD_UNKNOWN_ERROR");
		case Result::AGIT_DUNGEON_RAID_ACCOUNT_DATA: return TEXT("AGIT_DUNGEON_RAID_ACCOUNT_DATA");
		case Result::AGIT_DUNGEON_RAID_NOT_YET_RESET: return TEXT("AGIT_DUNGEON_RAID_NOT_YET_RESET");
		case Result::AGIT_DUNGEON_RAID_NOT_EXIST_DETAIL_DATA: return TEXT("AGIT_DUNGEON_RAID_NOT_EXIST_DETAIL_DATA");
		case Result::AGIT_DUNGEON_RAID_INVALID_USER_STATE: return TEXT("AGIT_DUNGEON_RAID_INVALID_USER_STATE");
		case Result::AGIT_DUNGEON_RAID_INVALID_PARRAM: return TEXT("AGIT_DUNGEON_RAID_INVALID_PARRAM");
		case Result::AGIT_DUNGEON_RAID_NOT_GUILD_MEMBER: return TEXT("AGIT_DUNGEON_RAID_NOT_GUILD_MEMBER");
		case Result::AGIT_DUNGEON_RAID_OPEN_GRADE_ERROR: return TEXT("AGIT_DUNGEON_RAID_OPEN_GRADE_ERROR");
		case Result::AGIT_DUNGEON_RAID_CONFIG_DATA_ERROR: return TEXT("AGIT_DUNGEON_RAID_CONFIG_DATA_ERROR");
		case Result::AGIT_DUNGEON_RAID_CONTENTS_GUILD_LEVEL_ERROR: return TEXT("AGIT_DUNGEON_RAID_CONTENTS_GUILD_LEVEL_ERROR");
		case Result::AGIT_DUNGEON_RAID_GUILD_LEVEL_ERROR: return TEXT("AGIT_DUNGEON_RAID_GUILD_LEVEL_ERROR");
		case Result::AGIT_DUNGEON_RAID_DETAIL_DATA_ERROR: return TEXT("AGIT_DUNGEON_RAID_DETAIL_DATA_ERROR");
		case Result::AGIT_DUNGEON_RAID_ALREADY_OPENED: return TEXT("AGIT_DUNGEON_RAID_ALREADY_OPENED");
		case Result::AGIT_DUNGEON_RAID_NO_MAP_DATA: return TEXT("AGIT_DUNGEON_RAID_NO_MAP_DATA");
		case Result::AGIT_DUNGEON_RAID_NOT_ENOUGH_GUILD_ASSET: return TEXT("AGIT_DUNGEON_RAID_NOT_ENOUGH_GUILD_ASSET");
		case Result::AGIT_DUNGEON_RAID_FAIL_TO_CREATE_CHANNEL: return TEXT("AGIT_DUNGEON_RAID_FAIL_TO_CREATE_CHANNEL");
		case Result::AGIT_DUNGEON_RAID_NO_USER_DATA: return TEXT("AGIT_DUNGEON_RAID_NO_USER_DATA");
		case Result::AGIT_DUNGEON_RAID_GUILD_JOINED_DAY_ERROR: return TEXT("AGIT_DUNGEON_RAID_GUILD_JOINED_DAY_ERROR");
		case Result::AGIT_DUNGEON_RAID_MAX_CLEAR_COUNT: return TEXT("AGIT_DUNGEON_RAID_MAX_CLEAR_COUNT");
		case Result::AGIT_DUNGEON_RAID_NOT_AVAILABLE_TIME: return TEXT("AGIT_DUNGEON_RAID_NOT_AVAILABLE_TIME");
		case Result::AGIT_DUNGEON_RAID_DUNGEON_NOT_CLOSED: return TEXT("AGIT_DUNGEON_RAID_DUNGEON_NOT_CLOSED");
		case Result::AGIT_DUNGEON_RAID_DB_OPERATION_ERROR: return TEXT("AGIT_DUNGEON_RAID_DB_OPERATION_ERROR");
		case Result::AGIT_DUNGEON_RAID_NO_MAP_TO_CLOSE: return TEXT("AGIT_DUNGEON_RAID_NO_MAP_TO_CLOSE");
		case Result::AGIT_DUNGEON_RAID_NO_CHANNEL_TO_CLOSE: return TEXT("AGIT_DUNGEON_RAID_NO_CHANNEL_TO_CLOSE");
		case Result::AGIT_DUNGEON_RAID_FAIL_TO_CREATE_RAID_DATA: return TEXT("AGIT_DUNGEON_RAID_FAIL_TO_CREATE_RAID_DATA");
		case Result::AGIT_DUNGEON_RAID_NOT_EXIST_RAID_DATA: return TEXT("AGIT_DUNGEON_RAID_NOT_EXIST_RAID_DATA");
		case Result::AGIT_DUNGEON_RAID_DUNGEON_NOT_OPENED: return TEXT("AGIT_DUNGEON_RAID_DUNGEON_NOT_OPENED");
		case Result::AGIT_DUNGEON_RAID_NOT_EXIST_USER_CHANNEL: return TEXT("AGIT_DUNGEON_RAID_NOT_EXIST_USER_CHANNEL");
		case Result::AGIT_DUNGEON_RAID_MAP_TYPE_ERROR: return TEXT("AGIT_DUNGEON_RAID_MAP_TYPE_ERROR");
		case Result::AGIT_DUNGEON_NO_CHANNEL_TO_ENTER: return TEXT("AGIT_DUNGEON_NO_CHANNEL_TO_ENTER");
		case Result::AGIT_DUNGEON_RAID_WARP_ERROR_ABNORMALITY_EFFECT: return TEXT("AGIT_DUNGEON_RAID_WARP_ERROR_ABNORMALITY_EFFECT");
		case Result::AGIT_DUNGEON_RAID_NOT_ENOUGH_USER_TICKET_COUNT: return TEXT("AGIT_DUNGEON_RAID_NOT_ENOUGH_USER_TICKET_COUNT");
		case Result::AGIT_DUNGEON_RAID_NOT_FOUND_GUILD: return TEXT("AGIT_DUNGEON_RAID_NOT_FOUND_GUILD");
		case Result::AGIT_DUNGEON_RAID_DESTROYING: return TEXT("AGIT_DUNGEON_RAID_DESTROYING");
		case Result::AGIT_DUNGEON_RAID_FAIL_TO_CLOSE_USER_EXISTS: return TEXT("AGIT_DUNGEON_RAID_FAIL_TO_CLOSE_USER_EXISTS");
		case Result::CHAT_GUILD_GROUP_INVALID_ROOM_NAME: return TEXT("CHAT_GUILD_GROUP_INVALID_ROOM_NAME");
		case Result::CHAT_GUILD_GROUP_JOIN_OVER_LIMIT: return TEXT("CHAT_GUILD_GROUP_JOIN_OVER_LIMIT");
		case Result::CHAT_GUILD_GROUP_MEMBER_OVER_LIMIT: return TEXT("CHAT_GUILD_GROUP_MEMBER_OVER_LIMIT");
		case Result::CHAT_GUILD_GROUP_INVITE_OVER_LIMIT: return TEXT("CHAT_GUILD_GROUP_INVITE_OVER_LIMIT");
		case Result::CHAT_GUILD_GROUP_INVALID_ROOM_ID: return TEXT("CHAT_GUILD_GROUP_INVALID_ROOM_ID");
		case Result::CHAT_GUILD_GROUP_ALREADY_INVITED: return TEXT("CHAT_GUILD_GROUP_ALREADY_INVITED");
		case Result::CHAT_GUILD_GROUP_NOT_INVITED: return TEXT("CHAT_GUILD_GROUP_NOT_INVITED");
		case Result::CHAT_GUILD_GROUP_ALREADY_JOINED: return TEXT("CHAT_GUILD_GROUP_ALREADY_JOINED");
		case Result::CHAT_GUILD_GROUP_DB_FAILED: return TEXT("CHAT_GUILD_GROUP_DB_FAILED");
		case Result::CHAT_GUILD_GROUP_NOT_LEADER_GUILD: return TEXT("CHAT_GUILD_GROUP_NOT_LEADER_GUILD");
		case Result::CHAT_GUILD_GROUP_NOT_JOIN_GUILD: return TEXT("CHAT_GUILD_GROUP_NOT_JOIN_GUILD");
		case Result::CHAT_GUILD_GROUP_DO_NOT_WITHDRAWAL: return TEXT("CHAT_GUILD_GROUP_DO_NOT_WITHDRAWAL");
		case Result::CHAT_GUILD_GROUP_DO_WRONG_GUILD_ID: return TEXT("CHAT_GUILD_GROUP_DO_WRONG_GUILD_ID");
		case Result::CHAT_GUILD_GROUP_DATA_DIFF: return TEXT("CHAT_GUILD_GROUP_DATA_DIFF");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString ResultEnumToDisplayKey(Result value)
{
	switch (value)
	{
		case Result::RESULT_SUCCESS: return TEXT("RESULT_SUCCESS");
		case Result::LOGIN_ERROR_RETRY: return TEXT("LOGIN_ERROR_RETRY");
		case Result::LOGIN_ERROR_RESTART: return TEXT("LOGIN_ERROR_RESTART");
		case Result::LOGIN_ERROR_UNKNOWN: return TEXT("LOGIN_ERROR_UNKNOWN");
		case Result::LOGIN_ERROR_WRONG_WZ_PACKET_VERSION: return TEXT("LOGIN_ERROR_WRONG_WZ_PACKET_VERSION");
		case Result::LOGIN_ERROR_INTERNAL_SERVER_ERROR: return TEXT("LOGIN_ERROR_INTERNAL_SERVER_ERROR");
		case Result::LOGIN_ERROR_INTERNAL_DB_ERROR: return TEXT("LOGIN_ERROR_INTERNAL_DB_ERROR");
		case Result::LOGIN_ERROR_NEED_UPDATE_ACCOUNT_NAME: return TEXT("LOGIN_ERROR_NEED_UPDATE_ACCOUNT_NAME");
		case Result::LOGIN_ERROR_ID_PW_AUTH_FAILED: return TEXT("LOGIN_ERROR_ID_PW_AUTH_FAILED");
		case Result::LOGIN_ERROR_INVALID_AUTHORIZATION: return TEXT("LOGIN_ERROR_INVALID_AUTHORIZATION");
		case Result::LOGIN_ERROR_GETTING_READY_FOR_GATEWAYSERVER: return TEXT("LOGIN_ERROR_GETTING_READY_FOR_GATEWAYSERVER");
		case Result::LOGIN_ERROR_INVALID_ACCOUNT_ID: return TEXT("LOGIN_ERROR_INVALID_ACCOUNT_ID");
		case Result::LOGIN_ERROR_INVALID_VID: return TEXT("LOGIN_ERROR_INVALID_VID");
		case Result::LOGIN_ERROR_INCORRECT_TOKEN: return TEXT("LOGIN_ERROR_INCORRECT_TOKEN");
		case Result::LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB: return TEXT("LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB");
		case Result::LOGIN_ERROR_TIME_OUT: return TEXT("LOGIN_ERROR_TIME_OUT");
		case Result::LOGIN_ERROR_INVALID_AUTH_STATUS: return TEXT("LOGIN_ERROR_INVALID_AUTH_STATUS");
		case Result::LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN: return TEXT("LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN");
		case Result::LOGIN_ERROR_SERVER_MAINTENANCE: return TEXT("LOGIN_ERROR_SERVER_MAINTENANCE");
		case Result::LOGIN_ERROR_NOT_MATCHED_VID_PLANETWORLD_ID: return TEXT("LOGIN_ERROR_NOT_MATCHED_VID_PLANETWORLD_ID");
		case Result::LOGIN_ERROR_HIVE_VERIFY_TOKEN_SETTING: return TEXT("LOGIN_ERROR_HIVE_VERIFY_TOKEN_SETTING");
		case Result::LOGIN_ERROR_DELETED_ACCOUNT: return TEXT("LOGIN_ERROR_DELETED_ACCOUNT");
		case Result::SELECT_SERVER_ERROR_INACCESSIBLE: return TEXT("SELECT_SERVER_ERROR_INACCESSIBLE");
		case Result::SELECT_SERVER_ERROR_OVERLOAD: return TEXT("SELECT_SERVER_ERROR_OVERLOAD");
		case Result::SELECT_SERVER_ERROR_NOT_FOUND: return TEXT("SELECT_SERVER_ERROR_NOT_FOUND");
		case Result::LOGIN_ERROR_HIVE_REGISTER_DEVICE_SETTING: return TEXT("LOGIN_ERROR_HIVE_REGISTER_DEVICE_SETTING");
		case Result::LOGIN_ERROR_NO_CHARACTER_CREATED: return TEXT("LOGIN_ERROR_NO_CHARACTER_CREATED");
		case Result::LOGIN_ERROR_HIVE_INVALID_PARAMETER: return TEXT("LOGIN_ERROR_HIVE_INVALID_PARAMETER");
		case Result::LOGIN_ERROR_HIVE_REQUEST_JSON: return TEXT("LOGIN_ERROR_HIVE_REQUEST_JSON");
		case Result::LOGIN_ERROR_HIVE_DB_ERROR: return TEXT("LOGIN_ERROR_HIVE_DB_ERROR");
		case Result::LOGIN_ERROR_HIVE_INVALID_TOKEN_AWS: return TEXT("LOGIN_ERROR_HIVE_INVALID_TOKEN_AWS");
		case Result::LOGIN_ERROR_HIVE_INVALID_APPID: return TEXT("LOGIN_ERROR_HIVE_INVALID_APPID");
		case Result::LOGIN_ERROR_HIVE_IDP_TOKEN_ERROR: return TEXT("LOGIN_ERROR_HIVE_IDP_TOKEN_ERROR");
		case Result::LOGIN_ERROR_HIVE_INVALID_TOKEN: return TEXT("LOGIN_ERROR_HIVE_INVALID_TOKEN");
		case Result::LOGIN_ERROR_HIVE_NO_TOKEN_HEADER: return TEXT("LOGIN_ERROR_HIVE_NO_TOKEN_HEADER");
		case Result::LOGIN_ERROR_HIVE_UNKNOWN_ERROR: return TEXT("LOGIN_ERROR_HIVE_UNKNOWN_ERROR");
		case Result::LOGIN_ERROR_HIVE_UNABLE_TO_CONNECT: return TEXT("LOGIN_ERROR_HIVE_UNABLE_TO_CONNECT");
		case Result::LOGIN_ERROR_HIVE_CONNECT: return TEXT("LOGIN_ERROR_HIVE_CONNECT");
		case Result::LOGIN_ERROR_HIVE_PARSE: return TEXT("LOGIN_ERROR_HIVE_PARSE");
		case Result::LOGIN_ERROR_HIVE_NO_RESULT_CODE: return TEXT("LOGIN_ERROR_HIVE_NO_RESULT_CODE");
		case Result::LOGIN_ERROR_HIVE_UNKNOWN_RESULT_CODE: return TEXT("LOGIN_ERROR_HIVE_UNKNOWN_RESULT_CODE");
		case Result::LOGIN_ERROR_HIVE_NO_REGISTER_DEVICE_PARAMETER: return TEXT("LOGIN_ERROR_HIVE_NO_REGISTER_DEVICE_PARAMETER");
		case Result::LOGIN_ERROR_HIVE_CONNECT_REGISTER_DEVICE: return TEXT("LOGIN_ERROR_HIVE_CONNECT_REGISTER_DEVICE");
		case Result::LOGIN_ERROR_HIVE_PARSE_REGISTER_DEVICE: return TEXT("LOGIN_ERROR_HIVE_PARSE_REGISTER_DEVICE");
		case Result::SELECT_CHANNEL_ERROR_UNKNOWN: return TEXT("SELECT_CHANNEL_ERROR_UNKNOWN");
		case Result::SELECT_CHANNEL_ERROR_INVALID_STATUS: return TEXT("SELECT_CHANNEL_ERROR_INVALID_STATUS");
		case Result::SELECT_CHANNEL_ERROR_INVALID_AREA: return TEXT("SELECT_CHANNEL_ERROR_INVALID_AREA");
		case Result::SELECT_CHANNEL_ERROR_SAME_AREA_CHANNEL: return TEXT("SELECT_CHANNEL_ERROR_SAME_AREA_CHANNEL");
		case Result::SELECT_CHANNEL_ERROR_MAP_TYPE: return TEXT("SELECT_CHANNEL_ERROR_MAP_TYPE");
		case Result::SELECT_CHANNEL_ERROR_IN_COMBAT: return TEXT("SELECT_CHANNEL_ERROR_IN_COMBAT");
		case Result::SELECT_CHANNEL_ERROR_IN_SHUTTLE: return TEXT("SELECT_CHANNEL_ERROR_IN_SHUTTLE");
		case Result::SELECT_CHANNEL_ERROR_INVALID_CHANNEL: return TEXT("SELECT_CHANNEL_ERROR_INVALID_CHANNEL");
		case Result::SELECT_CHANNEL_ERROR_CONGESTION_FULL: return TEXT("SELECT_CHANNEL_ERROR_CONGESTION_FULL");
		case Result::ZPAY_LOGIN_ERROR_INVALID_AUTH: return TEXT("ZPAY_LOGIN_ERROR_INVALID_AUTH");
		case Result::ZPAY_LOGIN_ERROR_ENTER_SERVER: return TEXT("ZPAY_LOGIN_ERROR_ENTER_SERVER");
		case Result::ZPAY_LOGIN_ERROR_PLAY_USER: return TEXT("ZPAY_LOGIN_ERROR_PLAY_USER");
		case Result::ZPAY_LOGIN_ERROR_UNLOCK_CONTENTS: return TEXT("ZPAY_LOGIN_ERROR_UNLOCK_CONTENTS");
		case Result::ITEM_ERROR_UNKNOWN: return TEXT("ITEM_ERROR_UNKNOWN");
		case Result::ITEM_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("ITEM_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::ITEM_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("ITEM_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::ITEM_ERROR_INVALID_USER: return TEXT("ITEM_ERROR_INVALID_USER");
		case Result::ITEM_ERROR_INVALID_USER_STATUS: return TEXT("ITEM_ERROR_INVALID_USER_STATUS");
		case Result::ITEM_ERROR_NOT_ALLOWED_LEVEL: return TEXT("ITEM_ERROR_NOT_ALLOWED_LEVEL");
		case Result::ITEM_ERROR_ITEM_NOT_FOUND: return TEXT("ITEM_ERROR_ITEM_NOT_FOUND");
		case Result::ITEM_ERROR_INVALID_ITEM_ID: return TEXT("ITEM_ERROR_INVALID_ITEM_ID");
		case Result::ITEM_ERROR_INVALID_ITEM_DB_ID: return TEXT("ITEM_ERROR_INVALID_ITEM_DB_ID");
		case Result::ITEM_ERROR_ADD_INVENTORY_FULL: return TEXT("ITEM_ERROR_ADD_INVENTORY_FULL");
		case Result::ITEM_ERROR_ADD_NOT_ALLOWED_GENDER: return TEXT("ITEM_ERROR_ADD_NOT_ALLOWED_GENDER");
		case Result::ITEM_ERROR_ADD_INVALID_STACK_COUNT: return TEXT("ITEM_ERROR_ADD_INVALID_STACK_COUNT");
		case Result::ITEM_ERROR_ADD_WEIGHT_FULL: return TEXT("ITEM_ERROR_ADD_WEIGHT_FULL");
		case Result::ITEM_ERROR_NOT_ENOUGH_MONEY: return TEXT("ITEM_ERROR_NOT_ENOUGH_MONEY");
		case Result::ITEM_ERROR_OVERFLOW_MAX_MONEY: return TEXT("ITEM_ERROR_OVERFLOW_MAX_MONEY");
		case Result::ITEM_ERROR_DELETE_LOCKED: return TEXT("ITEM_ERROR_DELETE_LOCKED");
		case Result::ITEM_ERROR_LOCK_ALREADY_LOCKED: return TEXT("ITEM_ERROR_LOCK_ALREADY_LOCKED");
		case Result::ITEM_ERROR_LOCK_ALREADY_UNLOCKED: return TEXT("ITEM_ERROR_LOCK_ALREADY_UNLOCKED");
		case Result::ITEM_ERROR_LOCK_NOT_LOCKABLE: return TEXT("ITEM_ERROR_LOCK_NOT_LOCKABLE");
		case Result::ITEM_ERROR_DECREASE_INSUFFICIENT: return TEXT("ITEM_ERROR_DECREASE_INSUFFICIENT");
		case Result::ITEM_ERROR_EQUIP_NOT_EQUIP_ITEM: return TEXT("ITEM_ERROR_EQUIP_NOT_EQUIP_ITEM");
		case Result::ITEM_ERROR_EQUIP_ALREADY_EQUIPPED: return TEXT("ITEM_ERROR_EQUIP_ALREADY_EQUIPPED");
		case Result::ITEM_ERROR_EQUIP_ALREADY_UNEQUIPPED: return TEXT("ITEM_ERROR_EQUIP_ALREADY_UNEQUIPPED");
		case Result::ITEM_ERROR_EQUIP_INVALID_PRESET_ID: return TEXT("ITEM_ERROR_EQUIP_INVALID_PRESET_ID");
		case Result::ITEM_ERROR_EQUIP_INVALID_PRESET_NAME: return TEXT("ITEM_ERROR_EQUIP_INVALID_PRESET_NAME");
		case Result::ITEM_ERROR_EQUIP_CHANGE_WEAPON_TPYE: return TEXT("ITEM_ERROR_EQUIP_CHANGE_WEAPON_TPYE");
		case Result::ITEM_ERROR_EQUIP_CAN_NOT_UNEQUIP_WEAPON: return TEXT("ITEM_ERROR_EQUIP_CAN_NOT_UNEQUIP_WEAPON");
		case Result::ITEM_ERROR_SOCKET_INVALID_TARGET: return TEXT("ITEM_ERROR_SOCKET_INVALID_TARGET");
		case Result::ITEM_ERROR_SOCKET_ALREADY_SOCKETED: return TEXT("ITEM_ERROR_SOCKET_ALREADY_SOCKETED");
		case Result::ITEM_ERROR_SOCKET_IS_NOT_EMPTY: return TEXT("ITEM_ERROR_SOCKET_IS_NOT_EMPTY");
		case Result::ITEM_ERROR_USE_NOT_USABLE_ITEM: return TEXT("ITEM_ERROR_USE_NOT_USABLE_ITEM");
		case Result::ITEM_ERROR_USE_INVALID_EFFECT_TYPE: return TEXT("ITEM_ERROR_USE_INVALID_EFFECT_TYPE");
		case Result::ITEM_ERROR_USE_INVALID_EFFECT_PARAM: return TEXT("ITEM_ERROR_USE_INVALID_EFFECT_PARAM");
		case Result::ITEM_ERROR_USE_NOT_ENOUGH_ITEMS: return TEXT("ITEM_ERROR_USE_NOT_ENOUGH_ITEMS");
		case Result::ITEM_ERROR_USE_IN_COOLDOWN: return TEXT("ITEM_ERROR_USE_IN_COOLDOWN");
		case Result::ITEM_ERROR_USE_POTION_HP_MP_FULL: return TEXT("ITEM_ERROR_USE_POTION_HP_MP_FULL");
		case Result::ITEM_ERROR_USE_LOCKED: return TEXT("ITEM_ERROR_USE_LOCKED");
		case Result::ITEM_ERROR_USE_ALREADY_LEARN_SKILL: return TEXT("ITEM_ERROR_USE_ALREADY_LEARN_SKILL");
		case Result::ITEM_ERROR_USE_MAX_USED_BONUS_STAT_POINT_ITEM: return TEXT("ITEM_ERROR_USE_MAX_USED_BONUS_STAT_POINT_ITEM");
		case Result::ITEM_ERROR_USE_PVP_ALIGNMENT: return TEXT("ITEM_ERROR_USE_PVP_ALIGNMENT");
		case Result::ITEM_ERROR_ENCHANT_INVALID_SCROLL: return TEXT("ITEM_ERROR_ENCHANT_INVALID_SCROLL");
		case Result::ITEM_ERROR_ENCHANT_INVALID_AMOUNT: return TEXT("ITEM_ERROR_ENCHANT_INVALID_AMOUNT");
		case Result::ITEM_ERROR_ENCHANT_RATE_FAILED: return TEXT("ITEM_ERROR_ENCHANT_RATE_FAILED");
		case Result::ITEM_ERROR_ENCHANT_EQUIPED_ITEM_CANNOT_OVER_SAFE_LEVEL: return TEXT("ITEM_ERROR_ENCHANT_EQUIPED_ITEM_CANNOT_OVER_SAFE_LEVEL");
		case Result::ITEM_ERROR_ENCHANT_LOCKED: return TEXT("ITEM_ERROR_ENCHANT_LOCKED");
		case Result::ITEM_ERROR_ENCHANT_INVALID_TARGET: return TEXT("ITEM_ERROR_ENCHANT_INVALID_TARGET");
		case Result::ITEM_ERROR_ENCHANT_INVALID_LEVEL: return TEXT("ITEM_ERROR_ENCHANT_INVALID_LEVEL");
		case Result::ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY: return TEXT("ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY");
		case Result::ITEM_ERROR_DECOMPOSE_INVALID_EQUIPED: return TEXT("ITEM_ERROR_DECOMPOSE_INVALID_EQUIPED");
		case Result::ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY_EQUIP: return TEXT("ITEM_ERROR_DECOMPOSE_INVALID_CATEGORY_EQUIP");
		case Result::ITEM_ERROR_DECOMPOSE_PAID_ITEM: return TEXT("ITEM_ERROR_DECOMPOSE_PAID_ITEM");
		case Result::ITEM_ERROR_DECOMPOSE_INVALID_REWARD_ID: return TEXT("ITEM_ERROR_DECOMPOSE_INVALID_REWARD_ID");
		case Result::ITEM_ERROR_NPCSHOP_UNABLE_TO_SELL: return TEXT("ITEM_ERROR_NPCSHOP_UNABLE_TO_SELL");
		case Result::ITEM_ERROR_ADD_STORE_FULL: return TEXT("ITEM_ERROR_ADD_STORE_FULL");
		case Result::ITEM_ERROR_ADD_STORAGE_FULL: return TEXT("ITEM_ERROR_ADD_STORAGE_FULL");
		case Result::ITEM_ERROR_EXPIRED_DATE_TIME: return TEXT("ITEM_ERROR_EXPIRED_DATE_TIME");
		case Result::ITEM_ERROR_UNLOCK: return TEXT("ITEM_ERROR_UNLOCK");
		case Result::ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_TARGET_CATEGORY: return TEXT("ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_TARGET_CATEGORY");
		case Result::ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_LEVEL: return TEXT("ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_LEVEL");
		case Result::ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_SCROLL: return TEXT("ITEM_ERROR_ENCHANT_MULTI_NOT_MATCHED_SCROLL");
		case Result::ITEM_ERROR_USABLE_IN_SAFE_ZONE: return TEXT("ITEM_ERROR_USABLE_IN_SAFE_ZONE");
		case Result::ITEM_ERROR_COMBAT_STATUS: return TEXT("ITEM_ERROR_COMBAT_STATUS");
		case Result::ITEM_ERROR_DELETE_LIST_EMPTY: return TEXT("ITEM_ERROR_DELETE_LIST_EMPTY");
		case Result::ITEM_ERROR_CREATE_LIST_EMPTY: return TEXT("ITEM_ERROR_CREATE_LIST_EMPTY");
		case Result::ITEM_ERROR_ABNORMALITY_STAT_ALREADY_IN_EFFECT: return TEXT("ITEM_ERROR_ABNORMALITY_STAT_ALREADY_IN_EFFECT");
		case Result::ITEM_ERROR_FUSE_DIFFERENT_ITEMS: return TEXT("ITEM_ERROR_FUSE_DIFFERENT_ITEMS");
		case Result::ITEM_ERROR_FUSE_CHARM_SAME_TEMPLATE_ID: return TEXT("ITEM_ERROR_FUSE_CHARM_SAME_TEMPLATE_ID");
		case Result::ITEM_ERROR_FUSE_DIFFERENT_EQUIP_GROUP_ID: return TEXT("ITEM_ERROR_FUSE_DIFFERENT_EQUIP_GROUP_ID");
		case Result::ITEM_ERROR_FUSE_NOT_EQUIP_ITEM: return TEXT("ITEM_ERROR_FUSE_NOT_EQUIP_ITEM");
		case Result::ITEM_ERROR_FUSE_INVALID_FUSE_ITEM: return TEXT("ITEM_ERROR_FUSE_INVALID_FUSE_ITEM");
		case Result::ITEM_ERROR_FUSE_LESS_THAN_MIN_LEVEL: return TEXT("ITEM_ERROR_FUSE_LESS_THAN_MIN_LEVEL");
		case Result::ITEM_ERROR_FUSE_NOT_SAME_ENCHANT_LEVEL: return TEXT("ITEM_ERROR_FUSE_NOT_SAME_ENCHANT_LEVEL");
		case Result::ITEM_ERROR_FUSE_REFINE_DATA_NOT_EXISTS: return TEXT("ITEM_ERROR_FUSE_REFINE_DATA_NOT_EXISTS");
		case Result::ITEM_ERROR_FUSE_SELECT_REFINE_DATA: return TEXT("ITEM_ERROR_FUSE_SELECT_REFINE_DATA");
		case Result::ITEM_ERROR_INVALID_USER_DEATH: return TEXT("ITEM_ERROR_INVALID_USER_DEATH");
		case Result::ITEM_ERROR_INVALID_MAGICAL_FORGE: return TEXT("ITEM_ERROR_INVALID_MAGICAL_FORGE");
		case Result::ITEM_ERROR_INVALID_MAGICAL_FORGE_MATIRAL_ITEM_EFFECT_TYPE: return TEXT("ITEM_ERROR_INVALID_MAGICAL_FORGE_MATIRAL_ITEM_EFFECT_TYPE");
		case Result::ITEM_ERROR_MAGICAL_FORGE_INVALID_TARGET: return TEXT("ITEM_ERROR_MAGICAL_FORGE_INVALID_TARGET");
		case Result::ITEM_ERROR_MAGICAL_FORGE_INVALID_MATARIAL: return TEXT("ITEM_ERROR_MAGICAL_FORGE_INVALID_MATARIAL");
		case Result::ITEM_ERROR_MAGICAL_FORGE_MAX_LEVEL: return TEXT("ITEM_ERROR_MAGICAL_FORGE_MAX_LEVEL");
		case Result::ITEM_ERROR_MAGICAL_FORGE_LOCKED: return TEXT("ITEM_ERROR_MAGICAL_FORGE_LOCKED");
		case Result::ITEM_ERROR_MAGICAL_FORGE_INVALID_GRADE: return TEXT("ITEM_ERROR_MAGICAL_FORGE_INVALID_GRADE");
		case Result::ITEM_ERROR_NOT_ALLOWED_MAP: return TEXT("ITEM_ERROR_NOT_ALLOWED_MAP");
		case Result::REWARD_ERROR_UNKNOWN: return TEXT("REWARD_ERROR_UNKNOWN");
		case Result::REWARD_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("REWARD_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::REWARD_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("REWARD_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::REWARD_ERROR_INVALID_REWARD: return TEXT("REWARD_ERROR_INVALID_REWARD");
		case Result::REWARD_ERROR_INVALID_REWARD_TYPE: return TEXT("REWARD_ERROR_INVALID_REWARD_TYPE");
		case Result::REWARD_ERROR_CURRENCY_EXCEEDED: return TEXT("REWARD_ERROR_CURRENCY_EXCEEDED");
		case Result::VEHICLE_ERROR_UNKNOWN: return TEXT("VEHICLE_ERROR_UNKNOWN");
		case Result::VEHICLE_ERROR_INVALID_ID: return TEXT("VEHICLE_ERROR_INVALID_ID");
		case Result::VEHICLE_ERROR_ALREADY_MOUNTED: return TEXT("VEHICLE_ERROR_ALREADY_MOUNTED");
		case Result::VEHICLE_ERROR_IN_TRANSFORM: return TEXT("VEHICLE_ERROR_IN_TRANSFORM");
		case Result::VEHICLE_ERROR_FORBIDDEN_AREA: return TEXT("VEHICLE_ERROR_FORBIDDEN_AREA");
		case Result::VEHICLE_ERROR_DISABLE_BEHAVIOR: return TEXT("VEHICLE_ERROR_DISABLE_BEHAVIOR");
		case Result::VEHICLE_ERROR_COMBAT_STATUS: return TEXT("VEHICLE_ERROR_COMBAT_STATUS");
		case Result::VEHICLE_ERROR_NOT_YET_MOVE_TIME: return TEXT("VEHICLE_ERROR_NOT_YET_MOVE_TIME");
		case Result::MAIL_ERROR_UNKNOWN: return TEXT("MAIL_ERROR_UNKNOWN");
		case Result::MAIL_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("MAIL_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::MAIL_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("MAIL_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::MAIL_ERROR_CURRENCY_EXCEEDED: return TEXT("MAIL_ERROR_CURRENCY_EXCEEDED");
		case Result::MAIL_ERROR_INVALID_MAIL_BOX_TYPE: return TEXT("MAIL_ERROR_INVALID_MAIL_BOX_TYPE");
		case Result::MAIL_ERROR_INVALID_CATEGORY_TYPE: return TEXT("MAIL_ERROR_INVALID_CATEGORY_TYPE");
		case Result::MAIL_ERROR_INVALID_MAIL_DB_ID: return TEXT("MAIL_ERROR_INVALID_MAIL_DB_ID");
		case Result::MAIL_ERROR_RECV_ALREADY_EXPIRED: return TEXT("MAIL_ERROR_RECV_ALREADY_EXPIRED");
		case Result::MAIL_ERROR_RECV_INVENTORY_FULL: return TEXT("MAIL_ERROR_RECV_INVENTORY_FULL");
		case Result::MAIL_ERROR_ALREADY_READ_MAIL: return TEXT("MAIL_ERROR_ALREADY_READ_MAIL");
		case Result::MAIL_ERROR_INVALID_CURRENCY_TYPE: return TEXT("MAIL_ERROR_INVALID_CURRENCY_TYPE");
		case Result::MAIL_ERROR_INVALID_MAIL_STRING_ID: return TEXT("MAIL_ERROR_INVALID_MAIL_STRING_ID");
		case Result::MAIL_ERROR_EMPTY_DELETED_MAIL_DB_ID_LIST: return TEXT("MAIL_ERROR_EMPTY_DELETED_MAIL_DB_ID_LIST");
		case Result::MAIL_ERROR_DB_OPERATION_MAIL_LIST_DELETE: return TEXT("MAIL_ERROR_DB_OPERATION_MAIL_LIST_DELETE");
		case Result::MAIL_ERROR_ALREADY_OHTER_SERVER_RECV: return TEXT("MAIL_ERROR_ALREADY_OHTER_SERVER_RECV");
		case Result::MAIL_ERROR_INVALID_MAIL_TYPE_ID: return TEXT("MAIL_ERROR_INVALID_MAIL_TYPE_ID");
		case Result::SKILL_ERROR_UNKNOWN: return TEXT("SKILL_ERROR_UNKNOWN");
		case Result::SKILL_ERROR_INVALID_POS: return TEXT("SKILL_ERROR_INVALID_POS");
		case Result::SKILL_ERROR_INVALID_INFO: return TEXT("SKILL_ERROR_INVALID_INFO");
		case Result::SKILL_ERROR_IN_COOL_DOWN: return TEXT("SKILL_ERROR_IN_COOL_DOWN");
		case Result::SKILL_ERROR_INVALID_STATE: return TEXT("SKILL_ERROR_INVALID_STATE");
		case Result::SKILL_ERROR_DISABLED_SKILL: return TEXT("SKILL_ERROR_DISABLED_SKILL");
		case Result::SKILL_ERROR_OTHER_SKILL_IN_PROGRESS: return TEXT("SKILL_ERROR_OTHER_SKILL_IN_PROGRESS");
		case Result::SKILL_ERROR_CANNOT_USE_PASSIVE: return TEXT("SKILL_ERROR_CANNOT_USE_PASSIVE");
		case Result::SKILL_ERROR_INVALID_PREV_SKILL_INFO: return TEXT("SKILL_ERROR_INVALID_PREV_SKILL_INFO");
		case Result::SKILL_ERROR_INVALID_CHAIN_CONDITION: return TEXT("SKILL_ERROR_INVALID_CHAIN_CONDITION");
		case Result::SKILL_ERROR_HP_LESS: return TEXT("SKILL_ERROR_HP_LESS");
		case Result::SKILL_ERROR_HP_GREATER: return TEXT("SKILL_ERROR_HP_GREATER");
		case Result::SKILL_ERROR_MP_LESS: return TEXT("SKILL_ERROR_MP_LESS");
		case Result::SKILL_ERROR_MP_GREATER: return TEXT("SKILL_ERROR_MP_GREATER");
		case Result::SKILL_ERROR_ONLY_USER: return TEXT("SKILL_ERROR_ONLY_USER");
		case Result::SKILL_ERROR_SP_LESS: return TEXT("SKILL_ERROR_SP_LESS");
		case Result::SKILL_ERROR_NOT_ALLOWED_CLASS: return TEXT("SKILL_ERROR_NOT_ALLOWED_CLASS");
		case Result::SKILL_ERROR_NOT_ALLOWED_TARGET: return TEXT("SKILL_ERROR_NOT_ALLOWED_TARGET");
		case Result::SKILL_ERROR_NO_REQUIRED_ITEM: return TEXT("SKILL_ERROR_NO_REQUIRED_ITEM");
		case Result::SKILL_ERROR_TARGET_NOT_IN_RANGE: return TEXT("SKILL_ERROR_TARGET_NOT_IN_RANGE");
		case Result::SKILL_ERROR_CURSOR_OUT_OF_RANGE: return TEXT("SKILL_ERROR_CURSOR_OUT_OF_RANGE");
		case Result::SKILL_ERROR_INVALID_SKILL_ID: return TEXT("SKILL_ERROR_INVALID_SKILL_ID");
		case Result::SKILL_ERROR_ENCHANT_MAX_SKILL_LEVEL: return TEXT("SKILL_ERROR_ENCHANT_MAX_SKILL_LEVEL");
		case Result::SKILL_ERROR_INVALID_COST_DATA: return TEXT("SKILL_ERROR_INVALID_COST_DATA");
		case Result::SKILL_ERROR_INVALID_CHAIN_SKILL_HIT_CONDITION: return TEXT("SKILL_ERROR_INVALID_CHAIN_SKILL_HIT_CONDITION");
		case Result::SKILL_ERROR_LEARN_NOT_EXIST_ROOT_SKILL: return TEXT("SKILL_ERROR_LEARN_NOT_EXIST_ROOT_SKILL");
		case Result::SKILL_ERROR_LEARN_INVALID_ESSENTIAL_PASSIVITY: return TEXT("SKILL_ERROR_LEARN_INVALID_ESSENTIAL_PASSIVITY");
		case Result::SKILL_ERROR_LEARN_ALREADY_LEARN_SKILL: return TEXT("SKILL_ERROR_LEARN_ALREADY_LEARN_SKILL");
		case Result::SKILL_ERROR_NOT_MATCH_TRIGGER_CONDITION: return TEXT("SKILL_ERROR_NOT_MATCH_TRIGGER_CONDITION");
		case Result::SKILL_ERROR_NOT_USABLE_IN_SAFE_ZONE: return TEXT("SKILL_ERROR_NOT_USABLE_IN_SAFE_ZONE");
		case Result::SKILL_ERROR_UNREACHABLE_STRAIGHT: return TEXT("SKILL_ERROR_UNREACHABLE_STRAIGHT");
		case Result::SKILL_ERROR_CHECK_COLLISION_FOR_SKILL: return TEXT("SKILL_ERROR_CHECK_COLLISION_FOR_SKILL");
		case Result::ABNORMALITY_ERROR_UNKNOWN: return TEXT("ABNORMALITY_ERROR_UNKNOWN");
		case Result::ABNORMALITY_ERROR_IGNORE_BY_PRIORITY: return TEXT("ABNORMALITY_ERROR_IGNORE_BY_PRIORITY");
		case Result::ABNORMALITY_ERROR_EXCEED_STACK_COUNT: return TEXT("ABNORMALITY_ERROR_EXCEED_STACK_COUNT");
		case Result::RESURRECT_RESULT_FAILED_NOT_SPAWNED: return TEXT("RESURRECT_RESULT_FAILED_NOT_SPAWNED");
		case Result::RESURRECT_RESULT_FAILED_ALIVE: return TEXT("RESURRECT_RESULT_FAILED_ALIVE");
		case Result::RESURRECT_RESULT_FAILED_NOT_PAST_MIN_TICK: return TEXT("RESURRECT_RESULT_FAILED_NOT_PAST_MIN_TICK");
		case Result::QUEST_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("QUEST_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::QUEST_ERROR_INVALID_STATUS: return TEXT("QUEST_ERROR_INVALID_STATUS");
		case Result::QUEST_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE: return TEXT("QUEST_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE");
		case Result::QUEST_ERROR_INVALID_DATA_SHEET: return TEXT("QUEST_ERROR_INVALID_DATA_SHEET");
		case Result::QUEST_ERROR_INVALID_ACCEPT_CHPATER_ID: return TEXT("QUEST_ERROR_INVALID_ACCEPT_CHPATER_ID");
		case Result::QUEST_ERROR_INVALID_ACCEPT_STORY_ID: return TEXT("QUEST_ERROR_INVALID_ACCEPT_STORY_ID");
		case Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_TYPE: return TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_TYPE");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_FIRST_QUEST: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_FIRST_QUEST");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_LAST_QUEST: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_LAST_QUEST");
		case Result::QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID: return TEXT("QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_REWARED: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_REWARED");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_STORY: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_STORY");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_QUEST: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_NEXT_QUEST");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_PRE_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_PRE_EVENT_ACTION");
		case Result::QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_POST_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_ACCEPT_NOT_COMPLETED_POST_EVENT_ACTION");
		case Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_ACTIVE_COUNT_MAX: return TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_ACTIVE_COUNT_MAX");
		case Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_REPEAT_RANDOM_PICK: return TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_REPEAT_RANDOM_PICK");
		case Result::QUEST_ERROR_INVALID_UPDATE_WARP_STATUS: return TEXT("QUEST_ERROR_INVALID_UPDATE_WARP_STATUS");
		case Result::QUEST_ERROR_INVALID_UPDATE_STATUS: return TEXT("QUEST_ERROR_INVALID_UPDATE_STATUS");
		case Result::QUEST_ERROR_INVALID_UPDATE_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_UPDATE_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_UPDATE_OBJECTIVE: return TEXT("QUEST_ERROR_INVALID_UPDATE_OBJECTIVE");
		case Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED: return TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED");
		case Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_TELEPORT: return TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_TELEPORT");
		case Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_INSTANCE_FIELD: return TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_INSTANCE_FIELD");
		case Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_LAST_PUBLIC_FIELD: return TEXT("QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_LAST_PUBLIC_FIELD");
		case Result::QUEST_ERROR_INVALID_UPDATE_RANGE: return TEXT("QUEST_ERROR_INVALID_UPDATE_RANGE");
		case Result::QUEST_ERROR_INVALID_UPDATE_WARP_TO_INSTANCE_FIELD: return TEXT("QUEST_ERROR_INVALID_UPDATE_WARP_TO_INSTANCE_FIELD");
		case Result::QUEST_ERROR_INVALID_UPDATE_WARP_TO_PUBLIC_FIELD: return TEXT("QUEST_ERROR_INVALID_UPDATE_WARP_TO_PUBLIC_FIELD");
		case Result::QUEST_ERROR_INVALID_REWARD_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_REWARD_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_PROCESS_POST_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_PROCESS_POST_EVENT_ACTION");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_LAST_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_LAST_QUEST_ID");
		case Result::QUEST_ERROR_ALREADY_REWARD_QUEST_ID: return TEXT("QUEST_ERROR_ALREADY_REWARD_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_PRE_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_PRE_EVENT_ACTION");
		case Result::QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_POST_EVENT_ACTION: return TEXT("QUEST_ERROR_INVALID_REWARD_NOT_COMPLETED_POST_EVENT_ACTION");
		case Result::QUEST_ERROR_NOT_REWARDED: return TEXT("QUEST_ERROR_NOT_REWARDED");
		case Result::QUEST_ERROR_NOT_ENOUGH_LEVEL: return TEXT("QUEST_ERROR_NOT_ENOUGH_LEVEL");
		case Result::QUEST_ERROR_NOT_ENOUGH_MAIN_STORY_ID: return TEXT("QUEST_ERROR_NOT_ENOUGH_MAIN_STORY_ID");
		case Result::QUEST_ERROR_INVALID_WARP_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_WARP_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE: return TEXT("QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE");
		case Result::QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE_NOT_ACCEPTED: return TEXT("QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE_NOT_ACCEPTED");
		case Result::QUEST_ERROR_NOT_ENOUGH_MONEY: return TEXT("QUEST_ERROR_NOT_ENOUGH_MONEY");
		case Result::QUEST_ERROR_GIVE_UP_STATE: return TEXT("QUEST_ERROR_GIVE_UP_STATE");
		case Result::QUEST_ERROR_INVALID_REWARD_QUEST_TYPE: return TEXT("QUEST_ERROR_INVALID_REWARD_QUEST_TYPE");
		case Result::QUEST_ERROR_GATEWAY_SERVER_CONNECTION: return TEXT("QUEST_ERROR_GATEWAY_SERVER_CONNECTION");
		case Result::QUEST_ERROR_ALREADY_COMPLETE_QUEST_ID: return TEXT("QUEST_ERROR_ALREADY_COMPLETE_QUEST_ID");
		case Result::QUEST_ERROR_NOT_RPOGRESS_QUEST_ID: return TEXT("QUEST_ERROR_NOT_RPOGRESS_QUEST_ID");
		case Result::QUEST_ERROR_QUEST_ID: return TEXT("QUEST_ERROR_QUEST_ID");
		case Result::QUEST_ERROR_INVALID_USER_DATA: return TEXT("QUEST_ERROR_INVALID_USER_DATA");
		case Result::QUEST_ERROR_NOT_ACCEPTABLE_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_ACCEPTABLE_GUILD_QUEST");
		case Result::QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_MONEY: return TEXT("QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_MONEY");
		case Result::QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_REFRESH_COUNT: return TEXT("QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_REFRESH_COUNT");
		case Result::QUEST_ERROR_QUEST_BOARD_NOT_QUEST_BOARD_NPC: return TEXT("QUEST_ERROR_QUEST_BOARD_NOT_QUEST_BOARD_NPC");
		case Result::QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_ACCEPT_COUNT: return TEXT("QUEST_ERROR_QUEST_BOARD_NOT_ENOUGH_ACCEPT_COUNT");
		case Result::QUEST_ERROR_QUEST_BOARD_INVALID_NPC: return TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_NPC");
		case Result::QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_ID: return TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_ID");
		case Result::QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_DATA: return TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_DATA");
		case Result::QUEST_ERROR_QUEST_BOARD_INVALID_TYPE: return TEXT("QUEST_ERROR_QUEST_BOARD_INVALID_TYPE");
		case Result::QUEST_ERROR_INVALID_QUEST_INDEX: return TEXT("QUEST_ERROR_INVALID_QUEST_INDEX");
		case Result::QUEST_ERROR_NOT_SCROLL_QUEST: return TEXT("QUEST_ERROR_NOT_SCROLL_QUEST");
		case Result::QUEST_ERROR_NOT_REMAIN_REFRESH: return TEXT("QUEST_ERROR_NOT_REMAIN_REFRESH");
		case Result::QUEST_ERROR_INVALID_WARP_NOT_MAP_WORLD: return TEXT("QUEST_ERROR_INVALID_WARP_NOT_MAP_WORLD");
		case Result::QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID_NOT_POP_UP: return TEXT("QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID_NOT_POP_UP");
		case Result::WARP_ERROR_INVALID_STATUS: return TEXT("WARP_ERROR_INVALID_STATUS");
		case Result::WARP_ERROR_INVALID_VOLUME: return TEXT("WARP_ERROR_INVALID_VOLUME");
		case Result::WARP_ERROR_NOT_IN_VOLUME: return TEXT("WARP_ERROR_NOT_IN_VOLUME");
		case Result::WARP_ERROR_LEAVE_INSTANCE_SINGLE: return TEXT("WARP_ERROR_LEAVE_INSTANCE_SINGLE");
		case Result::WARP_ERROR_IN_COMBAT: return TEXT("WARP_ERROR_IN_COMBAT");
		case Result::WARP_ERROR_INVALID_SPOT_INFO: return TEXT("WARP_ERROR_INVALID_SPOT_INFO");
		case Result::WARP_ERROR_INVALID_MAP: return TEXT("WARP_ERROR_INVALID_MAP");
		case Result::WARP_ERROR_NOT_SPAWNED: return TEXT("WARP_ERROR_NOT_SPAWNED");
		case Result::WARP_ERROR_NOT_ALIVE: return TEXT("WARP_ERROR_NOT_ALIVE");
		case Result::WARP_ERROR_CAN_NOT_DO_ACTION_WARP: return TEXT("WARP_ERROR_CAN_NOT_DO_ACTION_WARP");
		case Result::WARP_ERROR_NOT_ENOUGH_DUNGEON_TIME: return TEXT("WARP_ERROR_NOT_ENOUGH_DUNGEON_TIME");
		case Result::WARP_ERROR_NOT_OPEN_DUNGEON_TIME: return TEXT("WARP_ERROR_NOT_OPEN_DUNGEON_TIME");
		case Result::WARP_ERROR_NOT_ENOUGH_DUNGEON_COST: return TEXT("WARP_ERROR_NOT_ENOUGH_DUNGEON_COST");
		case Result::WARP_ERROR_NEED_INSTANCE_SINGLE_MAP: return TEXT("WARP_ERROR_NEED_INSTANCE_SINGLE_MAP");
		case Result::WARP_ERROR_TOPO_MAP_NOT_EXIST: return TEXT("WARP_ERROR_TOPO_MAP_NOT_EXIST");
		case Result::WARP_ERROR_RANDOM_POS_FAILED: return TEXT("WARP_ERROR_RANDOM_POS_FAILED");
		case Result::WARP_ERROR_INVALID_MAP_ID: return TEXT("WARP_ERROR_INVALID_MAP_ID");
		case Result::WARP_ERROR_INVALID_CHANNEL_ID: return TEXT("WARP_ERROR_INVALID_CHANNEL_ID");
		case Result::WARP_ERROR_CHANNEL_CONGESTION_FULL: return TEXT("WARP_ERROR_CHANNEL_CONGESTION_FULL");
		case Result::WARP_ERROR_INVALID_MAIN_QUEST: return TEXT("WARP_ERROR_INVALID_MAIN_QUEST");
		case Result::WARP_ERROR_INVALID_LEVEL: return TEXT("WARP_ERROR_INVALID_LEVEL");
		case Result::WARP_ERROR_WAIT_ACK_DESPAWN_READYING: return TEXT("WARP_ERROR_WAIT_ACK_DESPAWN_READYING");
		case Result::WARP_ERROR_NOT_OPEN_INVADE_TIME: return TEXT("WARP_ERROR_NOT_OPEN_INVADE_TIME");
		case Result::WARP_ERROR_NOT_ENOUGH_INVADE_TIME: return TEXT("WARP_ERROR_NOT_ENOUGH_INVADE_TIME");
		case Result::WARP_ERROR_NOT_ENOUGH_INVADE_COST: return TEXT("WARP_ERROR_NOT_ENOUGH_INVADE_COST");
		case Result::WARP_ERROR_WORLD_BOSS_OPEN: return TEXT("WARP_ERROR_WORLD_BOSS_OPEN");
		case Result::WARP_ERROR_WORLD_BOSS_ENOUGH_COST: return TEXT("WARP_ERROR_WORLD_BOSS_ENOUGH_COST");
		case Result::WARP_ERROR_WORLD_BOSS_INVALID_STATUS: return TEXT("WARP_ERROR_WORLD_BOSS_INVALID_STATUS");
		case Result::WARP_ERROR_WORLD_BOSS_ALREADY_ENTER: return TEXT("WARP_ERROR_WORLD_BOSS_ALREADY_ENTER");
		case Result::WARP_ERROR_INVALID_SPAWN_GROUP_ID: return TEXT("WARP_ERROR_INVALID_SPAWN_GROUP_ID");
		case Result::WARP_ERROR_LEAVE_INSTANCE_SINGLE_DELAY_TIME: return TEXT("WARP_ERROR_LEAVE_INSTANCE_SINGLE_DELAY_TIME");
		case Result::WARP_ERROR_INVALID_NPC: return TEXT("WARP_ERROR_INVALID_NPC");
		case Result::WARP_ERROR_INVALID_NPC_IN_SPAWN_GROUP: return TEXT("WARP_ERROR_INVALID_NPC_IN_SPAWN_GROUP");
		case Result::WARP_ERROR_UNLOCK: return TEXT("WARP_ERROR_UNLOCK");
		case Result::WARP_ERROR_UNKNOWN: return TEXT("WARP_ERROR_UNKNOWN");
		case Result::PARTY_ERROR_UNKNOWN: return TEXT("PARTY_ERROR_UNKNOWN");
		case Result::PARTY_ERROR_INVALID_PARTY: return TEXT("PARTY_ERROR_INVALID_PARTY");
		case Result::PARTY_ERROR_ALREADY_PARTY_MEMBER: return TEXT("PARTY_ERROR_ALREADY_PARTY_MEMBER");
		case Result::PARTY_ERROR_ALREADY_PARTY_INVITED: return TEXT("PARTY_ERROR_ALREADY_PARTY_INVITED");
		case Result::PARTY_ERROR_NOT_JOINED: return TEXT("PARTY_ERROR_NOT_JOINED");
		case Result::PARTY_ERROR_NOT_LEADER: return TEXT("PARTY_ERROR_NOT_LEADER");
		case Result::PARTY_ERROR_NOT_MEMBER: return TEXT("PARTY_ERROR_NOT_MEMBER");
		case Result::PARTY_ERROR_NOT_FOUND_USER: return TEXT("PARTY_ERROR_NOT_FOUND_USER");
		case Result::PARTY_ERROR_NOT_LOGIN_USER: return TEXT("PARTY_ERROR_NOT_LOGIN_USER");
		case Result::PARTY_ERROR_MEMBER_FULL: return TEXT("PARTY_ERROR_MEMBER_FULL");
		case Result::PARTY_ERROR_NOT_INVITED: return TEXT("PARTY_ERROR_NOT_INVITED");
		case Result::PARTY_ERROR_CANNOT_INVITE_SELF: return TEXT("PARTY_ERROR_CANNOT_INVITE_SELF");
		case Result::PARTY_ERROR_CANNOT_KICK_SELF: return TEXT("PARTY_ERROR_CANNOT_KICK_SELF");
		case Result::PARTY_ERROR_ALREADY_LEADER: return TEXT("PARTY_ERROR_ALREADY_LEADER");
		case Result::PARTY_ERROR_INVALID_DROP_OWNERSHIP_TYPE: return TEXT("PARTY_ERROR_INVALID_DROP_OWNERSHIP_TYPE");
		case Result::PARTY_ERROR_WARP_TO_LEADER_MYSELF: return TEXT("PARTY_ERROR_WARP_TO_LEADER_MYSELF");
		case Result::PARTY_ERROR_INVALID_LEADER_MAP: return TEXT("PARTY_ERROR_INVALID_LEADER_MAP");
		case Result::PARTY_ERROR_INVALID_LEADER_MAP_SPWAN_SPOT: return TEXT("PARTY_ERROR_INVALID_LEADER_MAP_SPWAN_SPOT");
		case Result::PARTY_ERROR_LEADER_IN_INSTANCE_MAP: return TEXT("PARTY_ERROR_LEADER_IN_INSTANCE_MAP");
		case Result::PARTY_ERROR_NOT_ALL_READY: return TEXT("PARTY_ERROR_NOT_ALL_READY");
		case Result::PARTY_ERROR_USER_IN_INSTANCE_MAP: return TEXT("PARTY_ERROR_USER_IN_INSTANCE_MAP");
		case Result::PARTY_ERROR_SAME_PARTY: return TEXT("PARTY_ERROR_SAME_PARTY");
		case Result::PARTY_DUNGEON_ERROR_ALREADY_PARTY_MATCHING_WAIT: return TEXT("PARTY_DUNGEON_ERROR_ALREADY_PARTY_MATCHING_WAIT");
		case Result::PARTY_DUNGEON_ERROR_NOT_PARTY_MATCHING_WAIT: return TEXT("PARTY_DUNGEON_ERROR_NOT_PARTY_MATCHING_WAIT");
		case Result::PARTY_DUNGEON_ERROR_MATCHING_TIMEOUT: return TEXT("PARTY_DUNGEON_ERROR_MATCHING_TIMEOUT");
		case Result::PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY: return TEXT("PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY");
		case Result::PARTY_DUNGEON_ERROR_NOT_PLAYING: return TEXT("PARTY_DUNGEON_ERROR_NOT_PLAYING");
		case Result::PARTY_DUNGEON_ERROR_NOT_HELPCALL: return TEXT("PARTY_DUNGEON_ERROR_NOT_HELPCALL");
		case Result::PARTY_DUNGEON_ERROR_INVALID_MAP: return TEXT("PARTY_DUNGEON_ERROR_INVALID_MAP");
		case Result::PARTY_DUNGEON_ERROR_NO_REMAIN_HELP_COUNT: return TEXT("PARTY_DUNGEON_ERROR_NO_REMAIN_HELP_COUNT");
		case Result::PARTY_DUNGEON_ERROR_ALREADY_HELP_STATUS: return TEXT("PARTY_DUNGEON_ERROR_ALREADY_HELP_STATUS");
		case Result::PARTY_DUNGEON_ERROR_PLAYING: return TEXT("PARTY_DUNGEON_ERROR_PLAYING");
		case Result::PARTY_DUNGEON_ERROR_JUST_BEFORE_END: return TEXT("PARTY_DUNGEON_ERROR_JUST_BEFORE_END");
		case Result::PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY_MEMBER: return TEXT("PARTY_DUNGEON_ERROR_NOT_UNLOCKED_DIFFICULTY_MEMBER");
		case Result::PARTY_ERROR_NOT_INVITABLE_USER: return TEXT("PARTY_ERROR_NOT_INVITABLE_USER");
		case Result::PARTY_ERROR_NOT_INVITABLE_OTHER_SERVER_USER: return TEXT("PARTY_ERROR_NOT_INVITABLE_OTHER_SERVER_USER");
		case Result::PARTY_DUNGEON_ERROR_CANNOT_INVITE: return TEXT("PARTY_DUNGEON_ERROR_CANNOT_INVITE");
		case Result::PARTY_DUNGEON_ERROR_CANNOT_INTER_SERVER: return TEXT("PARTY_DUNGEON_ERROR_CANNOT_INTER_SERVER");
		case Result::PARTY_DUNGEON_ERROR_INVALID_AREA: return TEXT("PARTY_DUNGEON_ERROR_INVALID_AREA");
		case Result::PARTY_ERROR_INVALID_HOME_WORLD_ID: return TEXT("PARTY_ERROR_INVALID_HOME_WORLD_ID");
		case Result::PARTY_ERROR_CANNOT_INVITE_OTHER_ENTER_USER: return TEXT("PARTY_ERROR_CANNOT_INVITE_OTHER_ENTER_USER");
		case Result::CHANGE_WEAPON_TYPE_ERROR_UNKNOWN: return TEXT("CHANGE_WEAPON_TYPE_ERROR_UNKNOWN");
		case Result::CHANGE_WEAPON_TYPE_ERROR_INVALID_USER_STATE: return TEXT("CHANGE_WEAPON_TYPE_ERROR_INVALID_USER_STATE");
		case Result::CHANGE_WEAPON_TYPE_ERROR_INVALID_DURATION: return TEXT("CHANGE_WEAPON_TYPE_ERROR_INVALID_DURATION");
		case Result::CHANGE_WEAPON_TYPE_ERROR_IN_CHANGE_CLASS: return TEXT("CHANGE_WEAPON_TYPE_ERROR_IN_CHANGE_CLASS");
		case Result::CHANGE_WEAPON_TYPE_ERROR_COOLDOWN_TIME: return TEXT("CHANGE_WEAPON_TYPE_ERROR_COOLDOWN_TIME");
		case Result::PRESET_ERROR_COOLDOWN_TIME: return TEXT("PRESET_ERROR_COOLDOWN_TIME");
		case Result::EVENT_ACTION_ERROR_INVALID_STATUS: return TEXT("EVENT_ACTION_ERROR_INVALID_STATUS");
		case Result::EVENT_ACTION_ERROR_INVALID_MAP: return TEXT("EVENT_ACTION_ERROR_INVALID_MAP");
		case Result::EVENT_ACTION_ERROR_INVALID_SPOT: return TEXT("EVENT_ACTION_ERROR_INVALID_SPOT");
		case Result::EVENT_ACTION_ERROR_INVALID_REQUEST: return TEXT("EVENT_ACTION_ERROR_INVALID_REQUEST");
		case Result::EVENT_ACTION_ERROR_INVALID_MAIN_QUEST: return TEXT("EVENT_ACTION_ERROR_INVALID_MAIN_QUEST");
		case Result::EVENT_ACTION_ERROR_INVALID_QUESTID: return TEXT("EVENT_ACTION_ERROR_INVALID_QUESTID");
		case Result::EVENT_ACTION_ERROR_INVALID_POST_EVENT_ACTION_QUEST_NOT_COMPLETED: return TEXT("EVENT_ACTION_ERROR_INVALID_POST_EVENT_ACTION_QUEST_NOT_COMPLETED");
		case Result::EVENT_ACTION_ERROR_INVALID_INDEX: return TEXT("EVENT_ACTION_ERROR_INVALID_INDEX");
		case Result::EVENT_ACTION_ERROR_INVALID_EVENT_ACTION_GROUP_ID: return TEXT("EVENT_ACTION_ERROR_INVALID_EVENT_ACTION_GROUP_ID");
		case Result::EVENT_ACTION_ERROR_INVALID_PARAM: return TEXT("EVENT_ACTION_ERROR_INVALID_PARAM");
		case Result::EVENT_ACTION_ERROR_ALL_COMPLETED: return TEXT("EVENT_ACTION_ERROR_ALL_COMPLETED");
		case Result::EVENT_ACTION_ERROR_NOT_ALIVE: return TEXT("EVENT_ACTION_ERROR_NOT_ALIVE");
		case Result::EVENT_ACTION_ERROR_USER_WARP: return TEXT("EVENT_ACTION_ERROR_USER_WARP");
		case Result::TERRITORY_ERROR_INVALID_STATUS: return TEXT("TERRITORY_ERROR_INVALID_STATUS");
		case Result::TERRITORY_ERROR_INVALID_ID: return TEXT("TERRITORY_ERROR_INVALID_ID");
		case Result::TERRITORY_ERROR_INVALID_POSITION: return TEXT("TERRITORY_ERROR_INVALID_POSITION");
		case Result::DROP_OBJECT_LOOT_ERROR_UNKNOWN: return TEXT("DROP_OBJECT_LOOT_ERROR_UNKNOWN");
		case Result::DROP_OBJECT_LOOT_ERROR_INVALID_USER_STATE: return TEXT("DROP_OBJECT_LOOT_ERROR_INVALID_USER_STATE");
		case Result::DROP_OBJECT_LOOT_ERROR_INVALID_DROP_OBJECT: return TEXT("DROP_OBJECT_LOOT_ERROR_INVALID_DROP_OBJECT");
		case Result::DROP_OBJECT_LOOT_ERROR_INVALID_DISTANCE: return TEXT("DROP_OBJECT_LOOT_ERROR_INVALID_DISTANCE");
		case Result::DROP_OBJECT_LOOT_ERROR_ALREADY_IN_LOOT: return TEXT("DROP_OBJECT_LOOT_ERROR_ALREADY_IN_LOOT");
		case Result::DROP_OBJECT_LOOT_ERROR_NOT_OWNERSHIP: return TEXT("DROP_OBJECT_LOOT_ERROR_NOT_OWNERSHIP");
		case Result::DROP_OBJECT_LOOT_ERROR_NOT_MATCH_GAME_OPTION: return TEXT("DROP_OBJECT_LOOT_ERROR_NOT_MATCH_GAME_OPTION");
		case Result::DROP_OBJECT_LOOT_ERROR_NOT_MY_ITEM_PARTY_LOOT: return TEXT("DROP_OBJECT_LOOT_ERROR_NOT_MY_ITEM_PARTY_LOOT");
		case Result::DROP_OBJECT_LOOT_GUILD_STORE_OWNERSHIP: return TEXT("DROP_OBJECT_LOOT_GUILD_STORE_OWNERSHIP");
		case Result::DROP_OBJECT_LOOT_ERROR_TRY_AGAIN: return TEXT("DROP_OBJECT_LOOT_ERROR_TRY_AGAIN");
		case Result::BOOKMARK_ERROR_UNKNOWN: return TEXT("BOOKMARK_ERROR_UNKNOWN");
		case Result::BOOKMARK_ERROR_INVALID_STATUS: return TEXT("BOOKMARK_ERROR_INVALID_STATUS");
		case Result::BOOKMARK_ERROR_LIST_MAX: return TEXT("BOOKMARK_ERROR_LIST_MAX");
		case Result::BOOKMARK_ERROR_INVALID_MAP: return TEXT("BOOKMARK_ERROR_INVALID_MAP");
		case Result::BOOKMARK_ERROR_INVALID_TITLE_LEN: return TEXT("BOOKMARK_ERROR_INVALID_TITLE_LEN");
		case Result::BOOKMARK_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("BOOKMARK_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::BOOKMARK_ERROR_TELEPORT_INVALID_ITEM: return TEXT("BOOKMARK_ERROR_TELEPORT_INVALID_ITEM");
		case Result::BOOKMARK_ERROR_INVALID_SHARE_TYPE: return TEXT("BOOKMARK_ERROR_INVALID_SHARE_TYPE");
		case Result::BOOKMARK_ERROR_NOT_GUILD_MEMBER: return TEXT("BOOKMARK_ERROR_NOT_GUILD_MEMBER");
		case Result::BOOKMARK_ERROR_NO_SHARE_RIGHT: return TEXT("BOOKMARK_ERROR_NO_SHARE_RIGHT");
		case Result::BOOKMARK_ERROR_CANNOT_INTER_SERVER: return TEXT("BOOKMARK_ERROR_CANNOT_INTER_SERVER");
		case Result::BOOKMARK_ERROR_NOT_SHARE_TIME: return TEXT("BOOKMARK_ERROR_NOT_SHARE_TIME");
		case Result::BOOKMARK_ERROR_NO_PARTY_MEMBERS: return TEXT("BOOKMARK_ERROR_NO_PARTY_MEMBERS");
		case Result::BOOKMARK_ERROR_NO_GUILD_MEMBERS: return TEXT("BOOKMARK_ERROR_NO_GUILD_MEMBERS");
		case Result::BOOKMARK_ERROR_INVALID_TYPE: return TEXT("BOOKMARK_ERROR_INVALID_TYPE");
		case Result::BOOKMARK_ERROR_ALREADY_REGISTERED: return TEXT("BOOKMARK_ERROR_ALREADY_REGISTERED");
		case Result::BOOKMARK_ERROR_CANNOT_WARP_INTER_SERVER: return TEXT("BOOKMARK_ERROR_CANNOT_WARP_INTER_SERVER");
		case Result::INTERACT_ERROR_INVALID_STATE: return TEXT("INTERACT_ERROR_INVALID_STATE");
		case Result::INTERACT_ERROR_INVALID_NPC: return TEXT("INTERACT_ERROR_INVALID_NPC");
		case Result::INTERACT_ERROR_INVALID_NPC_RANGE: return TEXT("INTERACT_ERROR_INVALID_NPC_RANGE");
		case Result::INTERACT_ERROR_INVALID_PROP_START_USERCOUNT_OVER: return TEXT("INTERACT_ERROR_INVALID_PROP_START_USERCOUNT_OVER");
		case Result::INTERACT_ERROR_INVALID_PROP_MAX_USERCOUNT_LIMIT_OVER: return TEXT("INTERACT_ERROR_INVALID_PROP_MAX_USERCOUNT_LIMIT_OVER");
		case Result::INTERACT_ERROR_INVALID_PROP_START_ALREADY: return TEXT("INTERACT_ERROR_INVALID_PROP_START_ALREADY");
		case Result::INTERACT_ERROR_INVALID_PROP_NOT_ENDABLE: return TEXT("INTERACT_ERROR_INVALID_PROP_NOT_ENDABLE");
		case Result::INTERACT_ERROR_INVALID_PROP_STATUS_QUEST_NOT_ENDABLE: return TEXT("INTERACT_ERROR_INVALID_PROP_STATUS_QUEST_NOT_ENDABLE");
		case Result::INTERACT_ERROR_INVALID_PROP_STATUS_NOT_CANCEL_ABLE: return TEXT("INTERACT_ERROR_INVALID_PROP_STATUS_NOT_CANCEL_ABLE");
		case Result::INTERACT_ERROR_INVALID_PROP: return TEXT("INTERACT_ERROR_INVALID_PROP");
		case Result::INTERACT_ERROR_INVALID_QUEST_PROP: return TEXT("INTERACT_ERROR_INVALID_QUEST_PROP");
		case Result::INTERACT_ERROR_NOT_SPAWNED_PROP: return TEXT("INTERACT_ERROR_NOT_SPAWNED_PROP");
		case Result::INTERACT_ERROR_INVALID_PROP_RANGE: return TEXT("INTERACT_ERROR_INVALID_PROP_RANGE");
		case Result::INTERACT_ERROR_DEACTIVATE_EVENT_PRAYER: return TEXT("INTERACT_ERROR_DEACTIVATE_EVENT_PRAYER");
		case Result::INTERACT_ERROR_INVADE_PORTAL_PROP_REGISTRATION_ALREADY: return TEXT("INTERACT_ERROR_INVADE_PORTAL_PROP_REGISTRATION_ALREADY");
		case Result::INTERACT_ERROR_INVALID_PROP_TYPE: return TEXT("INTERACT_ERROR_INVALID_PROP_TYPE");
		case Result::INTERACT_ERROR_NOT_ALIVE: return TEXT("INTERACT_ERROR_NOT_ALIVE");
		case Result::BONUS_STAT_ERROR_UNKNOWN: return TEXT("BONUS_STAT_ERROR_UNKNOWN");
		case Result::BONUS_STAT_ERROR_INVALID_USER_STATUS: return TEXT("BONUS_STAT_ERROR_INVALID_USER_STATUS");
		case Result::BONUS_STAT_ERROR_INVALID_PKT_DATA: return TEXT("BONUS_STAT_ERROR_INVALID_PKT_DATA");
		case Result::BONUS_STAT_ERROR_INVALID_STAT_TYPE: return TEXT("BONUS_STAT_ERROR_INVALID_STAT_TYPE");
		case Result::BONUS_STAT_ERROR_DB_FAILED: return TEXT("BONUS_STAT_ERROR_DB_FAILED");
		case Result::BONUS_STAT_ERROR_DUPLICATE_STAT_TYPE: return TEXT("BONUS_STAT_ERROR_DUPLICATE_STAT_TYPE");
		case Result::BONUS_STAT_ERROR_NOT_ENOUGH_STAT_POINT: return TEXT("BONUS_STAT_ERROR_NOT_ENOUGH_STAT_POINT");
		case Result::BONUS_STAT_ERROR_MAX_STAT: return TEXT("BONUS_STAT_ERROR_MAX_STAT");
		case Result::SKILL_SLOT_ERROR_UNKNOWN: return TEXT("SKILL_SLOT_ERROR_UNKNOWN");
		case Result::SKILL_SLOT_ERROR_INVALID_USER_STATUS: return TEXT("SKILL_SLOT_ERROR_INVALID_USER_STATUS");
		case Result::SKILL_SLOT_ERROR_INVALID_PKT_DATA: return TEXT("SKILL_SLOT_ERROR_INVALID_PKT_DATA");
		case Result::SKILL_SLOT_ERROR_INVALID_SKILL_ID: return TEXT("SKILL_SLOT_ERROR_INVALID_SKILL_ID");
		case Result::SKILL_SLOT_ERROR_SAME_SLOT: return TEXT("SKILL_SLOT_ERROR_SAME_SLOT");
		case Result::SKILL_SLOT_ERROR_INVALID_SKILL_SLOT_ID: return TEXT("SKILL_SLOT_ERROR_INVALID_SKILL_SLOT_ID");
		case Result::SKILL_SLOT_ERROR_INVALID_WEAPON_TYPE: return TEXT("SKILL_SLOT_ERROR_INVALID_WEAPON_TYPE");
		case Result::SKILL_SLOT_ERROR_SAME_AUTO_FLAG: return TEXT("SKILL_SLOT_ERROR_SAME_AUTO_FLAG");
		case Result::SKILL_SLOT_ERROR_NOT_RESIST_SLOT: return TEXT("SKILL_SLOT_ERROR_NOT_RESIST_SLOT");
		case Result::SKILL_SLOT_ERROR_DB_FAILED: return TEXT("SKILL_SLOT_ERROR_DB_FAILED");
		case Result::SKILL_SLOT_ERROR_DUPLICATE_SKILL_GROUP: return TEXT("SKILL_SLOT_ERROR_DUPLICATE_SKILL_GROUP");
		case Result::SKILL_SLOT_ERROR_SAME_INTERVAL_VALUE: return TEXT("SKILL_SLOT_ERROR_SAME_INTERVAL_VALUE");
		case Result::SKILL_SLOT_ERROR_OVER_INTERVAL_SECOND: return TEXT("SKILL_SLOT_ERROR_OVER_INTERVAL_SECOND");
		case Result::USER_OPTIPON_ERROR_DB_FAILED: return TEXT("USER_OPTIPON_DB_FAILED");
		case Result::PORTAL_WARP_ERROR_INVALID_STATUS: return TEXT("PORTAL_WARP_ERROR_INVALID_STATUS");
		case Result::PORTAL_WARP_ERROR_UNKNOWN: return TEXT("PORTAL_WARP_ERROR_UNKNOWN");
		case Result::PORTAL_WARP_ERROR_INVALID_SPOT_INFO: return TEXT("PORTAL_WARP_ERROR_INVALID_SPOT_INFO");
		case Result::QUICK_SLOT_ERROR_UNKNOWN: return TEXT("QUICK_SLOT_ERROR_UNKNOWN");
		case Result::QUICK_SLOT_ERROR_INVALID_USER_STATUS: return TEXT("QUICK_SLOT_ERROR_INVALID_USER_STATUS");
		case Result::QUICK_SLOT_ERROR_OVERFLOW_MAX_PAGE_SIZE: return TEXT("QUICK_SLOT_ERROR_OVERFLOW_MAX_PAGE_SIZE");
		case Result::QUICK_SLOT_ERROR_ITEM_NOT_EXIST_TARGET: return TEXT("QUICK_SLOT_ERROR_ITEM_NOT_EXIST_TARGET");
		case Result::QUICK_SLOT_ERROR_INVALID_QUICK_SLOT_ID: return TEXT("QUICK_SLOT_ERROR_INVALID_QUICK_SLOT_ID");
		case Result::QUICK_SLOT_ERROR_QUICK_SLOT_ID_DIFF_ITEM_DB_ID: return TEXT("QUICK_SLOT_ERROR_QUICK_SLOT_ID_DIFF_ITEM_DB_ID");
		case Result::QUICK_SLOT_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("QUICK_SLOT_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::QUICK_SLOT_ERROR_SET_DB_ERROR: return TEXT("QUICK_SLOT_ERROR_SET_DB_ERROR");
		case Result::QUICK_SLOT_ERROR_AUTO_DB_ERROR: return TEXT("QUICK_SLOT_ERROR_AUTO_DB_ERROR");
		case Result::QUICK_SLOT_ERROR_INVALID_ITEM_TYPE_AUTO: return TEXT("QUICK_SLOT_ERROR_INVALID_ITEM_TYPE_AUTO");
		case Result::FAIRY_ERROR_INVALID_STATUS: return TEXT("FAIRY_ERROR_INVALID_STATUS");
		case Result::FAIRY_ERROR_UNKNOWN: return TEXT("FAIRY_ERROR_UNKNOWN");
		case Result::FAIRY_ERROR_INVALID_FAIRY_ID: return TEXT("FAIRY_ERROR_INVALID_FAIRY_ID");
		case Result::FAIRY_ERROR_NOT_HAVE_FAIRY: return TEXT("FAIRY_ERROR_NOT_HAVE_FAIRY");
		case Result::FAIRY_ERROR_NOT_ENOUGH_FAIRY: return TEXT("FAIRY_ERROR_NOT_ENOUGH_FAIRY");
		case Result::FAIRY_ERROR_MAX_AMOUNT_OVER: return TEXT("FAIRY_ERROR_MAX_AMOUNT_OVER");
		case Result::FAIRY_ERROR_ALREADY_EQUIP_FAIRY: return TEXT("FAIRY_ERROR_ALREADY_EQUIP_FAIRY");
		case Result::FAIRY_ERROR_INVALID_DATA: return TEXT("FAIRY_ERROR_INVALID_DATA");
		case Result::FAIRY_ERROR_INVALID_PACKET_DATA: return TEXT("FAIRY_ERROR_INVALID_PACKET_DATA");
		case Result::FAIRY_SUMMON_ERROR_INVALID_FAIRY_GRADE: return TEXT("FAIRY_SUMMON_ERROR_INVALID_FAIRY_GRADE");
		case Result::FAIRY_SUMMON_ERROR_MAX_CONFIRM_COUNT: return TEXT("FAIRY_SUMMON_ERROR_MAX_CONFIRM_COUNT");
		case Result::FAIRY_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM: return TEXT("FAIRY_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM");
		case Result::FAIRY_CONFRIM_ERROR_INVALID_FAIRY_CONFIRM_ID: return TEXT("FAIRY_CONFRIM_ERROR_INVALID_FAIRY_CONFIRM_ID");
		case Result::FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT: return TEXT("FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT");
		case Result::FAIRY_CONFRIM_ERROR_EXPIRED: return TEXT("FAIRY_CONFRIM_ERROR_EXPIRED");
		case Result::FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("FAIRY_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::FAIRY_ENCHANT_ERROR_MAX_LEVEL_OVER: return TEXT("FAIRY_ENCHANT_ERROR_MAX_LEVEL_OVER");
		case Result::FAIRY_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL: return TEXT("FAIRY_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL");
		case Result::FAIRY_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("FAIRY_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::FAIRY_COMPOSE_ERROR_NOT_SAME_GRADE: return TEXT("FAIRY_COMPOSE_ERROR_NOT_SAME_GRADE");
		case Result::FAIRY_COMPOSE_ERROR_MAX_COMPOSE_COUNT: return TEXT("FAIRY_COMPOSE_ERROR_MAX_COMPOSE_COUNT");
		case Result::FAIRY_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT: return TEXT("FAIRY_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT");
		case Result::FAIRY_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("FAIRY_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::FAIRY_COMPOSE_ERROR_LEGEND_GRADE: return TEXT("FAIRY_COMPOSE_ERROR_LEGEND_GRADE");
		case Result::CHAT_ERROR_INVALID_CHAT_TYPE: return TEXT("CHAT_ERROR_INVALID_CHAT_TYPE");
		case Result::CHAT_ERROR_MESSAGE_LEN_ZERO: return TEXT("CHAT_ERROR_MESSAGE_LEN_ZERO");
		case Result::CHAT_ERROR_MESSAGE_LEN_OVER: return TEXT("CHAT_ERROR_MESSAGE_LEN_OVER");
		case Result::CHAT_ERROR_NOT_FOUND_USER: return TEXT("CHAT_ERROR_NOT_FOUND_USER");
		case Result::CHAT_ERROR_CAN_NOT_SEND_TO_ME: return TEXT("CHAT_ERROR_CAN_NOT_SEND_TO_ME");
		case Result::CHAT_ERROR_DETECTED_AS_SPAM: return TEXT("CHAT_ERROR_DETECTED_AS_SPAM");
		case Result::CHAT_ERROR_EXCEED_FREE_INVADE_CHAT: return TEXT("CHAT_ERROR_EXCEED_FREE_INVADE_CHAT");
		case Result::CHAT_ERROR_RESET_FREE_INVADE_CHAT: return TEXT("CHAT_ERROR_RESET_FREE_INVADE_CHAT");
		case Result::CHAT_ERROR_USER_BANNED: return TEXT("CHAT_ERROR_USER_BANNED");
		case Result::CHAT_ERROR_SERVER_CONNECTION: return TEXT("CHAT_ERROR_SERVER_CONNECTION");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_TYPE: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_TYPE");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_AMOUNT: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_CURRENCY_AMOUNT");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_NOT_ENOUGH_CURRENCY: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_NOT_ENOUGH_CURRENCY");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_NO_USER_IN_WORLD: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_NO_USER_IN_WORLD");
		case Result::CHAT_ERROR_PAID_INVADE_CHAT_FAIL_REQ_PAY: return TEXT("CHAT_ERROR_PAID_INVADE_CHAT_FAIL_REQ_PAY");
		case Result::CHAT_ERROR_INVADE_CHAT_LEVEL: return TEXT("CHAT_ERROR_INVADE_CHAT_LEVEL");
		case Result::CHAT_ERROR_INVADE_CHAT_DB_UPDATE_ERROR: return TEXT("CHAT_ERROR_INVADE_CHAT_DB_UPDATE_ERROR");
		case Result::CHAT_ERROR_INTER_CHAT_NOT_INTER_SERVER_USER: return TEXT("CHAT_ERROR_INTER_CHAT_NOT_INTER_SERVER_USER");
		case Result::CHAT_ERROR_INTER_CHAT_NO_INTER_SERVER_SESSION: return TEXT("CHAT_ERROR_INTER_CHAT_NO_INTER_SERVER_SESSION");
		case Result::RESURRECT_ERROR_INVALID_RESURRECT_ID: return TEXT("RESURRECT_ERROR_INVALID_RESURRECT_ID");
		case Result::RESURRECT_ERROR_COST_VALUE: return TEXT("RESURRECT_ERROR_COST_VALUE");
		case Result::RESURRECT_ERROR_INVASIONOUT: return TEXT("RESURRECT_ERROR_INVASIONOUT");
		case Result::DEATH_ERROR_INVALID_ID: return TEXT("DEATH_ERROR_INVALID_ID");
		case Result::DEATH_ERROR_RECOVERY_GOLD_COST: return TEXT("DEATH_ERROR_RECOVERY_GOLD_COST");
		case Result::DEATH_ERROR_RECOVERY_DIA_COST: return TEXT("DEATH_ERROR_RECOVERY_DIA_COST");
		case Result::DEATH_ERRORR_RECOVERY_ITEM_COST: return TEXT("DEATH_ERRORR_RECOVERY_ITEM_COST");
		case Result::DEATH_ERROR_RECOVERY_TIME_OUT: return TEXT("DEATH_ERROR_RECOVERY_TIME_OUT");
		case Result::DEATH_ERROR_RECOVERY_COUNT: return TEXT("DEATH_ERROR_RECOVERY_COUNT");
		case Result::GUILD_CREATE_ERROR_INVALID_NAME_LENGTH: return TEXT("GUILD_CREATE_ERROR_INVALID_NAME_LENGTH");
		case Result::GUILD_CREATE_ERROR_INVALID_DESC_LENGTH: return TEXT("GUILD_CREATE_ERROR_INVALID_DESC_LENGTH");
		case Result::GUILD_CREATE_ERROR_NOT_ALLOWED_NAME: return TEXT("GUILD_CREATE_ERROR_NOT_ALLOWED_NAME");
		case Result::GUILD_CREATE_ERROR_DUPLICATION_NAME: return TEXT("GUILD_CREATE_ERROR_DUPLICATION_NAME");
		case Result::GUILD_CREATE_ERROR_HAD_GUILD: return TEXT("GUILD_CREATE_ERROR_HAD_GUILD");
		case Result::GUILD_CREATE_ERROR_NO_GOLD: return TEXT("GUILD_CREATE_ERROR_NO_GOLD");
		case Result::GUILD_ERROR_HAD_GUILD: return TEXT("GUILD_ERROR_HAD_GUILD");
		case Result::GUILD_ERROR_DB: return TEXT("GUILD_ERROR_DB");
		case Result::GUILD_ERROR_UNKNOWN: return TEXT("GUILD_ERROR_UNKNOWN");
		case Result::GUILD_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("GUILD_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::GUILD_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("REWARD_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::GUILD_DELETE_ERROR_MEMBER_CNT: return TEXT("GUILD_DELETE_ERROR_MEMBER_CNT");
		case Result::GUILD_ERROR_NOT_EXSIT_MEMEBER: return TEXT("GUILD_ERROR_NOT_EXSIT_MEMEBER");
		case Result::GUILD_ERROR_NOT_REQUEST_JOIN: return TEXT("GUILD_ERROR_NOT_REQUEST_JOIN");
		case Result::GUILD_JOIN_ERROR_FULL: return TEXT("GUILD_JOIN_ERROR_FULL");
		case Result::GUILD_JOIN_ERROR_COOLDOWN_TIME: return TEXT("GUILD_JOIN_ERROR_COOLDOWN_TIME");
		case Result::GUILD_JOIN_ERROR_INVALID_GUILD: return TEXT("GUILD_JOIN_ERROR_INAVLID_GUILD");
		case Result::GUILD_JOIN_ERROR_USER_LEVEL: return TEXT("GUILD_JOIN_ERROR_USER_LEVEL");
		case Result::GUILD_LEAVE_ERROR_CAPTAIN: return TEXT("GUILD_LEAVE_ERROR_CAPTAIN");
		case Result::GUILD_ERROR_FULL_GRADE_NUM: return TEXT("GUILD_ERROR_FULL_GRADE_NUM");
		case Result::GUILD_DELETE_ERROR_GRADE: return TEXT("GUILD_DELETE_ERROR_GRADE");
		case Result::GUILD_ERROR_GRADE: return TEXT("GUILD_ERROR_GRADE");
		case Result::GUILD_ERROR_NOT_FIND: return TEXT("GUILD_ERROR_NOT_FIND");
		case Result::GUILD_ERROR_DAILY_DONATE_OVER: return TEXT("GUILD_ERROR_DAILY_DONATE_OVER");
		case Result::GUILD_ERROR_ENOUGH_CURRENCY: return TEXT("GUILD_ERROR_ENOUGH_CURRENCY");
		case Result::GUILD_ERROR_GUILD_COIN_FULLY: return TEXT("GUILD_ERROR_GUILD_COIN_FULLY");
		case Result::GUILD_ERROR_NOT_ENOUGH_GUILD_ASSET: return TEXT("GUILD_ERROR_NOT_ENOUGH_GUILD_ASSET");
		case Result::GUILD_ERROR_ATTENDANCE_ALREADY: return TEXT("GUILD_ERROR_ATTENDANCE_ALREADY");
		case Result::GUILD_GRADE_ERROR_INAVLID_MEMBER: return TEXT("GUILD_GRADE_ERROR_INAVLID_MEMBER");
		case Result::GUILD_GRADE_ERROR_DO_NOT_CHANGE_GRADE_LEVEL: return TEXT("GUILD_GRADE_ERROR_DO_NOT_CHANGE_GRADE_LEVEL");
		case Result::GUILD_ERROR_BUILDING_INAVLID_ID: return TEXT("GUILD_ERROR_BUILDING_INAVLID_ID");
		case Result::GUILD_ERROR_BUILDING_DB: return TEXT("GUILD_ERROR_BUILDING_DB");
		case Result::GUILD_ERROR_BUILDING_AUTH: return TEXT("GUILD_ERROR_BUILDING_AUTH");
		case Result::GUILD_ERROR_BUILDING_OPEN: return TEXT("GUILD_ERROR_BUILDING_OPEN");
		case Result::GUILD_ERROR_BUILDING_UPGRADE: return TEXT("GUILD_ERROR_BUILDING_UPGRADE");
		case Result::GUILD_ERROR_BUILDING_DATE: return TEXT("GUILD_ERROR_BUILDING_DATE");
		case Result::GUILD_ERROR_BUILDING_LEVEL: return TEXT("GUILD_ERROR_BUILDING_LEVEL");
		case Result::GUILD_ERROR_GUILD_CAMP_LEVEL: return TEXT("GUILD_ERROR_GUILD_CAMP_LEVEL");
		case Result::GUILD_ERROR_BUILDING_MAX_LEVEL: return TEXT("GUILD_ERROR_BUILDING_MAX_LEVEL");
		case Result::GUILD_ERROR_BUILDING_ASSET: return TEXT("GUILD_ERROR_BUILDING_ASSET");
		case Result::GUILD_ERROR_BUILDING_PASSIVITY: return TEXT("GUILD_ERROR_BUILDING_PASSIVITY");
		case Result::GUILD_ERROR_BUILDING_UNAUTHORIZED_LEVELUP: return TEXT("GUILD_ERROR_BUILDING_UNAUTHORIZED_LEVELUP");
		case Result::GUILD_ERROR_UNABLE_GUILD_LEVEL: return TEXT("GUILD_ERROR_UNABLE_GUILD_LEVEL");
		case Result::GUILD_BUY_SHOP_ERROR_UNABLE_GUILD_LEVEL: return TEXT("GUILD_BUY_SHOP_ERROR_UNABLE_GUILD_LEVEL");
		case Result::GUILD_BUY_SHOP_INVALID_PRODUCT_ID: return TEXT("GUILD_BUY_SHOP_INVALID_PRODUCT_ID");
		case Result::GUILD_BUY_SHOP_ERROR_OVER_CNT: return TEXT("GUILD_BUY_SHOP_ERROR_OVER_CNT");
		case Result::GUILD_SHOP_ERROR_ENOUGH_CURRENCY: return TEXT("GUILD_SHOP_ERROR_ENOUGH_CURRENCY");
		case Result::GUILD_ERROR_CHANGE_MEMBER_GRADE: return TEXT("GUILD_ERROR_CHANGE_MEMBER_GRADE");
		case Result::GUILD_JOIN_ERROR_FULL_OF_APPLICANTS: return TEXT("GUILD_JOIN_ERROR_FULL_OF_APPLICANTS");
		case Result::GUILD_JOIN_ERROR_OVER_REQUEST_CNT: return TEXT("GUILD_JOIN_ERROR_OVER_REQUEST_CNT");
		case Result::GUILD_ACTIVITY_ERROR_GUILD_JOIN: return TEXT("GUILD_ACTIVITY_ERROR_GUILD_JOIN");
		case Result::GUILD_ERROR_EXIST_HOSTILE_GUILD: return TEXT("GUILD_ERROR_EXIST_HOSTILE_GUILD");
		case Result::GUILD_ERROR_INVALID_USER: return TEXT("GUILD_ERROR_INVALID_USER");
		case Result::GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID: return TEXT("GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID");
		case Result::GUILD_ERROR_ALLY_GUILD_ID: return TEXT("GUILD_ERROR_ALLY_GUILD_ID");
		case Result::GUILD_ERROR_ALLY_REQUEST_GUILD_ID: return TEXT("GUILD_ERROR_ALLY_REQUEST_GUILD_ID");
		case Result::GUILD_ERROR_UNKNOWN_GUILD_ID: return TEXT("GUILD_ERROR_UNKNOWN_GUILD_ID");
		case Result::GUILD_ERROR_MAX_REQUESTED: return TEXT("GUILD_ERROR_MAX_REQUESTED");
		case Result::GUILD_JOIN_ERROR_UNABLE: return TEXT("GUILD_JOIN_ERROR_UNABLE");
		case Result::GUILD_JOIN_ERROR_EXIST_LIST: return TEXT("GUILD_JOIN_ERROR_EXIST_LIST");
		case Result::GUILD_ERROR_NOT_JOIN: return TEXT("GUILD_ERROR_NOT_JOIN");
		case Result::GUILD_ERROR_INVALID_GUILD_CHAT_ID: return TEXT("GUILD_ERROR_INVALID_GUILD_CHAT_ID");
		case Result::GUILD_SEARCH_ERROR_COOLDOWN: return TEXT("GUILD_SEARCH_ERROR_COOLDOWN");
		case Result::GUILD_ERROR_GUILD_ASSET_FULLY: return TEXT("GUILD_ERROR_GUILD_ASSET_FULLY");
		case Result::GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_HOSTILE_GUILD: return TEXT("GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_HOSTILE_GUILD");
		case Result::GUILD_SEARCH_DILPLOMACY_ERROR_REQUEST_ALLY_GUILD: return TEXT("GUILD_SEARCH_DILPLOMACY_ERROR_REQUEST_ALLY_GUILD");
		case Result::GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_ALLY_GUILD: return TEXT("GUILD_SEARCH_DILPLOMACY_ERROR_EXIST_ALLY_GUILD");
		case Result::GUILD_SEARCH_ERROR_MATCHING: return TEXT("GUILD_SEARCH_ERROR_MATCHING");
		case Result::GUILD_ERROR_EXIST_ALLY_GUILD: return TEXT("GUILD_ERROR_EXIST_ALLY_GUILD");
		case Result::GUILD_ERROR_EXIST_ALLY_REQUEST_GUILD: return TEXT("GUILD_ERROR_EXIST_ALLY_REQUEST_GUILD");
		case Result::GUILD_ERROR_SAME_GUILD_ID: return TEXT("GUILD_ERROR_SAME_GUILD_ID");
		case Result::GUILD_ERROR_UNAUTHORIZED_DIPLOMACY: return TEXT("GUILD_ERROR_UNAUTHORIZED_DIPLOMACY");
		case Result::GUILD_ERROR_HOSTILE_GUILD_ID: return TEXT("GUILD_ERROR_HOSTILE_GUILD_ID");
		case Result::GUILD_ERROR_ERROR_FULL: return TEXT("GUILD_ERROR_ERROR_FULL");
		case Result::GUILD_ERROR_OPPONENT_FULL: return TEXT("GUILD_ERROR_OPPONENT_FULL");
		case Result::GUILD_ERROR_BUILDING_REACTIVATE_TIME: return TEXT("GUILD_ERROR_BUILDING_REACTIVATE_TIME");
		case Result::GUILD_GIFT_ERROR_INVALID_ITEM_EFFECT_PARAM: return TEXT("GUILD_GIFT_ERROR_INVALID_ITEM_EFFECT_PARAM");
		case Result::GUILD_CREATE_ERROR_INSERT_GUILD: return TEXT("GUILD_CREATE_ERROR_INSERT_GUILD");
		case Result::GUILD_CREATE_ERROR_INSERT_GUILD_ROW_COUNT: return TEXT("GUILD_CREATE_ERROR_INSERT_GUILD_ROW_COUNT");
		case Result::GUILD_CREATE_ERROR_INSERT_GUILD_MEMBER: return TEXT("GUILD_CREATE_ERROR_INSERT_GUILD_MEMBER");
		case Result::GUILD_CREATE_ERROR_DELETE_GUILD_REQUEST_JOIN: return TEXT("GUILD_CREATE_ERROR_DELETE_GUILD_REQUEST_JOIN");
		case Result::GUILD_DUNGEON_WARP_ERROR_NOT_ENOUGH_TICKET: return TEXT("GUILD_DUNGEON_WARP_ERROR_NOT_ENOUGH_TICKET");
		case Result::GUILD_DUNGEON_WARP_ERROR_IN_GUILD_DUNGEON_MAP: return TEXT("GUILD_DUNGEON_WARP_ERROR_IN_GUILD_DUNGEON_MAP");
		case Result::GUILD_DUNGEON_WARP_ERROR_STORE_FULL: return TEXT("GUILD_DUNGEON_WARP_ERROR_STORE_FULL");
		case Result::GUILD_DUNGEON_WARP_ERROR_DONT_DESTROY: return TEXT("GUILD_DUNGEON_WARP_ERROR_DONT_DESTROY");
		case Result::GUILD_DUNGEON_WARP_ERROR_LEVEL: return TEXT("GUILD_DUNGEON_WARP_ERROR_LEVEL");
		case Result::GUILD_DUNGEON_WARP_ERROR_ABNORMALITY_EFFECT: return TEXT("GUILD_DUNGEON_WARP_ERROR_ABNORMALITY_EFFECT");
		case Result::GUILD_DUNGEON_WATCHING_ERROR_MAP: return TEXT("GUILD_DUNGEON_WATCHING_ERROR_MAP");
		case Result::GUILD_DUNGEON_WATCHING_NO_ONE_HERE: return TEXT("GUILD_DUNGEON_WATCHING_NO_ONE_HERE");
		case Result::GUILD_DUNGEON_RESET_TIME: return TEXT("GUILD_DUNGEON_RESET_TIME");
		case Result::GUILD_DUNGEON_WARP_ERROR_ENOUGH_GUILD_LEVEL: return TEXT("GUILD_DUNGEON_WARP_ERROR_ENOUGH_GUILD_LEVEL");
		case Result::GUILD_DUNGEON_WARP_ERROR_THE_DAY_JOINED_GUILD: return TEXT("GUILD_DUNGEON_WARP_ERROR_THE_DAY_JOINED_GUILD");
		case Result::GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER: return TEXT("GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER");
		case Result::GUILD_DUNGEON_WARP_ERROR_ANOTHER_GUILD_LEVEL: return TEXT("GUILD_DUNGEON_WARP_ERROR_ANOTHER_GUILD_LEVEL");
		case Result::GUILD_ERROR_INTER_ALLY_PREPARING: return TEXT("GUILD_ERROR_INTER_ALLY_PREPARING");
		case Result::GUILD_ERROR_INTER_ALLY_REQUEST_UNKNOWN_GUILD_ID: return TEXT("GUILD_ERROR_INTER_ALLY_REQUEST_UNKNOWN_GUILD_ID");
		case Result::GUILD_ERROR_INTER_ALLY_REQUEST_ERROR_DB: return TEXT("GUILD_ERROR_INTER_ALLY_REQUEST_ERROR_DB");
		case Result::GUILD_ERROR_INTER_ALLY_ACCEPT_UNKNOWN_GUILD_ID: return TEXT("GUILD_ERROR_INTER_ALLY_ACCEPT_UNKNOWN_GUILD_ID");
		case Result::GUILD_ERROR_INTER_ALLY_ACCEPT_ERROR_DB: return TEXT("GUILD_ERROR_INTER_ALLY_ACCEPT_ERROR_DB");
		case Result::GUILD_LEAVE_ERROR_CANNOT_STATE: return TEXT("GUILD_LEAVE_ERROR_CANNOT_STATE");
		case Result::AGIT_WARP_ERROR_IN_AGIT_MAP: return TEXT("AGIT_WARP_ERROR_IN_AGIT_MAP");
		case Result::AGIT_WARP_ERROR_EXPIRE: return TEXT("AGIT_WARP_ERROR_EXPIRE");
		case Result::AGIT_WARP_ERROR_NOT_AGIT_MAP: return TEXT("AGIT_WARP_ERROR_NOT_AGIT_MAP");
		case Result::AGIT_INVALID_USER: return TEXT("AGIT_INVALID_USER");
		case Result::AGIT_ERROR_NO_MORE_RENTAL_EXPANSION: return TEXT("AGIT_ERROR_NO_MORE_RENTAL_EXPANSION");
		case Result::AGIT_INVALID_RENTAL: return TEXT("AGIT_INVALID_RENTAL");
		case Result::AGIT_ERROR_NOT_RENTAING: return TEXT("AGIT_ERROR_NOT_RENTAING");
		case Result::ITEM_ERROR_REFINE_INVALID_TARGET: return TEXT("ITEM_ERROR_REFINE_INVALID_TARGET");
		case Result::ITEM_ERROR_REFINE_INVALID_STAT_DATA: return TEXT("ITEM_ERROR_REFINE_INVALID_STAT_DATA");
		case Result::ITEM_ERROR_REFINE_OPTION_STONE_NOT_MEET_THE_CONDITION: return TEXT("ITEM_ERROR_REFINE_OPTION_STONE_NOT_MEET_THE_CONDITION");
		case Result::ITEM_ERROR_REFINE_LOCKED: return TEXT("ITEM_ERROR_REFINE_LOCKED");
		case Result::ARCAS_ERROR_INVALID_STATUS: return TEXT("ARCAS_ERROR_INVALID_STATUS");
		case Result::ARCAS_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("ARCAS_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::ARCAS_ERROR_MEDITATION: return TEXT("ARCAS_ERROR_MEDITATION");
		case Result::ARCAS_ERROR_NOT_MEDITATION: return TEXT("ARCAS_ERROR_NOT_MEDITATION");
		case Result::ARCAS_ERROR_NOT_REMOVE_ARCAS_BUFF: return TEXT("ARCAS_ERROR_NOT_REMOVE_ARCAS_BUFF");
		case Result::ARCAS_ERROR_ARCAS_BUFF_ON: return TEXT("ARCAS_ERROR_ARCAS_BUFF_ON");
		case Result::ARCAS_ERROR_NOT_USE_ARCAS_POINT_GAIN: return TEXT("ARCAS_ERROR_NOT_USE_ARCAS_POINT_GAIN");
		case Result::ARCAS_ERROR_INVALID_ARCAS_POINT_DATA: return TEXT("ARCAS_ERROR_INVALID_ARCAS_POINT_DATA");
		case Result::ARCAS_ERROR_NOT_ENOUGH_ARCAS_POINT: return TEXT("ARCAS_ERROR_NOT_ENOUGH_ARCAS_POINT");
		case Result::ARCAS_ERROR_INVALID_ARCAS_POINT_AND_NOT_EXIST_FAIRY_BUFF_ABNORMALITY: return TEXT("ARCAS_ERROR_INVALID_ARCAS_POINT_AND_NOT_EXIST_FAIRY_BUFF_ABNORMALITY");
		case Result::ARCAS_ERROR_INVALID_PASSIVITY_ID: return TEXT("ARCAS_ERROR_INVALID_PASSIVITY_ID");
		case Result::ARCAS_ERROR_NOT_ITEM_USE_MAX_ARCAS_POINT: return TEXT("ARCAS_ERROR_NOT_ITEM_USE_MAX_ARCAS_POINT");
		case Result::USER_COMMON_ERROR_INVALID_STAT_POINT_DATA: return TEXT("USER_COMMON_ERROR_INVALID_STAT_POINT_DATA");
		case Result::USER_COMMON_ERROR_INVALID_USER: return TEXT("USER_COMMON_ERROR_INVALID_USER");
		case Result::COSTUME_ERROR_UNKNOWN: return TEXT("COSTUME_ERROR_UNKNOWN");
		case Result::COSTUME_ERROR_INVALID_STATUS: return TEXT("COSTUME_ERROR_INVALID_STATUS");
		case Result::COSTUME_ERROR_INVALID_COSTUME_ID: return TEXT("COSTUME_ERROR_INVALID_COSTUME_ID");
		case Result::COSTUME_ERROR_NOT_HAVE_COSTUME: return TEXT("COSTUME_ERROR_NOT_HAVE_COSTUME");
		case Result::COSTUME_ERROR_ALREADY_EQUIP_COSTUME: return TEXT("COSTUME_ERROR_ALREADY_EQUIP_COSTUME");
		case Result::COSTUME_ERROR_NOT_ENOUGH_COSTUME: return TEXT("COSTUME_ERROR_NOT_ENOUGH_COSTUME");
		case Result::COSTUME_ERROR_MAX_AMOUNT_OVER: return TEXT("COSTUME_ERROR_MAX_AMOUNT_OVER");
		case Result::COSTUME_ERROR_INVALID_DATA: return TEXT("COSTUME_ERROR_INVALID_DATA");
		case Result::COSTUME_ERROR_INVALID_PACKET_DATA: return TEXT("COSTUME_ERROR_INVALID_PACKET_DATA");
		case Result::COSTUME_SUMMON_ERROR_INVALID_COSTUME_GRADE: return TEXT("COSTUME_SUMMON_ERROR_INVALID_COSTUME_GRADE");
		case Result::COSTUME_SUMMON_ERROR_MAX_CONFIRM_COUNT: return TEXT("COSTUME_SUMMON_ERROR_MAX_CONFIRM_COUNT");
		case Result::COSTUME_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM: return TEXT("COSTUME_SUMMON_ERROR_INVALID_ITEM_EFFECT_PARAM");
		case Result::COSTUME_CONFRIM_ERROR_INVALID_COSTUME_CONFIRM_ID: return TEXT("COSTUME_CONFRIM_ERROR_INVALID_COSTUME_CONFIRM_ID");
		case Result::COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT: return TEXT("COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CHANGE_COUNT");
		case Result::COSTUME_CONFRIM_ERROR_EXPIRED: return TEXT("COSTUME_CONFRIM_ERROR_EXPIRED");
		case Result::COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("COSTUME_CONFRIM_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::COSTUME_ENCHANT_ERROR_MAX_LEVEL_OVER: return TEXT("COSTUME_ENCHANT_ERROR_MAX_LEVEL_OVER");
		case Result::COSTUME_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL: return TEXT("COSTUME_ENCHANT_ERROR_NOT_ENOUGH_MATERIAL");
		case Result::COSTUME_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("COSTUME_ENCHANT_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::COSTUME_COMPOSE_ERROR_NOT_SAME_GRADE: return TEXT("COSTUME_COMPOSE_ERROR_NOT_SAME_GRADE");
		case Result::COSTUME_COMPOSE_ERROR_MAX_COMPOSE_COUNT: return TEXT("COSTUME_COMPOSE_ERROR_MAX_COMPOSE_COUNT");
		case Result::COSTUME_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT: return TEXT("COSTUME_COMPOSE_ERROR_MAX_MULTIPLE_COMPOSE_COUNT");
		case Result::COSTUME_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("COSTUME_COMPOSE_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::COSTUME_COMPOSE_ERROR_LEGEND_GRADE: return TEXT("COSTUME_COMPOSE_ERROR_LEGEND_GRADE");
		case Result::COSTUME_SKILL_ERROR_NOT_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_NOT_COSTUME_SKILL");
		case Result::COSTUME_SKILL_ERROR_NOT_COSTUME_SKILl_ID: return TEXT("COSTUME_SKILL_ERROR_NOT_COSTUME_SKILl_ID");
		case Result::COSTUME_SKILL_ERROR_ALREADY_LEARNED_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_ALREADY_LEARNED_COSTUME_SKILL");
		case Result::COSTUME_SKILL_ERROR_NOT_LEARNED_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_NOT_LEARNED_COSTUME_SKILL");
		case Result::COSTUME_SKILL_ERROR_INVALID_SKILL_ID: return TEXT("COSTUME_SKILL_ERROR_INVALID_SKILL_ID");
		case Result::COSTUME_SKILL_ERROR_FAIL_TO_SET_SKILL_SLOT: return TEXT("COSTUME_SKILL_ERROR_FAIL_TO_SET_SKILL_SLOT");
		case Result::COSTUME_SKILL_ERROR_NO_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_NO_COSTUME_SKILL");
		case Result::COSTUME_SKILL_ERROR_NO_EQUIPED_COSTUME_SKILL: return TEXT("COSTUME_SKILL_ERROR_NO_EQUIPED_COSTUME_SKILL");
		case Result::COSTUME_CHANGE_ERROR_INVALID_GRADE: return TEXT("COSTUME_CHANGE_ERROR_INVALID_GRADE");
		case Result::COSTUME_CHANGE_ERROR_EXCLUDE_ID: return TEXT("COSTUME_CHANGE_ERROR_EXCLUDE_ID");
		case Result::COSTUME_CHANGE_ERROR_SAME_COSTUME_ID: return TEXT("COSTUME_CHANGE_ERROR_SAME_COSTUME_ID");
		case Result::COSTUME_CHANGE_ERROR_NOT_IN_PROBABILITY_POOL: return TEXT("COSTUME_CHANGE_ERROR_NOT_IN_PROBABILITY_POOL");
		case Result::COSTUME_CHANGE_ERROR_SELECT_COSTUME: return TEXT("COSTUME_CHANGE_ERROR_SELECT_COSTUME");
		case Result::COSTUME_CHANGE_ERROR_NOT_SELECT_COSTUME: return TEXT("COSTUME_CHANGE_ERROR_NOT_SELECT_COSTUME");
		case Result::DUNGEON_ERROR_UNKNOWN: return TEXT("DUNGEON_ERROR_UNKNOWN");
		case Result::DUNGEON_ERROR_INVALID_DUNGEON_GROUP_ID: return TEXT("DUNGEON_ERROR_INVALID_DUNGEON_GROUP_ID");
		case Result::DUNGEON_ERROR_INVALID_DUNGEON_LIST_IDX: return TEXT("DUNGEON_ERROR_INVALID_DUNGEON_LIST_IDX");
		case Result::DUNGEON_ERROR_NOT_ENOUGH_COST: return TEXT("DUNGEON_ERROR_NOT_ENOUGH_COST");
		case Result::DUNGEON_ERROR_INVALID_STATUS: return TEXT("DUNGEON_ERROR_INVALID_STATUS");
		case Result::DUNGEON_ERROR_NOT_ENOUGH_TIME: return TEXT("DUNGEON_ERROR_NOT_ENOUGH_TIME");
		case Result::DUNGEON_ERROR_TIME_OVERFLOW: return TEXT("DUNGEON_ERROR_TIME_OVERFLOW");
		case Result::DUNGEON_ERROR_NOT_OPEN_TIME: return TEXT("DUNGEON_ERROR_NOT_OPEN_TIME");
		case Result::DUNGEON_ERROR_DISABLE_TIME_CHARGE: return TEXT("DUNGEON_ERROR_DISABLE_TIME_CHARGE");
		case Result::DEPOT_ERROR_UNKNOWN: return TEXT("DEPOT_ERROR_UNKNOWN");
		case Result::DEPOT_ERROR_NOT_ENOUGH_MONEY: return TEXT("DEPOT_ERROR_NOT_ENOUGH_MONEY");
		case Result::DEPOT_EXTEND_ERROR_UNKNOWN: return TEXT("DEPOT_EXTEND_ERROR_UNKNOWN");
		case Result::DEPOT_EXTEND_ERROR_MAX_SLOT: return TEXT("DEPOT_EXTEND_ERROR_MAX_SLOT");
		case Result::DEPOT_EXTEND_ERROR_NOT_ENOUGH_MONEY: return TEXT("DEPOT_EXTEND_NOT_ENOUGH_MONEY");
		case Result::DEPOT_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("DEPOT_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::DEPOT_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("DEPOT_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::DEPOT_ERROR_INVALID_USER_STATE: return TEXT("DEPOT_ERROR_INVALID_USER_STATE");
		case Result::DEPOT_ERROR_INVALID_USER: return TEXT("DEPOT_ERROR_INVALID_USER");
		case Result::DEPOT_ERROR_DONT_MOVE_ITEM: return TEXT("DEPOT_ERROR_DONT_MOVE_ITEM");
		case Result::ITEM_COLLECTION_ERROR_INVALID_COLLECTION_ID: return TEXT("ITEM_COLLECTION_INVALID_COLLECTION_ID");
		case Result::ITEM_COLLECTION_ERROR_INVALID_COLLECTION_CONDITION_ID: return TEXT("ITEM_COLLECTION_INVALID_COLLECTION_CONDITION_ID");
		case Result::ITEM_COLLECTION_ERROR_NOT_SUITABLE_ITEM: return TEXT("ITEM_COLLECTION_ERROR_NOT_SUITABLE_ITEM");
		case Result::ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_ITEM: return TEXT("ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_ITEM");
		case Result::ITEM_COLLECTION_ERROR_INVALID_CATEGORY_ID: return TEXT("ITEM_COLLECTION_ERROR_INVALID_CATEGORY_ID");
		case Result::ITEM_COLLECTION_ERROR_INVALID_UNLOCK_TYPE: return TEXT("ITEM_COLLECTION_ERROR_INVALID_UNLOCK_TYPE");
		case Result::ITEM_COLLECTION_ERROR_DOES_NOT_MEET_THE_UNLOCK_CONDITION: return TEXT("ITEM_COLLECTION_ERROR_DOES_NOT_MEET_THE_UNLOCK_CONDITION");
		case Result::ITEM_COLLECTION_ERROR_INVALID_QUEST_TYPE: return TEXT("ITEM_COLLECTION_ERROR_INVALID_QUEST_TYPE");
		case Result::ITEM_COLLECTION_ERROR_EXPIRE_DATE_TIME: return TEXT("ITEM_COLLECTION_ERROR_EXPIRE_DATE_TIME");
		case Result::ITEM_COLLECTION_ERROR_INVALID_BOOKMARK: return TEXT("ITEM_COLLECTION_ERROR_INVALID_BOOKMARK");
		case Result::SPIRIT_SHOT_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("SPIRIT_SHOT_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::SPIRIT_SHOT_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE: return TEXT("SPIRIT_SHOT_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE");
		case Result::SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_CATEGORY_ID: return TEXT("SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_CATEGORY_ID");
		case Result::SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_LEVEL_ID: return TEXT("SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_LEVEL_ID");
		case Result::SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_NEXT_LEVEL_ID: return TEXT("SPIRIT_SHOT_ERROR_INVALID_SPIRIT_SHOT_NEXT_LEVEL_ID");
		case Result::SPIRIT_SHOT_ERROR_INVALID_MODIFY_ENCHANT_RATE_COUNT: return TEXT("SPIRIT_SHOT_ERROR_INVALID_MODIFY_ENCHANT_RATE_COUNT");
		case Result::SPIRIT_SHOT_ERROR_INVALID_ITEM_COUNT: return TEXT("SPIRIT_SHOT_ERROR_INVALID_ITEM_COUNT");
		case Result::SPIRIT_SHOT_ERROR_ALREADY_SAME_STATE_AS_BEFORE: return TEXT("SPIRIT_SHOT_ERROR_ALREADY_SAME_STATE_AS_BEFORE");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_INVALID_STATUS: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_INVALID_STATUS");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_ALREADY_SAME_STATE_AS_BEFORE: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_ALREADY_SAME_STATE_AS_BEFORE");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_LEVEL: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_LEVEL");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_ITEM: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_ITEM");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_UNLOCK_CONTENTS: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_UNLOCK_CONTENTS");
		case Result::SPIRIT_SHOT_ENCHANT_ERROR_NOT_ENOUGH_MONEY: return TEXT("SPIRIT_SHOT_ENCHANT_ERROR_NOT_ENOUGH_MONEY");
		case Result::POTION_AUTO_USE_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("POTION_AUTO_USE_GATEWAY_OPERATION_FAILED");
		case Result::POTION_AUTO_USE_ERROR_ALREADY_SAME_STATE_AS_BEFORE: return TEXT("POTION_AUTO_USE_ERROR_ALREADY_SAME_STATE_AS_BEFORE");
		case Result::BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::BM_SHOP_ERROR_UNKNOWN: return TEXT("BM_SHOP_ERROR_UNKNOWN");
		case Result::BM_SHOP_NO_GATEWAY_SERVER_CONNECTION: return TEXT("BM_SHOP_NO_GATEWAY_SERVER_CONNECTION");
		case Result::BM_SHOP_ERROR_INVALID_USER: return TEXT("BM_SHOP__ERROR_INVALID_USER");
		case Result::BM_SHOP_ERROR_IDENDTIFIER: return TEXT("BM_SHOP_ERROR_IDENDTIFIER");
		case Result::BM_SHOP_ERROR_NOT_HIVE_SYNC_MARKET_PID: return TEXT("BM_SHOP_ERROR_NOT_HIVE_SYNC_MARKET_PID");
		case Result::BM_SHOP_ERROR_INVALID_USER_STATUS: return TEXT("BM_SHOP__ERROR_INVALID_USER_STATUS");
		case Result::BM_SHOP_ERROR_NOT_ALLOWED_LEVEL: return TEXT("BM_SHOP_ERROR_NOT_ALLOWED_LEVEL");
		case Result::BM_SHOP_ERROR_NOT_ALLOWED_GUILD_LEVEL: return TEXT("BM_SHOP_ERROR_NOT_ALLOWED_GUILD_LEVEL");
		case Result::BM_SHOP_ERROR_ITEM_NOT_FOUND: return TEXT("BM_SHOP_ERROR_ITEM_NOT_FOUND");
		case Result::BM_SHOP_ERROR_INVALID_ITEM_ID: return TEXT("BM_SHOP_ERROR_INVALID_ITEM_ID");
		case Result::BM_SHOP_ERROR_PRODCUT_NOT_FOUND: return TEXT("BM_SHOP_ERROR_PRODCUT_NOT_FOUND");
		case Result::BM_SHOP_ERROR_INVALID_PRODCUT_ID: return TEXT("BM_SHOP_ERROR_INVALID_PRODCUT_ID");
		case Result::BM_SHOP_ERROR_INVALID_TAB_ID: return TEXT("BM_SHOP_ERROR_INVALID_TAB_ID");
		case Result::BM_SHOP_ERROR_NOT_ENOUGH_MONEY: return TEXT("BM_SHOP_ERROR_NOT_ENOUGH_MONEY");
		case Result::BM_SHOP_ERROR_OVERFLOW_MAX_MONEY: return TEXT("BM_SHOP_OVERFLOW_MAX_MONEY");
		case Result::BM_SHOP_ERROR_OVER_QUANTIY: return TEXT("BM_SHOP_ERROR_OVER_QUANTIY");
		case Result::BM_SHOP_ERROR_SALE_DATETIME: return TEXT("BM_SHOP_ERROR_SALE_DATETIME");
		case Result::BM_SHOP_ERROR_CASH_PURCHASE: return TEXT("BM_SHOP_ERROR_CASH_PURCHASE");
		case Result::BM_SHOP_ERROR_PURCHASE: return TEXT("BM_SHOP_ERROR_PURCHASE");
		case Result::BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID: return TEXT("BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID");
		case Result::BM_SHOP_ERROR_DB_OPERATION_FAILED: return TEXT("BM_SHOP_ERROR_DB_OPERATION_FAILED");
		case Result::BM_SHOP_ERROR_HIVE_OPERATION_FAILED: return TEXT("BM_SHOP_ERROR_HIVE_OPERATION_FAILED");
		case Result::BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT: return TEXT("BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT");
		case Result::BM_SHOP_ERROR_IMPOSSIBLE_ITEM: return TEXT("BM_SHOP_ERROR_IMPOSSIBLE_ITEM");
		case Result::BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY: return TEXT("");
		case Result::BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT: return TEXT("");
		case Result::BM_SHOP_DONT_TRY_SAVE_DB: return TEXT("");
		case Result::BM_SHOP_ERROR_MONEY: return TEXT("BM_SHOP_ERROR__MONEY");
		case Result::BM_SHOP_ERROR_SAVE_BULK_SALE: return TEXT("BM_SHOP_ERROR_SAVE_BULK_SALE");
		case Result::BM_SHOP_ERROR_CASH_ITEM_ONE_AT_TIME: return TEXT("BM_SHOP_ERROR_CASH_ITEM_ONE_AT_TIME_FOR");
		case Result::BM_SHOP_ERROR_BULK_SALE_ITEM: return TEXT("BM_SHOP_ERROR_BULK_SALE_ITEM");
		case Result::BM_SHOP_ERROR_POPUP_FAILED: return TEXT("BM_SHOP_ERROR_POPUP_FAILED");
		case Result::BM_SHOP_ERROR_STEPUP_FAILED: return TEXT("BM_SHOP_ERROR_STEPUP_FAILED");
		case Result::BM_SHOP_DUPLICATION_COST_ITEM: return TEXT("BM_SHOP_DUPLICATION_COST_ITEM");
		case Result::BM_SHOP_COUPON_ITEM_INVALID: return TEXT("BM_SHOP_COUPON_ITEM_INVALID");
		case Result::BM_SHOP_CAN_NOT_COUPON_ITEM_FOR_CASH: return TEXT("BM_SHOP_CAN_NOT_COUPON_ITEM_FOR_CASH");
		case Result::BM_SHOP_CAN_NOT_COUPON_ITEM: return TEXT("BM_SHOP_CAN_NOT_COUPON_ITEM");
		case Result::BM_SHOP_ERROR_PURCHASE_OHTER_ACCOUNT: return TEXT("BM_SHOP_ERROR_PURCHASE_OHTER_ACCOUNT");
		case Result::BM_SHOP_ERROR_PRODCUT_OOAP_NOT_FOUND: return TEXT("BM_SHOP_ERROR_PRODCUT_OOAP_NOT_FOUND");
		case Result::BM_SHOP_RESTORE_ERROR_ITEM_NOT_FOUND: return TEXT("BM_SHOP_RESTORE_ERROR_ITEM_NOT_FOUND");
		case Result::BM_SHOP_RESTORE_ERROR_PRODCUT_NOT_FOUND: return TEXT("BM_SHOP_RESTORE_ERROR_PRODCUT_NOT_FOUND");
		case Result::BM_SHOP_RESTORE_ERROR_NOT_HIVE_SYNC_MARKET_PID: return TEXT("BM_SHOP_RESTORE_ERROR_NOT_HIVE_SYNC_MARKET_PID");
		case Result::BM_SHOP_ERROR_GATEWAY_FIND_USER: return TEXT("BM_SHOP_ERROR_GATEWAY_FIND_USER");
		case Result::BM_SHOP_ERROR_GATEWAY_INVALID_APPID: return TEXT("BM_SHOP_ERROR_GATEWAY_INVALID_APPID");
		case Result::BM_SHOP_ERROR_PG_UNLOCK: return TEXT("BM_SHOP_ERROR_PG_UNLOCK");
		case Result::BM_SHOP_ERROR_PG_UNCONSUMED_COOLTIME: return TEXT("BM_SHOP_ERROR_PG_UNCONSUMED_COOLTIME");
		case Result::SPACE_CRACK_ERROR_PLAY_LIMIT_COUNT: return TEXT("SPACE_CRACK_ERROR_PLAY_LIMIT_COUNT");
		case Result::SPACE_CRACK_ERROR_REWARD_COMPLETED: return TEXT("SPACE_CRACK_ERROR_REWARD_COMPLETED+Completed");
		case Result::SPACE_CRACK_ERROR_REWARD_DATA_ID: return TEXT("SPACE_CRACK_ERROR_REWARD_DATA_ID");
		case Result::SPACE_CRACK_ERROR_REWARD_BOX_DATA_ID: return TEXT("SPACE_CRACK_ERROR_REWARD_BOX_DATA_ID");
		case Result::SPACE_CRACK_ERROR_ENTRY_TIME: return TEXT("SPACE_CRACK_ERROR_ENTRY_TIME");
		case Result::MULTI_LEVEL_RANK_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("MULTI_LEVEL_RANK_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::MULTI_LEVEL_RANK_ERROR_INVALID_STATUS: return TEXT("MULTI_LEVEL_RANK_ERROR_INVALID_STATUS");
		case Result::MULTI_LEVEL_RANK_ERROR_PROMOTION_NOT_MEET_THE_CONDITION: return TEXT("MULTI_LEVEL_RANK_ERROR_PROMOTION_NOT_MEET_THE_CONDITION");
		case Result::MULTI_LEVEL_RANK_ERROR_PROMOTION_PROCESS_FAILED: return TEXT("MULTI_LEVEL_RANK_ERROR_PROMOTION_PROCESS_FAILED");
		case Result::MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_MEET_THE_CONDITION: return TEXT("MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_MEET_THE_CONDITION");
		case Result::MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_ENOUGH_MONEY: return TEXT("MULTI_LEVEL_RANK_ERROR_CHANGE_RANK_NOT_ENOUGH_MONEY");
		case Result::MULTI_LEVEL_RANK_ERROR_ACCEPT_QUEST_NOT_ENOUGH_REQUIRE_EXP: return TEXT("MULTI_LEVEL_RANK_ERROR_ACCEPT_QUEST_NOT_ENOUGH_REQUIRE_EXP");
		case Result::MULTI_LEVEL_RANK_ERROR_ACCEPT_BEFORE_RANK_NOT_CLEAR_STATE: return TEXT("MULTI_LEVEL_RANK_ERROR_ACCEPT_BEFORE_RANK_NOT_CLEAR_STATE");
		case Result::MULTI_LEVEL_RANK_ERROR_ACCEPT_ALREADY_ACCEPTED_QUEST: return TEXT("MULTI_LEVEL_RANK_ERROR_ACCEPT_ALREADY_ACCEPTED_QUEST");
		case Result::MULTI_LEVEL_RANK_ERROR_REWARD_RANK_NOT_MEET_THE_CONDITION: return TEXT("MULTI_LEVEL_RANK_ERROR_REWARD_RANK_NOT_MEET_THE_CONDITION");
		case Result::MULTI_LEVEL_RANK_ERROR_ALREADY_RECEIVED_REWARD_RANK: return TEXT("MULTI_LEVEL_RANK_ERROR_ALREADY_RECEIVED_REWARD_RANK");
		case Result::MARKET_ERROR_UNKNOWN: return TEXT("MARKET_ERROR_UNKNOWN");
		case Result::MARKET_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("MARKET_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::MARKET_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("MARKET_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::MARKET_ERROR_NOT_TRADABLE_ITEM: return TEXT("MARKET_ERROR_NOT_TRADABLE_ITEM");
		case Result::MARKET_ERROR_ITEM_REGISTER_COUNT_OVER: return TEXT("MARKET_ERROR_ITEM_REGISTER_COUNT_OVER");
		case Result::MARKET_ERROR_NOT_ENOUGH_ITEM_COUNT: return TEXT("MARKET_ERROR_NOT_ENOUGH_ITEM_COUNT");
		case Result::MARKET_ERROR_CANT_REGISTER_EQUIPED_ITEM: return TEXT("MARKET_ERROR_CANT_REGISTER_EQUIPED_ITEM");
		case Result::MARKET_ERROR_NOT_ENOUGH_REGISTER_FEE: return TEXT("MARKET_ERROR_NOT_ENOUGH_REGISTER_FEE");
		case Result::MARKET_ERROR_INVALID_REGISTER_PRICE: return TEXT("MARKET_ERROR_INVALID_REGISTER_PRICE");
		case Result::MARKET_ERROR_INVALID_MARKET_TRANSACTION: return TEXT("MARKET_ERROR_INVALID_MARKET_TRANSACTION");
		case Result::MARKET_ERROR_EXCESS_DIA: return TEXT("MARKET_ERROR_EXCESS_DIA");
		case Result::MARKET_ERROR_NOT_FOUND_TRANSACTION: return TEXT("MARKET_ERROR_NOT_FOUND_TRANSACTION");
		case Result::MARKET_ERROR_CANT_PURCHASE_MY_TRANSACTION: return TEXT("MARKET_ERROR_CANT_PURCHASE_MY_TRANSACTION");
		case Result::MARKET_ERROR_NOT_ENOUGH_DIA: return TEXT("MARKET_ERROR_NOT_ENOUGH_DIA");
		case Result::MARKET_ERROR_CANT_CANCEL_OTHER_TRANSACTION: return TEXT("MARKET_ERROR_CANT_CANCEL_OTHER_TRANSACTION");
		case Result::MARKET_ERROR_INVALIDE_MARKET_CATEGORY: return TEXT("MARKET_ERROR_INVALIDE_MARKET_CATEGORY");
		case Result::MARKET_ERROR_INVALIDE_ITEM_GRADE: return TEXT("MARKET_ERROR_INVALIDE_ITEM_GRADE");
		case Result::MARKET_ERROR_INVALIDE_PLAYING_USER: return TEXT("MARKET_ERROR_INVALIDE_PLAYING_USER");
		case Result::MARKET_ERROR_INVALIDE_ITEM_ID: return TEXT("MARKET_ERROR_INVALIDE_ITEM_ID");
		case Result::MARKET_ERROR_NOT_EXIST_ITEM: return TEXT("MARKET_ERROR_NOT_EXIST_ITEM");
		case Result::MARKET_ERROR_INVALID_ITEM_LEVEL: return TEXT("MARKET_ERROR_INVALID_ITEM_LEVEL");
		case Result::MARKET_ERROR_IS_NOT_EQUIP_ITEM: return TEXT("MARKET_ERROR_IS_NOT_EQUIP_ITEM");
		case Result::MARKET_ERROR_NO_UPDATED_TRANSACTION: return TEXT("MARKET_ERROR_NO_UPDATED_TRANSACTION");
		case Result::MARKET_ERROR_BOOKMARK_LIMIT_EXCEEDED: return TEXT("MARKET_ERROR_BOOKMARK_LIMIT_EXCEEDED");
		case Result::MARKET_ERROR_DB_REQUEST_CALCULATE: return TEXT("MARKET_ERROR_DB_REQUEST_CALCULATE");
		case Result::MARKET_ERROR_CALCULATE: return TEXT("MARKET_ERROR_CALCULATE");
		case Result::MARKET_ERROR_CALCULATE_NOT_PERCHASED_TRANSACTION: return TEXT("MARKET_ERROR_CALCULATE_NOT_PERCHASED_TRANSACTION");
		case Result::MARKET_ERROR_CALCULATE_TRANSACTION_CACULATE: return TEXT("MARKET_ERROR_CALCULATE_TRANSACTION_CACULATE");
		case Result::MARKET_ERROR_CALCULATE_CURRENCY_UPDATE: return TEXT("MARKET_ERROR_CALCULATE_CURRENCY_UPDATE");
		case Result::MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_COMMIT: return TEXT("MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_COMMIT");
		case Result::MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_ROLLBACK: return TEXT("MARKET_ERROR_CALCULATE_CALL_END_TRANSACTION_ROLLBACK");
		case Result::MARKET_ERROR_PURCHASE: return TEXT("MARKET_ERROR_PURCHASE");
		case Result::MARKET_ERROR_DB_REQUEST_MOVE_ITEM: return TEXT("MARKET_ERROR_DB_REQUEST_MOVE_ITEM");
		case Result::MARKET_ERROR_DB_ITEM_MOVE_ON_INVENTORY: return TEXT("MARKET_ERROR_DB_ITEM_MOVE_ON_INVENTORY");
		case Result::MARKET_ERROR_DB_MOVE_ITEM_BY_CREATE: return TEXT("MARKET_ERROR_DB_MOVE_ITEM_BY_CREATE");
		case Result::MARKET_ERROR_DB_MOVE_ITEM_BY_INCREASE: return TEXT("MARKET_ERROR_DB_MOVE_ITEM_BY_INCREASE");
		case Result::MARKET_ERROR_DB_CANCEL_DELETE_TRANSACTION: return TEXT("MARKET_ERROR_DB_CANCEL_DELETE_TRANSACTION");
		case Result::MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION: return TEXT("MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION");
		case Result::MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_DELETED: return TEXT("MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_DELETED");
		case Result::MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_PURCHASED: return TEXT("MARKET_ERROR_DB_PURCHASE_INSERT_MARKET_PURCHASED");
		case Result::MARKET_ERROR_DB_PURCHASE_DELETE_MARKET: return TEXT("MARKET_ERROR_DB_PURCHASE_DELETE_MARKET");
		case Result::MARKET_ERROR_DB_PURCHASE_DB_OPERATION: return TEXT("MARKET_ERROR_DB_PURCHASE_DB_OPERATION");
		case Result::MARKET_ERROR_CANCEL: return TEXT("MARKET_ERROR_CANCEL");
		case Result::MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_COMMIT: return TEXT("MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_COMMIT");
		case Result::MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_ROLLBACK: return TEXT("MARKET_ERROR_DB_MOVE_ITEM_CALL_END_TRANSACTION_ROLLBACK");
		case Result::MARKET_ERROR_CANNOT_USE_GM: return TEXT("MARKET_ERROR_CANNOT_USE_GM");
		case Result::ATTENDANCE_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("ATTENDANCE_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::ATTENDANCE_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("ATTENDANCE_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::ATTENDANCE_ERROR_INVALID_ID: return TEXT("ATTENDANCE_ERROR_INVALID_ID");
		case Result::ATTENDANCE_ERROR_EXPIRED: return TEXT("ATTENDANCE_ERROR_EXPIRED");
		case Result::ATTENDANCE_ERROR_ALREADY_HAVE: return TEXT("ATTENDANCE_ERROR_ALREADY_HAVE");
		case Result::ATTENDANCE_ERROR_NO_HAVE: return TEXT("ATTENDANCE_ERROR_NO_HAVE");
		case Result::ATTENDANCE_ERROR_ALREADY_CHECK_IN: return TEXT("ATTENDANCE_ERROR_ALREADY_CHECK_IN");
		case Result::ATTENDANCE_ERROR_NOT_ENOUGH_MONEY: return TEXT("ATTENDANCE_ERROR_NOT_ENOUGH_MONEY");
		case Result::ATTENDANCE_ERROR_NOT_ENOUGH_ITEM: return TEXT("ATTENDANCE_ERROR_NOT_ENOUGH_ITEM");
		case Result::ATTENDANCE_ERROR_OVERFLOW_MAX_MONEY: return TEXT("ATTENDANCE_ERROR_OVERFLOW_MAX_MONEY");
		case Result::ATTENDANCE_ERROR_NOT_OPEN: return TEXT("ATTENDANCE_ERROR_NOT_OPEN");
		case Result::ATTENDANCE_ERROR_RECEIVE_DURATION: return TEXT("ATTENDANCE_ERROR_RECEIVE_DURATION");
		case Result::NPC_EVENT_SHOP_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("NPC_EVENT_SHOP_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::NPC_EVENT_SHOP_ERROR_UNKNOWN: return TEXT("NPC_EVENT_SHOP_ERROR_UNKNOWN");
		case Result::NPC_EVENT_SHOP_NO_GATEWAY_SERVER_CONNECTION: return TEXT("NPC_EVENT_SHOP_NO_GATEWAY_SERVER_CONNECTION");
		case Result::NPC_EVENT_SHOP_ERROR_INVALID_USER: return TEXT("NPC_EVENT_SHOP__ERROR_INVALID_USER");
		case Result::NPC_EVENT_SHOP_ERROR_INVALID_USER_STATUS: return TEXT("NPC_EVENT_SHOP__ERROR_INVALID_USER_STATUS");
		case Result::NPC_EVENT_SHOP_ERROR_NOT_ALLOWED_LEVEL: return TEXT("NPC_EVENT_SHOP_ERROR_NOT_ALLOWED_LEVEL");
		case Result::NPC_EVENT_SHOP_ERROR_ITEM_NOT_FOUND: return TEXT("NPC_EVENT_SHOP_ERROR_ITEM_NOT_FOUND");
		case Result::NPC_EVENT_SHOP_ERROR_INVALID_ITEM_ID: return TEXT("NPC_EVENT_SHOP_ERROR_INVALID_ITEM_ID");
		case Result::NPC_EVENT_SHOP_ERROR_INVALID_NPC: return TEXT("NPC_EVENT_SHOP_ERROR_INVALID_NPC");
		case Result::NPC_EVENT_SHOP_ERROR_DIFFERENT_CHANNEL: return TEXT("NPC_EVENT_SHOP_ERROR_DIFFERENT_CHANNEL");
		case Result::NPC_EVENT_SHOP_ERROR_NOT_ENOUGH_MONEY: return TEXT("NPC_EVENT_SHOP_ERROR_NOT_ENOUGH_MONEY");
		case Result::NPC_EVENT_SHOP_ERROR_OVERFLOW_MAX_MONEY: return TEXT("NPC_EVENT_SHOP_OVERFLOW_MAX_MONEY");
		case Result::NPC_EVENT_SHOP_ERROR_OVERFLOW_CURRENCY_DELTA: return TEXT("NPC_EVENT_SHOP_ERROR_OVERFLOW_CURRENCY_DELTA");
		case Result::NPC_EVENT_SHOP_ERROR_OVER_QUANTIY: return TEXT("NPC_EVENT_SHOP_ERROR_OVER_QUANTIY");
		case Result::NPC_EVENT_SHOP_ERROR_GUILD_MEMBER_GRADE: return TEXT("NPC_EVENT_SHOP_ERROR_GUILD_MEMBER_GRADE");
		case Result::NPC_EVENT_SHOP_ERROR_GUILD_ID: return TEXT("NPC_EVENT_SHOP_ERROR_GUILD_ID");
		case Result::NPC_EVENT_SHOP_ERROR_GUILD_LEVEL: return TEXT("NPC_EVENT_SHOP_ERROR_GUILD_LEVEL");
		case Result::NPC_EVENT_SHOP_ERROR_SALE_DATETIME: return TEXT("NPC_EVENT_SHOP_ERROR_SALE_DATETIME");
		case Result::NPC_EVENT_SHOP_ERROR_PURCHASE: return TEXT("NPC_EVENT_SHOP_ERROR_PURCHASE");
		case Result::NPC_EVENT_SHOP_DUPLICATION_COST_ITEM: return TEXT("NPC_EVENT_SHOP_DUPLICATION_COST_ITEM");
		case Result::OFF_LINE_PLAY_AUTO_ATTACK: return TEXT("OFF_LINE_PLAY_AUTO_ATTACK");
		case Result::OFF_LINE_PLAY_USER_DEATH: return TEXT("OFF_LINE_PLAY_USER_DEATH");
		case Result::OFF_LINE_PLAY_EXPIRY_TIME: return TEXT("OFF_LINE_PLAY_EXPIRY_TIME");
		case Result::OFF_LINE_PLAY_AREA: return TEXT("OFF_LINE_PLAY_AREA");
		case Result::OFF_LINE_LOAD_ERROR: return TEXT("OFF_LINE_LOAD_ERROR");
		case Result::CLIENT_CINEMATIC_ERROR_INVALID_STATUS: return TEXT("CLIENT_CINEMATIC_ERROR_INVALID_STATUS");
		case Result::CLIENT_CINEMATIC_ALREADY: return TEXT("CLIENT_CINEMATIC_ALREADY");
		case Result::CLIENT_CINEMATIC_END_ERROR_SEQUENCE_RES_ID: return TEXT("CLIENT_CINEMATIC_END_ERROR_SEQUENCE_RES_ID");
		case Result::CLIENT_CINEMATIC_ERROR_NOT_PLAYING: return TEXT("CLIENT_CINEMATIC_ERROR_NOT_PLAYING");
		case Result::CLIENT_CINEMATIC_ERROR_WARP: return TEXT("CLIENT_CINEMATIC_ERROR_WARP");
		case Result::CRAFT_ERROR_INVALID_STATUS: return TEXT("CRAFT_ERROR_INVALID_STATUS");
		case Result::CRAFT_ERROR_EXCEED_LIMIT_COUNT_ACCOUNT: return TEXT("CRAFT_ERROR_EXCEED_LIMIT_COUNT_ACCOUNT");
		case Result::CRAFT_ERROR_LACK_OF_MATERIAL: return TEXT("CRAFT_ERROR_LACK_OF_MATERIAL");
		case Result::CRAFT_ERROR_EXPIRE_DATE_ITEM_CRAFT: return TEXT("CRAFT_ERROR_EXPIRE_DATE_ITEM_CRAFT");
		case Result::CRAFT_ERROR_EXCEED_LIMIT_COUNT_SERVER: return TEXT("CRAFT_ERROR_EXCEED_LIMIT_COUNT_SERVER");
		case Result::CRAFT_ERROR_NOT_MEET_THE_UNLOCK_CONDITION: return TEXT("CRAFT_ERROR_NOT_MEET_THE_UNLOCK_CONDITION");
		case Result::CRAFT_ERROR_EXCEED_MAX_CRAFT_COUNT: return TEXT("CRAFT_ERROR_EXCEED_MAX_CRAFT_COUNT");
		case Result::CRAFT_ERROR_ONLY_SOME_ITEMS_WERE_CRAFTED: return TEXT("CRAFT_ERROR_ONLY_SOME_ITEMS_WERE_CRAFTED");
		case Result::GUILD_STORE_UNKNOWN_ERROR: return TEXT("GUILD_STORE_UNKNOWN_ERROR");
		case Result::GUILD_STORE_ERROR_UNKNOWN_GUILD_ID: return TEXT("GUILD_STORE_ERROR_UNKNOWN_GUILD_ID");
		case Result::GUILD_STORE_ERROR_NOT_HAVE_GRADE_TO_DISTRIBUTE: return TEXT("GUILD_STORE_ERROR_NO_HAVE_GRADE_TO_DISTRIBUTE");
		case Result::GUILD_STORE_ERROR_NOT_GUILD_MEMBER_TO_DIRTIBUTE: return TEXT("GUILD_STORE_ERROR_NO_HAVE_GRADE_TO_DISTRIBUTE");
		case Result::GUILD_STORE_ERROR_GRADE: return TEXT("GUILD_STORE_ERROR_GRADE");
		case Result::GUILD_STORE_ERROR_NO_ITEM_TO_DISTRIBUTE: return TEXT("GUILD_STORE_ERROR_NO_ITEM_TO_DISTRIBUTE");
		case Result::GUILD_STORE_ERROR_NO_GUILD_MEMBER_TO_RECEIVE_ITEM: return TEXT("GUILD_STORE_ERROR_NO_GUILD_MEMBER_TO_RECEIVE_ITEM");
		case Result::GUILD_STORE_ERROR_ITEM_AMOUNT: return TEXT("GUILD_STORE_ERROR_ITEM_AMOUNT");
		case Result::GUILD_STORE_ERROR_EXPIRED_ITEM: return TEXT("GUILD_STORE_ERROR_EXPIRED_ITEM");
		case Result::GUILD_STORE_ERROR_TRY_AGAIN: return TEXT("GUILD_STORE_ERROR_TRY_AGAIN");
		case Result::GUILD_EXTEND_ERROR_NOT_ENOUGH_MONEY: return TEXT("GUILD_EXTEND_ERROR_NOT_ENOUGH_MONEY");
		case Result::GUILD_EXTEND_ERROR_MAX_SLOT: return TEXT("GUILD_EXTEND_ERROR_MAX_SLOT");
		case Result::GUILD_EXTEND_ERROR_FULL: return TEXT("GUILD_EXTEND_ERROR_MAX_SLOT");
		case Result::GUILD_ITEM_COLLECTION_ERROR_UNKNOWN: return TEXT("GUILD_ITEM_COLLECTION_ERROR_UNKNOWN");
		case Result::GUILD_ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_CONDITION: return TEXT("GUILD_ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_CONDITION");
		case Result::GUILD_ITEM_COLLECTION_INVALID_CONDITION_ID: return TEXT("GUILD_ITEM_COLLECTION_INVALID_CONDITION_ID");
		case Result::GUILD_ITEM_COLLECTION_USER_REGISTER_COUNT_LIMIT_OVER: return TEXT("GUILD_ITEM_COLLECTION_USER_REGISTER_COUNT_LIMIT_OVER");
		case Result::GUILD_ITEM_COLLECTION_INACTIVE_EVENT_COLLECTION: return TEXT("GUILD_ITEM_COLLECTION_INACTIVE_EVENT_COLLECTION");
		case Result::GUILD_ITEM_COLLECTION_ALREADY_CLEAR_CONDITION: return TEXT("GUILD_ITEM_COLLECTION_ALREADY_CLEAR_CONDITION");
		case Result::GUILD_ITEM_COLLECTION_ALREADY_CLEAR_COLLECTION: return TEXT("GUILD_ITEM_COLLECTION_ALREADY_CLEAR_COLLECTION");
		case Result::GUILD_ITEM_COLLECTION_NOT_SUITABLE_ITEM: return TEXT("GUILD_ITEM_COLLECTION_NOT_SUITABLE_ITEM");
		case Result::TREASURE_MONSTER_ERROR_SAFE_TYTERRITORY: return TEXT("TREASURE_MONSTER_ERROR_SAFE_TYTERRITORY");
		case Result::TREASURE_MONSTER_ERROR_ALREADY_CREATED: return TEXT("TREASURE_MONSTER_ERROR_ALREADY_CREATED");
		case Result::TREASURE_MONSTER_ERROR_INVALID_AREA: return TEXT("TREASURE_MONSTER_ERROR_INVALID_AREA");
		case Result::ACHIEVEMENT_ERROR_UNKNOWN: return TEXT("ACHIEVEMENT_ERROR_UNKNOWN");
		case Result::ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_ID: return TEXT("ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_ID");
		case Result::ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_THEME_ID: return TEXT("ACHIEVEMENT_ERROR_COULD_NOT_FIND_ACHIEVEMENT_THEME_ID");
		case Result::ACHIEVEMENT_ERROR_NOT_MEET_THE_REWARD_CONDITION: return TEXT("ACHIEVEMENT_ERROR_NOT_MEET_THE_REWARD_CONDITION");
		case Result::MONSTER_KNOWLEDGE_ERROR_INVALID_USER: return TEXT("MONSTER_KNOWLEDGE_INVALID_USER");
		case Result::MONSTER_KNOWLEDGE_ERROR_CANT_LEVEL_UP_KNOWLEDGE: return TEXT("MONSTER_KNOWLEDGE_ERROR_CANT_LEVEL_UP_KNOWLEDGE");
		case Result::MONSTER_KNOWLEDGE_ERROR_INVALID_ITEM_AMOUNT: return TEXT("MONSTER_KNOWLEDGE_ERROR_INVALID_ITEM_AMOUNT");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_GAIN_EXP_ITEM: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_GAIN_EXP_ITEM");
		case Result::MONSTER_KNOWLEDGE_ERROR_NO_KNOWLEDGE_LEVEL_UP_AVAILABLE: return TEXT("MONSTER_KNOWLEDGE_ERROR_NO_KNOWLEDGE_LEVEL_UP_AVAILABLE");
		case Result::MONSTER_KNOWLEDGE_ERROR_CANT_SLATE_SLOT_ACTIVATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_CANT_SLATE_SLOT_ACTIVATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_RESET_SLATE_SLOT_ITEM: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_RESET_SLATE_SLOT_ITEM");
		case Result::MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE: return TEXT("MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE");
		case Result::MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE_EXCEEDED_CURRENCY: return TEXT("MONSTER_KNOWLEDGE_ERROR_DB_REQUEST_FAILURE_EXCEEDED_CURRENCY");
		case Result::MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_EFFECT_PARAM: return TEXT("MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_EFFECT_PARAM");
		case Result::MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_TEMPLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_GAIN_EXP_ITEM_TEMPLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_SLATE_SLOT_OPEN_COST: return TEXT("MONSTER_KNOWLEDGE_ERROR_SLATE_SLOT_OPEN_COST");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_ENOUGHT_MONSTER_BOOK_POINT: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_ENOUGHT_MONSTER_BOOK_POINT");
		case Result::MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_EFFECT_PARAM: return TEXT("MONSTER_KNOWLEDGE_INVALID_UNLOCK_SLATE_ITEM_EFFECT_PARAM");
		case Result::MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_TEMPLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_UNLOCK_SLATE_ITEM_TEMPLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_UNLOCK_SLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_ALREADY_UNLOCK_SLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_RESET_SLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_RESET_SLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_LEVEL: return TEXT("MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_LEVEL");
		case Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_EXP: return TEXT("MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_EXP");
		case Result::MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_KNOWLEDGE: return TEXT("MONSTER_KNOWLEDGE_ERROR_NOT_EXIST_KNOWLEDGE");
		case Result::MONSTER_KNOWLEDGE_ERROR_LOCKED_SLATE: return TEXT("MONSTER_KNOWLEDGE_ERROR_LOCKED_SLATE");
		case Result::MONSTER_KNOWLEDGE_ERROR_INVALID_KNOWLEDGE_MAX_EXP: return TEXT("MONSTER_KNOWLEDGE_ERROR_INVALID_KNOWLEDGE_MAX_EXP");
		case Result::SLATE_PROBABILITY_FAIL_TO_ACTIVATE_SPECIAL_NODE: return TEXT("SLATE_PROBABILITY_FAIL_TO_ACTIVATE_SPECIAL_NODE");
		case Result::SLATE_ERROR_INVALID_SLATE_ID: return TEXT("SLATE_ERROR_INVALID_SLATE_ID");
		case Result::SLATE_ERROR_INVALID_SPECIAL_NODE_ID: return TEXT("SLATE_ERROR_INVALID_SPECIAL_NODE_ID");
		case Result::SLATE_ERROR_INVALID_USER: return TEXT("SLATE_ERROR_INVALID_USER");
		case Result::SLATE_ERROR_LOCKED_SLATE: return TEXT("SLATE_ERROR_LOCKED_SLATE");
		case Result::SLATE_ERROR_CANT_ACTIVATE_SPECIAL_NODE_ANYMORE: return TEXT("SLATE_ERROR_CANT_ACTIVATE_SPECIAL_NODE_ANYMORE");
		case Result::SLATE_ERROR_DB_REQUEST_FAILURE: return TEXT("SLATE_ERROR_DB_REQUEST_FAILURE");
		case Result::CODEX_ERROR_INVALID_USER: return TEXT("CODEX_ERROR_INVALID_USER");
		case Result::CODEX_ERROR_INVALID_CATEGORY_ID: return TEXT("CODEX_ERROR_INVALID_CATEGORY_ID");
		case Result::CODEX_ERROR_NOT_ENOUGTH_MONSTER_KNOWLEDGE: return TEXT("CODEX_ERROR_NOT_ENOUGTH_MONSTER_KNOWLEDGE");
		case Result::CODEX_ERROR_INVALID_CATEGORY_BALANCE_ID: return TEXT("CODEX_ERROR_INVALID_CATEGORY_BALANCE_ID");
		case Result::CODEX_ERROR_NOT_ENOUGH_MAP_GROUP_LEVEL_UP_COUNT: return TEXT("CODEX_ERROR_NOT_ENOUGH_MAP_GROUP_LEVEL_UP_COUNT");
		case Result::CODEX_CANT_EXCHANGE_ANYMORE_MAX_POINT: return TEXT("CODEX_CANT_EXCHANGE_ANYMORE_MAX_POINT");
		case Result::CODEX_ERROR_CATEGORY_LEVEL_UP_ZERO: return TEXT("CODEX_ERROR_CATEGORY_LEVEL_UP_ZERO");
		case Result::CODEX_ERROR_CATEGORY_LEVEL_UP_SAME_LEVEL: return TEXT("CODEX_ERROR_CATEGORY_LEVEL_UP_SAME_LEVEL");
		case Result::CODEX_ERROR_REQUEST_ITEM_AMOUNT_NOT_ENOUGH: return TEXT("CODEX_ERROR_REQUEST_ITEM_AMOUNT_NOT_ENOUGH");
		case Result::CODEX_ERROR_NOT_GAIN_EXP_ITEM: return TEXT("CODEX_ERROR_NOT_GAIN_EXP_ITEM");
		case Result::CODEX_ERROR_INVALID_MAP_GROUP_ID: return TEXT("CODEX_ERROR_INVALID_MAP_GROUP_ID");
		case Result::CODEX_ERROR_NOT_INGREDIENT_ITEM: return TEXT("CODEX_ERROR_NOT_INGREDIENT_ITEM");
		case Result::CODEX_ERROR_MAP_GROUP_MAX_EXP: return TEXT("CODEX_ERROR_MAP_GROUP_MAX_EXP");
		case Result::CODEX_ERROR_MAX_MONSTER_BOOK_POINT: return TEXT("CODEX_ERROR_MAX_MONSTER_BOOK_POINT");
		case Result::CODEX_ERROR_DB_REQUEST_FAILURE: return TEXT("CODEX_ERROR_DB_REQUEST_FAILURE");
		case Result::DEATH_PENALTY_ERROR_USER_LEVEL_DATA_TABLE: return TEXT("DEATH_PENALTY_ERROR_USER_LEVEL_DATA_TABLE");
		case Result::DEATH_ENALTY_ERROR_RESURREC_DATA_TABLE: return TEXT("DEATH_ENALTY_ERROR_RESURREC_DATA_TABLE");
		case Result::RECONNECT_ERROR_UNKNOWN: return TEXT("RECONNECT_ERROR_UNKNOWN");
		case Result::RECONNECT_ERROR_INVALID_ACCOUNT_ID: return TEXT("RECONNECT_ERROR_INVALID_ACCOUNT_ID");
		case Result::RECONNECT_ERROR_INVALID_STATE: return TEXT("RECONNECT_ERROR_INVALID_STATE");
		case Result::RECONNECT_ERROR_INCORRECT_TOKEN: return TEXT("RECONNECT_ERROR_INCORRECT_TOKEN");
		case Result::RECONNECT_ERROR_NOT_DISCONNECTED_YET: return TEXT("RECONNECT_ERROR_NOT_DISCONNECTED_YET");
		case Result::CREATE_USER_ERROR_UNKNOWN: return TEXT("CREATE_USER_ERROR_UNKNOWN");
		case Result::CREATE_USER_ERROR_INTERNAL_ERROR: return TEXT("CREATE_USER_ERROR_INTERNAL_ERROR");
		case Result::CREATE_USER_ERROR_DB_OPERATION_ERROR: return TEXT("CREATE_USER_ERROR_DB_OPERATION_ERROR");
		case Result::CREATE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER: return TEXT("CREATE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER");
		case Result::CREATE_USER_ERROR_ALREADY_EXIST_NAME: return TEXT("CREATE_USER_ERROR_ALREADY_EXIST_NAME");
		case Result::CREATE_USER_ERROR_INVALID_LENGTH_NAME: return TEXT("CREATE_USER_ERROR_INVALID_LENGTH_NAME");
		case Result::CREATE_USER_ERROR_NOT_ALLOWED_NAME: return TEXT("CREATE_USER_ERROR_NOT_ALLOWED_NAME");
		case Result::CREATE_USER_ERROR_INVALID_GENDER_TYPE: return TEXT("CREATE_USER_ERROR_INVALID_GENDER_TYPE");
		case Result::CREATE_USER_ERROR_INVALID_WEAPON_TYPE: return TEXT("CREATE_USER_ERROR_INVALID_WEAPON_TYPE");
		case Result::CREATE_USER_ERROR_PUSH_INITIAL_ITEM: return TEXT("CREATE_USER_ERROR_PUSH_INITIAL_ITEM");
		case Result::CREATE_USER_ERROR_SET_INITIAL_SKILL: return TEXT("CREATE_USER_ERROR_SET_INITIAL_SKILL");
		case Result::CREATE_USER_ERROR_SET_INITIAL_FAIRY: return TEXT("CREATE_USER_ERROR_SET_INITIAL_FAIRY");
		case Result::CREATE_USER_ERROR_NO_MORE_USER_SOCKET: return TEXT("CREATE_USER_ERROR_NO_MORE_USER_SOCKET");
		case Result::CREATE_USER_ERROR_PUSH_INITIAL_CURRENCY: return TEXT("CREATE_USER_ERROR_PUSH_INITIAL_CURRENCY");
		case Result::CREATE_USER_ERROR_SET_INITIAL_COSTUME: return TEXT("CREATE_USER_ERROR_SET_INITIAL_COSTUME");
		case Result::CREATE_USER_ERROR_ADD_INITIAL_MAIL: return TEXT("CREATE_USER_ERROR_ADD_INITIAL_MAIL");
		case Result::CREATE_USER_ERROR_SET_NEW_USER_ATTENDANCE: return TEXT("CREATE_USER_ERROR_SET_NEW_USER_ATTENDANCE");
		case Result::CREATE_USER_ERROR_SET_NEW_USER_COMMUNITY_EVENT_MISSION: return TEXT("CREATE_USER_ERROR_SET_NEW_USER_COMMUNITY_EVENT_MISSION");
		case Result::CREATE_USER_ERROR_PENALTY_FREE: return TEXT("CREATE_USER_ERROR_PENALTY_FREE");
		case Result::CREATE_USER_ERROR_ARCAS: return TEXT("CREATE_USER_ERROR_ARCAS");
		case Result::CREATE_USER_ERROR_QUEST_MAIN: return TEXT("CREATE_USER_ERROR_QUEST_MAIN");
		case Result::CREATE_USER_ERROR_COMBAT_OPTION: return TEXT("CREATE_USER_ERROR_COMBAT_OPTION");
		case Result::CREATE_USER_ERROR_OFF_LINE: return TEXT("CREATE_USER_ERROR_OFF_LINE");
		case Result::CREATE_USER_ERROR_SET_ISSUED_USER_ID: return TEXT("CREATE_USER_ERROR_SET_ISSUED_USER_ID");
		case Result::CREATE_USER_ERROR_INSERT_USER: return TEXT("CREATE_USER_ERROR_INSERT_USER");
		case Result::CREATE_USER_ERROR_INSERT_USER_ROW_COUNT: return TEXT("CREATE_USER_ERROR_INSERT_USER_ROW_COUNT");
		case Result::CREATE_USER_ERROR_CALL_END_TRANSACTION_COMMIT: return TEXT("CREATE_USER_ERROR_CALL_END_TRANSACTION_COMMIT");
		case Result::CREATE_USER_ERROR_CALL_END_TRANSACTION_ROLLBACK: return TEXT("CREATE_USER_ERROR_CALL_END_TRANSACTION_ROLLBACK");
		case Result::CREATE_USER_ERROR_SET_NEW_USER_SOCIAL_ACTON: return TEXT("CREATE_USER_ERROR_SET_NEW_USER_SOCIAL_ACTON");
		case Result::CREATE_USER_ERROR_CREATION_LIMIT: return TEXT("CREATE_USER_ERROR_CREATION_LIMIT");
		case Result::DELETE_USER_ERROR_UNKNOWN: return TEXT("DELETE_USER_ERROR_UNKNOWN");
		case Result::DELETE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER: return TEXT("DELETE_USER_ERROR_ALREADY_CREATING_OR_DELETING_USER");
		case Result::DELETE_USER_ERROR_INVALID_USER_DB_ID: return TEXT("DELETE_USER_ERROR_INVALID_USER_DB_ID");
		case Result::DELETE_USER_ERROR_DB_OPERATION_ERROR: return TEXT("DELETE_USER_ERROR_DB_OPERATION_ERROR");
		case Result::DELETE_USER_ERROR_GUILD_MEMBER_CANNOT: return TEXT("DELETE_USER_ERROR_GUILD_MEMBER_CANNOT");
		case Result::DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL: return TEXT("DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL");
		case Result::DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION: return TEXT("DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION");
		case Result::DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION: return TEXT("DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION");
		case Result::SELECT_USER_ERROR_UNKNOWN: return TEXT("SELECT_USER_ERROR_UNKNOWN");
		case Result::SELECT_USER_ERROR_WORLD_SERVER_DOWN: return TEXT("SELECT_USER_ERROR_WORLD_SERVER_DOWN");
		case Result::SELECT_USER_ERROR_MUST_CHANGE_USER_NAME: return TEXT("SELECT_USER_ERROR_MUST_CHANGE_USER_NAME");
		case Result::SELECT_USER_ERROR_LEAVING_WORLD: return TEXT("SELECT_USER_ERROR_LEAVING_WORLD");
		case Result::SELECT_USER_ERROR_INVALID_LOGIN_STATE: return TEXT("SELECT_USER_ERROR_INVALID_LOGIN_STATE");
		case Result::SELECT_USER_ERROR_BLOCK_USER: return TEXT("SELECT_USER_ERROR_BLOCK_USER");
		case Result::LOGOUT_ERROR_UNKNOWN: return TEXT("LOGOUT_ERROR_UNKNOWN");
		case Result::LOGOUT_ERROR_COMBAT_STATE: return TEXT("LOGOUT_ERROR_COMBAT_STATE");
		case Result::BUDDY_ERROR_UNKNOWN: return TEXT("BUDDY_ERROR_UNKNOWN");
		case Result::BUDDY_ERROR_INVALID_NAME_LENGTH: return TEXT("BUDDY_ERROR_INVALID_NAME_LENGTH");
		case Result::BUDDY_ERROR_REQUEST_MY_SELF: return TEXT("BUDDY_ERROR_REQUEST_MY_SELF");
		case Result::BUDDY_ERROR_NOT_FOUND_USER: return TEXT("BUDDY_ERROR_NOT_FOUND_USER");
		case Result::BUDDY_ERROR_INVALID_USER_ID: return TEXT("BUDDY_ERROR_INVALID_USER_ID");
		case Result::BUDDY_ERROR_DB_OPERATION_ERROR: return TEXT("BUDDY_ERROR_DB_OPERATION_ERROR");
		case Result::BUDDY_ERROR_INVALID_INVITATION: return TEXT("BUDDY_ERROR_INVALID_INVITATION");
		case Result::BUDDY_ERROR_ALREADY_BUDDY: return TEXT("BUDDY_ERROR_ALREADY_BUDDY");
		case Result::BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY: return TEXT("BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY");
		case Result::BUDDY_ERROR_ALREADY_INVITATION_SENT: return TEXT("BUDDY_ERROR_ALREADY_INVITATION_SENT");
		case Result::BUDDY_ERROR_ALREADY_INVITATION_RECV: return TEXT("BUDDY_ERROR_ALREADY_INVITATION_RECV");
		case Result::BUDDY_ERROR_SEND_INVITATION_FULL: return TEXT("BUDDY_ERROR_SEND_INVITATION_FULL");
		case Result::BUDDY_ERROR_REACHED_MAX_BUDDY: return TEXT("BUDDY_ERROR_REACHED_MAX_BUDDY");
		case Result::BUDDY_ERROR_NOT_BUDDY: return TEXT("BUDDY_ERROR_NOT_BUDDY");
		case Result::BUDDY_ERROR_DELETE_SEND_INVITATION: return TEXT("BUDDY_ERROR_DELETE_SEND_INVITATION");
		case Result::BUDDY_ERROR_DELETE_RECV_INVITATION: return TEXT("BUDDY_ERROR_DELETE_RECV_INVITATION");
		case Result::BUDDY_ERROR_TARGET_RECV_INVITATION_FULL: return TEXT("BUDDY_ERROR_TARGET_RECV_INVITATION_FULL");
		case Result::BUDDY_ERROR_INVALID_BUDDY_TAB_TYPE: return TEXT("BUDDY_ERROR_INVALID_BUDDY_TAB_TYPE");
		case Result::BUDDY_ERROR_INSERT_BUDDY_TO_ME: return TEXT("BUDDY_ERROR_INSERT_BUDDY_TO_ME");
		case Result::BUDDY_ERROR_INSERT_ME_TO_BUDDY: return TEXT("BUDDY_ERROR_INSERT_ME_TO_BUDDY");
		case Result::BUDDY_ERROR_DELETE_REQUEST_INVITATION: return TEXT("BUDDY_ERROR_DELETE_REQUEST_INVITATION");
		case Result::BUDDY_ERROR_INSERT_REQUEST_INVITATION: return TEXT("BUDDY_ERROR_INSERT_REQUEST_INVITATION");
		case Result::BUDDY_ERROR_INVALID_INVITATION_SAME_ACCOUNT: return TEXT("BUDDY_ERROR_INVALID_INVITATION_SAME_ACCOUNT");
		case Result::BUDDY_ERROR_CACHE_MISSING: return TEXT("BUDDY_ERROR_CACHE_MISSING");
		case Result::BUDDY_ERROR_WORLD_SERVER_ITEM: return TEXT("BUDDY_ERROR_WORLD_SERVER_ITEM");
		case Result::CHAT_BLACKLIST_ADD_ERROR_DB: return TEXT("CHAT_BLACKLIST_ADD_ERROR_DB");
		case Result::CHAT_BLACKLIST_ADD_ERROR_OVER: return TEXT("CHAT_BLACKLIST_ADD_ERROR_OVER");
		case Result::CHAT_BLACKLIST_ADD_ERROR_UNKNOWN_USER: return TEXT("CHAT_BLACKLIST_ADD_ERROR_UNKNOWN_USER");
		case Result::CHAT_BLACKLIST_ADD_ERROR_DUPLICATION: return TEXT("CHAT_BLACKLIST_ADD_ERROR_DUPLICATION");
		case Result::CHAT_BLACKLIST_REMOVE_ERROR_DB: return TEXT("CHAT_BLACKLIST_REMOVE_ERROR_DB");
		case Result::REPORT_BAD_CHAT_ERROR_DB: return TEXT("REPORT_CHAT_BAD_ERROR_DB");
		case Result::REPORT_BAD_CHAT_ERROR_OVER: return TEXT("REPORT_CHAT_BAD_ERROR_OVER");
		case Result::REPORT_BAD_CHAT_ERROR_UNKNOWN_USER: return TEXT("REPORT_BAD_CHAT_ERROR_UNKNOWN_USER");
		case Result::QUEST_ERROR_NOT_EXIST_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_EXIST_GUILD_QUEST");
		case Result::QUEST_ERROR_INVALID_GUILD_QUEST_DB_ID: return TEXT("QUEST_ERROR_INVALID_GUILD_QUEST_DB_ID");
		case Result::QUEST_ERROR_GUILD_QUEST_DB_OPERATION: return TEXT("QUEST_ERROR_GUILD_QUEST_DB_OPERATION");
		case Result::REGISTER_ERROR_UNKNOWN: return TEXT("");
		case Result::REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID: return TEXT("");
		case Result::HIVE_RECEIPT_VERIFY_ERROR_SETTING: return TEXT("");
		case Result::HIVE_RECEIPT_VERIFY_ERROR_HOST: return TEXT("");
		case Result::HIVE_RECEIPT_VERIFY_ERROR: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE: return TEXT("");
		case Result::HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_PARSE: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_DIFF_PAYLOAD: return TEXT("");
		case Result::HIVE_ERROR_RECEIPT_PAYLOAD_NONE: return TEXT("");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_SETTING: return TEXT("");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_HOST: return TEXT("");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT: return TEXT("");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT: return TEXT("");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_PARSE: return TEXT("");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT: return TEXT("");
		case Result::HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN: return TEXT("");
		case Result::HIVE_ERROR_TIME_ZONE_RESULT_FAIL: return TEXT("");
		case Result::HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN: return TEXT("");
		case Result::HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO: return TEXT("");
		case Result::INVENTORY_EXTEND_ERROR_UNKNOWN: return TEXT("DEPOT_EXTEND_ERROR_UNKNOWN");
		case Result::INVENTORY_EXTEND_ERROR_MAX_SLOT: return TEXT("INVENTORY_EXTEND_ERROR_MAX_SLOT");
		case Result::INVENTORY_EXTEND_ERROR_NOT_ENOUGH_MONEY: return TEXT("INVENTORY_EXTEND_NOT_ENOUGH_MONEY");
		case Result::CHECK_USER_EXIST_ERROR_USER_NAME_EMPTY: return TEXT("CHECK_USER_EXIST_ERROR_USER_NAME_EMPTY");
		case Result::CHECK_USER_EXIST_ERROR_USER_NAME_LENGTH: return TEXT("CHECK_USER_EXIST_ERROR_USER_NAME_LENGTH");
		case Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST: return TEXT("CHECK_USER_EXIST_ERROR_USER_NOT_EXIST");
		case Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD: return TEXT("CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD");
		case Result::TITLE_ERROR_INVALID_EQUIP_SLOT: return TEXT("TITLE_ERROR_INVALID_EQUIP_SLOT");
		case Result::TITLE_ERROR_INVALID_TITLE_ID: return TEXT("TITLE_ERROR_INVALID_TITLE_ID");
		case Result::TITLE_ERROR_NOT_ACTIVED_TITLE: return TEXT("TITLE_ERROR_NOT_ACTIVED_TITLE");
		case Result::TITLE_ERROR_UNKNOWN: return TEXT("TITLE_ERROR_UNKNOWN");
		case Result::REGISTER_USER_UNLOCKINFO_ERROR_UNKNOWN: return TEXT("REGISTER_USER_UNLOCKINFO_ERROR_UNKNOWN");
		case Result::STATISTICS_ERROR_UNKNOWN: return TEXT("STATISTICS_ERROR_UNKNOWN");
		case Result::STATISTICS_ERROR_INVALID_STATUS: return TEXT("STATISTICS_ERROR_INVALID_STATUS");
		case Result::CAMP_ERROR_INVALID: return TEXT("CAMP_ERROR_INVALID");
		case Result::CAMP_ERROR_GUILD_GRADE: return TEXT("CAMP_ERROR_GUILD_GRADE");
		case Result::CAMP_ERROR_GUILD_BUILDING: return TEXT("CAMP_ERROR_GUILD_BUILDING");
		case Result::CAMP_ERROR_SAFE_TYPE_TERRITORY: return TEXT("CAMP_ERROR_SAFE_TYPE_TERRITORY");
		case Result::CAMP_ERROR_INVALID_MAP: return TEXT("CAMP_ERROR_INVALID_MAP");
		case Result::CAMP_ERROR_INVALID_AREA: return TEXT("CAMP_ERROR_INVALID_AREA");
		case Result::CAMP_ERROR_INVALID_ITEM: return TEXT("CAMP_ERROR_INVALID_ITEM");
		case Result::CAMP_ERROR_INVALID_NPC: return TEXT("CAMP_ERROR_INVALID_NPC");
		case Result::CAMP_ERROR_SPAWN_CAMP: return TEXT("CAMP_ERROR_SPAWN_CAMP");
		case Result::CAMP_ERROR_UNABLE_INTERACTION: return TEXT("CAMP_ERROR_UNABLE_INTERACTION");
		case Result::CAMP_ERROR_NEAR_CAMPS_EXIST: return TEXT("CAMP_ERROR_NEAR_CAMPS_EXIST");
		case Result::CAMP_ERROR_CAMP_DESTROYING: return TEXT("CAMP_ERROR_CAMP_DESTROYING");
		case Result::CAMP_ERROR_INSTALLATION_LIMIT_DISTANCE: return TEXT("CAMP_ERROR_INSTALLATION_LIMIT_DISTANCE");
		case Result::CAMP_ERROR_ALREADY_INSTALLED: return TEXT("CAMP_ERROR_ALREADY_INSTALLED");
		case Result::CAMP_ERROR_INVADE_WORLD: return TEXT("CAMP_ERROR_INVADE_WORLD");
		case Result::CAMP_ERROR_UNAUTHORIZED: return TEXT("CAMP_ERROR_UNAUTHORIZED");
		case Result::SANCTUM_ERROR_GUILD_GRADE: return TEXT("CAMP_ERROR_GUILD_GRADE");
		case Result::SANCTUM_ERROR_GUILD_BUILDING: return TEXT("CAMP_ERROR_GUILD_BUILDING");
		case Result::SANCTUM_ERROR_INVALID_MAP: return TEXT("SANCTUM_ERROR_INVALID_MAP");
		case Result::SANCTUM_ERROR_INVALID_AREA: return TEXT("SANCTUM_ERROR_INVALID_AREA");
		case Result::SANCTUM_ERROR_INVALID_NPC: return TEXT("SANCTUM_ERROR_INVALID_NPC");
		case Result::SANCTUM_ERROR_SPAWN_SANCTUM: return TEXT("SANCTUM_ERROR_SPAWN_SANCTUM");
		case Result::SANCTUM_ERROR_UNABLE_INTERACTION: return TEXT("SANCTUM_ERROR_UNABLE_INTERACTION");
		case Result::SANCTUM_ERROR_NEAR_SANCTUMS_EXIST: return TEXT("SANCTUM_ERROR_NEAR_SANCTUMS_EXIST");
		case Result::SANCTUM_ERROR_SANCTUM_DESTROYING: return TEXT("SANCTUM_ERROR_SANCTUM_DESTROYING");
		case Result::SANCTUM_ERROR_INVADE_WORLD: return TEXT("SANCTUM_ERROR_INVADE_WORLD");
		case Result::SANCTUM_ERROR_UNAUTHORIZED: return TEXT("SANCTUM_ERROR_UNAUTHORIZED");
		case Result::SANCTUM_ERROR_ALREADY_INSTALLED: return TEXT("SANCTUM_ERROR_ALREADY_INSTALLED");
		case Result::SANCTUM_ERROR_BATTLE_BEGIN: return TEXT("SANCTUM_ERROR_BATTLE_BEGIN");
		case Result::SANCTUM_ERROR_EXIST_NEXUS: return TEXT("SANCTUM_ERROR_EXIST_NEXUS");
		case Result::PROFILE_ERROR_UNKNOWN: return TEXT("PROFILE_ERROR_UNKNOWN");
		case Result::PROFILE_ERROR_INVALID_LENGTH_TEXT: return TEXT("PROFILE_ERROR_INVALID_LENGTH_TEXT");
		case Result::PROFILE_ERROR_INTRO_DB_FAIL: return TEXT("PROFILE_ERROR_INTRO_DB_FAIL");
		case Result::PROFILE_ERROR_NOT_ALLOWED_TEXT: return TEXT("PROFILE_ERROR_NOT_ALLOWED_TEXT");
		case Result::PROFILE_ERROR_INVALID_USER: return TEXT("PROFILE_ERROR_INVALID_USER");
		case Result::PROFILE_ERROR_DATA_CACHE_FAIL: return TEXT("PROFILE_ERROR_DATA_CACHE_FAIL");
		case Result::PROFILE_ERROR_COMMENT_DB_FAIL: return TEXT("PROFILE_ERROR_COMMENT_DB_FAIL");
		case Result::PROFILE_ERROR_COMMENT_DELETED: return TEXT("PROFILE_ERROR_COMMENT_DELETED");
		case Result::PROFILE_ERROR_COMMENT_FULL: return TEXT("PROFILE_ERROR_COMMENT_FULL");
		case Result::PROFILE_ERROR_COMMENT_INPUT_SELF: return TEXT("PROFILE_ERROR_COMMENT_INPUT_SELF");
		case Result::PROFILE_ERROR_COMMENT_DELETE_INVALID: return TEXT("PROFILE_ERROR_COMMENT_DELETE_INVALID");
		case Result::PROFILE_ERROR_INVALID_KEYWORD: return TEXT("PROFILE_ERROR_INVALID_KEYWORD");
		case Result::INVADE_ERROR_UNKNOWN: return TEXT("INVADE_ERROR_UNKNOWN");
		case Result::INVADE_ERROR_INVALID_TIME: return TEXT("INVADE_ERROR_INVALID_TIME");
		case Result::INVADE_ERROR_PORTAL_MAX_COUNT: return TEXT("INVADE_ERROR_PORTAL_MAX_COUNT");
		case Result::INVADE_ERROR_INVALID_LEVEL: return TEXT("INVADE_ERROR_INVALID_LEVEL");
		case Result::INVADE_ERROR_ENTER_MAX_COUNT: return TEXT("INVADE_ERROR_ENTER_MAX_COUNT");
		case Result::INVADE_ERROR_NOT_ENOUGH_TIME: return TEXT("INVADE_ERROR_NOT_ENOUGH_TIME");
		case Result::INVADE_ERROR_INVALID_LOAD_USER_DATA: return TEXT("INVADE_ERROR_INVALID_LOAD_USER_DATA");
		case Result::INVADE_ERROR_INVALID_SAVE_USER_DATA: return TEXT("INVADE_ERROR_INVALID_SAVE_USER_DATA");
		case Result::INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA: return TEXT("INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA");
		case Result::INVADE_ERROR_UNLOCK: return TEXT("INVADE_ERROR_UNLOCK");
		case Result::INVADE_ERROR_INVALID_MAP: return TEXT("INVADE_ERROR_INVALID_MAP");
		case Result::USER_CONTEXT_UI_ERROR_INVALID_PROFILE_USER: return TEXT("USER_CONTEXT_UI_ERROR_INVALID_PROFILE_USER");
		case Result::USER_CONTEXT_UI_ERROR_IS_GM_USER: return TEXT("USER_CONTEXT_UI_ERROR_IS_GM_USER");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_SETTING: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_SETTING");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_HOST_SETTING: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_HOST_SETTING");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_NO_APP_ID: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_NO_APP_ID");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_UNABLE_TO_CONNECT: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_UNABLE_TO_CONNECT");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_PARSE: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_PARSE");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_NO_RESULT: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_NO_RESULT");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_RESULT_ERROR: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_RESULT_ERROR");
		case Result::HIVE_ERROR_IAP_UNCONSUMED_NO_REQUIRED_PARAMETER: return TEXT("HIVE_ERROR_IAP_UNCONSUMED_NO_REQUIRED_PARAMETER");
		case Result::TUTORIAL_ERROR_INTERNAL_DB_ERROR: return TEXT("TUTORIAL_ERROR_INTERNAL_DB_ERROR");
		case Result::TUTORIAL_ERROR_INVALID_TUTORIAL_ID: return TEXT("TUTORIAL_ERROR_INVALID_TUTORIAL_ID");
		case Result::PARTY_DUNGEON_ERROR_UNKNOWN: return TEXT("INVADE_ERROR_UNKNOWN");
		case Result::PARTY_DUNGEON_ERROR_ENTER_MAX_COUNT: return TEXT("INVADE_ERROR_ENTER_MAX_COUNT");
		case Result::PARTY_DUNGEON_ERROR_INVALID_LOAD_USER_DATA: return TEXT("INVADE_ERROR_INVALID_LOAD_USER_DATA");
		case Result::PARTY_DUNGEON_ERROR_INVALID_SAVE_USER_DATA: return TEXT("INVADE_ERROR_INVALID_SAVE_USER_DATA");
		case Result::PARTY_DUNGEON_ERROR_NOT_FOUND_PARTY_DUNGEON_MAP_DATA: return TEXT("INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA");
		case Result::PARTY_DUNGEON_ERROR_UNLOCK: return TEXT("INVADE_ERROR_UNLOCK");
		case Result::DUEL_ERROR_UNKNOWN: return TEXT("DUEL_ERROR_UNKNOWN");
		case Result::DUEL_ERROR_ALEREADY_INVITE_OTHER_USER: return TEXT("DUEL_ERROR_ALEREADY_INVITE_OTHER_USER");
		case Result::DUEL_ERROR_ALEREADY_PLAYING: return TEXT("DUEL_ERROR_ALEREADY_PLAYING");
		case Result::DUEL_ERROR_CANNOT_DUEL_TERRITORY: return TEXT("DUEL_ERROR_CANNOT_DUEL_TERRITORY");
		case Result::DUEL_ERROR_INVALID_USER: return TEXT("DUEL_ERROR_INVALID_USER");
		case Result::DUEL_ERROR_CANNOT_DUEL_TERRITORY_OTHER_USER: return TEXT("DUEL_ERROR_CANNOT_DUEL_TERRITORY_OTHER_USER");
		case Result::DUEL_ERROR_ALEREADY_INVITED_BY_OTHER_USER: return TEXT("DUEL_ERROR_ALEREADY_INVITED_BY_OTHER_USER");
		case Result::DUEL_ERROR_NOT_INVITE_USER: return TEXT("DUEL_ERROR_NOT_INVITE_USER");
		case Result::DUEL_ERROR_NOT_INVITED: return TEXT("DUEL_ERROR_NOT_INVITED");
		case Result::DUEL_ERROR_NOT_SAME_MAP: return TEXT("DUEL_ERROR_NOT_SAME_MAP");
		case Result::DUEL_ERROR_ARENA_ALREADY_PLAYING: return TEXT("DUEL_ERROR_ARENA_ALREADY_PLAYING");
		case Result::DUEL_ERROR_EXPIRED_INVITE_TIME: return TEXT("DUEL_ERROR_EXPIRED_INVITE_TIME");
		case Result::CHARACTER_SLOT_UNLOCK_ERROR_UNKNOWN: return TEXT("CHARACTER_SLOT_UNLOCK_ERROR_UNKNOWN");
		case Result::CHARACTER_SLOT_UNLOCK_ERROR_EXCEEDED: return TEXT("CHARACTER_SLOT_UNLOCK_ERROR_EXCEEDED");
		case Result::BATTLE_PASS_ERROR_UNKNOWN: return TEXT("BATTLE_PASS_ERROR_UNKNOWN");
		case Result::BATTLE_PASS_ERROR_BATTLE_PASS_NOT_EXISTS: return TEXT("BATTLE_PASS_ERROR_BATTLE_PASS_NOT_EXISTS");
		case Result::BATTLE_PASS_ERROR_NOT_MEET_THE_REWARD_CONDITION: return TEXT("BATTLE_PASS_ERROR_NOT_MEET_THE_REWARD_CONDITION");
		case Result::BATTLE_PASS_ERROR_ALREADY_REWARDED: return TEXT("BATTLE_PASS_ERROR_ALREADY_REWARDED");
		case Result::BATTLE_PASS_ERROR_TARGET_LEVEL_LESS_THAN_CURRENT_LEVEL: return TEXT("BATTLE_PASS_ERROR_TARGET_LEVEL_LESS_THAN_CURRENT_LEVEL");
		case Result::BATTLE_PASS_ERROR_EXCEED_MAX_LEVEL: return TEXT("BATTLE_PASS_ERROR_EXCEED_MAX_LEVEL");
		case Result::BATTLE_PASS_ERROR_BUY_LEVEL_CURRENCY_NOT_ENOUGH: return TEXT("BATTLE_PASS_ERROR_BUY_LEVEL_CURRENCY_NOT_ENOUGH");
		case Result::BATTLE_PASS_ERROR_ADDITONAL_REWARD_NOT_ACTIVATED: return TEXT("BATTLE_PASS_ERROR_ADDITONAL_REWARD_NOT_ACTIVATED");
		case Result::BATTLE_PASS_ERROR_ADDITONAL_REWARD_IS_ALREADY_ACTIVATED: return TEXT("BATTLE_PASS_ERROR_ADDITONAL_REWARD_IS_ALREADY_ACTIVATED");
		case Result::BATTLE_PASS_ERROR_ADDITONAL_ITEM_CONDITION_NOT_MEETED: return TEXT("BATTLE_PASS_ERROR_ADDITONAL_ITEM_CONDITION_NOT_MEETED");
		case Result::BATTLE_PASS_ERROR_ACTIVATE_REWARD_CURRENCY_NOT_ENOUGH: return TEXT("BATTLE_PASS_ERROR_ACTIVATE_REWARD_CURRENCY_NOT_ENOUGH");
		case Result::DELIVERY_BOX_ERROR_UNKNOWN: return TEXT("DELIVERY_BOX_ERROR_UNKNOWN");
		case Result::DELIVERY_BOX_ERROR_NOT_EVENT_TIME: return TEXT("DELIVERY_BOX_ERROR_NOT_EVENT_TIME");
		case Result::DELIVERY_BOX_ERROR_NOT_MEET_THE_EVENT_TYPE: return TEXT("DELIVERY_BOX_ERROR_NOT_MEET_THE_EVENT_TYPE");
		case Result::DELIVERY_BOX_ERROR_ALREADY_RECOVERED: return TEXT("DELIVERY_BOX_ERROR_ALREADY_RECOVERED");
		case Result::DELIVERY_BOX_ERROR_NOT_MEET_THE_CONDITION: return TEXT("DELIVERY_BOX_ERROR_NOT_MEET_THE_CONDITION");
		case Result::COMMON_EFFECT_ERROR_UNKNOWN: return TEXT("COMMON_EFFECT_ERROR_UNKNOWN");
		case Result::COMMON_ERROR_NO_GATEWAY_SERVER_CONNECTION: return TEXT("COMMON_ERROR_NO_GATEWAY_SERVER_CONNECTION");
		case Result::COMMON_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("COMMON_ERROR_GATEWAY_OPERATION_FAILED");
		case Result::KEYBOARD_MAPPING_ERROR_SAVE: return TEXT("KEYBOARD_MAPPING_ERROR_SAVE");
		case Result::KEYBOARD_MAPPING_ERROR_LOAD: return TEXT("KEYBOARD_MAPPING_ERROR_LOAD");
		case Result::PK_BOOK_ERROR_UNKNOWN: return TEXT("PK_BOOK_ERROR_UNKNOWN");
		case Result::PK_BOOK_ERROR_INVALID_DATA: return TEXT("PK_BOOK_ERROR_INVALID_DATA");
		case Result::PK_BOOK_ERROR_GATEWAY_FAIL: return TEXT("PK_BOOK_ERROR_GATEWAY_FAIL");
		case Result::PK_BOOK_ERROR_NOT_DIE_DATA: return TEXT("PK_BOOK_ERROR_NOT_DIE_DATA");
		case Result::PK_BOOK_ERROR_NOT_KILL_DATA: return TEXT("PK_BOOK_ERROR_NOT_KILL_DATA");
		case Result::PK_BOOK_ERROR_NOT_REMAIN_ACTION_COUNT: return TEXT("PK_BOOK_ERROR_NOT_REMAIN_ACTION_COUNT");
		case Result::PK_BOOK_ERROR_EXPIRED_ACTION_TIME: return TEXT("PK_BOOK_ERROR_EXPIRED_ACTION_TIME");
		case Result::PK_BOOK_ERROR_DISABLE_ACTION_TO_NPC: return TEXT("PK_BOOK_ERROR_DISABLE_ACTION_TO_NPC");
		case Result::PK_BOOK_ERROR_NOT_WARPABLE_MAP: return TEXT("PK_BOOK_ERROR_NOT_WARPABLE_MAP");
		case Result::PK_BOOK_ERROR_NOT_LOGINED_USER: return TEXT("PK_BOOK_ERROR_NOT_LOGINED_USER");
		case Result::PK_BOOK_ERROR_NOT_ENOUGH_ACTION_CURRENCY: return TEXT("PK_BOOK_ERROR_NOT_ENOUGH_ACTION_CURRENCY");
		case Result::PK_BOOK_ERROR_PK_MESSAGE_INVALID_TEXT: return TEXT("PK_BOOK_ERROR_PK_MESSAGE_INVALID_TEXT");
		case Result::PK_BOOK_ERROR_MONITORRING_USER_LIST_FULL: return TEXT("PK_BOOK_ERROR_MONITORRING_USER_LIST_FULL");
		case Result::PK_BOOK_ERROR_ANOTER_SERVER_USER: return TEXT("PK_BOOK_ERROR_ANOTER_SERVER_USER");
		case Result::PK_BOOK_ERROR_NO_WAPABLE_TO_USER: return TEXT("PK_BOOK_ERROR_NO_WAPABLE_TO_USER");
		case Result::PK_BOOK_ERROR_INVALID_USER: return TEXT("PK_BOOK_ERROR_INVALID_USER");
		case Result::PK_BOOK_ERROR_ALREADY_MONITORRING_USER_ADDED: return TEXT("PK_BOOK_ERROR_ALREADY_MONITORRING_USER_ADDED");
		case Result::PK_BOOK_ERROR_CANNOT_ADD_MYSELF: return TEXT("PK_BOOK_ERROR_CANNOT_ADD_MYSELF");
		case Result::PK_BOOK_ERROR_CANNOT_ADD_GM: return TEXT("PK_BOOK_ERROR_CANNOT_ADD_GM");
		case Result::PK_BOOK_ERROR_CANNOT_ACTION_TO_USER: return TEXT("PK_BOOK_ERROR_CANNOT_ACTION_TO_USER");
		case Result::GMTOOL_ERROR_BEGIN: return TEXT("GMTOOL_ERROR_BEGIN");
		case Result::GMTOOL_ERROR_UNKNOWN: return TEXT("GMTOOL_ERROR_UNKNOWN");
		case Result::GMTOOL_CONTENTS_EVENT_REG_ERROR_DB: return TEXT("GMTOOL_CONTENTS_EVENT_REG_ERROR_DB");
		case Result::GMTOOL_CONTENTS_EVENT_REG_ALREADY: return TEXT("GMTOOL_CONTENTS_EVENT_REG_ALREADY");
		case Result::GMTOOL_CONTENTS_EVENT_REG_EXPIRED_DATE: return TEXT("GMTOOL_CONTENTS_EVENT_REG_EXPIRED_DATE");
		case Result::GMTOOL_CONTENTS_EVENT_REG_NOT_EXIST_DATA_SHEET: return TEXT("GMTOOL_CONTENTS_EVENT_REG_NOT_EXIST_DATA_SHEET");
		case Result::GMTOOL_CONTENTS_EVENT_REMOVE_NOT_REG: return TEXT("GMTOOL_CONTENTS_EVENT_REMOVE_NOT_REG");
		case Result::GMTOOL_CONTENTS_EVENT_REMOVE_ERROR_DB: return TEXT("GMTOOL_CONTENTS_EVENT_REMOVE_ERROR_DB");
		case Result::GMTOOL_GM_USER_CACHE_MISSING: return TEXT("GMTOOL_GM_USER_CACHE_MISSING");
		case Result::APP_GUARD_ERROR_INVALID_PLATFORM: return TEXT("APP_GUARD_ERROR_INVALID_PLATFORM");
		case Result::APP_GUARD_ERROR_INVALID_ACTIVE_MODE: return TEXT("APP_GUARD_ERROR_INVALID_ACTIVE_MODE");
		case Result::DELETE_ACCOUNT_HIVE_CONNECT_ERROR: return TEXT("DELETE_ACCOUNT_HIVE_CONNECT_ERROR");
		case Result::DELETE_ACCOUNT_ERROR: return TEXT("DELETE_ACCOUNT_ERROR");
		case Result::DELETE_ACCOUNT_DB_ERROR: return TEXT("DELETE_ACCOUNT_DB_ERROR");
		case Result::HIVE_DELETE_ACCOUNT_ERROR: return TEXT("HIVE_DELETE_ACCOUNT_ERROR");
		case Result::HIVE_DELETE_ACCOUNT_CERTIFICATION_KEY_ERROR: return TEXT("HIVE_DELETE_ACCOUNT_ERROR");
		case Result::HIVE_DELETE_ACCOUNT_TOKEN_ERROR: return TEXT("HIVE_DELETE_ACCOUNT_TOKEN_ERROR");
		case Result::HIVE_DELETE_ACCOUNT_PARSE_ERROR: return TEXT("HIVE_DELETE_ACCOUNT_PARSE_ERROR");
		case Result::CREATE_USER_ERROR_DB_POSITION_ERROR: return TEXT("CREATE_USER_ERROR_DB_POSITION_ERROR");
		case Result::CHARACTER_CUSTOMIZING_ERROR_UNKNOWN: return TEXT("CHARACTER_CUSTOMIZING_ERROR_UNKNOWN");
		case Result::CHARACTER_CUSTOMIZING_ERROR_EXCEED_MAX_COUNT: return TEXT("CHARACTER_CUSTOMIZING_ERROR_EXCEED_MAX_COUNT");
		case Result::CHARACTER_CUSTOMIZING_ERROR_NOT_ENOUGH_COUNT: return TEXT("CHARACTER_CUSTOMIZING_ERROR_NOT_ENOUGH_COUNT");
		case Result::SOCIAL_EMOTION_ACTIVE: return TEXT("SOCIAL_EMOTION_ACTIVE");
		case Result::SEAL_ERROR_UNKNOWN: return TEXT("SEAL_ERROR_UNKNOWN");
		case Result::SEAL_ERROR_INVALID_STATUS: return TEXT("SEAL_ERROR_INVALID_STATUS");
		case Result::SEAL_ERROR_SLOT_LOCK_EXCEED_MAX: return TEXT("SEAL_ERROR_SLOT_LOCK_EXCEED_MAX");
		case Result::SEAL_ERROR_NOT_ENOUGH_LEVEL: return TEXT("SEAL_ERROR_NOT_ENOUGH_LEVEL");
		case Result::SEAL_ERROR_SLOT_NOT_USABLE_LEVEL: return TEXT("SEAL_ERROR_SLOT_NOT_USABLE_LEVEL");
		case Result::SEAL_ERROR_LEVEL_EXCEED_MAX: return TEXT("SEAL_ERROR_LEVEL_EXCEED_MAX");
		case Result::SEAL_ERROR_INVALID_STAT_POOL_ID: return TEXT("SEAL_ERROR_INVALID_STAT_POOL_ID");
		case Result::QUEST_ERROR_REQ_GUILD_QUEST_INFO: return TEXT("QUEST_ERROR_REQ_GUILD_QUEST_INFO");
		case Result::QUEST_ERROR_GUILD_QUEST_CONFIG_DATA: return TEXT("QUEST_ERROR_GUILD_QUEST_CONFIG_DATA");
		case Result::QUEST_ERROR_USER_IS_NOT_GUILD_MEMBER: return TEXT("QUEST_ERROR_USER_IS_NOT_GUILD_MEMBER");
		case Result::QUEST_ERROR_NOT_ENOUGH_GUILD_LEVEL: return TEXT("QUEST_ERROR_NOT_ENOUGH_GUILD_LEVEL");
		case Result::QUEST_ERROR_GUILD_QUEST_GENERATE: return TEXT("QUEST_ERROR_GUILD_QUEST_GENERATE");
		case Result::QUEST_ERROR_GUILD_QUEST_GENERATE_IN_DB: return TEXT("QUEST_ERROR_GUILD_QUEST_GENERATE_IN_DB");
		case Result::QUEST_ERROR_INVALID_USER_STATUS: return TEXT("QUEST_ERROR_INVALID_USER_STATUS");
		case Result::QUEST_ERROR_NOT_FOUND_GUILD_QUEST_ID_IN_DATA_SHEET: return TEXT("QUEST_ERROR_NOT_FOUND_GUILD_QUEST_ID_IN_DATA_SHEET");
		case Result::QUEST_ERROR_NOT_FOUND_GUILD_QUEST_INFO_IN_DATA_SHEET: return TEXT("QUEST_ERROR_NOT_FOUND_GUILD_QUEST_INFO_IN_DATA_SHEET");
		case Result::QUEST_ERROR_NOT_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_GUILD_QUEST");
		case Result::QUEST_ERROR_ALREADY_ACCEPT_GUILD_QUEST: return TEXT("QUEST_ERROR_ALREADY_ACCEPT_GUILD_QUEST");
		case Result::QUEST_ERROR_ALREADY_ACCEPTED_GUILD_QUEST: return TEXT("QUEST_ERROR_ALREADY_ACCEPTED_GUILD_QUEST");
		case Result::QUEST_ERROR_GUILD_NOT_ACCEPTABLE_TODAY: return TEXT("QUEST_ERROR_GUILD_NOT_ACCEPTABLE_TODAY");
		case Result::QUEST_ERROR_GUILD_EXCEED_ACCEPT_COUNT: return TEXT("QUEST_ERROR_GUILD_EXCEED_ACCEPT_COUNT");
		case Result::QUEST_ERROR_INVALID_REWARD_GUILD_QUEST_ID: return TEXT("QUEST_ERROR_INVALID_REWARD_GUILD_QUEST_ID");
		case Result::QUEST_ERROR_NOT_LAST_REWARD_GUILD_QUEST_ID: return TEXT("QUEST_ERROR_NOT_LAST_REWARD_GUILD_QUEST_ID");
		case Result::QUEST_ERROR_ALREADY_REWARDED_GUILD_QUEST_ID: return TEXT("QUEST_ERROR_ALREADY_REWARDED_GUILD_QUEST_ID");
		case Result::QUEST_ERROR_NOT_COMPLETED_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_COMPLETED_GUILD_QUEST");
		case Result::QUEST_ERROR_INVALID_GUILD_QUEST: return TEXT("QUEST_ERROR_INVALID_GUILD_QUEST");
		case Result::QUEST_ERROR_NOT_ACCEPTED_GUILD_QUEST: return TEXT("QUEST_ERROR_NOT_ACCEPTED_GUILD_QUEST");
		case Result::QUEST_ERROR_EXCEED_GUILD_QUEST_REFRESH_COUNT: return TEXT("QUEST_ERROR_EXCEED_GUILD_QUEST_REFRESH_COUNT");
		case Result::QUEST_ERROR_NOT_ENOUGH_GUILD_QUEST_REFRESH_CURRENCY: return TEXT("QUEST_ERROR_NOT_ENOUGH_GUILD_QUEST_REFRESH_CURRENCY");
		case Result::QUEST_ERROR_NOT_FOUND_GUILD_QUEST_REFRESH_INFO: return TEXT("QUEST_ERROR_NOT_FOUND_GUILD_QUEST_REFRESH_INFO");
		case Result::QUEST_ERROR_NO_REFRESHABLE_GUILD_QUEST: return TEXT("QUEST_ERROR_NO_REFRESHABLE_GUILD_QUEST");
		case Result::QUEST_ERROR_EXPIRED_GUILD_QUEST: return TEXT("QUEST_ERROR_EXPIRED_GUILD_QUEST");
		case Result::QUEST_ERROR_GUILD_NOT_ACCEPTABLE_QUEST: return TEXT("QUEST_ERROR_GUILD_NOT_ACCEPTABLE_QUEST");
		case Result::QUEST_ERROR_GUILD_REWARD_ITEM_CREATE: return TEXT("QUEST_ERROR_GUILD_REWARD_ITEM_CREATE");
		case Result::QUEST_ERROR_GUILD_NOT_REWARDABLE_QUEST: return TEXT("QUEST_ERROR_GUILD_NOT_REWARDABLE_QUEST");
		case Result::QUEST_ERROR_GUILD_ACCEPTED_QUEST: return TEXT("QUEST_ERROR_GUILD_ACCEPTED_QUEST");
		case Result::QUEST_ERROR_GUILD_REFRESH: return TEXT("QUEST_ERROR_GUILD_REFRESH");
		case Result::QUEST_ERROR_GUILD_REFRESH_REQ_DB: return TEXT("QUEST_ERROR_GUILD_REFRESH_REQ_DB");
		case Result::QUEST_ERROR_GUILD_EXCEED_COMPLETE_COUNT: return TEXT("QUEST_ERROR_GUILD_EXCEED_COMPLETE_COUNT");
		case Result::PERSONAL_TRADING_INVALID_USER_STATUS: return TEXT("PERSONAL_TRADING_INVALID_USER_STATUS");
		case Result::PERSONAL_TRADING_NOT_FOUND_USER: return TEXT("PERSONAL_TRADING_NOT_FOUND_USER");
		case Result::PERSONAL_TRADING_IMPOSSIBLE_USER: return TEXT("PERSONAL_TRADING_IMPOSSIBLE_USER");
		case Result::PERSONAL_TRADING_ERROR_INVALID_RANGE: return TEXT("PERSONAL_TRADING_ERROR_INVALID_RANGE");
		case Result::PERSONAL_TRADING_INVALID_OTHER_USER_STATUS: return TEXT("PERSONAL_TRADING_INVALID_OTHER_USER_STATUS");
		case Result::PERSONAL_TRADING_DONT_ACTION: return TEXT("PERSONAL_TRADING_DONT_ACTION");
		case Result::PERSONAL_TRADING_ERROR_OHTER_TRADING: return TEXT("PERSONAL_TRADING_ERROR_OHTER_TRADING");
		case Result::PERSONAL_TRADING_ERROR_TRAGET_USER_OHTER_TRADING: return TEXT("PERSONAL_TRADING_ERROR_TRAGET_USER_OHTER_TRADING");
		case Result::PERSONAL_TRADING_ERROR_SAME_POS: return TEXT("PERSONAL_TRADING_ERROR_SAME_POS");
		case Result::PERSONAL_TRADING_ERROR_TRY_CONFIRM: return TEXT("PERSONAL_TRADING_ERROR_TRY_CONFIRM");
		case Result::PERSONAL_TRADING_ERROR_NOT_TRADING: return TEXT("PERSONAL_TRADING_ERROR_NOT_TRADING");
		case Result::PERSONAL_TRADING_ERROR_ITEM_ADD: return TEXT("PERSONAL_TRADING_ERROR_ITEM_ADD");
		case Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_DIA: return TEXT("PERSONAL_TRADING_ERROR_NOT_ENOUGH_DIA");
		case Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_DIA: return TEXT("PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_DIA");
		case Result::PERSONAL_TRADING_ERROR_NOT_DIA_FULLY: return TEXT("PERSONAL_TRADING_ERROR_NOT_DIA_FULLY");
		case Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_INVENTORY_SLOT: return TEXT("PERSONAL_TRADING_ERROR_NOT_ENOUGH_INVENTORY_SLOT");
		case Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_INVENTORY_SLOT: return TEXT("PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_INVENTORY_SLOT");
		case Result::PERSONAL_TRADING_ERROR_WEIGHT_FULL: return TEXT("PERSONAL_TRADING_ERROR_WEIGHT_FULL");
		case Result::PERSONAL_TRADING_ERROR_TARGET_WEIGHT_FULL: return TEXT("PERSONAL_TRADING_ERROR_WEIGHT_FULL");
		case Result::PERSONAL_TRADING_ERROR_TIME_OUT: return TEXT("PERSONAL_TRADING_ERROR_TIME_OUT");
		case Result::PERSONAL_TRADING_ERROR_LEVEL: return TEXT("PERSONAL_TRADING_ERROR_LEVEL");
		case Result::PERSONAL_TRADING_ERROR_TARGET_LEVEL: return TEXT("PERSONAL_TRADING_ERROR_TARGET_LEVEL");
		case Result::PERSONAL_TRADING_ERROR_TRY_COUNT: return TEXT("PERSONAL_TRADING_ERROR_TRY_COUNT");
		case Result::PERSONAL_TRADING_ERROR_TRAGET_TRY_COUNT: return TEXT("PERSONAL_TRADING_ERROR_TRAGET_TRY_COUNT");
		case Result::PERSONAL_TRADING_ERROR_UNKNOWN: return TEXT("PERSONAL_TRADING_ERROR_UNKNOWN");
		case Result::PERSONAL_TRADING_ERROR_INVALID_ITEM: return TEXT("PERSONAL_TRADING_ERROR_INVALID_ITEM");
		case Result::PERSONAL_TRADING_ALREADY_TRADING: return TEXT("PERSONAL_TRADING_ALREADY_TRADING");
		case Result::PERSONAL_TRADING_ERROR_USABLE_IN_SAFE_ZONE: return TEXT("PERSONAL_TRADING_ERROR_USABLE_IN_SAFE_ZONE");
		case Result::PERSONAL_TRADING_ERROR_CANNOT_USABLE_GM: return TEXT("PERSONAL_TRADING_ERROR_CANNOT_USABLE_GM");
		case Result::PERSONAL_TRADING_ERROR_OTHER_USABLE_IN_SAFE_ZONE: return TEXT("PERSONAL_TRADING_ERROR_OTHER_USABLE_IN_SAFE_ZONE");
		case Result::PERSONAL_TRADING_ERROR_NO_DEVICE_REGISTERED: return TEXT("PERSONAL_TRADING_ERROR_NO_DEVICE_REGISTERED");
		case Result::COMMUNITY_EVENT_MISSION_INVALID_USER_STATUS: return TEXT("COMMUNITY_EVENT_MISSION_INVALID_USER_STATUS");
		case Result::COMMUNITY_EVENT_MISSION_INVALID_DATA: return TEXT("COMMUNITY_EVENT_MISSION_INVALID_DATA");
		case Result::COMMUNITY_EVENT_MISSION_NOT_EVENT_PERIOD: return TEXT("COMMUNITY_EVENT_MISSION_NOT_EVENT_PERIOD");
		case Result::COMMUNITY_EVENT_MISSION_NOT_TAB_OPEN_PERIOD: return TEXT("COMMUNITY_EVENT_MISSION_NOT_TAB_OPEN_PERIOD");
		case Result::COMMUNITY_EVENT_MISSION_NOT_EXIST_MISSION_DATA: return TEXT("COMMUNITY_EVENT_MISSION_NOT_EXIST_MISSION_DATA");
		case Result::COMMUNITY_EVENT_MISSION_NOT_MEET_THE_CONDITION_MISSION_COUNT: return TEXT("COMMUNITY_EVENT_MISSION_NOT_MEET_THE_CONDITION_MISSION_COUNT");
		case Result::COMMUNITY_EVENT_MISSION_IS_ALREADY_REWARDED_MISSION: return TEXT("COMMUNITY_EVENT_MISSION_IS_ALREADY_REWARDED_MISSION");
		case Result::GUILD_AUCTION_ERROR_INVALID_ID: return TEXT("GUILD_AUCTION_ERROR_INVALID_ID");
		case Result::GUILD_AUCTION_ERROR_EXPIRED_DATE_TIME: return TEXT("GUILD_AUCTION_ERROR_EXPIRED_DATE_TIME");
		case Result::GUILD_AUCTION_ERROR_DIVISION_STATE: return TEXT("GUILD_AUCTION_ERROR_DIVISION_STATE");
		case Result::GUILD_AUCTION_ERROR_ENOUGH_CURRENCY: return TEXT("GUILD_AUCTION_ERROR_ENOUGH_CURRENCY");
		case Result::GUILD_AUCTION_ERROR_APPOINTMENT: return TEXT("GUILD_AUCTION_ERROR_APPOINTMENT");
		case Result::GUILD_AUCTION_ERROR_PARTICIPANTS: return TEXT("GUILD_AUCTION_ERROR_PARTICIPANTS");
		case Result::GUILD_AUCTION_ERROR_DB: return TEXT("GUILD_AUCTION_ERROR_DB");
		case Result::GUILD_AUCTION_ERROR_BUY_STATE: return TEXT("GUILD_AUCTION_ERROR_BUY_STATE");
		case Result::GUILD_AUCTION_ERROR_RETURN_CURRENCY: return TEXT("GUILD_AUCTION_ERROR_RETURN_CURRENCY");
		case Result::GUILD_AUCTION_ERROR_DUPLICATION_REGISTER: return TEXT("GUILD_AUCTION_ERROR_DUPLICATION_REGISTER");
		case Result::GUILD_LEAVE_ERROR_AUCTION_DATA: return TEXT("GUILD_LEAVE_ERROR_AUCTION_DATA");
		case Result::GUILD_AUCTION_ERROR_JOINED_TIME: return TEXT("GUILD_AUCTION_ERROR_JOINED_TIME");
		case Result::GUILD_AUCTION_ERROR_FULL_REGISTRATION: return TEXT("GUILD_AUCTION_ERROR_FULL_REGISTRATION");
		case Result::GUILD_AUCTION_ERROR_HIGHER_BID: return TEXT("GUILD_AUCTION_ERROR_HIGHER_BID");
		case Result::GUILD_AUCTION_ERROR_EMPTY_ITEM: return TEXT("GUILD_AUCTION_ERROR_EMPTY_ITEM");
		case Result::GUILD_AUCTION_ERROR_SAFE_CAPACITY_FULL: return TEXT("GUILD_AUCTION_ERROR_SAFE_CAPACITY_FULL");
		case Result::GUILD_SNS_URL_ERROR_INVALID_ID: return TEXT("GUILD_SNS_URL_ERROR_INVALID_ID");
		case Result::GUILD_SNS_URL_ERROR_INVALID_LEN: return TEXT("GUILD_SNS_URL_ERROR_INVALID_LEN");
		case Result::NICKNAME_CHANGE_ERROR_NOT_ALLOWED_NAME: return TEXT("NICKNAME_CHANGE_ERROR_NOT_ALLOWED_NAME");
		case Result::NICKNAME_CHANGE_ERROR_INVALID_LENGTH_NAME: return TEXT("NICKNAME_CHANGE_ERROR_INVALID_LENGTH_NAME");
		case Result::NICKNAME_CHANGE_ERROR_DUPLICATION_NAME: return TEXT("NICKNAME_CHANGE_ERROR_DUPLICATION_NAME");
		case Result::NICKNAME_CHANGE_ERROR_NOT_TOWN: return TEXT("NICKNAME_CHANGE_ERROR_NOT_TOWN");
		case Result::NICKNAME_CHANGE_ERROR_GUILD_APPLICANTS: return TEXT("NICKNAME_CHANGE_ERROR_GUILD_APPLICANTS");
		case Result::NICKNAME_CHANGE_ERROR_DB: return TEXT("NICKNAME_CHANGE_ERROR_DB");
		case Result::INVADE_ERROR_PORTAL_SPAWNING: return TEXT("INVADE_ERROR_PORTAL_SPAWNING");
		case Result::SERVER_MIGRATE_ERROR_UNKNOWN_ERROR: return TEXT("SERVER_MIGRATE_ERROR_UNKNOWN_ERROR");
		case Result::SERVER_MIGRATE_ERROR_INVALID_USER_STATUS: return TEXT("SERVER_MIGRATE_ERROR_INVALID_USER_STATUS");
		case Result::SERVER_MIGRATE_ERROR_NOT_MEET_THE_CONDITION: return TEXT("SERVER_MIGRATE_ERROR_NOT_MEET_THE_CONDITION");
		case Result::SERVER_MIGRATE_ERROR_ALREADY_IN_PROCESS: return TEXT("SERVER_MIGRATE_ERROR_ALREADY_IN_PROCESS");
		case Result::SERVER_MIGRATE_ERROR_SERVER_SESSION_IS_INVALID: return TEXT("SERVER_MIGRATE_ERROR_SERVER_SESSION_IS_INVALID");
		case Result::SERVER_MIGRATE_ERROR_DB_QUERY_FAILED: return TEXT("SERVER_MIGRATE_ERROR_DB_QUERY_FAILED");
		case Result::SERVER_MIGRATE_ERROR_DATA_TRANSFER_IN_PROCESS: return TEXT("SERVER_MIGRATE_ERROR_DATA_TRANSFER_IN_PROCESS");
		case Result::SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_ACCOUNT: return TEXT("SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_ACCOUNT");
		case Result::SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_USER: return TEXT("SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_USER");
		case Result::SERVER_MIGRATE_ERROR_CHECK_CONDITION_GUILD_MEMBER: return TEXT("SERVER_MIGRATE_ERROR_CHECK_CONDITION_INVALID_USER");
		case Result::SERVER_MIGRATE_ERROR_NICKNAME_ALREADY_CHANGED: return TEXT("SERVER_MIGRATE_ERROR_NICKNAME_ALREADY_CHANGED");
		case Result::SERVER_MIGRATE_ERROR_GUILD_NAME_ALREADY_CHANGED: return TEXT("SERVER_MIGRATE_ERROR_GUILD_NAME_ALREADY_CHANGED");
		case Result::SERVER_MIGRATE_ERROR_TARGET_SERVER_EXCEED_INCOMING_COUNT: return TEXT("SERVER_MIGRATE_ERROR_TARGET_SERVER_EXCEED_INCOMING_COUNT");
		case Result::SERVER_MIGRATE_ERROR_CURRENT_SERVER_EXCEED_OUTGOING_COUNT: return TEXT("SERVER_MIGRATE_ERROR_CURRENT_SERVER_EXCEED_OUTGOING_COUNT");
		case Result::SERVER_MIGRATE_ERROR_NOT_ENOUGH_CURRENCY: return TEXT("SERVER_MIGRATE_ERROR_NOT_ENOUGH_CURRENCY");
		case Result::ABUSE_PACKET_SHORT_TICK: return TEXT("ABUSE_PACKET_SHORT_TICK");
		case Result::GUILD_WAR_UNKNOWN_ERROR: return TEXT("GUILD_WAR_UNKNOWN_ERROR");
		case Result::GUILD_WAR_MATCH_NOT_FOUND_ENEMY_GUILD: return TEXT("GUILD_WAR_MATCH_NOT_FOUND_ENEMY_GUILD");
		case Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MINIMUM_LIMIT: return TEXT("GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MINIMUM_LIMIT");
		case Result::GUILD_WAR_ENTER_FAIL_NOT_REGISTERED_IN_ENTRY: return TEXT("GUILD_WAR_ENTER_FAIL_NOT_REGISTERED_IN_ENTRY");
		case Result::GUILD_WAR_ENTER_FAIL_INVALID_STATE: return TEXT("GUILD_WAR_ENTER_FAIL_INVALID_STATE");
		case Result::GUILD_WAR_MATCH_REGISTER_FAIL_ALREADY_REGISTER: return TEXT("GUILD_WAR_MATCH_REGISTER_FAIL_ALREADY_REGISTER");
		case Result::GUILD_WAR_MATCH_REGISTER_FAIL_STATE: return TEXT("GUILD_WAR_MATCH_REGISTER_FAIL_STATE");
		case Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MAX_LIMIT: return TEXT("GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MAX_LIMIT");
		case Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_GRADE_LIMIT: return TEXT("GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_GRADE_LIMIT");
		case Result::GUILD_WAR_MATCH_REGISTER_FAIL_LEVEL: return TEXT("GUILD_WAR_MATCH_REGISTER_FAIL_LEVEL");
		case Result::GUILD_WAR_ERROR_THE_DAY_JOINED_GUILD: return TEXT("GUILD_WAR_ERROR_THE_DAY_JOINED_GUILD");
		case Result::AGIT_DUNGEON_RAID_UNKNOWN_ERROR: return TEXT("AGIT_DUNGEON_RAID_UNKNOWN_ERROR");
		case Result::AGIT_DUNGEON_RAID_WORLD_UNKNOWN_ERROR: return TEXT("AGIT_DUNGEON_RAID_WORLD_UNKNOWN_ERROR");
		case Result::AGIT_DUNGEON_RAID_ACCOUNT_DATA: return TEXT("AGIT_DUNGEON_RAID_ACCOUNT_DATA");
		case Result::AGIT_DUNGEON_RAID_NOT_YET_RESET: return TEXT("AGIT_DUNGEON_RAID_NOT_YET_RESET");
		case Result::AGIT_DUNGEON_RAID_NOT_EXIST_DETAIL_DATA: return TEXT("AGIT_DUNGEON_RAID_NOT_EXIST_DETAIL_DATA");
		case Result::AGIT_DUNGEON_RAID_INVALID_USER_STATE: return TEXT("AGIT_DUNGEON_RAID_INVALID_USER_STATE");
		case Result::AGIT_DUNGEON_RAID_INVALID_PARRAM: return TEXT("AGIT_DUNGEON_RAID_INVALID_PARRAM");
		case Result::AGIT_DUNGEON_RAID_NOT_GUILD_MEMBER: return TEXT("AGIT_DUNGEON_RAID_NOT_GUILD_MEMBER");
		case Result::AGIT_DUNGEON_RAID_OPEN_GRADE_ERROR: return TEXT("AGIT_DUNGEON_RAID_OPEN_GRADE_ERROR");
		case Result::AGIT_DUNGEON_RAID_CONFIG_DATA_ERROR: return TEXT("AGIT_DUNGEON_RAID_CONFIG_DATA_ERROR");
		case Result::AGIT_DUNGEON_RAID_CONTENTS_GUILD_LEVEL_ERROR: return TEXT("AGIT_DUNGEON_RAID_CONTENTS_GUILD_LEVEL_ERROR");
		case Result::AGIT_DUNGEON_RAID_GUILD_LEVEL_ERROR: return TEXT("AGIT_DUNGEON_RAID_GUILD_LEVEL_ERROR");
		case Result::AGIT_DUNGEON_RAID_DETAIL_DATA_ERROR: return TEXT("AGIT_DUNGEON_RAID_DETAIL_DATA_ERROR");
		case Result::AGIT_DUNGEON_RAID_ALREADY_OPENED: return TEXT("AGIT_DUNGEON_RAID_ALREADY_OPENED");
		case Result::AGIT_DUNGEON_RAID_NO_MAP_DATA: return TEXT("AGIT_DUNGEON_RAID_NO_MAP_DATA");
		case Result::AGIT_DUNGEON_RAID_NOT_ENOUGH_GUILD_ASSET: return TEXT("AGIT_DUNGEON_RAID_NOT_ENOUGH_GUILD_ASSET");
		case Result::AGIT_DUNGEON_RAID_FAIL_TO_CREATE_CHANNEL: return TEXT("AGIT_DUNGEON_RAID_FAIL_TO_CREATE_CHANNEL");
		case Result::AGIT_DUNGEON_RAID_NO_USER_DATA: return TEXT("AGIT_DUNGEON_RAID_NO_USER_DATA");
		case Result::AGIT_DUNGEON_RAID_GUILD_JOINED_DAY_ERROR: return TEXT("AGIT_DUNGEON_RAID_GUILD_JOINED_DAY_ERROR");
		case Result::AGIT_DUNGEON_RAID_MAX_CLEAR_COUNT: return TEXT("AGIT_DUNGEON_RAID_MAX_CLEAR_COUNT");
		case Result::AGIT_DUNGEON_RAID_NOT_AVAILABLE_TIME: return TEXT("AGIT_DUNGEON_RAID_NOT_AVAILABLE_TIME");
		case Result::AGIT_DUNGEON_RAID_DUNGEON_NOT_CLOSED: return TEXT("AGIT_DUNGEON_RAID_DUNGEON_NOT_CLOSED");
		case Result::AGIT_DUNGEON_RAID_DB_OPERATION_ERROR: return TEXT("AGIT_DUNGEON_RAID_DB_OPERATION_ERROR");
		case Result::AGIT_DUNGEON_RAID_NO_MAP_TO_CLOSE: return TEXT("AGIT_DUNGEON_RAID_NO_MAP_TO_CLOSE");
		case Result::AGIT_DUNGEON_RAID_NO_CHANNEL_TO_CLOSE: return TEXT("AGIT_DUNGEON_RAID_NO_MAP_TO_CLOSE");
		case Result::AGIT_DUNGEON_RAID_FAIL_TO_CREATE_RAID_DATA: return TEXT("AGIT_DUNGEON_RAID_FAIL_TO_CREATE_RAID_DATA");
		case Result::AGIT_DUNGEON_RAID_NOT_EXIST_RAID_DATA: return TEXT("AGIT_DUNGEON_RAID_NOT_EXIST_RAID_DATA");
		case Result::AGIT_DUNGEON_RAID_DUNGEON_NOT_OPENED: return TEXT("AGIT_DUNGEON_RAID_DUNGEON_NOT_OPENED");
		case Result::AGIT_DUNGEON_RAID_NOT_EXIST_USER_CHANNEL: return TEXT("AGIT_DUNGEON_RAID_NOT_EXIST_USER_CHANNEL");
		case Result::AGIT_DUNGEON_RAID_MAP_TYPE_ERROR: return TEXT("AGIT_DUNGEON_RAID_MAP_TYPE_ERROR");
		case Result::AGIT_DUNGEON_NO_CHANNEL_TO_ENTER: return TEXT("AGIT_DUNGEON_NO_CHANNEL_TO_ENTER");
		case Result::AGIT_DUNGEON_RAID_WARP_ERROR_ABNORMALITY_EFFECT: return TEXT("AGIT_DUNGEON_RAID_WARP_ERROR_ABNORMALITY_EFFECT");
		case Result::AGIT_DUNGEON_RAID_NOT_ENOUGH_USER_TICKET_COUNT: return TEXT("AGIT_DUNGEON_RAID_NOT_ENOUGH_USER_TICKET_COUNT");
		case Result::AGIT_DUNGEON_RAID_NOT_FOUND_GUILD: return TEXT("AGIT_DUNGEON_RAID_NOT_FOUND_GUILD");
		case Result::AGIT_DUNGEON_RAID_DESTROYING: return TEXT("AGIT_DUNGEON_RAID_DESTROYING");
		case Result::AGIT_DUNGEON_RAID_FAIL_TO_CLOSE_USER_EXISTS: return TEXT("AGIT_DUNGEON_RAID_FAIL_TO_CLOSE_USER_EXISTS");
		case Result::CHAT_GUILD_GROUP_INVALID_ROOM_NAME: return TEXT("CHAT_GUILD_GROUP_INVALID_ROOM_NAME");
		case Result::CHAT_GUILD_GROUP_JOIN_OVER_LIMIT: return TEXT("CHAT_GUILD_GROUP_JOIN_OVER_LIMIT");
		case Result::CHAT_GUILD_GROUP_MEMBER_OVER_LIMIT: return TEXT("CHAT_GUILD_GROUP_MEMBER_OVER_LIMIT");
		case Result::CHAT_GUILD_GROUP_INVITE_OVER_LIMIT: return TEXT("CHAT_GUILD_GROUP_INVITE_OVER_LIMIT");
		case Result::CHAT_GUILD_GROUP_INVALID_ROOM_ID: return TEXT("CHAT_GUILD_GROUP_INVALID_ROOM_ID");
		case Result::CHAT_GUILD_GROUP_ALREADY_INVITED: return TEXT("CHAT_GUILD_GROUP_ALREADY_INVITED");
		case Result::CHAT_GUILD_GROUP_NOT_INVITED: return TEXT("CHAT_GUILD_GROUP_NOT_INVITED");
		case Result::CHAT_GUILD_GROUP_ALREADY_JOINED: return TEXT("CHAT_GUILD_GROUP_ALREADY_JOINED");
		case Result::CHAT_GUILD_GROUP_DB_FAILED: return TEXT("CHAT_GUILD_GROUP_DB_FAILED");
		case Result::CHAT_GUILD_GROUP_NOT_LEADER_GUILD: return TEXT("CHAT_GUILD_GROUP_NOT_LEADER_GUILD");
		case Result::CHAT_GUILD_GROUP_NOT_JOIN_GUILD: return TEXT("CHAT_GUILD_GROUP_NOT_JOIN_GUILD");
		case Result::CHAT_GUILD_GROUP_DO_NOT_WITHDRAWAL: return TEXT("CHAT_GUILD_GROUP_DO_NOT_WITHDRAWAL");
		case Result::CHAT_GUILD_GROUP_DO_WRONG_GUILD_ID: return TEXT("CHAT_GUILD_GROUP_DO_WRONG_GUILD_ID");
		case Result::CHAT_GUILD_GROUP_DATA_DIFF: return TEXT("CHAT_GUILD_GROUP_DATA_DIFF");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidResult(Result value) noexcept
{
	return (value >= Result::RESULT_SUCCESS && value < Result::PACKET_RESULT_MAX);
}

}	// namespace PD
