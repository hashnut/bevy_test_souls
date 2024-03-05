// Fill out your copyright notice in the Description page of Project Settings.


#include "GsScopeLobby.h"

#include "ScopeLobby/GsLobbyDataManager.h"
#include "ScopeLobby/GsLobbyCharacterManager.h"
#include "ScopeLobby/GsLobbyPlayableEventManager.h"
#include "ScopeLobby/GsLobbyCharacterPresetManager.h"

void UGsScopeLobby::Create()
{
	UGsScopeBase::Create();

	for (uint8 mgrType = 0; mgrType < EManagerType::Max; ++mgrType)
	{
		switch (mgrType) 
		{
		case EManagerType::LobbyCharacter:
			CreateUObjectManager<UGsLobbyCharacterManager>(this);
			break;
		case EManagerType::LobbyData:
			CreateUObjectManager<UGsLobbyDataManager>(this);
			break;
		case EManagerType::PlayableEvent:
			CreateUObjectManager<UGsLobbyPlayableEventManager>(this);
			break;
		case EManagerType::LobbyCharacterPreset:
			CreateUObjectManager<UGsLobbyCharacterPresetManager>(this);
			break;
		default:
			check(false);
			break;
		}
	}
}
