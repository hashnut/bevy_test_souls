// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyIntro.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsLobbyConstant.h"

#include "UI/UIContent/Window/GsUIWindowIntro.h"


FGsStateLobbyIntro::FGsStateLobbyIntro() : FGsContentsLobbyBase(FGsContentsMode::InLobby::INTRO)
{
}

FGsStateLobbyIntro::~FGsStateLobbyIntro()
{
}

void FGsStateLobbyIntro::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyIntro::Enter()"));

	if (UGsUIManager* uiManager = GUI())
	{
		if (const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData())
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(
				lobbyConstant->_pathWindowIntro.Id, lobbyConstant->_pathWindowIntro.Path);
		}
	}
}

void FGsStateLobbyIntro::Exit()
{
	// UGsUIWindowIntro 위젯에서 스스로 닫으므로 닫아줄 필요 없음

	FGsContentsLobbyBase::Exit();
}