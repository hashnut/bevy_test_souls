// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayBlockUI.h"
#include "Components/Border.h"
#include "Management/ScopeGlobal/GsUIManager.h"


void UGsUITrayBlockUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_borderBg->OnMouseButtonUpEvent.BindDynamic(this, &UGsUITrayBlockUI::OnBorderMouseButtonUp);
}

void UGsUITrayBlockUI::NativeConstruct()
{
	Super::NativeConstruct();

	SetTextDefault();
}

void UGsUITrayBlockUI::NativeDestruct()
{
	_showRemainTime = 0;
	_bHideUI = false;

	Super::NativeDestruct();
}

void UGsUITrayBlockUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0 < _showRemainTime)
	{
		_showRemainTime -= InDeltaTime;
		if (0 > _showRemainTime)
		{
			_showRemainTime = 0;

			SetRenderOpacity(_bHideUI ? 0.f : 1.f);
		}
	}
}

FReply UGsUITrayBlockUI::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITrayBlockUI::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITrayBlockUI::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

void UGsUITrayBlockUI::Show()
{
	// 이미 켜져있는 경우엔 시간 세팅 안함
	if (false == IsVisible())
	{
		_showRemainTime = SHOW_DELAY_TIME;
		SetRenderOpacity(0.f);
	}

	// 새로 들어올 경우 기존 세팅을 날린다
	SetHideUI(false);
	SetTextDefault();

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 주의: 컨트롤러에서 켠 상태에서 스크린샷모드 진입 후 컨트롤러에서 껐을 경우, 스크린샷모드를 나가며 다시 켜는 문제가 있음
	// 최종 상태에 맞춰 복구하도록 backup값도 세팅함. UGsUIWidgetBase::SetHide 로직 참고.
	if (_bIsHide)
	{
		_backupVisibility = ESlateVisibility::SelfHitTestInvisible;
	}
}

void UGsUITrayBlockUI::Hide()
{
	_showRemainTime = 0;
	_bHideUI = false;

	SetVisibility(ESlateVisibility::Collapsed);

	// 주의: 컨트롤러에서 켠 상태에서 스크린샷모드 진입 후 컨트롤러에서 껐을 경우, 스크린샷모드를 나가며 다시 켜는 문제가 있음
	// 최종 상태에 맞춰 복구하도록 backup값도 세팅함. UGsUIWidgetBase::SetHide 로직 참고.
	if (_bIsHide)
	{
		_backupVisibility = ESlateVisibility::Collapsed;
	}
}

void UGsUITrayBlockUI::SetTextMessage(const FText& InText)
{
	_contentText = InText;
}

void UGsUITrayBlockUI::SetHideUI(bool bInHideUI)
{
	_bHideUI = bInHideUI;

	if (_bHideUI)
	{
		SetRenderOpacity(0.f);
	}
}

void UGsUITrayBlockUI::SetTextDefault()
{
	// TEXT: 서버에 요청중입니다
	FText textDefault;
	FText::FindText(TEXT("LauncherText"), TEXT("Requesting"), textDefault);

	SetTextMessage(textDefault);
}

FEventReply UGsUITrayBlockUI::OnBorderMouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	// 뭐가 막고있는지 보기 위함
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsBlockUIController* blockController = uiMgr->GetBlockUIController())
		{
			blockController->PrintLog();
		}
	}

	return FEventReply(false);
}
