
#include "GsUIHUDEventActionMessageProgressBarEX.h"

#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"

#include "../UTIL/GsTimeSyncUtil.h"

#include "T1Project.h"
#include "UMG/Public/Blueprint/WidgetBlueprintGeneratedClass.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"


void UGsUIHUDEventActionMessageProgressBarEX::NativeConstruct()
{
	Super::NativeConstruct();

	_beingTime = 0;
	_endTime = 0;

	SetTimerVisibility(false);
}

void UGsUIHUDEventActionMessageProgressBarEX::NativeDestruct()
{
	_beingTime = 0;
	_endTime = 0;

	SetTimerVisibility(false);

	Super::NativeDestruct();
}

void UGsUIHUDEventActionMessageProgressBarEX::SetHUDMode(EGsUIHUDMode InMode)
{
	// 2023/2/27 PKT - HUDMode 에서 핸들링 되어지는것이 아니라 서버에서 핸들링 되어야 한다.
	//Super::SetHUDMode(InMode);
	/**
	 * EventAction Dialog는 HUD Mode에 영향을 받지 않는다. 추후 변경 될 요지 있음.
	 */
}

bool UGsUIHUDEventActionMessageProgressBarEX::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	UpdateTime(InDeltaTime);

	return true;
}

void UGsUIHUDEventActionMessageProgressBarEX::SetTimerVisibility(bool InIsVisibility)
{
	ESlateVisibility visibility = (InIsVisibility) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;

	_panelProgressBar->SetVisibility(visibility);
}

bool UGsUIHUDEventActionMessageProgressBarEX::IsTimerOn() const
{
	return (ESlateVisibility::Collapsed != _panelProgressBar->GetVisibility());
}

void UGsUIHUDEventActionMessageProgressBarEX::UpdateTime(float InDeltaTime /* = 0.f */)
{
	if (false == IsTimerOn())
	{
		return;
	}

	int32 totalSecond = FTimespan(_endTime.GetTicks() - _beingTime.GetTicks()).GetTotalSeconds();
	int32 elapsedTimeS = FTimespan(FGsTimeSyncUtil::GetClientNowDateTime() - _beingTime).GetTotalSeconds();
	elapsedTimeS = FMath::Clamp<int32>(elapsedTimeS, 0, totalSecond);

	int32 minute = StaticCast<int32>(totalSecond - elapsedTimeS) / 60;
	int32 Second = StaticCast<int32>(totalSecond - elapsedTimeS) % 60;

	_timeText->SetText(FText::FromString(FString::Format(TEXT("{0} : {1}"), { minute,  Second })));

	const float progress = (0 < totalSecond) ? FMath::Clamp((1.0f - (StaticCast<float>(elapsedTimeS) / totalSecond)), 0.f, 1.f) : 0.f;

	_timeBar->SetPercent(progress);

	const static int32 PLAY_ANIMATION_TIME_S = 10;
	if (PLAY_ANIMATION_TIME_S >= (totalSecond - elapsedTimeS) && false == IsPlayAnimation())
	{
		PlayAnimation();
	}
}

void UGsUIHUDEventActionMessageProgressBarEX::PlayAnimation()
{
	UWidgetBlueprintGeneratedClass* genClass = GetWidgetTreeOwningClass();
	if (genClass && 0 < genClass->Animations.Num())
	{
		const static int32 FIRST_ANIMATION_DATA = 0;
		if (UWidgetAnimation* animationData = genClass->Animations[FIRST_ANIMATION_DATA])
		{
			const static int32 NUM_LOOPS_TO_PLAY = 0;
			UUserWidget::PlayAnimation(animationData, 0.f, NUM_LOOPS_TO_PLAY, EUMGSequencePlayMode::Type::Forward, 1.f);

			_timeText->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK));
		}
	}
}

void UGsUIHUDEventActionMessageProgressBarEX::Stop()
{
	this->StopAllAnimations();
	_timeText->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID));
}

bool UGsUIHUDEventActionMessageProgressBarEX::IsPlayAnimation() const
{
	return this->IsPlayingAnimation();
}

void UGsUIHUDEventActionMessageProgressBarEX::SetData(const FText& InMessage, int64 InElapsedTime, int64 InTotalTime)
{
	SetTimerVisibility(true);

	Stop();

	_message->SetText(InMessage);

	_beingTime = (FGsTimeSyncUtil::GetClientNowDateTime().GetTicks() - (InElapsedTime * ETimespan::TicksPerSecond));

	_endTime = _beingTime.GetTicks() + (InTotalTime * ETimespan::TicksPerSecond);

	UpdateTime();
}