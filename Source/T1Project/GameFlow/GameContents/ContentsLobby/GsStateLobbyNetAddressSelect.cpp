// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyNetAddressSelect.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
//#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"

//#include "Sound/GsSoundPlayer.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"


#include "Data/GsLobbyConstant.h"
#include "Data/GsDataContainManager.h"
#include "UI/UIContent/Window/GsUIWindowNetAddress.h"
#include "TimerManager.h"


FGsStateLobbyNetAddressSelect::FGsStateLobbyNetAddressSelect() : FGsContentsLobbyBase(FGsContentsMode::InLobby::NETADDRESS_SELECT)
{
}

FGsStateLobbyNetAddressSelect::~FGsStateLobbyNetAddressSelect()
{
}

void FGsStateLobbyNetAddressSelect::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyNetAddressSelect::Enter()"));

	// 메세지 등록
	RegisterMessages();

#if WITH_EDITOR
	if (UGsUIManager* uiManager = GUI())
	{		
		if (const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData())
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(
				lobbyConstant->_pathWindowNetAddressSelect.Id, lobbyConstant->_pathWindowNetAddressSelect.Path);
			if (widget.IsValid())
			{
				_windowUI = Cast<UGsUIWindowNetAddress>(widget);
				_windowUI->OnClickServer.BindRaw(this, &FGsStateLobbyNetAddressSelect::OnClickSelectServer);
				_windowUI->SetData();
			}

			/*if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
			{
				soundPlayer->PlaySoundBgmByPath(lobbyConstant->_pathBgmTitle);
			}*/
		}
	}
#else
	OnClickSelectServer(0);	
#endif

	FGsUIHelper::TrayLobbyFadeIn(1.5f);
}

void FGsStateLobbyNetAddressSelect::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MUI& mUI = messageMgr->GetUI();
	_msgHandleUIList.Emplace(mUI.AddRaw(MessageUI::TITLE_CLICK_START, this, &FGsStateLobbyNetAddressSelect::OnClick));
}

void FGsStateLobbyNetAddressSelect::UnregisterMessages()
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		MUI& mUI = messageMgr->GetUI();
		for (MsgUIHandle& handleUI : _msgHandleUIList)
		{
			mUI.Remove(handleUI);
		}
	}
	_msgHandleUIList.Empty();	
}

void FGsStateLobbyNetAddressSelect::Exit()
{
	if (_windowUI.IsValid())
	{
		_windowUI->Close();
	}

	StopTimer();
	UnregisterMessages();
	
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyNetAddressSelect::Update(float In_deltaTime)
{
	FGsContentsLobbyBase::Update(In_deltaTime);
}

void FGsStateLobbyNetAddressSelect::StartTimer()
{
	FGsContentsLobbyBase::StartTimer();

	const UWorld* world = GHive()->GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}

	if (nullptr != world)
	{
		world->GetTimerManager().SetTimer(_timerHandle,
			FTimerDelegate::CreateRaw(this, &FGsStateLobbyNetAddressSelect::EndTimer),
			7.0f, false);
	}

	Blocking();
}

void FGsStateLobbyNetAddressSelect::EndTimer()
{
	FGsContentsLobbyBase::EndTimer();

	const UWorld* world = GHive()->GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}

	UnBlocking();
}

void FGsStateLobbyNetAddressSelect::StopTimer()
{
	FGsContentsLobbyBase::StopTimer();

	const UWorld* world = GHive()->GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}

	UnBlocking();
}

void FGsStateLobbyNetAddressSelect::OnClick(const IGsMessageParam* InParam)
{
	if (IsBlocking())
	{
		return;
	}

#if !WITH_EDITOR
	OnClickSelectServer(0);
#endif
}

void FGsStateLobbyNetAddressSelect::OnClickSelectServer(int inIndex)
{	
	GNet()->SetServerIndex(inIndex);

	StartTimer();

	FGsLobbyFlowMsgParam param(MessageStage::SELECTED_NETADDRESS);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);	
}
