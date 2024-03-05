// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorLobbyDoor.h"

// Sets default values
AGsActorLobbyDoor::AGsActorLobbyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGsActorLobbyDoor::OnUserStartMove()
{
	if (nullptr != _moveCallback)
	{
		_moveCallback();
	}
}

void AGsActorLobbyDoor::OnCamStartMove()
{
	if (nullptr != _camCallback)
	{
		_camCallback();
	}
}

void AGsActorLobbyDoor::OnEnterGame()
{
	if (nullptr != _enterCallback)
	{
		_enterCallback();
	}
}

void AGsActorLobbyDoor::OnStartGameAction(TFunction<void()> InMoveCallback, TFunction<void()> InCamCallback, TFunction<void()> InEnterCallback)
{
	_moveCallback = InMoveCallback;
	_camCallback = InCamCallback;
	_enterCallback = InEnterCallback;

	StartGame();
}