// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/AssetActionUtility.h"
#include "GsUSEMaterialUseFullPrecisionToggle.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class UNREALSUPPORTEDITOR_API UGsUSEMaterialUseFullPrecisionToggle : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UnrealSupportEditor AssetAction")
	void UseFullPrecisionOn();

	UFUNCTION(BlueprintCallable, Category = "UnrealSupportEditor AssetAction")
	void UseFullPrecisionOff();

};