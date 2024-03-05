// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateOption.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/GsMessageUI.h"

#include "UI/UIContent/Window/GsUIWindowOption.h"
#include "Input/GsInputEventMsgBase.h"


FGsGameStateOption::FGsGameStateOption() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsOption)
{
}

FGsGameStateOption::~FGsGameStateOption()
{
}

void FGsGameStateOption::Enter()
{
	FGsContentsGameBase::Enter();

	FGsMessageHolder* msg = GMessage();
	_delegatePressQuickExecute = msg->GetInput().AddRaw(MessageInput::QuickExecute, this, &FGsGameStateOption::OnPressQuickExecute);

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowOption"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowOption>(widget);
		}
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateOption::OnPressQuickExecute(const FGsInputEventMsgBase&)
{
	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowOption"));
		if (widget.IsValid())
			widget->Close();
	}
}

void FGsGameStateOption::Exit()
{
	/*if (_windowUI.IsValid())
	{
		_windowUI->Close();
	}*/

	_windowUI = nullptr;

	FGsMessageHolder* msg = GMessage();
	msg->GetInput().Remove(_delegatePressQuickExecute);

	FGsContentsGameBase::Exit();
}

void FGsGameStateOption::Update(float In_deltaTime)
{
	FGsContentsGameBase::Update(In_deltaTime);
}

void FGsGameStateOption::Init()
{
	FGsContentsGameBase::Init();
}

void FGsGameStateOption::Close()
{
	FGsContentsGameBase::Close();
}
