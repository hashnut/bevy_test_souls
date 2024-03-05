#pragma once

/**
 * 아이템컬렉션 아이템 장착 슬롯 상태
 * 관련 데이터 정렬 시, 이 enum 의 순서로 정렬함.
 */
enum class EItemCollectionSlotState : uint8
{
	ImmediatePossible,   // 아이템 등을 소유하고 있어 즉시 가능
	ConditionalPossible, // 강화 이후 등 조건부 가능
	Impossible,          // 관련 아이템을 소유하지 않음 등
    Completed,           // 완료
};