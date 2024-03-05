#pragma once

#include "CoreMinimal.h"
#include "EGsPlayableEventQuestGoalType.generated.h"

UENUM(BlueprintType)
enum class EGsPlayableEventQuestGoalType : uint8
{
	Move = 0,			// 이동
	Kill = 1,			// 몬스터 처치
	WaitSubDialogEnd	= 2,// 서브 대화 종료 대기
	WaitTimmer			=	3, // 타이머 대기
};
