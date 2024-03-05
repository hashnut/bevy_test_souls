#pragma once

#include "EGsIconType.generated.h"

UENUM(BlueprintType)
enum class EGsIconType : uint8
{
	NONE = 0,
	ICON_ITEM_INVENTORY_M,		// 아이템/재화 아이콘 인벤토리 M
	ICON_ITEM_INVENTORY_L,		// 아이템/재화 아이콘 인벤토리 L
	ICON_ITEM_INVENTORY_XL,		// 아이템/재화 아이콘 인벤토리 XL
	ICON_ITEM_SIMPLE_L,			// 아이템/재화 HUD 아이콘 L 
	ICON_ITEM_REWARD_L,			// 아이템/재화 보상 아이콘 L
	ICON_ITEM_CIRCLE_M,			// 아이템/재화 원형 아이콘 M

	ICON_INTERACTION,			// 인터랙션 버튼	
	ICON_ABNORMALITY_S,			// 상태이상 아이콘 S
	ICON_ABNORMALITY_M,			// 상태이상 아이콘 M
	ICON_MINIMAP_NPC_LIST,		// NPC 리스트 아이템 아이콘
	
	ICON_MINIMAP_ICON_S,		// 공통 미니맵 아이콘 S
	ICON_MINIMAP_ICON_M,		// 공통 미니맵 아이콘 M
	ICON_MINIMAP_LOCAL_PLAYER,	// 로컬 플레이어 미니맵 아이콘
	ICON_MINIMAP_SUB_QUEST,		// 미니맵 아이콘: 서브 퀘스트

	ICON_PARTY_WEAPON_TYPE,		// 파티 무기 타입 아이콘

	ICON_CONTINENTALMAP,		// 대륙 맵 아이콘
	ICON_REGIONMAP,				// 지역 맵 아이콘
	ICON_REGIONMAP_LOCAL_PLAYER,// 지역 로컬 플레이어 아이콘
	ICON_REGIONMAP_BOOKMARK,	// 지역 즐겨찾기(선택 스위칭)
	ICON_REGIONMAP_PORTAL,		// 지역 포탈(선택 스위칭)
	ICON_REGIONMAP_AUTOMOVE_TARGET,	// 지역 자동 이동 타겟(선택 스위칭)
	ICON_REGIONMAP_SPOT,		// 지역명 text 있음
	ICON_REGIONMAP_FIELD_BOSS,		// 필드 보스(상태 스위칭)
	ICON_REGIONMAP_MONSTER_DROP,	// 몬스터 스폰 아이콘

	ICON_CHAT_EMOJI,			//채팅 이모지 창 이모지 아이콘

	ICON_ITEM_COLLECTION_SLOT,		// 아이템 컬력션 전용 슬롯 아이콘
	ICON_ITEM_COLLECTION_SLOT_XL,	// 아이템 컬력션 전용 슬롯 아이콘(인벤토리 전용)

	ICON_ITEM_SHOP_SLOT,		// 상점 아이템 슬롯 아이콘

	ICON_REGIONMAP_PORTAL_IN_TOWN,		// regionmap portal in town

	ICON_CONTENTS_EVENT_GROUP,	// 컨텐츠이벤트그룹

	ICON_REGIONMAP_INVASION_PORTAL, // 침공 포탈

	ICON_MINIMAP_AUTO_MOVE_PATH,	// 자동 이동 경로 아이콘

	ICON_REGIONMAP_SANCTUM,			// 지역맵: 성소

};
