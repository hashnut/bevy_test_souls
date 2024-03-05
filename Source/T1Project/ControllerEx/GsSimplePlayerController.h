// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GsSimplePlayerController.generated.h"



//--------------------------------------------------------------------

/**
 * APlayerController 클래스를 상속 받고 
 */
UCLASS(config = Game, BlueprintType, Blueprintable, meta = (ShortTooltip = "A Simple Player Controller is an actor responsible for controlling a Pawn used by the player."))
class T1PROJECT_API AGsSimplePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AGsSimplePlayerController() = default;

public:
	virtual void InitPlayerState() override {};
	virtual void SpawnPlayerCameraManager() override {};
	virtual void SpawnDefaultHUD() override {};
	virtual void AddCheats(bool bForce = false) override {}

public:
	virtual void GetPlayerViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;
};


