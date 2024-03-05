// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateKeyBoardMapping.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/GsMessageUI.h"

#include "UI/UIContent/Window/GsUIWindowKeyBoardMapping.h"
#include "Input/GsInputEventMsgBase.h"


FGsGameStateKeyBoardMapping::FGsGameStateKeyBoardMapping() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsKeyBoardMapping)
{
}

FGsGameStateKeyBoardMapping::~FGsGameStateKeyBoardMapping()
{
}

void FGsGameStateKeyBoardMapping::Enter()
{
	FGsContentsGameBase::Enter();

	FGsMessageHolder* msg = GMessage();
	_delegatePressQuickExecute = msg->GetInput().AddRaw(MessageInput::QuickExecute, this, &FGsGameStateKeyBoardMapping::OnPressQuickExecute);

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowKeyBoardMapping"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowKeyBoardMapping>(widget);
		}
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	CloseNavigationBar();
}

void FGsGameStateKeyBoardMapping::OnPressQuickExecute(const FGsInputEventMsgBase&)
{
	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowKeyBoardMapping"));
		if (widget.IsValid())
			widget->Close();
	}
}

void FGsGameStateKeyBoardMapping::Exit()
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

void FGsGameStateKeyBoardMapping::Update(float In_deltaTime)
{
	FGsContentsGameBase::Update(In_deltaTime);
}

void FGsGameStateKeyBoardMapping::Init()
{
	FGsContentsGameBase::Init();
}

void FGsGameStateKeyBoardMapping::Close()
{
	FGsContentsGameBase::Close();
}
