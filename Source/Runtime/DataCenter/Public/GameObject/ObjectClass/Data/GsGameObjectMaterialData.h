#pragma once

#include "Engine/DataAsset.h"
#include "Animation/AnimNotify/EGsSkeletalMeshMaterialParam.h"
#include "GsGameObjectMaterialData.generated.h"

/**
 * 캐릭터 머터리얼 변경 정보 구조체
 */

UENUM(BlueprintType, meta = (Tooltip = "머터리얼 제어타입"))
enum class EGsGameObjectMaterial : uint8
{
	PARAMETER = 0,
	CUSTOM = 1,
};

USTRUCT(BlueprintType)
struct FGsGameObjectMaterialParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName MaterialParameterName;

	UPROPERTY(EditAnywhere)
	EGsSkeletalMeshMaterialParam ParamType = EGsSkeletalMeshMaterialParam::CurveScalar;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "ParamType == EGsSkeletalMeshMaterialParam::CurveScalar"/*, EditConditionHides*/))
	class UCurveFloat* MaterialParamCF = nullptr;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "ParamType == EGsSkeletalMeshMaterialParam::CurveVector"/*, EditConditionHides*/))
	class UCurveVector* MaterialParamCV = nullptr;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "ParamType == EGsSkeletalMeshMaterialParam::Scalar"/*, EditConditionHides*/))
	float MaterialParamF;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "ParamType == EGsSkeletalMeshMaterialParam::Vector"/*, EditConditionHides*/))
	FVector MaterialParamV;

	// 내부 사용용 totaltime;
	float TotalTime = 0.f;
};


UCLASS(Blueprintable, Category = "GsGameObjectMaterialData")
class DATACENTER_API UGsGameObjectMaterialData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EGsGameObjectMaterial _materialControlType = EGsGameObjectMaterial::PARAMETER;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "_materialControlType == EGsGameObjectMaterial::CUSTOM", EditConditionHides))
	class UMaterialInterface* _material = nullptr;

// 	UPROPERTY(EditAnywhere)
// 	bool _useMaterialIndex = false;
// 
// 	UPROPERTY(EditAnywhere, Meta = (EditCondition = "_useMaterialIndex", EditConditionHides))
// 	int _targetMaterialIndex = 0;

	// EditConditionHides을 배열(TArray)과 함께 이용할수 없는듯 일단 FGsGameObjectMaterialParam에서 사용 안함
	// https://issues.unrealengine.com/issue/UE-117661
	UPROPERTY(EditAnywhere)
	TArray<FGsGameObjectMaterialParam> _materialPrarams;
};
