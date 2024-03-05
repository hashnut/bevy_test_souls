#pragma once

/** 봉인 상태 */
enum class EGsSealState : uint8
{
	// 상태 모름. 조회에 사용.
	Unknown,
	// 비활성
	Deactivated,
	// 활성
	Activated,
	// 봉인완료
	Sealed,
};
