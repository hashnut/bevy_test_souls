#pragma once

#include "Classes/GsMessageHandler.h"
#include "Classes/GsMessageHandlerOneParam.h"
//-------------------------------------------------------------------------------------------------
// MessageContentHud
//-------------------------------------------------------------------------------------------------
UENUM()
enum class MessageContentHud : uint8
{
	INVALIDATE_ALL = 0,
	INVALIDATE_PLAYERINFO,
	INVALIDATE_QUEST,
	INVALIDATE_CONSUMEABLE,
	INVALIDATE_SKILL,
	INVALIDATE_SYSTEMINFO,
	INVALIDATE_CHAT,
	INVALIDATE_STAT,
	INVALIDATE_NPCFUNCTION,
	INVALIDATE_ARENA,
	INVALIDATE_COUNTERATTACK,
	INVALIDATE_SKILLTARGETSELECT,
	INVALIDATE_PRESET,
	INVALIDATE_PERSONAL_TRADING,

	ACTIVE_ENEMYINFO,
	DEACTIVE_ENEMYINFO,

	SKILL_ACTIVE,
	SKILL_RESERVE_ON,		// 스킬 선예약 - 스킬 On
	SKILL_RESERVE_OFF,		// 스킬 선예약 - 스킬 Off

	AREA_TRIGGER_BEGIN,		// 지역명 출력 이벤트에 사용
	AREA_TRIGGER_END,

	TOGGLE_AUTO_POTION,
	TOGGLE_SPIRIT_SHOT,

};

// 공유 선언
using	MsgHudHandle			= TPair<MessageContentHud, FDelegateHandle>;
using	MsgHudHandleArray		= TArray<MsgHudHandle>;

using	MHud					= TGsMessageHandler<MessageContentHud>;
using	MEnemyHud				= TGsMessageHandlerOneParam<MessageContentHud, struct TargetHUD&>;
using	MReserveSkillAction		= TGsMessageHandlerOneParam<MessageContentHud, int32>;
using	MAreaTriggerEvent		= TGsMessageHandlerOneParam<MessageContentHud, class AGsAreaNameHUD&>;



//-------------------------------------------------------------------------------------------------
// MessageContentHud // 플레이어블 이벤트 HUD
//-------------------------------------------------------------------------------------------------
UENUM()
enum class MessageContentPlayableEventHud : uint8
{
	INVALIDATE_ALL = 0,
	INVALIDATE_QUEST = 1,
	INVALIDATE_GUIDE = 2,
};

// 공유 선언
using	MsgPlayableEventHudHandle		= TPair<MessageContentPlayableEventHud, FDelegateHandle>;
using	MsgPlayableEventHudHandleArray	= TArray<MsgPlayableEventHudHandle>;

using	MPlayableEventHud				= TGsMessageHandler<MessageContentPlayableEventHud>;




//-------------------------------------------------------------------------------------------------
// MessageContentHUDRedDot 
//-------------------------------------------------------------------------------------------------
UENUM()
enum class MessageContentHUDRedDot : uint8
{
	INVEN = 0,
	MAIL,
	ITEM_COLLECTION,
	STAT,
	SKILL,
	BUDDY,
	GUILD,
	FAIRY,
	COSTUME,
	MARKET,
	CRAFT,
	COMMUNITY,
	BMSHOP,
	MULTILEVELRANK,
	QUEST,
	ACHEIEVEMENT,
	MONSTER_COLLECTION,
	TITLE,
	USER_PROFILE,
	DUNGEON,
	PK_BOOK,
	BATTLEPASS,
};

// 공유 선언
using	MsgHUDRedDotHandle				= TPair<MessageContentHUDRedDot, FDelegateHandle>;
using	MsgHUDRedDotHandleArray			= TArray<MsgHUDRedDotHandle>;

// UI 관리
using	MHudRedDot						= TGsMessageHandlerOneParam<MessageContentHUDRedDot, bool>;// Inventory RedDot	




//-------------------------------------------------------------------------------------------------
// MessageContentMinimap 
//-------------------------------------------------------------------------------------------------
UENUM()
enum class MessageContentMinimap : uint8
{
	ATTACH_ICON,		// icon root에 붙인다	
	ATTACH_WIDGET,		// icon 위젯만 붙인다
	SETPOS_ICON,		// 아이콘 위젯 위치 변경
	SETPOS_IMAGE,		// 씬 이미지 위치 변경
	INVALIDATE_IFF,		// 로컬, 리모트 iff 변경
};


// 공유 선언
//-------------------------------------------------------------------------------------------------
using	MsgMinimapHandle		= TPair<MessageContentMinimap, FDelegateHandle>;
using	MsgMinimapHandleArray	= TArray<MsgMinimapHandle>;

using	MUIMinimapIcon			= TGsMessageHandlerOneParam<MessageContentMinimap, class UGsMinimapIconBase*>;
using	MUIMinimapIconWidget	= TGsMessageHandlerOneParam<MessageContentMinimap, class UGsUIIconBase*>;
using	MUIMinimapSceneImage	= TGsMessageHandlerOneParam<MessageContentMinimap, const FVector2D&>;
using	MUIMinimapIconPos		= TGsMessageHandlerOneParam<MessageContentMinimap, const FVector2D&>;
using	MUIMinimapIconTeam		= TGsMessageHandlerOneParam<MessageContentMinimap, const struct FGsMinimapIconTeamMessageParam*>;


//-------------------------------------------------------------------------------------------------
// MessageContentMenu
//-------------------------------------------------------------------------------------------------
UENUM()
enum class MessageContentHudMainMenuAct : uint8
{	
	FORCED_DISABLE,		// 강제 잠금
	NORMAL_RULES,		// 일반 룰에 따른 처리
};

// 공유 선언
using	MsgHUDMainMenuActHandle = TPair<MessageContentHudMainMenuAct, FDelegateHandle>;
using	MsgHUDMainMenuActHandleArray = TArray<MsgHUDMainMenuActHandle>;

// UI 관리
using	MHudMainMenuAct = TGsMessageHandlerOneParam<MessageContentHudMainMenuAct, int>;