#include "GsStageManagerGame.h"
#include "GsStageGameTown.h"
#include "GsStageGameField.h"
#include "GsStageGameDungeon.h"
#include "GsStageGameSafetyZone.h"
#include "GsStageGameBase.h"
#include "GsStageGameGuildDungeon.h"
#include "GsStageGameArenaReady.h"
#include "GsStageGameArena.h"
#include "GsStageGameWorldBoss.h"
#include "GsStageGameChaos.h"
#include "GsStageGameNonPK.h"
#include "GsStageGameAgitDungeon.h"

//------------------------------------------------------------------------------
// 게임모드를 관리할때 필요한 인스턴스 할당관리자(동일 인스턴스가 아닌 상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
FGsStageGameBase* FGsStageGameAllocator::Alloc(FGsStageMode::Game inMode)
{
	switch (inMode)
	{
	case FGsStageMode::Game::TOWN:
		return new FGsStageGameTown();
	case FGsStageMode::Game::CHAOS:
		return new FGsStageGameChaos();
	case FGsStageMode::Game::SAFETY_ZONE:
		return new FGsStageGameSafetyZone();
	case FGsStageMode::Game::NON_PK:
		return new FGsStageGameNonPK();
	case FGsStageMode::Game::FIELD:
		return new FGsStageGameField();
	case FGsStageMode::Game::DUNGEON:
		return new FGsStageGameDungeon();	
	case FGsStageMode::Game::GUILD_DUNGEON:
		return new FGsStageGameGuildDungeon();	
	case FGsStageMode::Game::ARENA_READY:
		return new FGsStageGameArenaReady();
	case FGsStageMode::Game::WORLD_BOSS:
		return new FGsStageGameWorldBoss();
	case FGsStageMode::Game::ARENA:
		return new FGsStageGameArena();
	case FGsStageMode::Game::AGIT_DUNGEON:
		return new FGsStageGameAgitDungeon();
	}

	return nullptr;
}
//------------------------------------------------------------------------------
// 게임모드를 관리
//------------------------------------------------------------------------------
void FGsStageManagerGame::RemoveAll()
{
	Super::RemoveAll();
}
void FGsStageManagerGame::InitState()
{
	const FGsStageMode::Game allMode[] =
	{
		FGsStageMode::Game::TOWN,
		FGsStageMode::Game::FIELD,
		FGsStageMode::Game::CHAOS,
		FGsStageMode::Game::DUNGEON,
		FGsStageMode::Game::GUILD_DUNGEON,		
		FGsStageMode::Game::SAFETY_ZONE,
		FGsStageMode::Game::ARENA_READY,
		FGsStageMode::Game::ARENA,
		FGsStageMode::Game::WORLD_BOSS,
		FGsStageMode::Game::NON_PK,
		FGsStageMode::Game::AGIT_DUNGEON,
	};

	for (auto& e : allMode)
	{
		auto inst = MakeInstance(e);
		inst->Init();
	}

	Super::InitState();
}

void FGsStageManagerGame::OnReconnectionEnd()
{
	if (TSharedPtr<FGsStageGameBase> state = GetCurrentState().Pin())
	{
		state->OnReconectionEnd();
	}
}

FGsStageMode::Game FGsStageManagerGame::GetCurrentStageMode() const
{
	if(TSharedPtr<FGsStageGameBase> currState = GetCurrentState().Pin())
	{
		return currState->GetType();
	}

	return FGsStageMode::Game::GAME_MAX;
}