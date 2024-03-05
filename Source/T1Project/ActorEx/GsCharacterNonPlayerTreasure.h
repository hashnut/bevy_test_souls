// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsCharacterNonPlayer.h"
#include "GsCharacterNonPlayerTreasure.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API AGsCharacterNonPlayerTreasure : public AGsCharacterNonPlayer
{
	GENERATED_BODY()

protected:
	virtual void InitWidgetComponent() override;
	
};
