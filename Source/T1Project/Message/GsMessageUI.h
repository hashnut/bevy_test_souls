#pragma once

// bak1210
// 특정 컨텐츠 UI 메시지가 아닌 제너릭한 메시지 핸들링을 처리힌다.
// 
#include "Classes/GsMessageHandlerOneParam.h"

UENUM()
enum class MessageUI : uint16
{
	RECOVER_TO_DEFAULT_STATE= 0, // 창이 닫히며 스택에 복원할 창이 없을때 불림. InGame일 경우 HUD State로 보내기 위함.

	REQUEST_UPDATE_SKILL_SLOT, // 버튼 하나갱신. param: FGsUIMsgParamInt	
	INVALIDATE_SKILL_SLOT, // HUD 스킬버튼 갱신. param: FGsUIMsgParamUpdateSkillSlot
	RESERVE_SKILL_SLOT, // 스킬예약 표시. param: FGsUIMsgParamInt
	CANCEL_RESERVE_SKILL_SLOT, // 스킬예약 표시 취소. param: FGsUIMsgParamInt		
	REQUEST_SKILL_AUTO, // 오토 토글, param: FGsUIMsgParamRequestSkillAuto
	SKILL_ENCHANT_EFFECT_START, // 스킬 강화 시작(연출용), param: FGsUIMsgParamEnchantSkill
	SKILL_ENCHANT_EFFECT_END_PROGRESS, // 스킬 강화 PROGRESS 연출 끝남, param: nullptr
	SKILL_ENCHANT_EFFECT_RESULT, // 스킬 강화 결과(연출용), param: FGsUIMsgParamSkillEnchantResult	
	
	SKILL_UPDATE_REDDOT, // 스킬창 레드닷 업데이트 수행 

	INVALIDATE_SLOT_AQUIRE_ITEM, // 원거리 소모품 
	INVALIDATE_SLOT_AQUIRE_STAT, // 스텟
	DIMMED_SKILLBUTTON_ONCLICK, // 딤드처리된 HUD 스킬 버튼 터치상태
	
	CHANGE_REGION_MAP,			// 지역 맵 지도 변경
	CHANGE_REGION_MAP_SCENE,			// 지역 맵 지도 변경(씬 타입으로 고정)
	SELECT_REGION_SCENE_PORTAL,	//region scne 포탈 선택
	SELECT_REGION_SCENE_BOOKMARK,	//region scne 포탈 선택

	REWARD_RANDOM_ITEMBAG_INDEX,	// 랜덤 보상 받았을 때 ItemBagIndex
	QUEST_REWARD_FAIL,				// 퀘스트 보상 받기 실패
	
	BUDDY_ADD,					// 친구 추가 요청
	BUDDY_ADD_CANCEL,			// 친구 추가 삭제
	BUDDY_DELETE,				// 친구 삭제
	BUDDY_INVITE_ACCEPT,		// 친구 요청 수락
	BUDDY_INVITE_REFUSE,		// 친구 요청 거절	,
	BUDDY_WHISPER,				// 친구 귓속말
	BUDDY_INVITE_PARTY,			// 친구 파티 초대
	BUDDY_UPDATE_LIST,			// 친구 목록 갱신
	
	INVALID_PENALTY,			//Penalty 갱신 
	BLESS_ARCAS_POINT,			// 축복 수치 갱신
	
	COSTUME_CHANGE_SELECTION,	// Costume 선택 변경
	COSTUME_CHANGE_WEARON,		// Costume 착용 변경
	COSTUME_UPDATED,			// 새로운 코스춤 추가
	COSTUME_COLLECTION_UPDATE,
		
	ENABLE_CAMERA_BUTTON,		 // 카메라 버튼 보이기 여부

	PLAY_MOVIE,					// param: nullptr
	STOP_MOVIE,					// param: nullptr
	HIDE_UI_FOR_MOVIE,			// param: FGsUIMsgParamBool

	RECEIVE_CHANNEL_LIST,		// 채널 리스트 받음. param: nullptr

	CHANGE_HUD_MODE,			// HUD 모드 변경. param: FGsUIMsgParamInt (EGsUIHUDMode 값)
	SET_NAVIGATION_BAR_CURRENCY,	// param: FGsUIMsgParamInt
	SET_NAVIGATION_BAR_ITEM,		// param: FGsUIMsgParamInt
	HIDE_UI,					// UI 감춤. param: FGsUIMsgParamUInt (EGsUIHideFlags 값)
	SET_HITTESTINVISIBLE,		// 2021/10/21 PKT - 화면상 모든 UI SET_HITTESTINVISIBLE 설정

	TITLE_DOWNLOAD_TEXT,		// 타이틀화면 텍스트, FGsUIMsgParamText
	TITLE_DOWNLOAD_PERCENT,		// 타이틀화면 Percent, FGsUIMsgParamFloat
	TITLE_CLICK_START,			// 타이틀 화면 터치
	TITLE_CLICK_HIVE_LOGOUT,	// 하이브 계정 로그아웃

	CHARACTER_SLECT_EMPTY,		// 캐릭터 선택화면에서 캐릭터 삭제 후 갱신

	INIT_BOOK_MARK_ALL_INFO,			// 북마크 모든 정보 받기

	PREV_LOAD_MAP, // FCoreUObjectDelegates::PreLoadMap 시 
	POST_LOAD_MAP, // FCoreUObjectDelegates::PostLoadMapWithWorld 시

	AUTO_SCAN_ACTIVE,					// 자동 스캔 액티브
	SCAN_CLICK,							// 스캔 클릭
	OPTION_UI_UPDATE,					// 옵션 UI 업데이트
	OPTION_SELECT_LANGUAGE,				// 옵션 언어 선택 시. param: FGsUIMsgParamInt
	OPTION_SAVE_TO_SERVER_COMPLETED,	// 옵션 서버에 저장 완료
	OPTION_SAVE_TO_CLIENT_COMPLETED,	// 옵션 클라이언트에 저장 완료

	REQUEST_SHORTCUT_CONTENTS,			// 바로가기 컨텐츠 연결
	
	
	SHOW_REVIVE,					//부활 팝업 창 

	GAME_STATISTICS_UPDATE_LIST,	// 통계 리스트 갱신 param: FGsUIMsgParamInt(StatisticsTabId)
	GAME_STATISTICS_UPDATE_GRAPH,	// 통계 그래프 갱신 param: FGsUIMsgParamInt(StatisticsId)

	LEVEL_UP,

	CHANGE_REGION_ICON_FILTERING,	// region icon filtering change

	SKILL_FORCED_DISABLE,			// 스킬 강제막기. param: FGsUIMsgParamBool (T:막기, F: 풀기)

	INVADE_AIRRAID_STATE,			// 침공 내가 공습 시작/종료
	INVADE_MAPSTATE_UPDATE,			// 현재 맴의 침공 가능/불가 상태 업데이트
	//INVADE_STAYTIME_UPDATE,			// 침공중 남은 체류 시간 갱신
	INVADE_INVADER_COUNT,			// 침공가능 맵의 현재 침략자수 갱신
	INVADE_PORTAL_SPAWNSPOT_UPDATE,	// 침공가능 맵의 현재 열린 포털 위치 갱신
	INVADE_INVASIONTIME_UPDATE,		// 침공가능 시간이 됨, 침공 불가능 시간이 됨

	UPDATED_SANCTUM_INFO,			// 성소 정보 갱신됨

	INVALID_BOSS,
	CHANGE_FIELD_BOSS_BOOKMARK,
	INVALID_FEILD_BOSS_DEATH_TIME,

	INVALID_QUEST_BOARD_INDEX,			//퀘스트 인덱스 
	INITIALIZE_QUEST_BOARD,			//퀘스트 게시판 초기화	
	INVALID_QUEST_BOARD,			//퀘스트 게시판에서 퀘스트 수주, 포기 했을 경우 갱신 
	SELECT_QUEST_BOARD,				//퀘스트 게시판 퀘스트 선택
	SORT_QUEST_BOARD,				//퀘스트 정렬

	UI_RESERVED_QUEUE_EMPTY,		// 큐 처리하던 UI 비워짐
	UI_RESERVED_QUEUE_EMPTY_FOR_BM,	// 큐 처리하던 UI 비워짐(BM상점용)

	INVALIDATE_UI_SCALE,		// UIScale 갱신

	OPEN_MIDDLE_SMALL_POPUP,	// 가운데 영역에 팝업 열림 노티. (컨텍스트 UI, 대결 UI 등)

	INVALID_PLAYER_STAT,
	INVALID_PLAYER_STAT_DISTRIBUTION,
	INVALID_BONUS_STAT_ITEM_COUNT,

	KEYBOARD_MAPPING_UI_UPDATE, // 키보드 맵핑 UI 업데이트

	KEYBOARD_MAPPING_HUD_ENABLE,

	TRAY_FADE_EFFECT_ANI_FINISHED,
	INVALIDATE_PRESET,		// 프리셋 변경 알림
	INVALIDATE_PRESET_NAME,	// 프리셋 이름 변경 알림

	FINISH_SUB_DIALOG,				// sub dialog end

	SOCIALMOTION_ENABLE_UPDATE,
	SOCIALMOTION_STATE_UPDATE,

	RECOVER_TO_PREVIOUS_STATE,

	SHOW_WORLD_MAP_ITEM_DROP,
	HIDE_WORLD_MAP_ITEM_DROP, 
		
	PREVENTION_GAME_OVERINDULGENCE,

	SELECT_LEVEL_POPUP_MONSTER_INFO,		// 몬스터 정보 팝업: 레벨 선택

	CHANGED_SUMMON_MILEAGE_DATA,			// 소환 마일리지 정보 변경됨

	CHANGED_STAR_LEGACY_MAGNITUDE_OPEN_INFO,		// 별의 유산 등급 열림 정보 갱신
	CHANGED_STAR_LEGACY_SLOT_RELEASE_INFO,				// 별의 유산 슬롯 해제
	CHANGED_STAR_LEGACY_SLOT_SELECT_INFO,				// 별의 유산 슬롯 선택

	START_STAR_LEGACY_ROULETTE,							// 별의 유산 룰렛 연출 시작
	REROLL_STAR_LEGACY,									// 별의 유산 슬롯 리롤

	CHANGED_STAR_LEGACY_CATEGORY_TAB,					// 별의 유산 카테고리 탭 변경
};

using	MsgUIHandle				= TPair<MessageUI, FDelegateHandle>;
using	MsgUIHandleArray		= TArray<MsgUIHandle>;

using	MUI						= TGsMessageHandlerOneParam<MessageUI, const struct IGsMessageParam*>;