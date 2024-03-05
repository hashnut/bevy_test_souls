#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedCommonEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
// 계정 등급
enum class AccountGrade : uint8
{
	// 일반 유저 등급

	// 일반 유저
	NORMAL = 0,

	// 데이터 조작 권한 없는 점검 운영자
	OPERATOR = 1,

	// 데이터 조작 권한 없는 QA용
	QATESTER = 2,

	// 아래 등급 이상으로는 운영자 권한(치트 사용 가능)

	// 모든 권한 가진 운영자 (개발자/QA)
	ADMIN = 3,

	// GM 캐릭터 운영자
	GM = 4,

	MAX = 5,
};

UENUM()
// 서버 혼잡상태
enum class ServerCongestion : uint8
{

	// 서버 준비중
	ON_STANBY = 0,

	// 원활
	GOOD = 1,

	// 혼잡 (MAX의 60% 이상)
	CONGESTED = 2,

	// 꽉참
	FULL = 3,

	// 대기열조차 다 차서 입장을 할수 없는 상태
	RESTRICTED_ACCESS = 4,

	// 임시잠금
	BLOCK = 5,

	// 서버 다운이나 점검 등으로 접속 불가
	NOT_CONNECTABLE = 6,

	MAX = 7,
};

UENUM(BlueprintType)
// 맵 타입
enum class MapType : uint8
{
	NONE = 0,

	//  
 	/* 참고 - 추후 전장 타입등이 컨텐츠에 따라 필요하면 자유롭게 추가할 것 
	 속성 1. 유저 (맵, 위치) 디비 저장여부 
	 속성 2. 파티 워프 가능한지 
	 속성 3. EventAction 가능한지 
	 속성 4. Quest(Volatile - 휘발성으로 수락 할때부터 저장 안할지, Quest 초기화 -재연결, 부활 등 인스턴스맵에서 나가질 때) 
	 속성 5. 인스턴스에서 나가기버튼(퀘스트 초기화됨, 시공의틈새 클라 포탈 연출 없음-WarpReason변경)  
	*/

	// Public

	// 일반 필드
	PUBLIC_FIELD = 1,

	// Single

	// 싱글 필드 - 캐릭터 생성시 첫번째 맵에서만 사용하기로 함 - 인스턴스 성격이 아닌, 혼자만 들어가는 채널
	SINGLE_FIELD = 2,

	// Instance

	// 인스턴스 싱글 - 개인 프라이빗 이벤트 채널 or 개인 이벤트 공간
	INSTANCE_SINGLE = 3,

	// 인스턴스 필드 - 이벤트 필드, 멀티 던전 등
	INSTANCE_FIELD = 4,

	// Dungeon

	// 퍼브릭 던전(용어를 바꾸고 싶지만, 타팀 협업때문에 용어만 퍼블릭필드) - 퍼블릭이지만, 인스턴스(유저 현재맵정보를 DB저장을 안한다)로 동작한다.
	PUBLIC_DUNGEON = 5,

	// 기사단 던전 전용 map , 보스 몬스터만 존재한다 
	GUILD_DUNGEON = 6,

	// 파티 던전 
	PARTY_DUNGEON = 7,

	// 고레벨필드던전1
	ELITE_DUNGEON_1 = 8,

	// 월드보스 맵
	WORLD_BOSS = 9,

	// 인터서버 필드
	INTER_FIELD = 10,

	// 길드전
	GUILD_WAR = 11,

	// 기사단 영지 던전 전용 map , 보스 몬스터만 존재한다 
	AGIT_DUNGEON_RAID = 12,

	// 기사단 영지
	AGIT = 13,

	MAX = 14,
};

UENUM(BlueprintType)
// 맵 컨텐츠
enum class MapContentsType : uint8
{

	// 유저간 전투가 안되는 필드
	NORMAL = 0,

	// 유저간 전투가 가능한 필드
	BATTLE = 1,

	// 유저간 전투가 되더라도 PK성향치 감소 없음
	OUTLAW = 2,

	// 시공의틈새
	SPACE_CRACK = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// Warp 기능 작동 방식을 설정합니다
enum class WarpType : uint8
{

	// 별도의 안내 없이 Volume 진입 시 바로 이동 처리
	Immediately = 0,

	// 이동 전 다음과 같은 확인 창을 띄운 후 플레이어 선택에 따라 이동 여부 판단
	YesNoDialog = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 영역 속성 타입
enum class TerritoryType : uint8
{
	NONE = 0,

	// 마을
	TOWN = 1,

	// 안전구역
	SAFE_ZONE = 2,

	// 결투장 준비구역
	ARENA_READY = 3,

	// 결투장
	ARENA = 4,

	// 특정지역도착(파티던전)
	ARRIVAL = 5,

	// 환경 변경(클라전용)
	Env = 6,

	// 퀘스트 사운드(클라전용)
	SOUND = 7,

	// 시공의 틈새
	SPACE_CRACK = 8,

	// 침공전이 아닐때는 타운 침공전 시작시에는 무법지대
	TOWN_CHAOS = 9,

	// 캠프 배치 불가 지역
	NO_CAMP_ZONE = 10,

	// PK 불가 지역
	NON_PK = 11,

	MAX = 12,
};

UENUM(BlueprintType)
// 영역 체크 상황 타입
enum class TerritoryCheckType : uint8
{
	NONE = 0,

	// 침공전
	INVADE = 1,

	MAX = 2,
};

UENUM()
// 채널 혼잡도
enum class ChannelCongestionStatus : uint8
{

	// 부족
	LOW = 0,

	// 원활
	MEDIUM = 1,

	// 혼잡
	HIGH = 2,

	// 포화
	FULL = 3,

	MAX = 4,
};

UENUM()
// Warp 이유
enum class WarpReason : uint8
{

	// 일반적인 월드 진입
	ENTER_WORLD = 0,

	// 채널 변경 - 쿨타임 체크
	CHANGE_CHANNEL = 1,

	// 포탈
	PORTAL = 2,

	// 파티 매칭으로 인한 던전 입장 등
	PARTY_MATCHING = 3,

	// 부활
	RESURRECT = 4,

	// 운영자 명령어
	ADMIN_COMMAND = 5,

	// Quest Warp(UI버튼)) - 골드소모
	QUEST = 6,

	// EVENT_ACTION 으로 개인맵으로 들어가기 - Warp 완료 후 Quest 완료처리
	EVENT_ACTION_TO_INSTANCE_SINGLE = 7,

	// 개인맵에서 나가기(UI버튼)
	LEAVE_INSTANCE_SINGLE = 8,

	// EVENT_ACTION 으로 공용맵으로 나가기 - Warp 완료 후 Quest 완료처리
	EVENT_ACTION_RETURN_TO_PUBLIC = 9,

	// EVENT_ACTION 으로 동일한 맵 내의 위치이동 - Warp 완료 후 EventAction 완료처리
	EVENT_ACTION_TELEPORT = 10,

	// 위치 즐겨찾기 텔레포트
	BOOKMARK = 11,

	// 마을 귀환 주문서 사용
	USE_WARP_TO_TOWN_SCROLL = 12,

	// 랜덤 순간이동 주문서 사용
	USE_WARP_TO_RANDOM_SCROLL = 13,

	// 던전 시간 종료
	DUNGEON_TIME_OVER = 14,

	// 맵 제어 상태 툴 에서 이동
	MAP_EVENT = 15,

	// 공유된 위치 즐겨찾기 텔레포트
	BOOKMARK_SHARED = 16,

	// 파티장 채널로 이동
	PARTY_LEADER = 17,

	// 시공의 틈새 이동
	SPACE_CRACK = 18,

	// 월드맵을 통해 워프
	WORLD_MAP_PORTAL = 19,

	// 이벤트 던전 운용시간이 종료
	EVENT_DUNGEON_TIME_OVER = 20,

	// 기사단 던전 입장
	ENTER_GUILD_DUNGEON = 21,

	// 기사단 던전 나가기 버튼
	LEAVE_GUILD_DUNGEON = 22,

	// 기사단 캠프 위치로 이동
	GUILD_CAMP = 23,

	// 침공 이동
	INVADE = 24,

	// 침공 나가기
	INVADE_LEAVE = 25,

	// 침공 시간 종료
	INVADE_TIME_OVER = 26,

	// 유저 침공제한시간초과
	INVADE_USER_TIME_OVER = 27,

	// 파티던전 이동
	PARTY_DUNGEON = 28,

	// 파티던전 나가기
	PARTY_DUNGEON_LEAVE = 29,

	// 파티던전 시간 종료
	PARTY_DUNGEON_TIME_OVER = 30,

	// 고레벨필드던전 이동
	ELITE_DUNGEON = 31,

	// 고레벨필드던전 나가기 
	ELITE_DUNGEON_LEAVE = 32,

	// 고레벨필드던전 타임아웃 
	ELITE_DUNGEON_TIME_OVER = 33,

	// 결투장 이동
	ARENA = 34,

	// 결투장 나가기
	ARENA_LEAVE = 35,

	// 보물 몬스터 아이템 워프
	ITEM_TREASURE_MONSTER = 36,

	// 복수하기 워프
	REVENGE = 37,

	// 침공 귀환 주문서 사용
	INVADE_USE_SCROLL_LEAVE = 38,

	// 월드 보스 던전 이동
	WORLD_BOSS = 39,

	// 월드 보스 던전 시간종료
	WORLD_BOSS_TIME_OUT = 40,

	// 월드 보스 던전 나가기
	WORLD_BOSS_LEAVE = 41,

	// 채낼이 파괴가 된 상태 공용맵으로 워프시키자
	DESTROYED_THEN_RETURN_PUBLIC_FIELD = 42,

	// 월드맵을 통해 보스 위치로 워프
	WORLD_MAP_PORTAL_TO_BOSS = 43,

	// 고레벨필드던전 히든공간으로 이동
	ELITE_DUNGEON_HIDDEN = 44,

	// 던전 티켓을 통해 이동
	USE_DUNGEON_TICKET = 45,

	// 길드전 이동
	GUILD_WAR = 46,

	// 길드전 시간 종료
	GUILD_WAR_TIME_OUT = 47,

	// 길드전 나가기
	GUILD_WAR_LEAVE = 48,

	// 스킬 효과에 의한 이동
	SKILL_NOTIFY = 49,

	// 기사단 영지 던전 들어가기
	ENTER_AGIT_DUNGEON_RAID = 50,

	// 기사단 영지 던전 나가기 버튼 누름
	LEAVE_SELF_AGIT_DUNGEON_RAID = 51,

	// 기사단 영지 던전 들어가기
	ENTER_AGIT = 52,

	// 기사단 영지 던전 나가기 버튼
	LEAVE_AGIT = 53,

	// 기사단 영지 던전 랜덤 스크롤로 나가기
	LEAVE_AGIT_RANDOM_SCROLL = 54,

	// 기사단 영지 던전 종료 버튼 누름
	LEAVE_FORCE_AGIT_DUNGEON_RAID = 55,

	INVALID = 56,
};

UENUM()
// 커맨드 타입
enum class CommandType : uint8
{

	// 운영용 커맨드
	COMMAND_OPERATING = 0,

	// QA용 커맨드 : 일종의 CHEAT
	COMMAND_QA = 1,
};

UENUM()
// 커맨드 핸들러 타입
enum class CommandHandlerType : uint8
{

	// Gateway에서 실행되는 운영용 커맨드
	HANDLER_OPERATING_GATEWAY = 0,

	// World에서 실행되는 운영용 커맨드
	HANDLER_OPERATING_WORLD = 1,

	// Gateway에서 실행되는 QA용 커맨드
	HANDLER_QA_GATEWAY = 2,

	// World에서 실행되는 QA용 커맨드
	HANDLER_QA_WORLD = 3,

	// Gateway에서 실행되는 랭크 핸들러
	HANDLER_RANK_GATEWAY = 4,

	// Gateway에서 실행되는 하이브 아이템 핸들러
	HANDLER_HIVE_ITEM_GATEWAY = 5,

	// Auth에서 실행되는 운영용 커맨드
	HANDLER_OPERATING_AUTH = 6,

	// Directory에서 실행되는 운영용 커맨드
	HANDLER_OPERATING_DIRECTORY = 7,

	// Gateway에서 실행되는 Server To Server 핸들러
	HANDLER_SERVER_TO_SERVER_GATEWAY = 8,
};

UENUM(BlueprintType)
// 타임 티켓
enum class TimeTicketType : uint8
{

	// 경험치 무료 복구 비용
	TIME_TICKET_EXP_DEATH_PENALTY_FREE = 0,

	// 아이템 무료 복구 비용
	TIME_TICKET_ITEM_DEATH_PENALTY_FREE = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 재화 타입
enum class CurrencyType : uint8
{
	NONE = 0,

	// 계정 귀속 재화 타입

	// 다이아
	DIA = 1,

	// 무료 다이아(일본자금결제법대응)
	FREE_DIA = 2,

	// 마일리지 (다이아로 샀을경우 지급)
	MILEAGE = 3,

	// 골드
	GOLD = 4,

	// 캐쉬 IAP 지급(캐쉬로 구매했을경우지급 HardCurrency)
	CASH_MILEAGE = 5,

	// 도감 포인트
	MONSTER_BOOK_POINT = 6,

	// 유니크 페어리 소환 마일리지
	SUMMON_UNIQUE_FAIRY_MILEAGE = 7,

	// 유니크 코스튬 소환 마일리지
	SUMMON_UNIQUE_COSTUME_MILEAGE = 8,

	// 계정 귀속 재화 타입 최대 값
	MAX_ACCOUNT_TYPE = 9,

	// 유저 귀속 재화 타입
	START_USER_TYPE = 29,

	// 경험치 무료 복구 비용  (추후 Enum 변경할수도 있음)
	EXP_DEATH_PENALTY_FREE = 30,

	// 아이템 무료 복구 비용
	ITEM_DEATH_PENALTY_FREE = 31,

	// 복구 아이템 회복 관련 
	DEATH_EXP_RECOVERY = 33,

	// 페어리 소환 마일리지
	SUMMON_FAIRY_MILEAGE = 34,

	// 코스튬 소환 마일리지
	SUMMON_COSTUME_MILEAGE = 35,

	// 유저 귀속 재화 타입 최대 값
	MAX_USER_TYPE = 36,

	// 기타 재화 타입

	// 기사단 재화 타입

	// 기사단 운영비
	START_GUILD_TYPE = 99,

	// 기사단 운영비
	GUILD_ASSET = 100,

	// 기사단 운영비
	MAX_GUILD_TYPE = 101,

	// 게임 내 재화 타입 최대 값
	MAX_IN_GAME_TYPE = 102,

	// MAX_IN_GAME_TYPE 위에 정의해주세요!! 아래는BM상점에서 현금관련 상품을 위해 추가함(단순비교용)

	// 실제 현금(BM상점 단순비교용)
	CASH = 200,

	MAX = 201,
};

UENUM(BlueprintType)
// 보상 타입
enum class RewardType : uint8
{
	// 주 보상 타입

	// 전체 지급 (꾸러미의 모든 아이템을 확정 지급)
	ALL = 0,

	// 유저 선택 (유저가 선택한 한 가지를 확정 지급)
	PICK_ONE = 1,

	// 단일 랜덤 (확률에 따라 한 가지를 지급)
	RANDOM_ONE = 2,

	// 그룹 랜덤 (개별 확률 지급, 몬스터의 드랍에만 사용)
	RANDOM_GROUP = 3,

	// RewardBox별로 한개씩 랜덤으로 선택
	PICK_REWARD_BOX_LIST = 4,

	// 서브 보상 타입

	// 서브 보상 - 유저 선택 (유저가 선택한 한 가지를 확정 지급)
	SUB_PICK_ONE = 5,

	// 서브 보상 - 단일 랜덤 (확률에 따라 한 가지를 지급)
	SUB_RANDOM_ONE = 6,

	MAX = 7,
};

UENUM(BlueprintType)
// 무게 패널티 단계
enum class WeightPenaltyGrade : uint8
{

	// 무게 패널티 없음.
	NONE = 0,

	// 무게 패널티 1단계
	FIRST = 1,

	// 무게 패널티 2단계
	SECOND = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 파티 드랍아이템 소유권 유형
enum class PartyDropOwnershipType : uint8
{

	// 랜덤
	RANDOM = 0,

	// 순차
	SEQUENCE = 1,

	// 자유
	FREE = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 사망 패널티 복구 종류
enum class RestoreType : uint8
{

	// 무료
	Free = 0,

	// 재화
	Currency = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 시간 초기화 종류
enum class ResetTimeTicket : uint8
{
	RTT_NONE = 0,

	// 일일 리셋 (GC_NOTIFY_RESET_TIME_TICKET)
	RTT_DAILY = 1,

	// 주간 리셋 (GC_NOTIFY_RESET_TIME_TICKET)
	RTT_WEEKLY = 2,

	// 월간 리셋 (GC_NOTIFY_RESET_TIME_TICKET)
	RTT_MONTHLY = 3,

	// 던전 리셋 (GC_NOTIFY_RESET_TIME_TICKET_DUNGEON)
	RTT_DUNGEON = 4,

	// 주간 시공의 틈새  리셋 
	RTT_SPACEC_CRACK = 5,

	// 오프라인 시간 플레이 타임 리셋
	RTT_OFF_LINE_MODE = 6,

	// 사망 부활 리셋
	RTT_DEATH_RESURRECTION = 7,

	// 기사단 퀘스트 리셋
	RTT_GUILD_QUEST = 8,

	// 기사단 던전 리셋
	RTT_GUILD_DUNGEON = 9,

	// 침공 시작 스케줄
	RTT_INVADE_START = 10,

	// 침공 종료 스케줄
	RTT_INVADE_END = 11,

	// 침공 제한시간 리셋
	RTT_INVADE_RESET = 12,

	// 파티던전 제한시간 리셋
	RTT_PARTY_DUNGEON_RESET = 13,

	// 기사단 열성 포인트 리셋
	RTT_GUILD_ZEAL_POINT_RESET = 14,

	// 침공 무료 채팅 횟수 초기화
	RTT_FREE_INVADE_CHAT_RESET = 15,

	// 세금 정산 스케쥴
	RTT_CALCUATION_TAX = 16,

	// BM 상점 일일 초기화
	RTT_BM_DAILY = 17,

	// BM 상점 주간 초기화
	RTT_BM_WEEKLY = 18,

	// BM 상점 월간 초기화
	RTT_BM_MONTHLY = 19,

	// 이벤트 (커뮤니티 소녀) 출석
	RTT_ATTENDANCE_DAILY = 20,

	// 시즌패스 일일 초기화
	RTT_PASS_DAILY = 21,

	// 시즌패스 주간 초기화
	RTT_PASS_WEEKLY = 22,

	// 영지 던전: 공략 던전 리셋
	RTT_AGIT_DUNGEON_RAID = 23,

	MAX = 24,
};

UENUM(BlueprintType)
// 비용 타입
enum class CostType : uint8
{

	// 재화
	CURRENCY = 0,

	// 아이템
	ITEM = 1,

	MAX = 2,
};

UENUM()
// 아이템저장소
enum class ItemStorageType : uint8
{

	// 인벤토리
	INVENTORY = 0,

	// 창고
	DEPOT = 1,

	// 사망패널티
	DEATH = 2,

	// 거래소
	MARKET = 3,

	// 길드창고
	GUILD_STORE = 4,

	// 우편(수령 전)
	MAIL = 5,

	// 월드 거래소
	WDMARKET = 6,

	MAX = 7,
};

UENUM()
// 다단계 랭크 슬롯 상태
enum class MultiLevelRankSlotState : uint8
{

	// 잠긴 상태
	LOCK = 0,

	// 잠기지 않은 상태(미진행)
	UNLOCK = 1,

	// 진행 중
	IN_PROGRESS = 2,

	// 완료
	FINISH = 3,

	// 활성화 상태
	ACTIVE = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 구매횟수 제한 타입
enum class PurchaseLimitBuyerType : uint8
{
	NONE = 0,

	// 캐릭터
	CHARACTER = 1,

	// 서버
	SERVER = 2,

	// 계정
	ACCOUNT = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 구매횟수 제한 타입
enum class BMPurchaseLimitBuyerType : uint8
{
	NONE = 0,

	// 서버
	SERVER = 1,

	// 계정
	ACCOUNT = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 콘텐츠 타입
enum class ContentsType : uint8
{

	// 출석보상
	ATTENDANCE = 0,

	// 길드창고-아이템획득
	GUILD_STORE_ACQUIRED_ITEM = 1,

	// 길드창고-아이템 분배
	GUILD_STORE_DISTRIBUTED_ITEM = 2,

	// 고레벨필드던전1
	ELITE_DUNGEON_1 = 3,

	// 도감
	CODEX = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 콘텐츠 상태
enum class ContentsState : uint8
{

	// 레드닷
	RED_DOT = 0,

	// 보임
	VISIBLE = 1,

	// 숨김
	HIDDEN = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 콘텐츠 상태
enum class CanItemDrop : uint8
{

	// 기본 값 사용
	DEFAULT = 0,

	// 드랍
	DROP = 1,

	// 드랍하지 않음
	NO_DROP = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 게임 옵션 아이템 줍기 관련
enum class AutoDropItemGrade : uint8
{

	// 모두
	ALL = 0,

	// 매직
	MAGIC = 1,

	// 레어
	RARE = 2,
	OFF = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 공격 거리
enum class AutoAttackRange : uint8
{

	// 20 미터
	METER_20 = 0,

	// 30 미터
	METER_30 = 1,

	// 50 미터
	METER_50 = 2,

	// 제한없음
	NO_LIMIT = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 매너 모드
enum class MannerMode : uint8
{

	// 활성화
	ON = 0,

	// 파티,길드 제외
	EXCEPT_PARTY_GUILD = 1,

	// 비활성화
	OFF = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 디바이스 플랫폼
enum class DevicePlatformType : uint8
{

	// NONE
	NONE = 0,

	// WINDOWS
	WINDOWS = 1,

	// EDITOR
	EDITOR = 2,

	// ANDROID
	GOOLE_ANDROID = 3,

	// IOS
	APPLE_IOS = 4,

	// 기타
	ETC = 5,

	MAX = 6,
};

UENUM()
// 월드서버유저상태(일반/전투/캠프설치) - 행동제약과 무관
enum class UserStatus : uint8
{

	// 일반
	USER_STATE_NORMAL = 0,

	// 전투
	USER_STATE_COMBAT = 1,

	// 캠프설치
	USER_STATE_CAMP_INTERACT = 2,

	// PROP 인터렉션
	USER_STATE_PROP_INTERACT = 3,

	MAX = 4,
};

UENUM()
// 하이브 푸시 알림
enum class HiveNotificationType : uint8
{

	// 타플레이어에게 피격 당함
	TAKE_DAMAGE_IN_PVP = 0,

	// 사망
	DEAD = 1,

	// 무게
	WEIGHT_OVER = 2,

	// 던전 제한 시간 종료
	FINISH_DUNGEON = 3,

	// 오프라인 플레이 종료
	FINISH_OFFLINE_PLAY = 4,

	// 인벤토리가 가득 찼을 때,
	FULL_INVENTORY = 5,

	// 장비 획득 알림
	GET_EQUIP_ITEM = 6,

	// 스킬북 획득 알림
	GET_SKILL_BOOK = 7,

	// 거래소 판매 완료
	MARKET_ITEM_SOLD = 8,

	// 서버 이전 알림
	SERVER_MIGRATE = 9,

	MAX = 10,
};

UENUM(BlueprintType)
// NONE/ 일반서버/ 인터서버/ 침공서버
enum class EnterWorldServerType : uint8
{
	NONE = 0,

	// 일반서버 (1개 밖에 없다)
	NORMAL = 1,

	// 인터서버 (여러 개, 메인 월드서버나 다른 인터서버 간 동일한 맵ID를 소유할 수 없다)
	INTER_SERVER = 2,

	// 침공서버 (1개 밖에 없다, 메인 월드서버와 동일한 맵ID를 가지고 있다)
	INVADE_SERVER = 3,

	MAX = 4,
};

UENUM()
// NONE/ 필드 드랍/ 인벤토리로 지급/ 우편으로 바로지급
enum class RewardGiveType : uint8
{
	NONE = 0,

	// 필드 드랍
	FILED_DROP = 1,

	// 인벤토리로 지급
	INVENTORY = 2,

	// 우편으로 바로지급
	MAIL = 3,

	MAX = 4,
};

UENUM()
// 파티 타입
enum class PartyType : uint8
{

	// 일반 파티
	NORMAL = 0,

	// 협동 파티(파티 던전)
	COOP = 1,

	// 인터서버 파티
	INTER = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 던전 타입
enum class DungeonCategory : uint8
{

	// 일반 던전
	NORMAL = 0,

	// 특수 던전
	SPECIAL = 1,

	// 파티던전
	PARTY = 2,

	// 고레벨필드던전
	ELITE = 3,

	// 길드전
	ARENA = 4,

	MAX = 5,
};

UENUM()
// WG_ACK_ENTER_WORLD_FAIL 전송 실패 사유
enum class EnterWorldFailReason : uint8
{

	// 실패 사유 없음
	EWFR_NONE = 0,

	// 유효하지 않는 맵
	EWFR_INVALID_MAP = 1,

	// 채널입장 시도 실패
	EWFR_INAVLID_CHANNEL = 2,

	// 로그인 시 기사단 던전 맵으로 바로 입장 실패
	EWFR_LOGIN_INVALID_GUILD_DUNGEON = 3,

	// 연결 종료
	EWFR_FAILED_DISCONNECTED = 4,

	// 최종 진입 실패(LoginFailReason)
	EWFR_FAILED_COMPLETE = 5,

	// 다른 서버에서 월드보스 워프시 맵 입장 실패
	EWFR_INVALID_WORLD_BOSS = 6,

	// 이미 중복된 유저가 있다!
	EWFR_DUPLICATION_USER = 7,
};

UENUM()
// 대결 결과
enum class DuelResult : uint8
{

	// 대결 결과가 없음(무승부)
	NONE = 0,

	// 대결 승리
	WIN = 1,

	// 대결 패배
	LOSE = 2,
};

UENUM()
// Hive 채널 정보
enum class HiveChannel : uint8
{

	// Hive
	C2S = 0,

	// 카카오
	KAK = 1,

	// 라인
	LIN = 2,

	// 웨이보
	WEI = 3,

	// 스팀
	STE = 4,
};

UENUM()
// Hive 마켓 정보
enum class HiveMarket : uint8
{

	// SKT T store
	TS = 0,

	// KT Olleh Market
	OL = 1,

	// LGU+ Oz Store
	OZ = 2,

	// Apple App store
	AP = 3,

	// Google Play
	GO = 4,

	// Samsung Apps
	SA = 5,

	// Com2uS Lebi
	LE = 6,

	// ChinaMobile Mobile Market
	MM = 7,

	// Sina Weibo Point
	SN = 8,

	// Qihu 360 Point
	QP = 9,

	// Momo Momo Point
	MO = 10,

	// DeNA Mobage Point
	DN = 11,

	// Naver App Store
	NA = 12,

	// Amazon
	AM = 13,

	// ONE store
	ON = 14,

	// Funtap
	FU = 15,

	// Huawei
	HU = 16,

	// OPPO
	OP = 17,

	// VIVO
	VI = 18,

	// 샤오미
	XI = 19,

	// 텐센트
	TC = 20,

	// Hive Store(Crossplay)
	HS = 21,

	// Steam
	ST = 22,
};

UENUM(BlueprintType)
// 드랍 체크 타입
enum class DropCheckType : uint8
{

	// 맵
	MAP = 0,

	// NPC 등급
	RANK = 1,

	// NPC 종족
	RACE = 2,

	// NPC 속성
	ELEMENT = 3,

	// 침공 상태
	INVADE = 4,

	// 서버 ID
	SERVER = 5,

	// NpcFunctionType
	NPC_FUNCTION_TYPE = 6,

	// NpcStatCorrection - Npc스탯 보장 ID
	CORRECTION = 7,

	MAX = 8,
};

UENUM(BlueprintType)
// 침공 드랍 체크 타입
enum class InvadeDropType : uint8
{

	// 일반
	NORMAL = 0,

	// 침공 공격 타입
	ATTACK = 1,

	// 침공 방어 타입
	DEFENSE = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 발동 효과 타입
enum class CommonEffectType : uint8
{

	// 효과 없음 - 에러 아님
	NONE = 0,

	// 침공 포탈 설치
	SPAWN_INVADE_PORTAL = 1,

	// 던전 워프
	WARP_TO_DUNGEON = 2,

	// 침공 워프
	WARP_TO_INVADE = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// PK 결과
enum class PKBookResult : uint8
{

	// 처치
	KILL = 0,

	// 사망
	DIE = 1,

	// 도움
	ASSIST = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 즐겨찾기 타입
enum class BookMarkType : uint8
{

	// 위치 즐겨찾기
	POSITION = 0,

	// 아이템 컬렉션 즐겨찾기
	ITEM_COLLECTION = 1,

	// 길드 아이템 컬렉션 즐겨찾기
	GUILD_ITEM_COLLECTION = 2,

	// 거래소 즐겨찾기
	MARKET = 3,

	// 필드 보스 즐겨찾기
	FIELD_BOSS = 4,

	// 제작 즐겨찾기
	CRAFT = 5,

	// 스텟 즐겨찾기
	STAT = 6,

	// 월드 거래소 즐겨찾기
	WDMARKET = 7,

	// 기사단 영지 던전 공략
	AGIT_DUNGEON_RAID = 8,

	MAX = 9,
};

UENUM(BlueprintType)
// PK 기여자 타입
enum class PKContributorType : uint8
{

	// 유저
	USER = 0,

	// NPC
	NPC = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// PK북 대응 타입
enum class PKBookAction : uint8
{

	// 복수하기
	REVENGE = 0,

	// 조롱하기
	TEASE = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// PK 알림 타입
enum class PKNotifyType : uint8
{

	// 일반 PK타입
	NORMAL = 0,

	// 그림자전장 타입
	GUILD_WAR = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 사망 패널티 종류
enum class DeathPenaltyType : uint8
{
	NONE = 0,
	EXP = 1,
	ITEM = 2,

	MAX = 3,
};

UENUM()
// 개인거래 알림 타입
enum class PersonalTradingNoticeType : uint8
{

	// 거래 수락됨
	ACCEPTED = 0,

	// 거래 거절
	REJECT = 1,

	// 요청
	REQUEST = 2,

	// 중지
	ABORT = 3,

	// 중지: 유효하지 않은(어뷰징일수있다)
	ABORT_INVALID_STATUS = 4,

	// 요청시간 만료
	TIME_OUT = 5,

	// 잠금
	LOCK = 6,

	// 해제
	UNLOCK = 7,

	// 거래 최종  확인(수락)
	CONFIRM = 8,

	// 거래 성공
	SUCCESS = 9,

	MAX = 10,
};

UENUM()
// 개인거래 상태
enum class PersonalTradingStatus : uint8
{
	NONE = 0,

	// 거래 요청중
	WAINTING = 1,

	// 잠금 해제
	UNLOCK = 2,

	// 잠금 상태
	LOCK = 3,

	// 거래 최종 수락
	CONFIRM = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 소환 마일리지 타입
enum class SummonMileageActionType : uint8
{

	// 없음
	NONE = 0,

	// 페어리
	SUMMON_FAIRY = 1,

	// 코스튬
	SUMMON_COSTUME = 2,

	// Section 마일리지로 유니크 획득 로그에만 사용한다(DB값 사용금지)

	// 마일리지로 유니크 페어리 획득시
	UNIQUE_SUMMON_FAIRY = 3,

	// 마일리지로 유니크 코스튬 획득시
	UNIQUE_SUMMON_COSTUME = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 소환 마일리지 타입
enum class SummonMileageDataType : uint8
{

	// 없음
	NONE = 0,

	// 업적에서 가져와서 퉁치는 타입
	LEGACY_DATA = 1,

	// 2023년 8월 9일 신규 기획에 따른 일반적인 타입
	NORMAL_DATA = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 서버 이전 타입
enum class ServerMigrateType : uint8
{
	NONE = 0,

	// 계정만 이전
	ACCOUNT_ONLY = 1,

	// 기사단 정보 포함 이전
	INCLUDE_GUILD_INFO = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 서버 이전 제한 여부
enum class ServerMigrateLimitStatus : uint8
{
	NONE = 0,

	// 여유 서버
	FREE = 1,

	// 임박 서버
	IMMINENT = 2,

	// 제한 서버
	CLOSING = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 서버 이전 신청 조건
enum class ServerMigrateConditionType : uint8
{
	NONE = 0,

	// 유저가 서버 이전을 통해 이동할 서버를 선택했는지를 체크하는 타입, Clear = 선택완료/ Fail = 선택안함
	SERVER_CHOICE = 1,

	// 현재 캐릭터의 위치가 안전지역 마을인지를 체크, Clear = 안전지역마을/ Fail = 그외위치
	CHARACTER_LOCATION = 2,

	// 현재 캐릭터의 레벨과 ServerTransferConfig 테이블에 characterLevel 값과 비교, Clear = 현재캐릭터의레벨이같거나높음/ Fail = 현재캐릭터의레벨이낮음
	CHARACTER_LEVEL = 3,

	// 현재 계정 내 모든 캐릭터들이 파티에 가입되어 있는 상태인지를 체크, Clear = 파티상태가아님/ Fail = 파티상태임
	PARTY_JOINED = 4,

	// 현재 계정 내 모든 캐릭터들이 기사단에 가입되어 있는 상태인지를 체크( 계정서버이전시사용), Clear = 기사단에가입되어있는캐릭터가한명도없음/ Fail = 기사단에가입되어있는캐릭터가한명이상
	GUILD_JOINED = 5,

	// 현재 캐릭터를 제외한 계정 내 캐릭터들이 기사단에 가입되어 있는 상태인지를 체크( 기사단서버이전시사용), Clear = 기사단에가입되어있는캐릭터가한명도없음/ Fail = 기사단에가입되어있는캐릭터가한명이상
	GUILD_JOINED_SUB = 6,

	// 현재 계정 내 모든 캐릭터들이 기사단에 가입신청을 한 상태인지를 체크( 계정서버이전시사용), Clear = 기사단에가입신청한캐릭터가한명도없음/ Fail = 기사단에가입신청한캐릭터가한명이상
	GUILD_JOIN_APPLICATION = 7,

	// 현재 캐릭터를 제외한 계정 내 캐릭터들이 기사단에 가입신청을 한 상태인지를 체크( 기사단서버이전시사용), Clear = 기사단에가입신청한캐릭터가한명도없음/ Fail = 기사단에가입신청한캐릭터가한명이상
	GUILD_JOIN_APPLICATION_SUB = 8,

	// 현재 계정 내 모든 캐릭터들 중복구하지 않은 사망 패널티가 있는지를 체크, Clear = 복구하지않은사망패널티가있는캐릭터가한명도없음/ Fail = 복구하지않은사망패널티가있는캐릭터가한명이상
	RECOVERY_PENALTY = 9,

	// BM상점보관함에 아직 받지 않은 상품이 있는가를 체크, Clear = 받지않은상품없음/ Fail = 받지않은상품이한개이상
	BMSHOP_STORAGE = 10,

	// 거래소에 판매중인 상품이 있는지를 체크, Clear = 판매중인상품없음/ Fail = 판매중인상품있음
	MARKET_SELL = 11,

	// 거래소에 아직 정산받지 않은 목록이 있는지를 체크, Clear = 정산받을목록없음/ Fail = 정산받을목록이한개이상
	MARKET_CALCULATE = 12,

	// 현재 계정 내 모든 캐릭터들 중 보상을 수령하지 않은 개인/ 기사단 우편이 있는지를 체크, Clear = 보상을수령하지않은개인/ 기사단우편없음/ Fail = 보상을수령하지않은개인/ 기사단우편한개이상
	MAIL = 13,

	// 현재 캐릭터가 소속된 기사단의 레벨과 ServerTransferConfig 테이블에 guildLevel 값과 비교, Clear = 현재기사단의레벨이같거나높음/ Fail = 현재기사단의레벨이낮음
	GUILD_LEVEL = 14,

	// 현재 캐릭터가 소속된 기사단과 동맹인 기사단이 있는지를 체크, Clear = 동맹기사단없음/ Fail = 동맹기사단이한개이상
	GUILD_ALLY = 15,

	// 현재 캐릭터가 소속된 기사단이 적대로 등록한 기사단이 있는지를 체크, Clear = 적대기사단없음/ Fail = 적대기사단이한개이상
	GUILD_ENEMY = 16,

	// 현재 캐릭터가 소속된 기사단 내 분배 가능한 아이템이 있는지를 체크, Clear = 분배가능한아이템이없음/ Fail = 분배가능한아이템이한개이상
	GUILD_DIVISION = 17,

	// 현재 캐릭터가 소속된 기사단 내 경매 진행중인 아이템이 있는지를 체크, Clear = 경매진행중인아이템이없음/ Fail = 경매진행중인아이템이한개이상
	GUILD_AUCTION = 18,

	// 현재 캐릭터가 소속된 기사단에서 경매에 참여하였다가 실패하여 돌려받을 입찰금을 아직 회수하지 않았는가를 체크, Clear = 회수할입찰금이없음/ Fail = 회수할입찰금이있음
	GUILD_AUCTION_WITHDRAW = 19,

	// 현재 캐릭터가 소속된 기사단이 설치한 수호탑이 있는지를 체크, Clear = 설치한수호탑이없음/ Fail = 설치한수호탑이있음
	GUILD_CAMP = 20,

	// 현재 캐릭터가 소속된 기사단에 길드원이 없는지는 체크, Clear = 현재 캐릭터를 제외한 기사단원이 없음 / Fail = 현재 캐릭터를 제외한 기사단원이 있음
	GUILD_MEMBER = 21,

	// 거래소에 판매중인 상품이 있는지를 체크, Clear = 판매중인상품없음/ Fail = 판매중인상품있음
	WORLDMARKET_SELL = 22,

	// 거래소에 아직 정산받지 않은 목록이 있는지를 체크, Clear = 정산받을목록없음/ Fail = 정산받을목록이한개이상
	WORLDMARKET_CALCULATE = 23,

	// 기사단 영지 던전이 오픈 되어 있는지를 체크, Clear = 기사단 영지 던전이 닫혀 있음 / Fail = 기사단 영지 던전이 열려 있음
	GUILDRAID_CLOSED = 24,

	// 기사단장이 그림자 전장에 참여를 신청한 상태이거나 참여 중인 상태인지를 체크, Clear = 참여 신청이나 참여 중인 상태가 아님 / Fail = 참여 신청이나 참여 중인 상태임
	GUILDWAR_JOINED = 25,

	MAX = 26,
};

UENUM(BlueprintType)
// 그림자전장 랭킹 티어
enum class BattleArenaTier : uint8
{

	// 티어 없음
	NONE = 0,

	// 티어 - 챔피언
	CHAMPION = 1,

	// 티어 - 마스터
	MASTER = 2,

	// 티어 - 다이아몬드
	DIAMOND = 3,

	// 티어 - 골드
	GOLD = 4,

	// 티어 - 실버
	SILVER = 5,

	// 티어 - 브론즈
	BRONZE = 6,

	MAX = 7,
};

UENUM(BlueprintType)
// 컨텐츠 귀속 타입
enum class ContentsBindingType : uint8
{

	// 유저 귀속
	USER = 0,

	// 계정 귀속
	ACCOUNT = 1,

	MAX = 2,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline AccountGrade AccountGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return AccountGrade::NORMAL;
	if (str.Compare(TEXT("OPERATOR"), ESearchCase::IgnoreCase) == 0)	return AccountGrade::OPERATOR;
	if (str.Compare(TEXT("QATESTER"), ESearchCase::IgnoreCase) == 0)	return AccountGrade::QATESTER;
	if (str.Compare(TEXT("ADMIN"), ESearchCase::IgnoreCase) == 0)	return AccountGrade::ADMIN;
	if (str.Compare(TEXT("GM"), ESearchCase::IgnoreCase) == 0)	return AccountGrade::GM;
	return AccountGrade::MAX;
}
inline ServerCongestion ServerCongestionStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ON_STANBY"), ESearchCase::IgnoreCase) == 0)	return ServerCongestion::ON_STANBY;
	if (str.Compare(TEXT("GOOD"), ESearchCase::IgnoreCase) == 0)	return ServerCongestion::GOOD;
	if (str.Compare(TEXT("CONGESTED"), ESearchCase::IgnoreCase) == 0)	return ServerCongestion::CONGESTED;
	if (str.Compare(TEXT("FULL"), ESearchCase::IgnoreCase) == 0)	return ServerCongestion::FULL;
	if (str.Compare(TEXT("RESTRICTED_ACCESS"), ESearchCase::IgnoreCase) == 0)	return ServerCongestion::RESTRICTED_ACCESS;
	if (str.Compare(TEXT("BLOCK"), ESearchCase::IgnoreCase) == 0)	return ServerCongestion::BLOCK;
	if (str.Compare(TEXT("NOT_CONNECTABLE"), ESearchCase::IgnoreCase) == 0)	return ServerCongestion::NOT_CONNECTABLE;
	return ServerCongestion::MAX;
}
inline MapType MapTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return MapType::NONE;
	if (str.Compare(TEXT("PUBLIC_FIELD"), ESearchCase::IgnoreCase) == 0)	return MapType::PUBLIC_FIELD;
	if (str.Compare(TEXT("SINGLE_FIELD"), ESearchCase::IgnoreCase) == 0)	return MapType::SINGLE_FIELD;
	if (str.Compare(TEXT("INSTANCE_SINGLE"), ESearchCase::IgnoreCase) == 0)	return MapType::INSTANCE_SINGLE;
	if (str.Compare(TEXT("INSTANCE_FIELD"), ESearchCase::IgnoreCase) == 0)	return MapType::INSTANCE_FIELD;
	if (str.Compare(TEXT("PUBLIC_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return MapType::PUBLIC_DUNGEON;
	if (str.Compare(TEXT("GUILD_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return MapType::GUILD_DUNGEON;
	if (str.Compare(TEXT("PARTY_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return MapType::PARTY_DUNGEON;
	if (str.Compare(TEXT("ELITE_DUNGEON_1"), ESearchCase::IgnoreCase) == 0)	return MapType::ELITE_DUNGEON_1;
	if (str.Compare(TEXT("WORLD_BOSS"), ESearchCase::IgnoreCase) == 0)	return MapType::WORLD_BOSS;
	if (str.Compare(TEXT("INTER_FIELD"), ESearchCase::IgnoreCase) == 0)	return MapType::INTER_FIELD;
	if (str.Compare(TEXT("GUILD_WAR"), ESearchCase::IgnoreCase) == 0)	return MapType::GUILD_WAR;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID"), ESearchCase::IgnoreCase) == 0)	return MapType::AGIT_DUNGEON_RAID;
	if (str.Compare(TEXT("AGIT"), ESearchCase::IgnoreCase) == 0)	return MapType::AGIT;
	return MapType::MAX;
}
inline MapContentsType MapContentsTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return MapContentsType::NORMAL;
	if (str.Compare(TEXT("BATTLE"), ESearchCase::IgnoreCase) == 0)	return MapContentsType::BATTLE;
	if (str.Compare(TEXT("OUTLAW"), ESearchCase::IgnoreCase) == 0)	return MapContentsType::OUTLAW;
	if (str.Compare(TEXT("SPACE_CRACK"), ESearchCase::IgnoreCase) == 0)	return MapContentsType::SPACE_CRACK;
	return MapContentsType::MAX;
}
inline WarpType WarpTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("Immediately"), ESearchCase::IgnoreCase) == 0)	return WarpType::Immediately;
	if (str.Compare(TEXT("YesNoDialog"), ESearchCase::IgnoreCase) == 0)	return WarpType::YesNoDialog;
	return WarpType::MAX;
}
inline TerritoryType TerritoryTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::NONE;
	if (str.Compare(TEXT("TOWN"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::TOWN;
	if (str.Compare(TEXT("SAFE_ZONE"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::SAFE_ZONE;
	if (str.Compare(TEXT("ARENA_READY"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::ARENA_READY;
	if (str.Compare(TEXT("ARENA"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::ARENA;
	if (str.Compare(TEXT("ARRIVAL"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::ARRIVAL;
	if (str.Compare(TEXT("Env"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::Env;
	if (str.Compare(TEXT("SOUND"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::SOUND;
	if (str.Compare(TEXT("SPACE_CRACK"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::SPACE_CRACK;
	if (str.Compare(TEXT("TOWN_CHAOS"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::TOWN_CHAOS;
	if (str.Compare(TEXT("NO_CAMP_ZONE"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::NO_CAMP_ZONE;
	if (str.Compare(TEXT("NON_PK"), ESearchCase::IgnoreCase) == 0)	return TerritoryType::NON_PK;
	return TerritoryType::MAX;
}
inline TerritoryCheckType TerritoryCheckTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return TerritoryCheckType::NONE;
	if (str.Compare(TEXT("INVADE"), ESearchCase::IgnoreCase) == 0)	return TerritoryCheckType::INVADE;
	return TerritoryCheckType::MAX;
}
inline ChannelCongestionStatus ChannelCongestionStatusStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("LOW"), ESearchCase::IgnoreCase) == 0)	return ChannelCongestionStatus::LOW;
	if (str.Compare(TEXT("MEDIUM"), ESearchCase::IgnoreCase) == 0)	return ChannelCongestionStatus::MEDIUM;
	if (str.Compare(TEXT("HIGH"), ESearchCase::IgnoreCase) == 0)	return ChannelCongestionStatus::HIGH;
	if (str.Compare(TEXT("FULL"), ESearchCase::IgnoreCase) == 0)	return ChannelCongestionStatus::FULL;
	return ChannelCongestionStatus::MAX;
}
inline WarpReason WarpReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ENTER_WORLD"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ENTER_WORLD;
	if (str.Compare(TEXT("CHANGE_CHANNEL"), ESearchCase::IgnoreCase) == 0)	return WarpReason::CHANGE_CHANNEL;
	if (str.Compare(TEXT("PORTAL"), ESearchCase::IgnoreCase) == 0)	return WarpReason::PORTAL;
	if (str.Compare(TEXT("PARTY_MATCHING"), ESearchCase::IgnoreCase) == 0)	return WarpReason::PARTY_MATCHING;
	if (str.Compare(TEXT("RESURRECT"), ESearchCase::IgnoreCase) == 0)	return WarpReason::RESURRECT;
	if (str.Compare(TEXT("ADMIN_COMMAND"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ADMIN_COMMAND;
	if (str.Compare(TEXT("QUEST"), ESearchCase::IgnoreCase) == 0)	return WarpReason::QUEST;
	if (str.Compare(TEXT("EVENT_ACTION_TO_INSTANCE_SINGLE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::EVENT_ACTION_TO_INSTANCE_SINGLE;
	if (str.Compare(TEXT("LEAVE_INSTANCE_SINGLE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::LEAVE_INSTANCE_SINGLE;
	if (str.Compare(TEXT("EVENT_ACTION_RETURN_TO_PUBLIC"), ESearchCase::IgnoreCase) == 0)	return WarpReason::EVENT_ACTION_RETURN_TO_PUBLIC;
	if (str.Compare(TEXT("EVENT_ACTION_TELEPORT"), ESearchCase::IgnoreCase) == 0)	return WarpReason::EVENT_ACTION_TELEPORT;
	if (str.Compare(TEXT("BOOKMARK"), ESearchCase::IgnoreCase) == 0)	return WarpReason::BOOKMARK;
	if (str.Compare(TEXT("USE_WARP_TO_TOWN_SCROLL"), ESearchCase::IgnoreCase) == 0)	return WarpReason::USE_WARP_TO_TOWN_SCROLL;
	if (str.Compare(TEXT("USE_WARP_TO_RANDOM_SCROLL"), ESearchCase::IgnoreCase) == 0)	return WarpReason::USE_WARP_TO_RANDOM_SCROLL;
	if (str.Compare(TEXT("DUNGEON_TIME_OVER"), ESearchCase::IgnoreCase) == 0)	return WarpReason::DUNGEON_TIME_OVER;
	if (str.Compare(TEXT("MAP_EVENT"), ESearchCase::IgnoreCase) == 0)	return WarpReason::MAP_EVENT;
	if (str.Compare(TEXT("BOOKMARK_SHARED"), ESearchCase::IgnoreCase) == 0)	return WarpReason::BOOKMARK_SHARED;
	if (str.Compare(TEXT("PARTY_LEADER"), ESearchCase::IgnoreCase) == 0)	return WarpReason::PARTY_LEADER;
	if (str.Compare(TEXT("SPACE_CRACK"), ESearchCase::IgnoreCase) == 0)	return WarpReason::SPACE_CRACK;
	if (str.Compare(TEXT("WORLD_MAP_PORTAL"), ESearchCase::IgnoreCase) == 0)	return WarpReason::WORLD_MAP_PORTAL;
	if (str.Compare(TEXT("EVENT_DUNGEON_TIME_OVER"), ESearchCase::IgnoreCase) == 0)	return WarpReason::EVENT_DUNGEON_TIME_OVER;
	if (str.Compare(TEXT("ENTER_GUILD_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ENTER_GUILD_DUNGEON;
	if (str.Compare(TEXT("LEAVE_GUILD_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return WarpReason::LEAVE_GUILD_DUNGEON;
	if (str.Compare(TEXT("GUILD_CAMP"), ESearchCase::IgnoreCase) == 0)	return WarpReason::GUILD_CAMP;
	if (str.Compare(TEXT("INVADE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::INVADE;
	if (str.Compare(TEXT("INVADE_LEAVE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::INVADE_LEAVE;
	if (str.Compare(TEXT("INVADE_TIME_OVER"), ESearchCase::IgnoreCase) == 0)	return WarpReason::INVADE_TIME_OVER;
	if (str.Compare(TEXT("INVADE_USER_TIME_OVER"), ESearchCase::IgnoreCase) == 0)	return WarpReason::INVADE_USER_TIME_OVER;
	if (str.Compare(TEXT("PARTY_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return WarpReason::PARTY_DUNGEON;
	if (str.Compare(TEXT("PARTY_DUNGEON_LEAVE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::PARTY_DUNGEON_LEAVE;
	if (str.Compare(TEXT("PARTY_DUNGEON_TIME_OVER"), ESearchCase::IgnoreCase) == 0)	return WarpReason::PARTY_DUNGEON_TIME_OVER;
	if (str.Compare(TEXT("ELITE_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ELITE_DUNGEON;
	if (str.Compare(TEXT("ELITE_DUNGEON_LEAVE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ELITE_DUNGEON_LEAVE;
	if (str.Compare(TEXT("ELITE_DUNGEON_TIME_OVER"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ELITE_DUNGEON_TIME_OVER;
	if (str.Compare(TEXT("ARENA"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ARENA;
	if (str.Compare(TEXT("ARENA_LEAVE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ARENA_LEAVE;
	if (str.Compare(TEXT("ITEM_TREASURE_MONSTER"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ITEM_TREASURE_MONSTER;
	if (str.Compare(TEXT("REVENGE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::REVENGE;
	if (str.Compare(TEXT("INVADE_USE_SCROLL_LEAVE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::INVADE_USE_SCROLL_LEAVE;
	if (str.Compare(TEXT("WORLD_BOSS"), ESearchCase::IgnoreCase) == 0)	return WarpReason::WORLD_BOSS;
	if (str.Compare(TEXT("WORLD_BOSS_TIME_OUT"), ESearchCase::IgnoreCase) == 0)	return WarpReason::WORLD_BOSS_TIME_OUT;
	if (str.Compare(TEXT("WORLD_BOSS_LEAVE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::WORLD_BOSS_LEAVE;
	if (str.Compare(TEXT("DESTROYED_THEN_RETURN_PUBLIC_FIELD"), ESearchCase::IgnoreCase) == 0)	return WarpReason::DESTROYED_THEN_RETURN_PUBLIC_FIELD;
	if (str.Compare(TEXT("WORLD_MAP_PORTAL_TO_BOSS"), ESearchCase::IgnoreCase) == 0)	return WarpReason::WORLD_MAP_PORTAL_TO_BOSS;
	if (str.Compare(TEXT("ELITE_DUNGEON_HIDDEN"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ELITE_DUNGEON_HIDDEN;
	if (str.Compare(TEXT("USE_DUNGEON_TICKET"), ESearchCase::IgnoreCase) == 0)	return WarpReason::USE_DUNGEON_TICKET;
	if (str.Compare(TEXT("GUILD_WAR"), ESearchCase::IgnoreCase) == 0)	return WarpReason::GUILD_WAR;
	if (str.Compare(TEXT("GUILD_WAR_TIME_OUT"), ESearchCase::IgnoreCase) == 0)	return WarpReason::GUILD_WAR_TIME_OUT;
	if (str.Compare(TEXT("GUILD_WAR_LEAVE"), ESearchCase::IgnoreCase) == 0)	return WarpReason::GUILD_WAR_LEAVE;
	if (str.Compare(TEXT("SKILL_NOTIFY"), ESearchCase::IgnoreCase) == 0)	return WarpReason::SKILL_NOTIFY;
	if (str.Compare(TEXT("ENTER_AGIT_DUNGEON_RAID"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ENTER_AGIT_DUNGEON_RAID;
	if (str.Compare(TEXT("LEAVE_SELF_AGIT_DUNGEON_RAID"), ESearchCase::IgnoreCase) == 0)	return WarpReason::LEAVE_SELF_AGIT_DUNGEON_RAID;
	if (str.Compare(TEXT("ENTER_AGIT"), ESearchCase::IgnoreCase) == 0)	return WarpReason::ENTER_AGIT;
	if (str.Compare(TEXT("LEAVE_AGIT"), ESearchCase::IgnoreCase) == 0)	return WarpReason::LEAVE_AGIT;
	if (str.Compare(TEXT("LEAVE_AGIT_RANDOM_SCROLL"), ESearchCase::IgnoreCase) == 0)	return WarpReason::LEAVE_AGIT_RANDOM_SCROLL;
	if (str.Compare(TEXT("LEAVE_FORCE_AGIT_DUNGEON_RAID"), ESearchCase::IgnoreCase) == 0)	return WarpReason::LEAVE_FORCE_AGIT_DUNGEON_RAID;
	return WarpReason::INVALID;
}
inline CommandType CommandTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("COMMAND_OPERATING"), ESearchCase::IgnoreCase) == 0)	return CommandType::COMMAND_OPERATING;
	if (str.Compare(TEXT("COMMAND_QA"), ESearchCase::IgnoreCase) == 0)	return CommandType::COMMAND_QA;
	return CommandType::COMMAND_QA;
}
inline CommandHandlerType CommandHandlerTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("HANDLER_OPERATING_GATEWAY"), ESearchCase::IgnoreCase) == 0)	return CommandHandlerType::HANDLER_OPERATING_GATEWAY;
	if (str.Compare(TEXT("HANDLER_OPERATING_WORLD"), ESearchCase::IgnoreCase) == 0)	return CommandHandlerType::HANDLER_OPERATING_WORLD;
	if (str.Compare(TEXT("HANDLER_QA_GATEWAY"), ESearchCase::IgnoreCase) == 0)	return CommandHandlerType::HANDLER_QA_GATEWAY;
	if (str.Compare(TEXT("HANDLER_QA_WORLD"), ESearchCase::IgnoreCase) == 0)	return CommandHandlerType::HANDLER_QA_WORLD;
	if (str.Compare(TEXT("HANDLER_RANK_GATEWAY"), ESearchCase::IgnoreCase) == 0)	return CommandHandlerType::HANDLER_RANK_GATEWAY;
	if (str.Compare(TEXT("HANDLER_HIVE_ITEM_GATEWAY"), ESearchCase::IgnoreCase) == 0)	return CommandHandlerType::HANDLER_HIVE_ITEM_GATEWAY;
	if (str.Compare(TEXT("HANDLER_OPERATING_AUTH"), ESearchCase::IgnoreCase) == 0)	return CommandHandlerType::HANDLER_OPERATING_AUTH;
	if (str.Compare(TEXT("HANDLER_OPERATING_DIRECTORY"), ESearchCase::IgnoreCase) == 0)	return CommandHandlerType::HANDLER_OPERATING_DIRECTORY;
	if (str.Compare(TEXT("HANDLER_SERVER_TO_SERVER_GATEWAY"), ESearchCase::IgnoreCase) == 0)	return CommandHandlerType::HANDLER_SERVER_TO_SERVER_GATEWAY;
	return CommandHandlerType::HANDLER_SERVER_TO_SERVER_GATEWAY;
}
inline TimeTicketType TimeTicketTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("TIME_TICKET_EXP_DEATH_PENALTY_FREE"), ESearchCase::IgnoreCase) == 0)	return TimeTicketType::TIME_TICKET_EXP_DEATH_PENALTY_FREE;
	if (str.Compare(TEXT("TIME_TICKET_ITEM_DEATH_PENALTY_FREE"), ESearchCase::IgnoreCase) == 0)	return TimeTicketType::TIME_TICKET_ITEM_DEATH_PENALTY_FREE;
	return TimeTicketType::MAX;
}
inline CurrencyType CurrencyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::NONE;
	if (str.Compare(TEXT("DIA"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::DIA;
	if (str.Compare(TEXT("FREE_DIA"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::FREE_DIA;
	if (str.Compare(TEXT("MILEAGE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::MILEAGE;
	if (str.Compare(TEXT("GOLD"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::GOLD;
	if (str.Compare(TEXT("CASH_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::CASH_MILEAGE;
	if (str.Compare(TEXT("MONSTER_BOOK_POINT"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::MONSTER_BOOK_POINT;
	if (str.Compare(TEXT("SUMMON_UNIQUE_FAIRY_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE;
	if (str.Compare(TEXT("SUMMON_UNIQUE_COSTUME_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE;
	if (str.Compare(TEXT("MAX_ACCOUNT_TYPE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::MAX_ACCOUNT_TYPE;
	if (str.Compare(TEXT("START_USER_TYPE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::START_USER_TYPE;
	if (str.Compare(TEXT("EXP_DEATH_PENALTY_FREE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::EXP_DEATH_PENALTY_FREE;
	if (str.Compare(TEXT("ITEM_DEATH_PENALTY_FREE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::ITEM_DEATH_PENALTY_FREE;
	if (str.Compare(TEXT("DEATH_EXP_RECOVERY"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::DEATH_EXP_RECOVERY;
	if (str.Compare(TEXT("SUMMON_FAIRY_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::SUMMON_FAIRY_MILEAGE;
	if (str.Compare(TEXT("SUMMON_COSTUME_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::SUMMON_COSTUME_MILEAGE;
	if (str.Compare(TEXT("MAX_USER_TYPE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::MAX_USER_TYPE;
	if (str.Compare(TEXT("START_GUILD_TYPE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::START_GUILD_TYPE;
	if (str.Compare(TEXT("GUILD_ASSET"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::GUILD_ASSET;
	if (str.Compare(TEXT("MAX_GUILD_TYPE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::MAX_GUILD_TYPE;
	if (str.Compare(TEXT("MAX_IN_GAME_TYPE"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::MAX_IN_GAME_TYPE;
	if (str.Compare(TEXT("CASH"), ESearchCase::IgnoreCase) == 0)	return CurrencyType::CASH;
	return CurrencyType::MAX;
}
inline RewardType RewardTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ALL"), ESearchCase::IgnoreCase) == 0)	return RewardType::ALL;
	if (str.Compare(TEXT("PICK_ONE"), ESearchCase::IgnoreCase) == 0)	return RewardType::PICK_ONE;
	if (str.Compare(TEXT("RANDOM_ONE"), ESearchCase::IgnoreCase) == 0)	return RewardType::RANDOM_ONE;
	if (str.Compare(TEXT("RANDOM_GROUP"), ESearchCase::IgnoreCase) == 0)	return RewardType::RANDOM_GROUP;
	if (str.Compare(TEXT("PICK_REWARD_BOX_LIST"), ESearchCase::IgnoreCase) == 0)	return RewardType::PICK_REWARD_BOX_LIST;
	if (str.Compare(TEXT("SUB_PICK_ONE"), ESearchCase::IgnoreCase) == 0)	return RewardType::SUB_PICK_ONE;
	if (str.Compare(TEXT("SUB_RANDOM_ONE"), ESearchCase::IgnoreCase) == 0)	return RewardType::SUB_RANDOM_ONE;
	return RewardType::MAX;
}
inline WeightPenaltyGrade WeightPenaltyGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return WeightPenaltyGrade::NONE;
	if (str.Compare(TEXT("FIRST"), ESearchCase::IgnoreCase) == 0)	return WeightPenaltyGrade::FIRST;
	if (str.Compare(TEXT("SECOND"), ESearchCase::IgnoreCase) == 0)	return WeightPenaltyGrade::SECOND;
	return WeightPenaltyGrade::MAX;
}
inline PartyDropOwnershipType PartyDropOwnershipTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("RANDOM"), ESearchCase::IgnoreCase) == 0)	return PartyDropOwnershipType::RANDOM;
	if (str.Compare(TEXT("SEQUENCE"), ESearchCase::IgnoreCase) == 0)	return PartyDropOwnershipType::SEQUENCE;
	if (str.Compare(TEXT("FREE"), ESearchCase::IgnoreCase) == 0)	return PartyDropOwnershipType::FREE;
	return PartyDropOwnershipType::MAX;
}
inline RestoreType RestoreTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("Free"), ESearchCase::IgnoreCase) == 0)	return RestoreType::Free;
	if (str.Compare(TEXT("Currency"), ESearchCase::IgnoreCase) == 0)	return RestoreType::Currency;
	return RestoreType::MAX;
}
inline ResetTimeTicket ResetTimeTicketStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("RTT_NONE"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_NONE;
	if (str.Compare(TEXT("RTT_DAILY"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_DAILY;
	if (str.Compare(TEXT("RTT_WEEKLY"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_WEEKLY;
	if (str.Compare(TEXT("RTT_MONTHLY"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_MONTHLY;
	if (str.Compare(TEXT("RTT_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_DUNGEON;
	if (str.Compare(TEXT("RTT_SPACEC_CRACK"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_SPACEC_CRACK;
	if (str.Compare(TEXT("RTT_OFF_LINE_MODE"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_OFF_LINE_MODE;
	if (str.Compare(TEXT("RTT_DEATH_RESURRECTION"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_DEATH_RESURRECTION;
	if (str.Compare(TEXT("RTT_GUILD_QUEST"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_GUILD_QUEST;
	if (str.Compare(TEXT("RTT_GUILD_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_GUILD_DUNGEON;
	if (str.Compare(TEXT("RTT_INVADE_START"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_INVADE_START;
	if (str.Compare(TEXT("RTT_INVADE_END"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_INVADE_END;
	if (str.Compare(TEXT("RTT_INVADE_RESET"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_INVADE_RESET;
	if (str.Compare(TEXT("RTT_PARTY_DUNGEON_RESET"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_PARTY_DUNGEON_RESET;
	if (str.Compare(TEXT("RTT_GUILD_ZEAL_POINT_RESET"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_GUILD_ZEAL_POINT_RESET;
	if (str.Compare(TEXT("RTT_FREE_INVADE_CHAT_RESET"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_FREE_INVADE_CHAT_RESET;
	if (str.Compare(TEXT("RTT_CALCUATION_TAX"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_CALCUATION_TAX;
	if (str.Compare(TEXT("RTT_BM_DAILY"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_BM_DAILY;
	if (str.Compare(TEXT("RTT_BM_WEEKLY"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_BM_WEEKLY;
	if (str.Compare(TEXT("RTT_BM_MONTHLY"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_BM_MONTHLY;
	if (str.Compare(TEXT("RTT_ATTENDANCE_DAILY"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_ATTENDANCE_DAILY;
	if (str.Compare(TEXT("RTT_PASS_DAILY"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_PASS_DAILY;
	if (str.Compare(TEXT("RTT_PASS_WEEKLY"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_PASS_WEEKLY;
	if (str.Compare(TEXT("RTT_AGIT_DUNGEON_RAID"), ESearchCase::IgnoreCase) == 0)	return ResetTimeTicket::RTT_AGIT_DUNGEON_RAID;
	return ResetTimeTicket::MAX;
}
inline CostType CostTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CURRENCY"), ESearchCase::IgnoreCase) == 0)	return CostType::CURRENCY;
	if (str.Compare(TEXT("ITEM"), ESearchCase::IgnoreCase) == 0)	return CostType::ITEM;
	return CostType::MAX;
}
inline ItemStorageType ItemStorageTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("INVENTORY"), ESearchCase::IgnoreCase) == 0)	return ItemStorageType::INVENTORY;
	if (str.Compare(TEXT("DEPOT"), ESearchCase::IgnoreCase) == 0)	return ItemStorageType::DEPOT;
	if (str.Compare(TEXT("DEATH"), ESearchCase::IgnoreCase) == 0)	return ItemStorageType::DEATH;
	if (str.Compare(TEXT("MARKET"), ESearchCase::IgnoreCase) == 0)	return ItemStorageType::MARKET;
	if (str.Compare(TEXT("GUILD_STORE"), ESearchCase::IgnoreCase) == 0)	return ItemStorageType::GUILD_STORE;
	if (str.Compare(TEXT("MAIL"), ESearchCase::IgnoreCase) == 0)	return ItemStorageType::MAIL;
	if (str.Compare(TEXT("WDMARKET"), ESearchCase::IgnoreCase) == 0)	return ItemStorageType::WDMARKET;
	return ItemStorageType::MAX;
}
inline MultiLevelRankSlotState MultiLevelRankSlotStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("LOCK"), ESearchCase::IgnoreCase) == 0)	return MultiLevelRankSlotState::LOCK;
	if (str.Compare(TEXT("UNLOCK"), ESearchCase::IgnoreCase) == 0)	return MultiLevelRankSlotState::UNLOCK;
	if (str.Compare(TEXT("IN_PROGRESS"), ESearchCase::IgnoreCase) == 0)	return MultiLevelRankSlotState::IN_PROGRESS;
	if (str.Compare(TEXT("FINISH"), ESearchCase::IgnoreCase) == 0)	return MultiLevelRankSlotState::FINISH;
	if (str.Compare(TEXT("ACTIVE"), ESearchCase::IgnoreCase) == 0)	return MultiLevelRankSlotState::ACTIVE;
	return MultiLevelRankSlotState::MAX;
}
inline PurchaseLimitBuyerType PurchaseLimitBuyerTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return PurchaseLimitBuyerType::NONE;
	if (str.Compare(TEXT("CHARACTER"), ESearchCase::IgnoreCase) == 0)	return PurchaseLimitBuyerType::CHARACTER;
	if (str.Compare(TEXT("SERVER"), ESearchCase::IgnoreCase) == 0)	return PurchaseLimitBuyerType::SERVER;
	if (str.Compare(TEXT("ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return PurchaseLimitBuyerType::ACCOUNT;
	return PurchaseLimitBuyerType::MAX;
}
inline BMPurchaseLimitBuyerType BMPurchaseLimitBuyerTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return BMPurchaseLimitBuyerType::NONE;
	if (str.Compare(TEXT("SERVER"), ESearchCase::IgnoreCase) == 0)	return BMPurchaseLimitBuyerType::SERVER;
	if (str.Compare(TEXT("ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return BMPurchaseLimitBuyerType::ACCOUNT;
	return BMPurchaseLimitBuyerType::MAX;
}
inline ContentsType ContentsTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ATTENDANCE"), ESearchCase::IgnoreCase) == 0)	return ContentsType::ATTENDANCE;
	if (str.Compare(TEXT("GUILD_STORE_ACQUIRED_ITEM"), ESearchCase::IgnoreCase) == 0)	return ContentsType::GUILD_STORE_ACQUIRED_ITEM;
	if (str.Compare(TEXT("GUILD_STORE_DISTRIBUTED_ITEM"), ESearchCase::IgnoreCase) == 0)	return ContentsType::GUILD_STORE_DISTRIBUTED_ITEM;
	if (str.Compare(TEXT("ELITE_DUNGEON_1"), ESearchCase::IgnoreCase) == 0)	return ContentsType::ELITE_DUNGEON_1;
	if (str.Compare(TEXT("CODEX"), ESearchCase::IgnoreCase) == 0)	return ContentsType::CODEX;
	return ContentsType::MAX;
}
inline ContentsState ContentsStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("RED_DOT"), ESearchCase::IgnoreCase) == 0)	return ContentsState::RED_DOT;
	if (str.Compare(TEXT("VISIBLE"), ESearchCase::IgnoreCase) == 0)	return ContentsState::VISIBLE;
	if (str.Compare(TEXT("HIDDEN"), ESearchCase::IgnoreCase) == 0)	return ContentsState::HIDDEN;
	return ContentsState::MAX;
}
inline CanItemDrop CanItemDropStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("DEFAULT"), ESearchCase::IgnoreCase) == 0)	return CanItemDrop::DEFAULT;
	if (str.Compare(TEXT("DROP"), ESearchCase::IgnoreCase) == 0)	return CanItemDrop::DROP;
	if (str.Compare(TEXT("NO_DROP"), ESearchCase::IgnoreCase) == 0)	return CanItemDrop::NO_DROP;
	return CanItemDrop::MAX;
}
inline AutoDropItemGrade AutoDropItemGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ALL"), ESearchCase::IgnoreCase) == 0)	return AutoDropItemGrade::ALL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return AutoDropItemGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return AutoDropItemGrade::RARE;
	if (str.Compare(TEXT("OFF"), ESearchCase::IgnoreCase) == 0)	return AutoDropItemGrade::OFF;
	return AutoDropItemGrade::MAX;
}
inline AutoAttackRange AutoAttackRangeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("METER_20"), ESearchCase::IgnoreCase) == 0)	return AutoAttackRange::METER_20;
	if (str.Compare(TEXT("METER_30"), ESearchCase::IgnoreCase) == 0)	return AutoAttackRange::METER_30;
	if (str.Compare(TEXT("METER_50"), ESearchCase::IgnoreCase) == 0)	return AutoAttackRange::METER_50;
	if (str.Compare(TEXT("NO_LIMIT"), ESearchCase::IgnoreCase) == 0)	return AutoAttackRange::NO_LIMIT;
	return AutoAttackRange::MAX;
}
inline MannerMode MannerModeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ON"), ESearchCase::IgnoreCase) == 0)	return MannerMode::ON;
	if (str.Compare(TEXT("EXCEPT_PARTY_GUILD"), ESearchCase::IgnoreCase) == 0)	return MannerMode::EXCEPT_PARTY_GUILD;
	if (str.Compare(TEXT("OFF"), ESearchCase::IgnoreCase) == 0)	return MannerMode::OFF;
	return MannerMode::MAX;
}
inline DevicePlatformType DevicePlatformTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return DevicePlatformType::NONE;
	if (str.Compare(TEXT("WINDOWS"), ESearchCase::IgnoreCase) == 0)	return DevicePlatformType::WINDOWS;
	if (str.Compare(TEXT("EDITOR"), ESearchCase::IgnoreCase) == 0)	return DevicePlatformType::EDITOR;
	if (str.Compare(TEXT("GOOLE_ANDROID"), ESearchCase::IgnoreCase) == 0)	return DevicePlatformType::GOOLE_ANDROID;
	if (str.Compare(TEXT("APPLE_IOS"), ESearchCase::IgnoreCase) == 0)	return DevicePlatformType::APPLE_IOS;
	if (str.Compare(TEXT("ETC"), ESearchCase::IgnoreCase) == 0)	return DevicePlatformType::ETC;
	return DevicePlatformType::MAX;
}
inline UserStatus UserStatusStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("USER_STATE_NORMAL"), ESearchCase::IgnoreCase) == 0)	return UserStatus::USER_STATE_NORMAL;
	if (str.Compare(TEXT("USER_STATE_COMBAT"), ESearchCase::IgnoreCase) == 0)	return UserStatus::USER_STATE_COMBAT;
	if (str.Compare(TEXT("USER_STATE_CAMP_INTERACT"), ESearchCase::IgnoreCase) == 0)	return UserStatus::USER_STATE_CAMP_INTERACT;
	if (str.Compare(TEXT("USER_STATE_PROP_INTERACT"), ESearchCase::IgnoreCase) == 0)	return UserStatus::USER_STATE_PROP_INTERACT;
	return UserStatus::MAX;
}
inline HiveNotificationType HiveNotificationTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("TAKE_DAMAGE_IN_PVP"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::TAKE_DAMAGE_IN_PVP;
	if (str.Compare(TEXT("DEAD"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::DEAD;
	if (str.Compare(TEXT("WEIGHT_OVER"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::WEIGHT_OVER;
	if (str.Compare(TEXT("FINISH_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::FINISH_DUNGEON;
	if (str.Compare(TEXT("FINISH_OFFLINE_PLAY"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::FINISH_OFFLINE_PLAY;
	if (str.Compare(TEXT("FULL_INVENTORY"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::FULL_INVENTORY;
	if (str.Compare(TEXT("GET_EQUIP_ITEM"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::GET_EQUIP_ITEM;
	if (str.Compare(TEXT("GET_SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::GET_SKILL_BOOK;
	if (str.Compare(TEXT("MARKET_ITEM_SOLD"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::MARKET_ITEM_SOLD;
	if (str.Compare(TEXT("SERVER_MIGRATE"), ESearchCase::IgnoreCase) == 0)	return HiveNotificationType::SERVER_MIGRATE;
	return HiveNotificationType::MAX;
}
inline EnterWorldServerType EnterWorldServerTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return EnterWorldServerType::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return EnterWorldServerType::NORMAL;
	if (str.Compare(TEXT("INTER_SERVER"), ESearchCase::IgnoreCase) == 0)	return EnterWorldServerType::INTER_SERVER;
	if (str.Compare(TEXT("INVADE_SERVER"), ESearchCase::IgnoreCase) == 0)	return EnterWorldServerType::INVADE_SERVER;
	return EnterWorldServerType::MAX;
}
inline RewardGiveType RewardGiveTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return RewardGiveType::NONE;
	if (str.Compare(TEXT("FILED_DROP"), ESearchCase::IgnoreCase) == 0)	return RewardGiveType::FILED_DROP;
	if (str.Compare(TEXT("INVENTORY"), ESearchCase::IgnoreCase) == 0)	return RewardGiveType::INVENTORY;
	if (str.Compare(TEXT("MAIL"), ESearchCase::IgnoreCase) == 0)	return RewardGiveType::MAIL;
	return RewardGiveType::MAX;
}
inline PartyType PartyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return PartyType::NORMAL;
	if (str.Compare(TEXT("COOP"), ESearchCase::IgnoreCase) == 0)	return PartyType::COOP;
	if (str.Compare(TEXT("INTER"), ESearchCase::IgnoreCase) == 0)	return PartyType::INTER;
	return PartyType::MAX;
}
inline DungeonCategory DungeonCategoryStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return DungeonCategory::NORMAL;
	if (str.Compare(TEXT("SPECIAL"), ESearchCase::IgnoreCase) == 0)	return DungeonCategory::SPECIAL;
	if (str.Compare(TEXT("PARTY"), ESearchCase::IgnoreCase) == 0)	return DungeonCategory::PARTY;
	if (str.Compare(TEXT("ELITE"), ESearchCase::IgnoreCase) == 0)	return DungeonCategory::ELITE;
	if (str.Compare(TEXT("ARENA"), ESearchCase::IgnoreCase) == 0)	return DungeonCategory::ARENA;
	return DungeonCategory::MAX;
}
inline EnterWorldFailReason EnterWorldFailReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("EWFR_NONE"), ESearchCase::IgnoreCase) == 0)	return EnterWorldFailReason::EWFR_NONE;
	if (str.Compare(TEXT("EWFR_INVALID_MAP"), ESearchCase::IgnoreCase) == 0)	return EnterWorldFailReason::EWFR_INVALID_MAP;
	if (str.Compare(TEXT("EWFR_INAVLID_CHANNEL"), ESearchCase::IgnoreCase) == 0)	return EnterWorldFailReason::EWFR_INAVLID_CHANNEL;
	if (str.Compare(TEXT("EWFR_LOGIN_INVALID_GUILD_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return EnterWorldFailReason::EWFR_LOGIN_INVALID_GUILD_DUNGEON;
	if (str.Compare(TEXT("EWFR_FAILED_DISCONNECTED"), ESearchCase::IgnoreCase) == 0)	return EnterWorldFailReason::EWFR_FAILED_DISCONNECTED;
	if (str.Compare(TEXT("EWFR_FAILED_COMPLETE"), ESearchCase::IgnoreCase) == 0)	return EnterWorldFailReason::EWFR_FAILED_COMPLETE;
	if (str.Compare(TEXT("EWFR_INVALID_WORLD_BOSS"), ESearchCase::IgnoreCase) == 0)	return EnterWorldFailReason::EWFR_INVALID_WORLD_BOSS;
	if (str.Compare(TEXT("EWFR_DUPLICATION_USER"), ESearchCase::IgnoreCase) == 0)	return EnterWorldFailReason::EWFR_DUPLICATION_USER;
	return EnterWorldFailReason::EWFR_DUPLICATION_USER;
}
inline DuelResult DuelResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return DuelResult::NONE;
	if (str.Compare(TEXT("WIN"), ESearchCase::IgnoreCase) == 0)	return DuelResult::WIN;
	if (str.Compare(TEXT("LOSE"), ESearchCase::IgnoreCase) == 0)	return DuelResult::LOSE;
	return DuelResult::LOSE;
}
inline HiveChannel HiveChannelStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("C2S"), ESearchCase::IgnoreCase) == 0)	return HiveChannel::C2S;
	if (str.Compare(TEXT("KAK"), ESearchCase::IgnoreCase) == 0)	return HiveChannel::KAK;
	if (str.Compare(TEXT("LIN"), ESearchCase::IgnoreCase) == 0)	return HiveChannel::LIN;
	if (str.Compare(TEXT("WEI"), ESearchCase::IgnoreCase) == 0)	return HiveChannel::WEI;
	if (str.Compare(TEXT("STE"), ESearchCase::IgnoreCase) == 0)	return HiveChannel::STE;
	return HiveChannel::STE;
}
inline HiveMarket HiveMarketStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("TS"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::TS;
	if (str.Compare(TEXT("OL"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::OL;
	if (str.Compare(TEXT("OZ"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::OZ;
	if (str.Compare(TEXT("AP"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::AP;
	if (str.Compare(TEXT("GO"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::GO;
	if (str.Compare(TEXT("SA"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::SA;
	if (str.Compare(TEXT("LE"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::LE;
	if (str.Compare(TEXT("MM"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::MM;
	if (str.Compare(TEXT("SN"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::SN;
	if (str.Compare(TEXT("QP"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::QP;
	if (str.Compare(TEXT("MO"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::MO;
	if (str.Compare(TEXT("DN"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::DN;
	if (str.Compare(TEXT("NA"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::NA;
	if (str.Compare(TEXT("AM"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::AM;
	if (str.Compare(TEXT("ON"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::ON;
	if (str.Compare(TEXT("FU"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::FU;
	if (str.Compare(TEXT("HU"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::HU;
	if (str.Compare(TEXT("OP"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::OP;
	if (str.Compare(TEXT("VI"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::VI;
	if (str.Compare(TEXT("XI"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::XI;
	if (str.Compare(TEXT("TC"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::TC;
	if (str.Compare(TEXT("HS"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::HS;
	if (str.Compare(TEXT("ST"), ESearchCase::IgnoreCase) == 0)	return HiveMarket::ST;
	return HiveMarket::ST;
}
inline DropCheckType DropCheckTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("MAP"), ESearchCase::IgnoreCase) == 0)	return DropCheckType::MAP;
	if (str.Compare(TEXT("RANK"), ESearchCase::IgnoreCase) == 0)	return DropCheckType::RANK;
	if (str.Compare(TEXT("RACE"), ESearchCase::IgnoreCase) == 0)	return DropCheckType::RACE;
	if (str.Compare(TEXT("ELEMENT"), ESearchCase::IgnoreCase) == 0)	return DropCheckType::ELEMENT;
	if (str.Compare(TEXT("INVADE"), ESearchCase::IgnoreCase) == 0)	return DropCheckType::INVADE;
	if (str.Compare(TEXT("SERVER"), ESearchCase::IgnoreCase) == 0)	return DropCheckType::SERVER;
	if (str.Compare(TEXT("NPC_FUNCTION_TYPE"), ESearchCase::IgnoreCase) == 0)	return DropCheckType::NPC_FUNCTION_TYPE;
	if (str.Compare(TEXT("CORRECTION"), ESearchCase::IgnoreCase) == 0)	return DropCheckType::CORRECTION;
	return DropCheckType::MAX;
}
inline InvadeDropType InvadeDropTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return InvadeDropType::NORMAL;
	if (str.Compare(TEXT("ATTACK"), ESearchCase::IgnoreCase) == 0)	return InvadeDropType::ATTACK;
	if (str.Compare(TEXT("DEFENSE"), ESearchCase::IgnoreCase) == 0)	return InvadeDropType::DEFENSE;
	return InvadeDropType::MAX;
}
inline CommonEffectType CommonEffectTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CommonEffectType::NONE;
	if (str.Compare(TEXT("SPAWN_INVADE_PORTAL"), ESearchCase::IgnoreCase) == 0)	return CommonEffectType::SPAWN_INVADE_PORTAL;
	if (str.Compare(TEXT("WARP_TO_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return CommonEffectType::WARP_TO_DUNGEON;
	if (str.Compare(TEXT("WARP_TO_INVADE"), ESearchCase::IgnoreCase) == 0)	return CommonEffectType::WARP_TO_INVADE;
	return CommonEffectType::MAX;
}
inline PKBookResult PKBookResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("KILL"), ESearchCase::IgnoreCase) == 0)	return PKBookResult::KILL;
	if (str.Compare(TEXT("DIE"), ESearchCase::IgnoreCase) == 0)	return PKBookResult::DIE;
	if (str.Compare(TEXT("ASSIST"), ESearchCase::IgnoreCase) == 0)	return PKBookResult::ASSIST;
	return PKBookResult::MAX;
}
inline BookMarkType BookMarkTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("POSITION"), ESearchCase::IgnoreCase) == 0)	return BookMarkType::POSITION;
	if (str.Compare(TEXT("ITEM_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return BookMarkType::ITEM_COLLECTION;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return BookMarkType::GUILD_ITEM_COLLECTION;
	if (str.Compare(TEXT("MARKET"), ESearchCase::IgnoreCase) == 0)	return BookMarkType::MARKET;
	if (str.Compare(TEXT("FIELD_BOSS"), ESearchCase::IgnoreCase) == 0)	return BookMarkType::FIELD_BOSS;
	if (str.Compare(TEXT("CRAFT"), ESearchCase::IgnoreCase) == 0)	return BookMarkType::CRAFT;
	if (str.Compare(TEXT("STAT"), ESearchCase::IgnoreCase) == 0)	return BookMarkType::STAT;
	if (str.Compare(TEXT("WDMARKET"), ESearchCase::IgnoreCase) == 0)	return BookMarkType::WDMARKET;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID"), ESearchCase::IgnoreCase) == 0)	return BookMarkType::AGIT_DUNGEON_RAID;
	return BookMarkType::MAX;
}
inline PKContributorType PKContributorTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("USER"), ESearchCase::IgnoreCase) == 0)	return PKContributorType::USER;
	if (str.Compare(TEXT("NPC"), ESearchCase::IgnoreCase) == 0)	return PKContributorType::NPC;
	return PKContributorType::MAX;
}
inline PKBookAction PKBookActionStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("REVENGE"), ESearchCase::IgnoreCase) == 0)	return PKBookAction::REVENGE;
	if (str.Compare(TEXT("TEASE"), ESearchCase::IgnoreCase) == 0)	return PKBookAction::TEASE;
	return PKBookAction::MAX;
}
inline PKNotifyType PKNotifyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return PKNotifyType::NORMAL;
	if (str.Compare(TEXT("GUILD_WAR"), ESearchCase::IgnoreCase) == 0)	return PKNotifyType::GUILD_WAR;
	return PKNotifyType::MAX;
}
inline DeathPenaltyType DeathPenaltyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return DeathPenaltyType::NONE;
	if (str.Compare(TEXT("EXP"), ESearchCase::IgnoreCase) == 0)	return DeathPenaltyType::EXP;
	if (str.Compare(TEXT("ITEM"), ESearchCase::IgnoreCase) == 0)	return DeathPenaltyType::ITEM;
	return DeathPenaltyType::MAX;
}
inline PersonalTradingNoticeType PersonalTradingNoticeTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ACCEPTED"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::ACCEPTED;
	if (str.Compare(TEXT("REJECT"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::REJECT;
	if (str.Compare(TEXT("REQUEST"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::REQUEST;
	if (str.Compare(TEXT("ABORT"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::ABORT;
	if (str.Compare(TEXT("ABORT_INVALID_STATUS"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::ABORT_INVALID_STATUS;
	if (str.Compare(TEXT("TIME_OUT"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::TIME_OUT;
	if (str.Compare(TEXT("LOCK"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::LOCK;
	if (str.Compare(TEXT("UNLOCK"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::UNLOCK;
	if (str.Compare(TEXT("CONFIRM"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::CONFIRM;
	if (str.Compare(TEXT("SUCCESS"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingNoticeType::SUCCESS;
	return PersonalTradingNoticeType::MAX;
}
inline PersonalTradingStatus PersonalTradingStatusStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingStatus::NONE;
	if (str.Compare(TEXT("WAINTING"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingStatus::WAINTING;
	if (str.Compare(TEXT("UNLOCK"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingStatus::UNLOCK;
	if (str.Compare(TEXT("LOCK"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingStatus::LOCK;
	if (str.Compare(TEXT("CONFIRM"), ESearchCase::IgnoreCase) == 0)	return PersonalTradingStatus::CONFIRM;
	return PersonalTradingStatus::MAX;
}
inline SummonMileageActionType SummonMileageActionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SummonMileageActionType::NONE;
	if (str.Compare(TEXT("SUMMON_FAIRY"), ESearchCase::IgnoreCase) == 0)	return SummonMileageActionType::SUMMON_FAIRY;
	if (str.Compare(TEXT("SUMMON_COSTUME"), ESearchCase::IgnoreCase) == 0)	return SummonMileageActionType::SUMMON_COSTUME;
	if (str.Compare(TEXT("UNIQUE_SUMMON_FAIRY"), ESearchCase::IgnoreCase) == 0)	return SummonMileageActionType::UNIQUE_SUMMON_FAIRY;
	if (str.Compare(TEXT("UNIQUE_SUMMON_COSTUME"), ESearchCase::IgnoreCase) == 0)	return SummonMileageActionType::UNIQUE_SUMMON_COSTUME;
	return SummonMileageActionType::MAX;
}
inline SummonMileageDataType SummonMileageDataTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SummonMileageDataType::NONE;
	if (str.Compare(TEXT("LEGACY_DATA"), ESearchCase::IgnoreCase) == 0)	return SummonMileageDataType::LEGACY_DATA;
	if (str.Compare(TEXT("NORMAL_DATA"), ESearchCase::IgnoreCase) == 0)	return SummonMileageDataType::NORMAL_DATA;
	return SummonMileageDataType::MAX;
}
inline ServerMigrateType ServerMigrateTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateType::NONE;
	if (str.Compare(TEXT("ACCOUNT_ONLY"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateType::ACCOUNT_ONLY;
	if (str.Compare(TEXT("INCLUDE_GUILD_INFO"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateType::INCLUDE_GUILD_INFO;
	return ServerMigrateType::MAX;
}
inline ServerMigrateLimitStatus ServerMigrateLimitStatusStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateLimitStatus::NONE;
	if (str.Compare(TEXT("FREE"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateLimitStatus::FREE;
	if (str.Compare(TEXT("IMMINENT"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateLimitStatus::IMMINENT;
	if (str.Compare(TEXT("CLOSING"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateLimitStatus::CLOSING;
	return ServerMigrateLimitStatus::MAX;
}
inline ServerMigrateConditionType ServerMigrateConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::NONE;
	if (str.Compare(TEXT("SERVER_CHOICE"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::SERVER_CHOICE;
	if (str.Compare(TEXT("CHARACTER_LOCATION"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::CHARACTER_LOCATION;
	if (str.Compare(TEXT("CHARACTER_LEVEL"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::CHARACTER_LEVEL;
	if (str.Compare(TEXT("PARTY_JOINED"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::PARTY_JOINED;
	if (str.Compare(TEXT("GUILD_JOINED"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_JOINED;
	if (str.Compare(TEXT("GUILD_JOINED_SUB"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_JOINED_SUB;
	if (str.Compare(TEXT("GUILD_JOIN_APPLICATION"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_JOIN_APPLICATION;
	if (str.Compare(TEXT("GUILD_JOIN_APPLICATION_SUB"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_JOIN_APPLICATION_SUB;
	if (str.Compare(TEXT("RECOVERY_PENALTY"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::RECOVERY_PENALTY;
	if (str.Compare(TEXT("BMSHOP_STORAGE"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::BMSHOP_STORAGE;
	if (str.Compare(TEXT("MARKET_SELL"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::MARKET_SELL;
	if (str.Compare(TEXT("MARKET_CALCULATE"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::MARKET_CALCULATE;
	if (str.Compare(TEXT("MAIL"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::MAIL;
	if (str.Compare(TEXT("GUILD_LEVEL"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_LEVEL;
	if (str.Compare(TEXT("GUILD_ALLY"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_ALLY;
	if (str.Compare(TEXT("GUILD_ENEMY"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_ENEMY;
	if (str.Compare(TEXT("GUILD_DIVISION"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_DIVISION;
	if (str.Compare(TEXT("GUILD_AUCTION"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_AUCTION;
	if (str.Compare(TEXT("GUILD_AUCTION_WITHDRAW"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_AUCTION_WITHDRAW;
	if (str.Compare(TEXT("GUILD_CAMP"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_CAMP;
	if (str.Compare(TEXT("GUILD_MEMBER"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILD_MEMBER;
	if (str.Compare(TEXT("WORLDMARKET_SELL"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::WORLDMARKET_SELL;
	if (str.Compare(TEXT("WORLDMARKET_CALCULATE"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::WORLDMARKET_CALCULATE;
	if (str.Compare(TEXT("GUILDRAID_CLOSED"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILDRAID_CLOSED;
	if (str.Compare(TEXT("GUILDWAR_JOINED"), ESearchCase::IgnoreCase) == 0)	return ServerMigrateConditionType::GUILDWAR_JOINED;
	return ServerMigrateConditionType::MAX;
}
inline BattleArenaTier BattleArenaTierStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return BattleArenaTier::NONE;
	if (str.Compare(TEXT("CHAMPION"), ESearchCase::IgnoreCase) == 0)	return BattleArenaTier::CHAMPION;
	if (str.Compare(TEXT("MASTER"), ESearchCase::IgnoreCase) == 0)	return BattleArenaTier::MASTER;
	if (str.Compare(TEXT("DIAMOND"), ESearchCase::IgnoreCase) == 0)	return BattleArenaTier::DIAMOND;
	if (str.Compare(TEXT("GOLD"), ESearchCase::IgnoreCase) == 0)	return BattleArenaTier::GOLD;
	if (str.Compare(TEXT("SILVER"), ESearchCase::IgnoreCase) == 0)	return BattleArenaTier::SILVER;
	if (str.Compare(TEXT("BRONZE"), ESearchCase::IgnoreCase) == 0)	return BattleArenaTier::BRONZE;
	return BattleArenaTier::MAX;
}
inline ContentsBindingType ContentsBindingTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("USER"), ESearchCase::IgnoreCase) == 0)	return ContentsBindingType::USER;
	if (str.Compare(TEXT("ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return ContentsBindingType::ACCOUNT;
	return ContentsBindingType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString AccountGradeEnumToString(AccountGrade value)
{
	switch (value)
	{
		case AccountGrade::NORMAL: return TEXT("NORMAL");
		case AccountGrade::OPERATOR: return TEXT("OPERATOR");
		case AccountGrade::QATESTER: return TEXT("QATESTER");
		case AccountGrade::ADMIN: return TEXT("ADMIN");
		case AccountGrade::GM: return TEXT("GM");
	}
	return FString();
}
inline FString ServerCongestionEnumToString(ServerCongestion value)
{
	switch (value)
	{
		case ServerCongestion::ON_STANBY: return TEXT("ON_STANBY");
		case ServerCongestion::GOOD: return TEXT("GOOD");
		case ServerCongestion::CONGESTED: return TEXT("CONGESTED");
		case ServerCongestion::FULL: return TEXT("FULL");
		case ServerCongestion::RESTRICTED_ACCESS: return TEXT("RESTRICTED_ACCESS");
		case ServerCongestion::BLOCK: return TEXT("BLOCK");
		case ServerCongestion::NOT_CONNECTABLE: return TEXT("NOT_CONNECTABLE");
	}
	return FString();
}
inline FString MapTypeEnumToString(MapType value)
{
	switch (value)
	{
		case MapType::NONE: return TEXT("NONE");
		case MapType::PUBLIC_FIELD: return TEXT("PUBLIC_FIELD");
		case MapType::SINGLE_FIELD: return TEXT("SINGLE_FIELD");
		case MapType::INSTANCE_SINGLE: return TEXT("INSTANCE_SINGLE");
		case MapType::INSTANCE_FIELD: return TEXT("INSTANCE_FIELD");
		case MapType::PUBLIC_DUNGEON: return TEXT("PUBLIC_DUNGEON");
		case MapType::GUILD_DUNGEON: return TEXT("GUILD_DUNGEON");
		case MapType::PARTY_DUNGEON: return TEXT("PARTY_DUNGEON");
		case MapType::ELITE_DUNGEON_1: return TEXT("ELITE_DUNGEON_1");
		case MapType::WORLD_BOSS: return TEXT("WORLD_BOSS");
		case MapType::INTER_FIELD: return TEXT("INTER_FIELD");
		case MapType::GUILD_WAR: return TEXT("GUILD_WAR");
		case MapType::AGIT_DUNGEON_RAID: return TEXT("AGIT_DUNGEON_RAID");
		case MapType::AGIT: return TEXT("AGIT");
	}
	return FString();
}
inline FString MapContentsTypeEnumToString(MapContentsType value)
{
	switch (value)
	{
		case MapContentsType::NORMAL: return TEXT("NORMAL");
		case MapContentsType::BATTLE: return TEXT("BATTLE");
		case MapContentsType::OUTLAW: return TEXT("OUTLAW");
		case MapContentsType::SPACE_CRACK: return TEXT("SPACE_CRACK");
	}
	return FString();
}
inline FString WarpTypeEnumToString(WarpType value)
{
	switch (value)
	{
		case WarpType::Immediately: return TEXT("Immediately");
		case WarpType::YesNoDialog: return TEXT("YesNoDialog");
	}
	return FString();
}
inline FString TerritoryTypeEnumToString(TerritoryType value)
{
	switch (value)
	{
		case TerritoryType::NONE: return TEXT("NONE");
		case TerritoryType::TOWN: return TEXT("TOWN");
		case TerritoryType::SAFE_ZONE: return TEXT("SAFE_ZONE");
		case TerritoryType::ARENA_READY: return TEXT("ARENA_READY");
		case TerritoryType::ARENA: return TEXT("ARENA");
		case TerritoryType::ARRIVAL: return TEXT("ARRIVAL");
		case TerritoryType::Env: return TEXT("Env");
		case TerritoryType::SOUND: return TEXT("SOUND");
		case TerritoryType::SPACE_CRACK: return TEXT("SPACE_CRACK");
		case TerritoryType::TOWN_CHAOS: return TEXT("TOWN_CHAOS");
		case TerritoryType::NO_CAMP_ZONE: return TEXT("NO_CAMP_ZONE");
		case TerritoryType::NON_PK: return TEXT("NON_PK");
	}
	return FString();
}
inline FString TerritoryCheckTypeEnumToString(TerritoryCheckType value)
{
	switch (value)
	{
		case TerritoryCheckType::NONE: return TEXT("NONE");
		case TerritoryCheckType::INVADE: return TEXT("INVADE");
	}
	return FString();
}
inline FString ChannelCongestionStatusEnumToString(ChannelCongestionStatus value)
{
	switch (value)
	{
		case ChannelCongestionStatus::LOW: return TEXT("LOW");
		case ChannelCongestionStatus::MEDIUM: return TEXT("MEDIUM");
		case ChannelCongestionStatus::HIGH: return TEXT("HIGH");
		case ChannelCongestionStatus::FULL: return TEXT("FULL");
	}
	return FString();
}
inline FString WarpReasonEnumToString(WarpReason value)
{
	switch (value)
	{
		case WarpReason::ENTER_WORLD: return TEXT("ENTER_WORLD");
		case WarpReason::CHANGE_CHANNEL: return TEXT("CHANGE_CHANNEL");
		case WarpReason::PORTAL: return TEXT("PORTAL");
		case WarpReason::PARTY_MATCHING: return TEXT("PARTY_MATCHING");
		case WarpReason::RESURRECT: return TEXT("RESURRECT");
		case WarpReason::ADMIN_COMMAND: return TEXT("ADMIN_COMMAND");
		case WarpReason::QUEST: return TEXT("QUEST");
		case WarpReason::EVENT_ACTION_TO_INSTANCE_SINGLE: return TEXT("EVENT_ACTION_TO_INSTANCE_SINGLE");
		case WarpReason::LEAVE_INSTANCE_SINGLE: return TEXT("LEAVE_INSTANCE_SINGLE");
		case WarpReason::EVENT_ACTION_RETURN_TO_PUBLIC: return TEXT("EVENT_ACTION_RETURN_TO_PUBLIC");
		case WarpReason::EVENT_ACTION_TELEPORT: return TEXT("EVENT_ACTION_TELEPORT");
		case WarpReason::BOOKMARK: return TEXT("BOOKMARK");
		case WarpReason::USE_WARP_TO_TOWN_SCROLL: return TEXT("USE_WARP_TO_TOWN_SCROLL");
		case WarpReason::USE_WARP_TO_RANDOM_SCROLL: return TEXT("USE_WARP_TO_RANDOM_SCROLL");
		case WarpReason::DUNGEON_TIME_OVER: return TEXT("DUNGEON_TIME_OVER");
		case WarpReason::MAP_EVENT: return TEXT("MAP_EVENT");
		case WarpReason::BOOKMARK_SHARED: return TEXT("BOOKMARK_SHARED");
		case WarpReason::PARTY_LEADER: return TEXT("PARTY_LEADER");
		case WarpReason::SPACE_CRACK: return TEXT("SPACE_CRACK");
		case WarpReason::WORLD_MAP_PORTAL: return TEXT("WORLD_MAP_PORTAL");
		case WarpReason::EVENT_DUNGEON_TIME_OVER: return TEXT("EVENT_DUNGEON_TIME_OVER");
		case WarpReason::ENTER_GUILD_DUNGEON: return TEXT("ENTER_GUILD_DUNGEON");
		case WarpReason::LEAVE_GUILD_DUNGEON: return TEXT("LEAVE_GUILD_DUNGEON");
		case WarpReason::GUILD_CAMP: return TEXT("GUILD_CAMP");
		case WarpReason::INVADE: return TEXT("INVADE");
		case WarpReason::INVADE_LEAVE: return TEXT("INVADE_LEAVE");
		case WarpReason::INVADE_TIME_OVER: return TEXT("INVADE_TIME_OVER");
		case WarpReason::INVADE_USER_TIME_OVER: return TEXT("INVADE_USER_TIME_OVER");
		case WarpReason::PARTY_DUNGEON: return TEXT("PARTY_DUNGEON");
		case WarpReason::PARTY_DUNGEON_LEAVE: return TEXT("PARTY_DUNGEON_LEAVE");
		case WarpReason::PARTY_DUNGEON_TIME_OVER: return TEXT("PARTY_DUNGEON_TIME_OVER");
		case WarpReason::ELITE_DUNGEON: return TEXT("ELITE_DUNGEON");
		case WarpReason::ELITE_DUNGEON_LEAVE: return TEXT("ELITE_DUNGEON_LEAVE");
		case WarpReason::ELITE_DUNGEON_TIME_OVER: return TEXT("ELITE_DUNGEON_TIME_OVER");
		case WarpReason::ARENA: return TEXT("ARENA");
		case WarpReason::ARENA_LEAVE: return TEXT("ARENA_LEAVE");
		case WarpReason::ITEM_TREASURE_MONSTER: return TEXT("ITEM_TREASURE_MONSTER");
		case WarpReason::REVENGE: return TEXT("REVENGE");
		case WarpReason::INVADE_USE_SCROLL_LEAVE: return TEXT("INVADE_USE_SCROLL_LEAVE");
		case WarpReason::WORLD_BOSS: return TEXT("WORLD_BOSS");
		case WarpReason::WORLD_BOSS_TIME_OUT: return TEXT("WORLD_BOSS_TIME_OUT");
		case WarpReason::WORLD_BOSS_LEAVE: return TEXT("WORLD_BOSS_LEAVE");
		case WarpReason::DESTROYED_THEN_RETURN_PUBLIC_FIELD: return TEXT("DESTROYED_THEN_RETURN_PUBLIC_FIELD");
		case WarpReason::WORLD_MAP_PORTAL_TO_BOSS: return TEXT("WORLD_MAP_PORTAL_TO_BOSS");
		case WarpReason::ELITE_DUNGEON_HIDDEN: return TEXT("ELITE_DUNGEON_HIDDEN");
		case WarpReason::USE_DUNGEON_TICKET: return TEXT("USE_DUNGEON_TICKET");
		case WarpReason::GUILD_WAR: return TEXT("GUILD_WAR");
		case WarpReason::GUILD_WAR_TIME_OUT: return TEXT("GUILD_WAR_TIME_OUT");
		case WarpReason::GUILD_WAR_LEAVE: return TEXT("GUILD_WAR_LEAVE");
		case WarpReason::SKILL_NOTIFY: return TEXT("SKILL_NOTIFY");
		case WarpReason::ENTER_AGIT_DUNGEON_RAID: return TEXT("ENTER_AGIT_DUNGEON_RAID");
		case WarpReason::LEAVE_SELF_AGIT_DUNGEON_RAID: return TEXT("LEAVE_SELF_AGIT_DUNGEON_RAID");
		case WarpReason::ENTER_AGIT: return TEXT("ENTER_AGIT");
		case WarpReason::LEAVE_AGIT: return TEXT("LEAVE_AGIT");
		case WarpReason::LEAVE_AGIT_RANDOM_SCROLL: return TEXT("LEAVE_AGIT_RANDOM_SCROLL");
		case WarpReason::LEAVE_FORCE_AGIT_DUNGEON_RAID: return TEXT("LEAVE_FORCE_AGIT_DUNGEON_RAID");
	}
	return FString();
}
inline FString CommandTypeEnumToString(CommandType value)
{
	switch (value)
	{
		case CommandType::COMMAND_OPERATING: return TEXT("COMMAND_OPERATING");
		case CommandType::COMMAND_QA: return TEXT("COMMAND_QA");
	}
	return FString();
}
inline FString CommandHandlerTypeEnumToString(CommandHandlerType value)
{
	switch (value)
	{
		case CommandHandlerType::HANDLER_OPERATING_GATEWAY: return TEXT("HANDLER_OPERATING_GATEWAY");
		case CommandHandlerType::HANDLER_OPERATING_WORLD: return TEXT("HANDLER_OPERATING_WORLD");
		case CommandHandlerType::HANDLER_QA_GATEWAY: return TEXT("HANDLER_QA_GATEWAY");
		case CommandHandlerType::HANDLER_QA_WORLD: return TEXT("HANDLER_QA_WORLD");
		case CommandHandlerType::HANDLER_RANK_GATEWAY: return TEXT("HANDLER_RANK_GATEWAY");
		case CommandHandlerType::HANDLER_HIVE_ITEM_GATEWAY: return TEXT("HANDLER_HIVE_ITEM_GATEWAY");
		case CommandHandlerType::HANDLER_OPERATING_AUTH: return TEXT("HANDLER_OPERATING_AUTH");
		case CommandHandlerType::HANDLER_OPERATING_DIRECTORY: return TEXT("HANDLER_OPERATING_DIRECTORY");
		case CommandHandlerType::HANDLER_SERVER_TO_SERVER_GATEWAY: return TEXT("HANDLER_SERVER_TO_SERVER_GATEWAY");
	}
	return FString();
}
inline FString TimeTicketTypeEnumToString(TimeTicketType value)
{
	switch (value)
	{
		case TimeTicketType::TIME_TICKET_EXP_DEATH_PENALTY_FREE: return TEXT("TIME_TICKET_EXP_DEATH_PENALTY_FREE");
		case TimeTicketType::TIME_TICKET_ITEM_DEATH_PENALTY_FREE: return TEXT("TIME_TICKET_ITEM_DEATH_PENALTY_FREE");
	}
	return FString();
}
inline FString CurrencyTypeEnumToString(CurrencyType value)
{
	switch (value)
	{
		case CurrencyType::NONE: return TEXT("NONE");
		case CurrencyType::DIA: return TEXT("DIA");
		case CurrencyType::FREE_DIA: return TEXT("FREE_DIA");
		case CurrencyType::MILEAGE: return TEXT("MILEAGE");
		case CurrencyType::GOLD: return TEXT("GOLD");
		case CurrencyType::CASH_MILEAGE: return TEXT("CASH_MILEAGE");
		case CurrencyType::MONSTER_BOOK_POINT: return TEXT("MONSTER_BOOK_POINT");
		case CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE: return TEXT("SUMMON_UNIQUE_FAIRY_MILEAGE");
		case CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE: return TEXT("SUMMON_UNIQUE_COSTUME_MILEAGE");
		case CurrencyType::MAX_ACCOUNT_TYPE: return TEXT("MAX_ACCOUNT_TYPE");
		case CurrencyType::START_USER_TYPE: return TEXT("START_USER_TYPE");
		case CurrencyType::EXP_DEATH_PENALTY_FREE: return TEXT("EXP_DEATH_PENALTY_FREE");
		case CurrencyType::ITEM_DEATH_PENALTY_FREE: return TEXT("ITEM_DEATH_PENALTY_FREE");
		case CurrencyType::DEATH_EXP_RECOVERY: return TEXT("DEATH_EXP_RECOVERY");
		case CurrencyType::SUMMON_FAIRY_MILEAGE: return TEXT("SUMMON_FAIRY_MILEAGE");
		case CurrencyType::SUMMON_COSTUME_MILEAGE: return TEXT("SUMMON_COSTUME_MILEAGE");
		case CurrencyType::MAX_USER_TYPE: return TEXT("MAX_USER_TYPE");
		case CurrencyType::START_GUILD_TYPE: return TEXT("START_GUILD_TYPE");
		case CurrencyType::GUILD_ASSET: return TEXT("GUILD_ASSET");
		case CurrencyType::MAX_GUILD_TYPE: return TEXT("MAX_GUILD_TYPE");
		case CurrencyType::MAX_IN_GAME_TYPE: return TEXT("MAX_IN_GAME_TYPE");
		case CurrencyType::CASH: return TEXT("CASH");
	}
	return FString();
}
inline FString RewardTypeEnumToString(RewardType value)
{
	switch (value)
	{
		case RewardType::ALL: return TEXT("ALL");
		case RewardType::PICK_ONE: return TEXT("PICK_ONE");
		case RewardType::RANDOM_ONE: return TEXT("RANDOM_ONE");
		case RewardType::RANDOM_GROUP: return TEXT("RANDOM_GROUP");
		case RewardType::PICK_REWARD_BOX_LIST: return TEXT("PICK_REWARD_BOX_LIST");
		case RewardType::SUB_PICK_ONE: return TEXT("SUB_PICK_ONE");
		case RewardType::SUB_RANDOM_ONE: return TEXT("SUB_RANDOM_ONE");
	}
	return FString();
}
inline FString WeightPenaltyGradeEnumToString(WeightPenaltyGrade value)
{
	switch (value)
	{
		case WeightPenaltyGrade::NONE: return TEXT("NONE");
		case WeightPenaltyGrade::FIRST: return TEXT("FIRST");
		case WeightPenaltyGrade::SECOND: return TEXT("SECOND");
	}
	return FString();
}
inline FString PartyDropOwnershipTypeEnumToString(PartyDropOwnershipType value)
{
	switch (value)
	{
		case PartyDropOwnershipType::RANDOM: return TEXT("RANDOM");
		case PartyDropOwnershipType::SEQUENCE: return TEXT("SEQUENCE");
		case PartyDropOwnershipType::FREE: return TEXT("FREE");
	}
	return FString();
}
inline FString RestoreTypeEnumToString(RestoreType value)
{
	switch (value)
	{
		case RestoreType::Free: return TEXT("Free");
		case RestoreType::Currency: return TEXT("Currency");
	}
	return FString();
}
inline FString ResetTimeTicketEnumToString(ResetTimeTicket value)
{
	switch (value)
	{
		case ResetTimeTicket::RTT_NONE: return TEXT("RTT_NONE");
		case ResetTimeTicket::RTT_DAILY: return TEXT("RTT_DAILY");
		case ResetTimeTicket::RTT_WEEKLY: return TEXT("RTT_WEEKLY");
		case ResetTimeTicket::RTT_MONTHLY: return TEXT("RTT_MONTHLY");
		case ResetTimeTicket::RTT_DUNGEON: return TEXT("RTT_DUNGEON");
		case ResetTimeTicket::RTT_SPACEC_CRACK: return TEXT("RTT_SPACEC_CRACK");
		case ResetTimeTicket::RTT_OFF_LINE_MODE: return TEXT("RTT_OFF_LINE_MODE");
		case ResetTimeTicket::RTT_DEATH_RESURRECTION: return TEXT("RTT_DEATH_RESURRECTION");
		case ResetTimeTicket::RTT_GUILD_QUEST: return TEXT("RTT_GUILD_QUEST");
		case ResetTimeTicket::RTT_GUILD_DUNGEON: return TEXT("RTT_GUILD_DUNGEON");
		case ResetTimeTicket::RTT_INVADE_START: return TEXT("RTT_INVADE_START");
		case ResetTimeTicket::RTT_INVADE_END: return TEXT("RTT_INVADE_END");
		case ResetTimeTicket::RTT_INVADE_RESET: return TEXT("RTT_INVADE_RESET");
		case ResetTimeTicket::RTT_PARTY_DUNGEON_RESET: return TEXT("RTT_PARTY_DUNGEON_RESET");
		case ResetTimeTicket::RTT_GUILD_ZEAL_POINT_RESET: return TEXT("RTT_GUILD_ZEAL_POINT_RESET");
		case ResetTimeTicket::RTT_FREE_INVADE_CHAT_RESET: return TEXT("RTT_FREE_INVADE_CHAT_RESET");
		case ResetTimeTicket::RTT_CALCUATION_TAX: return TEXT("RTT_CALCUATION_TAX");
		case ResetTimeTicket::RTT_BM_DAILY: return TEXT("RTT_BM_DAILY");
		case ResetTimeTicket::RTT_BM_WEEKLY: return TEXT("RTT_BM_WEEKLY");
		case ResetTimeTicket::RTT_BM_MONTHLY: return TEXT("RTT_BM_MONTHLY");
		case ResetTimeTicket::RTT_ATTENDANCE_DAILY: return TEXT("RTT_ATTENDANCE_DAILY");
		case ResetTimeTicket::RTT_PASS_DAILY: return TEXT("RTT_PASS_DAILY");
		case ResetTimeTicket::RTT_PASS_WEEKLY: return TEXT("RTT_PASS_WEEKLY");
		case ResetTimeTicket::RTT_AGIT_DUNGEON_RAID: return TEXT("RTT_AGIT_DUNGEON_RAID");
	}
	return FString();
}
inline FString CostTypeEnumToString(CostType value)
{
	switch (value)
	{
		case CostType::CURRENCY: return TEXT("CURRENCY");
		case CostType::ITEM: return TEXT("ITEM");
	}
	return FString();
}
inline FString ItemStorageTypeEnumToString(ItemStorageType value)
{
	switch (value)
	{
		case ItemStorageType::INVENTORY: return TEXT("INVENTORY");
		case ItemStorageType::DEPOT: return TEXT("DEPOT");
		case ItemStorageType::DEATH: return TEXT("DEATH");
		case ItemStorageType::MARKET: return TEXT("MARKET");
		case ItemStorageType::GUILD_STORE: return TEXT("GUILD_STORE");
		case ItemStorageType::MAIL: return TEXT("MAIL");
		case ItemStorageType::WDMARKET: return TEXT("WDMARKET");
	}
	return FString();
}
inline FString MultiLevelRankSlotStateEnumToString(MultiLevelRankSlotState value)
{
	switch (value)
	{
		case MultiLevelRankSlotState::LOCK: return TEXT("LOCK");
		case MultiLevelRankSlotState::UNLOCK: return TEXT("UNLOCK");
		case MultiLevelRankSlotState::IN_PROGRESS: return TEXT("IN_PROGRESS");
		case MultiLevelRankSlotState::FINISH: return TEXT("FINISH");
		case MultiLevelRankSlotState::ACTIVE: return TEXT("ACTIVE");
	}
	return FString();
}
inline FString PurchaseLimitBuyerTypeEnumToString(PurchaseLimitBuyerType value)
{
	switch (value)
	{
		case PurchaseLimitBuyerType::NONE: return TEXT("NONE");
		case PurchaseLimitBuyerType::CHARACTER: return TEXT("CHARACTER");
		case PurchaseLimitBuyerType::SERVER: return TEXT("SERVER");
		case PurchaseLimitBuyerType::ACCOUNT: return TEXT("ACCOUNT");
	}
	return FString();
}
inline FString BMPurchaseLimitBuyerTypeEnumToString(BMPurchaseLimitBuyerType value)
{
	switch (value)
	{
		case BMPurchaseLimitBuyerType::NONE: return TEXT("NONE");
		case BMPurchaseLimitBuyerType::SERVER: return TEXT("SERVER");
		case BMPurchaseLimitBuyerType::ACCOUNT: return TEXT("ACCOUNT");
	}
	return FString();
}
inline FString ContentsTypeEnumToString(ContentsType value)
{
	switch (value)
	{
		case ContentsType::ATTENDANCE: return TEXT("ATTENDANCE");
		case ContentsType::GUILD_STORE_ACQUIRED_ITEM: return TEXT("GUILD_STORE_ACQUIRED_ITEM");
		case ContentsType::GUILD_STORE_DISTRIBUTED_ITEM: return TEXT("GUILD_STORE_DISTRIBUTED_ITEM");
		case ContentsType::ELITE_DUNGEON_1: return TEXT("ELITE_DUNGEON_1");
		case ContentsType::CODEX: return TEXT("CODEX");
	}
	return FString();
}
inline FString ContentsStateEnumToString(ContentsState value)
{
	switch (value)
	{
		case ContentsState::RED_DOT: return TEXT("RED_DOT");
		case ContentsState::VISIBLE: return TEXT("VISIBLE");
		case ContentsState::HIDDEN: return TEXT("HIDDEN");
	}
	return FString();
}
inline FString CanItemDropEnumToString(CanItemDrop value)
{
	switch (value)
	{
		case CanItemDrop::DEFAULT: return TEXT("DEFAULT");
		case CanItemDrop::DROP: return TEXT("DROP");
		case CanItemDrop::NO_DROP: return TEXT("NO_DROP");
	}
	return FString();
}
inline FString AutoDropItemGradeEnumToString(AutoDropItemGrade value)
{
	switch (value)
	{
		case AutoDropItemGrade::ALL: return TEXT("ALL");
		case AutoDropItemGrade::MAGIC: return TEXT("MAGIC");
		case AutoDropItemGrade::RARE: return TEXT("RARE");
		case AutoDropItemGrade::OFF: return TEXT("OFF");
	}
	return FString();
}
inline FString AutoAttackRangeEnumToString(AutoAttackRange value)
{
	switch (value)
	{
		case AutoAttackRange::METER_20: return TEXT("METER_20");
		case AutoAttackRange::METER_30: return TEXT("METER_30");
		case AutoAttackRange::METER_50: return TEXT("METER_50");
		case AutoAttackRange::NO_LIMIT: return TEXT("NO_LIMIT");
	}
	return FString();
}
inline FString MannerModeEnumToString(MannerMode value)
{
	switch (value)
	{
		case MannerMode::ON: return TEXT("ON");
		case MannerMode::EXCEPT_PARTY_GUILD: return TEXT("EXCEPT_PARTY_GUILD");
		case MannerMode::OFF: return TEXT("OFF");
	}
	return FString();
}
inline FString DevicePlatformTypeEnumToString(DevicePlatformType value)
{
	switch (value)
	{
		case DevicePlatformType::NONE: return TEXT("NONE");
		case DevicePlatformType::WINDOWS: return TEXT("WINDOWS");
		case DevicePlatformType::EDITOR: return TEXT("EDITOR");
		case DevicePlatformType::GOOLE_ANDROID: return TEXT("GOOLE_ANDROID");
		case DevicePlatformType::APPLE_IOS: return TEXT("APPLE_IOS");
		case DevicePlatformType::ETC: return TEXT("ETC");
	}
	return FString();
}
inline FString UserStatusEnumToString(UserStatus value)
{
	switch (value)
	{
		case UserStatus::USER_STATE_NORMAL: return TEXT("USER_STATE_NORMAL");
		case UserStatus::USER_STATE_COMBAT: return TEXT("USER_STATE_COMBAT");
		case UserStatus::USER_STATE_CAMP_INTERACT: return TEXT("USER_STATE_CAMP_INTERACT");
		case UserStatus::USER_STATE_PROP_INTERACT: return TEXT("USER_STATE_PROP_INTERACT");
	}
	return FString();
}
inline FString HiveNotificationTypeEnumToString(HiveNotificationType value)
{
	switch (value)
	{
		case HiveNotificationType::TAKE_DAMAGE_IN_PVP: return TEXT("TAKE_DAMAGE_IN_PVP");
		case HiveNotificationType::DEAD: return TEXT("DEAD");
		case HiveNotificationType::WEIGHT_OVER: return TEXT("WEIGHT_OVER");
		case HiveNotificationType::FINISH_DUNGEON: return TEXT("FINISH_DUNGEON");
		case HiveNotificationType::FINISH_OFFLINE_PLAY: return TEXT("FINISH_OFFLINE_PLAY");
		case HiveNotificationType::FULL_INVENTORY: return TEXT("FULL_INVENTORY");
		case HiveNotificationType::GET_EQUIP_ITEM: return TEXT("GET_EQUIP_ITEM");
		case HiveNotificationType::GET_SKILL_BOOK: return TEXT("GET_SKILL_BOOK");
		case HiveNotificationType::MARKET_ITEM_SOLD: return TEXT("MARKET_ITEM_SOLD");
		case HiveNotificationType::SERVER_MIGRATE: return TEXT("SERVER_MIGRATE");
	}
	return FString();
}
inline FString EnterWorldServerTypeEnumToString(EnterWorldServerType value)
{
	switch (value)
	{
		case EnterWorldServerType::NONE: return TEXT("NONE");
		case EnterWorldServerType::NORMAL: return TEXT("NORMAL");
		case EnterWorldServerType::INTER_SERVER: return TEXT("INTER_SERVER");
		case EnterWorldServerType::INVADE_SERVER: return TEXT("INVADE_SERVER");
	}
	return FString();
}
inline FString RewardGiveTypeEnumToString(RewardGiveType value)
{
	switch (value)
	{
		case RewardGiveType::NONE: return TEXT("NONE");
		case RewardGiveType::FILED_DROP: return TEXT("FILED_DROP");
		case RewardGiveType::INVENTORY: return TEXT("INVENTORY");
		case RewardGiveType::MAIL: return TEXT("MAIL");
	}
	return FString();
}
inline FString PartyTypeEnumToString(PartyType value)
{
	switch (value)
	{
		case PartyType::NORMAL: return TEXT("NORMAL");
		case PartyType::COOP: return TEXT("COOP");
		case PartyType::INTER: return TEXT("INTER");
	}
	return FString();
}
inline FString DungeonCategoryEnumToString(DungeonCategory value)
{
	switch (value)
	{
		case DungeonCategory::NORMAL: return TEXT("NORMAL");
		case DungeonCategory::SPECIAL: return TEXT("SPECIAL");
		case DungeonCategory::PARTY: return TEXT("PARTY");
		case DungeonCategory::ELITE: return TEXT("ELITE");
		case DungeonCategory::ARENA: return TEXT("ARENA");
	}
	return FString();
}
inline FString EnterWorldFailReasonEnumToString(EnterWorldFailReason value)
{
	switch (value)
	{
		case EnterWorldFailReason::EWFR_NONE: return TEXT("EWFR_NONE");
		case EnterWorldFailReason::EWFR_INVALID_MAP: return TEXT("EWFR_INVALID_MAP");
		case EnterWorldFailReason::EWFR_INAVLID_CHANNEL: return TEXT("EWFR_INAVLID_CHANNEL");
		case EnterWorldFailReason::EWFR_LOGIN_INVALID_GUILD_DUNGEON: return TEXT("EWFR_LOGIN_INVALID_GUILD_DUNGEON");
		case EnterWorldFailReason::EWFR_FAILED_DISCONNECTED: return TEXT("EWFR_FAILED_DISCONNECTED");
		case EnterWorldFailReason::EWFR_FAILED_COMPLETE: return TEXT("EWFR_FAILED_COMPLETE");
		case EnterWorldFailReason::EWFR_INVALID_WORLD_BOSS: return TEXT("EWFR_INVALID_WORLD_BOSS");
		case EnterWorldFailReason::EWFR_DUPLICATION_USER: return TEXT("EWFR_DUPLICATION_USER");
	}
	return FString();
}
inline FString DuelResultEnumToString(DuelResult value)
{
	switch (value)
	{
		case DuelResult::NONE: return TEXT("NONE");
		case DuelResult::WIN: return TEXT("WIN");
		case DuelResult::LOSE: return TEXT("LOSE");
	}
	return FString();
}
inline FString HiveChannelEnumToString(HiveChannel value)
{
	switch (value)
	{
		case HiveChannel::C2S: return TEXT("C2S");
		case HiveChannel::KAK: return TEXT("KAK");
		case HiveChannel::LIN: return TEXT("LIN");
		case HiveChannel::WEI: return TEXT("WEI");
		case HiveChannel::STE: return TEXT("STE");
	}
	return FString();
}
inline FString HiveMarketEnumToString(HiveMarket value)
{
	switch (value)
	{
		case HiveMarket::TS: return TEXT("TS");
		case HiveMarket::OL: return TEXT("OL");
		case HiveMarket::OZ: return TEXT("OZ");
		case HiveMarket::AP: return TEXT("AP");
		case HiveMarket::GO: return TEXT("GO");
		case HiveMarket::SA: return TEXT("SA");
		case HiveMarket::LE: return TEXT("LE");
		case HiveMarket::MM: return TEXT("MM");
		case HiveMarket::SN: return TEXT("SN");
		case HiveMarket::QP: return TEXT("QP");
		case HiveMarket::MO: return TEXT("MO");
		case HiveMarket::DN: return TEXT("DN");
		case HiveMarket::NA: return TEXT("NA");
		case HiveMarket::AM: return TEXT("AM");
		case HiveMarket::ON: return TEXT("ON");
		case HiveMarket::FU: return TEXT("FU");
		case HiveMarket::HU: return TEXT("HU");
		case HiveMarket::OP: return TEXT("OP");
		case HiveMarket::VI: return TEXT("VI");
		case HiveMarket::XI: return TEXT("XI");
		case HiveMarket::TC: return TEXT("TC");
		case HiveMarket::HS: return TEXT("HS");
		case HiveMarket::ST: return TEXT("ST");
	}
	return FString();
}
inline FString DropCheckTypeEnumToString(DropCheckType value)
{
	switch (value)
	{
		case DropCheckType::MAP: return TEXT("MAP");
		case DropCheckType::RANK: return TEXT("RANK");
		case DropCheckType::RACE: return TEXT("RACE");
		case DropCheckType::ELEMENT: return TEXT("ELEMENT");
		case DropCheckType::INVADE: return TEXT("INVADE");
		case DropCheckType::SERVER: return TEXT("SERVER");
		case DropCheckType::NPC_FUNCTION_TYPE: return TEXT("NPC_FUNCTION_TYPE");
		case DropCheckType::CORRECTION: return TEXT("CORRECTION");
	}
	return FString();
}
inline FString InvadeDropTypeEnumToString(InvadeDropType value)
{
	switch (value)
	{
		case InvadeDropType::NORMAL: return TEXT("NORMAL");
		case InvadeDropType::ATTACK: return TEXT("ATTACK");
		case InvadeDropType::DEFENSE: return TEXT("DEFENSE");
	}
	return FString();
}
inline FString CommonEffectTypeEnumToString(CommonEffectType value)
{
	switch (value)
	{
		case CommonEffectType::NONE: return TEXT("NONE");
		case CommonEffectType::SPAWN_INVADE_PORTAL: return TEXT("SPAWN_INVADE_PORTAL");
		case CommonEffectType::WARP_TO_DUNGEON: return TEXT("WARP_TO_DUNGEON");
		case CommonEffectType::WARP_TO_INVADE: return TEXT("WARP_TO_INVADE");
	}
	return FString();
}
inline FString PKBookResultEnumToString(PKBookResult value)
{
	switch (value)
	{
		case PKBookResult::KILL: return TEXT("KILL");
		case PKBookResult::DIE: return TEXT("DIE");
		case PKBookResult::ASSIST: return TEXT("ASSIST");
	}
	return FString();
}
inline FString BookMarkTypeEnumToString(BookMarkType value)
{
	switch (value)
	{
		case BookMarkType::POSITION: return TEXT("POSITION");
		case BookMarkType::ITEM_COLLECTION: return TEXT("ITEM_COLLECTION");
		case BookMarkType::GUILD_ITEM_COLLECTION: return TEXT("GUILD_ITEM_COLLECTION");
		case BookMarkType::MARKET: return TEXT("MARKET");
		case BookMarkType::FIELD_BOSS: return TEXT("FIELD_BOSS");
		case BookMarkType::CRAFT: return TEXT("CRAFT");
		case BookMarkType::STAT: return TEXT("STAT");
		case BookMarkType::WDMARKET: return TEXT("WDMARKET");
		case BookMarkType::AGIT_DUNGEON_RAID: return TEXT("AGIT_DUNGEON_RAID");
	}
	return FString();
}
inline FString PKContributorTypeEnumToString(PKContributorType value)
{
	switch (value)
	{
		case PKContributorType::USER: return TEXT("USER");
		case PKContributorType::NPC: return TEXT("NPC");
	}
	return FString();
}
inline FString PKBookActionEnumToString(PKBookAction value)
{
	switch (value)
	{
		case PKBookAction::REVENGE: return TEXT("REVENGE");
		case PKBookAction::TEASE: return TEXT("TEASE");
	}
	return FString();
}
inline FString PKNotifyTypeEnumToString(PKNotifyType value)
{
	switch (value)
	{
		case PKNotifyType::NORMAL: return TEXT("NORMAL");
		case PKNotifyType::GUILD_WAR: return TEXT("GUILD_WAR");
	}
	return FString();
}
inline FString DeathPenaltyTypeEnumToString(DeathPenaltyType value)
{
	switch (value)
	{
		case DeathPenaltyType::NONE: return TEXT("NONE");
		case DeathPenaltyType::EXP: return TEXT("EXP");
		case DeathPenaltyType::ITEM: return TEXT("ITEM");
	}
	return FString();
}
inline FString PersonalTradingNoticeTypeEnumToString(PersonalTradingNoticeType value)
{
	switch (value)
	{
		case PersonalTradingNoticeType::ACCEPTED: return TEXT("ACCEPTED");
		case PersonalTradingNoticeType::REJECT: return TEXT("REJECT");
		case PersonalTradingNoticeType::REQUEST: return TEXT("REQUEST");
		case PersonalTradingNoticeType::ABORT: return TEXT("ABORT");
		case PersonalTradingNoticeType::ABORT_INVALID_STATUS: return TEXT("ABORT_INVALID_STATUS");
		case PersonalTradingNoticeType::TIME_OUT: return TEXT("TIME_OUT");
		case PersonalTradingNoticeType::LOCK: return TEXT("LOCK");
		case PersonalTradingNoticeType::UNLOCK: return TEXT("UNLOCK");
		case PersonalTradingNoticeType::CONFIRM: return TEXT("CONFIRM");
		case PersonalTradingNoticeType::SUCCESS: return TEXT("SUCCESS");
	}
	return FString();
}
inline FString PersonalTradingStatusEnumToString(PersonalTradingStatus value)
{
	switch (value)
	{
		case PersonalTradingStatus::NONE: return TEXT("NONE");
		case PersonalTradingStatus::WAINTING: return TEXT("WAINTING");
		case PersonalTradingStatus::UNLOCK: return TEXT("UNLOCK");
		case PersonalTradingStatus::LOCK: return TEXT("LOCK");
		case PersonalTradingStatus::CONFIRM: return TEXT("CONFIRM");
	}
	return FString();
}
inline FString SummonMileageActionTypeEnumToString(SummonMileageActionType value)
{
	switch (value)
	{
		case SummonMileageActionType::NONE: return TEXT("NONE");
		case SummonMileageActionType::SUMMON_FAIRY: return TEXT("SUMMON_FAIRY");
		case SummonMileageActionType::SUMMON_COSTUME: return TEXT("SUMMON_COSTUME");
		case SummonMileageActionType::UNIQUE_SUMMON_FAIRY: return TEXT("UNIQUE_SUMMON_FAIRY");
		case SummonMileageActionType::UNIQUE_SUMMON_COSTUME: return TEXT("UNIQUE_SUMMON_COSTUME");
	}
	return FString();
}
inline FString SummonMileageDataTypeEnumToString(SummonMileageDataType value)
{
	switch (value)
	{
		case SummonMileageDataType::NONE: return TEXT("NONE");
		case SummonMileageDataType::LEGACY_DATA: return TEXT("LEGACY_DATA");
		case SummonMileageDataType::NORMAL_DATA: return TEXT("NORMAL_DATA");
	}
	return FString();
}
inline FString ServerMigrateTypeEnumToString(ServerMigrateType value)
{
	switch (value)
	{
		case ServerMigrateType::NONE: return TEXT("NONE");
		case ServerMigrateType::ACCOUNT_ONLY: return TEXT("ACCOUNT_ONLY");
		case ServerMigrateType::INCLUDE_GUILD_INFO: return TEXT("INCLUDE_GUILD_INFO");
	}
	return FString();
}
inline FString ServerMigrateLimitStatusEnumToString(ServerMigrateLimitStatus value)
{
	switch (value)
	{
		case ServerMigrateLimitStatus::NONE: return TEXT("NONE");
		case ServerMigrateLimitStatus::FREE: return TEXT("FREE");
		case ServerMigrateLimitStatus::IMMINENT: return TEXT("IMMINENT");
		case ServerMigrateLimitStatus::CLOSING: return TEXT("CLOSING");
	}
	return FString();
}
inline FString ServerMigrateConditionTypeEnumToString(ServerMigrateConditionType value)
{
	switch (value)
	{
		case ServerMigrateConditionType::NONE: return TEXT("NONE");
		case ServerMigrateConditionType::SERVER_CHOICE: return TEXT("SERVER_CHOICE");
		case ServerMigrateConditionType::CHARACTER_LOCATION: return TEXT("CHARACTER_LOCATION");
		case ServerMigrateConditionType::CHARACTER_LEVEL: return TEXT("CHARACTER_LEVEL");
		case ServerMigrateConditionType::PARTY_JOINED: return TEXT("PARTY_JOINED");
		case ServerMigrateConditionType::GUILD_JOINED: return TEXT("GUILD_JOINED");
		case ServerMigrateConditionType::GUILD_JOINED_SUB: return TEXT("GUILD_JOINED_SUB");
		case ServerMigrateConditionType::GUILD_JOIN_APPLICATION: return TEXT("GUILD_JOIN_APPLICATION");
		case ServerMigrateConditionType::GUILD_JOIN_APPLICATION_SUB: return TEXT("GUILD_JOIN_APPLICATION_SUB");
		case ServerMigrateConditionType::RECOVERY_PENALTY: return TEXT("RECOVERY_PENALTY");
		case ServerMigrateConditionType::BMSHOP_STORAGE: return TEXT("BMSHOP_STORAGE");
		case ServerMigrateConditionType::MARKET_SELL: return TEXT("MARKET_SELL");
		case ServerMigrateConditionType::MARKET_CALCULATE: return TEXT("MARKET_CALCULATE");
		case ServerMigrateConditionType::MAIL: return TEXT("MAIL");
		case ServerMigrateConditionType::GUILD_LEVEL: return TEXT("GUILD_LEVEL");
		case ServerMigrateConditionType::GUILD_ALLY: return TEXT("GUILD_ALLY");
		case ServerMigrateConditionType::GUILD_ENEMY: return TEXT("GUILD_ENEMY");
		case ServerMigrateConditionType::GUILD_DIVISION: return TEXT("GUILD_DIVISION");
		case ServerMigrateConditionType::GUILD_AUCTION: return TEXT("GUILD_AUCTION");
		case ServerMigrateConditionType::GUILD_AUCTION_WITHDRAW: return TEXT("GUILD_AUCTION_WITHDRAW");
		case ServerMigrateConditionType::GUILD_CAMP: return TEXT("GUILD_CAMP");
		case ServerMigrateConditionType::GUILD_MEMBER: return TEXT("GUILD_MEMBER");
		case ServerMigrateConditionType::WORLDMARKET_SELL: return TEXT("WORLDMARKET_SELL");
		case ServerMigrateConditionType::WORLDMARKET_CALCULATE: return TEXT("WORLDMARKET_CALCULATE");
		case ServerMigrateConditionType::GUILDRAID_CLOSED: return TEXT("GUILDRAID_CLOSED");
		case ServerMigrateConditionType::GUILDWAR_JOINED: return TEXT("GUILDWAR_JOINED");
	}
	return FString();
}
inline FString BattleArenaTierEnumToString(BattleArenaTier value)
{
	switch (value)
	{
		case BattleArenaTier::NONE: return TEXT("NONE");
		case BattleArenaTier::CHAMPION: return TEXT("CHAMPION");
		case BattleArenaTier::MASTER: return TEXT("MASTER");
		case BattleArenaTier::DIAMOND: return TEXT("DIAMOND");
		case BattleArenaTier::GOLD: return TEXT("GOLD");
		case BattleArenaTier::SILVER: return TEXT("SILVER");
		case BattleArenaTier::BRONZE: return TEXT("BRONZE");
	}
	return FString();
}
inline FString ContentsBindingTypeEnumToString(ContentsBindingType value)
{
	switch (value)
	{
		case ContentsBindingType::USER: return TEXT("USER");
		case ContentsBindingType::ACCOUNT: return TEXT("ACCOUNT");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString AccountGradeEnumToDisplayKey(AccountGrade value)
{
	switch (value)
	{
		case AccountGrade::NORMAL: return TEXT("");
		case AccountGrade::OPERATOR: return TEXT("");
		case AccountGrade::QATESTER: return TEXT("");
		case AccountGrade::ADMIN: return TEXT("");
		case AccountGrade::GM: return TEXT("");
	}
	return FString();
}
inline FString ServerCongestionEnumToDisplayKey(ServerCongestion value)
{
	switch (value)
	{
		case ServerCongestion::ON_STANBY: return TEXT("");
		case ServerCongestion::GOOD: return TEXT("");
		case ServerCongestion::CONGESTED: return TEXT("");
		case ServerCongestion::FULL: return TEXT("");
		case ServerCongestion::RESTRICTED_ACCESS: return TEXT("");
		case ServerCongestion::BLOCK: return TEXT("");
		case ServerCongestion::NOT_CONNECTABLE: return TEXT("");
	}
	return FString();
}
inline FString MapTypeEnumToDisplayKey(MapType value)
{
	switch (value)
	{
		case MapType::NONE: return TEXT("");
		case MapType::PUBLIC_FIELD: return TEXT("");
		case MapType::SINGLE_FIELD: return TEXT("");
		case MapType::INSTANCE_SINGLE: return TEXT("");
		case MapType::INSTANCE_FIELD: return TEXT("");
		case MapType::PUBLIC_DUNGEON: return TEXT("");
		case MapType::GUILD_DUNGEON: return TEXT("");
		case MapType::PARTY_DUNGEON: return TEXT("");
		case MapType::ELITE_DUNGEON_1: return TEXT("");
		case MapType::WORLD_BOSS: return TEXT("");
		case MapType::INTER_FIELD: return TEXT("");
		case MapType::GUILD_WAR: return TEXT("");
		case MapType::AGIT_DUNGEON_RAID: return TEXT("");
		case MapType::AGIT: return TEXT("");
	}
	return FString();
}
inline FString MapContentsTypeEnumToDisplayKey(MapContentsType value)
{
	switch (value)
	{
		case MapContentsType::NORMAL: return TEXT("");
		case MapContentsType::BATTLE: return TEXT("");
		case MapContentsType::OUTLAW: return TEXT("");
		case MapContentsType::SPACE_CRACK: return TEXT("");
	}
	return FString();
}
inline FString WarpTypeEnumToDisplayKey(WarpType value)
{
	switch (value)
	{
		case WarpType::Immediately: return TEXT("");
		case WarpType::YesNoDialog: return TEXT("");
	}
	return FString();
}
inline FString TerritoryTypeEnumToDisplayKey(TerritoryType value)
{
	switch (value)
	{
		case TerritoryType::NONE: return TEXT("");
		case TerritoryType::TOWN: return TEXT("");
		case TerritoryType::SAFE_ZONE: return TEXT("");
		case TerritoryType::ARENA_READY: return TEXT("");
		case TerritoryType::ARENA: return TEXT("");
		case TerritoryType::ARRIVAL: return TEXT("");
		case TerritoryType::Env: return TEXT("");
		case TerritoryType::SOUND: return TEXT("");
		case TerritoryType::SPACE_CRACK: return TEXT("");
		case TerritoryType::TOWN_CHAOS: return TEXT("");
		case TerritoryType::NO_CAMP_ZONE: return TEXT("");
		case TerritoryType::NON_PK: return TEXT("");
	}
	return FString();
}
inline FString TerritoryCheckTypeEnumToDisplayKey(TerritoryCheckType value)
{
	switch (value)
	{
		case TerritoryCheckType::NONE: return TEXT("");
		case TerritoryCheckType::INVADE: return TEXT("");
	}
	return FString();
}
inline FString ChannelCongestionStatusEnumToDisplayKey(ChannelCongestionStatus value)
{
	switch (value)
	{
		case ChannelCongestionStatus::LOW: return TEXT("");
		case ChannelCongestionStatus::MEDIUM: return TEXT("");
		case ChannelCongestionStatus::HIGH: return TEXT("");
		case ChannelCongestionStatus::FULL: return TEXT("");
	}
	return FString();
}
inline FString WarpReasonEnumToDisplayKey(WarpReason value)
{
	switch (value)
	{
		case WarpReason::ENTER_WORLD: return TEXT("");
		case WarpReason::CHANGE_CHANNEL: return TEXT("");
		case WarpReason::PORTAL: return TEXT("");
		case WarpReason::PARTY_MATCHING: return TEXT("");
		case WarpReason::RESURRECT: return TEXT("");
		case WarpReason::ADMIN_COMMAND: return TEXT("");
		case WarpReason::QUEST: return TEXT("");
		case WarpReason::EVENT_ACTION_TO_INSTANCE_SINGLE: return TEXT("");
		case WarpReason::LEAVE_INSTANCE_SINGLE: return TEXT("");
		case WarpReason::EVENT_ACTION_RETURN_TO_PUBLIC: return TEXT("");
		case WarpReason::EVENT_ACTION_TELEPORT: return TEXT("");
		case WarpReason::BOOKMARK: return TEXT("");
		case WarpReason::USE_WARP_TO_TOWN_SCROLL: return TEXT("");
		case WarpReason::USE_WARP_TO_RANDOM_SCROLL: return TEXT("");
		case WarpReason::DUNGEON_TIME_OVER: return TEXT("");
		case WarpReason::MAP_EVENT: return TEXT("");
		case WarpReason::BOOKMARK_SHARED: return TEXT("");
		case WarpReason::PARTY_LEADER: return TEXT("");
		case WarpReason::SPACE_CRACK: return TEXT("");
		case WarpReason::WORLD_MAP_PORTAL: return TEXT("");
		case WarpReason::EVENT_DUNGEON_TIME_OVER: return TEXT("");
		case WarpReason::ENTER_GUILD_DUNGEON: return TEXT("");
		case WarpReason::LEAVE_GUILD_DUNGEON: return TEXT("");
		case WarpReason::GUILD_CAMP: return TEXT("");
		case WarpReason::INVADE: return TEXT("");
		case WarpReason::INVADE_LEAVE: return TEXT("");
		case WarpReason::INVADE_TIME_OVER: return TEXT("");
		case WarpReason::INVADE_USER_TIME_OVER: return TEXT("");
		case WarpReason::PARTY_DUNGEON: return TEXT("");
		case WarpReason::PARTY_DUNGEON_LEAVE: return TEXT("");
		case WarpReason::PARTY_DUNGEON_TIME_OVER: return TEXT("");
		case WarpReason::ELITE_DUNGEON: return TEXT("");
		case WarpReason::ELITE_DUNGEON_LEAVE: return TEXT("");
		case WarpReason::ELITE_DUNGEON_TIME_OVER: return TEXT("");
		case WarpReason::ARENA: return TEXT("");
		case WarpReason::ARENA_LEAVE: return TEXT("");
		case WarpReason::ITEM_TREASURE_MONSTER: return TEXT("");
		case WarpReason::REVENGE: return TEXT("");
		case WarpReason::INVADE_USE_SCROLL_LEAVE: return TEXT("");
		case WarpReason::WORLD_BOSS: return TEXT("");
		case WarpReason::WORLD_BOSS_TIME_OUT: return TEXT("");
		case WarpReason::WORLD_BOSS_LEAVE: return TEXT("");
		case WarpReason::DESTROYED_THEN_RETURN_PUBLIC_FIELD: return TEXT("");
		case WarpReason::WORLD_MAP_PORTAL_TO_BOSS: return TEXT("");
		case WarpReason::ELITE_DUNGEON_HIDDEN: return TEXT("");
		case WarpReason::USE_DUNGEON_TICKET: return TEXT("");
		case WarpReason::GUILD_WAR: return TEXT("");
		case WarpReason::GUILD_WAR_TIME_OUT: return TEXT("");
		case WarpReason::GUILD_WAR_LEAVE: return TEXT("");
		case WarpReason::SKILL_NOTIFY: return TEXT("");
		case WarpReason::ENTER_AGIT_DUNGEON_RAID: return TEXT("");
		case WarpReason::LEAVE_SELF_AGIT_DUNGEON_RAID: return TEXT("");
		case WarpReason::ENTER_AGIT: return TEXT("");
		case WarpReason::LEAVE_AGIT: return TEXT("");
		case WarpReason::LEAVE_AGIT_RANDOM_SCROLL: return TEXT("");
		case WarpReason::LEAVE_FORCE_AGIT_DUNGEON_RAID: return TEXT("");
	}
	return FString();
}
inline FString CommandTypeEnumToDisplayKey(CommandType value)
{
	switch (value)
	{
		case CommandType::COMMAND_OPERATING: return TEXT("");
		case CommandType::COMMAND_QA: return TEXT("");
	}
	return FString();
}
inline FString CommandHandlerTypeEnumToDisplayKey(CommandHandlerType value)
{
	switch (value)
	{
		case CommandHandlerType::HANDLER_OPERATING_GATEWAY: return TEXT("");
		case CommandHandlerType::HANDLER_OPERATING_WORLD: return TEXT("");
		case CommandHandlerType::HANDLER_QA_GATEWAY: return TEXT("");
		case CommandHandlerType::HANDLER_QA_WORLD: return TEXT("");
		case CommandHandlerType::HANDLER_RANK_GATEWAY: return TEXT("");
		case CommandHandlerType::HANDLER_HIVE_ITEM_GATEWAY: return TEXT("");
		case CommandHandlerType::HANDLER_OPERATING_AUTH: return TEXT("");
		case CommandHandlerType::HANDLER_OPERATING_DIRECTORY: return TEXT("");
		case CommandHandlerType::HANDLER_SERVER_TO_SERVER_GATEWAY: return TEXT("");
	}
	return FString();
}
inline FString TimeTicketTypeEnumToDisplayKey(TimeTicketType value)
{
	switch (value)
	{
		case TimeTicketType::TIME_TICKET_EXP_DEATH_PENALTY_FREE: return TEXT("");
		case TimeTicketType::TIME_TICKET_ITEM_DEATH_PENALTY_FREE: return TEXT("");
	}
	return FString();
}
inline FString CurrencyTypeEnumToDisplayKey(CurrencyType value)
{
	switch (value)
	{
		case CurrencyType::NONE: return TEXT("");
		case CurrencyType::DIA: return TEXT("");
		case CurrencyType::FREE_DIA: return TEXT("");
		case CurrencyType::MILEAGE: return TEXT("");
		case CurrencyType::GOLD: return TEXT("");
		case CurrencyType::CASH_MILEAGE: return TEXT("");
		case CurrencyType::MONSTER_BOOK_POINT: return TEXT("");
		case CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE: return TEXT("");
		case CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE: return TEXT("");
		case CurrencyType::MAX_ACCOUNT_TYPE: return TEXT("");
		case CurrencyType::START_USER_TYPE: return TEXT("");
		case CurrencyType::EXP_DEATH_PENALTY_FREE: return TEXT("");
		case CurrencyType::ITEM_DEATH_PENALTY_FREE: return TEXT("");
		case CurrencyType::DEATH_EXP_RECOVERY: return TEXT("");
		case CurrencyType::SUMMON_FAIRY_MILEAGE: return TEXT("");
		case CurrencyType::SUMMON_COSTUME_MILEAGE: return TEXT("");
		case CurrencyType::MAX_USER_TYPE: return TEXT("");
		case CurrencyType::START_GUILD_TYPE: return TEXT("");
		case CurrencyType::GUILD_ASSET: return TEXT("");
		case CurrencyType::MAX_GUILD_TYPE: return TEXT("");
		case CurrencyType::MAX_IN_GAME_TYPE: return TEXT("");
		case CurrencyType::CASH: return TEXT("");
	}
	return FString();
}
inline FString RewardTypeEnumToDisplayKey(RewardType value)
{
	switch (value)
	{
		case RewardType::ALL: return TEXT("");
		case RewardType::PICK_ONE: return TEXT("");
		case RewardType::RANDOM_ONE: return TEXT("");
		case RewardType::RANDOM_GROUP: return TEXT("");
		case RewardType::PICK_REWARD_BOX_LIST: return TEXT("");
		case RewardType::SUB_PICK_ONE: return TEXT("");
		case RewardType::SUB_RANDOM_ONE: return TEXT("");
	}
	return FString();
}
inline FString WeightPenaltyGradeEnumToDisplayKey(WeightPenaltyGrade value)
{
	switch (value)
	{
		case WeightPenaltyGrade::NONE: return TEXT("");
		case WeightPenaltyGrade::FIRST: return TEXT("");
		case WeightPenaltyGrade::SECOND: return TEXT("");
	}
	return FString();
}
inline FString PartyDropOwnershipTypeEnumToDisplayKey(PartyDropOwnershipType value)
{
	switch (value)
	{
		case PartyDropOwnershipType::RANDOM: return TEXT("");
		case PartyDropOwnershipType::SEQUENCE: return TEXT("");
		case PartyDropOwnershipType::FREE: return TEXT("");
	}
	return FString();
}
inline FString RestoreTypeEnumToDisplayKey(RestoreType value)
{
	switch (value)
	{
		case RestoreType::Free: return TEXT("");
		case RestoreType::Currency: return TEXT("");
	}
	return FString();
}
inline FString ResetTimeTicketEnumToDisplayKey(ResetTimeTicket value)
{
	switch (value)
	{
		case ResetTimeTicket::RTT_NONE: return TEXT("");
		case ResetTimeTicket::RTT_DAILY: return TEXT("");
		case ResetTimeTicket::RTT_WEEKLY: return TEXT("");
		case ResetTimeTicket::RTT_MONTHLY: return TEXT("");
		case ResetTimeTicket::RTT_DUNGEON: return TEXT("");
		case ResetTimeTicket::RTT_SPACEC_CRACK: return TEXT("");
		case ResetTimeTicket::RTT_OFF_LINE_MODE: return TEXT("");
		case ResetTimeTicket::RTT_DEATH_RESURRECTION: return TEXT("");
		case ResetTimeTicket::RTT_GUILD_QUEST: return TEXT("");
		case ResetTimeTicket::RTT_GUILD_DUNGEON: return TEXT("");
		case ResetTimeTicket::RTT_INVADE_START: return TEXT("");
		case ResetTimeTicket::RTT_INVADE_END: return TEXT("");
		case ResetTimeTicket::RTT_INVADE_RESET: return TEXT("");
		case ResetTimeTicket::RTT_PARTY_DUNGEON_RESET: return TEXT("");
		case ResetTimeTicket::RTT_GUILD_ZEAL_POINT_RESET: return TEXT("");
		case ResetTimeTicket::RTT_FREE_INVADE_CHAT_RESET: return TEXT("");
		case ResetTimeTicket::RTT_CALCUATION_TAX: return TEXT("");
		case ResetTimeTicket::RTT_BM_DAILY: return TEXT("");
		case ResetTimeTicket::RTT_BM_WEEKLY: return TEXT("");
		case ResetTimeTicket::RTT_BM_MONTHLY: return TEXT("");
		case ResetTimeTicket::RTT_ATTENDANCE_DAILY: return TEXT("");
		case ResetTimeTicket::RTT_PASS_DAILY: return TEXT("");
		case ResetTimeTicket::RTT_PASS_WEEKLY: return TEXT("");
		case ResetTimeTicket::RTT_AGIT_DUNGEON_RAID: return TEXT("");
	}
	return FString();
}
inline FString CostTypeEnumToDisplayKey(CostType value)
{
	switch (value)
	{
		case CostType::CURRENCY: return TEXT("CURRENCY");
		case CostType::ITEM: return TEXT("ITEM");
	}
	return FString();
}
inline FString ItemStorageTypeEnumToDisplayKey(ItemStorageType value)
{
	switch (value)
	{
		case ItemStorageType::INVENTORY: return TEXT("");
		case ItemStorageType::DEPOT: return TEXT("");
		case ItemStorageType::DEATH: return TEXT("");
		case ItemStorageType::MARKET: return TEXT("");
		case ItemStorageType::GUILD_STORE: return TEXT("");
		case ItemStorageType::MAIL: return TEXT("");
		case ItemStorageType::WDMARKET: return TEXT("");
	}
	return FString();
}
inline FString MultiLevelRankSlotStateEnumToDisplayKey(MultiLevelRankSlotState value)
{
	switch (value)
	{
		case MultiLevelRankSlotState::LOCK: return TEXT("");
		case MultiLevelRankSlotState::UNLOCK: return TEXT("");
		case MultiLevelRankSlotState::IN_PROGRESS: return TEXT("");
		case MultiLevelRankSlotState::FINISH: return TEXT("");
		case MultiLevelRankSlotState::ACTIVE: return TEXT("");
	}
	return FString();
}
inline FString PurchaseLimitBuyerTypeEnumToDisplayKey(PurchaseLimitBuyerType value)
{
	switch (value)
	{
		case PurchaseLimitBuyerType::NONE: return TEXT("");
		case PurchaseLimitBuyerType::CHARACTER: return TEXT("");
		case PurchaseLimitBuyerType::SERVER: return TEXT("");
		case PurchaseLimitBuyerType::ACCOUNT: return TEXT("");
	}
	return FString();
}
inline FString BMPurchaseLimitBuyerTypeEnumToDisplayKey(BMPurchaseLimitBuyerType value)
{
	switch (value)
	{
		case BMPurchaseLimitBuyerType::NONE: return TEXT("");
		case BMPurchaseLimitBuyerType::SERVER: return TEXT("");
		case BMPurchaseLimitBuyerType::ACCOUNT: return TEXT("");
	}
	return FString();
}
inline FString ContentsTypeEnumToDisplayKey(ContentsType value)
{
	switch (value)
	{
		case ContentsType::ATTENDANCE: return TEXT("");
		case ContentsType::GUILD_STORE_ACQUIRED_ITEM: return TEXT("");
		case ContentsType::GUILD_STORE_DISTRIBUTED_ITEM: return TEXT("");
		case ContentsType::ELITE_DUNGEON_1: return TEXT("");
		case ContentsType::CODEX: return TEXT("");
	}
	return FString();
}
inline FString ContentsStateEnumToDisplayKey(ContentsState value)
{
	switch (value)
	{
		case ContentsState::RED_DOT: return TEXT("");
		case ContentsState::VISIBLE: return TEXT("");
		case ContentsState::HIDDEN: return TEXT("");
	}
	return FString();
}
inline FString CanItemDropEnumToDisplayKey(CanItemDrop value)
{
	switch (value)
	{
		case CanItemDrop::DEFAULT: return TEXT("");
		case CanItemDrop::DROP: return TEXT("");
		case CanItemDrop::NO_DROP: return TEXT("");
	}
	return FString();
}
inline FString AutoDropItemGradeEnumToDisplayKey(AutoDropItemGrade value)
{
	switch (value)
	{
		case AutoDropItemGrade::ALL: return TEXT("");
		case AutoDropItemGrade::MAGIC: return TEXT("");
		case AutoDropItemGrade::RARE: return TEXT("");
		case AutoDropItemGrade::OFF: return TEXT("");
	}
	return FString();
}
inline FString AutoAttackRangeEnumToDisplayKey(AutoAttackRange value)
{
	switch (value)
	{
		case AutoAttackRange::METER_20: return TEXT("");
		case AutoAttackRange::METER_30: return TEXT("");
		case AutoAttackRange::METER_50: return TEXT("");
		case AutoAttackRange::NO_LIMIT: return TEXT("");
	}
	return FString();
}
inline FString MannerModeEnumToDisplayKey(MannerMode value)
{
	switch (value)
	{
		case MannerMode::ON: return TEXT("");
		case MannerMode::EXCEPT_PARTY_GUILD: return TEXT("");
		case MannerMode::OFF: return TEXT("");
	}
	return FString();
}
inline FString DevicePlatformTypeEnumToDisplayKey(DevicePlatformType value)
{
	switch (value)
	{
		case DevicePlatformType::NONE: return TEXT("");
		case DevicePlatformType::WINDOWS: return TEXT("");
		case DevicePlatformType::EDITOR: return TEXT("");
		case DevicePlatformType::GOOLE_ANDROID: return TEXT("");
		case DevicePlatformType::APPLE_IOS: return TEXT("");
		case DevicePlatformType::ETC: return TEXT("");
	}
	return FString();
}
inline FString UserStatusEnumToDisplayKey(UserStatus value)
{
	switch (value)
	{
		case UserStatus::USER_STATE_NORMAL: return TEXT("");
		case UserStatus::USER_STATE_COMBAT: return TEXT("");
		case UserStatus::USER_STATE_CAMP_INTERACT: return TEXT("");
		case UserStatus::USER_STATE_PROP_INTERACT: return TEXT("");
	}
	return FString();
}
inline FString HiveNotificationTypeEnumToDisplayKey(HiveNotificationType value)
{
	switch (value)
	{
		case HiveNotificationType::TAKE_DAMAGE_IN_PVP: return TEXT("");
		case HiveNotificationType::DEAD: return TEXT("");
		case HiveNotificationType::WEIGHT_OVER: return TEXT("");
		case HiveNotificationType::FINISH_DUNGEON: return TEXT("");
		case HiveNotificationType::FINISH_OFFLINE_PLAY: return TEXT("");
		case HiveNotificationType::FULL_INVENTORY: return TEXT("");
		case HiveNotificationType::GET_EQUIP_ITEM: return TEXT("");
		case HiveNotificationType::GET_SKILL_BOOK: return TEXT("");
		case HiveNotificationType::MARKET_ITEM_SOLD: return TEXT("");
		case HiveNotificationType::SERVER_MIGRATE: return TEXT("");
	}
	return FString();
}
inline FString EnterWorldServerTypeEnumToDisplayKey(EnterWorldServerType value)
{
	switch (value)
	{
		case EnterWorldServerType::NONE: return TEXT("");
		case EnterWorldServerType::NORMAL: return TEXT("");
		case EnterWorldServerType::INTER_SERVER: return TEXT("");
		case EnterWorldServerType::INVADE_SERVER: return TEXT("");
	}
	return FString();
}
inline FString RewardGiveTypeEnumToDisplayKey(RewardGiveType value)
{
	switch (value)
	{
		case RewardGiveType::NONE: return TEXT("");
		case RewardGiveType::FILED_DROP: return TEXT("");
		case RewardGiveType::INVENTORY: return TEXT("");
		case RewardGiveType::MAIL: return TEXT("");
	}
	return FString();
}
inline FString PartyTypeEnumToDisplayKey(PartyType value)
{
	switch (value)
	{
		case PartyType::NORMAL: return TEXT("");
		case PartyType::COOP: return TEXT("");
		case PartyType::INTER: return TEXT("");
	}
	return FString();
}
inline FString DungeonCategoryEnumToDisplayKey(DungeonCategory value)
{
	switch (value)
	{
		case DungeonCategory::NORMAL: return TEXT("");
		case DungeonCategory::SPECIAL: return TEXT("");
		case DungeonCategory::PARTY: return TEXT("");
		case DungeonCategory::ELITE: return TEXT("");
		case DungeonCategory::ARENA: return TEXT("");
	}
	return FString();
}
inline FString EnterWorldFailReasonEnumToDisplayKey(EnterWorldFailReason value)
{
	switch (value)
	{
		case EnterWorldFailReason::EWFR_NONE: return TEXT("");
		case EnterWorldFailReason::EWFR_INVALID_MAP: return TEXT("");
		case EnterWorldFailReason::EWFR_INAVLID_CHANNEL: return TEXT("");
		case EnterWorldFailReason::EWFR_LOGIN_INVALID_GUILD_DUNGEON: return TEXT("");
		case EnterWorldFailReason::EWFR_FAILED_DISCONNECTED: return TEXT("");
		case EnterWorldFailReason::EWFR_FAILED_COMPLETE: return TEXT("");
		case EnterWorldFailReason::EWFR_INVALID_WORLD_BOSS: return TEXT("");
		case EnterWorldFailReason::EWFR_DUPLICATION_USER: return TEXT("");
	}
	return FString();
}
inline FString DuelResultEnumToDisplayKey(DuelResult value)
{
	switch (value)
	{
		case DuelResult::NONE: return TEXT("");
		case DuelResult::WIN: return TEXT("");
		case DuelResult::LOSE: return TEXT("");
	}
	return FString();
}
inline FString HiveChannelEnumToDisplayKey(HiveChannel value)
{
	switch (value)
	{
		case HiveChannel::C2S: return TEXT("");
		case HiveChannel::KAK: return TEXT("");
		case HiveChannel::LIN: return TEXT("");
		case HiveChannel::WEI: return TEXT("");
		case HiveChannel::STE: return TEXT("");
	}
	return FString();
}
inline FString HiveMarketEnumToDisplayKey(HiveMarket value)
{
	switch (value)
	{
		case HiveMarket::TS: return TEXT("");
		case HiveMarket::OL: return TEXT("");
		case HiveMarket::OZ: return TEXT("");
		case HiveMarket::AP: return TEXT("");
		case HiveMarket::GO: return TEXT("");
		case HiveMarket::SA: return TEXT("");
		case HiveMarket::LE: return TEXT("");
		case HiveMarket::MM: return TEXT("");
		case HiveMarket::SN: return TEXT("");
		case HiveMarket::QP: return TEXT("");
		case HiveMarket::MO: return TEXT("");
		case HiveMarket::DN: return TEXT("");
		case HiveMarket::NA: return TEXT("");
		case HiveMarket::AM: return TEXT("");
		case HiveMarket::ON: return TEXT("");
		case HiveMarket::FU: return TEXT("");
		case HiveMarket::HU: return TEXT("");
		case HiveMarket::OP: return TEXT("");
		case HiveMarket::VI: return TEXT("");
		case HiveMarket::XI: return TEXT("");
		case HiveMarket::TC: return TEXT("");
		case HiveMarket::HS: return TEXT("");
		case HiveMarket::ST: return TEXT("");
	}
	return FString();
}
inline FString DropCheckTypeEnumToDisplayKey(DropCheckType value)
{
	switch (value)
	{
		case DropCheckType::MAP: return TEXT("");
		case DropCheckType::RANK: return TEXT("");
		case DropCheckType::RACE: return TEXT("");
		case DropCheckType::ELEMENT: return TEXT("");
		case DropCheckType::INVADE: return TEXT("");
		case DropCheckType::SERVER: return TEXT("");
		case DropCheckType::NPC_FUNCTION_TYPE: return TEXT("");
		case DropCheckType::CORRECTION: return TEXT("");
	}
	return FString();
}
inline FString InvadeDropTypeEnumToDisplayKey(InvadeDropType value)
{
	switch (value)
	{
		case InvadeDropType::NORMAL: return TEXT("");
		case InvadeDropType::ATTACK: return TEXT("");
		case InvadeDropType::DEFENSE: return TEXT("");
	}
	return FString();
}
inline FString CommonEffectTypeEnumToDisplayKey(CommonEffectType value)
{
	switch (value)
	{
		case CommonEffectType::NONE: return TEXT("");
		case CommonEffectType::SPAWN_INVADE_PORTAL: return TEXT("");
		case CommonEffectType::WARP_TO_DUNGEON: return TEXT("");
		case CommonEffectType::WARP_TO_INVADE: return TEXT("");
	}
	return FString();
}
inline FString PKBookResultEnumToDisplayKey(PKBookResult value)
{
	switch (value)
	{
		case PKBookResult::KILL: return TEXT("");
		case PKBookResult::DIE: return TEXT("");
		case PKBookResult::ASSIST: return TEXT("");
	}
	return FString();
}
inline FString BookMarkTypeEnumToDisplayKey(BookMarkType value)
{
	switch (value)
	{
		case BookMarkType::POSITION: return TEXT("");
		case BookMarkType::ITEM_COLLECTION: return TEXT("");
		case BookMarkType::GUILD_ITEM_COLLECTION: return TEXT("");
		case BookMarkType::MARKET: return TEXT("");
		case BookMarkType::FIELD_BOSS: return TEXT("");
		case BookMarkType::CRAFT: return TEXT("");
		case BookMarkType::STAT: return TEXT("");
		case BookMarkType::WDMARKET: return TEXT("");
		case BookMarkType::AGIT_DUNGEON_RAID: return TEXT("");
	}
	return FString();
}
inline FString PKContributorTypeEnumToDisplayKey(PKContributorType value)
{
	switch (value)
	{
		case PKContributorType::USER: return TEXT("");
		case PKContributorType::NPC: return TEXT("");
	}
	return FString();
}
inline FString PKBookActionEnumToDisplayKey(PKBookAction value)
{
	switch (value)
	{
		case PKBookAction::REVENGE: return TEXT("");
		case PKBookAction::TEASE: return TEXT("");
	}
	return FString();
}
inline FString PKNotifyTypeEnumToDisplayKey(PKNotifyType value)
{
	switch (value)
	{
		case PKNotifyType::NORMAL: return TEXT("");
		case PKNotifyType::GUILD_WAR: return TEXT("");
	}
	return FString();
}
inline FString DeathPenaltyTypeEnumToDisplayKey(DeathPenaltyType value)
{
	switch (value)
	{
		case DeathPenaltyType::NONE: return TEXT("");
		case DeathPenaltyType::EXP: return TEXT("");
		case DeathPenaltyType::ITEM: return TEXT("");
	}
	return FString();
}
inline FString PersonalTradingNoticeTypeEnumToDisplayKey(PersonalTradingNoticeType value)
{
	switch (value)
	{
		case PersonalTradingNoticeType::ACCEPTED: return TEXT("");
		case PersonalTradingNoticeType::REJECT: return TEXT("");
		case PersonalTradingNoticeType::REQUEST: return TEXT("");
		case PersonalTradingNoticeType::ABORT: return TEXT("");
		case PersonalTradingNoticeType::ABORT_INVALID_STATUS: return TEXT("");
		case PersonalTradingNoticeType::TIME_OUT: return TEXT("");
		case PersonalTradingNoticeType::LOCK: return TEXT("");
		case PersonalTradingNoticeType::UNLOCK: return TEXT("");
		case PersonalTradingNoticeType::CONFIRM: return TEXT("");
		case PersonalTradingNoticeType::SUCCESS: return TEXT("");
	}
	return FString();
}
inline FString PersonalTradingStatusEnumToDisplayKey(PersonalTradingStatus value)
{
	switch (value)
	{
		case PersonalTradingStatus::NONE: return TEXT("");
		case PersonalTradingStatus::WAINTING: return TEXT("");
		case PersonalTradingStatus::UNLOCK: return TEXT("");
		case PersonalTradingStatus::LOCK: return TEXT("");
		case PersonalTradingStatus::CONFIRM: return TEXT("");
	}
	return FString();
}
inline FString SummonMileageActionTypeEnumToDisplayKey(SummonMileageActionType value)
{
	switch (value)
	{
		case SummonMileageActionType::NONE: return TEXT("");
		case SummonMileageActionType::SUMMON_FAIRY: return TEXT("");
		case SummonMileageActionType::SUMMON_COSTUME: return TEXT("");
		case SummonMileageActionType::UNIQUE_SUMMON_FAIRY: return TEXT("");
		case SummonMileageActionType::UNIQUE_SUMMON_COSTUME: return TEXT("");
	}
	return FString();
}
inline FString SummonMileageDataTypeEnumToDisplayKey(SummonMileageDataType value)
{
	switch (value)
	{
		case SummonMileageDataType::NONE: return TEXT("");
		case SummonMileageDataType::LEGACY_DATA: return TEXT("");
		case SummonMileageDataType::NORMAL_DATA: return TEXT("");
	}
	return FString();
}
inline FString ServerMigrateTypeEnumToDisplayKey(ServerMigrateType value)
{
	switch (value)
	{
		case ServerMigrateType::NONE: return TEXT("");
		case ServerMigrateType::ACCOUNT_ONLY: return TEXT("");
		case ServerMigrateType::INCLUDE_GUILD_INFO: return TEXT("");
	}
	return FString();
}
inline FString ServerMigrateLimitStatusEnumToDisplayKey(ServerMigrateLimitStatus value)
{
	switch (value)
	{
		case ServerMigrateLimitStatus::NONE: return TEXT("");
		case ServerMigrateLimitStatus::FREE: return TEXT("");
		case ServerMigrateLimitStatus::IMMINENT: return TEXT("");
		case ServerMigrateLimitStatus::CLOSING: return TEXT("");
	}
	return FString();
}
inline FString ServerMigrateConditionTypeEnumToDisplayKey(ServerMigrateConditionType value)
{
	switch (value)
	{
		case ServerMigrateConditionType::NONE: return TEXT("");
		case ServerMigrateConditionType::SERVER_CHOICE: return TEXT("");
		case ServerMigrateConditionType::CHARACTER_LOCATION: return TEXT("");
		case ServerMigrateConditionType::CHARACTER_LEVEL: return TEXT("");
		case ServerMigrateConditionType::PARTY_JOINED: return TEXT("");
		case ServerMigrateConditionType::GUILD_JOINED: return TEXT("");
		case ServerMigrateConditionType::GUILD_JOINED_SUB: return TEXT("");
		case ServerMigrateConditionType::GUILD_JOIN_APPLICATION: return TEXT("");
		case ServerMigrateConditionType::GUILD_JOIN_APPLICATION_SUB: return TEXT("");
		case ServerMigrateConditionType::RECOVERY_PENALTY: return TEXT("");
		case ServerMigrateConditionType::BMSHOP_STORAGE: return TEXT("");
		case ServerMigrateConditionType::MARKET_SELL: return TEXT("");
		case ServerMigrateConditionType::MARKET_CALCULATE: return TEXT("");
		case ServerMigrateConditionType::MAIL: return TEXT("");
		case ServerMigrateConditionType::GUILD_LEVEL: return TEXT("");
		case ServerMigrateConditionType::GUILD_ALLY: return TEXT("");
		case ServerMigrateConditionType::GUILD_ENEMY: return TEXT("");
		case ServerMigrateConditionType::GUILD_DIVISION: return TEXT("");
		case ServerMigrateConditionType::GUILD_AUCTION: return TEXT("");
		case ServerMigrateConditionType::GUILD_AUCTION_WITHDRAW: return TEXT("");
		case ServerMigrateConditionType::GUILD_CAMP: return TEXT("");
		case ServerMigrateConditionType::GUILD_MEMBER: return TEXT("");
		case ServerMigrateConditionType::WORLDMARKET_SELL: return TEXT("");
		case ServerMigrateConditionType::WORLDMARKET_CALCULATE: return TEXT("");
		case ServerMigrateConditionType::GUILDRAID_CLOSED: return TEXT("");
		case ServerMigrateConditionType::GUILDWAR_JOINED: return TEXT("");
	}
	return FString();
}
inline FString BattleArenaTierEnumToDisplayKey(BattleArenaTier value)
{
	switch (value)
	{
		case BattleArenaTier::NONE: return TEXT("");
		case BattleArenaTier::CHAMPION: return TEXT("");
		case BattleArenaTier::MASTER: return TEXT("");
		case BattleArenaTier::DIAMOND: return TEXT("");
		case BattleArenaTier::GOLD: return TEXT("");
		case BattleArenaTier::SILVER: return TEXT("");
		case BattleArenaTier::BRONZE: return TEXT("");
	}
	return FString();
}
inline FString ContentsBindingTypeEnumToDisplayKey(ContentsBindingType value)
{
	switch (value)
	{
		case ContentsBindingType::USER: return TEXT("");
		case ContentsBindingType::ACCOUNT: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidAccountGrade(AccountGrade value) noexcept
{
	return (value >= AccountGrade::NORMAL && value < AccountGrade::MAX);
}

inline bool IsValidServerCongestion(ServerCongestion value) noexcept
{
	return (value >= ServerCongestion::ON_STANBY && value < ServerCongestion::MAX);
}

inline bool IsValidMapType(MapType value) noexcept
{
	return (value >= MapType::NONE && value < MapType::MAX);
}

inline bool IsValidMapContentsType(MapContentsType value) noexcept
{
	return (value >= MapContentsType::NORMAL && value < MapContentsType::MAX);
}

inline bool IsValidWarpType(WarpType value) noexcept
{
	return (value >= WarpType::Immediately && value < WarpType::MAX);
}

inline bool IsValidTerritoryType(TerritoryType value) noexcept
{
	return (value >= TerritoryType::NONE && value < TerritoryType::MAX);
}

inline bool IsValidTerritoryCheckType(TerritoryCheckType value) noexcept
{
	return (value >= TerritoryCheckType::NONE && value < TerritoryCheckType::MAX);
}

inline bool IsValidChannelCongestionStatus(ChannelCongestionStatus value) noexcept
{
	return (value >= ChannelCongestionStatus::LOW && value < ChannelCongestionStatus::MAX);
}

inline bool IsValidWarpReason(WarpReason value) noexcept
{
	return (value >= WarpReason::ENTER_WORLD && value < WarpReason::INVALID);
}

inline bool IsValidCommandType(CommandType value) noexcept
{
	return (value >= CommandType::COMMAND_OPERATING && value <= CommandType::COMMAND_QA);
}

inline bool IsValidCommandHandlerType(CommandHandlerType value) noexcept
{
	return (value >= CommandHandlerType::HANDLER_OPERATING_GATEWAY && value <= CommandHandlerType::HANDLER_SERVER_TO_SERVER_GATEWAY);
}

inline bool IsValidTimeTicketType(TimeTicketType value) noexcept
{
	return (value >= TimeTicketType::TIME_TICKET_EXP_DEATH_PENALTY_FREE && value < TimeTicketType::MAX);
}

inline bool IsValidCurrencyType(CurrencyType value) noexcept
{
	return (value >= CurrencyType::NONE && value < CurrencyType::MAX);
}

inline bool IsValidRewardType(RewardType value) noexcept
{
	return (value >= RewardType::ALL && value < RewardType::MAX);
}

inline bool IsValidWeightPenaltyGrade(WeightPenaltyGrade value) noexcept
{
	return (value >= WeightPenaltyGrade::NONE && value < WeightPenaltyGrade::MAX);
}

inline bool IsValidPartyDropOwnershipType(PartyDropOwnershipType value) noexcept
{
	return (value >= PartyDropOwnershipType::RANDOM && value < PartyDropOwnershipType::MAX);
}

inline bool IsValidRestoreType(RestoreType value) noexcept
{
	return (value >= RestoreType::Free && value < RestoreType::MAX);
}

inline bool IsValidResetTimeTicket(ResetTimeTicket value) noexcept
{
	return (value >= ResetTimeTicket::RTT_NONE && value < ResetTimeTicket::MAX);
}

inline bool IsValidCostType(CostType value) noexcept
{
	return (value >= CostType::CURRENCY && value < CostType::MAX);
}

inline bool IsValidItemStorageType(ItemStorageType value) noexcept
{
	return (value >= ItemStorageType::INVENTORY && value < ItemStorageType::MAX);
}

inline bool IsValidMultiLevelRankSlotState(MultiLevelRankSlotState value) noexcept
{
	return (value >= MultiLevelRankSlotState::LOCK && value < MultiLevelRankSlotState::MAX);
}

inline bool IsValidPurchaseLimitBuyerType(PurchaseLimitBuyerType value) noexcept
{
	return (value >= PurchaseLimitBuyerType::NONE && value < PurchaseLimitBuyerType::MAX);
}

inline bool IsValidBMPurchaseLimitBuyerType(BMPurchaseLimitBuyerType value) noexcept
{
	return (value >= BMPurchaseLimitBuyerType::NONE && value < BMPurchaseLimitBuyerType::MAX);
}

inline bool IsValidContentsType(ContentsType value) noexcept
{
	return (value >= ContentsType::ATTENDANCE && value < ContentsType::MAX);
}

inline bool IsValidContentsState(ContentsState value) noexcept
{
	return (value >= ContentsState::RED_DOT && value < ContentsState::MAX);
}

inline bool IsValidCanItemDrop(CanItemDrop value) noexcept
{
	return (value >= CanItemDrop::DEFAULT && value < CanItemDrop::MAX);
}

inline bool IsValidAutoDropItemGrade(AutoDropItemGrade value) noexcept
{
	return (value >= AutoDropItemGrade::ALL && value < AutoDropItemGrade::MAX);
}

inline bool IsValidAutoAttackRange(AutoAttackRange value) noexcept
{
	return (value >= AutoAttackRange::METER_20 && value < AutoAttackRange::MAX);
}

inline bool IsValidMannerMode(MannerMode value) noexcept
{
	return (value >= MannerMode::ON && value < MannerMode::MAX);
}

inline bool IsValidDevicePlatformType(DevicePlatformType value) noexcept
{
	return (value >= DevicePlatformType::NONE && value < DevicePlatformType::MAX);
}

inline bool IsValidUserStatus(UserStatus value) noexcept
{
	return (value >= UserStatus::USER_STATE_NORMAL && value < UserStatus::MAX);
}

inline bool IsValidHiveNotificationType(HiveNotificationType value) noexcept
{
	return (value >= HiveNotificationType::TAKE_DAMAGE_IN_PVP && value < HiveNotificationType::MAX);
}

inline bool IsValidEnterWorldServerType(EnterWorldServerType value) noexcept
{
	return (value >= EnterWorldServerType::NONE && value < EnterWorldServerType::MAX);
}

inline bool IsValidRewardGiveType(RewardGiveType value) noexcept
{
	return (value >= RewardGiveType::NONE && value < RewardGiveType::MAX);
}

inline bool IsValidPartyType(PartyType value) noexcept
{
	return (value >= PartyType::NORMAL && value < PartyType::MAX);
}

inline bool IsValidDungeonCategory(DungeonCategory value) noexcept
{
	return (value >= DungeonCategory::NORMAL && value < DungeonCategory::MAX);
}

inline bool IsValidEnterWorldFailReason(EnterWorldFailReason value) noexcept
{
	return (value >= EnterWorldFailReason::EWFR_NONE && value <= EnterWorldFailReason::EWFR_DUPLICATION_USER);
}

inline bool IsValidDuelResult(DuelResult value) noexcept
{
	return (value >= DuelResult::NONE && value <= DuelResult::LOSE);
}

inline bool IsValidHiveChannel(HiveChannel value) noexcept
{
	return (value >= HiveChannel::C2S && value <= HiveChannel::STE);
}

inline bool IsValidHiveMarket(HiveMarket value) noexcept
{
	return (value >= HiveMarket::TS && value <= HiveMarket::ST);
}

inline bool IsValidDropCheckType(DropCheckType value) noexcept
{
	return (value >= DropCheckType::MAP && value < DropCheckType::MAX);
}

inline bool IsValidInvadeDropType(InvadeDropType value) noexcept
{
	return (value >= InvadeDropType::NORMAL && value < InvadeDropType::MAX);
}

inline bool IsValidCommonEffectType(CommonEffectType value) noexcept
{
	return (value >= CommonEffectType::NONE && value < CommonEffectType::MAX);
}

inline bool IsValidPKBookResult(PKBookResult value) noexcept
{
	return (value >= PKBookResult::KILL && value < PKBookResult::MAX);
}

inline bool IsValidBookMarkType(BookMarkType value) noexcept
{
	return (value >= BookMarkType::POSITION && value < BookMarkType::MAX);
}

inline bool IsValidPKContributorType(PKContributorType value) noexcept
{
	return (value >= PKContributorType::USER && value < PKContributorType::MAX);
}

inline bool IsValidPKBookAction(PKBookAction value) noexcept
{
	return (value >= PKBookAction::REVENGE && value < PKBookAction::MAX);
}

inline bool IsValidPKNotifyType(PKNotifyType value) noexcept
{
	return (value >= PKNotifyType::NORMAL && value < PKNotifyType::MAX);
}

inline bool IsValidDeathPenaltyType(DeathPenaltyType value) noexcept
{
	return (value >= DeathPenaltyType::NONE && value < DeathPenaltyType::MAX);
}

inline bool IsValidPersonalTradingNoticeType(PersonalTradingNoticeType value) noexcept
{
	return (value >= PersonalTradingNoticeType::ACCEPTED && value < PersonalTradingNoticeType::MAX);
}

inline bool IsValidPersonalTradingStatus(PersonalTradingStatus value) noexcept
{
	return (value >= PersonalTradingStatus::NONE && value < PersonalTradingStatus::MAX);
}

inline bool IsValidSummonMileageActionType(SummonMileageActionType value) noexcept
{
	return (value >= SummonMileageActionType::NONE && value < SummonMileageActionType::MAX);
}

inline bool IsValidSummonMileageDataType(SummonMileageDataType value) noexcept
{
	return (value >= SummonMileageDataType::NONE && value < SummonMileageDataType::MAX);
}

inline bool IsValidServerMigrateType(ServerMigrateType value) noexcept
{
	return (value >= ServerMigrateType::NONE && value < ServerMigrateType::MAX);
}

inline bool IsValidServerMigrateLimitStatus(ServerMigrateLimitStatus value) noexcept
{
	return (value >= ServerMigrateLimitStatus::NONE && value < ServerMigrateLimitStatus::MAX);
}

inline bool IsValidServerMigrateConditionType(ServerMigrateConditionType value) noexcept
{
	return (value >= ServerMigrateConditionType::NONE && value < ServerMigrateConditionType::MAX);
}

inline bool IsValidBattleArenaTier(BattleArenaTier value) noexcept
{
	return (value >= BattleArenaTier::NONE && value < BattleArenaTier::MAX);
}

inline bool IsValidContentsBindingType(ContentsBindingType value) noexcept
{
	return (value >= ContentsBindingType::USER && value < ContentsBindingType::MAX);
}

