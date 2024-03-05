// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDTradeReceive.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Management/ScopeGame/GsPersonalTradingManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/GsMessageHolder.h"


void UGsUIHUDTradeReceive::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAccept->OnClicked.AddDynamic(this, &UGsUIHUDTradeReceive::OnClickAccept);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIHUDTradeReceive::OnClickCancel);
}

void UGsUIHUDTradeReceive::NativeConstruct()
{
	Super::NativeConstruct();

	_approveProgressTimeSec = GSPersonalTrading()->GetApproveProgressTimeSec();

	FGsMessageHolder* msg = GMessage();
	_msgHudHandlerList.Emplace(msg->GetContentsHud().AddUObject(MessageContentHud::INVALIDATE_PERSONAL_TRADING,
		this, &UGsUIHUDTradeReceive::InvalidateUI));

	Hide();
}

void UGsUIHUDTradeReceive::NativeDestruct()
{
	FGsMessageHolder* msg = GMessage();
	for (MsgHudHandle& handler : _msgHudHandlerList)
	{
		msg->GetContentsHud().Remove(handler);
	}
	_msgHudHandlerList.Empty();

	Super::NativeDestruct();
}

void UGsUIHUDTradeReceive::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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
		_progressBarReceive->SetPercent(spendTime / (float)_approveProgressTimeSec);
	}
}

void UGsUIHUDTradeReceive::SetTradeInfo(const FString& InUserName)
{
	_textBlockUserName->SetText(FText::FromString(InUserName));

	float remainTime = GCoolTime()->GetCoolTime(EGsCoolTime::PersonalTrading, 0, 0);
	float spendTime = _approveProgressTimeSec - remainTime;
	_progressBarReceive->SetPercent(spendTime / (float)_approveProgressTimeSec);
}

void UGsUIHUDTradeReceive::Show()
{
	_isOpened = true;
	SetHUDMode(GUI()->GetHUDMode());
}

void UGsUIHUDTradeReceive::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);

	_isOpened = false;

	_textBlockUserName->SetText(FText::GetEmpty());
	_progressBarReceive->SetPercent(0.f);
}

void UGsUIHUDTradeReceive::SetHUDMode(EGsUIHUDMode InMode)
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

void UGsUIHUDTradeReceive::OnClickAccept()
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (_isOpened)
	{
		tradeManager->TryReqPersonalTradingAccept();
		//Hide();
	}
}

void UGsUIHUDTradeReceive::OnClickCancel()
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

void UGsUIHUDTradeReceive::InvalidateUI()
{
	FGsPersonalTradingManager* tradingManager = GSPersonalTrading();
	if (nullptr == tradingManager)
		return;

	if (false == tradingManager->IsTradeFromMe() &&
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
