// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define  DROPOJBECT_EFFECT false

#include "CoreMinimal.h"
#include "GameObject/ObjectClass/GsGameObjectPropObject.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "GsDropObjectSpawnComponent.generated.h"

class UGsSimpleTimer;
class UCurveFloat;

/**
 * 
 */
 
UCLASS()
class T1PROJECT_API UGsDropObjectSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

private:	
	float _degree;
	FVector _direction;
	FVector _startLocation;
	FVector _destLocation;
	UPROPERTY()
	UGsSimpleTimer* _timer;
	UPROPERTY()
	UCurveFloat* _spawnCurve;
	float _height;

public:
	UGsDropObjectSpawnComponent();	
	virtual void EndPlay(const EEndPlayReason::Type inReason) override;

public:
	void Start(const FVector& inStart, const FVector& inDest);
	virtual void TickComponent(float inDelta, ELevelTick inTickType, FActorComponentTickFunction* inThisTickFunction) override;
	UFUNCTION()
	void OnSpawn(float inDelta);
	UFUNCTION()
	void OnFinishSpawn(const UObject* inTimer);	
};
