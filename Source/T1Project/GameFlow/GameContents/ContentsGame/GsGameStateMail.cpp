// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateMail.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsMailManager.h"

#include "UI/UIContent/Window/GsUIWindowMail.h"
#include "UI/UIContent/Window/GsUIWindowMailDetail.h"

#include "Message/MessageParam/GsUIMessageParam.h"


FGsGameStateMail::FGsGameStateMail() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsMail)
{
}

FGsGameStateMail::~FGsGameStateMail()
{
}

void FGsGameStateMail::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowMail"));
		if (widget.IsValid())
		{
			_windowMailUI = Cast<UGsUIWindowMail>(widget);
			_windowMailUI->InIt();
		}
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);

	// 메세지 등록
	RegisterMessages();
	// 서버로 목록 요청
	GSMail()->SendReqMailList();
}

void FGsGameStateMail::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_windowMailUI = nullptr;
	// 메세지 해지
	UnregisterMessages();

	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MAIL, false);

	FGsContentsGameBase::Exit();
}

void FGsGameStateMail::Update(float In_deltaTime)
{
	FGsContentsGameBase::Update(In_deltaTime);
}

void FGsGameStateMail::Init()
{
	FGsContentsGameBase::Init();
}

void FGsGameStateMail::Close()
{
	FGsContentsGameBase::Close();
}

void FGsGameStateMail::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	auto& mailHandler = msg->GetMail();

	_listMailDelegate.Emplace(mailHandler.AddRaw(MessageContentMail::ACTIVE_WINDOW_MAILDETAIL,
		this, &FGsGameStateMail::OpenWindowMailDetail));

	_listMailDelegate.Emplace(mailHandler.AddRaw(MessageContentMail::ACTIVE_WINDOW_INVALIDTEALL,
		this, &FGsGameStateMail::WindowUIInvalidateAll));
}

void FGsGameStateMail::UnregisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	auto& mailHandler = msg->GetMail();

	for (auto handler : _listMailDelegate)
	{
		mailHandler.Remove(handler);
	}
	_listMailDelegate.Empty();
}

void FGsGameStateMail::WindowUIInvalidateAll(const IGsMessageParam* InParam)
{
	if (_windowMailUI.IsValid())
	{
		_windowMailUI->InvalidateAll();
	}
}

void FGsGameStateMail::OpenWindowMailDetail(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamMail* param = InParam->Cast<const FGsUIMsgParamMail>();

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowMailDetail"));
		if (widget.IsValid())
		{
			_windowMailDetailUI = Cast<UGsUIWindowMailDetail>(widget);
			_windowMailDetailUI->SetData(param->_mailDBId);
		}
	}
}