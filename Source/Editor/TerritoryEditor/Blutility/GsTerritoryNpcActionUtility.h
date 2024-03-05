// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//UE4
#include "Editor/Blutility/Classes/ActorActionUtility.h"

//Territory
#include "Define/GsTerritorySpawnDefine.h"
#include "GsTerritorySpawnActorActionUtility.h"
#include "GsTerritoryNpcActionUtility.generated.h"

/**
 *
 */
 class AGsTerritorySpawnActor;

UCLASS(Blueprintable)
class TERRITORYEDITOR_API UGsTerritoryNpcActionUtility : public UGsTerritorySpawnActorActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory spawn actor action")
		void OpenNpcTable();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory spawn actor action")
		void OverrideAIData(OffensiveType inOffensive, float inSightRadius, bool inIsAggroLink);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory spawn actor action")
		void SetOffensive(OffensiveType inOffensive);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory spawn actor action")
		void SetSightRadius(float inSightRadius);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory spawn actor action")
		void SetIsAggoLink(bool inIsAggroLink);
};
