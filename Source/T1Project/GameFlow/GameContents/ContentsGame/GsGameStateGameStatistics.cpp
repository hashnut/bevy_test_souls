// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateGameStatistics.h"
#include "Management/ScopeGlobal/GsUIManager.h"


FGsGameStateGameStatistics::FGsGameStateGameStatistics() : 
	FGsContentsGameBase(FGsContentsMode::InGame::ContentsGameStatistics)
{
}

FGsGameStateGameStatistics::~FGsGameStateGameStatistics()
{
}

void FGsGameStateGameStatistics::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->Open(TEXT("WindowGameStatistics"));
	}
}

void FGsGameStateGameStatistics::Exit()
{
	FGsContentsGameBase::Exit();
}
