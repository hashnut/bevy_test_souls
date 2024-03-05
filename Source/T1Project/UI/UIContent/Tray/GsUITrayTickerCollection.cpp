#include "GsUITrayTickerCollection.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "T1Project.h"

void UGsUITrayTickerCollection::SetData(const FInitData& InInitData)
{
	_textName->SetText(InInitData._name);
	_textDesc->SetText(InInitData._desc);

	if (nullptr == _animationStart)
	{
		GSLOG(Warning, TEXT("nullptr == _animationStart, %s"), *GetName());
	}
	else
	{
		const float EndTime = _animationStart->GetEndTime();
		GSLOG(Log, TEXT("EndTime:%f"), EndTime);

		const UWorld* World = GetWorld();
		if (nullptr == World)
		{
			GSLOG(Error, TEXT("nullptr == World"));
		}
		else
		{
			World->GetTimerManager().SetTimer(_timerOnAnimationEnd, this, &UGsUITrayTickerCollection::OnAnimationEnded, EndTime, false);
			PlayAnimation(_animationStart);
		}
	}
}

void UGsUITrayTickerCollection::SetData(FText InTitle, FText InName, FText InDesc)
{
	_textTitle->SetText(InTitle);

	SetData({InName, InDesc});
}

void UGsUITrayTickerCollection::OnAnimationEnded()
{
	Close();
}
