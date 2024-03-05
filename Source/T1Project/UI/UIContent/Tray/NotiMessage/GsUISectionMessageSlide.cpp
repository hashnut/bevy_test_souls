#include "GsUISectionMessageSlide.h"

#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"
#include "RichTextBlock.h"
#include "Border.h"

void UGsUISectionMessageSlide::NativeConstruct()
{
	Super::NativeConstruct();

	_taskManager._taskFinished = [this]() {
		_panelSlot->SetPosition(FVector2D(_orgPos.X, _orgPos.Y));
		_richTextBlock->SetText(FText::GetEmpty());
		_border->SetVisibility(ESlateVisibility::Collapsed);
	};
}

void UGsUISectionMessageSlide::OnInitWork(TWeakPtr<FGsTickerMessageDataEx> data) //const FGsTickerMessageDataEx* work)
{
	static float animFadeInLength = _animationFade != nullptr ? _animationFade->GetEndTime() : 0.3f;
	static float animFadeOutLength = animFadeInLength - 0.1f;
	static float animLength = animFadeInLength + animFadeOutLength;

	static FString nameStartDelayTask = TEXT("SectionDelay");
	static FString nameFadeinTask	= TEXT("SectionFadeIn");
	static FString nameInitTask		= TEXT("SectionInit"); // 티커 slide를 위한 slot 사이즈가  update 이후 갱신되므로, 삽입
	static FString nameSlideTask	= TEXT("SectionSlide");
	static FString nameFadeoutTask	= TEXT("SectionFadeOut");

	if (auto work = data.Pin())
	{
		float playTime = work->_playTime;
		float fadeInTime  = animFadeInLength;
		float fadeOutTime = animFadeOutLength;
		float delayTime   = work->_delayTime;

		auto fadeInTask = MakeShared<FGsTaskTrayAct>(nameFadeinTask, 0.1f, [this, data, playTime]() {
				if (_onStartProgress) _onStartProgress(_section, data);
				
				_animSpeed = playTime < 1 ? _animSpeed / playTime : 1.f;
				StartAnimation(EGsTickerSectionAnimationType::FADE_IN); //alpha

				if(auto messageData = data.Pin())
				{
					_border->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					_panelSlot->SetPosition(FVector2D(_orgPos.X, _orgPos.Y));
					_richTextBlock->SetText(messageData->_text); 
				}
			});

		auto slideInitTask = MakeShared<FGsTaskTrayAct>(nameInitTask, fadeInTime, [this]() {
				
				float width = (_richTextBlock->GetDesiredSize().X - _maxRange) * 0.5f;
				float xPos = (width <= 0) ? 0.f : width;

				_panelSlot->SetPosition(FVector2D(_orgPos.X + xPos, _orgPos.Y));
			});

		auto slideTask = MakeShared<FGsTaskTrayActSlide>(nameSlideTask, playTime - animLength, [this]() {
				
				StartAnimation(EGsTickerSectionAnimationType::NONE);
			}, nullptr, MakeWeakObjectPtr(this));

		auto fadeOutTask = MakeShared<FGsTaskTrayAct>(nameFadeoutTask, fadeOutTime, [this, playTime]() {

				_animSpeed = playTime < 1 ? _animSpeed / playTime : 1.f;
				StartAnimation(EGsTickerSectionAnimationType::FADE_OUT);

			}, [this, data]() { 
				
				_workQueue.Remove(data);
				if (_onCompleteProgress) _onCompleteProgress(_section, data);
			});

		if (delayTime > 0.f)
		{
			_taskManager.pushTask(MakeShared<FGsTaskTrayAct>(nameStartDelayTask, delayTime));
		}
		
		_taskManager.pushTask(fadeInTask);
		_taskManager.pushTask(slideInitTask);
		_taskManager.pushTask(slideTask);
		_taskManager.pushTask(fadeOutTask);
	}
}

void  UGsUISectionMessageSlide::InitSection()
{
	if (_maxRange > 0)
	{
		Super::InitSection();
	}
}

void UGsUISectionMessageSlide::InitSection(float maxRange, UCanvasPanelSlot* panelSlot)
{
	_maxRange = maxRange;
	_panelSlot = panelSlot;

	_orgPos = _panelSlot->GetPosition();

	Super::InitSection(maxRange, panelSlot);
}
/// <summary>
/// Slide Task
/// </summary>
void UGsUISectionMessageSlide::FGsTaskTrayActSlide::onStart()
{
	Super::onStart();

	_speed = (_widget->_maxRange * 0.5f) / (_widget.IsValid() ? _widget->_slideAnimSpeed * _widget->_animSpeed : 1.f);
	_xMoveLength = _widget.IsValid() ? (_widget->_richTextBlock->GetDesiredSize().X - _widget->_maxRange) : 0.f;
	_animEndTime = _deltaTime = _xMoveLength < 0 ? _deltaTime : _xMoveLength / _speed;
	_orgPos = _widget.IsValid() ? _widget->_panelSlot->GetPosition() : FVector2D::ZeroVector;
}

void UGsUISectionMessageSlide::FGsTaskTrayActSlide::update(float delta)
{
	Super::update(delta);

	if (_widget.IsValid() && _xMoveLength > 0)
	{
		_widget->_panelSlot->SetPosition(FVector2D(_orgPos.X + (_deltaTime - _animEndTime) * _speed, _orgPos.Y));
	}
}

