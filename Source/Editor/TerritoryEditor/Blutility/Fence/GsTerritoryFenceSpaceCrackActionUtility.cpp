// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceSpaceCrackActionUtility.h"
#include "Actor/Fence/GsTerritoryFenceSpaceCrack.h"

void UGsTerritoryFenceSpaceCrackActionUtility::EditEnvEvent(EGsEnvEvent inEnvEvent)
{
	AGsTerritoryFenceSpaceCrack* fence = Cast<AGsTerritoryFenceSpaceCrack>(GetSelectedFence());
	if (nullptr == fence)
	{
		return;
	}

	fence->SetEnvEvent(inEnvEvent);
}