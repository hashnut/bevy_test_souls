// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSimplePlayerController.h"

#include "GameObject/GsGlobalGameObjects.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"


void AGsSimplePlayerController::GetPlayerViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	if (UGsGameObjectLocalPlayer* local = GGamObjects()->GetLocalPlayer())
	{
		out_Location = local->GetLocation();
		out_Rotation = local->GetRotation();

		GetPawn()->SetActorLocation(out_Location);
	}
}