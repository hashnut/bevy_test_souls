// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryGameSpot.h"

//ue4
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/Texture2D.h"

//t1 project
#include "UTIL/GsText.h"
#include "UTIL/GsDir.h"

//territory editor
#include "Util/GsTerritoryZoneShapeUtil.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "GsTerritoryEditorSetting.h"

FString AGsTerritoryGameSpot::GetSpotDescription()
{
	return FString(TEXT("Index : ")) + FString::FromInt(_index) + FString(TEXT("\n"))
		+ FString(TEXT("Name : ")) + _name.ToString() + FString(TEXT("\n"))
		+ FString(TEXT("Team : ")) + FGsTextUtil::GetStringFromEnum("IffTeamType", _teamType) + FString(TEXT("\n"))		
		+ FString(TEXT("Random range : ")) + FString::SanitizeFloat(_radius);
}

AGsTerritorySpawnSpot::AGsTerritorySpawnSpot()
{
	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_spawnSpotColor;
	}
}

void AGsTerritorySpawnSpot::InvalidWidgetText()
{
	if (_isValid)
	{
		_description = FString(TEXT("SpawnSpot\n")) + GetSpotDescription();
	}

	SetTerritoryTitleText(TEXT("Spawn spot"));

	Super::InvalidWidgetText();
}

AGsTerritoryResurrectionSpot::AGsTerritoryResurrectionSpot()
{
	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_resurrectionSpotColor;
	}
}

void AGsTerritoryResurrectionSpot::InvalidWidgetText()
{
	if (_isValid)
	{
		_description = FString(TEXT("ResurrectionSpot\n")) + GetSpotDescription();
	}

	SetTerritoryTitleText(TEXT("Resurrect spot"));

	Super::InvalidWidgetText();
}

AGsTerritoryTeleportSpot::AGsTerritoryTeleportSpot()
{
	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_teleportSpotColor;
	}
}

void AGsTerritoryTeleportSpot::InvalidWidgetText()
{
	if (_isValid)
	{
		_description = FString(TEXT("TeleportSpot\n")) + GetSpotDescription();
	}

	SetTerritoryTitleText(TEXT("Teleport spot"));

	Super::InvalidWidgetText();
}
