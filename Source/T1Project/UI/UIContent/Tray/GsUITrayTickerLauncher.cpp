// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Tray/GsUITrayTickerLauncher.h"
#include "UI/UIContent/Tray/GsUITrayTickerMessage.h"
#include "UMG/Public/Components/TextBlock.h"


void UGsUITrayTickerLauncher::NativeDestruct()
{
	_remainTime = 0.0f;
	_dataQueue.Empty();
	_lastText = FText::GetEmpty();

	Super::NativeDestruct();
}

void UGsUITrayTickerLauncher::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (0 < _remainTime)
	{
		_remainTime -= InDeltaTime;
		if (0 >= _remainTime)
		{
			_remainTime = 0.0f;
			OnEndPlayTime();
		}
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUITrayTickerLauncher::SetData(const FText& InText, bool bInPriority, float InPlayTime,
	EGsTickerAnimationType InAnimType)
{
	// 동일 메시지 중복하여 연속 출력 방지. 큐의 마지막에 넣었던 텍스트를 저장했다가 비교.
	if (_lastText.EqualTo(InText))
	{
		return;
	}

	_lastText = InText;

	FGsTickerMessageData data;
	data._text = InText;
	data._playTime = InPlayTime;
	data._animationType = InAnimType;

	// 우선순위가 높으면, 현재 메세지를 끊고 바로 재생
	if (bInPriority)
	{
		StartPlayInter(data);
		return;
	}

	_dataQueue.Enqueue(data);

	if (false == IsPlaying())
	{
		StartPlay();
	}
}

void UGsUITrayTickerLauncher::StartPlay()
{
	FGsTickerMessageData data;
	if (false == _dataQueue.Dequeue(data))
	{
		Close();
		return;
	}

	StartPlayInter(data);
}

void UGsUITrayTickerLauncher::StartPlayInter(FGsTickerMessageData& InData)
{
	if (_textBlock)
	{
		_textBlock->SetText(InData._text);
	}

	_remainTime = InData._playTime;

	// 블루프린트에 애니메이션 재생 이벤트 전달
	StartAnimation(InData._animationType);
}

void UGsUITrayTickerLauncher::OnEndPlayTime()
{
	StartPlay();
}

bool UGsUITrayTickerLauncher::IsPlaying() const
{
	return (0 < _remainTime) ? true : false;
}
