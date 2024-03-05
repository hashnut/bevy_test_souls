// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageStatic.h"
#include "Border.h"
#include "RichTextBlock.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

void UGsUISectionMessageStatic::OnInitWork(TWeakPtr<FGsTickerMessageDataEx> data) 
{
	static float animFadeInLength = _animationFade != nullptr ? _animationFade->GetEndTime() : 0.3f;
	static float animFadeOutLength = animFadeInLength - 0.1f;
	static float animLength = animFadeInLength + animFadeOutLength;

	static FString nameFadeinTask	= TEXT("SectionFadeIn");
	static FString nameStayTask		= TEXT("SectionStay");
	static FString nameFadeoutTask	= TEXT("SectionFadeOut");

	if (auto work = data.Pin())
	{
		float playTime    = work->_playTime;
		float fadeInTime  = animFadeInLength;
		float fadeOutTime = animFadeOutLength;
		
		auto fadeInTask = MakeShared<FGsTaskTrayAct>(nameFadeinTask, fadeInTime, [this, data, playTime]() {
				
				if (_onStartProgress) _onStartProgress(_section, data);

				_animSpeed = playTime < 1 ? _animSpeed / playTime : 1.f;
				StartAnimation(EGsTickerSectionAnimationType::FADE_IN);
				if (auto messageData = data.Pin())
				{
					_border->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					_richTextBlock->SetText(messageData->_text);

					if (_useBGColor && messageData->_type != NoticeType::MAX)
					{
						_bgColor = GetBackColor(messageData->_type);
					}
				}
			});

		auto stayTask = MakeShared<FGsTaskTrayAct>(nameStayTask, playTime - animLength, [this]() {

				StartAnimation(EGsTickerSectionAnimationType::NONE);
			});

		auto fadeOutTask = MakeShared<FGsTaskTrayAct>(nameFadeoutTask, fadeOutTime, [this, playTime]() {

				_animSpeed = playTime < 1 ? _animSpeed / playTime : 1.f;
				StartAnimation(EGsTickerSectionAnimationType::FADE_OUT);

			}, [this, data]() {
				
				_bgColor = _backBGColor;
				_workQueue.Remove(data);
				if (_onCompleteProgress) _onCompleteProgress(_section, data);
			});

		_taskManager.pushTask(fadeInTask);
		_taskManager.pushTask(stayTask);
		_taskManager.pushTask(fadeOutTask);
	}
}

void UGsUISectionMessageStatic::NativeConstruct()
{
	Super::NativeConstruct();

	_backBGColor = _border->ContentColorAndOpacity;
}

void UGsUISectionMessageStatic::NativeDestruct()
{
	if (_useBGColor)
	{
		_border->ContentColorAndOpacity = _backBGColor;
	}
	Super::NativeDestruct();
}

FLinearColor UGsUISectionMessageStatic::GetBackColor(NoticeType type)
{
	if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
	{
		static TMap<NoticeType, FLinearColor> colors = {
			TPair<NoticeType, FLinearColor>{NoticeType::Urgent, globalData->_bgColorUrgent },
			TPair<NoticeType, FLinearColor>{NoticeType::General, globalData->_bgColorGeneral },
			TPair<NoticeType, FLinearColor>{NoticeType::Event, globalData->_bgColorEvent },
			TPair<NoticeType, FLinearColor>{NoticeType::Immediate, globalData->_bgColorImmediate },
			TPair<NoticeType, FLinearColor>{NoticeType::Special, globalData->_bgColorSpecial }
		};

		if (auto iter = colors.Find(type))
			 return colors[type];
		else return _backBGColor;
	}
	return _backBGColor;
}
