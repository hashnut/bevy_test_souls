#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Runtime/Core/Public/Misc/EnumClassFlags.h"


enum class RankStoryState
{
	Normal,			// 진행하지 않은 상태
	InProgress,		// 진행 중인 상태
	Completed,		// 완료 상태
};

/**
 * 2021/06/30 PKT -
 * 보상과 재화를 같이 사용하는 구조체가 없음. ( 메일에는 있는데 가져다 쓰기가 이름이 달라서...)의 이유도 있고, 
 * 보상 관련 보상 방침이 변경 되었음.
 * https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=237031591
 * 이에 관련하여 다단계 랭크 시스템에서 사용할 구조체 만듬.
 */
struct FGsRefreshRewardBuffer
{
	enum class Type
	{
		None,
		Item,
		Currency,
	};
	const static int32 Invaild_Value = 0;
	Type _type = Type::None;
	int32 _id = Invaild_Value;
	int32 _minCount = Invaild_Value;
	int32 _maxCount = Invaild_Value;
};