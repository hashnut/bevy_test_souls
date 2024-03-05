// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsActorLobbyObject.generated.h"

UCLASS()
class T1PROJECT_API AGsActorLobbyObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGsActorLobbyObject();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool _isEffectStart = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeActor(bool inIsMale);

	UFUNCTION(BlueprintImplementableEvent)
	void ResetAllEffect();

	UFUNCTION()
	void SetIsEffectStart(bool inIsEffectStart) { _isEffectStart = inIsEffectStart; }
};
