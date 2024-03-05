// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsShapeData.generated.h"

class UMaterialInterface;

/**
 * 
 */
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsCylinder
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameObject")
	float height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameObject")
	float radius;
};

USTRUCT(BlueprintType)
struct T1PROJECT_API FPartsListData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameObject")
	int32 Face;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameObject")
	int32 Body;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameObject")
	int32 Cloak;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameObject")
	int32 Weapon;
};

// 조준 스킬 이펙트 리소스 정보를 설정
USTRUCT(BlueprintType)
struct T1PROJECT_API FAimSkillData
{
	GENERATED_BODY()
	
	// 조준 스킬 예시선 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath SkillGuideEffectPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* SkillGuideValidEffectMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* SkillGuideInvalidEffectMaterial = nullptr;

	// 조준 스킬 자동 발동 타이밍 (sec)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Tooltip = "Second"))
	float AutoTargetDelayTime = 0.5f;
};

USTRUCT(BlueprintType)
struct T1PROJECT_API FInteractionMotionWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath ToolStaticMeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ToolMaterial = nullptr;
};
