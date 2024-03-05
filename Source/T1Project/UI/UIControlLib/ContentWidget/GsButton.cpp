// Fill out your copyright notice in the Description page of Project Settings.


#include "GsButton.h"
#include "Widgets/Input/SButton.h"
#include "SlateApplication.h"
#include "Components/ButtonSlot.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "T1Project.h"
#include "Public/TimerManager.h"
#include "UI/UIControlLib/Slate/SButtonEx.h"
#include "UI/UIContent/Tray/GsUITrayLongPressBar.h"

#define USE_TOUCH_EFFECT

const FString DefaultButtonPressedSound = TEXT("/Game/Sound/UI/Snd_UI_Click.Snd_UI_Click");

UGsButton::UGsButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsFocusable = false;
}

TSharedRef<SWidget> UGsButton::RebuildWidget()
{
	// Editor 에서 _useLongPress 체크 유무에 따라 변경
	// 월드맵개선: 몬스터 리스트 버튼 사이즈가 일반 아이콘보다 커서 어색하여 외부 프로그바 사용
	// https://jira.com2us.com/jira/browse/CHR-23756
	// 하나라도 쓰면 쓰는거
	bool isUseLongPress = _useLongPress || _useLongPressOtherProgressBar;
	IsFocusable = (true == isUseLongPress) ? false : IsFocusable;
	_longPressState = isUseLongPress;

	// 재정의 했으므로 부모 호출하지 말 것
	MyButton = SNew(SButtonEx)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClickedEx))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressedEx))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleasedEx))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHoveredEx)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhoveredEx)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	if (SButtonEx* buttonEx = static_cast<SButtonEx*>(MyButton.Get()))
	{
		buttonEx->OnMouseButtonDownEx.BindUObject(this, &UGsButton::OnMouseButtonDownEx);
		buttonEx->OnMouseButtonUpEx.BindUObject(this, &UGsButton::OnMouseButtonUpEx);
		buttonEx->OnMouseButtonMoveEx.BindUObject(this, &UGsButton::OnMouseButtonMoveEx);
		buttonEx->OnMouseFocusLostEx.BindUObject(this, &UGsButton::OnMouseFocusLeave);
		buttonEx->OnnMouseCaptureLostEx.BindUObject(this, &UGsButton::OnMouseFocusLeave);
		buttonEx->OnMouseDragLeave.BindUObject(this, &UGsButton::OnMouseFocusLeave);
	}
	_oriClickLockSecond = _clickLockSecond;

	// 리셋
	_lockEndTime = 0;
	_lastClickTime = 0;

	return MyButton.ToSharedRef();
}

void UGsButton::ReleaseSlateResources(bool bReleaseChildren)
{
	if (true == _onFinishedLongPressGauge.IsBound())
	{
		_onFinishedLongPressGauge.Clear();
	}
	_isPressedBtn = false;

	_clickLockSecond = _oriClickLockSecond;
	
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UGsButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	_isPressedBtn = false;

	// 첫 값을 백업해둔다. Disable 시 바뀐 값에서 복원해주기 위함.
#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		_backupColor = ColorAndOpacity;
		_backupBgColor = BackgroundColor;
		_bIsFirst = false;
	}
	else
	{
		if (_bIsFirst)
		{
			_backupColor = ColorAndOpacity;
			_backupBgColor = BackgroundColor;
			_bIsFirst = false;
		}
	}
#else
	if (_bIsFirst)
	{
		_backupColor = ColorAndOpacity;
		_backupBgColor = BackgroundColor;
		_bIsFirst = false;
	}
#endif
	// Disable시의 색상 적용을 위해 한 번 호출해준다.
	SetIsEnabled(bIsEnabled);
}

void UGsButton::SlateHandlePressedEx()
{
	UButton::SlateHandlePressed();

	// 터치 이펙트 출력
	if (_clickVFXPath.IsValid())
	{
#ifndef USE_TOUCH_EFFECT
		UClass* vfxClass = GPath()->GetClassByObjectPath(_clickVFXPath);
		if (vfxClass)
		{
			UGsBlueprintFunctionLibraryUI::AddVFXToPointerPos(vfxClass, _clickVFXZOrder);
		}
#endif
	}

	_isPressedBtn = true;

	if (EGsButtonSoundType::DEFAULT == _pressSoundType)
	{
		if (nullptr == WidgetStyle.PressedSlateSound.GetResourceObject())
		{
			if (false == _pressSound.IsValid())
			{
				UObject* soundObject = FSoftObjectPath(DefaultButtonPressedSound).TryLoad();
				if (soundObject)
				{
					_pressSound = Cast<USoundBase>(soundObject);

					UGameplayStatics::PlaySound2D(this, _pressSound.Get());
				}
			}
			else
			{
				UGameplayStatics::PlaySound2D(this, _pressSound.Get());
			}
		}
	}

	if (true == _longPressState)
	{
		if (UWorld* world = GetWorld())
		{
			// onClick 과 OnPress 의 차이를 두기위해 임시로 0.3 초 동안 Press 일때로 가정
			FTimerManager* timerMgr = &world->GetTimerManager();
			timerMgr->ClearTimer(_timerHandle);
			timerMgr->SetTimer(_timerHandle, this,
				&UGsButton::OnEndTimer, 0.3f, false);
		}
	}

	if (UWorld* world = GetWorld())
	{
		//FGeometry ViewportGeometry = UWidgetLayoutLibrary::GetViewportWidgetGeometry(world);
		if (APlayerController* controller = world->GetFirstPlayerController())
		{
			FVector2D location;
			controller->GetMousePosition(location.X, location.Y); //viewport 기준 마우스 포지션
		}
	}
}

FReply UGsButton::SlateHandleClickedEx()
{
	if (IsTimeLocked())
	{
		return FReply::Unhandled();
	}
	
	// 마지막 클릭시간, 락 타임 저장
	_lastClickTime = FApp::GetCurrentTime();
	_lockEndTime = _lastClickTime + _clickLockSecond;

	OnClicked.Broadcast();

	return FReply::Handled();
}

bool UGsButton::IsTimeLocked()
{
	if (0 == _lockEndTime)
	{
		return false;
	}

	// 현재 클릭한 시간이 마지막에 클릭한 시간보다 작을 수 없으므로 리셋한다
	double currTime = FApp::GetCurrentTime();
	if (currTime < _lastClickTime)
	{
		_lastClickTime = 0;
		_lockEndTime = 0;

		return false;
	}

	return (currTime <= _lockEndTime) ? true : false;
}

void UGsButton::SetIsEnabled(bool bInIsEnabled)
{
	SetIsEnabledGraphics(bInIsEnabled);

	Super::SetIsEnabled(bInIsEnabled);
}

void UGsButton::SetIsEnabledGraphics(bool bInIsEnabled)
{
	if (_bIsFirst) //property 갱신전에 SetIsEnabled 가 실행되면 _backupColor, _backupBgColor 가 바뀐다.
	{
		Super::SetIsEnabled(bInIsEnabled);
		return;
	}

	if (bInIsEnabled)
	{
		SetColorAndOpacity(_backupColor);
		SetBackgroundColor(_backupBgColor);
	}
	else
	{
		FLinearColor disableColor = FLinearColor(_disableBrightness, _disableBrightness, _disableBrightness, _disableAlpha);
		SetColorAndOpacity(disableColor);
		SetBackgroundColor(disableColor);
	}
}

void UGsButton::SlateHandleReleasedEx()
{
	UButton::SlateHandleReleased();

	LongPressedReleased();

	if (UWorld* world = GetWorld())
	{	// 2023/11/6 PKT - 이건 왜 있지?
		if (APlayerController* controller = world->GetFirstPlayerController())
		{
			FVector2D location;
			controller->GetMousePosition(location.X, location.Y);
		}
	}
}

void UGsButton::SlateHandleHoveredEx()
{
	UButton::SlateHandleHovered();
}

void UGsButton::SlateHandleUnhoveredEx()
{
	UButton::SlateHandleUnhovered();

	LongPressedReleased();
}

void UGsButton::OnMouseButtonDownEx(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnMouseButtonEventDownEx.ExecuteIfBound(MyGeometry, MouseEvent);
}

void UGsButton::OnMouseButtonUpEx(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnMouseButtonEventUpEx.ExecuteIfBound(MyGeometry, MouseEvent);
}

void UGsButton::OnMouseButtonMoveEx(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnMouseButtonEventMoveEx.ExecuteIfBound(MyGeometry, MouseEvent);
}

void UGsButton::OnMouseFocusLeave()
{
	LongPressedReleased();
}

void UGsButton::LongPressedReleased()
{
	if (_longPressState)
	{	// 2023/11/6 PKT - Long Press가 진행 중이 였다면 중지!!
		_isPressedBtn = false;

		if (_timerHandle.IsValid())
		{
			UWorld* world = GetWorld();
			if (nullptr != world)
			{
				world->GetTimerManager().ClearTimer(_timerHandle);
			}
		}

		if (_longPressObjPtr.IsValid())
		{
			if (UGsUITrayLongPressBar* widget = Cast<UGsUITrayLongPressBar>(_longPressObjPtr.Get()))
			{
				if (true == widget->_onFinishedLongPress.IsBoundToObject(this))
				{
					widget->_onFinishedLongPress.Clear();
				}
				UGsBlueprintFunctionLibraryUI::RemoveWidget(widget);
			}
			_longPressObjPtr.Reset();
		}
	}
}

void UGsButton::OnEndTimer()
{
	if (false == _isPressedBtn)
		return;

	if (nullptr != GetWorld())
	{
		// 기존 tray long press
		if(_useLongPressOtherProgressBar == false)
		{ 
			UGsUITrayLongPressBar* trayLongPress = UGsBlueprintFunctionLibraryUI::AddLongPressBar(this, _longPressGaugeBarZOrder);
			_longPressObjPtr = trayLongPress;
			if (trayLongPress)
			{
				if (false == trayLongPress->_onFinishedLongPress.IsBoundToObject(this))
				{
					trayLongPress->_onFinishedLongPress.AddUObject(this, &UGsButton::OnFinishedLongPressTrayMessage);
				}
			}
		}
		else
		{
			// 외부 프로그래스바 start
			if (_callbackOtherProgressBarStart)
			{
				_callbackOtherProgressBarStart([this](bool In_isFullGauge)
				{
					OnFinishedLongPressTrayMessage(In_isFullGauge);
				});
			}
		}
	}
	//GSLOG(Warning, TEXT("UGsButton::OnEndTimer"));
}

void UGsButton::OnFinishedLongPressTrayMessage(bool InIsFullGauge)
{
	if (true == InIsFullGauge)
	{
		_onFinishedLongPressGauge.Broadcast();
	}

	if (SButtonEx* buttonEx = static_cast<SButtonEx*>(MyButton.Get()))
	{
		buttonEx->OnFinishedLongPress();
	}

	//FSlateApplication::Get().ReleaseAllPointerCapture();
}

void UGsButton::OnChangeLongPress(bool InState)
{
	_longPressState = InState;
}
