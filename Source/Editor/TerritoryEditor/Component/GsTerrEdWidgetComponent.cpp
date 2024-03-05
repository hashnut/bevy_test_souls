// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdWidgetComponent.h"
//#include "T1Project/UTIL/GsEditorUtile.h"
#include "T1Project/UTIL/GsUIUtil.h"
#include "Editor/GsReregisterComponentInterface.h"
#include "Editor/TerritoryEditor/GsTerritoryLevelManager.h"

UGsTerrEdWidgetComponent::UGsTerrEdWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.2f;

#if WITH_EDITOR
	bTickInEditor = true;
#endif
}

bool UGsTerrEdWidgetComponent::IsInViewDistance(UWorld* inWorld, enum ELevelTick inTickType)
{
	FVector location;

	if (ELevelTick::LEVELTICK_ViewportsOnly == inTickType)
	{
#if WITH_EDITOR
		if (UGsUIUtil::TryGetEditorCameraLocation(location))
		{
			return ((location - GetComponentLocation()).Size() < FGsTerritoryLevelManager::_widgetVisibleDistance);
		}
#endif
	}
	else
	{
		if (UGsUIUtil::TryGetCameraLocation(inWorld, location))
		{
			return ((location - GetComponentLocation()).Size() < FGsTerritoryLevelManager::_widgetVisibleDistance);
		}
	}

	return false;
}
