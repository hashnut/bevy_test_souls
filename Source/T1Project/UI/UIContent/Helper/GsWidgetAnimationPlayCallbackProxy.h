// copy from Engine\Source\Runtime\UMG\Public\Animation\WidgetAnimationPlayCallbackProxy.h
#pragma once

#include "Runtime/UMG/Public/Animation/WidgetAnimationPlayCallbackProxy.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GsWidgetAnimationPlayCallbackProxy.generated.h"

class UUMGSequencePlayer;
class UWidgetAnimation;
class UWorld;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetAnimationResult);

UCLASS(MinimalAPI)
class UGsWidgetAnimationPlayCallbackProxy : public UObject
{
	GENERATED_BODY()

public:
	// Called when animation has been completed
	UPROPERTY(BlueprintAssignable)
	FWidgetAnimationResult Finished;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DisplayName = "Play Animation with Finished event", ShortToolTip = "Play Animation and trigger event on Finished",  ToolTip="Play Animation on widget and trigger Finish event when the animation is done."), Category = "User Interface|Animation")
	static UGsWidgetAnimationPlayCallbackProxy* CreatePlayAnimationProxyObject(class UUMGSequencePlayer*& Result, class UUserWidget* Widget, class UWidgetAnimation* InAnimation, float StartAtTime = 0.0f, int32 NumLoopsToPlay = 1, EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward, float PlaybackSpeed = 1.0f);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DisplayName = "Play Animation Time Range with Finished event", ShortToolTip = "Play Animation Time Range and trigger event on Finished", ToolTip = "Play Animation Time Range on widget and trigger Finish event when the animation is done."), Category = "User Interface|Animation")
	static UGsWidgetAnimationPlayCallbackProxy* CreatePlayAnimationTimeRangeProxyObject(class UUMGSequencePlayer*& Result, class UUserWidget* Widget, class UWidgetAnimation* InAnimation, float StartAtTime = 0.0f, float EndAtTime = 0.0f, int32 NumLoopsToPlay = 1, EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward, float PlaybackSpeed = 1.0f);

private:
	class UUMGSequencePlayer* ExecutePlayAnimation(class UUserWidget* Widget, class UWidgetAnimation* InAnimation, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed);
	class UUMGSequencePlayer* ExecutePlayAnimationTimeRange(class UUserWidget* Widget, class UWidgetAnimation* InAnimation, float StartAtTime, float EndAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed);
	void OnFinished(class UUMGSequencePlayer& Player);
	void OnFinished_Delayed();

	// Pointer to the world, needed to delay the results slightly
	TWeakObjectPtr<UWorld> WorldPtr;

	FDelegateHandle OnFinishedHandle;
};

DECLARE_DELEGATE_OneParam(FGsUISequencePlayerFinished, const UWidgetAnimation*)

/**
 * UGsWidgetAnimationPlayCallbackProxy 를 이용한 시퀀스 플레이어
 */
UCLASS()
class UGsUISequencePlayer final : public UObject
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<UGsWidgetAnimationPlayCallbackProxy> _widgetAnimationPlayCallbackProxy;

	TWeakObjectPtr<UWidgetAnimation> _animation;

	TWeakObjectPtr<UUMGSequencePlayer> _umgSequencePlayer;

	FGsUISequencePlayerFinished _onFinished;

public:
	void Play(UUserWidget* Widget, UWidgetAnimation* InAnimation, FGsUISequencePlayerFinished InOnFinished);
	void Reset();
	bool IsPlaying() const;

protected:
	UFUNCTION()
	void OnFinished();
};