// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateRanking.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsRankingManager.h"
#include "UI/UIContent/Window/Ranking/GsUIWindowRanking.h"


FGsGameStateRanking::FGsGameStateRanking() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsRanking)
{
}
FGsGameStateRanking::~FGsGameStateRanking()
{
}

void FGsGameStateRanking::Enter()
{
	FGsContentsGameBase::Enter();

	RegisterMessages();

	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> rankingWindow = uiManager->OpenAndGetWidget(TEXT("WindowRanking"));
		if (rankingWindow.IsValid())
		{
			_windowUI = Cast<UGsUIWindowRanking>(rankingWindow);
		}
	}

	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateRanking::Exit()
{
	RemoveMessage();

	if (FGsRankingManager* rankingManager = GSRanking())
	{
		rankingManager->ReleaseRankingData();
	}

	FGsContentsGameBase::Exit();
}

void FGsGameStateRanking::RemoveMessage()
{
	FGsMessageHolder* msg = GMessage();
	MRanking& msgRanking = msg->GetRanking();
	for (TPair<MessageContentRanking, FDelegateHandle>& rankingMsgHandler : _rankingMsgHandlerList)
	{
		msgRanking.Remove(rankingMsgHandler);
	}
	_rankingMsgHandlerList.Empty();
}

void FGsGameStateRanking::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	MRanking& msgRanking = msg->GetRanking();

	_rankingMsgHandlerList.Emplace(msgRanking.AddRaw(MessageContentRanking::INVALIDATE,
		this, &FGsGameStateRanking::OnUpdateRankingPage));
}

void FGsGameStateRanking::OnUpdateRankingPage(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI->OnUpdateRankingPage();
	}
}