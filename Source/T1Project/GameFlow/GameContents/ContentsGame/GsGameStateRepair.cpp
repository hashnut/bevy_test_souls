// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateRepair.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"


FGsGameStateRepair::FGsGameStateRepair() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsRepair)
{
}
FGsGameStateRepair::~FGsGameStateRepair()
{
}

void FGsGameStateRepair::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->Open(TEXT("WindowRepair"));
	}

	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateRepair::Exit()
{
	FGsContentsGameBase::Exit();
}