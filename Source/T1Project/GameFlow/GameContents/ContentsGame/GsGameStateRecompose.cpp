// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateRecompose.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"


FGsGameStateRecompose::FGsGameStateRecompose() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsRecompose)
{
}
FGsGameStateRecompose::~FGsGameStateRecompose()
{
}

void FGsGameStateRecompose::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->Open(TEXT("WindowRecompose"));
	}

	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateRecompose::Exit()
{
	FGsContentsGameBase::Exit();
}