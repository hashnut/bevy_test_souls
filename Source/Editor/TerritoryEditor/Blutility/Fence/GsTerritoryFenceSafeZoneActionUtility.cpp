// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceSafeZoneActionUtility.h"
#include "Blutility/Public/EditorUtilityLibrary.h"
#include "Actor/Fence/GsTerritoryFenceBase.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Actor/Fence/GsTerritoryFenceSafeZone.h"


void UGsTerritoryFenceSafeZoneActionUtility::EditSound(FGsSchemaSoundResDataRow inSound)
{
	AGsTerritoryFenceSafeZone* fence = Cast<AGsTerritoryFenceSafeZone>(GetSelectedFence());
	if (nullptr == fence)
	{
		return;
	}

	fence->SetSound(inSound);
}
