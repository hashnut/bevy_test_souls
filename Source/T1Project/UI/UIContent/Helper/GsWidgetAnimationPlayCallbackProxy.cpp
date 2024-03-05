// copy from Engine\Source\Runtime\UMG\Private\Animation\WidgetAnimationPlayCallbackProxy.cpp
#include "GsWidgetAnimationPlayCallbackProxy.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/UMG/Public/Animation/UMGSequencePlayer.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "T1Project.h"

/************************************************************************/
/* UGsWidgetAnimationPlayCallbackProxy                                  */
/************************************************************************/

UGsWidgetAnimationPlayCallbackProxy* UGsWidgetAnimationPlayCallbackProxy::CreatePlayAnimationProxyObject(class UUMGSequencePlayer*& Result, class UUserWidget* Widget, UWidgetAnimation* InAnimation, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed)
{
	UGsWidgetAnimationPlayCallbackProxy* Proxy = NewObject<UGsWidgetAnimationPlayCallbackProxy>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	Result = Proxy->ExecutePlayAnimation(Widget, InAnimation, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
	return Proxy;
}

UGsWidgetAnimationPlayCallbackProxy* UGsWidgetAnimationPlayCallbackProxy::CreatePlayAnimationTimeRangeProxyObject(class UUMGSequencePlayer*& Result, class UUserWidget* Widget, UWidgetAnimation* InAnimation, float StartAtTime, float EndAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed)
{
	UGsWidgetAnimationPlayCallbackProxy* Proxy = NewObject<UGsWidgetAnimationPlayCallbackProxy>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	Result = Proxy->ExecutePlayAnimationTimeRange(Widget, InAnimation, StartAtTime, EndAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
	return Proxy;
}

class UUMGSequencePlayer* UGsWidgetAnimationPlayCallbackProxy::ExecutePlayAnimation(class UUserWidget* Widget, UWidgetAnimation* InAnimation, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed)
{
	if (!Widget)
	{
		return nullptr;
	}

	WorldPtr = Widget->GetWorld();

	UUMGSequencePlayer* Player = Widget->PlayAnimation(InAnimation, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
	if (Player)
	{
		Player->OnSequenceFinishedPlaying().AddUObject(this, &UGsWidgetAnimationPlayCallbackProxy::OnFinished);
	}

	return Player;
}

class UUMGSequencePlayer* UGsWidgetAnimationPlayCallbackProxy::ExecutePlayAnimationTimeRange(class UUserWidget* Widget, UWidgetAnimation* InAnimation, float StartAtTime, float EndAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed)
{
	if (!Widget)
	{
		return nullptr;
	}

	WorldPtr = Widget->GetWorld();

	UUMGSequencePlayer* Player = Widget->PlayAnimationTimeRange(InAnimation, StartAtTime, EndAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
	if (Player)
	{
		OnFinishedHandle = Player->OnSequenceFinishedPlaying().AddUObject(this, &UGsWidgetAnimationPlayCallbackProxy::OnFinished);
	}

	return Player;
}

void UGsWidgetAnimationPlayCallbackProxy::OnFinished(class UUMGSequencePlayer& Player)
{
	Player.OnSequenceFinishedPlaying().Remove(OnFinishedHandle);

	// We delay the Finish trigger to next frame.
	if (UWorld* World = WorldPtr.Get())
	{
		// Use a dummy timer handle as we don't need to store it for later but we don't need to look for something to clear
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &UGsWidgetAnimationPlayCallbackProxy::OnFinished_Delayed, 0.001f, false);
	}
}

void UGsWidgetAnimationPlayCallbackProxy::OnFinished_Delayed()
{
	Finished.Broadcast();
}


/************************************************************************/
/* UGsUISequencePlayer                                                  */
/************************************************************************/

void UGsUISequencePlayer::Play(UUserWidget* Widget, UWidgetAnimation* InAnimation, FGsUISequencePlayerFinished InOnFinished)
{
	GSLOG(Log, TEXT("Play()"));

	Reset();

	_onFinished = MoveTemp(InOnFinished);
	_animation = InAnimation;

	if (nullptr == Widget || nullptr == InAnimation)
	{
		GSLOG(Error, TEXT("nullptr == Widget || nullptr == InAnimation"));
		OnFinished();
		return;
	}

	UUMGSequencePlayer* UMGSequencePlayer = nullptr;
	_widgetAnimationPlayCallbackProxy = UGsWidgetAnimationPlayCallbackProxy::CreatePlayAnimationProxyObject(UMGSequencePlayer, Widget, _animation.Get());
	_umgSequencePlayer = MakeWeakObjectPtr(UMGSequencePlayer);

	if (!_widgetAnimationPlayCallbackProxy.IsValid())
	{
		GSLOG(Error, TEXT("nullptr == _widgetAnimationPlayCallbackProxy"));
		OnFinished();
	}
	else
	{
		_widgetAnimationPlayCallbackProxy->Finished.AddUniqueDynamic(this, &UGsUISequencePlayer::OnFinished);
	}
}

void UGsUISequencePlayer::Reset()
{
	GSLOG(Log, TEXT("Reset()"));

	// 기존 애니가 실행중이었다면, 스탑 및 이벤트 처리 등
	if (IsPlaying())
	{
		(*_umgSequencePlayer).Stop();
	}

	OnFinished();
}

bool UGsUISequencePlayer::IsPlaying() const
{
	if (!_umgSequencePlayer.IsValid())
	{
		return false;
	}

	return _umgSequencePlayer->IsPlayingForward();
}

void UGsUISequencePlayer::OnFinished()
{
	GSLOG(Log, TEXT("OnFinished(), _onFinished.IsBound():%d"), _onFinished.IsBound());

	if (_widgetAnimationPlayCallbackProxy.IsValid())
	{
		_widgetAnimationPlayCallbackProxy.Reset();
	}

	UWidgetAnimation* Animation = nullptr;
	if (_animation.IsValid())
	{
		Animation = _animation.Get();
		_animation.Reset();
	}

	_umgSequencePlayer.Reset();

	if (_onFinished.IsBound())
	{
		// 단 1회 콜백만 허용.
		_onFinished.Execute(Animation);
		_onFinished.Unbind();
	}
}
