// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateSiegeWarfare.h"
#include "Management/ScopeGlobal/GsUIManager.h"


FGsGameStateSiegeWarfare::FGsGameStateSiegeWarfare() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsSiegeWarfare)
{
}

FGsGameStateSiegeWarfare::~FGsGameStateSiegeWarfare()
{
}

void FGsGameStateSiegeWarfare::Enter()
{
	FGsContentsGameBase::Enter();

	UGsUIManager* uiMgr = GUI();
	if (false == uiMgr->IsActiveWidget(TEXT("WindowAccumulateTax")))
	{
		uiMgr->Open(TEXT("WindowAccumulateTax"));
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤)
	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateSiegeWarfare::Exit()
{

	FGsContentsGameBase::Exit();
}
