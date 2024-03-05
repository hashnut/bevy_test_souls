#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedGuildEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 기사단일일활동타입
enum class GuildDailyActivityType : uint8
{

	// 출석
	ATTENDANCE = 0,

	// 일반기부
	NORMAL_DONATE = 1,

	// 고급기부
	ADVANCED_DONATE = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 기사단등급
enum class GuildMemberGradeType : uint8
{

	// 초기값용
	UNKNOWN = 0,

	// 일반기사
	KNIGHT = 1,

	// 이급기사
	SECONDKNIGHT = 2,

	// 일급기사
	FIRSTKNIGHT = 3,

	// 부단장
	SUBCAPTAIN = 4,

	// 기사단장
	CAPTAIN = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// 기사단가입유형
enum class GuildJoinType : uint8
{

	// 즉시가입 유형
	IMMEDIATE = 0,

	// 승인가입 유형
	APPROVAL = 1,

	// 가입불가 유형
	UNABLE = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 가입상태
enum class GuildJoinState : uint8
{

	// 미 가입 상태
	ABLE_JOIN_STATE = 0,

	// 요청 중인 상태
	REQUESTED_STATE = 1,

	// 가입 상태
	JOINED_STATE = 2,

	// 재 가입 제한 상태
	RESTRICTED_REJOIN = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 기사단 구매제한 타입
enum class GuildShopPurchaseLimitType : uint8
{

	// 구매제한 없음
	GUILDSHOP_PURCHASE_LIMIT_OF_NONE = 0,

	// 일간 구매 제한
	GUILDSHOP_PURCHASE_LIMIT_OF_DAY = 1,

	// 주간 구매 제한
	GUILDSHOP_PURCHASE_LIMIT_OF_WEEK = 2,

	// 월간 구매 제한
	GUILDSHOP_PURCHASE_LIMIT_OF_MONTH = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 기사단 능력치  티입
enum class GuildStatType : uint8
{

	// 없음
	NONE = 0,

	// 길드 레벨
	GUILD_LEVEL = 1,

	// 최대 길드 인원
	GUILD_USER_NUMBER = 2,

	// 길드 버프
	GUILD_BUFF = 3,

	// 건물 패시브 스탯
	GUILD_PASSIVE_STAT = 4,

	// 기사단 캠프
	GUILD_CAMP = 5,

	// 기사단 던전 버프(기사단 던전 클리어 단계 조건)
	GUILD_DUNGEON_BUFF = 6,

	MAX = 7,
};

UENUM(BlueprintType)
// 기사단 건물 데이터가 변경 되는 경우
enum class GuildBuildingNotifyType : uint8
{

	// 없음
	NONE = 0,

	// 신규 건물 오픈
	OPEN = 1,

	// 신규 건물 오픈 만료
	OPEN_CLOSE = 2,

	// 업그레이드 시작 
	UPGRADE_START = 3,

	// 업그레이드 완료
	UPGRADE_END = 4,

	// 건물 패시브 시작
	PASSIVITY_START = 5,

	// 건물 패시브 종료
	PASSIVITY_END = 6,

	// 건물 패시브 재 사용 시간 종료
	BUFF_REACTIVATE_TIME = 7,

	// 캠프 생성
	ENTER_CAMP = 8,

	// 캠프 소멸
	LEAVE_CAMP = 9,

	MAX = 10,
};

UENUM(BlueprintType)
// 기사단 건물 상태
enum class GuildBuildingStateType : uint8
{

	// 없음
	GUILD_BUILDING_STATE_NONE = 0,

	// 건물 오픈
	GUILD_BUILDING_STATE_OPEN = 1,

	// 건물 업그레이드
	GUILD_BUILDING_STATE_UPGRADE = 2,

	MAX = 3,
};

UENUM()
// 길드전 상태
enum class GuildWarState : uint8
{

	// 기본값
	NONE = 0,

	// 길드전등록
	REGISTER = 1,

	// 길드전 준비 
	READY = 2,

	// 길드전 플레이 시작대기
	PLAYWAIT = 3,

	// 길드전플레이
	PLAY = 4,

	// 길드전종료
	END = 5,

	// 길드전닫음
	DESTORY = 6,

	MAX = 7,
};

UENUM()
// 길드전 종료 이유
enum class GuildWarEndReason : uint8
{

	// 길드워보스처치
	KILL_BOSS = 0,

	// 게임타임아웃종료
	TIMEOUT = 1,

	// 게임팀인원미달종료
	TEAM_MEMBER_MINIMUM_CUT = 2,

	// 목표포인트달성
	POINT = 3,

	MAX = 4,
};

UENUM()
// 기사단 동맹 요청 타입
enum class GuildAllyType : uint8
{

	// 아무것도아님
	GUILD_ALLY_NONE = 0,

	// 동맹 신청 대기
	GUILD_ALLY_WAITING_FOR_REQUESTS = 1,

	// 동맹 수락 대기
	GUILD_ALLY_WAITING_FOR_ACCEPTANCE = 2,

	// 동맹 요청 취소/거절
	GUILD_ALLY_WAITING_CANCEL = 3,

	// 동맹 요청 타임 아웃
	GUILD_ALLY_TIME_OUT = 4,

	// 동맹 수락
	GUILD_ALLY_ACCEPT = 5,

	// 동맹 파기
	GUILD_ALLY_BREAK = 6,

	// 동맹 신청 대기(인터서버 일때)
	GUILD_ALLY_REQUESTS_FOR_INTER = 7,

	// 동맹 파괴(인터서버 일때)
	GUILD_ALLY_BREAK_FOR_INTER = 8,

	// 동맹 수락(인터서버 일때)
	GUILD_ALLY_ACCEPT_FOR_INTER = 9,

	// 동맹 수락 실패(인터서버 일때)
	GUILD_ALLY_REQUEST_FAIL_FOR_INTER = 10,

	// 동맹 수락(인터서버 일때)
	GUILD_ALLY_ACCEPT_FAIL_FOR_INTER = 11,

	MAX = 12,
};

UENUM(BlueprintType)
// 기사단 아이템 콜렉션
enum class GuildItemCollectionType : uint8
{

	// 기사단 아이템 콜렉션 없음
	NONE = 0,

	// 이벤트 콜렉션 타입(공용)
	EVENT_SHARING = 1,

	// 일반 콜렉션 타입(공용)
	SHARING = 2,

	// 무작위 콜렉션 타입(전용)
	RANDOM = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 길드 아이템 획득 출처
enum class GuildItemAcquireType : uint8
{

	// 획득 출처 없음
	NONE = 0,

	// 기사단 던전 보상
	DUNGEON = 1,

	// 필드 보스
	FIELD_BOSS = 2,

	// 영지 공략 던전
	AGIT_DUNGEON_RAID = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 길드 캠프 상태
enum class GuildCampStateType : uint8
{

	// 없음
	NONE = 0,

	// 건설중
	READY = 1,

	// 건설 완료
	COMPLETE = 2,

	// 점령
	OCCUPIED = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 길드 캠프 파괴 Type
enum class GuildCampDestructionType : uint8
{

	// 없음
	NONE = 0,

	// 유저가 파괴하는 경우
	UserDestruction = 1,

	// 시간 만료
	ExpiredTime = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 기사단 경매장 상태
enum class GuildDivisionState : uint8
{

	// 없음
	NONE = 0,

	// 보관
	STORAGE = 1,

	// 경매 진행중
	AUCTION = 2,

	// 경매 완료
	DONE = 3,

	// 유찰
	FAILED = 4,

	//  메일로 즉시 전송
	SEND_MAIL = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// 기사단 경매장 정산 타입
enum class GuildAuctionResultType : uint8
{

	// 일반
	NORMAL = 0,

	// 참여자
	PARTICIPANTS = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 기사단 분배(경매) 타입
enum class GuildAuctionType : uint8
{

	// 없음
	NONE = 0,

	// 일반
	NORMAL = 1,

	// 지정 경매
	APPOINTMENT = 2,

	// 참여자 경매
	PARTICIPANTS = 3,

	// 직접분배
	DIRECT_DISTRIBUTION = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 기사단 수호탑 데이터가 변경 되는 경우
enum class GuildCampNotifyType : uint8
{

	// 없음
	NONE = 0,

	// 수호탑이 공격을 받는 경우
	UNDER_ATTACK = 1,

	// 수호탑 파괴
	Destruction = 2,

	// 수호탑 해체 
	Dismantle = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 기사단 창고 UI 탭 관련 인덱스 정보
enum class GuildStoreTabType : uint8
{

	// 없음
	NONE = 0,

	// 분배
	DISTRIBUTION = 1,

	// 경매
	AUCTION = 2,

	// 정산
	CALCULATE = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 기사단 창고 & 경매 아이템 메시지 종류
enum class GuildStoreItemNotifyType : uint8
{

	// 없음
	NONE = 0,

	// 등록
	REGISTER = 1,

	// 입찰
	NORMAL = 2,

	// 경매 완료
	DONE = 3,

	// 기사단 던전, 보스 획득
	ACQUISITION = 4,

	// 획득설정 
	ACQUISITION_SETTING = 5,

	// 보관시간초과로 인한 자동 경매 시작
	STORAGE_TIME_AUTO_AUCTION = 6,

	// 획득내역초과로 인한 자동 경매 시작
	ACQUISITION_AUTO_AUCTION = 7,

	// 경매 유찰
	FAILED = 8,

	// 길드 생성시 동기화 목적 - 서버만 사용
	CREATE_GUILD = 9,

	// 직접 분배
	DIRECT = 10,

	MAX = 11,
};

UENUM(BlueprintType)
// 기사단 입찰 실패 이유
enum class GuildStoreAvailableResultType : uint8
{

	// 없음
	NONE = 0,

	// 가입 대기 시간 지나지 않아 경매 참여 불가
	STANDBY = 1,

	// 기사단원 등급이 낮아 참여 불가
	AUTHORITY = 2,

	// 지정 경매 품목의 입찰 대상 아니라서
	APPOINTMENT = 3,

	// 참여자 경매 품목 대상이 아니라서
	ACQUISITION = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 영지 공략 던전 수행 상태
enum class AgitDungeonRaidState : uint8
{

	// 기본 값
	NONE = 0,

	// 공략 시작
	OPEN = 1,

	// 공략 종료
	CLOSED = 2,

	// 공략 완료
	CLEAR = 3,

	MAX = 4,
};

UENUM()
// 영지 공략 던전 티켓 변동
enum class AgitDungeonRaidTicketChange : uint8
{

	// 기본 값
	NONE = 0,

	// 티켓 개수 증가
	INCREASE = 1,

	// 티켓 개수 감소
	DECREASE = 2,

	// 티켓 개수 초기화
	RESET = 3,

	MAX = 4,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline GuildDailyActivityType GuildDailyActivityTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ATTENDANCE"), ESearchCase::IgnoreCase) == 0)	return GuildDailyActivityType::ATTENDANCE;
	if (str.Compare(TEXT("NORMAL_DONATE"), ESearchCase::IgnoreCase) == 0)	return GuildDailyActivityType::NORMAL_DONATE;
	if (str.Compare(TEXT("ADVANCED_DONATE"), ESearchCase::IgnoreCase) == 0)	return GuildDailyActivityType::ADVANCED_DONATE;
	return GuildDailyActivityType::MAX;
}
inline GuildMemberGradeType GuildMemberGradeTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return GuildMemberGradeType::UNKNOWN;
	if (str.Compare(TEXT("KNIGHT"), ESearchCase::IgnoreCase) == 0)	return GuildMemberGradeType::KNIGHT;
	if (str.Compare(TEXT("SECONDKNIGHT"), ESearchCase::IgnoreCase) == 0)	return GuildMemberGradeType::SECONDKNIGHT;
	if (str.Compare(TEXT("FIRSTKNIGHT"), ESearchCase::IgnoreCase) == 0)	return GuildMemberGradeType::FIRSTKNIGHT;
	if (str.Compare(TEXT("SUBCAPTAIN"), ESearchCase::IgnoreCase) == 0)	return GuildMemberGradeType::SUBCAPTAIN;
	if (str.Compare(TEXT("CAPTAIN"), ESearchCase::IgnoreCase) == 0)	return GuildMemberGradeType::CAPTAIN;
	return GuildMemberGradeType::MAX;
}
inline GuildJoinType GuildJoinTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("IMMEDIATE"), ESearchCase::IgnoreCase) == 0)	return GuildJoinType::IMMEDIATE;
	if (str.Compare(TEXT("APPROVAL"), ESearchCase::IgnoreCase) == 0)	return GuildJoinType::APPROVAL;
	if (str.Compare(TEXT("UNABLE"), ESearchCase::IgnoreCase) == 0)	return GuildJoinType::UNABLE;
	return GuildJoinType::MAX;
}
inline GuildJoinState GuildJoinStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ABLE_JOIN_STATE"), ESearchCase::IgnoreCase) == 0)	return GuildJoinState::ABLE_JOIN_STATE;
	if (str.Compare(TEXT("REQUESTED_STATE"), ESearchCase::IgnoreCase) == 0)	return GuildJoinState::REQUESTED_STATE;
	if (str.Compare(TEXT("JOINED_STATE"), ESearchCase::IgnoreCase) == 0)	return GuildJoinState::JOINED_STATE;
	if (str.Compare(TEXT("RESTRICTED_REJOIN"), ESearchCase::IgnoreCase) == 0)	return GuildJoinState::RESTRICTED_REJOIN;
	return GuildJoinState::MAX;
}
inline GuildShopPurchaseLimitType GuildShopPurchaseLimitTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("GUILDSHOP_PURCHASE_LIMIT_OF_NONE"), ESearchCase::IgnoreCase) == 0)	return GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_NONE;
	if (str.Compare(TEXT("GUILDSHOP_PURCHASE_LIMIT_OF_DAY"), ESearchCase::IgnoreCase) == 0)	return GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_DAY;
	if (str.Compare(TEXT("GUILDSHOP_PURCHASE_LIMIT_OF_WEEK"), ESearchCase::IgnoreCase) == 0)	return GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_WEEK;
	if (str.Compare(TEXT("GUILDSHOP_PURCHASE_LIMIT_OF_MONTH"), ESearchCase::IgnoreCase) == 0)	return GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_MONTH;
	return GuildShopPurchaseLimitType::MAX;
}
inline GuildStatType GuildStatTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildStatType::NONE;
	if (str.Compare(TEXT("GUILD_LEVEL"), ESearchCase::IgnoreCase) == 0)	return GuildStatType::GUILD_LEVEL;
	if (str.Compare(TEXT("GUILD_USER_NUMBER"), ESearchCase::IgnoreCase) == 0)	return GuildStatType::GUILD_USER_NUMBER;
	if (str.Compare(TEXT("GUILD_BUFF"), ESearchCase::IgnoreCase) == 0)	return GuildStatType::GUILD_BUFF;
	if (str.Compare(TEXT("GUILD_PASSIVE_STAT"), ESearchCase::IgnoreCase) == 0)	return GuildStatType::GUILD_PASSIVE_STAT;
	if (str.Compare(TEXT("GUILD_CAMP"), ESearchCase::IgnoreCase) == 0)	return GuildStatType::GUILD_CAMP;
	if (str.Compare(TEXT("GUILD_DUNGEON_BUFF"), ESearchCase::IgnoreCase) == 0)	return GuildStatType::GUILD_DUNGEON_BUFF;
	return GuildStatType::MAX;
}
inline GuildBuildingNotifyType GuildBuildingNotifyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::NONE;
	if (str.Compare(TEXT("OPEN"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::OPEN;
	if (str.Compare(TEXT("OPEN_CLOSE"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::OPEN_CLOSE;
	if (str.Compare(TEXT("UPGRADE_START"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::UPGRADE_START;
	if (str.Compare(TEXT("UPGRADE_END"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::UPGRADE_END;
	if (str.Compare(TEXT("PASSIVITY_START"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::PASSIVITY_START;
	if (str.Compare(TEXT("PASSIVITY_END"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::PASSIVITY_END;
	if (str.Compare(TEXT("BUFF_REACTIVATE_TIME"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::BUFF_REACTIVATE_TIME;
	if (str.Compare(TEXT("ENTER_CAMP"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::ENTER_CAMP;
	if (str.Compare(TEXT("LEAVE_CAMP"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingNotifyType::LEAVE_CAMP;
	return GuildBuildingNotifyType::MAX;
}
inline GuildBuildingStateType GuildBuildingStateTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("GUILD_BUILDING_STATE_NONE"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingStateType::GUILD_BUILDING_STATE_NONE;
	if (str.Compare(TEXT("GUILD_BUILDING_STATE_OPEN"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingStateType::GUILD_BUILDING_STATE_OPEN;
	if (str.Compare(TEXT("GUILD_BUILDING_STATE_UPGRADE"), ESearchCase::IgnoreCase) == 0)	return GuildBuildingStateType::GUILD_BUILDING_STATE_UPGRADE;
	return GuildBuildingStateType::MAX;
}
inline GuildWarState GuildWarStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildWarState::NONE;
	if (str.Compare(TEXT("REGISTER"), ESearchCase::IgnoreCase) == 0)	return GuildWarState::REGISTER;
	if (str.Compare(TEXT("READY"), ESearchCase::IgnoreCase) == 0)	return GuildWarState::READY;
	if (str.Compare(TEXT("PLAYWAIT"), ESearchCase::IgnoreCase) == 0)	return GuildWarState::PLAYWAIT;
	if (str.Compare(TEXT("PLAY"), ESearchCase::IgnoreCase) == 0)	return GuildWarState::PLAY;
	if (str.Compare(TEXT("END"), ESearchCase::IgnoreCase) == 0)	return GuildWarState::END;
	if (str.Compare(TEXT("DESTORY"), ESearchCase::IgnoreCase) == 0)	return GuildWarState::DESTORY;
	return GuildWarState::MAX;
}
inline GuildWarEndReason GuildWarEndReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("KILL_BOSS"), ESearchCase::IgnoreCase) == 0)	return GuildWarEndReason::KILL_BOSS;
	if (str.Compare(TEXT("TIMEOUT"), ESearchCase::IgnoreCase) == 0)	return GuildWarEndReason::TIMEOUT;
	if (str.Compare(TEXT("TEAM_MEMBER_MINIMUM_CUT"), ESearchCase::IgnoreCase) == 0)	return GuildWarEndReason::TEAM_MEMBER_MINIMUM_CUT;
	if (str.Compare(TEXT("POINT"), ESearchCase::IgnoreCase) == 0)	return GuildWarEndReason::POINT;
	return GuildWarEndReason::MAX;
}
inline GuildAllyType GuildAllyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("GUILD_ALLY_NONE"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_NONE;
	if (str.Compare(TEXT("GUILD_ALLY_WAITING_FOR_REQUESTS"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_WAITING_FOR_REQUESTS;
	if (str.Compare(TEXT("GUILD_ALLY_WAITING_FOR_ACCEPTANCE"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_WAITING_FOR_ACCEPTANCE;
	if (str.Compare(TEXT("GUILD_ALLY_WAITING_CANCEL"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_WAITING_CANCEL;
	if (str.Compare(TEXT("GUILD_ALLY_TIME_OUT"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_TIME_OUT;
	if (str.Compare(TEXT("GUILD_ALLY_ACCEPT"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_ACCEPT;
	if (str.Compare(TEXT("GUILD_ALLY_BREAK"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_BREAK;
	if (str.Compare(TEXT("GUILD_ALLY_REQUESTS_FOR_INTER"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_REQUESTS_FOR_INTER;
	if (str.Compare(TEXT("GUILD_ALLY_BREAK_FOR_INTER"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_BREAK_FOR_INTER;
	if (str.Compare(TEXT("GUILD_ALLY_ACCEPT_FOR_INTER"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_ACCEPT_FOR_INTER;
	if (str.Compare(TEXT("GUILD_ALLY_REQUEST_FAIL_FOR_INTER"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_REQUEST_FAIL_FOR_INTER;
	if (str.Compare(TEXT("GUILD_ALLY_ACCEPT_FAIL_FOR_INTER"), ESearchCase::IgnoreCase) == 0)	return GuildAllyType::GUILD_ALLY_ACCEPT_FAIL_FOR_INTER;
	return GuildAllyType::MAX;
}
inline GuildItemCollectionType GuildItemCollectionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildItemCollectionType::NONE;
	if (str.Compare(TEXT("EVENT_SHARING"), ESearchCase::IgnoreCase) == 0)	return GuildItemCollectionType::EVENT_SHARING;
	if (str.Compare(TEXT("SHARING"), ESearchCase::IgnoreCase) == 0)	return GuildItemCollectionType::SHARING;
	if (str.Compare(TEXT("RANDOM"), ESearchCase::IgnoreCase) == 0)	return GuildItemCollectionType::RANDOM;
	return GuildItemCollectionType::MAX;
}
inline GuildItemAcquireType GuildItemAcquireTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildItemAcquireType::NONE;
	if (str.Compare(TEXT("DUNGEON"), ESearchCase::IgnoreCase) == 0)	return GuildItemAcquireType::DUNGEON;
	if (str.Compare(TEXT("FIELD_BOSS"), ESearchCase::IgnoreCase) == 0)	return GuildItemAcquireType::FIELD_BOSS;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID"), ESearchCase::IgnoreCase) == 0)	return GuildItemAcquireType::AGIT_DUNGEON_RAID;
	return GuildItemAcquireType::MAX;
}
inline GuildCampStateType GuildCampStateTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildCampStateType::NONE;
	if (str.Compare(TEXT("READY"), ESearchCase::IgnoreCase) == 0)	return GuildCampStateType::READY;
	if (str.Compare(TEXT("COMPLETE"), ESearchCase::IgnoreCase) == 0)	return GuildCampStateType::COMPLETE;
	if (str.Compare(TEXT("OCCUPIED"), ESearchCase::IgnoreCase) == 0)	return GuildCampStateType::OCCUPIED;
	return GuildCampStateType::MAX;
}
inline GuildCampDestructionType GuildCampDestructionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildCampDestructionType::NONE;
	if (str.Compare(TEXT("UserDestruction"), ESearchCase::IgnoreCase) == 0)	return GuildCampDestructionType::UserDestruction;
	if (str.Compare(TEXT("ExpiredTime"), ESearchCase::IgnoreCase) == 0)	return GuildCampDestructionType::ExpiredTime;
	return GuildCampDestructionType::MAX;
}
inline GuildDivisionState GuildDivisionStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildDivisionState::NONE;
	if (str.Compare(TEXT("STORAGE"), ESearchCase::IgnoreCase) == 0)	return GuildDivisionState::STORAGE;
	if (str.Compare(TEXT("AUCTION"), ESearchCase::IgnoreCase) == 0)	return GuildDivisionState::AUCTION;
	if (str.Compare(TEXT("DONE"), ESearchCase::IgnoreCase) == 0)	return GuildDivisionState::DONE;
	if (str.Compare(TEXT("FAILED"), ESearchCase::IgnoreCase) == 0)	return GuildDivisionState::FAILED;
	if (str.Compare(TEXT("SEND_MAIL"), ESearchCase::IgnoreCase) == 0)	return GuildDivisionState::SEND_MAIL;
	return GuildDivisionState::MAX;
}
inline GuildAuctionResultType GuildAuctionResultTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return GuildAuctionResultType::NORMAL;
	if (str.Compare(TEXT("PARTICIPANTS"), ESearchCase::IgnoreCase) == 0)	return GuildAuctionResultType::PARTICIPANTS;
	return GuildAuctionResultType::MAX;
}
inline GuildAuctionType GuildAuctionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildAuctionType::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return GuildAuctionType::NORMAL;
	if (str.Compare(TEXT("APPOINTMENT"), ESearchCase::IgnoreCase) == 0)	return GuildAuctionType::APPOINTMENT;
	if (str.Compare(TEXT("PARTICIPANTS"), ESearchCase::IgnoreCase) == 0)	return GuildAuctionType::PARTICIPANTS;
	if (str.Compare(TEXT("DIRECT_DISTRIBUTION"), ESearchCase::IgnoreCase) == 0)	return GuildAuctionType::DIRECT_DISTRIBUTION;
	return GuildAuctionType::MAX;
}
inline GuildCampNotifyType GuildCampNotifyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildCampNotifyType::NONE;
	if (str.Compare(TEXT("UNDER_ATTACK"), ESearchCase::IgnoreCase) == 0)	return GuildCampNotifyType::UNDER_ATTACK;
	if (str.Compare(TEXT("Destruction"), ESearchCase::IgnoreCase) == 0)	return GuildCampNotifyType::Destruction;
	if (str.Compare(TEXT("Dismantle"), ESearchCase::IgnoreCase) == 0)	return GuildCampNotifyType::Dismantle;
	return GuildCampNotifyType::MAX;
}
inline GuildStoreTabType GuildStoreTabTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildStoreTabType::NONE;
	if (str.Compare(TEXT("DISTRIBUTION"), ESearchCase::IgnoreCase) == 0)	return GuildStoreTabType::DISTRIBUTION;
	if (str.Compare(TEXT("AUCTION"), ESearchCase::IgnoreCase) == 0)	return GuildStoreTabType::AUCTION;
	if (str.Compare(TEXT("CALCULATE"), ESearchCase::IgnoreCase) == 0)	return GuildStoreTabType::CALCULATE;
	return GuildStoreTabType::MAX;
}
inline GuildStoreItemNotifyType GuildStoreItemNotifyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::NONE;
	if (str.Compare(TEXT("REGISTER"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::REGISTER;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::NORMAL;
	if (str.Compare(TEXT("DONE"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::DONE;
	if (str.Compare(TEXT("ACQUISITION"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::ACQUISITION;
	if (str.Compare(TEXT("ACQUISITION_SETTING"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::ACQUISITION_SETTING;
	if (str.Compare(TEXT("STORAGE_TIME_AUTO_AUCTION"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::STORAGE_TIME_AUTO_AUCTION;
	if (str.Compare(TEXT("ACQUISITION_AUTO_AUCTION"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::ACQUISITION_AUTO_AUCTION;
	if (str.Compare(TEXT("FAILED"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::FAILED;
	if (str.Compare(TEXT("CREATE_GUILD"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::CREATE_GUILD;
	if (str.Compare(TEXT("DIRECT"), ESearchCase::IgnoreCase) == 0)	return GuildStoreItemNotifyType::DIRECT;
	return GuildStoreItemNotifyType::MAX;
}
inline GuildStoreAvailableResultType GuildStoreAvailableResultTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildStoreAvailableResultType::NONE;
	if (str.Compare(TEXT("STANDBY"), ESearchCase::IgnoreCase) == 0)	return GuildStoreAvailableResultType::STANDBY;
	if (str.Compare(TEXT("AUTHORITY"), ESearchCase::IgnoreCase) == 0)	return GuildStoreAvailableResultType::AUTHORITY;
	if (str.Compare(TEXT("APPOINTMENT"), ESearchCase::IgnoreCase) == 0)	return GuildStoreAvailableResultType::APPOINTMENT;
	if (str.Compare(TEXT("ACQUISITION"), ESearchCase::IgnoreCase) == 0)	return GuildStoreAvailableResultType::ACQUISITION;
	return GuildStoreAvailableResultType::MAX;
}
inline AgitDungeonRaidState AgitDungeonRaidStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AgitDungeonRaidState::NONE;
	if (str.Compare(TEXT("OPEN"), ESearchCase::IgnoreCase) == 0)	return AgitDungeonRaidState::OPEN;
	if (str.Compare(TEXT("CLOSED"), ESearchCase::IgnoreCase) == 0)	return AgitDungeonRaidState::CLOSED;
	if (str.Compare(TEXT("CLEAR"), ESearchCase::IgnoreCase) == 0)	return AgitDungeonRaidState::CLEAR;
	return AgitDungeonRaidState::MAX;
}
inline AgitDungeonRaidTicketChange AgitDungeonRaidTicketChangeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AgitDungeonRaidTicketChange::NONE;
	if (str.Compare(TEXT("INCREASE"), ESearchCase::IgnoreCase) == 0)	return AgitDungeonRaidTicketChange::INCREASE;
	if (str.Compare(TEXT("DECREASE"), ESearchCase::IgnoreCase) == 0)	return AgitDungeonRaidTicketChange::DECREASE;
	if (str.Compare(TEXT("RESET"), ESearchCase::IgnoreCase) == 0)	return AgitDungeonRaidTicketChange::RESET;
	return AgitDungeonRaidTicketChange::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString GuildDailyActivityTypeEnumToString(GuildDailyActivityType value)
{
	switch (value)
	{
		case GuildDailyActivityType::ATTENDANCE: return TEXT("ATTENDANCE");
		case GuildDailyActivityType::NORMAL_DONATE: return TEXT("NORMAL_DONATE");
		case GuildDailyActivityType::ADVANCED_DONATE: return TEXT("ADVANCED_DONATE");
	}
	return FString();
}
inline FString GuildMemberGradeTypeEnumToString(GuildMemberGradeType value)
{
	switch (value)
	{
		case GuildMemberGradeType::UNKNOWN: return TEXT("UNKNOWN");
		case GuildMemberGradeType::KNIGHT: return TEXT("KNIGHT");
		case GuildMemberGradeType::SECONDKNIGHT: return TEXT("SECONDKNIGHT");
		case GuildMemberGradeType::FIRSTKNIGHT: return TEXT("FIRSTKNIGHT");
		case GuildMemberGradeType::SUBCAPTAIN: return TEXT("SUBCAPTAIN");
		case GuildMemberGradeType::CAPTAIN: return TEXT("CAPTAIN");
	}
	return FString();
}
inline FString GuildJoinTypeEnumToString(GuildJoinType value)
{
	switch (value)
	{
		case GuildJoinType::IMMEDIATE: return TEXT("IMMEDIATE");
		case GuildJoinType::APPROVAL: return TEXT("APPROVAL");
		case GuildJoinType::UNABLE: return TEXT("UNABLE");
	}
	return FString();
}
inline FString GuildJoinStateEnumToString(GuildJoinState value)
{
	switch (value)
	{
		case GuildJoinState::ABLE_JOIN_STATE: return TEXT("ABLE_JOIN_STATE");
		case GuildJoinState::REQUESTED_STATE: return TEXT("REQUESTED_STATE");
		case GuildJoinState::JOINED_STATE: return TEXT("JOINED_STATE");
		case GuildJoinState::RESTRICTED_REJOIN: return TEXT("RESTRICTED_REJOIN");
	}
	return FString();
}
inline FString GuildShopPurchaseLimitTypeEnumToString(GuildShopPurchaseLimitType value)
{
	switch (value)
	{
		case GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_NONE: return TEXT("GUILDSHOP_PURCHASE_LIMIT_OF_NONE");
		case GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_DAY: return TEXT("GUILDSHOP_PURCHASE_LIMIT_OF_DAY");
		case GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_WEEK: return TEXT("GUILDSHOP_PURCHASE_LIMIT_OF_WEEK");
		case GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_MONTH: return TEXT("GUILDSHOP_PURCHASE_LIMIT_OF_MONTH");
	}
	return FString();
}
inline FString GuildStatTypeEnumToString(GuildStatType value)
{
	switch (value)
	{
		case GuildStatType::NONE: return TEXT("NONE");
		case GuildStatType::GUILD_LEVEL: return TEXT("GUILD_LEVEL");
		case GuildStatType::GUILD_USER_NUMBER: return TEXT("GUILD_USER_NUMBER");
		case GuildStatType::GUILD_BUFF: return TEXT("GUILD_BUFF");
		case GuildStatType::GUILD_PASSIVE_STAT: return TEXT("GUILD_PASSIVE_STAT");
		case GuildStatType::GUILD_CAMP: return TEXT("GUILD_CAMP");
		case GuildStatType::GUILD_DUNGEON_BUFF: return TEXT("GUILD_DUNGEON_BUFF");
	}
	return FString();
}
inline FString GuildBuildingNotifyTypeEnumToString(GuildBuildingNotifyType value)
{
	switch (value)
	{
		case GuildBuildingNotifyType::NONE: return TEXT("NONE");
		case GuildBuildingNotifyType::OPEN: return TEXT("OPEN");
		case GuildBuildingNotifyType::OPEN_CLOSE: return TEXT("OPEN_CLOSE");
		case GuildBuildingNotifyType::UPGRADE_START: return TEXT("UPGRADE_START");
		case GuildBuildingNotifyType::UPGRADE_END: return TEXT("UPGRADE_END");
		case GuildBuildingNotifyType::PASSIVITY_START: return TEXT("PASSIVITY_START");
		case GuildBuildingNotifyType::PASSIVITY_END: return TEXT("PASSIVITY_END");
		case GuildBuildingNotifyType::BUFF_REACTIVATE_TIME: return TEXT("BUFF_REACTIVATE_TIME");
		case GuildBuildingNotifyType::ENTER_CAMP: return TEXT("ENTER_CAMP");
		case GuildBuildingNotifyType::LEAVE_CAMP: return TEXT("LEAVE_CAMP");
	}
	return FString();
}
inline FString GuildBuildingStateTypeEnumToString(GuildBuildingStateType value)
{
	switch (value)
	{
		case GuildBuildingStateType::GUILD_BUILDING_STATE_NONE: return TEXT("GUILD_BUILDING_STATE_NONE");
		case GuildBuildingStateType::GUILD_BUILDING_STATE_OPEN: return TEXT("GUILD_BUILDING_STATE_OPEN");
		case GuildBuildingStateType::GUILD_BUILDING_STATE_UPGRADE: return TEXT("GUILD_BUILDING_STATE_UPGRADE");
	}
	return FString();
}
inline FString GuildWarStateEnumToString(GuildWarState value)
{
	switch (value)
	{
		case GuildWarState::NONE: return TEXT("NONE");
		case GuildWarState::REGISTER: return TEXT("REGISTER");
		case GuildWarState::READY: return TEXT("READY");
		case GuildWarState::PLAYWAIT: return TEXT("PLAYWAIT");
		case GuildWarState::PLAY: return TEXT("PLAY");
		case GuildWarState::END: return TEXT("END");
		case GuildWarState::DESTORY: return TEXT("DESTORY");
	}
	return FString();
}
inline FString GuildWarEndReasonEnumToString(GuildWarEndReason value)
{
	switch (value)
	{
		case GuildWarEndReason::KILL_BOSS: return TEXT("KILL_BOSS");
		case GuildWarEndReason::TIMEOUT: return TEXT("TIMEOUT");
		case GuildWarEndReason::TEAM_MEMBER_MINIMUM_CUT: return TEXT("TEAM_MEMBER_MINIMUM_CUT");
		case GuildWarEndReason::POINT: return TEXT("POINT");
	}
	return FString();
}
inline FString GuildAllyTypeEnumToString(GuildAllyType value)
{
	switch (value)
	{
		case GuildAllyType::GUILD_ALLY_NONE: return TEXT("GUILD_ALLY_NONE");
		case GuildAllyType::GUILD_ALLY_WAITING_FOR_REQUESTS: return TEXT("GUILD_ALLY_WAITING_FOR_REQUESTS");
		case GuildAllyType::GUILD_ALLY_WAITING_FOR_ACCEPTANCE: return TEXT("GUILD_ALLY_WAITING_FOR_ACCEPTANCE");
		case GuildAllyType::GUILD_ALLY_WAITING_CANCEL: return TEXT("GUILD_ALLY_WAITING_CANCEL");
		case GuildAllyType::GUILD_ALLY_TIME_OUT: return TEXT("GUILD_ALLY_TIME_OUT");
		case GuildAllyType::GUILD_ALLY_ACCEPT: return TEXT("GUILD_ALLY_ACCEPT");
		case GuildAllyType::GUILD_ALLY_BREAK: return TEXT("GUILD_ALLY_BREAK");
		case GuildAllyType::GUILD_ALLY_REQUESTS_FOR_INTER: return TEXT("GUILD_ALLY_REQUESTS_FOR_INTER");
		case GuildAllyType::GUILD_ALLY_BREAK_FOR_INTER: return TEXT("GUILD_ALLY_BREAK_FOR_INTER");
		case GuildAllyType::GUILD_ALLY_ACCEPT_FOR_INTER: return TEXT("GUILD_ALLY_ACCEPT_FOR_INTER");
		case GuildAllyType::GUILD_ALLY_REQUEST_FAIL_FOR_INTER: return TEXT("GUILD_ALLY_REQUEST_FAIL_FOR_INTER");
		case GuildAllyType::GUILD_ALLY_ACCEPT_FAIL_FOR_INTER: return TEXT("GUILD_ALLY_ACCEPT_FAIL_FOR_INTER");
	}
	return FString();
}
inline FString GuildItemCollectionTypeEnumToString(GuildItemCollectionType value)
{
	switch (value)
	{
		case GuildItemCollectionType::NONE: return TEXT("NONE");
		case GuildItemCollectionType::EVENT_SHARING: return TEXT("EVENT_SHARING");
		case GuildItemCollectionType::SHARING: return TEXT("SHARING");
		case GuildItemCollectionType::RANDOM: return TEXT("RANDOM");
	}
	return FString();
}
inline FString GuildItemAcquireTypeEnumToString(GuildItemAcquireType value)
{
	switch (value)
	{
		case GuildItemAcquireType::NONE: return TEXT("NONE");
		case GuildItemAcquireType::DUNGEON: return TEXT("DUNGEON");
		case GuildItemAcquireType::FIELD_BOSS: return TEXT("FIELD_BOSS");
		case GuildItemAcquireType::AGIT_DUNGEON_RAID: return TEXT("AGIT_DUNGEON_RAID");
	}
	return FString();
}
inline FString GuildCampStateTypeEnumToString(GuildCampStateType value)
{
	switch (value)
	{
		case GuildCampStateType::NONE: return TEXT("NONE");
		case GuildCampStateType::READY: return TEXT("READY");
		case GuildCampStateType::COMPLETE: return TEXT("COMPLETE");
		case GuildCampStateType::OCCUPIED: return TEXT("OCCUPIED");
	}
	return FString();
}
inline FString GuildCampDestructionTypeEnumToString(GuildCampDestructionType value)
{
	switch (value)
	{
		case GuildCampDestructionType::NONE: return TEXT("NONE");
		case GuildCampDestructionType::UserDestruction: return TEXT("UserDestruction");
		case GuildCampDestructionType::ExpiredTime: return TEXT("ExpiredTime");
	}
	return FString();
}
inline FString GuildDivisionStateEnumToString(GuildDivisionState value)
{
	switch (value)
	{
		case GuildDivisionState::NONE: return TEXT("NONE");
		case GuildDivisionState::STORAGE: return TEXT("STORAGE");
		case GuildDivisionState::AUCTION: return TEXT("AUCTION");
		case GuildDivisionState::DONE: return TEXT("DONE");
		case GuildDivisionState::FAILED: return TEXT("FAILED");
		case GuildDivisionState::SEND_MAIL: return TEXT("SEND_MAIL");
	}
	return FString();
}
inline FString GuildAuctionResultTypeEnumToString(GuildAuctionResultType value)
{
	switch (value)
	{
		case GuildAuctionResultType::NORMAL: return TEXT("NORMAL");
		case GuildAuctionResultType::PARTICIPANTS: return TEXT("PARTICIPANTS");
	}
	return FString();
}
inline FString GuildAuctionTypeEnumToString(GuildAuctionType value)
{
	switch (value)
	{
		case GuildAuctionType::NONE: return TEXT("NONE");
		case GuildAuctionType::NORMAL: return TEXT("NORMAL");
		case GuildAuctionType::APPOINTMENT: return TEXT("APPOINTMENT");
		case GuildAuctionType::PARTICIPANTS: return TEXT("PARTICIPANTS");
		case GuildAuctionType::DIRECT_DISTRIBUTION: return TEXT("DIRECT_DISTRIBUTION");
	}
	return FString();
}
inline FString GuildCampNotifyTypeEnumToString(GuildCampNotifyType value)
{
	switch (value)
	{
		case GuildCampNotifyType::NONE: return TEXT("NONE");
		case GuildCampNotifyType::UNDER_ATTACK: return TEXT("UNDER_ATTACK");
		case GuildCampNotifyType::Destruction: return TEXT("Destruction");
		case GuildCampNotifyType::Dismantle: return TEXT("Dismantle");
	}
	return FString();
}
inline FString GuildStoreTabTypeEnumToString(GuildStoreTabType value)
{
	switch (value)
	{
		case GuildStoreTabType::NONE: return TEXT("NONE");
		case GuildStoreTabType::DISTRIBUTION: return TEXT("DISTRIBUTION");
		case GuildStoreTabType::AUCTION: return TEXT("AUCTION");
		case GuildStoreTabType::CALCULATE: return TEXT("CALCULATE");
	}
	return FString();
}
inline FString GuildStoreItemNotifyTypeEnumToString(GuildStoreItemNotifyType value)
{
	switch (value)
	{
		case GuildStoreItemNotifyType::NONE: return TEXT("NONE");
		case GuildStoreItemNotifyType::REGISTER: return TEXT("REGISTER");
		case GuildStoreItemNotifyType::NORMAL: return TEXT("NORMAL");
		case GuildStoreItemNotifyType::DONE: return TEXT("DONE");
		case GuildStoreItemNotifyType::ACQUISITION: return TEXT("ACQUISITION");
		case GuildStoreItemNotifyType::ACQUISITION_SETTING: return TEXT("ACQUISITION_SETTING");
		case GuildStoreItemNotifyType::STORAGE_TIME_AUTO_AUCTION: return TEXT("STORAGE_TIME_AUTO_AUCTION");
		case GuildStoreItemNotifyType::ACQUISITION_AUTO_AUCTION: return TEXT("ACQUISITION_AUTO_AUCTION");
		case GuildStoreItemNotifyType::FAILED: return TEXT("FAILED");
		case GuildStoreItemNotifyType::CREATE_GUILD: return TEXT("CREATE_GUILD");
		case GuildStoreItemNotifyType::DIRECT: return TEXT("DIRECT");
	}
	return FString();
}
inline FString GuildStoreAvailableResultTypeEnumToString(GuildStoreAvailableResultType value)
{
	switch (value)
	{
		case GuildStoreAvailableResultType::NONE: return TEXT("NONE");
		case GuildStoreAvailableResultType::STANDBY: return TEXT("STANDBY");
		case GuildStoreAvailableResultType::AUTHORITY: return TEXT("AUTHORITY");
		case GuildStoreAvailableResultType::APPOINTMENT: return TEXT("APPOINTMENT");
		case GuildStoreAvailableResultType::ACQUISITION: return TEXT("ACQUISITION");
	}
	return FString();
}
inline FString AgitDungeonRaidStateEnumToString(AgitDungeonRaidState value)
{
	switch (value)
	{
		case AgitDungeonRaidState::NONE: return TEXT("NONE");
		case AgitDungeonRaidState::OPEN: return TEXT("OPEN");
		case AgitDungeonRaidState::CLOSED: return TEXT("CLOSED");
		case AgitDungeonRaidState::CLEAR: return TEXT("CLEAR");
	}
	return FString();
}
inline FString AgitDungeonRaidTicketChangeEnumToString(AgitDungeonRaidTicketChange value)
{
	switch (value)
	{
		case AgitDungeonRaidTicketChange::NONE: return TEXT("NONE");
		case AgitDungeonRaidTicketChange::INCREASE: return TEXT("INCREASE");
		case AgitDungeonRaidTicketChange::DECREASE: return TEXT("DECREASE");
		case AgitDungeonRaidTicketChange::RESET: return TEXT("RESET");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString GuildDailyActivityTypeEnumToDisplayKey(GuildDailyActivityType value)
{
	switch (value)
	{
		case GuildDailyActivityType::ATTENDANCE: return TEXT("");
		case GuildDailyActivityType::NORMAL_DONATE: return TEXT("");
		case GuildDailyActivityType::ADVANCED_DONATE: return TEXT("");
	}
	return FString();
}
inline FString GuildMemberGradeTypeEnumToDisplayKey(GuildMemberGradeType value)
{
	switch (value)
	{
		case GuildMemberGradeType::UNKNOWN: return TEXT("");
		case GuildMemberGradeType::KNIGHT: return TEXT("");
		case GuildMemberGradeType::SECONDKNIGHT: return TEXT("");
		case GuildMemberGradeType::FIRSTKNIGHT: return TEXT("");
		case GuildMemberGradeType::SUBCAPTAIN: return TEXT("");
		case GuildMemberGradeType::CAPTAIN: return TEXT("");
	}
	return FString();
}
inline FString GuildJoinTypeEnumToDisplayKey(GuildJoinType value)
{
	switch (value)
	{
		case GuildJoinType::IMMEDIATE: return TEXT("");
		case GuildJoinType::APPROVAL: return TEXT("");
		case GuildJoinType::UNABLE: return TEXT("");
	}
	return FString();
}
inline FString GuildJoinStateEnumToDisplayKey(GuildJoinState value)
{
	switch (value)
	{
		case GuildJoinState::ABLE_JOIN_STATE: return TEXT("");
		case GuildJoinState::REQUESTED_STATE: return TEXT("");
		case GuildJoinState::JOINED_STATE: return TEXT("");
		case GuildJoinState::RESTRICTED_REJOIN: return TEXT("");
	}
	return FString();
}
inline FString GuildShopPurchaseLimitTypeEnumToDisplayKey(GuildShopPurchaseLimitType value)
{
	switch (value)
	{
		case GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_NONE: return TEXT("");
		case GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_DAY: return TEXT("");
		case GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_WEEK: return TEXT("");
		case GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_MONTH: return TEXT("");
	}
	return FString();
}
inline FString GuildStatTypeEnumToDisplayKey(GuildStatType value)
{
	switch (value)
	{
		case GuildStatType::NONE: return TEXT("");
		case GuildStatType::GUILD_LEVEL: return TEXT("");
		case GuildStatType::GUILD_USER_NUMBER: return TEXT("");
		case GuildStatType::GUILD_BUFF: return TEXT("");
		case GuildStatType::GUILD_PASSIVE_STAT: return TEXT("");
		case GuildStatType::GUILD_CAMP: return TEXT("");
		case GuildStatType::GUILD_DUNGEON_BUFF: return TEXT("");
	}
	return FString();
}
inline FString GuildBuildingNotifyTypeEnumToDisplayKey(GuildBuildingNotifyType value)
{
	switch (value)
	{
		case GuildBuildingNotifyType::NONE: return TEXT("");
		case GuildBuildingNotifyType::OPEN: return TEXT("");
		case GuildBuildingNotifyType::OPEN_CLOSE: return TEXT("");
		case GuildBuildingNotifyType::UPGRADE_START: return TEXT("");
		case GuildBuildingNotifyType::UPGRADE_END: return TEXT("");
		case GuildBuildingNotifyType::PASSIVITY_START: return TEXT("");
		case GuildBuildingNotifyType::PASSIVITY_END: return TEXT("");
		case GuildBuildingNotifyType::BUFF_REACTIVATE_TIME: return TEXT("");
		case GuildBuildingNotifyType::ENTER_CAMP: return TEXT("");
		case GuildBuildingNotifyType::LEAVE_CAMP: return TEXT("");
	}
	return FString();
}
inline FString GuildBuildingStateTypeEnumToDisplayKey(GuildBuildingStateType value)
{
	switch (value)
	{
		case GuildBuildingStateType::GUILD_BUILDING_STATE_NONE: return TEXT("");
		case GuildBuildingStateType::GUILD_BUILDING_STATE_OPEN: return TEXT("");
		case GuildBuildingStateType::GUILD_BUILDING_STATE_UPGRADE: return TEXT("");
	}
	return FString();
}
inline FString GuildWarStateEnumToDisplayKey(GuildWarState value)
{
	switch (value)
	{
		case GuildWarState::NONE: return TEXT("");
		case GuildWarState::REGISTER: return TEXT("");
		case GuildWarState::READY: return TEXT("");
		case GuildWarState::PLAYWAIT: return TEXT("");
		case GuildWarState::PLAY: return TEXT("");
		case GuildWarState::END: return TEXT("");
		case GuildWarState::DESTORY: return TEXT("");
	}
	return FString();
}
inline FString GuildWarEndReasonEnumToDisplayKey(GuildWarEndReason value)
{
	switch (value)
	{
		case GuildWarEndReason::KILL_BOSS: return TEXT("");
		case GuildWarEndReason::TIMEOUT: return TEXT("");
		case GuildWarEndReason::TEAM_MEMBER_MINIMUM_CUT: return TEXT("");
		case GuildWarEndReason::POINT: return TEXT("");
	}
	return FString();
}
inline FString GuildAllyTypeEnumToDisplayKey(GuildAllyType value)
{
	switch (value)
	{
		case GuildAllyType::GUILD_ALLY_NONE: return TEXT("");
		case GuildAllyType::GUILD_ALLY_WAITING_FOR_REQUESTS: return TEXT("");
		case GuildAllyType::GUILD_ALLY_WAITING_FOR_ACCEPTANCE: return TEXT("");
		case GuildAllyType::GUILD_ALLY_WAITING_CANCEL: return TEXT("");
		case GuildAllyType::GUILD_ALLY_TIME_OUT: return TEXT("");
		case GuildAllyType::GUILD_ALLY_ACCEPT: return TEXT("");
		case GuildAllyType::GUILD_ALLY_BREAK: return TEXT("");
		case GuildAllyType::GUILD_ALLY_REQUESTS_FOR_INTER: return TEXT("");
		case GuildAllyType::GUILD_ALLY_BREAK_FOR_INTER: return TEXT("");
		case GuildAllyType::GUILD_ALLY_ACCEPT_FOR_INTER: return TEXT("");
		case GuildAllyType::GUILD_ALLY_REQUEST_FAIL_FOR_INTER: return TEXT("");
		case GuildAllyType::GUILD_ALLY_ACCEPT_FAIL_FOR_INTER: return TEXT("");
	}
	return FString();
}
inline FString GuildItemCollectionTypeEnumToDisplayKey(GuildItemCollectionType value)
{
	switch (value)
	{
		case GuildItemCollectionType::NONE: return TEXT("");
		case GuildItemCollectionType::EVENT_SHARING: return TEXT("");
		case GuildItemCollectionType::SHARING: return TEXT("");
		case GuildItemCollectionType::RANDOM: return TEXT("");
	}
	return FString();
}
inline FString GuildItemAcquireTypeEnumToDisplayKey(GuildItemAcquireType value)
{
	switch (value)
	{
		case GuildItemAcquireType::NONE: return TEXT("");
		case GuildItemAcquireType::DUNGEON: return TEXT("");
		case GuildItemAcquireType::FIELD_BOSS: return TEXT("");
		case GuildItemAcquireType::AGIT_DUNGEON_RAID: return TEXT("");
	}
	return FString();
}
inline FString GuildCampStateTypeEnumToDisplayKey(GuildCampStateType value)
{
	switch (value)
	{
		case GuildCampStateType::NONE: return TEXT("");
		case GuildCampStateType::READY: return TEXT("");
		case GuildCampStateType::COMPLETE: return TEXT("");
		case GuildCampStateType::OCCUPIED: return TEXT("");
	}
	return FString();
}
inline FString GuildCampDestructionTypeEnumToDisplayKey(GuildCampDestructionType value)
{
	switch (value)
	{
		case GuildCampDestructionType::NONE: return TEXT("");
		case GuildCampDestructionType::UserDestruction: return TEXT("");
		case GuildCampDestructionType::ExpiredTime: return TEXT("");
	}
	return FString();
}
inline FString GuildDivisionStateEnumToDisplayKey(GuildDivisionState value)
{
	switch (value)
	{
		case GuildDivisionState::NONE: return TEXT("");
		case GuildDivisionState::STORAGE: return TEXT("");
		case GuildDivisionState::AUCTION: return TEXT("");
		case GuildDivisionState::DONE: return TEXT("");
		case GuildDivisionState::FAILED: return TEXT("");
		case GuildDivisionState::SEND_MAIL: return TEXT("");
	}
	return FString();
}
inline FString GuildAuctionResultTypeEnumToDisplayKey(GuildAuctionResultType value)
{
	switch (value)
	{
		case GuildAuctionResultType::NORMAL: return TEXT("");
		case GuildAuctionResultType::PARTICIPANTS: return TEXT("");
	}
	return FString();
}
inline FString GuildAuctionTypeEnumToDisplayKey(GuildAuctionType value)
{
	switch (value)
	{
		case GuildAuctionType::NONE: return TEXT("");
		case GuildAuctionType::NORMAL: return TEXT("");
		case GuildAuctionType::APPOINTMENT: return TEXT("");
		case GuildAuctionType::PARTICIPANTS: return TEXT("");
		case GuildAuctionType::DIRECT_DISTRIBUTION: return TEXT("");
	}
	return FString();
}
inline FString GuildCampNotifyTypeEnumToDisplayKey(GuildCampNotifyType value)
{
	switch (value)
	{
		case GuildCampNotifyType::NONE: return TEXT("");
		case GuildCampNotifyType::UNDER_ATTACK: return TEXT("");
		case GuildCampNotifyType::Destruction: return TEXT("");
		case GuildCampNotifyType::Dismantle: return TEXT("");
	}
	return FString();
}
inline FString GuildStoreTabTypeEnumToDisplayKey(GuildStoreTabType value)
{
	switch (value)
	{
		case GuildStoreTabType::NONE: return TEXT("");
		case GuildStoreTabType::DISTRIBUTION: return TEXT("");
		case GuildStoreTabType::AUCTION: return TEXT("");
		case GuildStoreTabType::CALCULATE: return TEXT("");
	}
	return FString();
}
inline FString GuildStoreItemNotifyTypeEnumToDisplayKey(GuildStoreItemNotifyType value)
{
	switch (value)
	{
		case GuildStoreItemNotifyType::NONE: return TEXT("");
		case GuildStoreItemNotifyType::REGISTER: return TEXT("");
		case GuildStoreItemNotifyType::NORMAL: return TEXT("");
		case GuildStoreItemNotifyType::DONE: return TEXT("");
		case GuildStoreItemNotifyType::ACQUISITION: return TEXT("");
		case GuildStoreItemNotifyType::ACQUISITION_SETTING: return TEXT("");
		case GuildStoreItemNotifyType::STORAGE_TIME_AUTO_AUCTION: return TEXT("");
		case GuildStoreItemNotifyType::ACQUISITION_AUTO_AUCTION: return TEXT("");
		case GuildStoreItemNotifyType::FAILED: return TEXT("");
		case GuildStoreItemNotifyType::CREATE_GUILD: return TEXT("");
		case GuildStoreItemNotifyType::DIRECT: return TEXT("");
	}
	return FString();
}
inline FString GuildStoreAvailableResultTypeEnumToDisplayKey(GuildStoreAvailableResultType value)
{
	switch (value)
	{
		case GuildStoreAvailableResultType::NONE: return TEXT("");
		case GuildStoreAvailableResultType::STANDBY: return TEXT("");
		case GuildStoreAvailableResultType::AUTHORITY: return TEXT("");
		case GuildStoreAvailableResultType::APPOINTMENT: return TEXT("");
		case GuildStoreAvailableResultType::ACQUISITION: return TEXT("");
	}
	return FString();
}
inline FString AgitDungeonRaidStateEnumToDisplayKey(AgitDungeonRaidState value)
{
	switch (value)
	{
		case AgitDungeonRaidState::NONE: return TEXT("");
		case AgitDungeonRaidState::OPEN: return TEXT("");
		case AgitDungeonRaidState::CLOSED: return TEXT("");
		case AgitDungeonRaidState::CLEAR: return TEXT("");
	}
	return FString();
}
inline FString AgitDungeonRaidTicketChangeEnumToDisplayKey(AgitDungeonRaidTicketChange value)
{
	switch (value)
	{
		case AgitDungeonRaidTicketChange::NONE: return TEXT("");
		case AgitDungeonRaidTicketChange::INCREASE: return TEXT("");
		case AgitDungeonRaidTicketChange::DECREASE: return TEXT("");
		case AgitDungeonRaidTicketChange::RESET: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidGuildDailyActivityType(GuildDailyActivityType value) noexcept
{
	return (value >= GuildDailyActivityType::ATTENDANCE && value < GuildDailyActivityType::MAX);
}

inline bool IsValidGuildMemberGradeType(GuildMemberGradeType value) noexcept
{
	return (value >= GuildMemberGradeType::UNKNOWN && value < GuildMemberGradeType::MAX);
}

inline bool IsValidGuildJoinType(GuildJoinType value) noexcept
{
	return (value >= GuildJoinType::IMMEDIATE && value < GuildJoinType::MAX);
}

inline bool IsValidGuildJoinState(GuildJoinState value) noexcept
{
	return (value >= GuildJoinState::ABLE_JOIN_STATE && value < GuildJoinState::MAX);
}

inline bool IsValidGuildShopPurchaseLimitType(GuildShopPurchaseLimitType value) noexcept
{
	return (value >= GuildShopPurchaseLimitType::GUILDSHOP_PURCHASE_LIMIT_OF_NONE && value < GuildShopPurchaseLimitType::MAX);
}

inline bool IsValidGuildStatType(GuildStatType value) noexcept
{
	return (value >= GuildStatType::NONE && value < GuildStatType::MAX);
}

inline bool IsValidGuildBuildingNotifyType(GuildBuildingNotifyType value) noexcept
{
	return (value >= GuildBuildingNotifyType::NONE && value < GuildBuildingNotifyType::MAX);
}

inline bool IsValidGuildBuildingStateType(GuildBuildingStateType value) noexcept
{
	return (value >= GuildBuildingStateType::GUILD_BUILDING_STATE_NONE && value < GuildBuildingStateType::MAX);
}

inline bool IsValidGuildWarState(GuildWarState value) noexcept
{
	return (value >= GuildWarState::NONE && value < GuildWarState::MAX);
}

inline bool IsValidGuildWarEndReason(GuildWarEndReason value) noexcept
{
	return (value >= GuildWarEndReason::KILL_BOSS && value < GuildWarEndReason::MAX);
}

inline bool IsValidGuildAllyType(GuildAllyType value) noexcept
{
	return (value >= GuildAllyType::GUILD_ALLY_NONE && value < GuildAllyType::MAX);
}

inline bool IsValidGuildItemCollectionType(GuildItemCollectionType value) noexcept
{
	return (value >= GuildItemCollectionType::NONE && value < GuildItemCollectionType::MAX);
}

inline bool IsValidGuildItemAcquireType(GuildItemAcquireType value) noexcept
{
	return (value >= GuildItemAcquireType::NONE && value < GuildItemAcquireType::MAX);
}

inline bool IsValidGuildCampStateType(GuildCampStateType value) noexcept
{
	return (value >= GuildCampStateType::NONE && value < GuildCampStateType::MAX);
}

inline bool IsValidGuildCampDestructionType(GuildCampDestructionType value) noexcept
{
	return (value >= GuildCampDestructionType::NONE && value < GuildCampDestructionType::MAX);
}

inline bool IsValidGuildDivisionState(GuildDivisionState value) noexcept
{
	return (value >= GuildDivisionState::NONE && value < GuildDivisionState::MAX);
}

inline bool IsValidGuildAuctionResultType(GuildAuctionResultType value) noexcept
{
	return (value >= GuildAuctionResultType::NORMAL && value < GuildAuctionResultType::MAX);
}

inline bool IsValidGuildAuctionType(GuildAuctionType value) noexcept
{
	return (value >= GuildAuctionType::NONE && value < GuildAuctionType::MAX);
}

inline bool IsValidGuildCampNotifyType(GuildCampNotifyType value) noexcept
{
	return (value >= GuildCampNotifyType::NONE && value < GuildCampNotifyType::MAX);
}

inline bool IsValidGuildStoreTabType(GuildStoreTabType value) noexcept
{
	return (value >= GuildStoreTabType::NONE && value < GuildStoreTabType::MAX);
}

inline bool IsValidGuildStoreItemNotifyType(GuildStoreItemNotifyType value) noexcept
{
	return (value >= GuildStoreItemNotifyType::NONE && value < GuildStoreItemNotifyType::MAX);
}

inline bool IsValidGuildStoreAvailableResultType(GuildStoreAvailableResultType value) noexcept
{
	return (value >= GuildStoreAvailableResultType::NONE && value < GuildStoreAvailableResultType::MAX);
}

inline bool IsValidAgitDungeonRaidState(AgitDungeonRaidState value) noexcept
{
	return (value >= AgitDungeonRaidState::NONE && value < AgitDungeonRaidState::MAX);
}

inline bool IsValidAgitDungeonRaidTicketChange(AgitDungeonRaidTicketChange value) noexcept
{
	return (value >= AgitDungeonRaidTicketChange::NONE && value < AgitDungeonRaidTicketChange::MAX);
}

