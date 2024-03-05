// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "GsParticleSystemAssetActionUtility.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TERRITORYEDITOR_API UGsParticleSystemAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ParticleSystem AssetAction")
	void ModifySignificantLevelByLOD();

private:
	void ModifySignificantLevelByLOD_Internal(class UParticleSystem* inParticleSystem);
	bool TryGetLastLOD(OUT int32& out_LOD_level, const class UParticleEmitter* inEmitter);
	void SetSignificant(class UParticleEmitter* inEmitter, int32 inMaxLevel, int32 inLodLevel);
};
