#pragma once

#include "EGsArtLevelLoadConditionType.generated.h"

UENUM(BlueprintType)
enum class EGsArtLevelLoadConditionType : uint8
{
	Unload,
	Load,
	ByOption, //조건 논의 필요
	ByScalabilty,	//조건 논의 필요
};