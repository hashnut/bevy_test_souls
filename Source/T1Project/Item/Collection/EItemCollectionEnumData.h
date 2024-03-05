#pragma once

#include "CoreMinimal.h"

/**
 * 2022/12/27 PKT - Item Collection 재료들의 상태
 */
enum class ECollectionMaterialActionType
{
	ImmediatePossible,		// 2022/12/26 PKT - 즉시 등록 가능
	ConditionalPossible,	// 2022/12/26 PKT - 강화 이후 등록 가능
	Impossible,				// 2022/12/26 PKT - 불가능 한 상태
	Completed,				// 2022/12/26 PKT - 완료 상태
};

/**
 * Collection Sort Order
 */
enum class ECollectionSortOrder
{
	ImmediatePossible,		// 2021/10/07 PKT - 즉시 등록 가능
	ConditionalPossible,	// 2021/10/07 PKT - 조건부 등록 가능
	Progressing,			// 2021/10/07 PKT - 하나 이상의 아이템이 등록
	Idel,					// 2021/10/07 PKT - 아무것도 아닌 상태
	InActive,				// 2021/10/14 PKT - 활성화가 아닌 상태
	Completed,				// 2021/10/07 PKT - 완료 상태
	Max,
};

// 2021/10/07 PKT - 정렬 우선 순위
enum class ESortPriority
{
	ImmediatePossible,		// 2021/10/07 PKT - 즉시 등록 가능
	ConditionalPossible,	// 2021/10/07 PKT - 조건부 등록 가능
	Progressing,			// 2021/10/07 PKT - 하나 이상의 아이템이 등록
	Idel,					// 2021/10/07 PKT - 아무것도 아닌 상태
	InActive,				// 2021/10/14 PKT - 활성화가 아닌 상태
	Completed,				// 2021/10/07 PKT - 완료 상태
	Max,
};


// 2021/10/14 PKT - Item Collection State Change Reason
enum class ECollectionStateChangeType
{
	EBookMark = 0,		// 2021/10/14 PKT - 즐겨 찾기 변경
	EActive,			// 2021/10/14 PKT - 활성화 상태 변경
	ECollectionSlot,		// 2021/10/14 PKT - 수집 상태 변경
	EEtc,				// 2021/10/14 PKT - 그 외(?)

	EMax,
};

