// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDPenalty.h"
#include "Button.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"


#include "Message/MessageParam/GsMessageParam.h"
#include "Widget.h"
#include "TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "RecoveryPenalty/GsRecoveryPenaltyFunc.h"
#include "T1Project/UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"

const FName POPUP_PENALTY = TEXT("PopupRecoveryPenalty");

void UGsUIHUDPenalty::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_penaltyButton->OnClicked.AddDynamic(this, &UGsUIHUDPenalty::OnClickShowPopupPenalty);
}

void UGsUIHUDPenalty::NativeConstruct()
{
	Super::NativeConstruct();

	_msgUIHandle = GMessage()->GetUI().AddUObject(MessageUI::INVALID_PENALTY, this, &UGsUIHUDPenalty::OnChangePenaltyCount);
	InvalidPenaltyCount();
}

void UGsUIHUDPenalty::NativeDestruct()
{
	GMessage()->GetUI().Remove(_msgUIHandle);

	Super::NativeDestruct();
}

void UGsUIHUDPenalty::OnClickShowPopupPenalty()
{
	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_RECOVERY_PENALTY, this);
}	

void UGsUIHUDPenalty::SetHUDMode(EGsUIHUDMode InMode)
{	
	_hudMode = InMode;
	InvalidateAllInternal();
}

void UGsUIHUDPenalty::OnChangePenaltyCount(const IGsMessageParam* InParam)
{
	InvalidPenaltyCount();
}

void UGsUIHUDPenalty::InvalidPenaltyCount()
{
	int32 totalCount = FGsRecoveryPenaltyFunc::GetRecoveryPenaltyCount();
	bool isActive = totalCount > 0;
	_penaltyCountRedDot->SetRedDot(isActive, EGsIconRedDotType::NUMBER, totalCount);
}

void UGsUIHUDPenalty::InvalidateAllInternal()
{
	if (IsHideMode(_hudMode))
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		TArray<DeathPenaltyExp> expPenaltyList = GGameData()->GetUserData()->expPenalty;
		TArray<DeathPenaltyItem> itemPenaltyList = GGameData()->GetUserData()->itemPenalty;

		if (expPenaltyList.Num() > 0 || itemPenaltyList.Num() > 0)
		{
			_penaltyButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			_penaltyButton->SetVisibility(ESlateVisibility::Collapsed);		
			GUI()->CloseByKeyName(POPUP_PENALTY);
		}
	}
}
// 이벤트 시작
void UGsUIHUDPenalty::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	if (false == GUI()->IsActiveWidget(POPUP_PENALTY))
	{
		GUI()->Open(POPUP_PENALTY);
	}
}

// 이벤트 종료
void UGsUIHUDPenalty::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	GUI()->CloseByKeyName(POPUP_PENALTY);
}