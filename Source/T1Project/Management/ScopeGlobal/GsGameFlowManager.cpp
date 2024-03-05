#include "GsGameFlowManager.h"
#include "GameFlow/GsGameFlowBase.h"
#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/GsGameFlowGame.h"
#include "Classes/GsObjectState.h"
#include "Management/GsMessageHolder.h"
#include "GsNetManager.h"
#include "GameFlow/GsGameFlow.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
FGsGameFlowBase* FGsGameFlowAllocator::Alloc(FGsGameFlow::Mode inMode)
{
	if (FGsGameFlow::Mode::LOBBY == inMode)
	{
		return new FGsGameFlowLobby();
	}
	else if (FGsGameFlow::Mode::GAME == inMode)
	{
		return new FGsGameFlowGame();
	}
	return nullptr;
}


//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
void FGsGameFlowManager::Initialize()
{
	// MakeInstance
	for (int i = FGsGameFlow::Mode::LOBBY; i < FGsGameFlow::Mode::MAX; i++)
	{
		TSharedPtr<FGsGameFlowBase> instance = MakeInstance(static_cast<FGsGameFlow::Mode>(i));
		if(instance.IsValid())
		{
			instance->Init();
		}
	}
	ChangeState(FGsGameFlow::Mode::LOBBY);

	InitState();

	InitializeMessage();
}


void FGsGameFlowManager::Finalize()
{
	RemoveAll();

	FinalizeMessage();
}

void FGsGameFlowManager::Update(float inTick)
{
	if (nullptr != CurrentState && CurrentState.IsValid())
	{
		CurrentState.Pin()->Update(inTick);
	}
}

void FGsGameFlowManager::OnReconnectionEnd()
{
	if (nullptr != CurrentState && CurrentState.IsValid())
	{
#if RECONNECT_DEBUG
		GSLOG(Log, TEXT("FGsGameFlowManager - OnReconnectionEnd : %s"), *state->ToString());
#endif

		CurrentState.Pin()->OnReconnectionEnd();
	}
}

void FGsGameFlowManager::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();
	
	// 시스템 메시지 등록
	MSystem& system = msg->GetSystem();
	InitializeMessageDelegateList(&system);

	AddMessageDelegate(system.AddRaw(MessageSystem::AHUD_BEGIN_PLAY,
		this, &FGsGameFlowManager::OnChangeGameFlow));

	AddMessageDelegate(system.AddRaw(MessageSystem::RECONNECT_END,
		this, &FGsGameFlowManager::OnReconnectionEnd));

	AddMessageDelegate(system.AddRaw(MessageSystem::BACKTO_LOGIN,
		this, &FGsGameFlowManager::OnBacktoLogin));

	AddMessageDelegate(system.AddRaw(MessageSystem::BACKTO_LOBBY,
		this, &FGsGameFlowManager::OnBacktoLobby));

	AddMessageDelegate(system.AddRaw(MessageSystem::BACKTO_TITLE,
		this, &FGsGameFlowManager::OnBackToTitle));

	AddMessageDelegate(system.AddRaw(MessageSystem::RESTART_GAME,
		this, &FGsGameFlowManager::OnRestartGame));

	AddMessageDelegate(system.AddRaw(MessageSystem::EXIT_GAME,
		this, &FGsGameFlowManager::OnExitGame));

	AddMessageDelegate(system.AddRaw(MessageSystem::QUIT_GAME,
		this, &FGsGameFlowManager::OnQuitGame));
}

void FGsGameFlowManager::FinalizeMessage()
{
	//시스템 메지시 삭제
	RemoveAllMessageDelegate();	
}

void FGsGameFlowManager::OnChangeGameFlow()
{
	GSLOG(Log, TEXT("OnChangeGameFlow s"));
	ChangeState(FGsGameFlow::Mode::GAME);
	GSLOG(Log, TEXT("OnChangeGameFlow e"));
}

void FGsGameFlowManager::OnBacktoLogin()
{
	ChangeState(FGsGameFlow::Mode::LOBBY);
}

void FGsGameFlowManager::OnBacktoLobby()
{
	if (TSharedPtr<FGsGameFlowBase> state = GetCurrentFlow())
	{
		if (nullptr != state && state.IsValid())
		{
			if (FGsGameFlow::Mode::LOBBY == state->GetType())
			{				
				GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);				
			}
			else
			{
				ChangeState(FGsGameFlow::Mode::LOBBY);
			}
		}		
	}
}

void FGsGameFlowManager::OnBackToTitle()
{
	if (nullptr != CurrentState && CurrentState.IsValid())
	{
		CurrentState.Pin()->OnBackToTitle();
	}
}

void FGsGameFlowManager::OnExitGame()
{
	if (nullptr != CurrentState && CurrentState.IsValid())
	{
		CurrentState.Pin()->OnExitGame();
	}
}

void FGsGameFlowManager::OnQuitGame()
{
	if (nullptr != CurrentState && CurrentState.IsValid())
	{
		CurrentState.Pin()->OnQuitGame();
	}
}

void FGsGameFlowManager::OnRestartGame()
{
	if (nullptr != CurrentState && CurrentState.IsValid())
	{
		CurrentState.Pin()->OnRestartGame();
	}
}

TSharedPtr<FGsGameFlowBase> FGsGameFlowManager::GetCurrentFlow() const
{
	if (nullptr != CurrentState && CurrentState.IsValid())
	{
		return CurrentState.Pin();
	}

	return nullptr;
}

FGsGameFlow::Mode FGsGameFlowManager::GetCurrentFlowType() const
{
	if (nullptr != CurrentState && CurrentState.IsValid())
	{
		return CurrentState.Pin()->GetType();
	}

	return FGsGameFlow::Mode::MAX;
}

int32 FGsGameFlowManager::GetCurrentContentStateIndex() const
{
	if (nullptr != CurrentState && CurrentState.IsValid())
	{
		return CurrentState.Pin()->GetContentStateIndex();
	}

	return -1;
}

FGsGameFlowGame* FGsGameFlowManager::GetGameFlow() const
{
	if (nullptr != CurrentState && CurrentState.IsValid() && FGsGameFlow::Mode::GAME == CurrentState.Pin()->GetType())
	{
		return  StaticCastSharedPtr<FGsGameFlowGame>(CurrentState.Pin()).Get();
	}
	return nullptr;
}

FGsGameFlowLobby* FGsGameFlowManager::GetLobbyFlow() const
{
	if (nullptr != CurrentState && CurrentState.IsValid() && FGsGameFlow::Mode::LOBBY == CurrentState.Pin()->GetType())
	{
		return  StaticCastSharedPtr<FGsGameFlowLobby>(CurrentState.Pin()).Get();
	}
	return nullptr;
}
