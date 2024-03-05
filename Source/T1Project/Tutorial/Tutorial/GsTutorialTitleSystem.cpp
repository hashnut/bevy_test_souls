// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialTitleSystem.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "DataSchema/InterServer/GsSchemaInvadeBattleLevel.h"
#include "DataSchema/Map/GsSchemaMapData.h"


bool FGsTutorialTitleSystem::OnPrevStart()
{
	// 침공전 중 진행불가(침공전 탭이 우선 잡히기 때문에 튜토리얼 꼬임)
	const FGsSchemaMapData* currentMapData = GLevel()->GetCurrentMapInfo();
	if (GSInterServer() && GGameData() && currentMapData)
	{
		if (GSInterServer()->IsValidInvasionMap(currentMapData->id) || GGameData()->IsInvadeWorld())
		{
			return false;
		}
	}

	return Super::OnPrevStart();
}
