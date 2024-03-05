// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryFenceBaseActionUtility.h"
#include "GsTerritoryFenceSpaceCrackActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYEDITOR_API UGsTerritoryFenceSpaceCrackActionUtility : public UGsTerritoryFenceBaseActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void EditEnvEvent(EGsEnvEvent inEnvEvent);
};
