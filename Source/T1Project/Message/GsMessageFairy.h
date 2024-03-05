#pragma once

UENUM()
enum class MessageFairy : uint8
{
	CHANGE_SELECTION,	// 선택 페어리 변경 (object내 spawn)
	CHANGE_EQUIP,		// 소환 페어리 변경
	ENCHANT_RESULT,		// 페어리 enchant 결과 처리
	OBTAIN_RESULT,		// 페어리 합성/확정 결과 처리
	UPDATED,			// 새로운 페어리 추가, level up, amount
	COLLECTION_UPDATE,	// 페어리 콜렉션 개별 아이템 변동
	SUMMONWAIT_UPDATE,	// 페어리 소환 확정 변경
	SUMMON_COMPLETE,	// 페어리 소환 완료
	CEILING_RESULT,		// 페어리 천장 소환 결과 처리
	INITTAB_CHANGE,		// Fairy State Window init tab
	ENCHANT_READY,		// 페어리 성장 위치로
	ENCHANT_CANCEL,		// 페어리 info 위치로

	FIND_FAIRY,
};