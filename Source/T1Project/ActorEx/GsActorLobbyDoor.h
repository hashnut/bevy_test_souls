// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsActorLobbyDoor.generated.h"

UCLASS()
class T1PROJECT_API AGsActorLobbyDoor : public AActor
{
	GENERATED_BODY()

private:
	TFunction<void()> _moveCallback;
	TFunction<void()> _camCallback;
	TFunction<void()> _enterCallback;
	
public:	
	// Sets default values for this actor's properties
	AGsActorLobbyDoor();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "GsActorLobbyDoor")
	void OnUserStartMove();

	UFUNCTION(BlueprintCallable, Category = "GsActorLobbyDoor")
	void OnCamStartMove();
	
	UFUNCTION(BlueprintCallable, Category = "GsActorLobbyDoor")
	void OnEnterGame();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInitialized();

	void OnStartGameAction(TFunction<void()> InMoveCallback, TFunction<void()> InCamCallback, TFunction<void()> InEnterCallback);
};
