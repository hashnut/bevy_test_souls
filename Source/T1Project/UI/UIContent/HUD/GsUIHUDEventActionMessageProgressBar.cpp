
#include "GsUIHUDEventActionMessageProgressBar.h"

#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"

#include "../UTIL/GsTimeSyncUtil.h"

#include "T1Project.h"


void UGsUIHUDEventActionMessageProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	_beingTime = 0;
	_endTime = 0;
	_message = FText::GetEmpty();
	SetTimerVisibility(false);
}

void UGsUIHUDEventActionMessageProgressBar::NativeDestruct()
{
	_beingTime = 0;
	_endTime = 0;
	_message = FText::GetEmpty();
	SetTimerVisibility(false);

	Super::NativeDestruct();
}

void UGsUIHUDEventActionMessageProgressBar::SetHUDMode(EGsUIHUDMode InMode)
{
	// 2023/2/27 PKT - HUDMode 에서 핸들링 되어지는것이 아니라 서버에서 핸들링 되어야 한다.
	//Super::SetHUDMode(InMode);
	/**
	 * EventAction Dialog는 HUD Mode에 영향을 받지 않는다. 추후 변경 될 요지 있음.
	 */
}

bool UGsUIHUDEventActionMessageProgressBar::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	UpdateTime(InDeltaTime);

	return true;
}

void UGsUIHUDEventActionMessageProgressBar::SetTimerVisibility(bool InIsVisibility)
{
	ESlateVisibility visibility = (InIsVisibility) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	
	_panelProgressBar->SetVisibility(visibility);
}

bool UGsUIHUDEventActionMessageProgressBar::IsTimerOn() const
{
	return (ESlateVisibility::Collapsed != _panelProgressBar->GetVisibility());
}

void UGsUIHUDEventActionMessageProgressBar::UpdateTime(float InDeltaTime /* = 0.f */)
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

	if (_message.IsEmpty())
	{
		_timeText->SetText(FText::FromString(FString::Format(TEXT("{0} : {1}"), { minute,  Second })));
	}
	else
	{
		_timeText->SetText(FText::FromString(FString::Format(TEXT("{0} {1} : {2}"), { *_message.ToString(), minute, Second })));
	}
		
	const float progress = (0 < totalSecond) ? FMath::Clamp((1.0f - (StaticCast<float>(elapsedTimeS) / totalSecond)), 0.f, 1.f) : 0.f;

	_timeBar->SetPercent(progress);
}

void UGsUIHUDEventActionMessageProgressBar::SetData(const FText& InMessage, int64 InElapsedTime, int64 InTotalTime)
{
	SetTimerVisibility(true);

	_message = InMessage;

	_beingTime = (FGsTimeSyncUtil::GetClientNowDateTime().GetTicks() - (InElapsedTime * ETimespan::TicksPerSecond));

	_endTime = _beingTime.GetTicks() + (InTotalTime * ETimespan::TicksPerSecond);

	UpdateTime();
}