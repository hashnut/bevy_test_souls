#pragma once

#include "Animation/AnimNotify/EGsSkeletalMeshMaterialParam.h"
#include "GsMaterialParam.generated.h"

class UCurveVector;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FGsMaterialParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName MaterialParameterName;

	UPROPERTY(EditAnywhere)
	EGsSkeletalMeshMaterialParam ParamType = EGsSkeletalMeshMaterialParam::CurveScalar;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "ParamType == EGsSkeletalMeshMaterialParam::CurveScalar", EditConditionHides))
	UCurveFloat* MaterialParamCF = nullptr;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "ParamType == EGsSkeletalMeshMaterialParam::CurveVector", EditConditionHides))
	UCurveVector* MaterialParamCV = nullptr;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "ParamType == EGsSkeletalMeshMaterialParam::Scalar", EditConditionHides))
	float MaterialParamF;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "ParamType == EGsSkeletalMeshMaterialParam::Vector", EditConditionHides))
	FVector MaterialParamV;
};