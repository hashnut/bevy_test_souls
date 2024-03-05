// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GsLevelScriptActor.generated.h"

/**
 * 
 */

UCLASS()
class T1PROJECT_API AGsLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category="SingleMode")
	void EventFunctionKeyClick(int inKey);
};
