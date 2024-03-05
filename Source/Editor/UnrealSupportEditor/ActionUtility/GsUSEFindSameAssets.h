// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/AssetActionUtility.h"
#include "GsUSEFindSameAssets.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class UNREALSUPPORTEDITOR_API UGsUSEFindSameAssets : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UnrealSupportEditor AssetAction")
	void FindSameAssets();
};