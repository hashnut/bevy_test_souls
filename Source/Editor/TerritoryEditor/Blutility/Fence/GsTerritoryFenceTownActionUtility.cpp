// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceTownActionUtility.h"
#include "Blutility/Public/EditorUtilityLibrary.h"
#include "Actor/Fence/GsTerritoryFenceBase.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Actor/Fence/GsTerritoryFenceTown.h"
#include "Editor/TerritoryEditor/Actor/Fence/GsTerritoryFenceTownChaos.h"

void UGsTerritoryFenceTownActionUtility::EditSound(FGsSchemaSoundResDataRow inSound)
{
	AGsTerritoryFenceTown* fence = Cast<AGsTerritoryFenceTown>(GetSelectedFence());
	if (nullptr == fence)
	{
		return;
	}

	fence->SetSound(inSound);
}

void UGsTerritoryFenceTownActionUtility::ChangeTownChaosFence()
{
	AGsTerritoryFenceTown* fence = Cast<AGsTerritoryFenceTown>(GetSelectedFence());

	FTransform trs;
	trs.SetLocation(fence->GetActorLocation());

	TArray<FVector> pointArray;
	fence->TryGetPointArray(pointArray);

	AGsTerritoryFenceBase* newTownChaos = FGsTerritoryLevelManager::CreateFenceTownChaos(trs, fence->_name, fence->_sound);
	if (nullptr == newTownChaos)
	{
		return;
	}

	AGsTerritoryFenceTownChaos* castTownChaos = Cast<AGsTerritoryFenceTownChaos>(newTownChaos);
	if (nullptr == castTownChaos)
	{
		return;
	}

	castTownChaos->SetPointArray(pointArray);

	fence->DestroyManually();
}