#pragma once

#include "ERankLinkType.generated.h"

UENUM(BlueprintType)
enum class ERankLinkType : uint8
{
	SingleOnly,		// 오직 하나의 슬롯에 연결만 가능
	SingleToMulti,	// 한개에서 다중으로 연결 가능
	MultiToSingle,	// 다중에서 한개의 슬롯에만 연결 가능
};
