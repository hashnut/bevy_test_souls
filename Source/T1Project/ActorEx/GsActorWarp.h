// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsActorPropObject.h"
#include "GsActorWarp.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UWidgetComponent;
class UGsDropObjectSpawnComponent;
class UParticleSystemComponent;

UCLASS()
class T1PROJECT_API AGsActorWarp : public AGsActorPropObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USphereComponent* _sphereCollision = nullptr;
	
public:	
	// Sets default values for this actor's properties
	AGsActorWarp();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type inType) override;

public:
	USphereComponent* GetSphereComponent() { return _sphereCollision; }
};
