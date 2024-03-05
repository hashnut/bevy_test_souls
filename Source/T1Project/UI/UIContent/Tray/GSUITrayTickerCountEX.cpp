#include "GSUITrayTickerCountEX.h"
#include "../UTIL/GsTimeSyncUtil.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Blueprint/WidgetBlueprintGeneratedClass.h"
#include "TimerManager.h"

void UGSUITrayTickerCountEX::NativeConstruct()
{
	Super::NativeConstruct();

	SetHideVisibility();

	ClearTimer();
}

void UGSUITrayTickerCountEX::NativeDestruct()
{
	ClearTimer();

	Super::NativeDestruct();
}

void UGSUITrayTickerCountEX::PlayCount(int32 InCount)
{
	_textBlockTime->SetText(FText::AsNumber(InCount));

	UWidgetBlueprintGeneratedClass* genClass = this->GetWidgetTreeOwningClass();
	if (genClass && 0 < genClass->Animations.Num())
	{
		const static int32 FIRST_ANIMATION_DATA = 0;
		if (UWidgetAnimation* animationData = genClass->Animations[FIRST_ANIMATION_DATA])
		{
			RestoreShowVisibility();

			const static int32 NUM_LOOPS_TO_PLAY = 1;
			this->PlayAnimation(animationData, 0.f, NUM_LOOPS_TO_PLAY, EUMGSequencePlayMode::Type::Forward, 1.f);
		}
	}

	if (0 >= InCount)
	{
		ClearTimer();
	}
}

void UGSUITrayTickerCountEX::ClearTimer()
{
	_timerClearTime = 0;

	if (_timerHandler.IsValid())
	{
		if (UWorld* world = GetWorld())
		{
			world->GetTimerManager().ClearTimer(_timerHandler);
		}
	}	
}

void UGSUITrayTickerCountEX::SetCountData(int32 InCount, time_t InEndTime, TFunction<void()> InCallback /* = nullptr */)
{
	Super::SetData(InCallback);

	SetHideVisibility();

	if (UWorld* world = GetWorld())
	{
		ClearTimer();

		auto CallBackFunc = [this]()
		{
			time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
			this->PlayCount(_timerClearTime - currentTime);
		};

		_timerClearTime = InEndTime;

		time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
		time_t countBeginTime = (_timerClearTime - InCount);

		float delay = (countBeginTime > currentTime) ? float(countBeginTime - currentTime) : 0.f;

		const static float INTERVAL = 1.f;
		world->GetTimerManager().SetTimer(_timerHandler, CallBackFunc, INTERVAL, true, delay);
	}
}