// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsActorCreateChManager.generated.h"

UCLASS()
class T1PROJECT_API AGsActorCreateChManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGsActorCreateChManager();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type inType) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnSetSelectCH(int inIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetUnselectCH();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartRoamingLuna();
};
