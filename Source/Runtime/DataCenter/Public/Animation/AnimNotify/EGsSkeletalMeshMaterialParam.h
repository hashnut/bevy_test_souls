#pragma once

#include "EGsSkeletalMeshMaterialParam.generated.h"

UENUM(BlueprintType, meta = (Tooltip = "머터리얼 파라메터 제어타입"))
enum class EGsSkeletalMeshMaterialParam : uint8
{
	CurveScalar	= 0	UMETA(Tooltip = "ParamType CurveScalar"),
	CurveVector = 1	UMETA(Tooltip = "ParamType CurveVector"),
	Scalar		= 2 UMETA(Tooltip = "ParamType Scalar"),
	Vector		= 3 UMETA(Tooltip = "ParamType Vector"),
};