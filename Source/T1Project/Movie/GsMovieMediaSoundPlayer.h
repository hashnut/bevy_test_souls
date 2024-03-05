// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MediaSoundComponent.h"
#include "GsMovieMediaSoundPlayer.generated.h"

UCLASS()
class T1PROJECT_API AGsMovieMediaSoundPlayer : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movie")
	UMediaSoundComponent* _mediaSoundComp;
	
public:	
	// Sets default values for this actor's properties
	AGsMovieMediaSoundPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
