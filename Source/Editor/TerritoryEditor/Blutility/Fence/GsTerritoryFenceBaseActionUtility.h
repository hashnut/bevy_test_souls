// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorActionUtility.h"
#include "GsTerritoryFenceBaseActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYEDITOR_API UGsTerritoryFenceBaseActionUtility : public UActorActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void DeleteFence();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void EditName(FText inName);

protected:
	class AGsTerritoryFenceBase* GetSelectedFence();
};
