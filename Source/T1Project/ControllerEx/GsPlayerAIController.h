// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GsPlayerAIController.generated.h"

/**
 *
 */
class APawn;

UCLASS(Blueprintable)
class T1PROJECT_API AGsPlayerAIController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void OnPossess(APawn* in_owenr) override;
	UFUNCTION()
		virtual void OnUnPossess() override;
};
