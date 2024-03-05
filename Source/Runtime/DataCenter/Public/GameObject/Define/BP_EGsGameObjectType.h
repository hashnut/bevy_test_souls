#pragma once

#include "BP_EGsGameObjectType.generated.h"

// 블루프린트 노출용 GameObject Type 정의
UENUM(BlueprintType)
enum class BP_EGsGameObjectType : uint8
{
	Projectile,
	NonPlayer,
	Player,
	LocalPlayer
};