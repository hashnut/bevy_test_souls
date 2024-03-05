// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryFenceBaseActionUtility.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "GameObject/ObjectClass/Data/GsEnvCondition.h"
#include "GsTerritoryFenceEnvActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYEDITOR_API UGsTerritoryFenceEnvActionUtility : public UGsTerritoryFenceBaseActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void EditEnv(const TArray<FGsEnvCondition>& inEnvConditionArray);
};
