// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyServerSelect.h"

#include "Net/GsNetSendService.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsLobbyConstant.h"

#include "UI/UIContent/Window/GsUIWindowServerSelect.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"


FGsStateLobbyServerSelect::FGsStateLobbyServerSelect() : FGsContentsLobbyBase(FGsContentsMode::InLobby::SERVER_SELCET)
{
}

FGsStateLobbyServerSelect::~FGsStateLobbyServerSelect()
{
}

void FGsStateLobbyServerSelect::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyServerSelect::Enter()"));

	if (UGsUIManager* uiManager = GUI())
	{
		if (const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData())
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(
				lobbyConstant->_pathWindowServerList.Id, lobbyConstant->_pathWindowServerList.Path);
			if (widget.IsValid())
			{
				_windowUI = Cast<UGsUIWindowServerSelect>(widget);
				_windowUI->OnClickServer.BindRaw(this, &FGsStateLobbyServerSelect::OnClickSelectServer);
				_windowUI->SetData();
			}		
		}
	}
}

void FGsStateLobbyServerSelect::Exit()
{
	if (_windowUI.IsValid())
	{
		_windowUI->Close();
	}

	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyServerSelect::Update(float In_deltaTime)
{
	FGsContentsLobbyBase::Update( In_deltaTime);
}

void FGsStateLobbyServerSelect::OnClickSelectServer(const WorldId inWorldID)
{
	FGsNetSendService::SendAuthReqSelectServer(inWorldID);
}
