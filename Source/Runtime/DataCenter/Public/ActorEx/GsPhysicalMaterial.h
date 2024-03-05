// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsPhysicalMaterial.generated.h"

/**
 * 발자국 정보가 포함된 PhysicalMaterial
 */

class UParticleSystem;

UCLASS()
class DATACENTER_API UGsPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootPrint")
	UParticleSystem* FootPrintEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootPrint")
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootPrint")
	EGsFootSoundType _materialType = EGsFootSoundType::Dirt;
};
