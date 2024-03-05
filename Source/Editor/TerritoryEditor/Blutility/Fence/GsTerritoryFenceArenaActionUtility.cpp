// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceArenaActionUtility.h"
#include "Blutility/Public/EditorUtilityLibrary.h"
#include "Actor/Fence/GsTerritoryFenceBase.h"
#include "Actor/Fence/GsTerritoryFenceArena.h"
#include "Util/GsTerritoryEditorUtil.h"

void UGsTerritoryFenceArenaActionUtility::EditSound(FGsSchemaSoundResDataRow inSound)
{
	AGsTerritoryFenceArena* fence = Cast<AGsTerritoryFenceArena>(GetSelectedFence());
	if (nullptr == fence)
	{
		return;
	}

	fence->SetSound(inSound);
}
