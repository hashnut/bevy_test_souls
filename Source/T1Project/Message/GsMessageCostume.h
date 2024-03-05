#pragma once

UENUM()
enum class MessageCostume : uint8
{
	CHANGE_SELECTION,		// 선택 코스춤 변경 (object내 spawn)
	CHANGE_WEARON,			// 착용 코스춤 변경
	OBTAIN_RESULT,			// 코스춤 합성/확정 결과 처리
	UPDATED,				// 새로운 코스춤 추가, amount
	COLLECTION_UPDATE,		// 코스춤 콜렉션 개별 아이템 변동
	SUMMONWAIT_UPDATE,		// 코스춤 소환 확정 변경
	SUMMON_COMPLETE,		// 코스춤 소환 완료
	CEILING_RESULT,			// 코스춤 천장 소환 결과
	CHANGE_APPEARANCE,		// 코스춤 외형 변경
	EXCHANGE_COSTUME,		// 코스춤 교환권 사용
	EXCHANGE_EVENT_COMPLETE, // 교환 소환 이벤트 끝 - 윈도우 닫아야 함
	FIND_COSTUME,
};