#pragma once

// 컨텐츠이벤트 상태. 추가될 수 있음.
enum class EGsContentsEventState
{
	// 비활성 상태로 디스플레이 안함.
	Deactivated,
	// 활성 상태
	Activated,
	// 상태 무관. 조회에 사용.
	Any
};