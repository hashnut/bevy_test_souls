// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIHUDTradeSend.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Management/ScopeGame/GsPersonalTradingManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/GsMessageHolder.h"


void UGsUIHUDTradeSend::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIHUDTradeSend::OnClickCancel);
}

void UGsUIHUDTradeSend::NativeConstruct()
{
	Super::NativeConstruct();

	_approveProgressTimeSec = GSPersonalTrading()->GetApproveProgressTimeSec();

	FGsMessageHolder* msg = GMessage();
	_msgHudHandlerList.Emplace(msg->GetContentsHud().AddUObject(MessageContentHud::INVALIDATE_PERSONAL_TRADING,
		this, &UGsUIHUDTradeSend::InvalidateUI));

	Hide();
}

void UGsUIHUDTradeSend::NativeDestruct()
{
	FGsMessageHolder* msg = GMessage();
	for (MsgHudHandle& handler : _msgHudHandlerList)
	{
		msg->GetContentsHud().Remove(handler);
	}
	_msgHudHandlerList.Empty();

	Super::NativeDestruct();
}

void UGsUIHUDTradeSend::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isOpened)
	{
		float remainTime = GCoolTime()->GetCoolTime(EGsCoolTime::PersonalTrading, 0, 0);
		if (0 > remainTime)
		{
			remainTime = 0.f;
			
			if (FGsPersonalTradingManager* tradeManager = GSPersonalTrading())
			{
				if (tradeManager->TryReqPersonalTradingCancel())
				{
					tradeManager->SetCancelType(PersonalTradingCancelType::TIME_OVER);
				}
				Hide();
			}
		}
		float spendTime = _approveProgressTimeSec - remainTime;
		_progressBarSend->SetPercent(spendTime / (float)_approveProgressTimeSec);
	}
}

void UGsUIHUDTradeSend::SetTradeInfo(const FString& InUserName)
{
	_textBlockUserName->SetText(FText::FromString(InUserName));

	float remainTime = GCoolTime()->GetCoolTime(EGsCoolTime::PersonalTrading, 0, 0);
	float spendTime = _approveProgressTimeSec - remainTime;
	_progressBarSend->SetPercent(spendTime / (float)_approveProgressTimeSec);
}

void UGsUIHUDTradeSend::Show()
{
	_isOpened = true;
	SetHUDMode(GUI()->GetHUDMode());
}

void UGsUIHUDTradeSend::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);

	_isOpened = false;

	_textBlockUserName->SetText(FText::GetEmpty());
	_progressBarSend->SetPercent(0.f);
}

void UGsUIHUDTradeSend::SetHUDMode(EGsUIHUDMode InMode)
{
	if (IsHideMode(InMode) || false == _isOpened)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIHUDTradeSend::OnClickCancel()
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (_isOpened)
	{
		if (tradeManager->TryReqPersonalTradingCancel())
		{
			tradeManager->SetCancelType(PersonalTradingCancelType::CLICK);
		}
		Hide();
	}
}

void UGsUIHUDTradeSend::InvalidateUI()
{
	FGsPersonalTradingManager* tradingManager = GSPersonalTrading();
	if (nullptr == tradingManager)
		return;

	if (tradingManager->IsTradeFromMe() &&
		tradingManager->GetLocalPersonalTradingState() == PersonalTradingState::WAIT)
	{
		Show();
		SetTradeInfo(tradingManager->GetRemoteUserName());
	}
	else
	{
		Hide();
	}
}
