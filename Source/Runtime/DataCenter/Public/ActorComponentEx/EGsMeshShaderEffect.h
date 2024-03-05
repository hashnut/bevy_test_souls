#pragma once

#include "EGsMeshShaderEffect.generated.h"

UENUM(BlueprintType)
enum class EGsMeshShaderEffect : uint8
{
	None,
	Dissolve,
	Fresnel,

	Max,
};
