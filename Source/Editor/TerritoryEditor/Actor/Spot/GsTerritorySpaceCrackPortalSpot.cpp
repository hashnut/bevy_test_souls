// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritorySpaceCrackPortalSpot.h"

//ue4
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/Texture2D.h"
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/EditorEngine.h"

//t1 project
#include "UTIL/GsText.h"
#include "UTIL/GsDir.h"

//territory editor
#include "Util/GsTerritoryZoneShapeUtil.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "GsTerritoryEditorSetting.h"

AGsTerritorySpaceCrackPortalSpot::AGsTerritorySpaceCrackPortalSpot()
{
	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_spaceCrackSpotColor;
	}
}

void AGsTerritorySpaceCrackPortalSpot::InvalidWidgetText()
{
	if (_isValid)
	{
		_description = FString(TEXT("SpaceCrackPortalSpot\n")) + GetSpotDescription();
	}

	SetTerritoryTitleText(TEXT("SpaceCrackPortalSpot"));

	Super::InvalidWidgetText();
}