#pragma once

#include "Classes/GsMessageHandler.h"
#include "Classes/GsMessageHandlerOneParam.h"
// bak1210 
// 컨텐츠 레이어의 메시지는 여기에 추가합니다.
// 게임로직에서 활용되는 메시지
// MessageContents 에 Enum 추가 시, EGsGuideQuestWindowType 에도 추가 부탁드립니다.

UENUM()
enum class MessageContents : uint16
{
	ACTIVE_HUD = 0,				// HUD 활성화
	ACTIVE_WINDOW_OPTION,
	ACTIVE_WINDOW_QUEST,
	ACTIVE_WINDOW_SKILL,
	ACTIVE_WINDOW_CONTINENTALMAP,
	ACTIVE_WINDOW_REGIONMAP,
	ACTIVE_WINDOW_FAIRY,
	ACTIVE_WINDOW_NPC_SHOP,
	ACTIVE_WINDOW_GUILD,
	ACTIVE_WINDOW_COSTUME,
	ACTIVE_POPUP_BUDDY,
	ACTIVE_WINDOW_MAIL,
	ACTIVE_WINDOW_ITEM_COLLECTION,
	/*ACTIVE_WINDOW_MAILDETAIL,*/
	ACTIVE_WINDOW_DUNGEON,
	CLOSE_WINDOW_DUNGEON,
	ACTIVE_WINDOW_NPC_DEPOT,
	ACTIVE_WINDOW_MULTI_LEVEL_RANK,
	ACTIVE_WINDOW_MARKET,
	ACTIVE_WINDOW_CRAFT,
	ACTIVE_WINDOW_COMMUNITY,
	ACTIVE_QUICK_MENU, // 햄버거 메뉴 열림
	CLOSE_QUICK_MENU, // 햄버거 메뉴 닫으라고 메시지보냄
	ACTIVE_OFFLINEPLAY,
	ACTIVE_BMSHOP,
	/*ACTIVE_POPUP_BOSS,*/
	ACTIVE_WINDOW_RANKING,
	ACTIVE_SAVE_BATTERY,
	ACTIVE_WINDOW_ACHIEVEMENT,
	ACTIVE_WINDOW_MONSTER_COLLECTION,
	ACTIVE_INVENTORY,
	ACTIVE_TITLE,
	ACTIVE_WINDOW_GAME_STATISTICS,
	ACTIVE_WINDOW_USER_PROFILE,	
	ACTIVE_BOSS,
	ACTIVE_WINDOW_QUEST_BOARD,
	ACTIVE_WINDOW_PK_BOOK,
	CLOSE_WINDOW_PK_BOOK,
	ACTIVE_WINDOW_REPAIR,		// 아이템 장비 복구 Window
	ACTIVE_WINDOW_RECOMPOSE,	// 페어리 및 코스튬 재합성 Window
	ACTIVE_WINDOW_KEYBOARD_MAPPING,	// 윈도우 키보드맵핑 Window
	ACTIVE_WINDOW_INVADE,		// 2023/1/20 PKT - 침공전
	ACTIVE_WINDOW_SEAL,
	ACTIVE_WINDOW_BATTLEPASS,
	ACTIVE_WINDOW_SIEGE_WARFARE,
	ACTIVE_WINDOW_EXCHANGE_CUSTUME,
	ACTIVE_WINDOW_SPIRIT_SHOT,
	ACTIVE_WINDOW_AGIT_DUNGEON,
	ACTIVE_WINDOW_STAR_LEGACY,	

	REQ_ACTIVE_WINDOW_INVADE,
};

UENUM()
enum class MessageContentsLobby : uint8
{
	LOOK_AT_ON,
	LOOK_AT_OFF,
	ZOOM_HEAD,
	ZOOM_WHOLE_BODY,
	UPDATE_UI,
	GAME_ENTER_TEST,
	GAME_ENTER_CLEAR,
	HEIGHT_CHANGE,
};


UENUM()
enum class MessageContentQuest : uint8
{
	QUEST_UIACTION_TOUCH = 0,
	QUEST_UIACTION_TELEPORT,
	QUEST_UIACTION_GIVEUP,
	QUEST_MAIN_READY,
	QUEST_MAIN_ACCEPTED,
	QUEST_MAIN_COMPLETED,
	QUEST_MAIN_REWARDED,
	QUEST_RANK_ACCEPTED,
	QUEST_RANK_COMPLETED,
	QUEST_SUB_ACCEPTED,
	QUEST_SUB_COMPLETED,
	QUEST_SUB_REWARDED,
	QUEST_REPEAT_COMPLETED,
	QUEST_GUIDE_ACCEPTED,
	QUEST_GUIDE_COMPLETED,
	QUEST_GUIDE_REWARDED,
	QUEST_GUIDE_HUD_ONOFF,
	QUEST_GUIDE_INVALID_INSTANCE,
	QUEST_GUIDE_AREA_REFRESH,

	QUEST_EVENT_BEGIN_START,
	QUEST_EVENT_CLEAR_START,
	QUEST_EVENT_FINISHED,
};

UENUM()
enum class MessageContentEvent : uint8
{
	/**
	 * 특정 이벤트액션그룹이나 이벤트액션을 시작하도록 요청함.
	 * 이 메세지를 받으면 이전에 어떤 이벤트액션이 진행중이었던지 상관하지 않고 지정된 파라미터부터 시작함.
	 */
	EVENT_ACTION_PLAY = 0,

	/**
	 * 이벤트액션그룹의 마지막이벤트액션을 완료하면 발생.
	 * 이 이벤트는 EventActionPlay 이벤트와 쌍으로 동작하지만, 최종 호출을 보장하지 않음.
	 * 인터럽트 발생 시, 이 이벤트는 최종 발생하지 않을 수 있음에 주의.
	 */
	LAST_EVENT_ACTION_FINISHED,

	/**
	 * Stage / State 변경 시 Immediate 타입의 이벤트를 종료 시켜야 할 때가 있음.
	 */
	 ALL_CLEAR_EVENT_ACTION_IMMEDIATE,
};

UENUM()
enum class MessageContentContentsEvent : uint8
{
	CONTENTS_EVENT_GROUP_STATE_CHANGED,
	CONTENTS_EVENT_STATE_CHANGED,
	CONTENTS_EVENT_STATE_CHANGED_COMPLETED,
};

// bak1210 : 인벤쪽은 리펙토링 필요 
// (리펙토링시 대문자로 메시지 변경)
// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=263051108
// LSJ : 변경 완료 (사용하지 않거나 필요없는 메세지 제거)
UENUM()
enum class MessageContentInven : uint8
{
	OPEN,
	CLOSE,

	UPDATE_ITEM_WEIGHT,
	UPDATE_ITEM_WEIGHT_PENALTY,
	UPDATE_INVEN_SLOT_COUNT,

	UPDATE_INVEN_SLOT_INDEX,
	
	RESET_SELECTED_ICON,

	UPDATE_INVEN_EXTEND_SCROLL,
};

UENUM()
enum class MessageContentEnchant : uint8
{
	//// ITEM ////
	OPEN = 0,
	CLOSE,
	INVALIDATE,
	INVALIDATE_MULTI,
	INVALIDATE_REFINE,
	INVALIDATE_MAGICALFORGE,
	
	SHOW_ENCHANT_EFFECT,
	SHOW_MAGICALFORGE_EFFECT,

	CLOSE_ALL_RESULT_WIDGET,

	OPEN_EXPECT_REFINE_OPTION,
	CLOSE_EXPECT_REFINE_OPTION,
	INVALIDATE_EXPECT_REFINE_OPTION,

	////////아이템 컬렉션 강화//////////////
	OPEN_COLLECTION,
};

UENUM()
enum class MessageContentsFusion : uint8
{
	// 2023/9/26 PKT - 합성창
	UI_OPEN = 0,
	RECV_FUSTION_RESULT,	// 2023/10/4 PKT - 합성 결과( 성공 만 있음. )
};


UENUM()
enum class MessageContentItem : uint8
{
	///////////ItemDetail/////////////////////
	DETAIL_DISPLAY_ITEM,
	DETAIL_DISPLAY_ITEM_EX,
	DETAIL_DISPLAY_ITEM_BOX,
	DETAIL_DISPLAY_ITEM_TID,
	DETAIL_DISPLAY_CHAR_DETAIL,
	DETAIL_DISPLAY_CURRENCY,
	DETAIL_CLOSE,
	DETAIL_DISPLAY_ITEM_COMPARE,
	DETAIL_OPEN_MARKET,
	DETAIL_ITEM_COLLECTION,
	DETAIL_REGISTER_ITEM_COLLECTION,
	DETAIL_SHIFT_ITEM_COLLECTION,
	DETAIL_ITEM_COLLECTION_TEXT,
	DETAIL_OPEN_POPUP_SUMMONDETAIL,

	///////////ItemDecompose/////////////////
	DECOMPOSE_SINGLE,
	DECOMPOSE_MULTI,
	DECOMPOSE_RESULT,

	///////////QuickSlot/////////////////////
	QUICKSLOT_REGISTER,
	QUICKSLOT_SWAP,
	QUICKSLOT_REMOVE,
	QUICKSLOT_USE,
	QUICKSLOT_AUTOSTATE,
	QUICKSLOT_RESET_SELECTION,
	QUICKSLOT_AUTOSTATE_RECEIVE,

	///////////Depot/////////////////////
	DEPOT_ADD_ITEM,
	DEPOT_INVALIDATE,
	DEPOT_INVALIDATE_ALL,
	DEPOT_REMOVE_ITEM,
	DEPOT_BASKET_ADD,
	DEPOT_BASKET_ADD_LIST,
	DEPOT_BASKET_INVALIDATE,
	DEPOT_BASKET_REMOVE,
	DEPOT_STATE_DEPOSIT,
	DEPOT_STATE_TAKEOUT,
	DEPOT_EXPEND_SLOT,
	DEPOT_INVEN_INVALIDATE,

	///////////PickOne Box/////////////////
	PICKONEBOX_OPEN,
	PICKONEBOX_SELECT,

	///////////Common/////////////////
	INVALIDATE_INVENTORY,
	INVALIDATE_DEPOT,

	///////////Ticket Change NickName/////////////////
	CHANGE_NICKNAME_USE_TICKET,
	CHANGE_NICKNAME_CHECK_REQUEST,
	CHANGE_NICKNAME_CHECK_VALID,
	CHANGE_NICKNAME_CONFIRM_REQUEST,
	CHANGE_NICKNAME_CONFIRM,

	USE_WARP_ITEM,
};


UENUM()
enum class MessageContentItemSkill : uint8
{
	///////////Skill/////////////////
	DETAIL_OPEN_ITEM_SKILL,
};

UENUM()
enum class MessageContentPlayerInfo : uint8
{
	OPEN,
	CLOSE,

	EQUIP,
	EQUIP_ALL,
};


UENUM()
enum class MessageContentStoreBM : uint8
{
	OPEN_TAB,
	OPEN_SPECIAL_TAB,
	OPEN_BMSHOP_HUD,
	OPEN_BMSHOP_PROMOTION,
	OPEN_POPUP_BUY,
	OPEN_BULK_SALE,
	OPEN_POPUP_GROUP_DETAIL,
	OPEN_POPUP_SHOP,
	OPEN_COUPON_LIST,
	OPEN_BMSHOP_VALID_HIVE,

	KICK_Z_PAY_USER,

	INVALIDATE_PRODUCT,
	INVALIDATE_PRODUCT_LIST,
	INVALIDATE_STORAGE,
	INVALIDATE_POPUP_SHOP_PRODUCT,

	CHECK_VALID_PRODUCT,
	CHECK_VALID_RRODUCT_RESULT,
	CHECK_VALID_COUPON,

	USER_CHECK_RESTORE,
	ACCEPT_STORAGE,
	PURCHASE_RESTORE_CHECK,
	NOTHING_TO_RESTORE,
	PURCHASE_SUCCESS,
	SEND_TRANSACTION_FINISH_SINGLE,
	SALES_TIME_OVER,
	POPUP_PRODUCT_TIME_OVER,
	SEARCH_Z_PAY_USER_ACK,
	SEARCH_Z_PAY_USER_NOTIFY,

	DISPLAY_NOTHING_TO_RESTORE,

	CHANGE_TAB,
	CHANGE_TAB_FROM_ID,
	CHANGE_FOCUSING_PRODUCT,	

	REQ_BM_SHOP_ITEM_LIST,
	REQ_BM_SHOP_PREPARE_HIVE_ITEM,
	REQ_BM_SHOP_PURCHASE_ITEM,
	REQ_HIVE_PG_UNCONSUMED_PURCHASE,
	REQ_SAVE_BM_BULK_SALE_LIST,
	REQ_BM_SHOP_POPUP_LIST,
	REQ_BM_SHOP_ITEM_RESTORE,
	REQ_BM_DEPOT_ITEM_LIST,
	REQ_RECIVIE_BM_DEPOT,

	REQ_Z_PAY_PURCHASED_ITEM_LIST,
	REQ_SEACRHING_Z_PAY_USER,
	REQ_PREPARE_Z_PAY_PURCHASE_ITEM,
	REQ_Z_PAY_PURCHASE_ITEM,
	REQ_Z_PAY_ITEM_RESTORE,
	Z_PAY_CANNOT_RESTORE,
	OPEN_PG_PURCHASE_WND,

	SEND_ANLYTICS_EVENT,

	// 인앱결제 상품 중에 잘못 노출된 상품이 있는경우 서버에서 패킷받아 강제로 Close 처리
	CLOSE_BMSHOP_BY_BLOCK,

	// Hive Callback
	HIVE_MARKET_CONNECTION,
	HIVE_PRODUCT_LIST_INFO,
	HIVE_BLOCK_UI_OPEN,
	HIVE_BLOCK_UI_CLOSE,
	HIVE_ERROR,
	HIVE_ERRORS,
	HIVE_NEED_EXIT,

	HIVE_MARKET_CONNECT,
	HIVE_GET_PRODUCT_INFO,
	HIVE_GET_SUBSCRIPTION_PRODUCT_INFO,
	HIVE_RESTORE,
	HIVE_RESTORE_SUBSCRIPTION,
	HIVE_PURCHASE,

	/**
	 * Server migrate
	 */
	 INVALIDATE_SERVERMIGRATE,
};


UENUM()
enum class MessageContentCamera : uint8
{
	GREETING_ON,
	GREETING_OFF,
	START_LERP_ROT,
	DEBUG_DRAW_LOOK_AXIS_LINE,
	CLICK_FIXED_QUARTER_YAW,
	CHANGED_FIXED_QUARTER_YAW,
};


UENUM()
enum class MessageContentNpcShop : uint8
{
	OPEN,
	INVALIDATE_SHOP,
	INVALIDATE_BASKET,	
	INVALIDATE_PRODUCTS,
	INVALIDATE_WEIGHT,

	PURCHASE,
	SALE,

	INSERT_BASKET,
	REMOVE_BASKET,

	CHANGE_MODE,
	UPDATE_PURCHASE_AMOUNT,
};

UENUM()
enum class MessageContentGuild : uint8
{
	RECEIVE_JOIN_SUCCESS = 0, // 가입 완료
	RECEIVE_DISBAND, // 길드 해산됨
	RECEIVE_LEAVE, // 길드 탈퇴함
	RECEIVE_JOIN_APPROVE_RESULT, // 내가 가입 요청을 승인/거절한 결과 받음.
	RECEIVE_OPEN_GUILD, // 내 길드 상세정보 패킷(길드 메인페이지 입장 시)
	RECEIVE_GUILD_LIST, // 길드 리스트를 받음(가입창 열때, 가입창 갱신버튼)
	RECEIVE_JOIN_WAITING_GUILD_LIST, // 내가 신청중인 길드리스트를 받음(가입창)	
	INVALIDATE_GUILD_JOIN_LIST, // 길드 가입창 리스트 갱신 요청

	RECEIVE_MEMBER_LIST, // 길드원 리스트 받음(길드 메인창 길드원탭)
	RECEIVE_CHANGE_MEMBER_GRADE, // 등급 변경됨. FGsGuildMsgParamUser 사용.
	NOTIFY_CHANGE_MEMBER_GRADE, // 등급 변경됨. FGsGuildMsgParamMemberGrade 사용.
	NOTIFY_JOIN_MEMBER, // 길드원 가입노티
	RECEIVE_REMOVE_MEMBER, // 길드원 삭제 메시지(추방, 탈퇴 등)
	RECEIVE_AUTHORITY_LIST, // 등급별 권한 리스트 받음
	RECEIVE_CHANGE_AUTHORITY, // 권한 변경 요청 응답을 받음(길마)
	RECEIVE_CHANGE_MY_AUTHORITY, // 내 권한 변경됨
	RECEIVE_DAILY_ACTIVITY, // 일일활동 결과 받음(출석, 기부 등)
	RECEIVE_JOIN_WAITING_USER_LIST, // 가입신청한 유저 리스트
	NOTIFY_JOIN_REQUEST, // 가입 신청 유저 노티를 받음(권한있는 유저에게만 온다)
	CHANGE_GUILD_SETTING, // 길드 설정(가입방식, 설명글 등)이 변경됨. FGsGuildMsgParamSetting 사용.	
	RECEIVE_BUILDING_LIST, // 빌딩 리스트를 받음(길드 메인창 빌딩탭)
	RECEIVE_BUILDING_UPDATE, // 길드 건물 상태 변경. FGsGuildMsgParamInt 에 buildingId 넣어 보냄.
	RECEIVE_OTHER_GUILD_DETAIL_INFO, // 다른 길드 상세보기 받음
	NOTIFY_CHANGE_NOTICE, // 길드 공지 변경
	CHANGE_GUILD_LEVEL, // 길드 레벨 변경
	CHANGE_CONTRIBUTION, // 길드 공헌도 변경
	
	DIPLOMACY_INVALIDATE_LIST, // 길드 외교 리스트 화면 갱신
	DIPLOMACY_RESEND_LIST, // 해당 외교 페이지의 리스트를 서버에 재요청
	DIPLOMACY_ENEMY_STATE_NOTIFY, // 외교 적대 상태 바뀜. FGsGuildMsgParamDiplomacyNotify 사용
	DIPLOMACY_ALLY_STATE_NOTIFY, // 외교 동맹요청 상태 바뀜. FGsGuildMsgParamDiplomacyNotify 사용

	RECEIVE_QUEST_INFO,	// 퀘스트 정보 받음
	UPDATE_QUEST, // 퀘스트 정보 갱신 - 해당 퀘스트 하나만 갱신해야지
	RECEIVE_QUEST_REFRESH, // 퀘스트 새로고침 받음

	ITEM_COLLECTION_UPDATE_LIST,				// 2021/10/05 PKT - 길드 아이템 콜렉션 상태 변경
	ITEM_COLLECTION_INVAILDATE_STAT_STATUS,	// 2021/10/05 PKT - 완료 상태에 대한 스탯 변경( 갱신 )
	ITEM_COLLECTION_OPEN_POPUP_REWARD,		// 2021/10/27 PKT - 아이템 컬렉션 등록 시 보상 정보창 오픈
	ITEM_COLLECTION_CLEANUP,					// 2021/10/05 PKT - 길드 아이템 컬렉션 리셋
	ITEM_COLLECTION_INVAILDATE_ALL,			// 2021/10/29 PKT - 전체 리스트를 갱신한다.
	ITEM_COLLECTION_OPEN_SHORTCUT,					// 2021/10/29 PKT - 길드 아이템 콜렉션 바로가기

	CHECK_REDDOT,			// 2021/10/29 PKT - 외부 이벤트를 통해 레드닷을 체크 하는 거 말고 내부 오브젝트에서 레드닷 갱신이 필요할 때 보내는 이벤트

	STORAGE_NOTIFY_ACQUIRED,		// 기사단 창고 아이템 획득 알림
	STORAGE_NOTIFY_DISTRIBUTED,	// 기사단 창고 아이템 분배 알림
	STORAGE_EXTEND_SLOT,			// 기사단 창고 인벤토리 슬롯 확장
	STORAGE_INVALIDATE_ALL_DATA,	// 기사단 창고 전체 데이터 갱신
	STORAGE_INVALIDATE_PAGE,		// 기사단 창고 페이지 갱신
	STORAGE_INVALIDATE_INVENTORY,	// 기사단 창고 인벤토리 갱신
	STORAGE_INVALIDATE_DISTRIBUTION, // 기사단 창고 분배 페이지 하단 정보 갱신
	STORAGE_CLOSE,				// 기사단 창고 종료(기사단 메인 UI 갱신용)		

	INVALID_DUNGOEN_TAB,
	DUNGEON_RESET,

	CAMP_CONSTRUCTION, // 기사단 캠프 건설 완료
	CAMP_DESTRUCTION, // 기사단 캠프 파괴/해체 왼료

	SANCTUM_CONSTRUCTION_STATE_CHANGED, // 성소 수호탑 건설 상태 변경(건설 시작, 완료)	
	SANCTUM_DESTRUCTION, // 성소 수호탑 파괴

	STORAGE_INVALIDATE_LIST, // 정산 탭 하위 페이지 목록 갱신

	DIVISION_LIST_CHECKED, // 분배 탭 목록 체크 박스 변경
	DIVISION_INVALIDATE_REDDOT, // 분배 레드닷 갱신
	DIVISION_REGISTER, // 분배 등록 알림
	DIVISION_UPDATE_ITEM, // 분배 아이템 정보 갱신

	AUCTION_LIST_RESET, // 경매 탭 목록 정보 재요청
	AUCTION_PAYBACK_UPDATED, // 경매 회수금 변경 알림
	AUCTION_DETAIL_RECEIVE, // 경매&분배 상세 정보 수신
	AUCTION_INVALIDATE_REDDOT, // 경매 레드닷 갱신
	AUCTION_UPDATE_ITEM, // 경매 아이템 정보 갱신
	AUCTION_REMOVE_ITEM, // 경매 아이템 종료 알림

	AGIT_STATE_CHANGED, // 기사단 영지 상태 바뀜(인자를 넣어서 구분하면 될 것 같다?)
	AGIT_DUNGEON_RAID_INFO, // 기사단 영지 공략 던전 정보 받음
	AGIT_DUNGEON_RAID_DETAIL_INFO, // 기사단 영지 공략 던전 상세 정보 받음
	AGIT_DUNGEON_RAID_OPEN_CHANGED, // 기사단 영지던전 오픈 상태 바뀜

	AGIT_LEAVE, // 영지 나가기
	AGIT_ALERT_DESTROY, // 영지 종료 알림
	AGIT_DUNGEON_RAID_LEAVE, // 영지 공략 던전 나가기 
	AGIT_DUNGEON_RAID_RESULT, // 영지 공략 던전 결과
	AGIT_DUNGEON_RAID_BOOKMARK_LIST, // 영지 공략 던전 즐겨찾기 리스트 받음
	AGIT_DUNGEON_RAID_BOOKMARK_CHANGED, // 영지 공략 던전 즐겨찾기 정보 변경
	AGIT_DUNGEON_RAID_START_NOTIFY, // 영지 공략 던전 시작 노티
	AGIT_DUNGEON_RAID_CLOSE_NOTIFY, // 영지 공략 던전 종료 노티
	AGIT_DUNGEON_RAID_CLEAR_NOTIFY, // 영지 공략 던전 성공 노티
	AGIT_DUNGEON_RAID_YET_RESET, // 리셋 시점에 요청 보냈는데 리셋이 안끝났다고 받은 경우
};

UENUM()
enum class MessageContentMail : uint8
{
	ACTIVE_WINDOW_MAILDETAIL,	//메일 디테일 윈도우창 오픈
	ACTIVE_WINDOW_INVALIDTEALL,//메일함 갱신
	DACTIVE_MAIL_TIMEOVER,		//메일 시간 다됨
	ACTIVE_POPUP_ITEM_RECEIVE, //모든 메일보상
	NOTIFY_MAIL_UNREAD,		//새 메일 Notify
};

UENUM()
enum class MessageContentItemCollection : uint8
{
	SEARCH_MATERIAL,
	FIELD_INVALID,


	OPEN_HUD,		// 2021/08/23 PKT - HUD 를 통해 Item Collection Open
	OPEN_SHORTCUT,	// 2021/08/23 PKT - 바로가기 기능을 통한 Item Collection Open	
	SHORTCUT,		// 2021/08/20 PKT - 아이템 디테일 팝업 -> 아이템 콜렉션 바로 가기 기능
	INVENTORY_INVALIDATE,	// 2021/10/12 PKT - 아이템 콜렉션 인벤토리 갱신
};

UENUM()
enum class MessageContentMaterial : uint8
{
	EXIT,		// 2023/1/13 PKT - UI를 닫음.

};

UENUM()
enum class MessageContentChat : uint8
{
	ADD,
	ADD_GUILD,
	CHANGE_STATE,
	INVALIDATE_BLOCK_LIST,//차단 삭제 후 전체 리스트 갱신
	INVALIDATE,
	INVALIDATE_GUILD_MARK, //챗 유저 정보 창의 guild mark 갱신
	CHECK_EXIST_BY_NAME, //귓속말 유저 정보가 실제하는지 이름으로 체크
	UPDATE_INVASIONCHAT_COUNT, // 남은 침공 채팅 개수 변경
	OPERATE_MESSAGE_UPDATE,		// 오퍼레이팅 메세지 모든 탭에 추가?

	CHATGROUP_LIST,		// 챗 그룹 리스트 
	CHATGROUP_ADD,		// 챗 그룹 추가
	CHATGROUP_REMOVE,	// 챗 그룹 삭제
	CHATGROUP_INVALIDATE,	// 챗 그룹 정보 변경
	CHATGROUP_KICKOUT,	// 챗 그룹에서 추방됨
	CHATGROUP_LEAVE,	// 챗 그룹에서 떠남

	CHATGROUP_MEMBER_LIST, // 챗 그룹 멤버 리스트
	CHATGROUP_MEMBER_INVALIDATE, // 챗 그룹 멤버 변동
	CHATGROUP_MEMBER_ADD,	// 챗 그룹 멤버 추가
	CHATGROUP_MEMBER_REMOVE, // 챗 그룹 멤버 삭제

	CHATGROUP_GUEST_LIST,	// 챗 그룹 초대한 게스트 길드 리스트
	CHATGROUP_GUEST_REMOVE,
	CHATGROUP_GUEST_INVALIDATE, // 챗 그룹 초대한 게스트 길드 리스트 갱신

	CHATGROUP_INVITATION_NOTI, // 챗 그룹 초대장 알림
	CHATGROUP_INVITATION_LIST, // 챗 그룹 초대장 리스트
	CHATGROUP_INVITATION_REMOVE, // 챗 그룹 초대장 삭제
	CHATGROUP_INVITATION_ACCEPTED,
	CHATGROUP_INVITATION_REJECTED,
	CHATGROUP_INVITATION_CANCELD,
	
	CHATGROUP_ROOMNAME_CHANGED, // 챗 그룹 이름 변경
	CHATGROUP_ROOMMASTER_ENTRUSTED, // 챗 그룹 방장 변경
	CHATGROUP_ROOMAUTHGRADE_CHANGED // 챗 그룹 룹 입력 권한 변경
};


UENUM()
enum class MessageContentMarket : uint8
{
	UPDATE_ITEM_LIST, // 리스트 전체 갱신
	RCV_BOOKMARK_LIST, // 즐겨찾기 리스트 받음
	UPDATE_BOOKMARK, // 즐겨찾기 갱신, FGsMarketMsgParamItemId 사용
	RCV_ITEM_PRICE, // 아이템 시세정보 받음
	RCV_SELL_REGISTERED_ITEM, // 판매 등록. FGsMarketMsgParamId 사용
	RCV_SELL_CANCEL_ITEM, // 판매 취소. FGsMarketMsgParamId 사용
	RCV_ITEM_DETAIL_INFO, // 아이템 상세정보 받음. FGsMarketMsgParamId 사용
	UPDATE_RED_DOT, // 레드닷 갱신
	UPDATE_SEARCH, // 검색어 입력 완료. FGsMarketMsgParamString 사용
	UPDATE_FILTER, // 필터선택완료
	RCV_SEARCH_RESULT, // 검색 결과 받음
	REQ_REFRESH, // 새로고침 요청

	ADD_TO_PURCHASE_LIST, // 구매 리스트에 추가(체크박스)
	REMOVE_FROM_PURCHASE_LIST, // 구매 리스트에서 제거 (체크박스)

	RCV_COLLECTION_PRICE_LIST, // 컬렉션에서 거래소 최저 가격 정보 받음. FGsMarketMsgParamId 사용
	RCV_PURCHASE_RESULT, // 구매 결과 받음. FGsMarketMsgParamPurchaseResult 사용
	REQ_REFRESH_COLLECTION, // 해당 컬렉션 페이지에 필요한 갱신 패킷을 보내도록 함

	RCV_ACCUM_TAX_LIST, // 세금정보
};


UENUM()
enum class MessageContentMultiLevelRank : uint8
{
	INITIALIZE_SLOT_ALL, // slot 전체 초기화
	INVALIDATE_SLOT_ALL, // slot 전체 업데이트
	INVALIDATE_REWARD, // reward 업데이트( param : single )
	INVALIDATE_QUEST, // quest 업데이트( param : single )
	CHANGE_FOCUSED_SLOT, // 슬롯 선택을 변경한다.
};

UENUM()
enum class MessageContentOfflinePlay : uint8
{
	INVALIDATE_AVAILABLE_TIME,
};

UENUM()
enum class MessageContentCraft : uint8
{
	// packet
	NOTIFY_CRAFT_STAT_LIST_WORLD_CHANGED,	// 제작 스탯 리스트 변경 알림
	ACK_CRAFT_STAT_LIST,					// 제작 스탯 리스트 수신

	// command
	OPEN_CRAFT_WITH,						// 제작창 초기화
	CHANGE_FAVORITE_TARGET,					// 즐겨찾기 토글

	// event
	CHANGE_FAVORITE_LIST,					// 제작 즐겨찾기 목록 변경
	CHANGE_CRAFT_TARGET,					// 제작 대상 프러퍼티 변경됨
	OPEN_TAB,								// 제작창 탭 열림
	END_SPELLING,							// 제작 연출 종료
	RCV_RESULT,								// 제작 결과 받음
};

UENUM()
enum class MessageContentCommunity : uint8
{
	SELECT_TAB,
	TOUCH_GIRL,
	CHANGE_ACTION,
	REDDOT_UPDATE,
	ACTIVE_WINDOW_INVALIDTEALL,
	FINISH_EVENT_PROGRESS,		// 외부요인으로 이벤트 프로그래스 끝
	UPDATE_EVENT_EXPIRE,
};

UENUM()
enum class MessageContentRanking : uint8
{
	INVALIDATE,
};


//bak1210 : 공통 사용 인벤토리(인벤 병합필요) 
UENUM()
enum class MessageContentCommonInven : uint8
{
	INVALIDATE_ALL,			// 2021/10/12 PKT - Inventory Common을 갱신
	INVALIDATE_SELECTED_EFFECT,	// 2021/10/12 PKT - Inventory Common을 갱신
};

UENUM()
enum class MessageContentSaveBattery : uint8
{
	ENTER,
	EXIT,
	CLOSE,
	CLOSE_ALL,
	CLOSE_UI,
	INVALIDATE_INVEN,
};


UENUM()
enum class MessageContentAchievement : uint8
{
	UPDATE_THEME_ITEM,
	UPDATE_THEME_PROGRESS,
	CHANGE_THEME_LEVEL,
	UPDATE_ACHIEVEMENT_ITEM,
	UPDATE_ACHIEVEMENT_PROGRESS,
	CHANGE_ACHIEVEMENT_LEVEL,
	ACK_RECEIVE_THEME_REWARD,
	ACK_RECEIVE_ACHIVEMENT_REWARD,
	ACK_RECEIVE_REWARDALL,
};

UENUM()
enum class MessageContentMonsterKnowledge : uint8
{
	INVALIDATE_BOOK,					// 2023/2/6 PKT - 도감 북
	INVALIDATE_BOOK_SET,				// 2023/2/6 PKT - 도감 북

	INVALIDATE_SLATE,					// 2023/1/31 PKT - Slate 갱신
	SHOTCUT_SLATE,						// 2023/1/31 PKT - Slate 바로가기
	SPECIAL_NODE_EFFECT,				// 2023/12/08 ejrrb10 - 특수 노드 결과를 서버로부터 수신 받음

	INAVLIDATE_SLATE_CURRENCY,			// 2023/11/10 PKT - Currency / Item 등 Local Player가 소지하고 있는 상태가 변경

	USE_ITEM_SLATE,						// 2021/12/22 PKT - 지식의 석판 아이템을 사용하겠다.(지식의 석판 UI로 바로가기 기능)
	OPEN_UI_SLATE_USE,					// 2021/12/22 PKT - 아이템 사용을 위해 UI를 Open

	//EVENT_REDDOT_CATEGORY,				// 2022/01/07 PKT - 몬스터 일람 레드닷 관련
	//INVALIDATE_SLATE_CATEGORY,			// 2022/01/07 PKT - 몬스터 석판 관련 카테고리 갱신

	//INVALIDATE_MONSTER_LIST,			// 2022/01/07 PKT - 몬스터 일람 관련 몬스터 리스트

	//INVALIDATE_MONSTER,
	//INVALIDATE_USE_ITEM_CLEAR,			// 2022/04/27 PKT - 아이템 사용으로 인한 몬스터 레벨 업 시 아이템 사용 량 초기화

	//BATCH_LEVEL_UP,						// 2021/12/25 PKT - 일괄 레벨업
	//BATCH_ITEM_USE,						// 2021/12/25 PKT - 일괄 아이템 사용

	

	//EVENT_SLATE_ITEM,					// 2021/12/27 PKT - 지식의 석판 오픈 아이템 획득

	USE_ITEM_SLATE_RESET,				// 2021/12/22 PKT - 지식의 석판 아이템을 상묭하겠다.(스탯 초기화)
	//EVENT_SLATE_RESET,					// 2021/12/22 PKT - 지식의 석판 모든 스탯 초기화

	//EVENT_SLATE_ACTIVE,					// 2021/12/23 PKT - 지식의 석판 활성화
	//EVENT_SLATE_SLOT_ACTIVE,			// 2021/12/29 PKT - 지식의 석판의 슬롯 활성화

	INVALIDATE_CODEX,						// 2023/11/02 - 도감 리뉴얼
	INVALIDATE_KNOWLEDGE_CONVERT,				// 2023/11/02 - 총 획득 지식 및 도감 포인트 갱신
	INVALIDATE_CATEGORY_LEVEL_UP,			// 2023/11/09 - 대륙 탐사 관련 갱신
	INVALIDATE_CODEX_MAP_GROUP_EXP_UPDATE,	// 2023/11/09 - 몬스터 지식 획득 시, 업데이트
	INVALIDATE_CODEX_MAP_GROUP_GEM,			// 2023/11/09 - 몬스터 지식 획득 시, 업데이트 (지식의 결정 사용)
};

UENUM()
enum class MessageContentTitle : uint8
{
	UPDATE_TITLE,						// 2022/01/14 PKT - 칭호 정보 갱신
	UPDATE_COLLECTION,					// 2022/01/14 PKT - 칭호 컬렉션 정보 갱신
	UPDATE_EQUIP,						// 2022/01/14 PKT - 칭호 장착 정보 갱신		
	UPDATE_CATEGORY_REDDOT,				// 2022/01/24 PKT - 카테고리 레드닷 갱신
};


UENUM()
enum class MessageContentUnlock : uint8
{
	NOTIFY_CONTENTS_UNLOCK,				// 잠긴 컨텐츠 해제 알림
	INVALIDATE_ALL_UNLOCK,				// 잠금 관련 UI 갱신
};

UENUM()
enum class MessageContentObserver : uint8
{
	START_OBSERVE,						// 관전 시작
	CHANGE_OBSERVE_TARGET,				// 관전 타겟 변경됨
	CHANGE_OBSERVE_TARGET_LIST,			// 관전 타겟 리스트가 변경됨
};

UENUM()
enum class MessageContentTutorial : uint8
{
	START_TUTORIAL,
	END_TUTORIAL,  // 튜토리얼 내부에서 매니저에 전송
	FORCED_END_TUTORIAL, // 튜토리얼 강종 요청
	END_ACTION, //FGsTutorialMsgParamInt, actionId
	COMPLETE_TUTORIAL, // 튜토리얼 완료, FGsPrimitiveInt32, tutorialId
	COMPLETE_TUTORIAL_FOR_BM, // 튜토리얼 완료, 큐가 비어있을 경우에만 BM상점 팝업 출력을 위해 보냄 
	EVENT_ACTION_TUTORIAL_START, // 이벤트 액션에서 튜토리얼 호출
	OPEN_UI, // FGsTutorialMsgParamName, ui이름
	OPEN_UI_CHECK_PARAM, // FGsTutorialMsgParamOpenUI
	END_UI_INTERACTION, // 튜토리얼 UI 상호작용(클릭, 롱프레스 등) 완료
	LEARN_SKILL, // 스킬습득, FGsTutorialMsgParamSkill
	SUMMON_BUTTON_STATE, // 소환연출 버튼 상태
	FIRST_TUTORIAL, // 게임 첫 퀘스트 전용
};

UENUM()
enum class MessageContentUserProfile : uint8
{
	INVALIDATE_ALL,				// 유저 프로필 전체 갱신
	INVALIDATE_COMMENT,			// 유저 프로필 한마디 갱신
	INVALIDATE_INTRODUCTION,	// 유저 프로필 자기소개 갱신
	INVALIDATE_KEYWORD,			// 유저 프로필 키워드 갱신
	INVALIDATE_NICKNAME_HISTORY, // 유저 닉네임 변경 이력 갱신
	OPEN_KEYWORD_VOTE,			// 유저 프로필 키워드 투표 창 열기
	ERROR_INVALID_USER,			// 존재하지 않은 유저
};

UENUM()
enum class MessageContentUserContext : uint8
{
	OPEN,
	CLOSE,

	VALID_USER,			// 유효한 유저 (팝업 열기 or 정상적 기능 실행)
	INVALID_USER,		// 유효하지 않은 유저 (삭제/서버이동)
	INVALID_USER_OTHER_SERVER, // 유효하지 않은 타서버 유저
};

UENUM()
enum class MessageContentBoss : uint8
{
	INVALIDATE_ALL,				// 유저 프로필 전체 갱신
	INVALIDATE_COMMENT,			// 유저 프로필 한마디 갱신
	INVALIDATE_INTRODUCTION,	// 유저 프로필 자기소개 갱신
	INVALIDATE_KEYWORD,			// 유저 프로필 키워드 갱신
	OPEN_KEYWORD_VOTE,			// 유저 프로필 키워드 투표 창 열기
	ERROR_INVALID_USER,			// 존재하지 않은 유저
};

UENUM()
enum class MessageContentArena : uint8
{
	CHANGE_STATE,
	
	CLOSE_ARENA_UI,
};

UENUM()
enum class MessageContentBattlePass : uint8
{
	UPDATE_ACTIVE_ADDREWARD,	// 추가 보상 활성화
	UPDATE_LEVEL_EXP,			// 패스 레벨 경험치 갱신
	UPDATE_LEVEL_REWARD,		// 패스 레벨 보상 수령, 패스 레벨 추가 보상 수령
	UPDATE_LEVEL_REWARDALL,		// 패스 레벨 보상 모두받기 수령
	
	UPDATE_MISSONTAB,					// 패스 미션 탭 오픈 정보 갱신
	UPDATE_MISSIONTAB_MISSION_EXP,		// 패스 탭 미션 경험치 갱신

	UPDATE_RESET_MISSIONTAB,			// reset time 이 되어 미션 전체를 갱신

	UPDATE_BATTLEPASS,			// 새로 오픈되거나 close 된 배틀패스가 있을 때
};

UENUM()
enum class MessageContentSeal : uint8
{
	OPEN_SEAL,						// 봉인 열기
	SEAL,							// 봉인 정보

	REQ_SEAL_SLOT_CHANGE_ALL,		// 봉인 슬롯 전체 변경 요청
	ACK_SEAL_SLOT_CHANGE_ALL,		// 봉인 슬롯 전체 변경 응답

	REQ_SEAL_LEVEL_UP,				// 봉인 레벨업 요청
	ACK_SEAL_LEVEL_UP,				// 봉인 레벨업 응답

	TRY_SEAL_SLOT_REROLL_UNLOCK,	// 봉인슬롯 리롤 잠금/풀림 설정

	REQ_SEAL_SLOT_LOCK_SAVE,		// 봉인 슬롯 잠금 데이터 저장 요청
	ACK_SEAL_SLOT_LOCK_SAVE,		// 봉인 슬롯 전체 데이터 저장 응답
};

UENUM()
enum class MessageContentPersoncalTrading : uint8
{
	INVALIDATE_TARGET_ITEM,
	INVALIDATE_TARGET_DIA,
	INVALIDATE_TARGET_STATE,

	INVALIDATE_LOCAL_ITEM,
	INVALIDATE_LOCAL_DIA,
	INVALIDATE_LOCAL_STATE,

	CLOSE,
	CLOSE_POPUP_CHAT,
	CLOSE_ABORT,
};

UENUM()
enum class MessageContentHiveEvent : uint8
{
	SEND_ANALYTICS_EVENT,
	SEND_PURCHASE_EVENT,
};

using	MsgContentHiveEventHandle = TPair<MessageContentHiveEvent, FDelegateHandle>;
using	MsgContentHiveEventHandleArray = TArray<MsgContentHiveEventHandle>;

UENUM()
enum class MessageContentSpiritShot : uint8
{
	OPEN_SPIRIT_SHOT,						// 천마석 윈도우 오픈
	SPIRIT_SHOT_ENCHANT_INFO,				// [패킷] 천마석 강화 정보
	
	REQ_SPIRIT_SHOT_ENCHANT,				// [패킷] 천마석 강화 요청
	ACK_SPIRIT_SHOT_ENCHANT,				// [패킷] 천마석 강화 응답

	SPIRIT_SHOT_ENCHANT_EFFECT_FINISED,		// [연출] 천마석 강화 연출 완료
};

UENUM()
enum class MessageContentSanctum : uint8
{
	SANCTUM_BATTLE_FIRST_NOTIFY, // 30분 전 알림
	SANCTUM_BATTLE_FINAL_NOTIFY, // 5분 전 알림, 기존 수호탑 전부 파괴
	SANCTUM_BATTLE_START, // 성소 점령전 시작
	SANCTUM_BATTLE_BEFORE_END_NOTIFY, // 성소점령전 종료 전 알림(1, 2차)
	SANCTUM_BATTLE_END, // 성소 점령전 종료
	SANCTUM_BATTLE_OCCUPY_SUCCESS, // 성소 점령전 성소 점령 성공
	TRY_START_CONSTRUCT, // 성소 건설 시작 시도
	CANCEL_RESERVED_INTERACTION, // 성소 건설 시작되면 바로 인터렉션하는 예약 취소/제거
	TRY_START_RESERVED_INTERACTION, // 성소 건설 시작 시 예약된 인터렉션 시도
	INVALIDATE_SANCTUM_STATE, // 성소 상태 재갱신(기사단 가입상태 바뀔 경우 등)
};

UENUM()
enum class MessageContentsMigration : uint8
{
	MIGRATION_NAME_CHANGE,
	MIGRATION_GUILD_CHANGE,
};