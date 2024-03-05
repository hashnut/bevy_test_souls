// Fill out your copyright notice in the Description page of Project Settings.
#include "GsGameStateInvade.h"

#include "../GsContentsMode.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/GsMessageUI.h"
#include "Message/GsMessageInvasion.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "../UTIL/GsTimeSyncUtil.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"


FGsGameStateInvade::FGsGameStateInvade() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsPKBook)
{
}

FGsGameStateInvade::~FGsGameStateInvade()
{
}

void FGsGameStateInvade::RegisterMessages()
{
	MInvasion& MessageUiHolder = GMessage()->GetInvasion();

	_invadeDelegateMessage.Emplace(MessageUiHolder.AddRaw(MessageInvasion::INVALID_INVASION_USER_COUNT, this, &FGsGameStateInvade::InvalidateBattleBadge));

	_invadeDelegateMessage.Emplace(MessageUiHolder.AddRaw(MessageInvasion::INVALID_INVASION_ACTIVEPORTAL,
		this, &FGsGameStateInvade::InvasionPortalSpawnInfo));
}

void FGsGameStateInvade::UnregisterMessages()
{
	MInvasion& MessageUiHolder = GMessage()->GetInvasion();
	for (MsgInvasionHandle& msgHandler : _invadeDelegateMessage)
	{
		MessageUiHolder.Remove(msgHandler);
	}
	_invadeDelegateMessage.Empty();
}

void FGsGameStateInvade::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowInvade"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowInvade>(widget);
		}
	}

	// 2022/09/21 PKT - Message 등록
	RegisterMessages();

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateInvade::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_windowUI = nullptr;

	// 2022/09/21 PKT - Message 해지
	UnregisterMessages();

	FGsContentsGameBase::Exit();
}

void FGsGameStateInvade::InvalidateBattleBadge(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidateBattleBadge();
	}
}

void FGsGameStateInvade::InvasionPortalSpawnInfo(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvasionPortalSpawnInfo();
	}
}