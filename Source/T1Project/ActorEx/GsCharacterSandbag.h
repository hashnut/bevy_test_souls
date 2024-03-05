// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsCharacterNonPlayerBase.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "Quest/GsActorQuestTargetInterface.h"
#include "GsCharacterSandbag.generated.h"

UCLASS()
class T1PROJECT_API AGsCharacterSandbag : public AGsCharacterNonPlayerBase
{
	GENERATED_BODY()

public:
	AGsCharacterSandbag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
