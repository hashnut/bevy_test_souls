// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateQuest.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"

#include "UI/UIContent/Window/GsUIWindowQuest.h"

#include "Message/GsMessageUI.h"



FGsGameStateQuest::FGsGameStateQuest() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsQuest)
{
}

FGsGameStateQuest::~FGsGameStateQuest()
{
}

void FGsGameStateQuest::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowQuest"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowQuest>(widget);
		}
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateQuest::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_windowUI = nullptr;

	FGsContentsGameBase::Exit();
}

void FGsGameStateQuest::Update(float In_deltaTime)
{
	FGsContentsGameBase::Update(In_deltaTime);
}

void FGsGameStateQuest::Init()
{
	FGsContentsGameBase::Init();
}

void FGsGameStateQuest::Close()
{
	FGsContentsGameBase::Close();
}
