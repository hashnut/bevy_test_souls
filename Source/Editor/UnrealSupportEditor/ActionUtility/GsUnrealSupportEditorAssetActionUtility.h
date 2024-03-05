// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/AssetActionUtility.h"
#include "GsUnrealSupportEditorAssetActionUtility.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class UNREALSUPPORTEDITOR_API UGsUnrealSupportEditorAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UnrealSupportEditor AssetAction")
	void ApplyOverlayMaterialSetting();

	UFUNCTION(BlueprintCallable, Category = "UnrealSupportEditor AssetAction")
	void ChangeNotifyPlayParticleEffectToNotifyPlayParticleAnimationSpeedScale();

private:
	void OverlayMaterialSetting(USkeletalMeshComponent* TargetCom, UMaterialInstance* SrcMaterial, FSoftObjectPath SrcMatPath, FSoftObjectPath DestPath, UMaterialParameterCollection* MaterialParameterCollection);
	void OverlayMaterialSetting(UStaticMeshComponent* TargetCom, UMaterialInstance* SrcMaterial, FSoftObjectPath SrcMatPath, FSoftObjectPath DestPath, UMaterialParameterCollection* MaterialParameterCollection);
	void ParameterSetting(UMaterialInterface* MaterialInterface, UMaterialInstance* NewOverlayMaterial, UMaterialParameterCollection* MaterialParameterCollection);
};