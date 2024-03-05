#include "GsStageManagerLobby.h"

#include "GsStageLobbyIntro.h"
#include "GsStageLobbyTitle.h"
#include "GsStageLobbyCharacterSelect.h"
#include "GsStageLobbyCharacterCreate.h"
#include "GsStageLobbyCharacterCustomizing.h"
#include "GsStageLobbyPlayableEvent.h"
#include "GsStageLobbyPatch.h"

#include "GameFlow/Stage/StageGame/GsStageGameBase.h"
#include "Classes/GsStateMng.h"
#include "GsStageLobbyBase.h"


//------------------------------------------------------------------------------
// 게임모드를 관리할때 필요한 인스턴스 할당관리자(동일 인스턴스가 아닌 상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
FGsStageLobbyBase* FGsStageLobbyAllocator::Alloc(FGsStageMode::Lobby inMode)
{
	switch (inMode)
	{
	case FGsStageMode::INTRO:
		return new FGsStageLobbyIntro();
	case FGsStageMode::TITLE:
		return new FGsStageLobbyTitle();
	case FGsStageMode::CHARACTER_SELECT:
		return new FGsStageLobbyCharacterSelect();
	case FGsStageMode::CHARACTER_CREATE:
		return new FGsStageLobbyCharacterCreate();
	case FGsStageMode::CHARACTER_CUSTOMIZING:
		return new FGsStageLobbyCharacterCustomizing();	
	case FGsStageMode::PLAYABLE_EVENT:
		return new FGsStageLobbyPlayableEvent();
	case FGsStageMode::PATCH:
		return new FGsStageLobbyPatch();
	case FGsStageMode::LOBBY_MAX:
	default:
		return NULL;
	}
	return NULL;
}

//------------------------------------------------------------------------------
// 게임모드를 관리
//------------------------------------------------------------------------------
void FGsStageManagerLobby::RemoveAll()
{
	Super::RemoveAll();
}

void FGsStageManagerLobby::InitState()
{
	const FGsStageMode::Lobby allMode[]=
	{ 		
		FGsStageMode::Lobby::INTRO,
		FGsStageMode::Lobby::TITLE,
		FGsStageMode::Lobby::CHARACTER_SELECT,
		FGsStageMode::Lobby::CHARACTER_CREATE,		
		FGsStageMode::Lobby::CHARACTER_CUSTOMIZING,		
		FGsStageMode::Lobby::PLAYABLE_EVENT,
		FGsStageMode::Lobby::PATCH,
	};

	for (auto& e : allMode)
	{
		auto inst = MakeInstance(e);
		inst->Init();
	}

	Super::InitState();
}

void FGsStageManagerLobby::InitMessageHandler()
{
	
}

void FGsStageManagerLobby::Update(float In_deltaTime)
{
	/*if (0 == GetCurrentState().GetSharedReferenceCount())
	{
		ensure(0 == GetCurrentState().GetSharedReferenceCount());
	}

	GSLOG(Log, TEXT("FGsStageManagerLobby::Update GetCurrentState().GetSharedReferenceCount() : %d"),
		GetCurrentState().GetSharedReferenceCount());*/

	if(auto currState = GetCurrentState().Pin())
	{
		currState->Update(In_deltaTime);
	}
}

FGsStageMode::Lobby FGsStageManagerLobby::GetCurrentStageMode() const
{
	if (auto currState = GetCurrentState().Pin())
	{
		return currState->GetType();
	}

	return FGsStageMode::Lobby::LOBBY_MAX;
}