// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsTerrEdUniqueActor.h"
#include "GsTerrEdRTTActor.generated.h"

UCLASS()
class TERRITORYEDITOR_API AGsTerrEdRTTActor : public AGsTerrEdUniqueActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, Category = "RTT", meta = (ShortToolTip = "Modify id using ground aciton"))
	int32 _destMapId;
	UPROPERTY(VisibleInstanceOnly, Category = "RTT", meta=(ShortToolTip="Modify id using ground aciton"))
	int32 _destSpotId;
	
public:	
	// Sets default values for this actor's properties
	AGsTerrEdRTTActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
