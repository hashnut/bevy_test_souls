// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageRolling.h"
#include "Border.h"
#include "RichTextBlock.h"

void UGsUISectionMessageRolling::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_fadeRollingText->SetTextStyleSet(TextStyleSet);
	if (bOverrideDefaultStyle)
		_fadeRollingText->SetDefaultTextStyle(DefaultTextStyleOverride);
}

void UGsUISectionMessageRolling::NativeConstruct()
{
	Super::NativeConstruct();

	_addSpeedByRolling = 1.f;

	if (_fadeBorder)
	{
		_fadeBorder->SetVisibility(ESlateVisibility::Collapsed);
	}

	_fadeRollingText->SetText(FText::GetEmpty());

	_rollingTaskManager._taskFinished = [this]() {

		_fadeRollingText->SetText(FText::GetEmpty());
		_fadeBorder->SetVisibility(ESlateVisibility::Collapsed);
		_rollingData = nullptr;
	};

	_taskManager._taskFinished = [this]() {

		_richTextBlock->SetText(FText::GetEmpty());
		_border->SetVisibility(ESlateVisibility::Collapsed);
		_lastData = nullptr;
	};
}

void UGsUISectionMessageRolling::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_initSection)
	{
		_rollingTaskManager.doTasks(InDeltaTime);
	}
}

void UGsUISectionMessageRolling::OnInitWork(TWeakPtr<FGsTickerMessageDataEx> data)
{
	static float animFadeInLength = _animationFade != nullptr ? _animationFade->GetEndTime() : 0.3f;
	static float animFadeOutLength = animFadeInLength * 0.5f;
	static float animRollingLength = _rollingFadeOut != nullptr ? _rollingFadeOut->GetEndTime() : 0.3f;
	static float animLength = animFadeInLength + animFadeOutLength;

	static FString nameStartDelayTask = TEXT("SectionDelay");
	static FString nameFadeinTask	= TEXT("SectionFadeIn");
	static FString nameStayTask		= TEXT("SectionStay");
	static FString nameFadeoutTask	= TEXT("SectionFadeOut");
	static FString nameFadeRollTask = TEXT("SectionFadeRoll");

	if (auto work = data.Pin())
	{
		auto rollingData = _lastData;

		_workCount++;
		_lastData = data;

		float playTime = work->_playTime;
		float delayTime = work->_delayTime;
		float fadeInTime = animFadeInLength ;
		float fadeOutTime = animFadeOutLength;
		float fadeRollingTime = animRollingLength;
		float playRollingTime = rollingData.IsValid() ? playTime : playTime + fadeOutTime;

		if (IsWorkOverroad())
		{
			playTime *= 0.5f;
			playRollingTime *= 0.5f;
		}

		auto fadeInTask = MakeShared<FGsTaskTrayAct>(nameFadeinTask, fadeInTime, [this, data, playTime]() {

				if (_onStartProgress) _onStartProgress(_section, data);

				if (auto messageData = data.Pin())
				{
					_border->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					_richTextBlock->SetText(messageData->_text);

					_animSpeed = playTime < 1 ? 1.f / playTime : 1.f;
					StartAnimation(EGsTickerSectionAnimationType::FADE_IN);
				}
			});

		auto stayTask = MakeShared<FGsTaskTrayActCheckStay>(nameStayTask, playTime - animLength, [this, data]() {
	
				StartAnimation(EGsTickerSectionAnimationType::NONE);

			}, nullptr, [this, data, rollingData](float delta) {
				
				return IsWorkOverroad() && _lastData != data && !rollingData.IsValid();
			});

		auto fadeOutTask = MakeShared<FGsTaskTrayAct>(nameFadeoutTask, fadeOutTime, [this, data, playTime]() {

				_animSpeed = playTime < 1 ? 1.f / playTime : 1.f;
				StartAnimation(EGsTickerSectionAnimationType::FADE_OUT);

			}, [this, data]() {

				if (_lastData == data)
				{
					_workCount--;
					_workQueue.Remove(data);
					if (_onCompleteProgress) _onCompleteProgress(_section, data);
				}
			});
		
		auto fadeRollTask = MakeShared<FGsTaskTrayAct>(nameFadeRollTask, playRollingTime, [this, rollingData, fadeRollingTime, playRollingTime]() {

				if (auto messageData = rollingData.Pin())
				{	
					_fadeBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					_fadeRollingText->SetText(messageData->_text);

					_isRollingFadeOut = true;
					_rollingSpeed = fadeRollingTime / playRollingTime;

					StartAnimation(EGsTickerSectionAnimationType::ROLLING);
				}
			}, [this, rollingData]() {

				_workCount--;
				_workQueue.Remove(rollingData);
			
				_isRollingFadeOut = false;
				if (_onCompleteProgress) _onCompleteProgress(_section, rollingData);
			});

		if (IsWorkOverroad() && rollingData.IsValid())
		{
			_rollingTaskManager.pushTask(fadeRollTask); //새 메세지가 들어왔을 때 이전 메세지가 있으면 이전 메세지를 롤링
		}

		if (delayTime > 0.f)
		{
			_taskManager.pushTask(MakeShared<FGsTaskTrayAct>(nameStartDelayTask, delayTime));
		}

		//fadein ani가 실행되었으면 fadeout이 되어야 한다.
		_taskManager.pushTask(fadeInTask);
		_taskManager.pushTask(stayTask);
		_taskManager.pushTask(fadeOutTask);
	}
}

void UGsUISectionMessageRolling::OnClearWork()
{
	Super::OnClearWork();

	_rollingTaskManager.clearTask(true);

	_workCount = 0;
	_lastData = nullptr;

	_fadeBorder->SetVisibility(ESlateVisibility::Collapsed);
	_border->SetVisibility(ESlateVisibility::Collapsed);
	_fadeRollingText->SetText(FText::GetEmpty());
	_richTextBlock->SetText(FText::GetEmpty());
}

bool UGsUISectionMessageRolling::IsWorkOverroad()
{
	return _workCount > 1 ? true : false;  
}

void UGsUISectionMessageRolling::FinishCurrentTask(FString taskName)
{
	if (auto task = _taskManager.runTask().Pin())
	{
		if(task->_taskName == taskName)
			task->finish();
	}
}

void UGsUISectionMessageRolling::InitSection()
{
	Super::InitSection();

	_isRollingFadeOut = false;
}
