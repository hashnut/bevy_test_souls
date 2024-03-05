#pragma once

/** 천마석 성장 상태 */
enum class EGsSpiritShotEnchantState : uint8
{
	// 알 수 없음 (초기화 및 조회에 사용)
	UnKnown,

	// 잠겨 있음 (레벨 제한 등)
	Locked,

	// 활성화 (강화 가능)
	Activated,

	// 완료 (최대 수치까지 강화)
	Completed,
};