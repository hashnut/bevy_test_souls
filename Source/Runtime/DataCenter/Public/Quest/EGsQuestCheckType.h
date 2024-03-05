#pragma once

#include "EGsQuestCheckType.generated.h"

// 퀘스트 CheckType
UENUM(BlueprintType)
enum class EGsQuestCheckType : uint8
{
	None,
	Running,
	Complete,	
};
