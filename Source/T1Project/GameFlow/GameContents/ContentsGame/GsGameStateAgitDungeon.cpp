// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateAgitDungeon.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Guild/GsGuildHelper.h"


FGsGameStateAgitDungeon::FGsGameStateAgitDungeon() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsAgitDungeon)
{

}

FGsGameStateAgitDungeon::~FGsGameStateAgitDungeon()
{

}

void FGsGameStateAgitDungeon::Enter()
{
	FGsContentsGameBase::Enter();

	GUI()->Open(TEXT("WindowAgitDungeon"));
	
	SetNavigationBarItemId(FGsGuildHelper::GetGuildCoinItemId());
}

void FGsGameStateAgitDungeon::Exit()
{
	if (FGsEventProgressManager* eventProgressMgr = GSEventProgress())
	{
		eventProgressMgr->OnReqFinishEvent(EGsEventProgressType::AGIT_DUNGEON);
	}

	FGsContentsGameBase::Exit();
}
