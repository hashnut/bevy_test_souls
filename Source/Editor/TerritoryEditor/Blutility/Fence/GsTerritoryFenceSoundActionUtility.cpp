// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceSoundActionUtility.h"
#include "Actor/Fence/GsTerritoryFenceSound.h"


void UGsTerritoryFenceSoundActionUtility::EditSound(const TArray<FGsQuestSound> inQuestSoundArray)
{
	AGsTerritoryFenceSound* fence = Cast<AGsTerritoryFenceSound>(GetSelectedFence());
	if (nullptr == fence)
	{
		return;
	}

	fence->SetQuestSound(inQuestSoundArray);
}
