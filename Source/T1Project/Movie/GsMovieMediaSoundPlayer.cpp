// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMovieMediaSoundPlayer.h"
#include "MediaSoundComponent.h"

// Sets default values
AGsMovieMediaSoundPlayer::AGsMovieMediaSoundPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mediaSoundComp = CreateDefaultSubobject<UMediaSoundComponent>("MediaSound");
}

// Called when the game starts or when spawned
void AGsMovieMediaSoundPlayer::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AGsMovieMediaSoundPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

