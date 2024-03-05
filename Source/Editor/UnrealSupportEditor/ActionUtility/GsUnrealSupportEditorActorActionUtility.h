// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/ActorActionUtility.h"
#include "GsUnrealSupportEditorActorActionUtility.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class UNREALSUPPORTEDITOR_API UGsUnrealSupportEditorActorActionUtility : public UActorActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UnrealSupportEditor ActorAction")
	void ApplyOverlayMaterialSetting();
};
