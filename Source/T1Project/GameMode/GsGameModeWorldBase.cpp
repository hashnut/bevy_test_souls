// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameModeWorldBase.h"
#include "Engine/Engine.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/WorldComposition.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "Runtime/Engine/Classes/Engine/LevelScriptActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Core/Public/Misc/MessageDialog.h"
#include "Runtime/Landscape/classes/Landscape.h"
#include "Runtime/Core/Public/Math/Box.h"
#include "GameFramework/PlayerStart.h"
#include "NavigationSystem.h"

#include "T1Project.h"
#include "GsGameInstance.h"
#include "Level/TargetPoint/PlayerSpawnPoint.h"
#include "Level/LevelScriptActor/GsLevelScriptActor.h"
#include "Level/GsLevelDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ControllerEx/GsPlayerController.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Data/GsDataContainManager.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/GsMessageHolder.h"

#include "Cheat/GsCheatManager.h"

//network
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Net/GsNetSendService.h"
#include "GameObject/HUD/GsHUDLocalPlayer.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GameObject/Define/EGsArtLevelLoadConditionType.h"

void AGsGameModeWorldBase::StartPlay()
{
	Super::StartPlay();

#if SPAWN_LOCATION_DEBUG
	UGsLevelManager::SetGameWorld(GetWorld());
#endif

	UnloadDesignStreamingLevel();
	ClearDesignSteamingLevel();
	UnloadArtStreamingLevel();
	ClearArtSteamingLevel();

	GMessage()->GetSound().SendMessage(MessageSound::STOP_BGM, nullptr);
	//GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);
}

void AGsGameModeWorldBase::EndPlay(const EEndPlayReason::Type inEndPlayReason)
{
	UnloadDesignStreamingLevel();
	ClearDesignSteamingLevel();
	UnloadArtStreamingLevel();
	ClearArtSteamingLevel();

#if SPAWN_LOCATION_DEBUG
	UGsLevelManager::ClearGameWorld();
#endif

	GMessage()->GetSound().SendMessage(MessageSound::STOP_BGM, nullptr);

	Super::EndPlay(inEndPlayReason);
}

void AGsGameModeWorldBase::UnloadDesignStreamingLevel()
{
	UnlaodStreamingLevelInternal(_designLevelArray);
}

void AGsGameModeWorldBase::ClearDesignSteamingLevel()
{
	_designLevelArray.Empty();
}

bool AGsGameModeWorldBase::TryLoadCurrentDesignStreamingLevel()
{
	int32 levelId = GLevel()->GetCurrentLevelId();
	FString designLevelName = UGsLevelManager::GetDesignLevelFullPath(GetWorld(), levelId);

	return TryLoadStreamingLevelInternal(designLevelName, _designLevelArray);
}

void AGsGameModeWorldBase::UnloadArtStreamingLevel()
{
	UnlaodStreamingLevelInternal(_artLevelArray);
}

void AGsGameModeWorldBase::ClearArtSteamingLevel()
{
	_artLevelArray.Empty();
}

bool AGsGameModeWorldBase::TryLoadArtStreamingLevel(bool inIsForceLoad)
{
	int32 levelId = GLevel()->GetCurrentLevelId();
	FString artLevelName = UGsLevelManager::GetArtLevelFullPath(GetWorld(), levelId);

	if (inIsForceLoad)
	{
		return TryLoadStreamingLevelInternal(artLevelName, _artLevelArray);
	}

	if (IsNeedToLoadArtLevel(levelId))
	{
		return TryLoadStreamingLevelInternal(artLevelName, _artLevelArray);
	}

	return false;
}

bool AGsGameModeWorldBase::IsNeedToLoadArtLevel(int32 inLevelId)
{
	const FGsSchemaMapData* mapData = UGsLevelManager::GetMapData(inLevelId);
	if (nullptr == mapData)
	{
		return false;
	}

	switch (mapData->artLevelLoadCondition)
	{
	case EGsArtLevelLoadConditionType::Load:
	{
		return true;
	}
	break;
	case EGsArtLevelLoadConditionType::ByOption:
	{
		if (UGsGameUserSettings* userSetting = GGameUserSettings())
		{
			//조건에 대해 논의 필요		
			TMap<EGsOptionGraphic, int32> graphicMap;
			userSetting->GetGraphicSettings(graphicMap);
			if (graphicMap.Contains(EGsOptionGraphic::GRAPHIC_GRADE))
			{
				return graphicMap[EGsOptionGraphic::GRAPHIC_GRADE] != 1;
			}
		}
	}
	break;
	case EGsArtLevelLoadConditionType::ByScalabilty:
		//조건에 대해 논의 필요
		break;
	}

	return false;
}

void AGsGameModeWorldBase::UnlaodStreamingLevelInternal(TArray<TWeakObjectPtr<ULevelStreamingDynamic>>& inLevelArray)
{
	for (TWeakObjectPtr<ULevelStreaming> level : inLevelArray)
	{
		if (level.IsValid())
		{
			level.Get()->SetIsRequestingUnloadAndRemoval(true);
		}
	}

	inLevelArray.Empty();
}

bool AGsGameModeWorldBase::TryLoadStreamingLevelInternal(const FString& inLevelName, TArray<TWeakObjectPtr<ULevelStreamingDynamic>>& inLevelArray)
{
	bool isSuccess;
	ULevelStreamingDynamic* level = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), inLevelName, FVector::ZeroVector, FRotator::ZeroRotator, isSuccess);
	if (level)
	{
		inLevelArray.Add(level);
		return true;
	}

	return false;
}
