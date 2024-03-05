// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsCampNavModifier.generated.h"

class UNavModifierComponent;
/**
 * Camp NavModifier Class
 */
UCLASS()
class T1PROJECT_API AGsCampNavModifier : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AGsCampNavModifier();

public:
	void SetNavModifierSize(FVector InSize);
	void SetActiveModifier(bool InState);
	FVector GetFailSafePos();
	void InvalidateModifier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
