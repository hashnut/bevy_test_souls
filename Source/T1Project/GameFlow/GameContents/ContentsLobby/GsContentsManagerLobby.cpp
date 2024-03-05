// Fill out your copyright notice in the Description page of Project Settings.
#include "GsContentsManagerLobby.h"

#include "GsStateLobbyServerSelect.h"
#include "GsStateLobbyCharacterSelect.h"
#include "GsStateLobbyCharacterCreate.h"
#include "GsStateLobbyCharacterCreateEmpty.h"
#include "GsStateLobbyIntro.h"
#include "GsStateLobbyLogin.h"
#include "GsStateLobbyCharacterCustomizing.h"
#include "GsStateLobbyTitle.h"
#include "GsStateLobbyNetAddressSelect.h"
#include "GsStateLobbyPatch.h"
#include "GsStateLobbyRepairCheck.h"
#include "GsStateLobbyHiveInit.h"
#include "GsStateLobbyDivergence.h"
#include "GsStateLobbyHiveAuth.h"
#include "GsStateLobbyOfflinePlayBackGround.h"
//#include "GsStateLobbyHiveCheckMaintenance.h"
#include "GsStateLobbyHiveCheckAuthMaintenance.h"
#include "GsStateLobbyHiveCheckGateWayMaintenance.h"
#include "GsStateLobbyCheckNetHandshakeComplet.h"
#include "GsStateLobbyCheckPatch.h"
#include "GsStateLobbyHerculesAuth.h"
#include "GsStateLobbyPrePlayableMovie.h"
#include "GsStateLobbyPostPlayableMovie.h"
#include "GsStateLobbyZpay.h"

#include "GameFlow/GameContents/ContentsLobby/Hud/GsStateLobbyHud.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsGameBase.h"

#include "Container/GsSharedMap.h"


#define LOCTEXT_NAMESPACE "PD::Result" 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------

FGsContentsLobbyBase* FGsContentsLobbyAllocator::Alloc(FGsContentsMode::InLobby inMode)
{
	switch (inMode)
	{
	case FGsContentsMode::InLobby::INTRO:
		return new FGsStateLobbyIntro();
	case FGsContentsMode::InLobby::LOGIN:
		return new FGsStateLobbyLogin();
	case FGsContentsMode::InLobby::SERVER_SELCET:
		return new FGsStateLobbyServerSelect();
	case FGsContentsMode::InLobby::CHARACTER_SELECT:
		return new FGsStateLobbyCharacterSelect();
	case FGsContentsMode::InLobby::CHARACTER_CREATE:
		return new FGsStateLobbyCharacterCreate();
	case FGsContentsMode::InLobby::CHARACTER_CREATE_EMPTY_STATE:
		return new FGsStateLobbyCharacterCreateEmpty();
	case FGsContentsMode::InLobby::CHARACTER_CUSTOMIZING:
		return new FGsStateLobbyCharacterCustomizing();
	case FGsContentsMode::InLobby::TITLE:
		return new FGsStateLobbyTitle();
	case FGsContentsMode::NETADDRESS_SELECT:
		return new FGsStateLobbyNetAddressSelect();
	case FGsContentsMode::PATCH:
		return new FGsStateLobbyPatch();
	case FGsContentsMode::REPAIR_CHECK:
		return new FGsStateLobbyRepairCheck();
	case FGsContentsMode::HIVE_INIT:
		return new FGsStateLobbyHiveInit();
	case FGsContentsMode::DIVERGENCE:
		return new FGsStateLobbyDivergence();
	case FGsContentsMode::HIVE_AUTH:
		return new FGsStateLobbyHiveAuth();
	/*case FGsContentsMode::HIVECHECK_MAINTENANCE:
		return new FGsStateLobbyHiveCheckMaintenance();*/
	case FGsContentsMode::HIVECHECK_AUTH_MAINTENANCE:
		return new FGsStateLobbyHiveCheckAuthMaintenance();
	case FGsContentsMode::HIVECHECK_GATEWAY_MAINTENANCE:
		return new FGsStateLobbyHiveCheckGateWayMaintenance();
	case FGsContentsMode::OFFLINE_PLAY_BACKGROUND:
		return new FGsStateLobbyOfflinePlayBackGround();
	case FGsContentsMode::InLobby::HUD:
		return new FGsStateLobbyHud();
	case FGsContentsMode::InLobby::NETCHECK_HANDSHAKECOMPLETED:
		return new FGsStateLobbyCheckNetHandshakeComplet();
	case FGsContentsMode::PATCH_CHECK:
		return new FGsStateLobbyCheckPatch();
	case FGsContentsMode::HERCULES_AUTH:
		return new FGsStateLobbyHerculesAuth();
	case FGsContentsMode::PRE_PLAYABLE_MOVIE:
		return new FGsStateLobbyPrePlayableMovie();
	case FGsContentsMode::POST_PLAYABLE_MOVIE:
		return new FGsStateLobbyPostPlayableMovie();
	case FGsContentsMode::Z_PAY:
		return new FGsStateLobbyZpay();
	case FGsContentsMode::InLobby::MAX:
	default:
		return nullptr;
	}

	return nullptr;
}

//------------------------------------------------------------------------------
// 게임모드를 관리
//------------------------------------------------------------------------------
void FGsContentsManagerLobby::RemoveAll()
{	
	for (auto& iter : GetContainer())
	{
		iter.Value->Close();
	}

	Super::RemoveAll();
}

void FGsContentsManagerLobby::InitState()
{
	const FGsContentsMode::InLobby allMode[]=
	{ 
		FGsContentsMode::InLobby::INTRO,
		FGsContentsMode::InLobby::LOGIN,
		FGsContentsMode::InLobby::SERVER_SELCET,
		FGsContentsMode::InLobby::CHARACTER_SELECT,
		FGsContentsMode::InLobby::CHARACTER_CREATE,
		FGsContentsMode::InLobby::CHARACTER_CUSTOMIZING,
		FGsContentsMode::InLobby::CHARACTER_CREATE_EMPTY_STATE,
		FGsContentsMode::InLobby::TITLE,
		FGsContentsMode::InLobby::NETADDRESS_SELECT,
		FGsContentsMode::InLobby::PATCH,
		FGsContentsMode::InLobby::REPAIR_CHECK,
		FGsContentsMode::InLobby::HIVE_INIT,
		//FGsContentsMode::InLobby::HIVECHECK_MAINTENANCE,
		FGsContentsMode::InLobby::HIVECHECK_AUTH_MAINTENANCE,
		FGsContentsMode::InLobby::HIVECHECK_GATEWAY_MAINTENANCE,
		FGsContentsMode::InLobby::DIVERGENCE,
		FGsContentsMode::InLobby::HIVE_AUTH,
		FGsContentsMode::InLobby::OFFLINE_PLAY_BACKGROUND,
		FGsContentsMode::InLobby::HUD,
		FGsContentsMode::InLobby::NETCHECK_HANDSHAKECOMPLETED,
		FGsContentsMode::InLobby::PATCH_CHECK,
		FGsContentsMode::InLobby::HERCULES_AUTH,
		FGsContentsMode::InLobby::PRE_PLAYABLE_MOVIE,
		FGsContentsMode::InLobby::POST_PLAYABLE_MOVIE,
		FGsContentsMode::InLobby::Z_PAY,
	};

	for (auto& e : allMode)
	{
		auto inst = MakeInstance(e);
		inst->Init();
	}

	Super::InitState();	
}

void FGsContentsManagerLobby::Update(float In_deltaTime)
{
	if(TSharedPtr<FGsContentsLobbyBase> currState = GetCurrentState().Pin())
	{
		return currState->Update(In_deltaTime);
	}
}

FGsContentsMode::InLobby FGsContentsManagerLobby::GetCurrentContentsMode() const
{
	if(TSharedPtr<FGsContentsLobbyBase> currState = GetCurrentState().Pin())
	{
		return currState->GetType();
	}

	return FGsContentsMode::InLobby::MAX;
}

#undef LOCTEXT_NAMESPACE 
