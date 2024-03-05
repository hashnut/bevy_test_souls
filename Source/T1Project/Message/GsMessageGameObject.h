#pragma once

#include "Classes/GsMessageHandler.h"
#include "Classes/GsMessageHandlerOneParam.h"

// bak1210
// 게임오브젝트와 연관된 메시지는 여기에 정의한다.
UENUM()
enum class MessageGameObject : uint16
{

	COMMON_VEHICLE_RIDE,
	COMMON_TELEPORT,
	COMMON_SPAWN,
	COMMON_DESPAWN,
	COMMON_DIE,
	COMMON_HIT,

	COMMON_TARGET_CLEAR,			// Hidden(은신) 상태가 되면 은신 상태가 된 Target을 Clear

	COMMON_TRIGGER_EVENT_UPDATE,

	REMOTEPLAYER_DIE,

	LOCAL_HIT,
	LOCAL_DIE,
	LOCAL_DEAD,
	LOCAL_REVIVE,
	LOCAL_TARGET_CHANGED,
	LOCAL_TARGET_CLEAR,
	LOCAL_TARGET_CLEAR_FORCE,				// 외부 요인으로 인해 강제로 타겟을 클리어
	LOCAL_SKILL_START,
	LOCAL_SKILL_CANCELABLE,
	LOCAL_SKILL_END,
	LOCAL_COOLTIME_START,
	LOCAL_COOLTIME_END,
	LOCAL_INPUT_MOVE_END,
	LOCAL_SPAWN_ME_COMPLETE,
	LOCAL_SPAWN_ME_ACTION,
	LOCAL_REACTION,
	LOCAL_SPAWN_FADE_IN_END,			// 로컬 스폰 페이드 인 연출 끝 시점
	LOCAL_SPAWN_END_WARP,				// LocalPlayer 워프 연출 끝났을 때
	LOCAL_SKILL_START_FOR_EFFECT,
	LOCAL_ABNORMALITY_ADD,
	LOCAL_ABNORMALITY_REMOVE,
	LOCAL_ABNORMALITY_UPDATE,
	LOCAL_INPUT_TARGET_CHANGE,

	LOCAL_WARP,							// 로컬 OnWarp 호출시(서버 소셜 액션으로 워프 연출 시작)
	LOCAL_CONTENTS_WARP_REQUEST,		// 컨텐츠(월드맵, 지정위치 순간이동, 파티장이동...)에서 워프를 요청할때

	// LocalPlayer Skill Update 관련
	LOCAL_UPDATE_SKILL_LEAN,			// 새로 배운 스킬이 존재
	LOCAL_CHANGE_SKILL_SET,				// 스킬셋 교체 이벤트
	LOCAL_AUTOSKILL_UPDATE,				// 자동 사용 설정 스킬 사용
	LOCAL_UPDATE_SKILL_BONUS_LEVEL,		// 스킬 보너스 레벨 변경 시

	LOCAL_CHANGE_SKILL_SLOT,			// 스킬 슬롯 정보 바뀜
	LOCAL_CHANGE_SKILL_SLOT_EQUIP,		// 스킬 슬롯 정보 Equip 바뀜
	LOCAL_CHANGE_SKILL_SLOT_AUTO,		// 오토 세팅 정보 바뀜
	LOCAL_CHANGE_CHAIN_SKILL,			// 체인 스킬 변경
	LOCAL_CHANGE_BUFFSKILL_SLOT,		// 버프 스킬 슬롯 변경
	LOCAL_CHANGE_SKILL_CYCLE,			// 스킬 주기 정보 변경

	LOCAL_AUTOMOVE_REQUESTED,			// auto move controller->MoveTo requested
	LOCAL_AUTOMOVE_IGNORED,				// auto move ignored
	LOCAL_AUTOMOVE_STOP,				// auto move stop

	LOCAL_CAMP_HIT,						// 캠프와 충돌

	AI_MODE_FIRST_BATTLE,				// 선공 on, off
	//AI_MODE_ATTACK_ORDER,				// 공격 우선순위 on, off
	AI_MODE_ACTIVE,						// ai on, off(후처리)
	AI_MODE_ACTIVE_TOGGLE,				// ai 모드 on, off 토글(입력, 치트)
	AI_MODE_CONTENTS_CHANGE,			// 컨텐츠 변경 casting: EGsAIContentsType
	AI_MODE_SAVE_AUTO_POSITION,			// 위치 정보 저장
	AI_MODE_RETURN_START_POS_TYPE,		// 되돌아오기 모드 변경
	AI_MODE_CONUTER_ATTACK,				// 반격 on, off
	AI_STOP_CHECK_TOGGLE,				// AI STOP(연출 막는 플래그) 처리할지 말지 토글
	AI_AUTO_LOOT_DROP_ITEM_GRADE,		// 2021/10/26 PKT - 줍기 등급( 서버 옵션으로 변경 되면서 메세지 추가 )		

	AI_MOVE_REQUEST,

	AI_DRAW_SEARCH_RANGE,				// 검색 반경 그릴지
	AI_AUTO_MOVE_FAIL,					// 자동 이동 실패 출력할지

	AI_QUEST_AUTO_MOVE_START,			// 퀘스트 자동 이동 시작
	AI_QUEST_AUTO_MOVE_END,				// 퀘스트 자동 이동 끝
	AI_QUEST_AUTO_MOVE_STOP,			// 퀘스트 자동 이동 중지(퀘스트 HUD에서 TOGGLE로 끔)
	AI_QUEST_AUTO_BUTTON_CLICK,			// 퀘스트 오토버튼 클릭 이벤트
	AI_QUEST_AUTO_ACTIVE,				// 퀘스트 자동 진행 on/off


	AI_MOVE_NEXT_MAP_WARP,				// 자동으로 다음맵 가는 워프로 이동

	AI_CLEAR_RESERVE_JOB,				// clear reserve job
	AI_CLEAR_RESERVE_ALL_JOB_BY_TYPE,	// clear reserve all job by type
	AI_ADD_RESERVE_JOB,					// add reserve job

	DEBUG_AI_RESERVE_LIST_SHOW,				// 디버그용 예약 리스트 SHOW

	DEBUG_AI_RESERVE_LIST_UPDATE,			// 디버그용 예약 리스트 갱신

	AUTO_MOVE_CANT_FIND_NEXTMAP_WARP,	// 다음맵 워프 못찾음 https://jira.com2us.com/jira/browse/C2URWQ-6497
	
	AUTO_MOVE_CAMP_HIT_RESTART,			// 캠프 HIT후 이동 재시작시(예약된 정보 삭제시 사용) https://jira.com2us.com/jira/browse/C2URWQ-6363

	AUTO_MOVE_CONTENTS_TYPE_CHANGED,	// 자동 이동 타입 변경되었다 https://jira.com2us.com/jira/browse/C2URWQ-3193

	UPDATE_USER_SKILL,
	UPDATE_USER_DATA,
	UPDATE_USER_STATUS,
	UPDATE_USER_PARTS,
	CLEAR_SKILL_DATA,			
	UPDATE_USER_PRESET_ID,				// 프리셋 ID 변경
	UPDATE_USER_PRESET_EQUIP,			// 프리셋 ID 변경 후 장비 상태 변경

	MINIMAP_SPAWN_ICON,						// 로컬, remote, monster등 서버 스폰 기준 아이콘 생성(gameObject)
	MINIMAP_DESPAWN_ICON,					// 로컬, remote, monster등 서버 스폰 기준 아이콘 삭제(gameId)
	MINIMAP_DESPAWN_OBJ_ALL_ICON,			// OBJ 아이콘만 삭제
	MINIMAP_DESPAWN_ALL_ICON,				// 모든 아이콘 삭제
	MINIMAP_SPAWN_TYPE_ICON,
	MINIMAP_NORMAL_MOSNTER_ON_OFF,			// 미니맵 일반 몬스터 ON, OFF
	MINIMAP_SET_SAFE_ZONE,					// 안전지역 텍스트 표시
	MINIMAP_SET_CHANNEL,					// 채널표시
	MINIMAP_SPACE_CRACK_BUTTON_ACTIVE,		// 미니맵 시공의틈새 (버튼 활성화)	 
	MINIMAP_CONTRIBUTION_BUTTON_ACTIVE,		// 기여도 버튼 활성화
	MINIMAP_ELITE_DUNGEON_ACTIVE,			// 시련의 전당 Enter/Exit
	MINIMAP_SPAWN_INTER_PORTAL,				// 인터서버 포탈 스폰
	MINIMAP_DESPAWN_INTER_PORTAL,			// 인터서버 포탈 디스폰
	MINIMAP_AUTO_MOVE_PATH_ICON_INFO,		// 자동 이동 경로 아이콘 정보(추가 or 삭제)

	REGIONMAP_SPAWN_SERVER_ICON,			// 지역맵: 스폰 서버 아이콘
	REGIONMAP_DESPAWN_SERVER_ICON,			// 지역맵: 디스폰 서버 아이콘

	INTERACTION_BUTTON_CLICK,				// 인터랙션 버튼 클릭
	INTERACTION_DEBUG_DRAW_RANGE,			// (디버그)인터랙션 범위 그리기 on, off
	INTERACTION_CONTENTS_BY_NPC,			// 인터랙션에서 각종 컨텐츠로(NPC)
	INTERACTION_CONTENTS_BY_NPC_START,
	INTERACTION_CONTENTS_BY_QUEST_CANCEL,	// Quest 인터랙션이 조건에 부합하지 않아 캔들됨
	INTERACTION_CONTENTS_BY_PROP_START,		// Prop 인터랙션 시작 요청
	INTERACTION_CONTENTS_BY_PROP_END,		// Prop 인터랙션 종료 요청
	INTERACTION_CONTENTS_BY_PROP_CANCEL,	// Prop 인터랙션 켄슬됨

	INTERACTION_NPC_TABLE_ID,				// 인터랙션 NPC 테이블 ID

	PROP_DESPAWN,							// 프랍 오브젝트 삭제

	DROPOBJECT_SPAWN_FINISH,				// 드랍 오브젝트 스폰 연출 끝
	DROPOBJECT_PICK,						// 드랍 오브젝트 메시 픽
	DROPOBJECT_DESPAWN,						// 드랍 오브젝트 삭제
	LOOTING_BUTTON_CLICK,					// 루팅 버튼 클릭 (이건 다른 메세지 있는지 확인 할것)

	CLIENT_SPAWN_LIST_LOAD_FINISH,			// 클라이언트 스폰 리스트 로드 완료

	QUEST_ADD,								// 퀘스트 습득
	QUEST_ACCEPT,							// 퀘스트 수락
	QUEST_UPDATE,							// 퀘스트 갱신
	QUEST_COMPLETE_REWARD,					// 퀘스트 보상
	QUEST_UIACTION_TOUCH,					// 퀘스트 자동이동 on/off
	QUEST_UIACTION_TELEPORT,				// 퀘스트 자동이동 on/off
	QUEST_AUTOPROGRESS_REQUEST,				// 퀘스트 자동이동 요청
	QUEST_UIACTION_NPC_TO_TELEPORT,			// 서브 퀘스트 습득 전 npc 워프
	QUEST_UIACTION_NPC_TO_AUTOPROGRESS,		// 서브 퀘스트 습득 전 npc로 자동이동
	QUEST_POPUP_ACCEPT,						// 퀘스트 수락 팝업
	QUEST_GUIDE_POPUP_ACCEPT,				// 퀘스트 가이드 수락 팝업
	QUEST_GUIDE_POPUP_REWARD,				// 퀘스트 가이드 보상 팝업
	QUEST_GUIDE_AUTO_PROGRESS,				// 퀘스트 가이드 바로가기
	QUEST_REPEAT_REWARD_REFRESH,			// 퀘스트 반복 보상 새로고침
	QUEST_REPEAT_REACCEPT,					// 퀘스트 반복 새로고침으로 재 습득 (동일 인덱스)
	QUEST_REPEAT_REWARD_BOX_RECEIVE,		// 랜덤 리워드 박스 정보 받음////
	
	QUEST_TARGET_NPC,						// 퀘스트 타겟(NPC)
	QUEST_TARGET_MONSTER,					// 퀘스트 타겟(몬스터)
	QUEST_TARGET_POS,						// 퀘스트 타겟(위치)
	QUEST_TARGET_PROP,						// 퀘스트 타겟(프랍)
	QUEST_SUB_INTERATION_NPC,				// 퀘스트 서브 인터랙션 npc
	QUEST_SUB_INTERATION_NPC_LIST,			// 퀘스트 서브 인터랙션 npc List
	QUEST_WINDOW_OPEN,						// 퀘스트 윈도우 OPEN
	QUEST_WINDOW_UPDATE,					// 퀘스트 윈도우 업데이트
	QUEST_REPEAT_WINDOW_UPDATE,				// 퀘스트 윈도우 업데이트
	QUEST_WINDOW_UPDATE_OBJECTIVE,			// 퀘스트 윈도우 수행진행 업데이트
	QUEST_WINDOW_GUIDE_TAB_CLOSE,			// 퀘스트 윈도우 가이드 탭 닫힐 때

	QUEST_ACTIVE_OFF,						// 퀘스트 액티브 끄기(다른 컨텐츠에서)
	
	ABNORMALITY_UPDATE_NOT_LOCAL,			// 상태이상 갱신(LOCAL제외)
	PASSIVITY_ADD,							// 패시브 추가됨
	PASSIVITY_REMOVE,						// 패시브 제거됨
	PASSIVITY_UPDATE_COLLECTION,			// 콜렉션 패시브 정보 갱신됨, FGsPrimitiveInt32 사용
	ABNORMALITY_ALL_UPDATE,					// 상태이상 갱신(모두)
	
	WAYPOINT_MOVE_END,						// 웨이 포인트 이동 끝

	UI_MAP_PARTY_MEMBER,					// UI 맵: 파티 멤버 추가, 삭제

	UPDATE_PARTY_MEMBER,					// party member changed

	ADD_PARTY_MEMBER,						// 2022/07/27 PKT - 파티 멤버 추가

	UI_MAP_SPACE_CRACK_PORTAL,				// UI 맵: 시공의 틈 추가, 삭제

	UI_MAP_INVASION_NPC,					// UI MAP: INVASION NPC ICON
	
	CAMERA_DIALOG_START,					// 카메라 대화 연출 시작
	CAMERA_DIALOG_END,						// 카메라 대화 연출 끝
	CAMERA_DIALOG_START_LERP_END,				// 카메라 대화 보간 끝
	CAMERA_DIALOG_END_LERP_END,					// 카메라 대화 보간 끝
	CAMERA_DIALOG_MODE_VIEW_CHANGE,				// 대사  카메라 모드 뷰 변경
	CAMERA_EQUIP_VIEW_START,					// equip view start
	CAMERA_EQUIP_VIEW_END,						// equip view end

	TERRITORY_ENTERED,						// 펜스 진입 
	TERRITORY_LEFT,							// 펜스 퇴장		
	ENV_TERRITORY_ENTERED,					// 환경 펜스 진입 
	ENV_TERRITORY_LEFT,						// 환경 펜스 진입 
	SOUND_TERRITORY_ENTERED,				// 퀘스트 조건에 따라 사운드 플레이하는 펜스 진입 
	SOUND_TERRITORY_LEFT,					// 퀘스트 조건에 따라 사운드 플레이하는 펜스 나감

	BOOKMARK_ALL_LIST_CHANGE,				// 북마크 모든 리스트 체인지
	BOOKMARK_SEL_LIST_CHANGE,				// 북마크 선택 리스트 체인지

	PARTY_SETTING_ALL_LIST_CHANGE,			// 파티 설정 모든 리스트 갱신
	PARTY_SETTING_ONE_LIST_CHANGE,			// 파티 설정 하나 리스트 갱신

	PARTY_HUD_ALL_LIST_CHANGE,				// 파티 HUD 모든 리스트 갱신
	PARTY_HUD_ONE_LIST_CHANGE,				// 파티 HUD 하나 리스트 갱신

	PARTY_SKILL_TARGET_ALL_LIST_CHANGE,		// PARTY SKILL HUD ALL CHANGE
	PARTY_SKILL_TARGET_ONE_LIST_CHANGE,		// PARTY SKILL HUD ONE CHANGE

	PARTY_HUD_SIDE_TAB_UPDATE,				// 파티 HUD 사이드 탭 갱신
	PARTY_HUD_INFO_UPDATE,					// party info update
	PARTY_BREAKS_UP,						// 2022/06/19 PKT - 파티 깨진 상황
	PARTY_KICK,								// 2022/06/19 PKT - 파티 에서 추방 당함

	PLAYER_STAT_UPDATE,						// 플레이어 스탯 갱신

	BUDDY_PACKET_RESULT_ERROR,				// 친구 패킷 에러처리

	AUTO_MOVE_FAILED,						// 자동이동 실패(1. 도착 했는데 대상 NPC 없음, 2. 이동 자체 실패(못가는곳))
	
	ENV_START_EVENT,						// ENV Start 이벤트 (FGsPrimitiveInt32 사용)
	ENV_END_EVENT,							// ENV End 이벤트 (FGsPrimitiveInt32 사용)

	ENV_SEPARATE_LIGHT_SOURCE_START,						// ENV: 대화 연출 시작
	ENV_SEPARATE_LIGHT_SOURCE_END,							// ENV: 대화 연출 종료

	TEST_CLIENT_AUTOMOVE,					// 테스트 클라이언트 지원용 메시지
	TEST_CLIENT_WARP,						// 테스트 클라이언트 지원용 메시지

	PLAYER_RESURRECT,						//Player resurrect

	SpectatorTerrainTouchEffect,			// 2021/10/22 PKT - 직접 Touch 말고 간접적으로 지형 터치 이펙트를 보이고 싶을때 사용(개발용으로 만들었지만 따로 사용 할 수 도 있기 때문에 define처리 암함)

	REGIONMAP_SPAWN_TREASUREMONSTER_ICON,	// 지역맵: 스폰/디스폰 로컬 플레이어 보물 몬스터 아이콘
	REGIONMAP_UPDATE_TREASUREMONSTER_ICON,	// 지역맵: 로컬 플레이어 보물 몬스터 포지션 갱신

	OBJ_WARP_START,

	TALKBALLOON_COMMUNITY_RANK,				// 커뮤니티 소녀 말풍선 랭킹 정보
	TALKBALLOON_COMMUNITY_MARKET,			// 커뮤니티 소녀 말풍선 거래소 정보

	SPAW_REMOTE_PLAYER,						// 2022/09/29 PKT - Local이 아닌 타 유저 Spawn 메세지
	DESPAW_REMOTE_PLAYER,						// 2022/09/29 PKT - Local이 아닌 타 유저 DESpawn 메세지

	INTERACTION_CONTENTS_BY_INVASIONPROP_REGISTER, // 프랍에 자신을 등록
	INTERACTION_CONTENTS_BY_INVASIONPROP_START,	// Prop invasion 요청
	INTERACTION_CONTENTS_BY_INVASIONPROP_END,	// Prop Invasion 처리 성공
	INTERACTION_CONTENTS_BY_INVASIONPROP_CANCEL,

	REGIONMAP_SPAWN_INVASIONPORTAL_ICON,
		
	UI_CLICK_REGION_MAP_TYPE_TAB,						// 지역맵에서 맵 타입 탭 클릭

	UI_MAP_MONSTER_DROP_ICON,					// 몬스터 스폰 위치에 대한 아이콘
	UI_MAP_MONSTER_DROP_ICON_TOGGLE,			// 몬스터 스폰 위치 아이콘 토글
	UI_MAP_MONSTER_DROP_ACTIVE,					// 몬스터 스폰 위치 아이콘 활성화

	LOCAL_UPDATE_SPIRIT_SHOT,
	LOCAL_UPDATE_AUTO_POTION,

	UPDATE_SCAN_INFO,	// 스캔 정보 갱신

	//----------------------------------------------------------------------------------------------
	// GameObject 클래스 객체내에 포함되어있는 Meesage 객체에 대한 정의
	// 자신의 SubClass에서 주로 사용하려는 용도
	// 왠만한 정보는 상태 변경(StateEnter) 메세지로 처리가 될것으로 판단됨
	//----------------------------------------------------------------------------------------------
	ChangeTarget,
	StateEnter,				// 상태 진입
	StateReEnter,			// 동일 상태 재진입
	StateFailed,			// 상태 변경에 실패
	StateExit,				// 상태 탈출

	ENTER_QUEST_NPC_FENCE,	//퀘스트 NPC 근처 메시지
	LEAVE_QUEST_NPC_FENCE, //퀘스트 NPC 떠남 메시지
};

using	MsgGameObjHandle		= TPair<MessageGameObject, FDelegateHandle>;
using	MsgGameObjHandleArray	= TArray<MsgGameObjHandle>;

using	MGameObject				= TGsMessageHandlerOneParam<MessageGameObject, const struct IGsMessageParam*>;