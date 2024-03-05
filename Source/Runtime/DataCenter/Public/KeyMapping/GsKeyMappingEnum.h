// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsKeyMappingEnum.generated.h"


/*
  EGsKeyMappingType 은 '인풋' 을 의미합니다. 
  키보드의 키든, 터치이든, '인풋' 의 의미를 가져야 합니다.
  (상태를 추가하고 싶으면 EGsKeyMappingInputFlags 를 활용해 주세요)

  !!! 주의 !!! 
  현재 동기화를 위해 단축키 설정을 서버로 보내고 있어(SendKeyboardMappingSave 부분), 
  Enum 키를 중간에 추가하면 단축키 설정이 꼬이게 됩니다.
  따라서 신규 KeyMappingType 추가 시, 반드시 제일 마지막에 추가 부탁드립니다.
*/
UENUM(BlueprintType)
enum class EGsKeyMappingType : uint8
{
	NONE,
	KEY_MAPPING_MOVE_UP,
	KEY_MAPPING_MOVE_DOWN,
	KEY_MAPPING_MOVE_LEFT,
	KEY_MAPPING_MOVE_RIGHT,
	KEY_MAPPING_OK,
	KEY_MAPPING_NO,
	KEY_MAPPING_NO_ANDROID_BACK,
	KEY_MAPPING_NO_ANDROID_BACKSPACE,
	KEY_MAPPING_NORMALATTACK,
	KEY_MAPPING_PICKUP,
	KEY_MAPPING_SCAN,
	KEY_MAPPING_SWITCH_SPIRITSHOT,
	KEY_MAPPING_SWITCH_AUTO,
	KEY_MAPPING_SWITCH_AUTOPOTION,
	KEY_MAPPING_SKILLSLOT_1,
	KEY_MAPPING_SKILLSLOT_2,
	KEY_MAPPING_SKILLSLOT_3,
	KEY_MAPPING_SKILLSLOT_4,
	KEY_MAPPING_SKILLSLOT_5,
	KEY_MAPPING_SKILLSLOT_6,
	KEY_MAPPING_SKILLSLOT_7,
	KEY_MAPPING_SKILLSLOT_8,
	KEY_MAPPING_SKILLSLOT_9,
	KEY_MAPPING_SKILLSLOT_10,
	KEY_MAPPING_SKILLSLOT_11,
	KEY_MAPPING_SKILLSLOT_12,
	KEY_MAPPING_COSTUMESKILL_1,
	KEY_MAPPING_QUICKSLOT_1,
	KEY_MAPPING_QUICKSLOT_2,
	KEY_MAPPING_QUICKSLOT_3,
	KEY_MAPPING_QUICKSLOT_4,
	KEY_MAPPING_QUICKSLOT_5,
	KEY_MAPPING_QUICKSLOT_LEFT,
	KEY_MAPPING_QUICKSLOT_RIGHT,
	KEY_MAPPING_CAM_CHANGE,
	KEY_MAPPING_MINIMAP,
	KEY_MAPPING_BOOKMARK,
	KEY_MAPPING_HUD_TAP_PREV,
	KEY_MAPPING_HUD_TAP_NEXT,
	KEY_MAPPING_CHAT,
	KEY_MAPPING_CHARACTER,
	KEY_MAPPING_EVENT,
	KEY_MAPPING_BMSHOP,
	KEY_MAPPING_INVENTORY,
	KEY_MAPPING_SKILL,
	KEY_MAPPING_MENU,
	KEY_MAPPING_SAVEMODE,
	KEY_MAPPING_SCREENSHOTMODE,
	KEY_MAPPING_BUFFLIST,
	KEY_MAPPING_TARGET_CHANGE,

	TOUCH_MOVE,
	TOUCH_INTERACTION,

	KEY_MAPPING_QUICKSLOT_6,
	KEY_MAPPING_QUICKSLOT_7,
	KEY_MAPPING_QUICKSLOT_8,
	KEY_MAPPING_QUICKSLOT_9,
	KEY_MAPPING_QUICKSLOT_10,
	
	MAX
};
ENUM_RANGE_BY_COUNT(EGsKeyMappingType, EGsKeyMappingType::MAX);

//----------------------------------------------------------------------------------------------------------------------

/*
  EGsKeyMappingInputFlags 는 상태(SETTING 으로 시작)와, 상태에 대응되는 플래그(INPUT 으로 시작)을 정의합니다.
    현재 EGsKeyMappingInputFlags 는 아래와 같은 방법으로 사용할 수 있습니다.
    (함수는 FGsInputKeyEventValidator::IsValidEventToState 를 참고해 주세요)

  1. 현재 '특정 인풋'이 막혀 있는지 체크 (인자를 하드코딩할 필요는 없습니다) 할 수 있습니다.
    EGsKeyMappingInputFlags 가 EGsKeyMappingType 를 갖고 있는지 체크
	IsValidEventToState(InEvent, InState) 사용 -> 현재 상태 InState 에서, InEvent 인풋이 막혀있는지를 테스트.
	예를 들어, InEvent 가 KEY_MAPPING_OK 값이고, InState 가 SETTING_MOVE 이라면, 인풋이 막히지 않을 것입니다.

    IsValidEventToState 의 인자로 들어오는 녀석은 '하드코딩되지 않을 것'입니다.
    이는, 실제 함수를 사용할 때 각 Enum 에 어떤 녀석이 들어가 있는지 전혀 신경써도 되지 않아도 된다는 뜻입니다.
	대신, FGsInputKeyEventValidator::InitializeValidator 만 제대로 처리해 주면 됩니다.

  2. 현재 '상태'를 체크 (예외 처리)
	EGsKeyMappingInputFlags 가 EGsKeyMappingInputFlags 를 갖고 있는지 체크
	IsValidStateToState(InStateFlag, InState) 사용 -> 현재 상태 InState 가, InStateFlag 를 갖고 있는지 테스트
    특정 '상태'에서 인풋을 막아야 하는 예외처리에 사용합니다.
	InStateFlag 는 INPUT 으로 시작하는 녀석, InState 는 SETTING 으로 시작하는 녀석입니다.
*/
enum class EGsKeyMappingInputFlags : uint32
{
	// 인풋 블락을 해제해 줄 때 사용할 Flag 로, 어떤 인풋이든 입력할 수 있을 것입니다.
	INPUT_NONE = 0,

	// Input Flag -> Input 에 대한 Flag 로, FGsInputKeyEventValidator::InitializeValidator 에서
	// 각 flag 에 대해 EGsKeyMappingType 을 연결해 주어야 합니다
	INPUT_KEY_MOVE = 1 << 0,					// MOVE
	INPUT_KEY_OK = 1 << 1,						// KEY_MAPPING_OK
	INPUT_KEY_NO = 1 << 2,						// KEY_MAPPING_NO
	INPUT_KEY_MISC = 1 << 3,					// 그외

	INPUT_TOUCH_MOVE = 1 << 10,					// 터치 이동(줌, 터치 이동, 줌인)
	INPUT_TOUCH_INTERACTION = 1 << 11,			// 터치 인터랙션(NPC 몸통, 인터랙션 버튼)

	// State Flag -> 각 State 를 상징하는 flag. 예외 처리에 사용됩니다(IsValidStateToState).
	// 이런 식으로 사용한 이유는, 세팅이 '겹칠 수' 있기 때문입니다. 
	INPUT_LOCK_MOVE = 1 << 15,						// 이동
	INPUT_LOCK_OBSERVE = 1 << 16,					// 옵저버
	INPUT_LOCK_NPC = 1 << 17,						// 상점, 창고,.. 대화
	INPUT_LOCK_PLAYABLE = 1 << 18,					// 플레이어블
	INPUT_LOCK_HUD_CAMERA_MODE = 1 << 19,			// 카메라 연출 모드
	INPUT_LOCK_CINEMATIC = 1 << 20,					// 시네마틱
	INPUT_LOCK_SUMMON = 1 << 21,					// 소환
	INPUT_LOCK_TUTORIAL = 1 << 22,					// 튜토리얼
	INPUT_LOCK_SCREENSHOT = 1 << 23,				// 스크린샷 모드
	INPUT_CONTENTS_WAIT_SERVER_RESPONSE = 1 << 24,	// 서버 응답 대기
	INPUT_LOCK_POPUP = 1 << 25,						// 퀘스트 팝업 (퀘스트 팝업 열린 상태로 단축키 사용 막기 위함)
	INPUT_LOCK_TRAY = 1 << 26,						// 트레이 열린 상태에서는 인풋 블락
	INPUT_LOCK_COMMON_POPUP = 1 << 27,				// 일반 팝업 상황에서 막고싶은 경우

	// Setting Flag -> 각 상황별로, 세팅을 FGsInputEventMsgFlag 에 담아 보냅니다.
	// 그럼 특정 상황에 Block 되어야 하는 Input 을 제대로 막을 수 있습니다. true 로 막고, false 로 해제
	SETTING_MOVE = INPUT_LOCK_MOVE | INPUT_KEY_MOVE | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	SETTING_OBSERVE = INPUT_LOCK_OBSERVE | INPUT_KEY_MOVE | INPUT_KEY_OK | INPUT_KEY_MISC | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	SETTING_NPC = INPUT_LOCK_NPC | INPUT_KEY_MOVE | INPUT_KEY_OK | INPUT_KEY_MISC | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	SETTING_PLAYABLE = INPUT_LOCK_PLAYABLE | INPUT_KEY_MOVE | INPUT_KEY_OK | INPUT_KEY_NO | INPUT_KEY_MISC | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	SETTING_HUD_CAMERA_MODE = INPUT_LOCK_HUD_CAMERA_MODE | INPUT_KEY_MOVE | INPUT_KEY_NO | INPUT_KEY_MISC | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	SETTING_CINEMATIC = INPUT_LOCK_CINEMATIC | INPUT_KEY_MOVE | INPUT_KEY_OK | INPUT_KEY_NO | INPUT_KEY_MISC | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	// 소환창에서 소환 마일리지 팝업을 INPUT_KEY_NO로 닫기
	// https://jira.com2us.com/jira/browse/C2URWQ-8126
	SETTING_SUMMON = INPUT_LOCK_SUMMON | INPUT_KEY_MOVE | INPUT_KEY_OK | INPUT_KEY_MISC | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	SETTING_TUTORIAL = INPUT_LOCK_TUTORIAL | INPUT_KEY_MOVE | INPUT_KEY_OK | INPUT_KEY_NO | INPUT_KEY_MISC | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	SETTING_CAMERA_MODE_EQUIP_VIEW = INPUT_TOUCH_MOVE,
	SETTING_SCREENSHOT = INPUT_LOCK_SCREENSHOT | INPUT_KEY_OK | INPUT_KEY_NO | INPUT_KEY_MISC | INPUT_TOUCH_INTERACTION,
	SETTING_WAIT_SERVER_RESPONSE = INPUT_CONTENTS_WAIT_SERVER_RESPONSE | INPUT_KEY_MOVE | INPUT_KEY_OK | INPUT_KEY_MISC | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	SETTING_QUEST_POPUP = INPUT_LOCK_POPUP | INPUT_KEY_MOVE | INPUT_TOUCH_MOVE | INPUT_KEY_MISC,
	SETTING_TRAY = INPUT_LOCK_TRAY | INPUT_KEY_MOVE | INPUT_KEY_OK | INPUT_KEY_NO | INPUT_KEY_MISC | INPUT_TOUCH_MOVE | INPUT_TOUCH_INTERACTION,
	SETTING_COMMON_POPUP = INPUT_LOCK_COMMON_POPUP | INPUT_KEY_MOVE | INPUT_TOUCH_MOVE | INPUT_KEY_MISC | INPUT_TOUCH_INTERACTION,

	INPUT_LOCK_ALL = 0xff,
};
ENUM_CLASS_FLAGS(EGsKeyMappingInputFlags);