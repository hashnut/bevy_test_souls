// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceArenaReadyActionUtility.h"
#include "Blutility/Public/EditorUtilityLibrary.h"
#include "Actor/Fence/GsTerritoryFenceBase.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Actor/Fence/GsTerritoryFenceArenaReady.h"

void UGsTerritoryFenceArenaReadyActionUtility::EditSound(FGsSchemaSoundResDataRow inSound)
{
	AGsTerritoryFenceArenaReady* fence = Cast<AGsTerritoryFenceArenaReady>(GetSelectedFence());
	if (nullptr == fence)
	{
		return;
	}

	fence->SetSound(inSound);
}
