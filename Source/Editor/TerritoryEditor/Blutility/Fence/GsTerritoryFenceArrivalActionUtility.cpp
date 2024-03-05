// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceArrivalActionUtility.h"
#include "Blutility/Public/EditorUtilityLibrary.h"
#include "Actor/Fence/GsTerritoryFenceBase.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Actor/Fence/GsTerritoryFenceArrival.h"

void UGsTerritoryFenceArrivalActionUtility::EditSound(FGsSchemaSoundResDataRow inSound)
{
	AGsTerritoryFenceArrival* fence = Cast<AGsTerritoryFenceArrival>(GetSelectedFence());
	if (nullptr == fence)
	{
		return;
	}

	fence->SetSound(inSound);
}
