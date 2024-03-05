// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGainEffectSlot.h"
#include "Components/TextBlock.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "T1Project.h"


void UGsUIGainEffectSlot::Reset()
{
	SetVisibility(ESlateVisibility::Collapsed);
	SetRenderScale(FVector2D(1.f, 1.f));

	_remainSec = 0.f;
	_fadeSec = 0.f;
	_upRemainSec = 0.f;

	_animationList.Clear();
}

void UGsUIGainEffectSlot::UpdateTick(float InDeltaTime)
{
	if (0.f < _remainSec)
	{
		_remainSec -= InDeltaTime;
		if (0.f >= _remainSec)
		{
			Reset();
			return;
		}

		_fadeSec += InDeltaTime;
		if (_fadeOutStartSec <= _fadeSec)
		{
			float ratio = (_fadeSec - _fadeOutStartSec) / (_lifeTimeSec - _fadeOutStartSec);
			ratio = 1.f - ratio;

			SetRenderOpacity(ratio);
		}

		if (0.f < _upRemainSec)
		{
			_upRemainSec -= InDeltaTime;
			if (0.f > _upRemainSec)
			{
				_currPos = _targetPos;
				SetRenderTranslation(_targetPos);
				_upRemainSec = 0.f;
			}
			else
			{
				float ratio = _upRemainSec / _upTimeSec;
				ratio = 1.f - ratio;
				FVector2D tempPos = FMath::Lerp(_currPos, _targetPos, ratio);

				SetRenderTranslation(tempPos);
			}
		}
		/*		
		GSLOG(Warning, TEXT("name: %s, y: %.2f, curr: %.2f, target: %.2f, v: %d, o: %.2f"), *GetName(), 
			RenderTransform.Translation.Y, _currPos.Y, _targetPos.Y,
			(uint8)(GetVisibility()), GetRenderOpacity());
		*/

		_animationList.Update(InDeltaTime);
	}
}

void UGsUIGainEffectSlot::SetTextValue(const FText& InValue)
{
	_textBlockValue->SetText(InValue);
}

void UGsUIGainEffectSlot::SetStartEffect(float InFirstScale, float InPosX)
{	
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRenderOpacity(1.f);

	SetRenderScale(FVector2D(InFirstScale, InFirstScale));
	_currPos = FVector2D(InPosX, 0.f);
	_targetPos = FVector2D::ZeroVector;
	SetRenderTranslation(_currPos);
	
	_remainSec = _lifeTimeSec;
	_fadeSec = 0.f;

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayShowAnimation();
	_animationList.Play(EGsUIAnimationTriggerType::TRIGGER1, this);
}

void UGsUIGainEffectSlot::SetUpEffect(float InCurrPosY, float InNextPosY)
{
	SetRenderScale(FVector2D(1.f, 1.f));
	_currPos.Y = InCurrPosY;
	_targetPos = FVector2D(_currPos.X, InNextPosY);
	SetRenderTranslation(_currPos);
	
	_upRemainSec = _upTimeSec;

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayUpAnimation();
	_animationList.Play(EGsUIAnimationTriggerType::TRIGGER2, this);
}

bool UGsUIGainEffectSlot::IsEndLifeTime() const
{
	return (GetVisibility() == ESlateVisibility::Collapsed) ? true : false;
}

void UGsUIGainEffectSlot::SetCurveAnimationTarget(EGsUIAnimationTriggerType InTriggerType, UWidget* InTarget)
{
	_animationList.SetTarget(InTriggerType, InTarget);
}
